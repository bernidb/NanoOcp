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
typedef std::uint32_t BoxAndObjNo;

//==============================================================================
// Dy
//==============================================================================
namespace Dy
{
static constexpr BoxAndObjNo Settings_PwrOn         = 0x100;
static constexpr BoxAndObjNo Config_Mute            = 0x205;
static constexpr BoxAndObjNo Config_PotiLevel       = 0x206;
static constexpr BoxAndObjNo ChStatus_Isp           = 0x400;
static constexpr BoxAndObjNo ChStatus_Gr            = 0x401;
static constexpr BoxAndObjNo ChStatus_Ovl           = 0x402;

/**
 * Settings_PwrOn
 * Parameters for SetValueCommand: setting 1 == ON; 0 == OFF
 */
struct dbOcaObjectDef_Settings_PwrOn : Ocp1CommandDefinition
{
    dbOcaObjectDef_Settings_PwrOn() 
        : Ocp1CommandDefinition(GetONo(0x01, 0x00, 0x00, Settings_PwrOn), // ONO of Settings_PwrOn
                                OCP1DATATYPE_UINT8,             // Value type
                                4,                              // OcaSwitch level
                                1)                              // Prop_Setting
    {
    }
};

/**
 * Config_PotiLevel
 */
struct dbOcaObjectDef_Config_PotiLevel : Ocp1CommandDefinition
{
    dbOcaObjectDef_Config_PotiLevel(std::uint32_t channel)
        : Ocp1CommandDefinition(GetONo(0x01, 0x00, channel, Config_PotiLevel), // ONO of Config_PotiLevel
                                OCP1DATATYPE_FLOAT32,           // Value type
                                4,                              // OcaGain level
                                1)                              // Prop_Gain
    {
    }
};

/**
 * Config_Mute
 * Parameters for SetValueCommand: setting 1 == MUTE; 2 == UNMUTE
 */
struct dbOcaObjectDef_Config_Mute : Ocp1CommandDefinition
{
    dbOcaObjectDef_Config_Mute(std::uint32_t channel)
        : Ocp1CommandDefinition(GetONo(0x01, 0x00, channel, Config_Mute), // ONO of Config_Mute
                                OCP1DATATYPE_UINT8,             // Value type
                                4,                              // OcaMute level
                                1)                              // Prop_Setting
    {
    }
};

/**
 * ChStatus_Isp
 */
struct dbOcaObjectDef_ChStatus_Isp : Ocp1CommandDefinition
{
    dbOcaObjectDef_ChStatus_Isp(std::uint32_t channel)
        : Ocp1CommandDefinition(GetONo(0x01, 0x00, channel, ChStatus_Isp), // ONO of ChStatus_Isp
                                OCP1DATATYPE_BOOLEAN,           // Value type
                                5,                              // OcaBooleanSensor level
                                1)                              // Prop_Reading
    {
    }
};

/**
 * ChStatus_Gr
 */
struct dbOcaObjectDef_ChStatus_Gr : Ocp1CommandDefinition
{
    dbOcaObjectDef_ChStatus_Gr(std::uint32_t channel)
        : Ocp1CommandDefinition(GetONo(0x01, 0x00, channel, ChStatus_Gr), // ONO of ChStatus_Gr
                                OCP1DATATYPE_BOOLEAN,           // Value type
                                5,                              // OcaBooleanSensor level
                                1)                              // Prop_Reading
    {
    }
};

/**
 * ChStatus_Ovl
 */
struct dbOcaObjectDef_ChStatus_Ovl : Ocp1CommandDefinition
{
    dbOcaObjectDef_ChStatus_Ovl(std::uint32_t channel)
        : Ocp1CommandDefinition(GetONo(0x01, 0x00, channel, ChStatus_Ovl), // ONO of ChStatus_Ovl
                                OCP1DATATYPE_BOOLEAN,           // Value type
                                5,                              // OcaBooleanSensor level
                                1)                              // Prop_Reading
    {
    }
};

}


namespace Amp5D // NOTE: namespaces starting with numbers cause problems
{
static constexpr BoxAndObjNo ChStatus_Isp_5D        = 0x401; //ISP 5D
static constexpr BoxAndObjNo ChStatus_Gr_5D         = 0x402; //GR 5D
static constexpr BoxAndObjNo ChStatus_Ovl_5D        = 0x403; //OVL 5D

// TODO: define 5D objects. Copy paste? Or is there a more elegant way, 
// for example let the definitions above take in a GUID parameter and 
// internally the correct ONO is used depending on the GUID?

}

}