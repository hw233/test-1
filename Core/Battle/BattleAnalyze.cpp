#include "BattleAnalyze.h"
#include "Common/BinaryReader.h"
void BattleAnalyze::Analyze(Stream br)
{ 
    //BinaryReader br = BinaryReader(buf ,len);

    UInt32 cmd = 0;
    br >> cmd;
    UInt8 distance = 0;
    UInt8 mapId1 = 0, mapId2 = 0;
    br >> distance >> mapId1 >> mapId2;
    std::cout << "攻击距离"<< static_cast<UInt32>(distance) << "地图ID :" << static_cast<UInt32>(mapId1) << " VS " << static_cast<UInt32>(mapId2) << std::endl;

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
                    br >> fgtId; 
                    UInt8 type = 0;
                    br >> type;
                    br>> param;
                    std::cout << "普通AT战将：" << static_cast<UInt32>(fgtId) << "伤害:" << static_cast<UInt32>(param) << std::endl;
                } 
                break;
            case 2:
                {
                    UInt16 skillId = 0;
                    UInt8 count = 0;
                    br >> skillId >> count;
                    std::cout << "技能Id：" << static_cast<UInt32>(skillId) << " 目标数量: " << static_cast<UInt32>(count) << std::endl;
                    for(UInt8 i = 0; i < count; ++i)
                    {
                        UInt8 type = 0;
                        UInt8 fgtId = 0;
                        UInt16 param = 0;
                        br >> type >> fgtId ;
                        UInt8 paramType = 0;
                        br >> paramType;
                        br>> param;
                        std::cout << "类型：" << static_cast<UInt32>(type);
                        std::cout << "受影响战将编号：" << static_cast<UInt32>(fgtId);
                        std::cout << "数值：" << static_cast<UInt32>(param);
                        std::cout << std::endl;
                    }
                    break;
                }
            case 3:
                {
                    UInt16 skillId = 0;
                    br >> skillId;
                    UInt8 count = 0;
                    br >> count;
                    for(UInt8 i =0; i< count; ++i)
                    { 
                        UInt16 time = 0;
                        UInt8 fgtBSNumber = 0;
                        UInt16 param = 0;
                        br >> time >> fgtBSNumber;
                        UInt8 type = 0;
                        br >> type;
                        br>> param;
                        std::cout<< "伤害产生时间点" << static_cast<UInt32>(time) << "攻击战将：" << static_cast<UInt32>(fgtBSNumber) << "伤害:" << static_cast<UInt32>(param) << std::endl;

                    } 
                }
                break;
            case 4:
                {
                    UInt8 fgtId = 0;
                    br >> fgtId;
                    std::cout << "攻击战将：" << static_cast<UInt32>(fgtId) << std::endl;//<< "伤害:" << static_cast<UInt32>(param) << std::endl;
                }
                break;

            case 5:
                { 
                    UInt8 fgtBSNumber = 0;
                    UInt16 param = 0;
                    br >> fgtBSNumber;
                    UInt8 type = 0;
                    br >> type;
                    br>> param;
                    std::cout << "弓箭手攻击战将：" << static_cast<UInt32>(fgtBSNumber) <<  "伤害:" << static_cast<UInt32>(param) << std::endl;
                } 
                break;
            default:
                std::cout << "error type" << std::endl;
                break;
        } 
    } 
} 
