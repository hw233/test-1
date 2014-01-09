#include "MarryBoard.h"
#include "GObject/World.h"
#include "MsgID.h"
#include "Common/URandom.h"
#include "Server/SysMsg.h"
#include "Common/Itoa.h"
#include "Marry.h"
namespace GObject
{
    void* MarryBoard::_marryBoardTimer = NULL;
    UInt8 MarryBoard::answers[QuestionMax]={4 ,3 ,2 ,1 ,4 ,3 ,2 ,1 ,4 ,3 ,2 ,1 ,4 ,3 ,2 ,1 ,4 ,3 ,2 ,1 ,4 ,3 ,2 ,1 ,4 ,3 ,2 ,1 ,4 ,3 ,2 ,1 ,4 ,3 ,2 ,1 ,4 ,3 ,2 ,1 ,4 ,3 ,2 ,1 ,4 ,3 ,2 ,1 };
    std::string MarryBoard::doorName[8]={"麒麟门","汀兰门","墨韵门","弄玉门","听雪门","清凉门","星辉门","邀月门"};
    inline bool player_enum_marryBoard(GObject::Player * p, MarryBoard * marryBoard ,UInt8 type)
    {
        return marryBoard->sendMarryBoardInfo(p,type); 
    }
    inline bool player_enum_marryBoardAward(GObject::Player * p , MarryBoard * marryBoard) //结束发奖励
    {
        UInt32 lively = p->GetVar(VAR_MARRYBOARD_LIVELY);
        if(!lively)
            return true;
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
                    SYSMSGV(buf,4187);
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
                SYSMSGV(buf,4188);
                str += buf;
                {
                    SYSMSGV(buf,4189,mitem0[1].id,mitem0[1].count);
                    str +=buf;
                }
                mitemall[count].id = mitem0[1].id;
                mitemall[count++].count = mitem0[1].count;
            }
        }
        //全服热闹值
        if(marryBoard->_lively > 1999)
        {
            MailPackage::MailItem mitemN[][3] ={ {{15,2},{133,2},{0,0}},{{15,3},{56,3},{33,3}},{{503,2},{134,2},{0,0}}};  //红包ID
            MailPackage::MailItem *mitemL = mitemN[marryBoard->_norms-1]; 
            SYSMSGV(buf,4193,marryBoard->_lively);
            str += buf;
            for(UInt8 k=0;k<3;++k)  
            {
                if(mitemL[k].id ==0)
                    continue ; 
                strItems += Itoa(mitemL[k].id);
                strItems += ",";
                strItems += Itoa(mitemL[k].count);
                strItems += "|";
                {
                    SYSMSGV(buf,4189,mitemL[k].id,mitemL[k].count);
                    str +=buf;
                }
                mitemall[count].id = mitemL[k].id;
                mitemall[count++].count = mitemL[k].count;
            }
        }
        //个人积分奖励 
        {
            MailPackage::MailItem mitem1[] = {{9371, 2},{1126,2},{500,10},{501,10},{549,1},{509,5},{134,12},{1325,12},{9076,15}};
            UInt32 Score[] = {100,300,600,600,600,1000,2000,2000,4000};
            if(lively >=100)
            {
                SYSMSGV(buf,4184,lively);
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
        UInt32 YanHua = marryBoard->_YHlively;
        if(YanHua / 1000)
        {
            std::string str1 ;
            SYSMSG(title1, 4196);
            SYSMSGV(content1,4185,YanHua);
            MailPackage::MailItem mitem3[] = {{1325, 1}};
            mitem3[0].count = YanHua/1000;
            strItems += Itoa(mitem3[0].id);
            strItems += ",";
            strItems += Itoa(mitem3[0].count);
            strItems += "|";
            if(YanHua/1000 >= 255 )
            {
                for(UInt8 i =0 ;i< YanHua/1000/255 ; ++i )
                {
                    mitem3[0].count  = 255 ;
                    Mail * mail1 = p->GetMailBox()->newMail(NULL, 0x21, title1, content1, 0xFFFE0000);
                    if(mail1)
                        mailPackageManager.push(mail1->id, mitem3, 1, true);
                }
            }
            if((YanHua/1000)%255 != 0)
            {
                mitem3[0].count  = (YanHua/1000)%255;
                Mail * mail1 = p->GetMailBox()->newMail(NULL, 0x21, title1, content1, 0xFFFE0000);
                if(mail1)
                    mailPackageManager.push(mail1->id, mitem3, 1, true);
            }
        }

        //红包奖励
        UInt8 countR = 0;
        for(UInt8 i = 0 ; i < 8 ; i ++ )
        {
            if(marryBoard->_door[i] >= marryBoard->doorMax)
            {
                countR ++ ;
            }
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
        if(count)
        {
            Mail * mail = p->GetMailBox()->newMail(NULL, 0x21, title, content, 0xFFFE0000);
            if(mail)
                mailPackageManager.push(mail->id, mitemall, count, true);
            DBLOG1().PushUpdateData("insert into mailitem_histories(server_id, player_id, mail_id, mail_type, title, content_text, content_item, receive_time) values(%u, %" I64_FMT "u, %u, %u, '%s', '%s', '%s', %u)", cfg.serverLogId,p->getId(), mail->id, Activity, title, content, strItems.c_str(), mail->recvTime);
        }
        return true;
    }
    bool MarryBoard::CreateMarry(Player * man , Player * woman ,UInt8 norms, UInt32 mTime)
    {
        UInt32 now = TimeUtil::Now();
        if(mTime == 0 )
        {
            resetData();
            mTime = now + 1800 + 600;
        }
        if( mTime < now + 1800 )
        {
            return false;
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
        //SendPreMarryPresent(man,woman,norms);
        //GObject::globalOnlinePlayers.enumerate(player_enum_marryBoard,this,11);
        GObject::globalPlayers.enumerate(player_enum_marryBoard,this,11);
        return true;
    }
    void MarryBoard::MarryBoard_Timer()
    {
        UInt32 now = TimeUtil::Now();
    //    std::cout<<static_cast<UInt32>(_type)<<std::endl;
        if(now  < _atTime - 1800)
           return ;
        if(now >= (_atTime - 1800) && now < _atTime )
        {
            if(_type != 1)
            {
                GObject::globalOnlinePlayers.enumerate(player_enum_marryBoard,this,1);
                _type =1 ;
            }
        }
        else if(now >= _atTime  && now < _atTime + OneTime  )
        {
            if(_type != 2)
            {
                SetQuestionOnMarryBoard();
                _type =2 ;
                GObject::globalOnlinePlayers.enumerate(player_enum_marryBoard,this,_type);
            }
            if((now - _atTime)/30 != _askNum )
            {
                CheckTheAnswerBoard2();
            }
            GObject::globalOnlinePlayers.enumerate(player_enum_marryBoard,this,8);  //更新支持率
        }
        else if(now >= _atTime + OneTime && now < _atTime + 2* OneTime )
        {
            if(_type != 3)
            {
                _type =3 ;
                CheckTheAnswerBoard2();
                GObject::globalOnlinePlayers.enumerate(player_enum_marryBoard,this,3);
            }
        }
        else if(now >= _atTime +2 * OneTime && now < _atTime + 3 * OneTime)
        {
            if(now > (_atTime +2 * OneTime +30) && _rightDoor == 0 )
            {
                _rightDoor = uRand(7)+1;  //暂时不清楚是否能取到0或者8
                sendMarryBoardInfo(_man,4);
                sendMarryBoardInfo(_woman,4);
            }
            if(_type != 4)
            {
                GObject::globalPlayers.enumerate(player_enum_marryBoard,this,6);
                setDoorMax();
                _type =4 ;
                GObject::globalOnlinePlayers.enumerate(player_enum_marryBoard,this,_type);
            }
            GObject::globalOnlinePlayers.enumerate(player_enum_marryBoard,this,7);
        }
        else 
        {
            if(_type ==4 )
            {
                GObject::globalPlayers.enumerate(player_enum_marryBoardAward,this);
                _type = 0;
                GObject::globalPlayers.enumerate(player_enum_marryBoard,this,10);
                gMarryMgr.FinishMarry(_man->getId(),_woman->getId());
                WORLD().RemoveTimer(_marryBoardTimer);
                _marryBoardTimer = NULL;
    //          WORLD().CreateMarryBoard(_man->getId(),_woman->getId(),_norms,0);
            }
            return ;
        }
        GObject::globalOnlinePlayers.enumerate(player_enum_marryBoard,this,9);
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
                    st <<_man->getName();
                    st <<_woman->getName();
                    st << static_cast<UInt32>(_atTime - now); 
                    st << static_cast<UInt8>(_norms); 
                } 
                break;
            case 2:  //拜堂开始 
                { 
                    st <<static_cast<UInt8>(0x21);
                    st << _man->getName();
                    st << _woman->getName(); 
                    st << 300 - (_type == 2 ? now - _atTime: 0 ); 
                    st << static_cast<UInt8>(_norms); 
                    st <<30 - (_type == 2 ? now - _atTime: 0 )%30;
                    st << _questionId[_askNum];
                    st << static_cast<UInt8>(_askNum+1); 
                    st << static_cast<UInt8>(pl->GetVar(VAR_MARRYBOARD2_ANS)); 
                    st << static_cast<UInt8>( pl->GetVar(VAR_MARRYBOARD3));
                    st <<_support[0];
                    st <<_support[1];
                    st <<_support[2];
                    st <<_support[3];
                  //  std::cout<<"2:askNum:"<<static_cast<UInt32>(_askNum)<<std::endl;
                  //  std::cout<<"2:question::"<<static_cast<UInt32>(_questionId[_askNum])<<std::endl;
                  //  std::cout<<"2:_answers::"<<static_cast<UInt32>(_answers[_askNum])<<std::endl;
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
                    st << _questionId[(_askNum+1)%10];
                    UInt32 rand = uRand(10000);
                    pl->SetVar(VAR_MARRYBOARD3_KEY,rand);
                    if(_askNum == 9)
                        pl->SetVar(VAR_MARRYBOARD3,0);
                    pl->SetVar(VAR_MARRYBOARD2_ANS,0);
                }
                break;
            case 8:   //时刻更新支持率
                {
                    st << static_cast<UInt8>(0x22);
                    st <<_support[0];
                    st <<_support[1];
                    st <<_support[2];
                    st <<_support[3];
                 //   std::cout<<"support"<<static_cast<UInt32>(1)<<":"<<_support[0]<<std::endl;
                 //   std::cout<<"support"<<static_cast<UInt32>(2)<<":"<<_support[1]<<std::endl;
                 //   std::cout<<"support"<<static_cast<UInt32>(3)<<":"<<_support[2]<<std::endl;
                 //   std::cout<<"support"<<static_cast<UInt32>(4)<<":"<<_support[3]<<std::endl;
                }
                break;
            case 3:    //吃宴席开始
                {
                    if(_type != 3)
                        return false;
                    UInt32 MarryBoard3 = pl->GetVar(VAR_MARRYBOARD3);
                    pl->SetVar(VAR_MARRYBOARD3_KEY,100 + uRand(10000));
                    st <<static_cast<UInt8>(0x41);
                    st << _man->getName();
                    st << _woman->getName();
                    st << static_cast<UInt32>(_atTime + 2 * OneTime - now);            
                    st << static_cast<UInt8>(_norms); 
                    st <<static_cast<UInt8>(MarryBoard3%31);
                    st <<static_cast<UInt8>(MarryBoard3/31);
                    st <<wrapTheKey(pl->GetVar(VAR_MARRYBOARD3_KEY)); 
                }
                break;
            case 4:    //闹洞房开始
                {
                    st <<static_cast<UInt8>(0x61);
                    st << _man->getName();
                    st << _woman->getName();
                    st << (_type == 4 ? _atTime + 3 * OneTime -now : 0) ;
                    st << static_cast<UInt8>(_norms); 
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
                            st << ((_atTime + 2 * OneTime + 30) > now? (_atTime + 2 * OneTime + 30 - now):0) ;
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
                    {
                        st<<_door[i];
                        //std::cout<<"door"<<static_cast<UInt32>(i)<<":"<<_door[i]<<std::endl;
                    }
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
            case 10:
                    st <<static_cast<UInt8>(0x71);
                break;
            case 11 :
    //            pl->SetVar(VAR_MARRYBOARD_LIVELY,0);   //内测区循环测试
                sendTodayMarryInfo(pl);
                break;
        }
        if(type == 6 || type ==11)
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
            if(!_mAnswer)
                _mAnswer= ans;
        }
        else 
        {
            _support[ans-1]++;
        }

        answerNum |= (1 << _askNum );
        pl->SetVar(VAR_MARRYBOARD2_NUM,answerNum);
        pl->SetVar(VAR_MARRYBOARD2_ANS,ans);
        char str[16] = {0};
        sprintf(str, "F_140102_14");
        pl->udpLog("jiehunjinxing", str, "", "", "", "", "act");
    }
    void MarryBoard::selectDoor(Player * pl ,UInt8 door)
    {
        if(_type != 4)
            return ;
        if(door < 1 ||door > 8 || pl == NULL )
            return ;
        UInt32 now = TimeUtil::Now();
        if( pl == _man || pl == _woman)
        {
            if(now > _atTime + 2* OneTime && now <= _atTime + 2*OneTime + 30 && _rightDoor == 0 ) 
            {
                _rightDoor = door ;  
                _man->AddVar(VAR_MARRYBOARD_LIVELY,100);
                _woman->AddVar(VAR_MARRYBOARD_LIVELY,100);
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

                if(_door[door-1] == doorMax )
                {
                    if(door == _rightDoor)
                    {
                        finder = pl;
                        _lively += 500;
                        SYSMSG_BROADCASTV(577,pl->getCountry(),pl->getName().c_str(),doorName[door-1].c_str());
                    }
                    char str[16] = {0};
                    sprintf(str, "F_140102_17");
                    pl->udpLog("jiehunjinxing", str, "", "", "", "", "act");
                }
            }
            pl->SetVar(VAR_MARRYBOARD4_TIME,now);
            pl->AddVar(VAR_MARRYBOARD_LIVELY,10);
            _lively += 1;
            char str[16] = {0};
            sprintf(str, "F_140102_16");
            pl->udpLog("jiehunjinxing", str, "", "", "", "", "act");
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
        doorMax = plNum * 20 / 7;
    //    doorMax = 3;    //测试用例
    }
    void MarryBoard::resetData()
    {
        _man = NULL;
        _woman = NULL;
        finder = NULL;
        WORLD().RemoveTimer(_marryBoardTimer);
        _marryBoardTimer = NULL;
        _atTime = 0;
        _norms = 0;
        _type = 0;
        _lively = 0;
        _YHlively = 0;
        _askNum = 0;
        _mAnswer = 0;
        doorMax = 0;
        _rightDoor= 0 ;
        plNum = 0;
        for(UInt8 i =0; i < 4 ;++i )
            _support[i] = 0;
        for(UInt8 i =0 ; i < 8 ;++i )
            _door[i] = 0;
        for(UInt8 i =0 ; i < 10 ;++i )
            _questionId[i] = 0;
        for(UInt8 i =0 ; i < 10 ;++i )
           _answers[i] = 0;
        GObject::globalPlayers.enumerate(player_enum_marryBoard,this,10);
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
    UInt8 MarryBoard::getTheMaxSupport()
    {
       UInt8 max = 0 ;
       UInt8 count =0;
       for(UInt8 i=0 ;i<4;++i)
       {
           if(_support[i] > max)
           {
                max = _support[i]; 
                count = i;
           }
       }
       return count+1;
    }
    void MarryBoard::SendPreMarryPresent(Player* man ,Player* woman ,UInt8 norms)
    {
        if(norms < 1 || norms > 3)
            return ;
        SYSMSG(title, 4194);
        SYSMSGV(content, 4195);
        std::string strItems;
        MailPackage::MailItem mitem0[][2] = {
            {{9442,1},{9443,9}},
            {{9442,3},{9443,19}},
            {{9442,10},{9443,59}}
        };
        Mail * mail = man->GetMailBox()->newMail(NULL, 0x21, title, content, 0xFFFE0000);
        if(mail)
            mailPackageManager.push(mail->id, mitem0[norms-1], 2, true);
        Mail * mail1 = woman->GetMailBox()->newMail(NULL, 0x21, title, content, 0xFFFE0000);
        if(mail1)
            mailPackageManager.push(mail1->id, mitem0[norms-1], 2, true);
        strItems += Itoa(mitem0[norms-1][0].id);
        strItems += ",";
        strItems += Itoa(mitem0[norms-1][0].count);
        strItems += "|";
        strItems += Itoa(mitem0[norms-1][1].id);
        strItems += ",";
        strItems += Itoa(mitem0[norms-1][1].count);
        strItems += "|";
        DBLOG1().PushUpdateData("insert into mailitem_histories(server_id, player_id, mail_id, mail_type, title, content_text, content_item, receive_time) values(%u, %" I64_FMT "u, %u, %u, '%s', '%s', '%s', %u)", cfg.serverLogId,man->getId(), mail->id, Activity, title, content, strItems.c_str(), mail->recvTime);
        DBLOG1().PushUpdateData("insert into mailitem_histories(server_id, player_id, mail_id, mail_type, title, content_text, content_item, receive_time) values(%u, %" I64_FMT "u, %u, %u, '%s', '%s', '%s', %u)", cfg.serverLogId,woman->getId(), mail1->id, Activity, title, content, strItems.c_str(), mail1->recvTime);
    }
    void MarryBoard::sendTodayMarryInfo(Player *pl)
    {
        UInt32 now = TimeUtil::Now();
        if(_man==NULL || _woman==NULL ||_atTime==0 || _norms==0 || now >( _atTime + 3*OneTime ))
            return ;
        if(now > (_atTime - 1800)&& now < (_atTime + 3*OneTime))
        {
            sendMarryBoardInfo(pl,0);
            return ;
        }
        Stream st(REP::MARRYBOARD);
        st <<static_cast<UInt8>(0x72);
        st << _man->getName();
        st << _woman->getName(); 
        st << static_cast<UInt32>(_atTime - 1800 - now);
        st << static_cast<UInt8>(_norms);
        st <<Stream::eos;
        pl->send(st);
    }
    void MarryBoard::CheckTheAnswerBoard2()
    {
        UInt32 now = TimeUtil::Now();
        if(!_mAnswer)
            _mAnswer = getTheMaxSupport();
        if(_mAnswer == _answers[_askNum] )
            _lively += 100;
        else
            _lively += 50;
        _support[0]=0;
        _support[1]=0;
        _support[2]=0;
        _support[3]=0;
        GObject::globalOnlinePlayers.enumerate(player_enum_marryBoard,this,5);  //答题结束
        _askNum = (now - _atTime)/30;
        _mAnswer = 0;
        if(_askNum > 9)
            _askNum = 9;
    }
}



