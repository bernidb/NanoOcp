/*
  ==============================================================================

    MainComponent.h
    Created: 28 April 2022 11:21:42am
    Author:  Christian Ahrens

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

#include "../../Source/NanoOcp1.h"

namespace NanoOcp1
{
class NanoOcp1;
}

namespace NanoOcp1Demo
{

//==============================================================================
/*
    This component lives inside our window, and this is where you should put all
    your controls and content.
*/
class MainComponent   : public Component, juce::Button::Listener, juce::Slider::Listener, juce::TextEditor::Listener
{
public:
    //==============================================================================
    MainComponent();
    ~MainComponent();

    const std::vector<std::uint8_t> CreateMute5DData(int channel, bool muteState = true);
    const std::vector<std::uint8_t> CreateGain5DData(int channel, double gainValue = 0.0);
    const std::vector<std::uint8_t> CreatePowerOn5DData(bool onState = true);

    //==============================================================================
    void resized() override;

    //==============================================================================
    void buttonClicked(Button* button) override;

    //==============================================================================
    void sliderValueChanged(Slider* slider) override;

    //==============================================================================
    void textEditorReturnKeyPressed(TextEditor& editor) override;
    
private:
    std::unique_ptr<NanoOcp1::NanoOcp1Client>  m_nanoOcp1Client;
    
    //==============================================================================
    std::unique_ptr<TextEditor> m_ipAndPortEditor;

    std::unique_ptr<TextButton> m_5DpowerOnButton;

    std::unique_ptr<Slider>     m_5DgainSliderCh1;
    std::unique_ptr<Slider>     m_5DgainSliderCh2;
    std::unique_ptr<Slider>     m_5DgainSliderCh3;
    std::unique_ptr<Slider>     m_5DgainSliderCh4;

    std::unique_ptr<TextButton> m_5DmuteButtonCh1;
    std::unique_ptr<TextButton> m_5DmuteButtonCh2;
    std::unique_ptr<TextButton> m_5DmuteButtonCh3;
    std::unique_ptr<TextButton> m_5DmuteButtonCh4;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainComponent)
};

}
