/*
  ==============================================================================

    NanoOcp1.cpp
    Created: 04 March 2022 22:10:00pm
    Author:  Christian Ahrens

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

#include "Ocp1Connection.h"
#include "Ocp1ConnectionServer.h"


namespace NanoOcp1
{

class Ocp1Header
{
public:
    std::vector<std::uint8_t> GetSerializedData() 
    {
        std::vector<std::uint8_t> serializedData;

        serializedData.push_back(m_syncVal);
        serializedData.push_back(static_cast<std::uint8_t>(m_protoVers << 8));
        serializedData.push_back(static_cast<std::uint8_t>(m_protoVers));
        serializedData.push_back(static_cast<std::uint8_t>(m_msgSize << 24));
        serializedData.push_back(static_cast<std::uint8_t>(m_msgSize << 16));
        serializedData.push_back(static_cast<std::uint8_t>(m_msgSize << 8));
        serializedData.push_back(static_cast<std::uint8_t>(m_msgSize));
        serializedData.push_back(m_msgType);
        serializedData.push_back(static_cast<std::uint8_t>(m_msgCnt << 8));
        serializedData.push_back(static_cast<std::uint8_t>(m_msgCnt));

        return serializedData;
    };

    std::uint8_t                m_syncVal;      // 0x3b
    std::uint16_t               m_protoVers;    // 0x00 0x01
    std::uint32_t               m_msgSize;      // 0x00 0x00 0x00 0x1a
    std::uint8_t                m_msgType;      // 0x01 // Ocp1CndRrq
    std::uint16_t               m_msgCnt;       // 0x00 0x01
};


/**
 * Abstract representation of a general OCA Message.
 */
class Ocp1Message
{
public:
    enum MessageType
    {
        Command = 0,
	    CommandResponseRequired = 1,
	    Notification = 2,
	    Response = 3,
	    KeepAlive = 4
    };

    Ocp1Message(std::uint8_t msgType, const std::vector<std::uint8_t>& parameterData)
        : m_parameterData(parameterData)
    {
        m_header.m_syncVal = 0x3b;
        m_header.m_protoVers = static_cast<std::uint16_t>(1);
        m_header.m_msgType = msgType;
        m_header.m_msgCnt = static_cast<std::uint16_t>(1);

        // TODO: Compute m_msgSize from m_msgType and size of the parameterData.
        m_header.m_msgSize = static_cast<std::uint16_t>(1); 
    }

    /**
     * TODO
     */
    virtual std::vector<std::uint8_t> GetSerializedData() = 0;

    /**
     * TODO
     */
    juce::MemoryBlock GetMemoryBlock()
    {
        auto serializedData = GetSerializedData();
        return juce::MemoryBlock((const char*)serializedData.data(), serializedData.size());
    }

    /**
     * Factory method which creates a new Ocp1Message object based on a MemoryBlock.
     * 
     * TODO
     */
    static Ocp1Message* UnmarshalOcp1Message(const juce::MemoryBlock& receivedData);

    /**
     * TODO
     */
    std::uint8_t ValueToUint8(bool* pOk = nullptr) const;

    /**
     * TODO
     */
    bool ValueFromUint8(std::uint8_t newValue);

    /**
     * TODO
     */
    std::uint32_t ValueToUint32(bool* pOk = nullptr) const;

    /**
     * TODO
     */
    bool ValueFromUint32(std::uint32_t newValue);

    /**
     * TODO
     */
    juce::String ValueToString(bool* pOk = nullptr) const;

    /**
     * TODO
     */
    bool ValueFromString(juce::String newValue);


protected:
    Ocp1Header                  m_header;
    std::vector<std::uint8_t>   m_parameterData;
    static std::uint32_t        m_nextHandle;
};


/**
 * Representation of an OCA CommandResponseRequired message.
 */
class Ocp1CommandResponseRequired : public Ocp1Message
{
public:
    Ocp1CommandResponseRequired(std::uint32_t targetOno,
                                std::uint16_t methodDefLevel,
                                std::uint16_t methodIndex,
                                const std::vector<std::uint8_t>& parameterData,
                                std::uint32_t& handle)
        : Ocp1Message(static_cast<std::uint8_t>(CommandResponseRequired), parameterData),
            m_targetOno(targetOno),
            m_methodDefLevel(methodDefLevel),
            m_methodIndex(methodIndex)
    {
        m_handle = m_nextHandle;
        handle = m_handle;
        m_nextHandle++;
    }

    std::vector<std::uint8_t> GetSerializedData() override
    {
        std::vector<std::uint8_t> serializedData = m_header.GetSerializedData();

        // TODO

        return serializedData;
    };

protected:
    std::uint32_t               m_handle;
    std::uint32_t               m_targetOno;
    std::uint16_t               m_methodDefLevel;
    std::uint16_t               m_methodIndex;
};


/**
 * Representation of an Oca Notification message.
 */
class Ocp1Notification : public Ocp1Message
{
public:
    Ocp1Notification(std::uint32_t emitterOno,
                     std::uint16_t emitterPropertyDefLevel,
                     std::uint16_t emitterPropertyIndex,
                     const std::vector<std::uint8_t>& parameterData)
        : Ocp1Message(static_cast<std::uint8_t>(Notification), parameterData),
            m_emitterOno(emitterOno),
            m_emitterPropertyDefLevel(emitterPropertyDefLevel),
            m_emitterPropertyIndex(emitterPropertyIndex)
    {
    }

    std::vector<std::uint8_t> GetSerializedData() 
    {
        std::vector<std::uint8_t> serializedData = m_header.GetSerializedData();

        // TODO

        return serializedData;
    };

protected:
    std::uint32_t               m_emitterOno;
    std::uint16_t               m_emitterPropertyDefLevel;
    std::uint16_t               m_emitterPropertyIndex;
};


/**
 * Representation of an Oca KeepAlive message. 
 */
class Ocp1KeepAlive : public Ocp1Message
{
public:
    Ocp1KeepAlive(std::uint16_t heartBeat)
        : Ocp1Message(static_cast<std::uint8_t>(KeepAlive), std::vector<std::uint8_t>()),
            m_heartBeat(heartBeat)
    {
    }

    std::vector<std::uint8_t> GetSerializedData() 
    {
        std::vector<std::uint8_t> serializedData = m_header.GetSerializedData();

        serializedData.push_back(static_cast<std::uint8_t>(m_heartBeat << 8));
        serializedData.push_back(static_cast<std::uint8_t>(m_heartBeat));

        return serializedData;
    };

protected:
    std::uint16_t               m_heartBeat;    // 0x00 0x05
};




class NanoOcp1Base
{
public:
    //==============================================================================
    NanoOcp1Base(const juce::String& address, const int port);
	~NanoOcp1Base();

    void setAddress(const juce::String& address);
    const juce::String& getAddress();

    void setPort(const int port);
    const int getPort();

    //==============================================================================
    virtual bool start() = 0;
    virtual bool stop() = 0;

    //==============================================================================
    virtual bool sendData(const juce::MemoryBlock& data) = 0;

    //==============================================================================
    std::function<bool(const juce::MemoryBlock&)> onDataReceived;
    std::function<void()> onConnectionEstablished;
    std::function<void()> onConnectionLost;

    //==============================================================================
    static std::uint32_t GetDBONo(std::uint32_t type, std::uint32_t record, std::uint32_t channel, std::uint32_t boxAndObjectNumber);
    static std::uint32_t GetDBONo2(std::uint32_t type, std::uint32_t record, std::uint32_t channel, std::uint32_t boxNumber, std::uint32_t objectNumber);

protected:
    //==============================================================================
    bool processReceivedData(const juce::MemoryBlock& data);

private:
    //==============================================================================
    juce::String    m_address;
    int             m_port{ 0 };

};

class NanoOcp1Client : public NanoOcp1Base, public Ocp1Connection, public juce::Timer
{
public:
    //==============================================================================
    NanoOcp1Client();
    NanoOcp1Client(const juce::String& address, const int port);
    ~NanoOcp1Client();

    //==============================================================================
    bool start();
    bool stop();

    //==============================================================================
    bool sendData(const juce::MemoryBlock& data) override;

    //==============================================================================
    void connectionMade() override;
    void connectionLost() override;
    void messageReceived(const juce::MemoryBlock& message) override;

protected:
    //==============================================================================
    void timerCallback() override;

private:
    //==============================================================================
    bool m_started{ false };
};

class NanoOcp1Server : public NanoOcp1Base, public Ocp1ConnectionServer
{
public:
    //==============================================================================
    NanoOcp1Server();
    NanoOcp1Server(const juce::String& address, const int port);
    ~NanoOcp1Server();

    //==============================================================================
    bool start();
    bool stop();

    //==============================================================================
    bool sendData(const juce::MemoryBlock& data) override;

protected:
    //==============================================================================
    Ocp1Connection* createConnectionObject() override;

private:
    //==============================================================================

    std::unique_ptr<NanoOcp1Client> m_activeConnection;
};

}
