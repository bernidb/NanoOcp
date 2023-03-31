/*
  ==============================================================================

    MainComponent.cpp
    Created: 28 Jul 2020 11:21:42am
    Author:  Christian Ahrens

  ==============================================================================
*/

#include "MainComponent.h"

#include "../../Source/NanoOcp1.h"


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
		std::vector<std::uint8_t> parameterData;
		parameterData.push_back(static_cast<std::uint8_t>(0));
		parameterData.push_back(static_cast<std::uint8_t>(0)); // Position OFF

		std::uint32_t handle;
		m_nanoOcp1Client->sendData(NanoOcp1::Ocp1CommandResponseRequired(0x10000100,	// ONO of Settings_PwrOn
																		 4,				// OcaSwitch level
																		 2,				// SetPosition method
																		 parameterData,
																		 handle).GetMemoryBlock());
	};
    addAndMakeVisible(m_powerOffD40Button.get());

    m_powerOnD40Button = std::make_unique<TextButton>("Power On D40");
    m_powerOnD40Button->onClick = [=]() 
	{
	};
    addAndMakeVisible(m_powerOnD40Button.get());
	m_ipAndPortEditor = std::make_unique<TextEditor>();
	m_ipAndPortEditor->setTextToShowWhenEmpty(address + ";" + juce::String(port), getLookAndFeel().findColour(juce::TextEditor::ColourIds::textColourId).darker().darker());
	m_ipAndPortEditor->addListener(this);
	addAndMakeVisible(m_ipAndPortEditor.get());
    
	setSize(150, 150);
}

MainComponent::~MainComponent()
{
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
