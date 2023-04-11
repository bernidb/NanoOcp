/*
  ==============================================================================

    MainComponent.h
    Created: 28 April 2022 11:21:42am
    Author:  Christian Ahrens

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>


namespace NanoOcp1
{
    class NanoOcp1;
    class NanoOcp1Client;
}

namespace NanoOcp1Demo
{

//==============================================================================
/*
    This component lives inside our window, and this is where you should put all
    your controls and content.
*/
class MainComponent   : public Component, public juce::TextEditor::Listener
{
public:
    //==============================================================================
    MainComponent();
    ~MainComponent();

    //==============================================================================
    void resized() override;

    //==============================================================================
    void textEditorReturnKeyPressed(TextEditor& editor) override;

    //==============================================================================
    bool OnOcp1MessageReceived(const juce::MemoryBlock& message);
    
private:
    //==============================================================================
    std::unique_ptr<NanoOcp1::NanoOcp1Client>   m_nanoOcp1Client;
    
    //==============================================================================
    std::unique_ptr<TextEditor>     m_ipAndPortEditor;
    std::unique_ptr<TextButton>     m_connectedLED;
    std::unique_ptr<TextButton>     m_subscribeButton;
    std::unique_ptr<TextButton>     m_powerD40LED;
    std::unique_ptr<TextButton>     m_powerOnD40Button;
    std::unique_ptr<TextButton>     m_powerOffD40Button;
    std::unique_ptr<Slider>         m_gainSlider;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainComponent)
};

}
