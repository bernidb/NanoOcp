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
class MainComponent   : public Component
{
public:
    //==============================================================================
    MainComponent();
    ~MainComponent();

    //==============================================================================
    void paint (Graphics&) override;
    void resized() override;
    
private:
    std::unique_ptr<NanoOcp1::NanoOcp1Client>           m_nanoOcp1Client;
    
    //==============================================================================
    std::unique_ptr<TextButton>             m_powerOnD40Button;
    std::unique_ptr<TextButton>             m_powerOffD40Button;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainComponent)
};

}
