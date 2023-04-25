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
static constexpr BoxAndObjNo ChStatus_ISP           = 0x400;
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
                                4)                              // OcaSwitch level
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
                                4)                              // OcaGain level
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
                                4)                              // OcaMute level
    {
    }
};

/**
 * ChStatus_ISP
 * Parameters for SetValueCommand: setting 1 == MUTE; 2 == UNMUTE
 */
struct dbOcaObjectDef_ChStatus_ISP : Ocp1CommandDefinition
{
    dbOcaObjectDef_ChStatus_ISP(std::uint32_t channel)
        : Ocp1CommandDefinition(GetONo(0x01, 0x00, channel, ChStatus_ISP), // ONO of Config_Mute
                                OCP1DATATYPE_BOOLEAN,           // Value type
                                5)                              // OcaBooleanSensor level
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