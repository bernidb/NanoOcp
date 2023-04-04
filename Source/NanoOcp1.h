/*
  ==============================================================================

    NanoOcp1.cpp
    Created: 04 March 2022 22:10:00pm
    Author:  Christian Ahrens

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>


namespace NanoOcp1
{

class Ocp1Header
{
public:
    std::vector<std::uint8_t> GetSerializedData() {
        std::vector<std::uint8_t> serializedData;

        serializedData.push_back(m_syncVal);
        serializedData.push_back(static_cast<std::uint8_t>(m_protoVers << 8));
        serializedData.push_back(static_cast<std::uint8_t>(m_protoVers));
        serializedData.push_back(static_cast<std::uint8_t>(m_msgSize << 24));
        serializedData.push_back(static_cast<std::uint8_t>(m_msgSize << 16));
        serializedData.push_back(static_cast<std::uint8_t>(m_msgSize << 8));
        serializedData.push_back(static_cast<std::uint8_t>(m_msgSize));
        serializedData.push_back(m_msgType);
        serializedData.push_back(static_cast<std::uint8_t>(m_msgCnt << 8));
        serializedData.push_back(static_cast<std::uint8_t>(m_msgCnt));

        return serializedData;
    };

    std::uint8_t                m_syncVal;      // 0x3b
    std::uint16_t               m_protoVers;    // 0x00 0x01
    std::uint32_t               m_msgSize;      // 0x00 0x00 0x00 0x1a
    std::uint8_t                m_msgType;      // 0x01 // Ocp1CndRrq
    std::uint16_t               m_msgCnt;       // 0x00 0x01
};

class Ocp1Request
{
public:
    std::vector<std::uint8_t> GetSerializedData() {
        std::vector<std::uint8_t> serializedData;

        serializedData.push_back(static_cast<std::uint8_t>(m_cmdSize << 24));
        serializedData.push_back(static_cast<std::uint8_t>(m_cmdSize << 16));
        serializedData.push_back(static_cast<std::uint8_t>(m_cmdSize << 8));
        serializedData.push_back(static_cast<std::uint8_t>(m_cmdSize));
        serializedData.push_back(static_cast<std::uint8_t>(m_handle << 24));
        serializedData.push_back(static_cast<std::uint8_t>(m_handle << 16));
        serializedData.push_back(static_cast<std::uint8_t>(m_handle << 8));
        serializedData.push_back(static_cast<std::uint8_t>(m_handle));
        serializedData.push_back(static_cast<std::uint8_t>(m_targetONo << 24));
        serializedData.push_back(static_cast<std::uint8_t>(m_targetONo << 16));
        serializedData.push_back(static_cast<std::uint8_t>(m_targetONo << 8));
        serializedData.push_back(static_cast<std::uint8_t>(m_targetONo));
        serializedData.push_back(static_cast<std::uint8_t>(m_methIdDefLev << 8));
        serializedData.push_back(static_cast<std::uint8_t>(m_methIdDefLev));
        serializedData.push_back(static_cast<std::uint8_t>(m_methIdMethIdx << 8));
        serializedData.push_back(static_cast<std::uint8_t>(m_methIdMethIdx));
        serializedData.push_back(m_paramCnt);
        serializedData.insert(serializedData.end(), m_data.begin(), m_data.end());

        return serializedData;
    };

    std::uint32_t               m_cmdSize;      // 0x00 0x00 0x00 0x11
    std::uint32_t               m_handle;       // 0x00 0x00 0x00 0x01
    std::uint32_t               m_targetONo;    // 0x10 0x01 0x02 0x25
    std::uint16_t               m_methIdDefLev; // 0x00 0x04
    std::uint16_t               m_methIdMethIdx;// 0x00 0x01
    std::uint8_t                m_paramCnt;     // 0x00
    std::vector<std::uint8_t>   m_data;
};

class Ocp1ResponseBaseData
{
public:
    std::vector<std::uint8_t> GetSerializedData() {
        std::vector<std::uint8_t> serializedData;



        return serializedData;
    };

    std::uint8_t                m_respParamCnt;  // 0x03
    std::vector<std::uint8_t>   m_respParamData; // 0x00 0x00 0x00 0x00 0x00 0x01
};

class Ocp1BooleanSensorResponseData : public Ocp1ResponseBaseData
{
    // Resp Param Cnt  0x01
    // Resp Param Data
    // Value       0x00
};

class Ocp1SwitchResponseData : public Ocp1ResponseBaseData
{
    // Resp Param Cnt  0x03
    // Resp Param Data
    // Value       0x00 0x00
    // Min         0x00 0x00
    // Max         0x00 0x01

};

class Ocp1MuteResponseData : public Ocp1ResponseBaseData
{
    // Resp Param Cnt  0x01
    // Resp Param Data
    // Value       0x00
};

class Ocp1GainResponseData : public Ocp1ResponseBaseData
{
    // Resp Param Cnt  0x03
    // Resp Param Data
    // Value       0x00 0x00 0x00 0x00
    // Min         0x00 0x00 0x00 0x00
    // Max         0x00 0x00 0x00 0x00
};

class Ocp1StringActuatorResponseData : public Ocp1ResponseBaseData
{
    // Resp Param Cnt  0x01
    // Resp Param Data
    // SizeofStr   0x00 0x00
    // String -
};
 
class Ocp1StringSensorResponseData : public Ocp1ResponseBaseData
{
    // Resp Param Cnt  0x01
    // Resp Param Data
    // SizeofStr   0x00 0x00
    // String -
};

class Ocp1Int16SensorResponseData : public Ocp1ResponseBaseData
{
    // Resp Param Cnt  0x03
    // Resp Param Data
    // Value       0x00 0x00
    // Min         0x00 0x00
    // Max         0x00 0x00
};

class Ocp1Int32SensorResponseData : public Ocp1ResponseBaseData
{
    // Resp Param Cnt  0x03
    // Resp Param Data
    // Value       0x00 0x00 0x00 0x00
    // Min         0x00 0x00 0x00 0x00
    // Max         0x00 0x00 0x00 0x00
};

class Ocp1Int64SensorResponseData : public Ocp1ResponseBaseData
{
    // Resp Param Cnt  0x03
    // Resp Param Data
    // Value       0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00
    // Min         0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00
    // Max         0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00
};

class Ocp1Response
{
public:
    std::uint32_t           m_rspSize;  // 0x00 0x00 0x00 0x10
    std::uint32_t           m_handle;   // 0x00 0x00 0x00 0x01
    std::uint8_t            m_stat;     // 0x00
    Ocp1ResponseBaseData    m_respData; // 0x00 0x00 0x00 0x00 0x00 0x01
};

class NanoOcp1Base
{
public:
	enum DS100MatrixInputObjectNumber
	{
		MatrixInputBox = 0x00, // MatrixInput_Box
		MatrixInputMute = 0x01, // MatrixInput_Mute
		MatrixInputGain = 0x02, // MatrixInput_Gain
        MatrixInputDelay = 0x03, // MatrixInput_Delay
        MatrixInputDelayEnable = 0x04, // MatrixInput_DelayEnable
        MatrixInputEqEnable = 0x05, // MatrixInput_EqEnable
        MatrixInputPolarity = 0x06, // MatrixInput_Polarity
		MatrixInputChannelName = 0x07, // MatrixInput_ChannelName
		MatrixInputLevelMeterIn = 0x08, // MatrixInput_LevelMeterIn
		MatrixInputLevelMeterPreMute = 0x09, // MatrixInput_LevelMeterPreMute
		MatrixInputLevelMeterPostMute = 0x0A, // MatrixInput_LevelMeterPostMute
		MatrixInputReverbSendGain = 0x0D, // MatrixInput_ReverbSendGain
	};
	enum DS100MatrixOutputObjectNumber
	{
		MatrixOutputBox = 0x00, // MatrixOutput_Box
		MatrixOutputMute = 0x01, // MatrixOutput_Mute
		MatrixOutputGain = 0x02, // MatrixOutput_Gain
		MatrixOutputDelay = 0x03, // MatrixOutput_Delay
		MatrixOutputDelayEnable = 0x04, // MatrixOutput_DelayEnable
		MatrixOutputEqEnable = 0x05, // MatrixOutput_EqEnable
		MatrixOutputPolarity = 0x06, // MatrixOutput_Polarity
		MatrixOutputChannelName = 0x07, // MatrixOutput_ChannelName
		MatrixOutputLevelMeterIn = 0x08, // MatrixOutput_LevelMeterIn
		MatrixOutputLevelMeterPreMute = 0x09, // MatrixOutput_LevelMeterPreMute
		MatrixOutputLevelMeterPostMute = 0x0A, // MatrixOutput_LevelMeterPostMute
		MatrixOutputOSP = 0x0B, // MatrixOutput_OSP
		MatrixOutputReverbSelect = 0x0D, // MatrixOutput_ReverbSelect
		MatrixOutputReverbMute = 0x0E, // MatrixOutput_ReverbMute
		MatrixOutputReverbGain = 0x0F, // MatrixOutput_ReverbGain
		MatrixOutputReverbChannelName = 0x10, // MatrixOutput_ReverbChannelName
		MatrixOutputReverbLevelMeter = 0x11, // MatrixOutput_ReverbLevelMeter
	};
    enum DS100PositioningObjectNumber
    {
		PositioningBox = 0x00, // Positioning_Box
		PositioningPosition = 0x02, // Positioning_Source_Position
		PositioningEnable = 0x03, // Positioning_Source_Enable
		PositioningSpread = 0x04, // Positioning_Source_Spread
		PositioningSpeakerGroup = 0x06, // Positioning_Speaker_Group
		PositioningSpeakerPosition = 0x07, // Positioning_Speaker_Position
		PositioningPositionX = 0x08, // Positioning_Source_Position_X
		PositioningPositionY = 0x09, // Positioning_Source_Position_Y
		PositioningPositionZ = 0x0A, // Positioning_Source_Position_Z
		PositioningDelayMode = 0x0B, // Positioning_Source_DelayMode
    };
    enum DS100FunctionGroupObjectNumber
    {
		FunctionGroupBox = 0x00, // FunctionGroup_Box
		FunctionGroupName = 0x01, // FunctionGroup_Name
		FunctionGroupDelay = 0x02, // FunctionGroup_Delay
		FunctionGroupMode = 0x03, // FunctionGroup_Mode
		FunctionGroupModelName = 0x04, // FunctionGroup_ModeName
		FunctionGroupSpeakerCount = 0x05, // FunctionGroup_SpeakerCount
		FunctionGroupSpreadFactor = 0x06, // FunctionGroup_SpreadFactor
	};
    enum DS100ScenesObjectNumber
    {
        ScenesBox = 0x00, // Scenes_Box
        ScenesSceneIndex = 0x01, // Scene_SceneIndex
        ScenesSceneIndexNum = 0x02, // Scene_SceneIndexNum
        ScenesSceneName = 0x03, // Scene_SceneName
        ScenesSceneComment = 0x04, // Scene_SceneComment
    };
    enum DS100CoordinateMappingObjectNumber
    {
        CoordinateMappingBox = 0x00, // CoordinateMapping_Box
        CoordinateMappingSourcePosition = 0x01, // CoordinateMapping_Source_Position
    };

    // DS100 Box numbers
    static constexpr std::uint16_t DS100FixedBox { 0 };
    static constexpr std::uint16_t DS100MatrixInputBox { 5 };
    static constexpr std::uint16_t DS100MatrixOutputBox { 8 };
    static constexpr std::uint16_t DS100PositioningBox { 13 };
    static constexpr std::uint16_t DS100FunctionGroupBox { 14 };
    static constexpr std::uint16_t DS100CoordinateMappingBox { 22 };
    static constexpr std::uint16_t DS100ScenesBox { 23 };

public:
    //==============================================================================
    NanoOcp1Base(const juce::String& address, const int port);
	~NanoOcp1Base();

    void setAddress(const juce::String& address);
    const juce::String& getAddress();

    void setPort(const int port);
    const int getPort();

    //==============================================================================
    virtual bool start() = 0;
    virtual bool stop() = 0;

    //==============================================================================
    virtual bool sendData(const juce::MemoryBlock& data) = 0;

    //==============================================================================
    std::function<bool(const juce::MemoryBlock&)> onDataReceived;
    std::function<void()> onConnectionEstablished;
    std::function<void()> onConnectionLost;

    //==============================================================================
    static std::uint32_t GetDBONo(std::uint32_t type, std::uint32_t record, std::uint32_t channel, std::uint32_t boxAndObjectNumber);
    static std::uint32_t GetDBONo2(std::uint32_t type, std::uint32_t record, std::uint32_t channel, std::uint32_t boxNumber, std::uint32_t objectNumber);

protected:
    //==============================================================================
    bool processReceivedData(const juce::MemoryBlock& data);

private:
    //==============================================================================
    juce::String    m_address;
    int             m_port{ 0 };

};

class NanoOcp1Client : public NanoOcp1Base, public juce::InterprocessConnection, public juce::Timer
{
public:
    //==============================================================================
    NanoOcp1Client();
    NanoOcp1Client(const juce::String& address, const int port);
    ~NanoOcp1Client();

    //==============================================================================
    bool start();
    bool stop();

    //==============================================================================
    bool sendData(const juce::MemoryBlock& data) override;

    //==============================================================================
    void connectionMade() override;
    void connectionLost() override;
    void messageReceived(const juce::MemoryBlock& message) override;

protected:
    //==============================================================================
    void timerCallback() override;

private:
    //==============================================================================
    bool m_started{ false };
};

class NanoOcp1Server : public NanoOcp1Base, public juce::InterprocessConnectionServer
{
public:
    //==============================================================================
    NanoOcp1Server();
    NanoOcp1Server(const juce::String& address, const int port);
    ~NanoOcp1Server();

    //==============================================================================
    bool start();
    bool stop();

    //==============================================================================
    bool sendData(const MemoryBlock& data) override;

protected:
    //==============================================================================
    virtual InterprocessConnection* createConnectionObject() override;

private:
    //==============================================================================

    std::unique_ptr<NanoOcp1Client> m_activeConnection;
};

}
