Req: 3b00010000001a0100010000001100000001100102250004000100
OcpHeader
    Sync Val        0x3b
    Proto Vers      0x00 0x01
    MsgSize         0x00 0x00 0x00 0x1a
    MsgType         0x01 // OcaCndRrq
    MsgCnt          0x00 0x01
OcpData
    CmdSize         0x00 0x00 0x00 0x11
    Handle          0x00 0x00 0x00 0x01
    TargetONo       0x10 0x01 0x02 0x25
    MethIdDefLev    0x00 0x04
    MethIdDefLev    0x00 0x01
    ParamCnt        0x00
    Data            -

Response1: 3b00010000001903000100000010000000010003000000000001
OcpHeader
    Sync Val        0x3b
    Proto Vers      0x00 0x01
    MsgSize         0x00 0x00 0x00 0x19
    MsgType         0x03 // OcaRsp
    MsgCnt          0x00 0x01
OcpData
    RspSize         0x00 0x00 0x00 0x10
    Handle          0x00 0x00 0x00 0x01
    Stat            0x00
    Resp Param Cnt  0x03
    Resp Param Data 0x00 0x00 0x00 0x00 0x00 0x01


Response Data Structure (Resp Param Data):
Bool Sensor
    Resp Param Cnt  0x01
    Resp Param Data 
        Value       0x00
Switch
    Resp Param Cnt  0x03
    Resp Param Data
        Value       0x00 0x00
        Min         0x00 0x00
        Max         0x00 0x01
Mute
    Resp Param Cnt  0x01
    Resp Param Data 
        Value       0x00
Gain
    Resp Param Cnt  0x03
    Resp Param Data 
        Value       0x00 0x00 0x00 0x00
        Min         0x00 0x00 0x00 0x00
        Max         0x00 0x00 0x00 0x00
String Actuator
    Resp Param Cnt  0x01
    Resp Param Data 
        SizeofStr   0x00 0x00
        String      -
String Sensor
    Resp Param Cnt  0x01
    Resp Param Data 
        SizeofStr   0x00 0x00
        String      -
Int16Sensor
    Resp Param Cnt  0x03
    Resp Param Data
        Value       0x00 0x00
        Min         0x00 0x00
        Max         0x00 0x00   
Int32Sensor
    Resp Param Cnt  0x03
    Resp Param Data
        Value       0x00 0x00 0x00 0x00
        Min         0x00 0x00 0x00 0x00
        Max         0x00 0x00 0x00 0x00   
Int64Sensor
    Resp Param Cnt  0x03
    Resp Param Data
        Value       0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00
        Min         0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00
        Max         0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00

d&b ONo scheme  TTTTRRRRRRRRCCCCCBBBBBBBNNNNNNNN    0xtr 0xrc 0xcb 0xnn
    Type (T), 4 bits
    Record number (R), 8 bits
    Channel number (C), 5 bits
    Box number (B), 7 bits
    Object number (N), 8 bits


mute:
echo '3b00010000001b01000100000012000006c410008205000400020101'|xxd -r -p|nc 192.168.1.124 50014
echo '3b00010000001b01000100000012000006c510010205000400020101'|xxd -r -p|nc 192.168.1.124 50014
echo '3b00010000001b010001000000120000000710018205000400020101'|xxd -r -p|nc 192.168.1.124 50014
echo '3b00010000001b010001000000120000000810020205000400020101'|xxd -r -p|nc 192.168.1.124 50014

pwrOff:
echo '3b00010000001c01000100000013000000021000010000040002010000'|xxd -r -p|nc 192.168.1.124 50014

pwrOn:
echo '3b00010000001c01000100000013000000021000010000040002010001'|xxd -r -p|nc 192.168.1.124 50014

unmute:
echo '3b00010000001b010001000000120000000510008205000400020102'|xxd -r -p|nc 192.168.1.124 50014
echo '3b00010000001b010001000000120000000610010205000400020102'|xxd -r -p|nc 192.168.1.124 50014
echo '3b00010000001b010001000000120000000710018205000400020102'|xxd -r -p|nc 192.168.1.124 50014
echo '3b00010000001b010001000000120000000810020205000400020102'|xxd -r -p|nc 192.168.1.124 50014 