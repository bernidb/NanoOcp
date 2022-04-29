/*
  ==============================================================================

    NanoOcp1.cpp
    Created: 04 March 2022 22:10:00pm
    Author:  Christian Ahrens

  ==============================================================================
*/

#include "NanoOcp1.h"

#include <algorithm>
#include <cassert>
#include <cstring>
#include <iostream>

#include "../submodules/tcp_server_client/include/tcp_client.h"
#include "../submodules/tcp_server_client/include/tcp_server.h"


namespace NanoOcp1
{


//==============================================================================
NanoOcp1Base::NanoOcp1Base(std::string name, std::string serviceName)
{

}

NanoOcp1Base::~NanoOcp1Base()
{
}

//==============================================================================
NanoOcp1Server::NanoOcp1Server(std::string name, std::string serviceName) :
	NanoOcp1Base(name, serviceName)
{

	wantedIP = "127.0.0.1";
	incomingPacketHandler = [=](const std::string& clientIP, const char* msg, size_t size) { handleIncomingPacket(clientIP, msg, size); };
	disconnectionHandler = [=](const std::string& ip, const std::string& msg) { handleDisconnection(ip, msg); };

	m_tcpServer = std::make_unique<TcpServer>();
	m_tcpServer->subscribe(*this);
}

NanoOcp1Server::~NanoOcp1Server()
{
	m_tcpServer->close();
}

void NanoOcp1Server::handleIncomingPacket(const std::string& clientIP, const char* msg, size_t size)
{

}

void NanoOcp1Server::handleDisconnection(const std::string& ip, const std::string& msg)
{

}

//==============================================================================
NanoOcp1Client::NanoOcp1Client(std::string name, std::string serviceName) :
	NanoOcp1Base(name, serviceName)
{

	wantedIP = "127.0.0.1";
	incomingPacketHandler = [=](const char* msg, size_t size) { handleIncomingPacket(msg, size); };
	disconnectionHandler = [=](const pipe_ret_t& ret) { handleDisconnection(ret); };

	m_tcpClient = std::make_unique<TcpClient>();
	m_tcpClient->subscribe(*this);
	
	m_tcpClient->connectTo("192.168.1.124", 50014);
}

NanoOcp1Client::~NanoOcp1Client()
{
	m_tcpClient->close();
}

void NanoOcp1Client::handleIncomingPacket(const char* msg, size_t size)
{

}

void NanoOcp1Client::handleDisconnection(const pipe_ret_t& ret)
{

}

void NanoOcp1Client::powerOffD40() // "3b 0001 0000001c 01 0001 00000013 00000002 10000100 0004 0002 01 0000"
{
	Ocp1Header header;
	header.m_syncVal = 0x3b;
	header.m_protoVers = 0x0001;
	header.m_msgSize = 0x0000001c;
	header.m_msgType = 0x01;
	header.m_msgCnt = 0x0001;
	Ocp1Request request;
	request.m_cmdSize = 0x00000013;
	request.m_handle = 0x00000002;
	request.m_targetONo = 0x10000100;
	request.m_methIdDefLev = 0x0004;
	request.m_methIdMethIdx = 0x0002;
	request.m_paramCnt = 0x01;
	request.m_data.push_back(0x00);
	request.m_data.push_back(0x00);

	auto headerData = header.GetSerializedData();
	auto requestData = request.GetSerializedData();
	auto& msgData = headerData;
	msgData.insert(msgData.end(), requestData.begin(), requestData.end());
    auto retval = m_tcpClient->sendMsg((const char*)msgData.data(), msgData.size());
}

void NanoOcp1Client::powerOnD40() // "3b 0001 0000001c 01 0001 00000013 00000002 10000100 0004 0002 01 0001"
{
	Ocp1Header header;
	header.m_syncVal = 0x3b;
	header.m_protoVers = 0x0001;
	header.m_msgSize = 0x0000001c;
	header.m_msgType = 0x01;
	header.m_msgCnt = 0x0001;
	Ocp1Request request;
	request.m_cmdSize = 0x00000013;
	request.m_handle = 0x00000002;
	request.m_targetONo = 0x10000100;
	request.m_methIdDefLev = 0x0004;
	request.m_methIdMethIdx = 0x0002;
	request.m_paramCnt = 0x01;
	request.m_data.push_back(0x00);
	request.m_data.push_back(0x01);

	auto headerData = header.GetSerializedData();
	auto requestData = request.GetSerializedData();
	auto& msgData = headerData;
	msgData.insert(msgData.end(), requestData.begin(), requestData.end());
	auto retval = m_tcpClient->sendMsg((const char*)msgData.data(), msgData.size());
}

}
