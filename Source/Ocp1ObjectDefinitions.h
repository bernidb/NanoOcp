/*
  ==============================================================================

    Ocp1ObjectDefinitions.h
    Created: 3 Apr 2023 1:36:39pm
    Author:  bernardoe

  ==============================================================================
*/

#pragma once


#include "Ocp1Message.h"


namespace NanoOcp1
{

//==============================================================================
// Dy
//==============================================================================

enum BoxAndObjNo
{
    Settings_PwrOn      = 0x100,
    Config_Mute         = 0x205,
    Config_PotiLevel    = 0x206,
};

Ocp1CommandParameters dbOcaObjectDef_Dy_Settings_PwrOn
{
    GetONo(0x01, 0x00, 0x00, BoxAndObjNo::Settings_PwrOn),   // 0x10000100 ONO of Settings_PwrOn
    4,                                 // OcaSwitch level
    2,                                 // SetPosition method
    1,                                 // 1 Param
    std::vector<std::uint8_t>()        // Property value needs to be set outside this struct definition.
};

Ocp1CommandParameters dbOcaObjectDef_Dy_AddSubscription_Settings_PwrOn
{
    0x00000004,         // ONO of OcaSubscriptionManager
    3,                  // OcaSubscriptionManager level
    1,                  // AddSubscription method
    5,                  // 5 Params 
    DataFromOnoForSubscription(GetONo(0x01, 0x00, 0x00, BoxAndObjNo::Settings_PwrOn)) // 0x10000100 ONO of Settings_PwrOn
};

//Ocp1CommandParameters dbOcaObjectDef_Dy_RemoveSubscription_Settings_PwrOn
//{
//    0x00000004,         // ONO of OcaSubscriptionManager
//    3,                  // OcaSubscriptionManager level
//    2,                  // RemoveSubscription method
//    5,                  // 5 Params 
//    DataFromOnoForSubscription(GetONo(0x01, 0x00, 0x00, BoxAndObjNo::Settings_PwrOn)) // 0x10000100 ONO of Settings_PwrOn
//};

Ocp1CommandParameters dbOcaObjectDef_Dy_Config_PotiLevel
{
    std::uint32_t(),    // ONO of Config_PotiLevel needs to be set outside this struct definition.
    4,                  // OcaGain level
    2,                  // SetGain method
    1,                  // 1 Param
    std::vector<std::uint8_t>()  // Property value needs to be set outside this struct definition.
};

Ocp1CommandParameters dbOcaObjectDef_Dy_AddSubscription_Config_PotiLevel
{
    0x00000004,         // ONO of OcaSubscriptionManager
    3,                  // OcaSubscriptionManager level
    1,                  // AddSubscription method
    5,                  // 5 Params 
    std::vector<std::uint8_t>()  // Property value needs to be set outside this struct definition.
};

Ocp1CommandParameters dbOcaObjectDef_Dy_Config_Mute
{
    std::uint32_t(),    // ONO of Config_Mute needs to be set outside this struct definition.
    4,                  // OcaMute level
    2,                  // SetState method
    1,                  // 1 Param
    std::vector<std::uint8_t>()  // Property value needs to be set outside this struct definition.
};

Ocp1CommandParameters dbOcaObjectDef_Dy_AddSubscription_Config_Mute
{
    0x00000004,         // ONO of OcaSubscriptionManager
    3,                  // OcaSubscriptionManager level
    1,                  // AddSubscription method
    5,                  // 5 Params 
    std::vector<std::uint8_t>()  // Property value needs to be set outside this struct definition.
};


}