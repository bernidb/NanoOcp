/*
  ==============================================================================

    NanoOcp1.cpp
    Created: 04 March 2022 22:10:00pm
    Author:  Christian Ahrens

  ==============================================================================
*/

#pragma once

#include <chrono>
#include <future>
#include <map>
#include <mutex>
#include <string>
#include <thread>
#include <vector>

#ifdef _WIN32
#include <winsock2.h>
#include <iphlpapi.h>
#define sleep(x) Sleep(x * 1000)
#else
#include <netdb.h>
#include <ifaddrs.h>
#include <net/if.h>
#endif

#include "../submodules/tcp_server_client/include/client_observer.h"
#include "../submodules/tcp_server_client/include/server_observer.h"

class TcpClient;
class TcpServer;

namespace NanoOcp1
{

class Ocp1Header
{
public:
    std::vector<std::uint8_t> GetSerializedData() {
        std::vector<std::uint8_t> serializedData;

        serializedData.push_back(m_syncVal);
        serializedData.push_back(static_cast<std::uint8_t>(m_protoVers));
        serializedData.push_back(static_cast<std::uint8_t>(m_protoVers << 8));
        serializedData.push_back(static_cast<std::uint8_t>(m_msgSize));
        serializedData.push_back(static_cast<std::uint8_t>(m_msgSize << 8));
        serializedData.push_back(static_cast<std::uint8_t>(m_msgSize << 16));
        serializedData.push_back(static_cast<std::uint8_t>(m_msgSize << 24));
        serializedData.push_back(m_msgType);
        serializedData.push_back(static_cast<std::uint8_t>(m_msgCnt));
        serializedData.push_back(static_cast<std::uint8_t>(m_msgCnt << 8));

        return serializedData;
    };

    std::uint8_t                m_syncVal;      // 0x3b
    std::uint16_t               m_protoVers;    // 0x00 0x01
    std::uint32_t               m_msgSize;      // 0x00 0x00 0x00 0x1a
    std::uint8_t                m_msgType;      // 0x01 // Ocp1CndRrq
    std::uint16_t               m_msgCnt;       // 0x00 0x01
};

class Ocp1Request
{
public:
    std::vector<std::uint8_t> GetSerializedData() {
        std::vector<std::uint8_t> serializedData;

        serializedData.push_back(static_cast<std::uint8_t>(m_cmdSize));
        serializedData.push_back(static_cast<std::uint8_t>(m_cmdSize << 8));
        serializedData.push_back(static_cast<std::uint8_t>(m_cmdSize << 16));
        serializedData.push_back(static_cast<std::uint8_t>(m_cmdSize << 24));
        serializedData.push_back(static_cast<std::uint8_t>(m_handle));
        serializedData.push_back(static_cast<std::uint8_t>(m_handle << 8));
        serializedData.push_back(static_cast<std::uint8_t>(m_handle << 16));
        serializedData.push_back(static_cast<std::uint8_t>(m_handle << 24));
        serializedData.push_back(static_cast<std::uint8_t>(m_targetONo));
        serializedData.push_back(static_cast<std::uint8_t>(m_targetONo << 8));
        serializedData.push_back(static_cast<std::uint8_t>(m_targetONo << 16));
        serializedData.push_back(static_cast<std::uint8_t>(m_targetONo << 24));
        serializedData.push_back(static_cast<std::uint8_t>(m_methIdDefLev));
        serializedData.push_back(static_cast<std::uint8_t>(m_methIdDefLev << 8));
        serializedData.push_back(static_cast<std::uint8_t>(m_methIdMethIdx));
        serializedData.push_back(static_cast<std::uint8_t>(m_methIdMethIdx << 8));
        serializedData.push_back(m_paramCnt);
        serializedData.insert(serializedData.end(), m_data.begin(), m_data.end());

        return serializedData;
    };

    std::uint32_t               m_cmdSize;      // 0x00 0x00 0x00 0x11
    std::uint32_t               m_handle;       // 0x00 0x00 0x00 0x01
    std::uint32_t               m_targetONo;    // 0x10 0x01 0x02 0x25
    std::uint16_t               m_methIdDefLev; // 0x00 0x04
    std::uint16_t               m_methIdMethIdx;// 0x00 0x01
    std::uint8_t                m_paramCnt;     // 0x00
    std::vector<std::uint8_t>   m_data;
};

class Ocp1ResponseBaseData
{
public:
    std::vector<std::uint8_t> GetSerializedData() {
        std::vector<std::uint8_t> serializedData;



        return serializedData;
    };

    std::uint8_t                m_respParamCnt;  // 0x03
    std::vector<std::uint8_t>   m_respParamData; // 0x00 0x00 0x00 0x00 0x00 0x01
};

class Ocp1BooleanSensorResponseData : public Ocp1ResponseBaseData
{
    // Resp Param Cnt  0x01
    // Resp Param Data
    // Value       0x00
};

class Ocp1SwitchResponseData : public Ocp1ResponseBaseData
{
    // Resp Param Cnt  0x03
    // Resp Param Data
    // Value       0x00 0x00
    // Min         0x00 0x00
    // Max         0x00 0x01

};

class Ocp1MuteResponseData : public Ocp1ResponseBaseData
{
    // Resp Param Cnt  0x01
    // Resp Param Data
    // Value       0x00
};

class Ocp1GainResponseData : public Ocp1ResponseBaseData
{
    // Resp Param Cnt  0x03
    // Resp Param Data
    // Value       0x00 0x00 0x00 0x00
    // Min         0x00 0x00 0x00 0x00
    // Max         0x00 0x00 0x00 0x00
};

class Ocp1StringActuatorResponseData : public Ocp1ResponseBaseData
{
    // Resp Param Cnt  0x01
    // Resp Param Data
    // SizeofStr   0x00 0x00
    // String -
};
 
class Ocp1StringSensorResponseData : public Ocp1ResponseBaseData
{
    // Resp Param Cnt  0x01
    // Resp Param Data
    // SizeofStr   0x00 0x00
    // String -
};

class Ocp1Int16SensorResponseData : public Ocp1ResponseBaseData
{
    // Resp Param Cnt  0x03
    // Resp Param Data
    // Value       0x00 0x00
    // Min         0x00 0x00
    // Max         0x00 0x00
};

class Ocp1Int32SensorResponseData : public Ocp1ResponseBaseData
{
    // Resp Param Cnt  0x03
    // Resp Param Data
    // Value       0x00 0x00 0x00 0x00
    // Min         0x00 0x00 0x00 0x00
    // Max         0x00 0x00 0x00 0x00
};

class Ocp1Int64SensorResponseData : public Ocp1ResponseBaseData
{
    // Resp Param Cnt  0x03
    // Resp Param Data
    // Value       0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00
    // Min         0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00
    // Max         0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00
};

class Ocp1Response
{
public:
    std::uint32_t           m_rspSize;  // 0x00 0x00 0x00 0x10
    std::uint32_t           m_handle;   // 0x00 0x00 0x00 0x01
    std::uint8_t            m_stat;     // 0x00
    Ocp1ResponseBaseData    m_respData; // 0x00 0x00 0x00 0x00 0x00 0x01
};

class NanoOcp1Base
{

public:
    NanoOcp1Base(std::string name, std::string serviceName);
	~NanoOcp1Base();

private:

};

class NanoOcp1Server : public NanoOcp1Base, public server_observer_t
{
public:
    NanoOcp1Server(std::string name, std::string serviceName);
    ~NanoOcp1Server();

private:
    void handleIncomingPacket(const std::string& clientIP, const char* msg, size_t size);
    void handleDisconnection(const std::string& ip, const std::string& msg);

    std::unique_ptr<TcpServer> m_tcpServer;
};

class NanoOcp1Client : public NanoOcp1Base, public client_observer_t
{
public:
    NanoOcp1Client(std::string name, std::string serviceName);
    ~NanoOcp1Client();

    void powerOffD40();
    void powerOnD40();

private:
    void handleIncomingPacket(const char* msg, size_t size);
    void handleDisconnection(const pipe_ret_t& ret);

    std::unique_ptr<TcpClient> m_tcpClient;
};

}
