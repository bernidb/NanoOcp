/*
  ==============================================================================

    MainComponent.cpp
    Created: 28 Jul 2020 11:21:42am
    Author:  Christian Ahrens

  ==============================================================================
*/

#include "MainComponent.h"

#include "../../Source/NanoOcp1.h"
#include "../../Source/Ocp1ObjectDefinitions.h"


namespace NanoOcp1Demo
{

//==============================================================================
MainComponent::MainComponent()
{
    auto address = juce::String("127.0.0.1");
    auto port = 50014;

    // Editor to allow user input for ip address and port to use to connect
    m_ipAndPortEditor = std::make_unique<TextEditor>();
    m_ipAndPortEditor->setTextToShowWhenEmpty(address + ";" + juce::String(port), getLookAndFeel().findColour(juce::TextEditor::ColourIds::textColourId).darker().darker());
    m_ipAndPortEditor->addListener(this);
    addAndMakeVisible(m_ipAndPortEditor.get());
    
    // connected status visu
    m_connectedLED = std::make_unique<TextButton>("con");
    m_connectedLED->setColour(juce::TextButton::ColourIds::buttonOnColourId, juce::Colours::forestgreen);
    m_connectedLED->setColour(juce::TextButton::ColourIds::buttonColourId, juce::Colours::dimgrey);
    m_connectedLED->setToggleState(false, dontSendNotification);
    m_connectedLED->setEnabled(false);
    addAndMakeVisible(m_connectedLED.get());

    // Button for AddSubscription
    m_subscribePowerD40Button = std::make_unique<TextButton>("Pwr Subscribe");
    m_subscribePowerD40Button->setClickingTogglesState(true);
    m_subscribePowerD40Button->onClick = [=]()
    {
        if (m_subscribePowerD40Button->getToggleState())
        {
            std::uint32_t handle;
            m_nanoOcp1Client->sendData(NanoOcp1::Ocp1CommandResponseRequired(NanoOcp1::dbOcaObjectDef_Dy_AddSubscription_Settings_PwrOn,
                handle).GetMemoryBlock());
        }
        else
        {
            //std::uint32_t handle;
            //m_nanoOcp1Client->sendData(NanoOcp1::Ocp1CommandResponseRequired(NanoOcp1::dbOcaObjectDef_Dy_RemoveSubscription_Settings_PwrOn,
            //    handle).GetMemoryBlock());
        }
    };
    addAndMakeVisible(m_subscribePowerD40Button.get());

    // Button to act as Power display LED 
    m_powerD40LED = std::make_unique<TextButton>("Power LED");
    m_powerD40LED->setColour(juce::TextButton::ColourIds::buttonOnColourId, juce::Colours::forestgreen);
    m_powerD40LED->setColour(juce::TextButton::ColourIds::buttonColourId, juce::Colours::dimgrey);
    m_powerD40LED->setToggleState(false, dontSendNotification);
    m_powerD40LED->setEnabled(false);
    addAndMakeVisible(m_powerD40LED.get());

    // Button to power OFF the D40
    m_powerOffD40Button = std::make_unique<TextButton>("Pwr Off");
    m_powerOffD40Button->onClick = [=]() 
    {
        std::uint32_t handle;
        m_nanoOcp1Client->sendData(NanoOcp1::Ocp1CommandResponseRequired(NanoOcp1::dbOcaObjectDef_Dy_Settings_PwrOn_Off,
                                                                         handle).GetMemoryBlock());
    };
    addAndMakeVisible(m_powerOffD40Button.get());

    // Button to power ON the D40
    m_powerOnD40Button = std::make_unique<TextButton>("Pwr On");
    m_powerOnD40Button->onClick = [=]() 
    {
        std::uint32_t handle;
        m_nanoOcp1Client->sendData(NanoOcp1::Ocp1CommandResponseRequired(NanoOcp1::dbOcaObjectDef_Dy_Settings_PwrOn_On,
                                                                         handle).GetMemoryBlock());
    };
    addAndMakeVisible(m_powerOnD40Button.get());
    
    setSize(250, 200);

    // create the nano ocp1 client and fire it up
    m_nanoOcp1Client = std::make_unique<NanoOcp1::NanoOcp1Client>(address, port);
    m_nanoOcp1Client->onDataReceived = [=](const juce::MemoryBlock& message)
    {
        return OnOcp1MessageReceived(message);
    };
    m_nanoOcp1Client->onConnectionEstablished = [=]()
    {
        if (m_connectedLED)
            m_connectedLED->setToggleState(true, juce::dontSendNotification);
    };
    m_nanoOcp1Client->onConnectionLost = [=]()
    {
        if (m_connectedLED)
            m_connectedLED->setToggleState(false, juce::dontSendNotification);
    };
    m_nanoOcp1Client->start();
}

bool MainComponent::OnOcp1MessageReceived(const juce::MemoryBlock& message)
{
    std::unique_ptr<NanoOcp1::Ocp1Message> msgObj = NanoOcp1::Ocp1Message::UnmarshalOcp1Message(message);
    if (msgObj)
    {
        switch (msgObj->GetMessageType())
        {
            case NanoOcp1::Ocp1Message::Notification:
                {
                    NanoOcp1::Ocp1Notification* notifObj = static_cast<NanoOcp1::Ocp1Notification*>(msgObj.get());

                    // check ono, defLevel, propIdx

                    // Update GUI according to new value
                    std::uint16_t switchSetting = NanoOcp1::DataToUint16(notifObj->GetParameterData());
                    m_powerD40LED->setToggleState(switchSetting > 0, dontSendNotification);

                    return true;
                }
            case NanoOcp1::Ocp1Message::Response:
                {
                    NanoOcp1::Ocp1Response* responseObj = static_cast<NanoOcp1::Ocp1Response*>(msgObj.get());

                    if (responseObj->GetResponseStatus() != 0)
                    {
                        DBG("Got an OCA response for handle " << juce::String(responseObj->GetResponseHandle()) << 
                            " with status " << NanoOcp1::StatusToString(responseObj->GetResponseStatus()));
                    }

                    return true;
                }
            case NanoOcp1::Ocp1Message::KeepAlive:
                {
                    // Reset online timer

                    return true;
                }
            default:
                break;
        }
    }

    return false;
}

MainComponent::~MainComponent()
{
    m_nanoOcp1Client->stop();
}

void MainComponent::textEditorReturnKeyPressed(TextEditor& editor)
{
	if (&editor == m_ipAndPortEditor.get())
	{
		auto ip = editor.getText().upToFirstOccurrenceOf(";", false, true);
		auto port = editor.getText().fromLastOccurrenceOf(";", false, true).getIntValue();
		
		m_nanoOcp1Client->stop();
		m_nanoOcp1Client->setAddress(ip);
		m_nanoOcp1Client->setPort(port);
		m_nanoOcp1Client->start();
	}
}

void MainComponent::resized()
{
    auto bounds = getLocalBounds();

    auto connectionParamsHeight = 35;
    auto connectionLedWidth = 45;

    auto textEditorBounds = bounds.removeFromTop(connectionParamsHeight);
    auto connectedLedBounds = textEditorBounds.removeFromRight(connectionLedWidth);
    m_connectedLED->setBounds(connectedLedBounds.reduced(5));
    m_ipAndPortEditor->setBounds(textEditorBounds.reduced(5));

    auto button1Bounds = bounds;
    auto button2Bounds = button1Bounds.removeFromRight(button1Bounds.getWidth() / 2);
    auto button3Bounds = button2Bounds.removeFromBottom(button2Bounds.getHeight() / 2);
    auto button4Bounds = button1Bounds.removeFromBottom(button1Bounds.getHeight() / 2);

    button1Bounds.reduce(5, 5);
    button2Bounds.reduce(5, 5);
    button3Bounds.reduce(5, 5);
    button4Bounds.reduce(5, 5);

    m_subscribePowerD40Button->setBounds(button1Bounds);
    m_powerD40LED->setBounds(button2Bounds);
    m_powerOffD40Button->setBounds(button3Bounds);
    m_powerOnD40Button->setBounds(button4Bounds);
}

}
