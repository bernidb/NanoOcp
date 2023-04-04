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
