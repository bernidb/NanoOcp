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

    m_powerOffD40Button = std::make_unique<TextButton>("Power Off D40");
    m_powerOffD40Button->onClick = [=]() {
		NanoOcp1::Ocp1Header header;
		header.m_syncVal = 0x3b;
		header.m_protoVers = 0x0001;
		header.m_msgSize = 0x0000001c;
		header.m_msgType = 0x01;
		header.m_msgCnt = 0x0001;
		NanoOcp1::Ocp1Request request;
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

		m_nanoOcp1Client->sendData(juce::MemoryBlock((const char*)msgData.data(), msgData.size()));
	};
    addAndMakeVisible(m_powerOffD40Button.get());

    m_powerOnD40Button = std::make_unique<TextButton>("Power On D40");
    m_powerOnD40Button->onClick = [=]() {
		NanoOcp1::Ocp1Header header;
		header.m_syncVal = 0x3b;
		header.m_protoVers = 0x0001;
		header.m_msgSize = 0x0000001c;
		header.m_msgType = 0x01;
		header.m_msgCnt = 0x0001;
		NanoOcp1::Ocp1Request request;
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
		m_nanoOcp1Client->sendData(juce::MemoryBlock((const char*)msgData.data(), msgData.size()));
	};
    addAndMakeVisible(m_powerOnD40Button.get());
    
	setSize(150, 150);
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
    auto button2Bounds = button1Bounds.removeFromBottom(button1Bounds.getHeight() / 2);

    button1Bounds.reduce(5, 5);
    button2Bounds.reduce(5, 5);

    m_powerOffD40Button->setBounds(button1Bounds);
    m_powerOnD40Button->setBounds(button2Bounds);
}

}
