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

struct dbOcaObjectDef_Dy_Set_Settings_PwrOn : Ocp1CommandDefinition
{
    dbOcaObjectDef_Dy_Set_Settings_PwrOn(std::uint16_t setting) // 1 == ON; 0 == OFF
        : Ocp1CommandDefinition(GetONo(0x01, 0x00, 0x00, BoxAndObjNo::Settings_PwrOn), // ONO of Settings_PwrOn
                                4,                              // OcaSwitch level
                                2,                              // SetPosition method
                                1,                              // 1 Param
                                DataFromUint16(setting))        // Setting parameter
    {
    }
};

struct dbOcaObjectDef_Dy_Get_Settings_PwrOn : Ocp1CommandDefinition
{
    dbOcaObjectDef_Dy_Get_Settings_PwrOn()
        : Ocp1CommandDefinition(GetONo(0x01, 0x00, 0x00, BoxAndObjNo::Settings_PwrOn),   // ONO of Settings_PwrOn
                                4,                              // OcaSwitch level
                                1,                              // GetPosition method
                                0,                              // 0 Param
                                std::vector<std::uint8_t>())    // Empty parameters
    {
    }
};

struct dbOcaObjectDef_Dy_AddSubscription_Settings_PwrOn : Ocp1CommandDefinition
{
    dbOcaObjectDef_Dy_AddSubscription_Settings_PwrOn()
        : Ocp1CommandDefinition(0x00000004,                     // ONO of OcaSubscriptionManager
                                3,                              // OcaSubscriptionManager level
                                1,                              // AddSubscription method
                                5,                              // 5 Params 
                                DataFromOnoForSubscription(GetONo(0x01, 0x00, 0x00, BoxAndObjNo::Settings_PwrOn))) // ONO of Settings_PwrOn
    {
    }
};

//Ocp1CommandDefinition dbOcaObjectDef_Dy_RemoveSubscription_Settings_PwrOn
//{
//    0x00000004,         // ONO of OcaSubscriptionManager
//    3,                  // OcaSubscriptionManager level
//    2,                  // RemoveSubscription method
//    5,                  // 5 Params 
//    DataFromOnoForSubscription(GetONo(0x01, 0x00, 0x00, BoxAndObjNo::Settings_PwrOn)) // 0x10000100 ONO of Settings_PwrOn
//};

struct dbOcaObjectDef_Dy_Set_Config_PotiLevel : Ocp1CommandDefinition
{
    dbOcaObjectDef_Dy_Set_Config_PotiLevel(std::uint32_t channel, std::float_t value)
        : Ocp1CommandDefinition(GetONo(0x01, 0x00, channel, BoxAndObjNo::Config_PotiLevel), // ONO of Config_PotiLevel
                                4,                              // OcaGain level
                                2,                              // SetGain method
                                1,                              // 1 Param
                                DataFromFloat(value))           // Gain parameter
    {
    }
};

struct dbOcaObjectDef_Dy_Get_Config_PotiLevel : Ocp1CommandDefinition
{
    dbOcaObjectDef_Dy_Get_Config_PotiLevel(std::uint32_t channel)
        : Ocp1CommandDefinition(GetONo(0x01, 0x00, channel, BoxAndObjNo::Config_PotiLevel), // ONO of Config_PotiLevel
                                4,                              // OcaGain level
                                1,                              // GetGain method
                                0,                              // 0 Param
                                std::vector<std::uint8_t>())    // Empty parameters
    {
    }
};

struct dbOcaObjectDef_Dy_AddSubscription_Config_PotiLevel : Ocp1CommandDefinition
{
    dbOcaObjectDef_Dy_AddSubscription_Config_PotiLevel(std::uint32_t channel)
        : Ocp1CommandDefinition(0x00000004,                     // ONO of OcaSubscriptionManager
                                3,                              // OcaSubscriptionManager level
                                1,                              // AddSubscription method
                                5,                              // 5 Params 
                                DataFromOnoForSubscription(GetONo(0x01, 0x00, channel, BoxAndObjNo::Config_PotiLevel))) // ONO of Config_PotiLevel
    {
    }
};

struct dbOcaObjectDef_Dy_Set_Config_Mute : Ocp1CommandDefinition
{
    dbOcaObjectDef_Dy_Set_Config_Mute(std::uint32_t channel, std::uint16_t setting)
        : Ocp1CommandDefinition(GetONo(0x01, 0x00, channel, BoxAndObjNo::Config_Mute), // ONO of Config_Mute
                                4,                              // OcaMute level
                                2,                              // SetState method
                                1,                              // 1 Param
                                DataFromUint16(setting))        // Setting parameter
    {
    }
};

struct dbOcaObjectDef_Dy_Get_Config_Mute : Ocp1CommandDefinition
{
    dbOcaObjectDef_Dy_Get_Config_Mute(std::uint32_t channel)
        : Ocp1CommandDefinition(GetONo(0x01, 0x00, channel, BoxAndObjNo::Config_Mute), // ONO of Config_Mute
                                4,                              // OcaMute level
                                1,                              // GetState method
                                0,                              // 0 Param
                                std::vector<std::uint8_t>())    // Empty parameters
    {
    }
};

struct dbOcaObjectDef_Dy_AddSubscription_Config_Mute: Ocp1CommandDefinition
{
    dbOcaObjectDef_Dy_AddSubscription_Config_Mute(std::uint32_t channel)
        : Ocp1CommandDefinition(0x00000004,                     // ONO of OcaSubscriptionManager
                                3,                              // OcaSubscriptionManager level
                                1,                              // AddSubscription method
                                5,                              // 5 Params 
                                DataFromOnoForSubscription(GetONo(0x01, 0x00, channel, BoxAndObjNo::Config_Mute))) // ONO of Config_Mute
    {
    }
};


}