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
    m_nanoOcp1Client = std::make_unique<NanoOcp1::NanoOcp1Client>("NanoOcpClientInstance", JUCEApplication::getInstance()->getApplicationName().toStdString());

    m_powerOffD40Button = std::make_unique<TextButton>("Power Off D40");
    m_powerOffD40Button->onClick = [=]() { m_nanoOcp1Client->powerOffD40(); };
    addAndMakeVisible(m_powerOnD40Button.get());

    m_powerOnD40Button = std::make_unique<TextButton>("Power On D40");
    m_powerOnD40Button->onClick = [=]() { m_nanoOcp1Client->powerOnD40(); };
    addAndMakeVisible(m_powerOnD40Button.get());
    
	setSize(512, 512);
}

MainComponent::~MainComponent()
{
}

void MainComponent::paint (Graphics& g)
{
	g.fillAll(Colours::white);
}

void MainComponent::resized()
{
    auto button1Bounds = getLocalBounds();
    auto button2Bounds = button1Bounds.removeFromBottom(0.5f * button1Bounds.getHeight());

    button1Bounds.reduce(5, 5);
    button2Bounds.reduce(5, 5);

    m_powerOffD40Button->setBounds(button1Bounds);
    m_powerOnD40Button->setBounds(button2Bounds);
}

}
