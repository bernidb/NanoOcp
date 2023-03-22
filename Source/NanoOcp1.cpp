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

//==============================================================================
NanoOcp1Client::NanoOcp1Client() :
	NanoOcp1Client(juce::String(), 0)
{
}

NanoOcp1Client::NanoOcp1Client(const juce::String& address, const int port) :
	NanoOcp1Base(address, port), juce::InterprocessConnection()
{
}

NanoOcp1Client::~NanoOcp1Client()
{
	stop();
}

bool NanoOcp1Client::start()
{
	return connectToSocket(getAddress(), getPort(), 100);
}

bool NanoOcp1Client::stop()
{
	disconnect();

	return !isConnected();
}

bool NanoOcp1Client::sendData(const MemoryBlock& data)
{
	return juce::InterprocessConnection::sendMessage(data);
}

void NanoOcp1Client::connectionMade()
{

}

void NanoOcp1Client::connectionLost()
{

}

void NanoOcp1Client::messageReceived(const MemoryBlock& message)
{
	if (onDataReceived)
		onDataReceived(message);
}

//==============================================================================
NanoOcp1Server::NanoOcp1Server() :
	NanoOcp1Server(juce::String(), 0)
{
}

NanoOcp1Server::NanoOcp1Server(const juce::String& address, const int port) :
	NanoOcp1Base(address, port), juce::InterprocessConnectionServer()
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

bool NanoOcp1Server::sendData(const MemoryBlock& data)
{
	if (!m_activeConnection)
		return false;

	return m_activeConnection->sendData(data);
}

InterprocessConnection* NanoOcp1Server::createConnectionObject()
{
	m_activeConnection = std::make_unique<NanoOcp1Client>();
	m_activeConnection->onDataReceived = this->onDataReceived;

	return m_activeConnection.get();
}


}
