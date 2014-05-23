#include "Answer.h"
#include "MsgID.h"
#include "Package.h"
#include "GVar.h"
#include "Common/URandom.h"
#include "Server/SysMsg.h"
#include "GData/NewQuestionsTable.h"
#include "Common/Stream.h"
#include "GObject/World.h"

namespace GObject
{
    Answer* answerManager = new Answer();

    Answer::Answer()
    {
    }

    Answer::~Answer()
    {
    }

    void Answer::RandQuestions()
    {
        UInt8 div = MAX_QUESTION_NUM / MAX_ANSWER_NUM + 1;     
        for(UInt8 i=0; i<MAX_ANSWER_NUM; ++i)
        {
            UInt8 rand = uRand(div);
            m_answer[i] = rand + div * i + 1; 
            if(m_answer[i] > MAX_QUESTION_NUM)
                m_answer[i] = MAX_QUESTION_NUM;

            DB4().PushUpdateData("REPLACE INTO `questions` VALUES(%u, %u)", i+1, m_answer[i]);
        }
    }

    void Answer::AddQuestionsFromDB(UInt8 answerId, UInt16 questionsId)
    {
        if(0==answerId || answerId>MAX_ANSWER_NUM)
            return;

        if(0==questionsId || questionsId>MAX_QUESTION_NUM)
            return;

        m_answer[answerId-1] = questionsId;
    }

    void Answer::InitAnswerId(UInt8 answerId)
    {
        m_answerId = answerId;
    }

    void Answer::SendAnswerInfo(Player* pl)
    {
        GData::NewQuestionsData::newquestionsInfo * info = GData::newquestionsData.getNewQuestionsInfo(m_answer[m_answerId-1]);
        if(NULL == info)
            return;

        Stream st(REP::ACT);
        st << static_cast<UInt8>(0x32);
        st << static_cast<UInt8>(0x02);

        UInt32 nowTime = TimeUtil::Now();
        UInt32 endTime = 0;
        UInt32 awardTime = 0;
        if(World::getAnswerTime())
        {
            std::cout << "nowTime333 : " << nowTime << std::endl;
            if(GVAR.GetVar(GVAR_ANSWER_ENDTIME) > nowTime)
            {
                endTime = GVAR.GetVar(GVAR_ANSWER_ENDTIME) - nowTime;
                std::cout << "AnswerEnd111 : " << endTime << std::endl;
            }
            else
            {
                awardTime = GVAR.GetVar(GVAR_ANSWER_AWARDTIME) - nowTime;
                std::cout << "awardEnd222 : " << awardTime << std::endl;
            }
        }

        st << endTime;
        st << m_answerId;
        st << static_cast<UInt8>(info->answer);
        st << static_cast<UInt16>(info->id);
        UInt32 option = pl->GetVar(VAR_ANSWER_QUESTIONS_OPTION);
        UInt8 popt = 0;
        if(m_answerId == GET_BIT_5(option, 0))
            popt = GET_BIT_5(option, 1);
        st << popt;

        SelectNumInfo(st);

        UInt32 qStatus = pl->GetVar(VAR_ANSWER_QUESTIONS_STATUS);
        UInt8 ret = 0;
        if(1 == GET_BIT(qStatus, (m_answerId)))
        {
            if(0 == GET_BIT(qStatus, 0))
                ret = 1;
            else
                ret = 2;
        }
        st << static_cast<UInt8>(pl->GetVar(VAR_ANSWER_SUCCESSION_NUM));
        st << static_cast<UInt8>(pl->GetVar(VAR_ANSWER_ATTACK_NUM));
        st << awardTime;
        st << ret;

        UInt32 valueB = 0;
        UInt16 valueC = 0;
        UInt8 valueA = 0;
        UInt8 valueD = 0;
        std::map<UInt64, answerEnd>::iterator iterEnd = m_playerAnswerEnd.find(pl->getId());
        if(iterEnd != m_playerAnswerEnd.end())
        {
            valueB = iterEnd->second.valueB;
            valueC = iterEnd->second.valueC;
            valueA = iterEnd->second.valueA;
            valueD = iterEnd->second.valueD;
        }
        st << valueB;
        st << valueC;
        st << valueA;
        st << valueD;

        SkillAndLogInfo(pl, st);
        
        st << Stream::eos;
        pl->send(st);

        GameMsgHdr hdr(0x196, WORKER_THREAD_WORLD, pl, 0);
        GLOBAL().PushMsg(hdr, NULL);
    }
            
    void Answer::SelectNumInfo(Stream& st)
    {
        st << m_answerNum.retANum;
        std::cout << "NUMAAA: " << static_cast<UInt32>(m_answerNum.retANum) << std::endl;
        st << m_answerNum.retBNum;
        std::cout << "NUMBBB: " << static_cast<UInt32>(m_answerNum.retBNum) << std::endl;
        st << m_answerNum.retCNum;
        std::cout << "NUMCCC: " << static_cast<UInt32>(m_answerNum.retCNum) << std::endl;
        st << m_answerNum.retDNum;
        std::cout << "NUMDDD: " << static_cast<UInt32>(m_answerNum.retDNum) << std::endl;
    }

    void Answer::SkillAndLogInfo(Player* pl, Stream& st)
    {
        UInt8 countA = 0;
        UInt32 offsetA = 0;
        offsetA = st.size();
        st << countA;
        UInt32 skillStatus = pl->GetVar(VAR_ANSWER_SKILL_STATUS);
        for(UInt8 i=0; i<5; i++)
        {
            UInt8 id = GET_BIT_5(skillStatus, i);
            if(id == m_answerId)
            {
                st << static_cast<UInt8>(i+1);
                countA++;

                if(2 == i) // 有复苏之风技能，乌烟瘴气和颠倒乾坤失效
                    break;
            }
        }
        st.data<UInt8>(offsetA) = countA;

        st << static_cast<UInt8>(5);
        UInt32 skillNum = pl->GetVar(VAR_ANSWER_SKILL_USE_NUM);
        UInt32 useMark = pl->GetVar(VAR_ANSWER_SKILL_MARK);
        for(UInt8 i=0; i<5; i++)
        {
            UInt8 num = GET_BIT_2(skillNum, i);
            UInt8 mark = GET_BIT(useMark, i);
            st << static_cast<UInt8>(i+1);
            st << static_cast<UInt8>(3-num);
            st << static_cast<UInt8>(mark);
        }

        UInt8 countB = 0;
        UInt32 offsetB = 0;
        offsetB = st.size();
        st << countB;
        std::map<UInt64, std::list<answerLog>>::iterator iter = m_playerAnswerLog.find(pl->getId());
        if(iter != m_playerAnswerLog.end())
        {
            if(iter->second.size() > 10)
                countB = 10;
            else
                countB = iter->second.size();

            UInt8 countC = 0;
            std::list<answerLog>::iterator iterA = iter->second.begin();
            for(iterA; iterA!=iter->second.end(); ++iterA)
            {
                answerLog& log = (*iterA);
                if(log.logType>=1 && log.logType<=4)
                {
                    st << log.answerId;
                    st << log.logType;
                    if(1 == log.logType)
                        st << log.skillId;
                    else if(2==log.logType || 3==log.logType)
                        st << log.skillId << log.name.c_str();
                    else if(4 == log.logType)
                        st << log.value << log.ret;

                    countC++;
                    if(countC >= 10)
                        break;
                }
            }
        }
        st.data<UInt8>(offsetB) = countB;
    }

    void Answer::SelectAnswer(Player* pl, UInt8 opt)
    {
        if(NULL == pl)
            return;

        if(0 == opt || opt > 4)
            return;

        UInt32 status = pl->GetVar(VAR_ANSWER_QUESTIONS_STATUS);
        UInt8 mark = GET_BIT(status, m_answerId);
        if(1 == mark)
            return;

        switch(opt)
        {
        case 1:
            {
                m_answerNum.retANum++;
            }
            break;
        case 2:
            {
                m_answerNum.retBNum++;
            }
            break;
        case 3:
            {
                m_answerNum.retCNum++;
            }
            break;
        case 4:
            {
                m_answerNum.retDNum++;
            }
            break;
        }

        GData::NewQuestionsData::newquestionsInfo * info = GData::newquestionsData.getNewQuestionsInfo(m_answer[m_answerId-1]);
        if(NULL == info)
            return;

        if(opt == info->answer)
        {
            status = SET_BIT(status, 0);
            pl->AddVar(VAR_ANSWER_SUCCESSION_NUM, 1);
            UInt32 succorfail = pl->GetVar(VAR_ANSWER_QUESTIONS_SUCCORFAIL);
            succorfail = SET_BIT(succorfail, m_answerId);
            pl->SetVar(VAR_ANSWER_QUESTIONS_SUCCORFAIL, succorfail);
        }
        else
            pl->SetVar(VAR_ANSWER_SUCCESSION_NUM, 0);

        status = SET_BIT(status, m_answerId);
        pl->SetVar(VAR_ANSWER_QUESTIONS_STATUS, status);

        UInt32 option = pl->GetVar(VAR_ANSWER_QUESTIONS_OPTION);
        option = SET_BIT_5(option, 0, m_answerId);
        option = SET_BIT_5(option, 1, opt);
        pl->SetVar(VAR_ANSWER_QUESTIONS_OPTION, option);
        pl->udpLog("yzcm", "F_140523_1", "", "", "", "", "act");
    }

    void Answer::AnswerEnd(Player* pl)
    {
        if(NULL == pl)
            return;

        answerEnd answerEndInfo;
        UInt8 retA = 0;
        UInt32 status = pl->GetVar(VAR_ANSWER_QUESTIONS_STATUS);
        UInt8 mark = GET_BIT(status, m_answerId);
        if(1 == mark)
        {
            UInt8 awardA[5] = {8, 10, 12, 16, 20};
            UInt8 awardB[5] = {10, 20, 30, 40, 50};

            GData::NewQuestionsData::newquestionsInfo * info = GData::newquestionsData.getNewQuestionsInfo(m_answer[m_answerId-1]);
            if(NULL == info)
                return;
            
            UInt8 lvl = info->lvl;
            if(0==lvl || lvl>5)
                return;

            UInt8 ret = 0;
            UInt8 value = 0;
            UInt8 achValue = awardA[lvl-1];
            UInt32 pexp = 1000; 
            UInt8 plvl = pl->GetLev();
            UInt32 exp = (plvl - 10) * ((plvl > 99 ? 99 : plvl) / 10) * 5 + 25;
            exp = exp*60/5;

            if(1 == GET_BIT(status, 0))
            {
                value = awardB[lvl-1];
                status = CLR_BIT(status, 0);
                pl->SetVar(VAR_ANSWER_QUESTIONS_STATUS, status);
                UInt8 num = pl->GetVar(VAR_ANSWER_SUCCESSION_NUM);

                if(num>=3 && num<5)
                {
                    value += value * 0.1;             
                }
                else if(num>=5 && num<7)
                {
                    value += value * 0.2;
                    if(5 == num)
                        SYSMSG_BROADCASTV(5134, pl->getCountry(), pl->getName().c_str());
                }
                else if(num>=7 && num<10)
                {
                    value += value * 0.3;  
                }
                else if(num>=10 && num<15)
                {
                    value += value * 0.5;
                    if(10 == num)
                        SYSMSG_BROADCASTV(5135, pl->getCountry(), pl->getName().c_str());
                }
                else if(num>=15 && num<20)
                {
                    value += value;  
                    if(15 == num)
                        SYSMSG_BROADCASTV(5136, pl->getCountry(), pl->getName().c_str());
                }
                else if(num>=20 && num<=30)
                {
                    value += value;  
                    if(20 == num)
                        SYSMSG_BROADCASTV(5142, pl->getCountry(), pl->getName().c_str());
                }

                UInt32 skillStatus = pl->GetVar(VAR_ANSWER_SKILL_STATUS);
                UInt8 special = GET_BIT_5(skillStatus, 1);
                if(m_answerId == special) //幸运之星技能
                {
                    value += value;
                    exp += exp;
                    pexp += pexp;
                    achValue += achValue;
                }
                pl->AddVar(VAR_ANSWER_LITERARY_VALUE, value);
                SYSMSG_SENDV(5143, pl, value);
                answerEndInfo.valueA = value;
                ret = 1;
                retA = 2;
            }
            else
            {
                exp = exp / 2;
                pexp = pexp / 2;
                achValue = achValue / 2;
                retA = 1;
            }
            /*pl->AddExp(exp);
            pl->AddPExp(pexp);
            pl->getAchievement(achValue);*/
            struct Props
            {
                UInt32 aexp;
                UInt32 apexp;
                UInt32 aprestige;
                UInt32 ahonor;
            } props;
            memset(&props, 0, sizeof(Props));

            props.aexp = exp;
            props.apexp = pexp;
            props.ahonor = achValue;

            GameMsgHdr msg(0x321, pl->getThreadId(), pl, sizeof(props));
            GLOBAL().PushMsg(msg, &props);

            answerEndInfo.valueB = exp;
            answerEndInfo.valueC = pexp;
            answerEndInfo.valueD = achValue;
            m_playerAnswerEnd.insert(std::make_pair(pl->getId(), answerEndInfo));

            UInt32 score = pl->GetVar(VAR_ANSWER_LITERARY_VALUE);
            GameMsgHdr hdr(0x194, WORKER_THREAD_WORLD, pl, sizeof(score));
            GLOBAL().PushMsg(hdr, &score);

            answerLog log;
            log.logType = 4;
            log.answerId = m_answerId;
            log.value = value;
            log.ret = ret;
            std::map<UInt64, std::list<answerLog>>::iterator iter = m_playerAnswerLog.find(pl->getId());
            if(iter == m_playerAnswerLog.end())
            {
                std::list<answerLog> _answerLog;
                _answerLog.push_front(log);
                m_playerAnswerLog.insert(std::make_pair(pl->getId(), _answerLog));
            }
            else
                iter->second.push_front(log);
            
            
            Stream st0(REP::ACT);
            st0 << static_cast<UInt8>(0x32);
            st0 << static_cast<UInt8>(0x05);
            SkillAndLogInfo(pl, st0);
            st0 << Stream::eos;
            pl->send(st0);
        }
        else
        {
            pl->SetVar(VAR_ANSWER_SUCCESSION_NUM, 0);
        }

        Stream st(REP::ACT);
        st << static_cast<UInt8>(0x32);
        st << static_cast<UInt8>(0x06);
        st << static_cast<UInt32>(5);
        st << static_cast<UInt8>(retA);
        st << static_cast<UInt32>(answerEndInfo.valueB);
        st << static_cast<UInt16>(answerEndInfo.valueC);
        st << static_cast<UInt8>(answerEndInfo.valueA);
        st << static_cast<UInt8>(answerEndInfo.valueD);
        st << static_cast<UInt8>(pl->GetVar(VAR_ANSWER_SUCCESSION_NUM));
        st << Stream::eos;
        pl->send(st);
    }

    void Answer::AwardEnd(Player* pl)
    {
        UInt32 status = pl->GetVar(VAR_ANSWER_QUESTIONS_STATUS);
        status = CLR_BIT(status, 0);
        pl->SetVar(VAR_ANSWER_QUESTIONS_STATUS, status);
        pl->SetVar(VAR_ANSWER_SKILL_MARK, 0);

        SendAnswerInfo(pl);
    }

    void Answer::AwardEndClear()
    {
        if(m_answerId >= 30)
            return;

        m_answerId++;
        m_answerNum.retANum = 0;
        m_answerNum.retBNum = 0;
        m_answerNum.retCNum = 0;
        m_answerNum.retDNum = 0;
        if(m_playerAnswerEnd.size() > 0)
            m_playerAnswerEnd.clear();
    }

    void Answer::UseSkill(Player* pl, UInt8 skillId, UInt64 otherId)
    {
        if(NULL == pl)
            return;

        if(0==skillId || skillId>5)
            return;

        if(GVAR.GetVar(GVAR_ANSWER_ENDTIME) > GVAR.GetVar(GVAR_ANSWER_AWARDTIME)
            return;

        UInt32 status = pl->GetVar(VAR_ANSWER_SKILL_USE_NUM);
        UInt8 num = GET_BIT_2(status, (skillId-1));
        if(num>=3)
            return;

        UInt8 pos = skillId - 1;
        UInt32 useMark = pl->GetVar(VAR_ANSWER_SKILL_MARK);
        UInt8 mark = GET_BIT(useMark, pos);
        if(1 == mark)
            return;

        UInt8 id = m_answerId;
        UInt8 nextId = m_answerId + 1;

        UInt32 skillStatus = pl->GetVar(VAR_ANSWER_SKILL_STATUS);
        UInt8 qId = GET_BIT_5(skillStatus, pos);
        if(ANSWER_SKILL_A == skillId || ANSWER_SKILL_B == skillId)
        {
            if(id == qId)
                return;

            skillStatus = SET_BIT_5(skillStatus, pos, id);
            pl->SetVar(VAR_ANSWER_SKILL_STATUS, skillStatus);
        }
        else if(ANSWER_SKILL_C == skillId)
        {
            if(nextId == qId)
                return;

            skillStatus = SET_BIT_5(skillStatus, pos, nextId);
            pl->SetVar(VAR_ANSWER_SKILL_STATUS, skillStatus);
        }
        else
        {
            struct skilldata
            {
                UInt8 index;
                UInt8 nextQuestionId;
                UInt64 lastPlayerId;
            };
            skilldata sd = {pos, nextId, otherId};
            GameMsgHdr hdr(0x197, WORKER_THREAD_WORLD, pl, sizeof(skilldata));
            GLOBAL().PushMsg(hdr, &sd);
        }

        if(0 == num)
            status = SET_BIT_2(status, pos, 1);
        else if(1 == num)
            status = SET_BIT_2(status, pos, 2);
        else
            status = SET_BIT_2(status, pos, 3);
        pl->SetVar(VAR_ANSWER_SKILL_USE_NUM, status);

        useMark = SET_BIT(useMark, pos);
        pl->SetVar(VAR_ANSWER_SKILL_MARK, useMark);


        if(ANSWER_SKILL_A == skillId || ANSWER_SKILL_B == skillId || ANSWER_SKILL_C == skillId)
        {
            answerLog log;
            log.logType = 1;
            log.skillId = skillId;
            log.answerId = m_answerId;
            std::map<UInt64, std::list<answerLog>>::iterator iter = m_playerAnswerLog.find(pl->getId());
            if(iter == m_playerAnswerLog.end())
            {
                std::list<answerLog> _answerLog;
                _answerLog.push_front(log);
                m_playerAnswerLog.insert(std::make_pair(pl->getId(), _answerLog));
            }
            else
                iter->second.push_front(log);
        }
        else
        {
            Player* other = globalPlayers[otherId];
            if(NULL == other)
                return;

            std::cout << "nowTime666666666: " << otherId << std::endl;
            answerLog logA;
            logA.logType = 2;
            logA.skillId = skillId;
            logA.answerId = m_answerId;
            logA.name = other->getName().c_str();
            std::map<UInt64, std::list<answerLog>>::iterator iterA = m_playerAnswerLog.find(pl->getId());
            if(iterA == m_playerAnswerLog.end())
            {
                std::list<answerLog> _answerLogA;
                _answerLogA.push_front(logA);
                m_playerAnswerLog.insert(std::make_pair(pl->getId(), _answerLogA));
            }
            else
                iterA->second.push_front(logA);

            answerLog logB;
            logB.logType = 3;
            logB.skillId = skillId;
            logB.answerId = m_answerId;
            logB.name = pl->getName().c_str();
            std::map<UInt64, std::list<answerLog>>::iterator iterB = m_playerAnswerLog.find(other->getId());
            if(iterB == m_playerAnswerLog.end())
            {
                std::list<answerLog> _answerLogB;
                _answerLogB.push_front(logB);
                m_playerAnswerLog.insert(std::make_pair(other->getId(), _answerLogB));
            }
            else
                iterB->second.push_front(logB);

            Stream st0(REP::ACT);
            st0 << static_cast<UInt8>(0x32);
            st0 << static_cast<UInt8>(0x05);
            SkillAndLogInfo(other, st0);
            st0 << Stream::eos;
            other->send(st0);
        }

        Stream st(REP::ACT);
        st << static_cast<UInt8>(0x32);
        st << static_cast<UInt8>(0x05);
        SkillAndLogInfo(pl, st);
        st << Stream::eos;
        pl->send(st);
   }
}

