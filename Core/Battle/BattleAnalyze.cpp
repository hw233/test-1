#include "BattleAnalyze.h"
#include "Common/BinaryReader.h"
void BattleAnalyze::Analyze(Stream br)
{ 
    //BinaryReader br = BinaryReader(buf ,len);

    UInt32 cmd = 0;
    br >> cmd;
    UInt8 mapId1 = 0, mapId2 = 0;
    br >> mapId1 >> mapId2;
    std::cout << "地图ID :" << static_cast<UInt32>(mapId1) << " VS " << static_cast<UInt32>(mapId2) << std::endl;

    for(UInt8 index = 0; index < 2; ++index)
    {
        UInt16 fgtId1 = 0;
        br >> fgtId1; 
        UInt8 fgtCount1 = 0;
        br >> fgtCount1 ;
        std::cout << "武将编号："  << static_cast<UInt32>(fgtId1) << std::endl;
        for(UInt8 i = 0; i < fgtCount1 ; ++i)
        { 
            UInt8 fgtNum = 0;
            UInt16 x = 0;
            UInt16 y = 0;
            UInt16 hp = 0;
            br >> fgtNum >> x >> y >> hp;
            std::cout << "战斗编号: " << static_cast<UInt32>(fgtNum) << " 位置: " << static_cast<UInt32>(x) << " , " << static_cast<UInt32>(y) << " 血量 " << static_cast<UInt32>(hp) << std::endl;
        } 
    }

    UInt16 actCount = 0;
    br >> actCount;
    std::cout << "解析数量" << static_cast<UInt32>(actCount) << std::endl;
    for(UInt16 i = 0; i < actCount; ++i )
    { 
        UInt16 time = -1;
        UInt8 fgtId = -1;
        UInt8 type = -1;
        br >> time >> fgtId >> type;
        std::cout << "时间点：" << static_cast<UInt32>(time) << " 行动战将编号 ：" << static_cast<UInt32>(fgtId) <<" 行动类型 :"<< static_cast<UInt32>(type) << std::endl;
        switch(type)
        { 
            case 0:
                {
                    UInt16 y = 0;
                    br >> y;
                    std::cout << "骑兵行动 y 坐标：" << static_cast<UInt32>(y) << std::endl;
                }
                break;
            case 1:
                { 
                    UInt8 fgtId = 0;
                    UInt16 param = 0;
                    br >> fgtId >> param;
                    std::cout << "攻击战将：" << static_cast<UInt32>(fgtId) << "伤害:" << static_cast<UInt32>(param) << std::endl;
                } 
                break;
            case 2:
            case 3:
            case 4:
                {
                    UInt8 fgtId = 0;
                    br >> fgtId;
                    std::cout << "攻击战将：" << static_cast<UInt32>(fgtId) << std::endl;//<< "伤害:" << static_cast<UInt32>(param) << std::endl;
                }
                break;
            default:
                std::cout << "error type" << std::endl;
                break;
        } 
    } 
} 
