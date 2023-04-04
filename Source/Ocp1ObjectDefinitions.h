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
    1,                  // 1 Param
    DataFromUint16(0)   // Position OFF
};

Ocp1CommandParameters dbOcaObjectDef_Dy_Settings_PwrOn_On
{
    0x10000100,         // ONO of Settings_PwrOn
    4,                  // OcaSwitch level
    2,                  // SetPosition method
    1,                  // 1 Param
    DataFromUint16(1)   // Position ON
};

Ocp1CommandParameters dbOcaObjectDef_Dy_AddSubscription_Settings_PwrOn
{
    0x00000004,         // ONO of OcaSubscriptionManager
    3,                  // OcaSubscriptionManager level
    1,                  // AddSubscription method
    5,                  // 5 Params 
    DataFromOnoForSubscription(0x10000100) // ONO of Settings_PwrOn
};

//Ocp1CommandParameters dbOcaObjectDef_Dy_RemoveSubscription_Settings_PwrOn
//{
//    0x00000004,         // ONO of OcaSubscriptionManager
//    3,                  // OcaSubscriptionManager level
//    2,                  // RemoveSubscription method
//    5,                  // 5 Params 
//    DataFromOnoForSubscription(0x10000100) // ONO of Settings_PwrOn
//};

}