/*
  ==============================================================================

    Ocp1Connection.cpp
    Created: 04 April 2023 20:14:00pm
    Author:  Christian Ahrens

  ==============================================================================
*/

#include "Ocp1ConnectionServer.h"
#include "Ocp1Connection.h"


namespace NanoOcp1
{


Ocp1ConnectionServer::Ocp1ConnectionServer() : Thread("NanoOcp1 connection server")
{
}

Ocp1ConnectionServer::~Ocp1ConnectionServer()
{
    stop();
}

//==============================================================================
bool Ocp1ConnectionServer::beginWaitingForSocket(const int portNumber, const String& bindAddress)
{
    stop();

    socket.reset(new StreamingSocket());

    if (socket->createListener(portNumber, bindAddress))
    {
        startThread();
        return true;
    }

    socket.reset();
    return false;
}

void Ocp1ConnectionServer::stop()
{
    signalThreadShouldExit();

    if (socket != nullptr)
        socket->close();

    stopThread(4000);
    socket.reset();
}

int Ocp1ConnectionServer::getBoundPort() const noexcept
{
    return (socket == nullptr) ? -1 : socket->getBoundPort();
}

void Ocp1ConnectionServer::run()
{
    while ((!threadShouldExit()) && socket != nullptr)
    {
        std::unique_ptr<StreamingSocket> clientSocket(socket->waitForNextConnection());

        if (clientSocket != nullptr)
            if (auto* newConnection = createConnectionObject())
                newConnection->initialiseWithSocket(std::move(clientSocket));
    }
}

}