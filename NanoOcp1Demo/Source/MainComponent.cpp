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

	m_ipAndPortEditor = std::make_unique<TextEditor>();
	m_ipAndPortEditor->setTextToShowWhenEmpty(address + ";" + juce::String(port), getLookAndFeel().findColour(juce::TextEditor::ColourIds::textColourId).darker().darker());
	m_ipAndPortEditor->addListener(this);
	addAndMakeVisible(m_ipAndPortEditor.get());

	m_5DpowerOnButton = std::make_unique<TextButton>();
	m_5DpowerOnButton->setClickingTogglesState(true);
	m_5DpowerOnButton->setButtonText("PwrON");
	m_5DpowerOnButton->setColour(juce::TextButton::ColourIds::buttonOnColourId, juce::Colours::forestgreen);
	m_5DpowerOnButton->addListener(this);
	addAndMakeVisible(m_5DpowerOnButton.get());

	m_5DgainSliderCh1 = std::make_unique<Slider>();
	m_5DgainSliderCh1->setSliderStyle(juce::Slider::SliderStyle::LinearVertical);
	m_5DgainSliderCh1->setTextBoxStyle(juce::Slider::TextEntryBoxPosition::TextBoxBelow, false, 60, 15);
	m_5DgainSliderCh1->addListener(this);
	addAndMakeVisible(m_5DgainSliderCh1.get());
	m_5DgainSliderCh2 = std::make_unique<Slider>();
	m_5DgainSliderCh2->setSliderStyle(juce::Slider::SliderStyle::LinearVertical);
	m_5DgainSliderCh2->setTextBoxStyle(juce::Slider::TextEntryBoxPosition::TextBoxBelow, false, 60, 15);
	m_5DgainSliderCh2->addListener(this);
	addAndMakeVisible(m_5DgainSliderCh2.get());
	m_5DgainSliderCh3 = std::make_unique<Slider>();
	m_5DgainSliderCh3->setSliderStyle(juce::Slider::SliderStyle::LinearVertical);
	m_5DgainSliderCh3->setTextBoxStyle(juce::Slider::TextEntryBoxPosition::TextBoxBelow, false, 60, 15);
	m_5DgainSliderCh3->addListener(this);
	addAndMakeVisible(m_5DgainSliderCh3.get());
	m_5DgainSliderCh4 = std::make_unique<Slider>();
	m_5DgainSliderCh4->setSliderStyle(juce::Slider::SliderStyle::LinearVertical);
	m_5DgainSliderCh4->setTextBoxStyle(juce::Slider::TextEntryBoxPosition::TextBoxBelow, false, 60, 15);
	m_5DgainSliderCh4->addListener(this);
	addAndMakeVisible(m_5DgainSliderCh4.get());

	m_5DmuteButtonCh1 = std::make_unique<TextButton>();
	m_5DmuteButtonCh1->setClickingTogglesState(true);
	m_5DmuteButtonCh1->setButtonText("MUTE Ch1");
	m_5DmuteButtonCh1->setColour(juce::TextButton::ColourIds::buttonOnColourId, juce::Colours::indianred);
	m_5DmuteButtonCh1->addListener(this);
	addAndMakeVisible(m_5DmuteButtonCh1.get());
	m_5DmuteButtonCh2 = std::make_unique<TextButton>();
	m_5DmuteButtonCh2->setClickingTogglesState(true);
	m_5DmuteButtonCh2->setButtonText("MUTE Ch2");
	m_5DmuteButtonCh2->setColour(juce::TextButton::ColourIds::buttonOnColourId, juce::Colours::red);
	m_5DmuteButtonCh2->addListener(this);
	addAndMakeVisible(m_5DmuteButtonCh2.get());
	m_5DmuteButtonCh3 = std::make_unique<TextButton>();
	m_5DmuteButtonCh3->setClickingTogglesState(true);
	m_5DmuteButtonCh3->setButtonText("MUTE Ch3");
	m_5DmuteButtonCh3->setColour(juce::TextButton::ColourIds::buttonOnColourId, juce::Colours::red);
	m_5DmuteButtonCh3->addListener(this);
	addAndMakeVisible(m_5DmuteButtonCh3.get());
	m_5DmuteButtonCh4 = std::make_unique<TextButton>();
	m_5DmuteButtonCh4->setClickingTogglesState(true);
	m_5DmuteButtonCh4->setButtonText("MUTE Ch4");
	m_5DmuteButtonCh4->setColour(juce::TextButton::ColourIds::buttonOnColourId, juce::Colours::red);
	m_5DmuteButtonCh4->addListener(this);
	addAndMakeVisible(m_5DmuteButtonCh4.get());
    
	setSize(225, 300);
}

MainComponent::~MainComponent()
{
}

const std::vector<std::uint8_t> MainComponent::CreateMute5DData(int channel, bool muteState)
{
	NanoOcp1::Ocp1Header header;
	header.m_syncVal = 0x3b;
	header.m_protoVers = 0x0001;
	header.m_msgSize = 0x0000001c;
	header.m_msgType = 0x01;
	header.m_msgCnt = 0x0001;
	NanoOcp1::Ocp1Request request;
	request.m_cmdSize = 0x00000013;
	request.m_handle = 0x00000002;
	request.m_targetONo = NanoOcp1::NanoOcp1Base::GetDBONo2(2, 0, channel, 0x00, 0x01);
	request.m_methIdDefLev = 0x0004;
	request.m_methIdMethIdx = 0x0002;
	request.m_paramCnt = 0x01;
	request.m_data.push_back(0x00);
	request.m_data.push_back(muteState ? 0x01 : 0x02);

	auto headerData = header.GetSerializedData();
	auto requestData = request.GetSerializedData();
	auto& msgData = headerData;
	msgData.insert(msgData.end(), requestData.begin(), requestData.end());

	return msgData;
}

const std::vector<std::uint8_t> MainComponent::CreateGain5DData(int /*channel*/, double /*gainValue*/)
{
	NanoOcp1::Ocp1Header header;
	//header.m_syncVal = 0x3b;
	//header.m_protoVers = 0x0001;
	//header.m_msgSize = 0x0000001c;
	//header.m_msgType = 0x01;
	//header.m_msgCnt = 0x0001;
	//NanoOcp1::Ocp1Request request;
	//request.m_cmdSize = 0x00000013;
	//request.m_handle = 0x00000002;
	//request.m_targetONo = NanoOcp1::NanoOcp1Base::GetDBONo2(2, 0, channel, NanoOcp1::NanoOcp1Base::DS100MatrixInputBox, NanoOcp1::NanoOcp1Base::DS100MatrixInputObjectNumber::MatrixInputMute);
	//request.m_methIdDefLev = 0x0004;
	//request.m_methIdMethIdx = 0x0002;
	//request.m_paramCnt = 0x01;
	//request.m_data.push_back(0x00);
	//request.m_data.push_back(muteState ? 0x01 : 0x02);

	auto headerData = header.GetSerializedData();
	//auto requestData = request.GetSerializedData();
	auto& msgData = headerData;
	//msgData.insert(msgData.end(), requestData.begin(), requestData.end());

	return msgData;
}

const std::vector<std::uint8_t> MainComponent::CreatePowerOn5DData(bool onState)
{
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
	request.m_data.push_back(onState ? 0x01 : 0x00);

	auto headerData = header.GetSerializedData();
	auto requestData = request.GetSerializedData();
	auto& msgData = headerData;
	msgData.insert(msgData.end(), requestData.begin(), requestData.end());

	return msgData;
}

void MainComponent::buttonClicked(Button* button)
{
	if (button == m_5DmuteButtonCh1.get())
	{
		auto& muteData = CreateMute5DData(1, button->getToggleState());
		if (!m_nanoOcp1Client->isConnected())
			DBG("client not connected");
		else if (!m_nanoOcp1Client->sendData(juce::MemoryBlock((const char*)muteData.data(), muteData.size())))
			DBG("sending failed");
	}
	else if (button == m_5DmuteButtonCh2.get())
	{
		auto& muteData = CreateMute5DData(2, button->getToggleState());
		if (!m_nanoOcp1Client->isConnected())
			DBG("client not connected");
		else if (!m_nanoOcp1Client->sendData(juce::MemoryBlock((const char*)muteData.data(), muteData.size())))
			DBG("sending failed");
	}
	else if (button == m_5DmuteButtonCh3.get())
	{
		auto& muteData = CreateMute5DData(3, button->getToggleState());
		if (!m_nanoOcp1Client->isConnected())
			DBG("client not connected");
		else if (!m_nanoOcp1Client->sendData(juce::MemoryBlock((const char*)muteData.data(), muteData.size())))
			DBG("sending failed");
	}
	else if (button == m_5DmuteButtonCh4.get())
	{
		auto& muteData = CreateMute5DData(4, button->getToggleState());
		if (!m_nanoOcp1Client->isConnected())
			DBG("client not connected");
		else if (!m_nanoOcp1Client->sendData(juce::MemoryBlock((const char*)muteData.data(), muteData.size())))
			DBG("sending failed");
	}
	else if (button == m_5DpowerOnButton.get())
	{
		auto& pwrOnData = CreatePowerOn5DData(button->getToggleState());
		if (!m_nanoOcp1Client->isConnected())
			DBG("client not connected");
		else if (!m_nanoOcp1Client->sendData(juce::MemoryBlock((const char*)pwrOnData.data(), pwrOnData.size())))
			DBG("sending failed");
	}
}

void MainComponent::sliderValueChanged(Slider* slider)
{
	if (slider == m_5DgainSliderCh1.get())
	{
		auto& gainData = CreateGain5DData(1, slider->getValue());
		if (!m_nanoOcp1Client->isConnected())
			DBG("client not connected");
		else if (!m_nanoOcp1Client->sendData(juce::MemoryBlock((const char*)gainData.data(), gainData.size())))
			DBG("sending failed");
	}
	else if (slider == m_5DgainSliderCh2.get())
	{
		auto& gainData = CreateGain5DData(2, slider->getValue());
		if (!m_nanoOcp1Client->isConnected())
			DBG("client not connected");
		else if (!m_nanoOcp1Client->sendData(juce::MemoryBlock((const char*)gainData.data(), gainData.size())))
			DBG("sending failed");
	}
	else if (slider == m_5DgainSliderCh3.get())
	{
		auto& gainData = CreateGain5DData(3, slider->getValue());
		if (!m_nanoOcp1Client->isConnected())
			DBG("client not connected");
		else if (!m_nanoOcp1Client->sendData(juce::MemoryBlock((const char*)gainData.data(), gainData.size())))
			DBG("sending failed");
	}
	else if (slider == m_5DgainSliderCh4.get())
	{
		auto& gainData = CreateGain5DData(4, slider->getValue());
		if (!m_nanoOcp1Client->isConnected())
			DBG("client not connected");
		else if (!m_nanoOcp1Client->sendData(juce::MemoryBlock((const char*)gainData.data(), gainData.size())))
			DBG("sending failed");
	}
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
	auto connectionParamsHeight = 35;
	auto buttonHeight = 50;
	auto channelWidth = getWidth() / 4;

	auto bounds = getLocalBounds();

	auto textEditorBounds = bounds.removeFromTop(connectionParamsHeight).reduced(5);
	m_ipAndPortEditor->setBounds(textEditorBounds);

	auto pwrOnBounds = bounds.removeFromTop(buttonHeight).reduced(5);
	m_5DpowerOnButton->setBounds(pwrOnBounds);

	auto muteBounds = bounds.removeFromBottom(buttonHeight);
	m_5DmuteButtonCh1->setBounds(muteBounds.removeFromLeft(channelWidth).reduced(5));
	m_5DmuteButtonCh2->setBounds(muteBounds.removeFromLeft(channelWidth).reduced(5));
	m_5DmuteButtonCh3->setBounds(muteBounds.removeFromLeft(channelWidth).reduced(5));
	m_5DmuteButtonCh4->setBounds(muteBounds.reduced(5));

	auto gainBounds = bounds;
	m_5DgainSliderCh1->setBounds(gainBounds.removeFromLeft(channelWidth).reduced(5));
	m_5DgainSliderCh2->setBounds(gainBounds.removeFromLeft(channelWidth).reduced(5));
	m_5DgainSliderCh3->setBounds(gainBounds.removeFromLeft(channelWidth).reduced(5));
	m_5DgainSliderCh4->setBounds(gainBounds.reduced(5));
}

}
