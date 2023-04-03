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

    m_nanoOcp1Client = std::make_unique<NanoOcp1::NanoOcp1Client>(address, port);
	m_nanoOcp1Client->start();

    m_powerOffD40Button = std::make_unique<TextButton>("Power Off D40");
    m_powerOffD40Button->onClick = [=]() 
    {
        std::uint32_t handle;
        m_nanoOcp1Client->sendData(NanoOcp1::Ocp1CommandResponseRequired(NanoOcp1::dbOcaObjectDef_Dy_Settings_PwrOn_Off,
                                                                         handle).GetMemoryBlock());
    };
    addAndMakeVisible(m_powerOffD40Button.get());

    m_powerOnD40Button = std::make_unique<TextButton>("Power On D40");
    m_powerOnD40Button->onClick = [=]() 
    {
        std::uint32_t handle;
        m_nanoOcp1Client->sendData(NanoOcp1::Ocp1CommandResponseRequired(NanoOcp1::dbOcaObjectDef_Dy_Settings_PwrOn_On,
                                                                         handle).GetMemoryBlock());
    };
    addAndMakeVisible(m_powerOnD40Button.get());
	m_ipAndPortEditor = std::make_unique<TextEditor>();
	m_ipAndPortEditor->setTextToShowWhenEmpty(address + ";" + juce::String(port), getLookAndFeel().findColour(juce::TextEditor::ColourIds::textColourId).darker().darker());
	m_ipAndPortEditor->addListener(this);
	addAndMakeVisible(m_ipAndPortEditor.get());
    
    setSize(150, 150);




    // ==== TODO: move this code to NanoOcp1Client::messageReceived ====
    if(true)
    {
        NanoOcp1::Ocp1Notification notif1(static_cast<std::uint32_t>(0x10000100), // emitterOno
                                          static_cast<std::uint16_t>(4), // emitterPropertyDefLevel
                                          static_cast<std::uint16_t>(2), // emitterPropertyIndex
                                          NanoOcp1::DataFromUint16(1)); // parameterData

        juce::MemoryBlock notif1Mem = notif1.GetMemoryBlock();

        // recieve notification...

        std::unique_ptr<NanoOcp1::Ocp1Message> receivedMsg = NanoOcp1::Ocp1Message::UnmarshalOcp1Message(notif1Mem);

        switch (receivedMsg->GetMessageType())
        {
            case NanoOcp1::Ocp1Message::Notification:
                {
                    NanoOcp1::Ocp1Notification* notif2 = static_cast<NanoOcp1::Ocp1Notification*>(receivedMsg.get());

                    // check ono, defLevel, propIdx

                    std::uint16_t switchSetting = NanoOcp1::DataToUint16(notif2->GetParameterData());
                }
                break;
            case NanoOcp1::Ocp1Message::Response:
                {
                    // TODO
                }
                break;
            case NanoOcp1::Ocp1Message::KeepAlive:
                // Reset online timer
                break;
        }

    }
}

MainComponent::~MainComponent()
{
    m_nanoOcp1Client->stop();
}

{

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
    auto button1Bounds = getLocalBounds();
    auto button2Bounds = button1Bounds.removeFromBottom(button1Bounds.getHeight() / 2);
	auto connectionParamsHeight = 35;

	auto bounds = getLocalBounds();

	auto textEditorBounds = bounds.removeFromTop(connectionParamsHeight).reduced(5);
	m_ipAndPortEditor->setBounds(textEditorBounds);

    button1Bounds.reduce(5, 5);
    button2Bounds.reduce(5, 5);

    m_powerOffD40Button->setBounds(button1Bounds);
    m_powerOnD40Button->setBounds(button2Bounds);
}

}
