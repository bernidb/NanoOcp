/*
  ==============================================================================

    Ocp1Message.cpp
    Created: 31 Mar 2023 4:50:02pm
    Author:  bernardoe

  ==============================================================================
*/

#include "Ocp1Message.h"


namespace NanoOcp1
{

std::uint8_t DataToUint8(const std::vector<std::uint8_t>& parameterData, bool* pOk)
{
    std::uint8_t ret(0);
    bool ok = parameterData.size() > 0;
    if (ok)
    {
        ret = parameterData[0];
    }

    if (pOk != nullptr)
    {
        *pOk = ok;
    }

    return ret;
}

std::vector<std::uint8_t> DataFromUint8(std::uint8_t value)
{
    std::vector<std::uint8_t> ret;
    ret.push_back(value);

    return ret;
}


std::uint16_t DataToUint16(const std::vector<std::uint8_t>& parameterData, bool* pOk)
{
    std::uint16_t ret(0);
    bool ok = parameterData.size() > 0;
    if (ok)
    {
        for (size_t i = 0; (i < parameterData.size()) && (i < sizeof(std::uint16_t)); i++)
        {
            ret = (ret << 8) + parameterData[i];
        }
    }

    if (pOk != nullptr)
    {
        *pOk = ok;
    }

    return ret;
}

std::vector<std::uint8_t> DataFromUint16(std::uint16_t value)
{
    std::vector<std::uint8_t> ret;
    ret.push_back(static_cast<std::uint8_t>(value >> 8));
    ret.push_back(static_cast<std::uint8_t>(value));

    return ret;
}

std::uint32_t DataToUint32(const std::vector<std::uint8_t>& /*parameterData*/, bool* /*pOk*/)
{
    std::uint32_t ret(0);

    // TODO

    return ret;
}

std::vector<std::uint8_t> DataFromUint32(std::uint32_t /*value*/)
{
    std::vector<std::uint8_t> ret;

    // TODO

    return ret;
}

juce::String DataToString(const std::vector<std::uint8_t>& parameterData, bool* pOk)
{
    juce::String ret(0);

    bool ok = parameterData.size() >= 2; // At least 2 bytes for the string length
    if (ok)
    {
        ret.preallocateBytes(parameterData.size() - 1);
        for (size_t i = 2; i < parameterData.size(); i++)
        {
            ret += parameterData[i];
        }
    }
    
    if (pOk != nullptr)
    {
        *pOk = ok;
    }

    return ret;
}

std::vector<std::uint8_t> DataFromString(juce::String string)
{
    std::vector<std::uint8_t> ret;

    const char* pStringData = string.toRawUTF8();
    int stringLength = string.length();
    
    ret.reserve(stringLength + 2);
    ret.push_back(static_cast<std::uint8_t>(stringLength >> 8));
    ret.push_back(static_cast<std::uint8_t>(stringLength));
    for (int i = 0; i < stringLength; i++)
    {
        ret.push_back(pStringData[i]);
    }

    return ret;
}

std::float_t DataToFloat(const std::vector<std::uint8_t>& /*parameterData*/, bool* /*pOk*/)
{
    std::float_t ret(0);

    // TODO

    return ret;
}

std::vector<std::uint8_t> DataFromFloat(std::float_t /*value*/)
{
    std::vector<std::uint8_t> ret;

    // TODO

    return ret;
}


//==============================================================================
// Class Ocp1Header
//==============================================================================

std::vector<std::uint8_t> Ocp1Header::GetSerializedData() const
{
    std::vector<std::uint8_t> serializedData;

    serializedData.push_back(m_syncVal);
    serializedData.push_back(static_cast<std::uint8_t>(m_protoVers >> 8));
    serializedData.push_back(static_cast<std::uint8_t>(m_protoVers));
    serializedData.push_back(static_cast<std::uint8_t>(m_msgSize >> 24));
    serializedData.push_back(static_cast<std::uint8_t>(m_msgSize >> 16));
    serializedData.push_back(static_cast<std::uint8_t>(m_msgSize >> 8));
    serializedData.push_back(static_cast<std::uint8_t>(m_msgSize));
    serializedData.push_back(m_msgType);
    serializedData.push_back(static_cast<std::uint8_t>(m_msgCnt >> 8));
    serializedData.push_back(static_cast<std::uint8_t>(m_msgCnt));

    return serializedData;
};

std::uint32_t Ocp1Header::CalculateMessageSize(std::uint8_t msgType, size_t parameterDataLength)
{
    std::uint32_t ret(0);

    switch (msgType)
    {
        case Ocp1Message::Command:
            // TODO
            break;
        case Ocp1Message::CommandResponseRequired:
            ret = static_cast<std::uint32_t>(26 + parameterDataLength);
            break;
        case Ocp1Message::Notification:
            ret = static_cast<std::uint32_t>(37 + parameterDataLength);
            break;
        case Ocp1Message::Response:
            ret = static_cast<std::uint32_t>(19);
            break;
        case Ocp1Message::KeepAlive:
            ret = static_cast<std::uint32_t>(11);
            break;
        default:
            break;
    }

    return ret;
}


//==============================================================================
// Class Ocp1Message
//==============================================================================

std::uint32_t Ocp1Message::m_nextHandle = 1;

Ocp1Message* Ocp1Message::UnmarshalOcp1Message(const juce::MemoryBlock& receivedData)
{
    // Not enough data to fit even a Ocp1Header.
    if (receivedData.getSize() <= 10)
        return nullptr;

    // Message does not start with the sync byte.
    std::uint8_t syncVal = receivedData[0];
    if (syncVal != 0x3b)
        return nullptr;

    // Protocol version is expected to be 1.
    std::uint16_t protoVers = ((receivedData[1] << 8) + receivedData[2]);
    if (protoVers != 1)
        return nullptr;

    // Message has unexpected size.
    std::uint32_t msgSize = ((receivedData[3] << 24) + (receivedData[4] << 16) + (receivedData[5] << 8) + receivedData[6]);
    if (receivedData.getSize() != (msgSize + 1))
        return nullptr;

    // Message type outside expected range.
    std::uint8_t msgType = receivedData[7];
    if (msgType > KeepAlive)
        return nullptr;

    // At least one message expected. TODO: how to handle more than one?
    std::uint16_t msgCnt = ((receivedData[8] << 8) + receivedData[9]);
    if (msgCnt != 1)
        return nullptr;

    switch (msgType)
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


//==============================================================================
// Class Ocp1CommandResponseRequired
//==============================================================================

std::vector<std::uint8_t> Ocp1CommandResponseRequired::GetSerializedData()
{
    std::vector<std::uint8_t> serializedData = m_header.GetSerializedData();

    std::uint32_t commandSize(m_header.GetMessageSize() - 9); // Message size minus the header
    serializedData.push_back(static_cast<std::uint8_t>(commandSize >> 24));
    serializedData.push_back(static_cast<std::uint8_t>(commandSize >> 16));
    serializedData.push_back(static_cast<std::uint8_t>(commandSize >> 8));
    serializedData.push_back(static_cast<std::uint8_t>(commandSize));
    serializedData.push_back(static_cast<std::uint8_t>(m_handle >> 24));
    serializedData.push_back(static_cast<std::uint8_t>(m_handle >> 16));
    serializedData.push_back(static_cast<std::uint8_t>(m_handle >> 8));
    serializedData.push_back(static_cast<std::uint8_t>(m_handle));
    serializedData.push_back(static_cast<std::uint8_t>(m_targetOno >> 24));
    serializedData.push_back(static_cast<std::uint8_t>(m_targetOno >> 16));
    serializedData.push_back(static_cast<std::uint8_t>(m_targetOno >> 8));
    serializedData.push_back(static_cast<std::uint8_t>(m_targetOno));
    serializedData.push_back(static_cast<std::uint8_t>(m_methodDefLevel >> 8));
    serializedData.push_back(static_cast<std::uint8_t>(m_methodDefLevel));
    serializedData.push_back(static_cast<std::uint8_t>(m_methodIndex >> 8));
    serializedData.push_back(static_cast<std::uint8_t>(m_methodIndex));
    serializedData.push_back(static_cast<std::uint8_t>(1)); // ParameterCount
    for (size_t i = 0; i < m_parameterData.size(); i++)
    {
        serializedData.push_back(m_parameterData[i]);
    }

    return serializedData;
};



//==============================================================================
// Class Ocp1Notification
//==============================================================================

std::vector<std::uint8_t> Ocp1Notification::GetSerializedData()
{
    std::vector<std::uint8_t> serializedData = m_header.GetSerializedData();

    // TODO

    return serializedData;
};



//==============================================================================
// Class Ocp1KeepAlive
//==============================================================================

std::vector<std::uint8_t> Ocp1KeepAlive::GetSerializedData()
{
    std::vector<std::uint8_t> serializedData = m_header.GetSerializedData();

    serializedData.push_back(static_cast<std::uint8_t>(m_heartBeat >> 8));
    serializedData.push_back(static_cast<std::uint8_t>(m_heartBeat));

    return serializedData;
};

}