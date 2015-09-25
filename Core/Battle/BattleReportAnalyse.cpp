#include"BattleReportAnalyse.h"
using namespace std;
void ReportAnalyse::AnalyseReport(Stream st)
{
    if( st.size() == 0 )
        return;
    UInt32 protoId = 0;
    st>> protoId; 
    std::cout<< " the protoId is " << static_cast<UInt32>(protoId)<<std::endl;

    UInt8 mapId = 0;
    st>> mapId;
    cout<<" the map is "<<static_cast<UInt32>(mapId) <<std::endl;

    UInt8 forceNum = 0;
    st>> forceNum;
    cout<<" the total force count is "<< static_cast<UInt32>(forceNum)<<std::endl;

    for( UInt8 index = 0; index < forceNum ; ++index )
    {
        UInt8 forceId = 0;
        st>>forceId;
        cout<<" the forceId is "<<static_cast<UInt32>(forceId)<<std::endl;
        UInt8 heroNum = 0;
        st>>heroNum;
        cout<<" the hero num is "<<static_cast<UInt32>(heroNum)<<std::endl;
        for( UInt8 i=0 ; i < heroNum ; ++i )
        {
            UInt8 id = 0;
            UInt16 fighterId=0;
            UInt8  x = 0;
            UInt8  y = 0;
            UInt8 soldierNum = 0;
            UInt16 power = 0;
            st>>id;
            st>>fighterId;
            st>>x;
            st>>y;
            st>>soldierNum;
            st>>power;
            cout<<" id " <<  static_cast<UInt32>(id)<<"  fighterId  " <<  static_cast<UInt32>(fighterId)<<"  x : " << static_cast<UInt32>(x)<<"y : " << static_cast<UInt32>(y)<<" the solider num  " << static_cast<UInt32>(soldierNum)<<" power " <<static_cast<UInt32>(power)<<std::endl;
        }
    }

    //行动
    UInt16 actionNum  = 0;
    st>>actionNum;
    cout<<" the total act Num " << static_cast<UInt32>(actionNum)<<std::endl;
    for( UInt16 j = 0 ; j < actionNum ; ++j )
    {
        UInt8 actId = 0;
        UInt8 moveI = 0;
        UInt8 moveJ = 0;
        UInt8 time  = 0;
        UInt8 battle = 0;

        st>>actId>>moveI>>moveJ>>time>>battle;

        cout<< " the actId  is "<<static_cast<UInt32>(actId) <<" moveI " << static_cast<UInt32>(moveI) << " moveJ " << static_cast<UInt32>(moveJ) << " cost time " << static_cast<UInt32>(time) << " battle " << static_cast<UInt32>(battle)<<std::endl;

        if( battle == 1 )
        {
            cout<<" 发生战斗" <<std::endl;
            UInt8 effectId = 0;
            UInt8 res = 0;
            UInt32 battleId = 0;
            st>>effectId >> res >> battleId;
            cout<<" 受击战将id " << static_cast<UInt32>(effectId) <<" 战斗结果 " << static_cast<UInt32>(res)<< " 战报Id " << static_cast<UInt32>(battleId)<<std::endl;
        }
    }

    UInt32 actTime = 0;
    st>>actTime;
    cout<<" 战报开始时间 " <<static_cast<UInt32>(actTime)<<std::endl;

}
