#include "MarryBoard.h"
#include "GObject/World.h"
#include "MsgID.h"
#include "Common/URandom.h"
#include "Server/SysMsg.h"
#include "Common/Itoa.h"
namespace GObject
{
    void* MarryBoard::_marryBoardTimer = NULL;
    UInt8 MarryBoard::answers[QuestionMax]={4 ,3 ,2 ,1 ,4 ,3 ,2 ,1 ,4 ,3 ,2 ,1 ,4 ,3 ,2 ,1 ,4 ,3 ,2 ,1 ,4 ,3 ,2 ,1 ,4 ,3 ,2 ,1 ,4 ,3 ,2 ,1 ,4 ,3 ,2 ,1 ,4 ,3 ,2 ,1 ,4 ,3 ,2 ,1 ,4 ,3 ,2 ,1 };
    inline bool player_enum_myarryBoard(GObject::Player * p, MarryBoard * marryBoard ,UInt8 type)
    {
        return marryBoard->sendMarryBoardInfo(p,type); 
    }
    inline bool player_enum_marryBoardAward(GObject::Player * p , MarryBoard * marryBoard) //结束发奖励
    {
        return false;
        UInt32 lively = p->GetVar(VAR_MARRYBOARD_LIVELY);
        SYSMSG(title, 4183);
        std::string strItems;
        std::string str;
        UInt8 count = 0 ;
        MailPackage::MailItem mitemall[20];
        MailItemsInfo itemsInfo(mitemall, Activity, 1);
        //仙缘石奖励
        {
            MailPackage::MailItem mitem0[] = {{9371, 10},{9371,5}};
            if(marryBoard->finder != NULL)
            {
                if( p == marryBoard->finder)
                {
                    strItems += Itoa(mitem0[0].id);
                    strItems += ",";
                    strItems += Itoa(mitem0[0].count);
                    strItems += "|";
                    SYSMSGV(buf,4188);
                    str += buf;
                    SYSMSGV(buf1,4189,mitem0[0].id,mitem0[0].count);
                    str +=buf1;
                    mitemall[count].id = mitem0[0].id;
                    mitemall[count++].count = mitem0[0].count;
                }
            }
            else  if(p == marryBoard->_man || p == marryBoard->_woman)
            {
                strItems += Itoa(mitem0[1].id);
                strItems += ",";
                strItems += Itoa(mitem0[1].count);
                strItems += "|";
                SYSMSGV(buf,4187);
                str += buf;
                {
                    SYSMSGV(buf,4188,mitem0[1].id,mitem0[1].count);
                    str +=buf;
                }
                mitemall[count].id = mitem0[1].id;
                mitemall[count++].count = mitem0[1].count;
            }
        }
        //个人积分奖励 
        {
            MailPackage::MailItem mitem1[] = {{9371, 3},{1126,3},{500,10},{501,10},{549,1},{509,5},{1528,10},{1325,10},{9076,20}};
            UInt32 Score[] = {100,300,600,1000,1000,1000,2000,2000,4000};
            if(lively >=100)
            {
                SYSMSGV(buf,4184);
                str += buf;
            }
            for(UInt8 k=0;k<9;++k)  
            {
                if(lively < Score[k])
                    break ;
                strItems += Itoa(mitem1[k].id);
                strItems += ",";
                strItems += Itoa(mitem1[k].count);
                strItems += "|";
                {
                    SYSMSGV(buf,4189,mitem1[k].id,mitem1[k].count);
                    str +=buf;
                }
                mitemall[count].id = mitem1[k].id;
                mitemall[count++].count = mitem1[k].count;
            }
        }
        //烟花积分奖励
        UInt32 YanHua = p->GetVar(VAR_MARRYBOARD_YANHUA);
        if(YanHua / 1000)
        {
            SYSMSGV(buf,4185);
            str += buf;
            MailPackage::MailItem mitem3[] = {{1325, 1},{503,1}};
            mitem3[0].count = YanHua/1000;
            mitem3[1].count = YanHua/1000;
            strItems += Itoa(mitem3[0].id);
            strItems += ",";
            strItems += Itoa(mitem3[0].count);
            strItems += "|";
            strItems += Itoa(mitem3[1].id);
            strItems += ",";
            strItems += Itoa(mitem3[1].count);
            strItems += "|";
            {
                SYSMSGV(buf,4189,mitem3[0].id,mitem3[0].count);
                str +=buf;
            }
            {
                SYSMSGV(buf,4189,mitem3[1].id,mitem3[1].count);
                str +=buf;
            }
            mitemall[count].id = mitem3[0].id;
            mitemall[count++].count = mitem3[0].count;
            mitemall[count].id = mitem3[1].id;
            mitemall[count++].count = mitem3[1].count;
        }

        //红包奖励
        UInt8 countR = 0;
        for(UInt8 i = 0 ; i < 8 ; i ++ )
        {
            if(marryBoard->_door[i] >= marryBoard->doorMax)
                countR ++ ;
        }
        if(countR)
        {
            MailPackage::MailItem mitemR[] ={ {9444, 1},{9445,1},{9446 ,1}};  //红包ID
            MailPackage::MailItem mitem2 = mitemR[marryBoard->_norms-1]; 
            mitem2.id = mitem2.id;
            mitem2.count = countR;
            strItems += Itoa(mitem2.id);
            strItems += ",";
            strItems += Itoa(mitem2.count);
            strItems += "|";
            {
                SYSMSGV(buf,4186,countR,mitem2.count,mitem2.id);
                str +=buf;
            }
            mitemall[count].id = mitem2.id;
            mitemall[count++].count = mitem2.count;
        }
        SYSMSGV(content, 4190,str.c_str());
        Mail * mail = p->GetMailBox()->newMail(NULL, 0x21, title, content, 0xFFFE0000);
        if(mail && count )
            mailPackageManager.push(mail->id, mitemall, count -1 , true);
        DBLOG1().PushUpdateData("insert into mailitem_histories(server_id, player_id, mail_id, mail_type, title, content_text, content_item, receive_time) values(%u, %" I64_FMT "u, %u, %u, '%s', '%s', '%s', %u)", cfg.serverLogId,p->getId(), mail->id, Activity, title, content, strItems.c_str(), mail->recvTime);
        return true;
    }
    bool MarryBoard::CreateMarry(Player * man , Player * woman ,UInt8 norms, UInt32 mTime)
    {
        return false;
        UInt32 now = TimeUtil::Now();
        if( mTime < now + 1800 )
        {
    //        return false;
        }
        if(TimeUtil::SharpDay(0,now) > TimeUtil::SharpDay(0,_atTime))
        {
            resetData();
        }
        if(woman->IsMale() || !man->IsMale())
            return false;
        if(_man!=NULL || _woman!=NULL ||_atTime!=0 || _norms!=0 ||_marryBoardTimer!=NULL)
            return false;
        _atTime = mTime;
        _man = man;
        _woman = woman;
        _norms = norms;
        //_marryBoardTimer =WORLD().AddTimer(2 * 1000, MarryBoard_Timer, static_cast<void*>(NULL));
        return true;
    }
    void MarryBoard::MarryBoard_Timer()
    {
        UInt32 now = TimeUtil::Now();
        std::cout<<static_cast<UInt32>(_type)<<std::endl;
   //     if(now  < _atTime - 1800)
     //      return ;
        if(now >= _atTime - 1800 && now < _atTime )
        {
            if(_type != 1)
            {
                GObject::globalOnlinePlayers.enumerate(player_enum_myarryBoard,this,1);
                _type =1 ;
            }
        }
        else if(now >= _atTime  && now < _atTime + OneTime )
        {
            if(_type != 2)
            {
                SetQuestionOnMarryBoard();
                _type =2 ;
                GObject::globalOnlinePlayers.enumerate(player_enum_myarryBoard,this,_type);
            }
            if((now - _atTime)/30 != _askNum )
            {
                _support[0]=0;
                _support[1]=0;
                _support[2]=0;
                _support[3]=0;
                GObject::globalOnlinePlayers.enumerate(player_enum_myarryBoard,this,5);  //答题结束
                _askNum = (now - _atTime)/30;
                if(_askNum > 9)
                    _askNum = 9;
            }
            GObject::globalOnlinePlayers.enumerate(player_enum_myarryBoard,this,8);  //更新支持率
        }
        else if(now >= _atTime + OneTime && now < _atTime + 2* OneTime )
        {
            if(_type != 3)
            {
                _type =3 ;
                GObject::globalOnlinePlayers.enumerate(player_enum_myarryBoard,this,3);
            }
        }
        else if(now >= _atTime +2 * OneTime && now < _atTime + 3 * OneTime)
        {
            if(_type != 4)
            {
                GObject::globalPlayers.enumerate(player_enum_myarryBoard,this,6);
                setDoorMax();
                _type =4 ;
                GObject::globalOnlinePlayers.enumerate(player_enum_myarryBoard,this,_type);
            }
            GObject::globalOnlinePlayers.enumerate(player_enum_myarryBoard,this,7);
        }
        else 
        {
            if(_type ==4 )
            {
                GObject::globalPlayers.enumerate(player_enum_marryBoardAward,this);
                _type = 0;
            }
            return ;
        }
        GObject::globalOnlinePlayers.enumerate(player_enum_myarryBoard,this,9);
    }
    bool MarryBoard::sendMarryBoardInfo(Player * pl , UInt8 type)
    {
        UInt32 now = TimeUtil::Now();
        Stream st(REP::MARRYBOARD);
        if(type == 0)
            type = _type;
        switch( type )
        {
            case 1:   //游街开始
                {
                    st << static_cast<UInt8>(0x01);
                    st <<_man->getId();
                    st <<_woman->getId();
                    st << static_cast<UInt32>(_atTime - now); 
                } 
                break;
            case 2:  //拜堂开始 
                { 
                    st <<static_cast<UInt8>(0x21);
                    st << _man->getId();
                    st << _woman->getId(); 
                    st << 300 - (_type == 2 ? now - _atTime: 0 ); 
                    st <<30 - (_type == 2 ? now - _atTime: 0 )%30;
                    st << _questionId[_askNum];
                    st << static_cast<UInt8>(_askNum+1); 
                    st << static_cast<UInt8>(pl->GetVar(VAR_MARRYBOARD2_ANS)); 
                    st << static_cast<UInt8>( pl->GetVar(VAR_MARRYBOARD3));
                    st <<_support[0];
                    st <<_support[1];
                    st <<_support[2];
                    st <<_support[3];
                    std::cout<<"2:askNum:"<<static_cast<UInt32>(_askNum)<<std::endl;
                    std::cout<<"2:question::"<<static_cast<UInt32>(_questionId[_askNum])<<std::endl;
                    std::cout<<"2:_answers::"<<static_cast<UInt32>(_answers[_askNum])<<std::endl;
                }
                break;
            case 5:    //每次答题结束的时候调用
                {
                    if(pl->GetVar(VAR_MARRYBOARD2_ANS) == _answers[_askNum] && _answers[_askNum]!=0 )
                    {
                        pl->AddVar(VAR_MARRYBOARD3,1);
                        pl->AddVar(VAR_MARRYBOARD_LIVELY,5);
                    }
                    st <<static_cast<UInt8>(0x25);
                    st << _answers[_askNum]; 
                    st << static_cast<UInt8>(_man->GetVar(VAR_MARRYBOARD2_ANS));
                    st << static_cast<UInt8>(_woman->GetVar(VAR_MARRYBOARD2_ANS));
                    st << _questionId[_askNum+1];
                    UInt32 rand = uRand(10000);
                    pl->SetVar(VAR_MARRYBOARD3_KEY,rand);
                    if(_askNum == 9)
                        pl->SetVar(VAR_MARRYBOARD3,0);
                    pl->SetVar(VAR_MARRYBOARD2_ANS,0);
                    std::cout<<"2:askNum:"<<static_cast<UInt32>(_askNum)<<std::endl;
                    std::cout<<"2:question::"<<static_cast<UInt32>(_questionId[_askNum])<<std::endl;
                    std::cout<<"2:_answers::"<<static_cast<UInt32>(_answers[_askNum])<<std::endl;
                }
                break;
            case 8:   //时刻更新支持率
                {
                    st << static_cast<UInt8>(0x22);
                    st <<_support[0];
                    st <<_support[1];
                    st <<_support[2];
                    st <<_support[3];
                }
                break;
            case 3:    //吃宴席开始
                {
                    if(_type != 3)
                        return false;
                    UInt32 MarryBoard3 = pl->GetVar(VAR_MARRYBOARD3);
                    pl->SetVar(VAR_MARRYBOARD3_KEY,100 + uRand(10000));
                    st <<static_cast<UInt8>(0x41);
                    st << _man->getId();
                    st << _woman->getId();
                    st << static_cast<UInt32>(_atTime + 2 * OneTime - now);            
                    st <<static_cast<UInt8>(MarryBoard3%31);
                    st <<static_cast<UInt8>(MarryBoard3/31);
                    st <<wrapTheKey(pl->GetVar(VAR_MARRYBOARD3_KEY)); 
                }
                break;
            case 4:    //闹洞房开始
                {
                    st <<static_cast<UInt8>(0x61);
                    st << _man->getId();
                    st << _woman->getId();
                    st << (_type == 4 ? _atTime + 3 * OneTime -now : 0) ;
                    for(UInt8 i =0 ;i < 8 ;++i)
                        st<<_door[i];
                    st << doorMax ;
                    UInt32 knock =pl->GetVar(VAR_MARRYBOARD4_TIME);
                    if(pl == _man || pl == _woman )
                    {
                        st << _rightDoor ;
                        if(_rightDoor)
                            st << static_cast<UInt32>(0);
                        else
                            st << (_atTime + 2 * OneTime + 30 - now);
                    }
                    else 
                       st <<static_cast<UInt32>( (now - knock) > 15?0:(knock+15-now) );
                    pl->SetVar(VAR_MARRYBOARD4_TIME,now-15);
                }
                break;
            case 7: //更新门的进度
                {
                    st << static_cast<UInt8>(0x62);
                    for(UInt8 i =0 ;i < 8 ;++i)
                        st<<_door[i];
                } 
                break;
            case 6:    //计算当前活跃人数
                {
                    if(pl->GetVar(VAR_MARRYBOARD_LIVELY)) 
                        plNum ++ ;
                }
                break;
            case 9:  //更新活跃度
                {
                    st << static_cast<UInt8>(0x02);
                    st << _lively ; 
                    st << pl->GetVar(VAR_MARRYBOARD_LIVELY);
                    st << _YHlively ; 
                }
                break;
        }
        if(type == 6)
            return true;
        st <<Stream::eos;
        pl->send(st); 
        return true;
    }
    void MarryBoard::answerTheQuestionOn2(Player* pl ,UInt8 ans)
    {
        if(_type != 2)
            return ;
        if(ans <1 || ans >4 )
            return ;
        UInt32 answerNum = pl->GetVar(VAR_MARRYBOARD2_NUM);
        if( answerNum & ( 1 << _askNum ) )
            return ;
        if(pl == _man || pl == _woman)
        {
            if(ans == _answers[_askNum])
                _lively += 100;
            else 
                _lively += 50;
        }
        else 
        {
            _support[ans-1]++;
        }

        answerNum |= (1 << _askNum );
        pl->SetVar(VAR_MARRYBOARD2_NUM,answerNum);
        pl->SetVar(VAR_MARRYBOARD2_ANS,ans);
    }
    void MarryBoard::selectDoor(Player * pl ,UInt8 door)
    {
        if(_type != 4)
            return ;
        if(door > 7 || pl == NULL )
            return ;
        UInt32 now = TimeUtil::Now();
        if( pl == _man || pl == _woman)
        {
            if(now > _atTime + 2* OneTime && now <= _atTime + 2*OneTime + 30 && _rightDoor == 0 ) 
            {
                _rightDoor = door ;  
            }
        }
        else
        {
            if(_rightDoor == 0)
                _rightDoor = uRand(7)+1;  //暂时不清楚是否能取到0或者8
            UInt32 knockTime = pl->GetVar(VAR_MARRYBOARD4_TIME);
            if(now > _atTime + 2 * OneTime  && now < _atTime + 3 * OneTime && now - knockTime > 15)
            {
                if(_door[door-1] >= doorMax)
                    return ;
                if(_door[door-1] < doorMax)
                    _door[door-1] ++ ; 
                if(_door[door-1] == doorMax && door == _rightDoor)
                    finder = pl;
            }
            pl->SetVar(VAR_MARRYBOARD4_TIME,now);
            pl->AddVar(VAR_MARRYBOARD_LIVELY,10);
            _lively += 1;
        }
    }
    UInt32 MarryBoard::wrapTheKey(UInt32 plKey)
    {
        return (plKey + 23) * 6 / 3 - 7;
    }
    UInt32 MarryBoard::unWrapTheOutKey(UInt32 key)
    {
        return ((key- 22) / 4 + 77); 
    }
    void MarryBoard::setDoorMax()
    {
        doorMax = plNum * 8 * 20 / 7;
    }
    void MarryBoard::resetData()
    {
        _man = NULL;
        _woman = NULL;
        _marryBoardTimer = NULL;
        _atTime = 0;
        _norms = 0;
        _type = 0;
        _lively = 0;
        _YHlively = 0;
        _askNum = 0;
        doorMax = 0;
        plNum = 0;
        for(UInt8 i =0; i < 4 ;++i )
            _support[i] = 0;
        for(UInt8 i =0 ; i < 8 ;++i )
            _door[i] = 0;
        _rightDoor = 0;
    }
    void MarryBoard::SetQuestionOnMarryBoard()
    {
        UInt8 div = QuestionMax/10+1;     
        for(UInt8 i = 0 ; i < 10 ; ++i)
        {
            UInt8 rand = uRand(div);
            _questionId[i]=rand + div*i +1 ; 
            if(_questionId[i] > QuestionMax)
                _questionId[i] = QuestionMax;
            _answers[i] = answers[_questionId[i]-1];
        }
    }
}
