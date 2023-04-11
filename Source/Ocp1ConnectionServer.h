/*
  ==============================================================================

    Ocp1Connection.h
    Created: 04 April 2023 20:14:00pm
    Author:  Christian Ahrens

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>


namespace NanoOcp1
{

class Ocp1Connection;


//==============================================================================
/**
    Class taken from JUCE and modified to act as regular TCP connection without
    the magic header contents and all pipe related methods, etc.

    @see NanoOcp1::Ocp1Connection
*/
class Ocp1ConnectionServer : private Thread
{
public:
    //==============================================================================
    Ocp1ConnectionServer();
    ~Ocp1ConnectionServer() override;

    bool beginWaitingForSocket(int portNumber, const String& bindAddress = String());
    void stop();
    int getBoundPort() const noexcept;

protected:
    //==============================================================================
    virtual Ocp1Connection* createConnectionObject() = 0;

private:
    //==============================================================================
    std::unique_ptr<StreamingSocket> socket;

    void run() override;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Ocp1ConnectionServer)
};

}