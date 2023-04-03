/*
  ==============================================================================

    Ocp1ObjectDefinitions.h
    Created: 3 Apr 2023 1:36:39pm
    Author:  bernardoe

  ==============================================================================
*/

#pragma once


#include "../../Source/Ocp1Message.h"


namespace NanoOcp1
{

//==============================================================================
// Dy
//==============================================================================

Ocp1CommandParameters dbOcaObjectDef_Dy_Settings_PwrOn_Off
{ 
    0x10000100,         // ONO of Settings_PwrOn
    4,                  // OcaSwitch level
    2,                  // SetPosition method
    DataFromUint16(0)   // Position OFF
};

Ocp1CommandParameters dbOcaObjectDef_Dy_Settings_PwrOn_On
{
    0x10000100,         // ONO of Settings_PwrOn
    4,                  // OcaSwitch level
    2,                  // SetPosition method
    DataFromUint16(1)   // Position ON
};

}