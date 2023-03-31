/*
  ==============================================================================

    NanoOcp1.cpp
    Created: 04 March 2022 22:10:00pm
    Author:  Christian Ahrens

  ==============================================================================
*/

#include "NanoOcp1.h"


namespace NanoOcp1
{
std::uint32_t Ocp1Message::m_nextHandle = 1;

//==============================================================================
Ocp1Message* Ocp1Message::UnmarshalOcp1Message(const juce::MemoryBlock& receivedData)
{
    Ocp1Header header;

    // Not enough data to fit even a Ocp1Header.
    if (receivedData.getSize() <= 10)
        return nullptr;

    // Message does not start with the sync byte.
    header.m_syncVal = receivedData[0];
    if (header.m_syncVal != 0x3b)
        return nullptr;

    // Protocol version is expected to be 1.
    header.m_protoVers = ((receivedData[1] << 8) + receivedData[2]);
    if (header.m_protoVers != 1)
        return nullptr;

    // Message too short.
    header.m_msgSize = ((receivedData[3] << 24) + (receivedData[4] << 16) + (receivedData[5] << 8) + receivedData[6]);
    if (receivedData.getSize() < (header.m_msgSize + 1))
        return nullptr;

    // Message type outside expected range.
    header.m_msgType = receivedData[7];
    if ((header.m_msgType < 1) || (header.m_msgType > KeepAlive))
        return nullptr;

    // At least one message expected. TODO: how to handle more than one?
    header.m_msgCnt = ((receivedData[8] << 8) + receivedData[9]);
    if (header.m_msgCnt != 1)
        return nullptr;

    switch (header.m_msgType)
    {
        case Notification:
            {           
                std::uint32_t notificationSize = ((receivedData[10] << 24) + (receivedData[11] << 16) + (receivedData[12] << 8) + receivedData[13]);
                std::uint32_t newValueSize = notificationSize - 28;
                if (newValueSize < 1)
                    return nullptr;

                // Not a valid object number.
                std::uint32_t targetOno = ((receivedData[14] << 24) + (receivedData[15] << 16) + (receivedData[16] << 8) + receivedData[17]);
                if (targetOno == 0)
                    return nullptr;

                // Method DefinitionLevel expected to be ? TODO
                std::uint16_t methodDefLevel = ((receivedData[18] << 8) + receivedData[19]);
                if (methodDefLevel < 1)
                    return nullptr;

                // Method index expected to be ? TODO
                std::uint16_t methodIdx = ((receivedData[20] << 8) + receivedData[21]);
                if (methodIdx < 1)
                    return nullptr;

                // At least one parameter expected.
                std::uint8_t paramCount = receivedData[22];
                if (paramCount < 1)
                    return nullptr;

                std::uint16_t contextSize = ((receivedData[23] << 8) + receivedData[24]);

                // Not a valid object number.
                std::uint32_t emitterOno = ((receivedData[25 + contextSize] << 24) + (receivedData[26 + contextSize] << 16) + 
                                           (receivedData[27 + contextSize] << 8) + receivedData[28 + contextSize]);
                if (emitterOno == 0)
                    return nullptr;

                // Event definiton level expected to be 1 (OcaRoot).
                std::uint16_t eventDefLevel = ((receivedData[29 + contextSize] << 8) + receivedData[30 + contextSize]);
                if (eventDefLevel != 1)
                    return nullptr;

                // Event index expected to be 1 (OCA_EVENT_PROPERTY_CHANGED).
                std::uint16_t eventIdx = ((receivedData[31 + contextSize] << 8) + receivedData[32 + contextSize]);
                if (eventIdx != 1)
                    return nullptr;

                // Property definition level expected to be > 0.
                std::uint16_t propDefLevel = ((receivedData[33 + contextSize] << 8) + receivedData[34 + contextSize]);
                if (propDefLevel == 0)
                    return nullptr;

                // Property index expected to be > 0.
                std::uint16_t propIdx = ((receivedData[35 + contextSize] << 8) + receivedData[36 + contextSize]);
                if (propIdx == 0)
                    return nullptr;

                std::vector<std::uint8_t> parameterData;
                parameterData.reserve(newValueSize);
                for (std::uint32_t i = 0; i < newValueSize; i++)
                {
                    parameterData.push_back(static_cast<std::uint8_t>(receivedData[37 + contextSize + i]));
                }

                return new Ocp1Notification(emitterOno, propDefLevel, propIdx, parameterData);
            }

        case Response:
            {
                // TODO
                return nullptr;
            }

        case KeepAlive:
            {
                std::uint16_t heartbeat = ((receivedData[10] << 8) + receivedData[11]);

                return new Ocp1KeepAlive(heartbeat);
            }

        // No messages of these types should normally be received at a controller.
        case Command:
        case CommandResponseRequired:
        default:
            return nullptr;
    }
}

std::uint8_t Ocp1Message::ValueToUint8(bool* pOk) const
{
    std::uint8_t ret(0);
    bool ok = m_parameterData.size() > 0;
    if (ok)
    {
        ret = m_parameterData[0];
    }

    if (pOk != nullptr)
    {
        *pOk = ok;
    }

    return ret;
}

bool Ocp1Message::ValueFromUint8(std::uint8_t newValue)
{
    // Update internal value data.
    m_parameterData.clear();
    m_parameterData.push_back(newValue);

    // Update message size to match new length of data
    switch (m_header.m_msgType)
    {
        case CommandResponseRequired:
            m_header.m_msgSize = 26 + 1; // 26 Bytes message overhead + 1 byte value data
            break;
        default: 
            // TODO other msgtypes
            break;
    }
 
    return true; // TODO: make it void?
}

std::uint32_t Ocp1Message::ValueToUint32(bool* pOk) const
{
    std::uint32_t ret(0);
    bool ok = m_parameterData.size() > 0;
    if (ok)
    {
        for (size_t i = 0; (i < m_parameterData.size()) && (i < sizeof(std::uint32_t)); i++)
        {
            ret = (ret << 8) + m_parameterData[i];
        }
    }

    if (pOk != nullptr)
    {
        *pOk = ok;
    }

    return ret;
}

bool Ocp1Message::ValueFromUint32(std::uint32_t newValue)
{
    // Update internal value data.
    m_parameterData.clear();
    m_parameterData.push_back(static_cast<std::uint8_t>(newValue >> 24));
    m_parameterData.push_back(static_cast<std::uint8_t>(newValue >> 16));
    m_parameterData.push_back(static_cast<std::uint8_t>(newValue >> 8));
    m_parameterData.push_back(static_cast<std::uint8_t>(newValue));

    // Update message size to match new length of data
    switch (m_header.m_msgType)
    {
        case CommandResponseRequired:
            m_header.m_msgSize = 26 + 4; // 26 Bytes message overhead + 4 byte value data
            break;
        default:
            // TODO other msgtypes
            break;
    }

    return true; // TODO: make it void?
}

juce::String Ocp1Message::ValueToString(bool* pOk) const
{
    juce::String ret;
    bool ok = m_parameterData.size() >= 2; // At least 2 bytes for the string length
    if (ok)
    {
        ret.preallocateBytes(m_parameterData.size() - 1);
        for (size_t i = 2; i < m_parameterData.size(); i++)
        {
            ret += m_parameterData[i];
        }
    }

    if (pOk != nullptr)
    {
        *pOk = ok;
    }

    return ret;
}

bool Ocp1Message::ValueFromString(juce::String newValue)
{
    const char* pStringData = newValue.toRawUTF8();
    int stringLength = newValue.length();

    // Update internal value data.
    m_parameterData.clear();
    m_parameterData.reserve(stringLength + 2);
    m_parameterData.push_back(static_cast<std::uint8_t>(stringLength >> 8));
    m_parameterData.push_back(static_cast<std::uint8_t>(stringLength));
    for (int i = 0; i < stringLength; i++)
    {
        m_parameterData.push_back(pStringData[i]);
    }

    // Update message size to match new length of data.
    switch (m_header.m_msgType)
    {
        case CommandResponseRequired:
            // 26 Bytes message overhead + 2 bytes string length + number of characters.
            m_header.m_msgSize = 26 + 2 + stringLength; 
            break;
        default:
            // TODO other msgtypes
            break;
    }

    return true; // TODO: make it void?
}



//==============================================================================
NanoOcp1Base::NanoOcp1Base(const juce::String& address, const int port)
{
	setAddress(address);
	setPort(port);
}

NanoOcp1Base::~NanoOcp1Base()
{
}

void NanoOcp1Base::setAddress(const juce::String& address)
{
	m_address = address;
}

const juce::String& NanoOcp1Base::getAddress()
{
	return m_address;
}

void NanoOcp1Base::setPort(const int port)
{
	m_port = port;
}

const int NanoOcp1Base::getPort()
{
	return m_port;
}

bool NanoOcp1Base::processReceivedData(const juce::MemoryBlock& data)
{
	if (onDataReceived)
		return onDataReceived(data);

	return false;
}

std::uint32_t NanoOcp1Base::GetDBONo(std::uint32_t type, std::uint32_t record, std::uint32_t channel, std::uint32_t boxAndObjectNumber)
{
	return (std::uint32_t((type) & 0xF) << 28) 
		| (std::uint32_t((record) & 0xFF) << 20) 
		| (std::uint32_t((channel) & 0x1F) << 15) 
		| (std::uint32_t(boxAndObjectNumber) & 0x7FFF);
}

std::uint32_t NanoOcp1Base::GetDBONo2(std::uint32_t type, std::uint32_t record, std::uint32_t channel, std::uint32_t boxNumber, std::uint32_t objectNumber)
{
	return (std::uint32_t((type) & 0xF) << 28)
		| (std::uint32_t((record) & 0xFF) << 20)
		| (std::uint32_t((channel) & 0xFF) << 12)
		| (std::uint32_t((boxNumber) & 0x1F) << 7)
		| (std::uint32_t((objectNumber) & 0x7F));
}

//==============================================================================
NanoOcp1Client::NanoOcp1Client() :
	NanoOcp1Client(juce::String(), 0)
{
}

NanoOcp1Client::NanoOcp1Client(const juce::String& address, const int port) :
	NanoOcp1Base(address, port), Ocp1Connection()
{
}

NanoOcp1Client::~NanoOcp1Client()
{
	stop();
}

bool NanoOcp1Client::start()
{
	m_started = true;

	if (connectToSocket(getAddress(), getPort(), 50))
		return true; // connection immediatly established
	else
		startTimer(500); // start trying to establish connection

	return false;
}

bool NanoOcp1Client::stop()
{
	m_started = false;

	stopTimer();

	disconnect();

	return !isConnected();
}

bool NanoOcp1Client::sendData(const juce::MemoryBlock& data)
{
	if (!isConnected())
		return false;

	return Ocp1Connection::sendMessage(data);
}

void NanoOcp1Client::connectionMade()
{
	stopTimer();

	if (onConnectionEstablished)
		onConnectionEstablished();
}

void NanoOcp1Client::connectionLost()
{
	if (onConnectionLost)
		onConnectionLost();

	if (m_started)
		startTimer(500); // start trying to reestablish connection
}

void NanoOcp1Client::messageReceived(const juce::MemoryBlock& message)
{
	processReceivedData(message);
}

void NanoOcp1Client::timerCallback()
{
	if (connectToSocket(getAddress(), getPort(), 50))
		stopTimer(); // connection established, no need to retry
}

//==============================================================================
NanoOcp1Server::NanoOcp1Server() :
	NanoOcp1Server(juce::String(), 0)
{
}

NanoOcp1Server::NanoOcp1Server(const juce::String& address, const int port) :
	NanoOcp1Base(address, port), Ocp1ConnectionServer()
{
}

NanoOcp1Server::~NanoOcp1Server()
{
	stop();
}

bool NanoOcp1Server::start()
{
	return beginWaitingForSocket(getPort(), getAddress());
}

bool NanoOcp1Server::stop()
{
	if (m_activeConnection)
	{
		m_activeConnection->disconnect();
		return !m_activeConnection->isConnected();
	}
	else
		return true;
}

bool NanoOcp1Server::sendData(const juce::MemoryBlock& data)
{
	if (!m_activeConnection)
		return false;

	return m_activeConnection->sendData(data);
}

Ocp1Connection* NanoOcp1Server::createConnectionObject()
{
	m_activeConnection = std::make_unique<NanoOcp1Client>();
	m_activeConnection->onDataReceived = this->onDataReceived;

	return m_activeConnection.get();
}


}
