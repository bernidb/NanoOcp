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

std::float_t DataToFloat(const std::vector<std::uint8_t>& parameterData, bool* pOk)
{
    std::float_t ret(0);

    bool ok = (parameterData.size() == 4); // 4 bytes expected.
    ok = (sizeof(int) == sizeof(std::float_t)); // Required for pointer cast to work
    if (ok)
    {
        int intValue = ((parameterData[0] << 24) + (parameterData[1] << 16) + (parameterData[2] << 8) + parameterData[3]);
        ret = *(std::float_t*)&intValue;
    }

    if (pOk != nullptr)
    {
        *pOk = ok;
    }

    return ret;
}

std::vector<std::uint8_t> DataFromFloat(std::float_t floatValue)
{
    std::vector<std::uint8_t> ret;
    ret.reserve(4);

    jassert(sizeof(std::uint32_t) == sizeof(std::float_t)); // Required for pointer cast to work
    std::uint32_t intValue = *(std::uint32_t*)&floatValue;

    ret.push_back(static_cast<std::uint8_t>(intValue >> 24));
    ret.push_back(static_cast<std::uint8_t>(intValue >> 16));
    ret.push_back(static_cast<std::uint8_t>(intValue >> 8));
    ret.push_back(static_cast<std::uint8_t>(intValue));

    return ret;
}

std::vector<std::uint8_t> DataFromOnoForSubscription(std::uint32_t ono)
{
    std::vector<std::uint8_t> ret;
    ret.reserve(25);

    ret.push_back(static_cast<std::uint8_t>(ono >> 24)); // Emitter ONo
    ret.push_back(static_cast<std::uint8_t>(ono >> 16));
    ret.push_back(static_cast<std::uint8_t>(ono >> 8));
    ret.push_back(static_cast<std::uint8_t>(ono));
    ret.push_back(static_cast<std::uint8_t>(0x00)); // EventID def level: OcaRoot
    ret.push_back(static_cast<std::uint8_t>(0x01)); 
    ret.push_back(static_cast<std::uint8_t>(0x00)); // EventID idx: PropertyChanged
    ret.push_back(static_cast<std::uint8_t>(0x01)); 

    ret.push_back(static_cast<std::uint8_t>(ono >> 24)); // Subscriber ONo
    ret.push_back(static_cast<std::uint8_t>(ono >> 16));
    ret.push_back(static_cast<std::uint8_t>(ono >> 8));
    ret.push_back(static_cast<std::uint8_t>(ono));
    ret.push_back(static_cast<std::uint8_t>(0x00)); // Method def level: OcaSubscriptionManager
    ret.push_back(static_cast<std::uint8_t>(0x03)); 
    ret.push_back(static_cast<std::uint8_t>(0x00)); // Method idx: AddSubscription
    ret.push_back(static_cast<std::uint8_t>(0x01)); 

    ret.push_back(static_cast<std::uint8_t>(0x00)); // Context size: 0
    ret.push_back(static_cast<std::uint8_t>(0x00));
    ret.push_back(static_cast<std::uint8_t>(0x01)); // Delivery mode: Reliable

    ret.push_back(static_cast<std::uint8_t>(0x00)); // Destination info length: always 4
    ret.push_back(static_cast<std::uint8_t>(0x04));
    ret.push_back(static_cast<std::uint8_t>(0x00)); // Destination info (4 empty bytes)
    ret.push_back(static_cast<std::uint8_t>(0x00));
    ret.push_back(static_cast<std::uint8_t>(0x00));
    ret.push_back(static_cast<std::uint8_t>(0x00));

    return ret;
}

juce::String StatusToString(std::uint8_t status)
{
    juce::String result;

    switch (status)
    {
        case 0: // OCASTATUS_OK:
            result = juce::String("OK");
            break;
        case 1: // OCASTATUS_PROTOCOL_VERSION_ERROR:
            result = juce::String("ProtocolVersionError");
            break;
        case 2: // OCASTATUS_DEVICE_ERROR:
            result = juce::String("DeviceError");
            break;
        case 3: // OCASTATUS_LOCKED:
            result = juce::String("Locked");
            break;
        case 4: // OCASTATUS_BAD_FORMAT:
            result = juce::String("BadFormat");
            break;
        case 5: // OCASTATUS_BAD_ONO:
            result = juce::String("BadONo");
            break;
        case 6: // OCASTATUS_PARAMETER_ERROR:
            result = juce::String("ParameterError");
            break;
        case 7: // OCASTATUS_PARAMETER_OUT_OF_RANGE:
            result = juce::String("ParameterOutOfRange");
            break;
        case 8: // OCASTATUS_NOT_IMPLEMENTED:
            result = juce::String("NotImplemented");
            break;
        case 9: // OCASTATUS_INVALID_REQUEST:
            result = juce::String("InvalidRequest");
            break;
        case 10: // OCASTATUS_PROCESSING_FAILED:
            result = juce::String("ProcessingFailed");
            break;
        case 11: // OCASTATUS_BAD_METHOD:
            result = juce::String("BadMethod");
            break;
        case 12: // OCASTATUS_PARTIALLY_SUCCEEDED:
            result = juce::String("PartiallySucceeded");
            break;
        case 13: // OCASTATUS_TIMEOUT:
            result = juce::String("Timeout");
            break;
        case 14: // OCASTATUS_BUFFER_OVERFLOW:
            result = juce::String("BufferOverflow");
            break;
        case 15: // OCASTATUS_PERMISSION_DENIED:
            result = juce::String("PermissionDenied");
            break;
        default:
            result = juce::String(status);
            break;
    }

    return result;
}

std::uint32_t GetONo(std::uint32_t type, std::uint32_t record, std::uint32_t channel, std::uint32_t boxAndObjectNumber)
{
    return (std::uint32_t((type) & 0xF) << 28)
        | (std::uint32_t((record) & 0xFF) << 20)
        | (std::uint32_t((channel) & 0x1F) << 15)
        | (std::uint32_t(boxAndObjectNumber) & 0x7FFF);
}

std::uint32_t GetONoTy2(std::uint32_t type, std::uint32_t record, std::uint32_t channel, std::uint32_t boxNumber, std::uint32_t objectNumber)
{
    return (std::uint32_t((type) & 0xF) << 28)
        | (std::uint32_t((record) & 0xFF) << 20)
        | (std::uint32_t((channel) & 0xFF) << 12)
        | (std::uint32_t((boxNumber) & 0x1F) << 7)
        | (std::uint32_t((objectNumber) & 0x7F));
}


//==============================================================================
// Class Ocp1Header
//==============================================================================

Ocp1Header::Ocp1Header(const juce::MemoryBlock& memoryBlock)
    :   m_syncVal(static_cast<std::uint8_t>(0)),
        m_protoVers(static_cast<std::uint16_t>(0)),
        m_msgType(static_cast<std::uint8_t>(0)),
        m_msgCnt(static_cast<std::uint16_t>(0)),
        m_msgSize(static_cast<std::uint32_t>(0))
{
    jassert(memoryBlock.getSize() >= 10); // Not enough data to fit even a Ocp1Header.
    if (memoryBlock.getSize() >= 10)
    {
        m_syncVal = memoryBlock[0];
        jassert(m_syncVal == 0x3b); // Message does not start with the sync byte.

        m_protoVers = ((memoryBlock[1] << 8) + memoryBlock[2]);
        jassert(m_protoVers == 1); // Protocol version is expected to be 1.
        
        m_msgSize = ((memoryBlock[3] << 24) + (memoryBlock[4] << 16) + (memoryBlock[5] << 8) + memoryBlock[6]);
        jassert(m_msgSize >= Ocp1HeaderSize); // Message has unexpected size.

        m_msgType = memoryBlock[7];
        jassert(m_msgType <= Ocp1Message::KeepAlive); // Message type outside expected range.

        m_msgCnt = ((memoryBlock[8] << 8) + memoryBlock[9]);
        jassert(m_msgCnt > 0); // At least one message expected. 
    }
}

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
        case Ocp1Message::CommandResponseRequired:
            ret = static_cast<std::uint32_t>(26 + parameterDataLength);
            break;
        case Ocp1Message::Notification:
            ret = static_cast<std::uint32_t>(37 + parameterDataLength);
            break;
        case Ocp1Message::Response:
            ret = static_cast<std::uint32_t>(19 + parameterDataLength);
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

std::unique_ptr<Ocp1Message> Ocp1Message::UnmarshalOcp1Message(const juce::MemoryBlock& receivedData)
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
                std::uint32_t targetOno = (((receivedData[14] << 24) & 0xff000000) + 
                                           ((receivedData[15] << 16) & 0x00ff0000) + 
                                           ((receivedData[16] << 8)  & 0x0000ff00) + receivedData[17]);
                if (targetOno == 0)
                    return nullptr;

                // Method DefinitionLevel expected to be 3 (OcaSubscriptionManager)
                std::uint16_t methodDefLevel = ((receivedData[18] << 8) + receivedData[19]);
                if (methodDefLevel < 1)
                    return nullptr;

                // Method index expected to be 1 (AddSubscription)
                std::uint16_t methodIdx = ((receivedData[20] << 8) + receivedData[21]);
                if (methodIdx < 1)
                    return nullptr;

                // At least one parameter expected.
                std::uint8_t paramCount = receivedData[22];
                if (paramCount < 1)
                    return nullptr;

                std::uint16_t contextSize = ((receivedData[23] << 8) + receivedData[24]);

                // Not a valid object number.
                std::uint32_t emitterOno = (((receivedData[25 + contextSize] << 24) & 0xff000000) +
                                            ((receivedData[26 + contextSize] << 16) & 0x00ff0000) +
                                            ((receivedData[27 + contextSize] << 8)  & 0x0000ff00) + receivedData[28 + contextSize]);
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

                return std::make_unique<Ocp1Notification>(emitterOno, propDefLevel, propIdx, parameterData);
            }

        case Response:
            {
                std::uint32_t responseSize = ((receivedData[10] << 24) + (receivedData[11] << 16) + (receivedData[12] << 8) + receivedData[13]);
                std::uint32_t parameterDataLength = responseSize - 10;
                if (responseSize < 10)
                    return nullptr;

                // Not a valid handle.
                std::uint32_t handle = ((receivedData[14] << 24) + (receivedData[15] << 16) + (receivedData[16] << 8) + receivedData[17]);
                if (handle == 0)
                    return nullptr;

                std::uint8_t status = receivedData[18];
                std::uint8_t paramCount = receivedData[19];

                std::vector<std::uint8_t> parameterData;
                if (parameterDataLength > 0)
                {
                    parameterData.reserve(parameterDataLength);
                    for (std::uint32_t i = 0; i < parameterDataLength; i++)
                    {
                        parameterData.push_back(static_cast<std::uint8_t>(receivedData[20 + i]));
                    }
                }

                return std::make_unique<Ocp1Response>(handle, status, paramCount, parameterData);
            }

        case KeepAlive:
            {
                std::uint16_t heartbeat = ((receivedData[10] << 8) + receivedData[11]);

                return std::make_unique<Ocp1KeepAlive>(heartbeat);
            }

        case Command:
        case CommandResponseRequired:
            {
                // TODO
                return nullptr;
            }
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
    serializedData.push_back(static_cast<std::uint8_t>(m_paramCount));
    for (size_t i = 0; i < m_parameterData.size(); i++)
    {
        serializedData.push_back(m_parameterData[i]);
    }

    return serializedData;
};



//==============================================================================
// Class Ocp1Response
//==============================================================================

std::vector<std::uint8_t> Ocp1Response::GetSerializedData()
{
    std::vector<std::uint8_t> serializedData = m_header.GetSerializedData();

    std::uint32_t responseSize(m_header.GetMessageSize() - 9); // Message size minus the header
    serializedData.push_back(static_cast<std::uint8_t>(responseSize >> 24));
    serializedData.push_back(static_cast<std::uint8_t>(responseSize >> 16));
    serializedData.push_back(static_cast<std::uint8_t>(responseSize >> 8));
    serializedData.push_back(static_cast<std::uint8_t>(responseSize));
    serializedData.push_back(static_cast<std::uint8_t>(m_handle >> 24));
    serializedData.push_back(static_cast<std::uint8_t>(m_handle >> 16));
    serializedData.push_back(static_cast<std::uint8_t>(m_handle >> 8));
    serializedData.push_back(static_cast<std::uint8_t>(m_handle));
    serializedData.push_back(static_cast<std::uint8_t>(m_status));
    serializedData.push_back(static_cast<std::uint8_t>(m_paramCount));
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

    std::uint32_t notificationSize(m_header.GetMessageSize() - 9); // Message size minus the header
    serializedData.push_back(static_cast<std::uint8_t>(notificationSize >> 24));
    serializedData.push_back(static_cast<std::uint8_t>(notificationSize >> 16));
    serializedData.push_back(static_cast<std::uint8_t>(notificationSize >> 8));
    serializedData.push_back(static_cast<std::uint8_t>(notificationSize));
    serializedData.push_back(static_cast<std::uint8_t>(m_emitterOno >> 24)); // TargetOno
    serializedData.push_back(static_cast<std::uint8_t>(m_emitterOno >> 16));
    serializedData.push_back(static_cast<std::uint8_t>(m_emitterOno >> 8));
    serializedData.push_back(static_cast<std::uint8_t>(m_emitterOno));
    std::uint16_t methodDefLevel = 3; // OcaSubscriptionManager
    serializedData.push_back(static_cast<std::uint8_t>(methodDefLevel >> 8));
    serializedData.push_back(static_cast<std::uint8_t>(methodDefLevel));
    std::uint16_t methodIdx = 1; // AddSubscription
    serializedData.push_back(static_cast<std::uint8_t>(methodIdx >> 8));
    serializedData.push_back(static_cast<std::uint8_t>(methodIdx));
    std::uint16_t paramCount = 2;
    serializedData.push_back(static_cast<std::uint8_t>(paramCount));
    std::uint16_t contextLength = 0;
    serializedData.push_back(static_cast<std::uint8_t>(contextLength >> 8));
    serializedData.push_back(static_cast<std::uint8_t>(contextLength));
    serializedData.push_back(static_cast<std::uint8_t>(m_emitterOno >> 24)); // EmitterOno
    serializedData.push_back(static_cast<std::uint8_t>(m_emitterOno >> 16));
    serializedData.push_back(static_cast<std::uint8_t>(m_emitterOno >> 8));
    serializedData.push_back(static_cast<std::uint8_t>(m_emitterOno));
    std::uint16_t eventDefLevel = 1; // OcaRoot level
    serializedData.push_back(static_cast<std::uint8_t>(eventDefLevel >> 8));
    serializedData.push_back(static_cast<std::uint8_t>(eventDefLevel));
    std::uint16_t eventIdx = 1; // PropertyChanged event
    serializedData.push_back(static_cast<std::uint8_t>(eventIdx >> 8));
    serializedData.push_back(static_cast<std::uint8_t>(eventIdx));
    serializedData.push_back(static_cast<std::uint8_t>(m_emitterPropertyDefLevel >> 8));
    serializedData.push_back(static_cast<std::uint8_t>(m_emitterPropertyDefLevel));
    serializedData.push_back(static_cast<std::uint8_t>(m_emitterPropertyIndex >> 8));
    serializedData.push_back(static_cast<std::uint8_t>(m_emitterPropertyIndex));
    for (size_t i = 0; i < m_parameterData.size(); i++)
    {
        serializedData.push_back(m_parameterData[i]);
    }
    serializedData.push_back(static_cast<std::uint8_t>(1)); // Ending byte

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