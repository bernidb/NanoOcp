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
