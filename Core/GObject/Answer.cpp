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

            DB5().PushUpdateData("INSERT INTO `questions` (`answerId`, `questionsId`) VALUES ( %u, %u)", i+1, m_answer[i]);
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
        GData::NewQuestionsData::newquestionsInfo * info = GData::newquestionsData.getNewQuestionsInfo(m_answer[m_answerId]);
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
            if(GVAR.GetVar(GVAR_ANSWER_ENDTIME) > nowTime)
                endTime = GVAR.GetVar(GVAR_ANSWER_ENDTIME) - nowTime;
            else
                awardTime = GVAR.GetVar(GVAR_ANSWER_AWARDTIME) - nowTime;
        }

        st << endTime;
        st << m_answerId;
        st << static_cast<UInt8>(info->answer);
        st << m_answerNum.retANum;
        st << m_answerNum.retBNum;
        st << m_answerNum.retCNum;
        st << m_answerNum.retDNum;

        UInt32 qStatus = pl->GetVar(VAR_ANSWER_QUESTIONS_STATUS);
        UInt8 ret = 0;
        if(1 == GET_BIT(qStatus, (m_answerId)))
        {
            if(0 == GET_BIT(qStatus, 0))
                ret = 1;
            else
                ret = 2;
        }
        st << qStatus; 
        st << static_cast<UInt8>(pl->GetVar(VAR_ANSWER_SUCCESSION_NUM));
        st << static_cast<UInt8>(pl->GetVar(VAR_ANSWER_ATTACK_NUM));
        st << awardTime;
        st << ret;
        st << static_cast<UInt32>(m_answerEnd.valueB);
        st << static_cast<UInt16>(m_answerEnd.valueC);
        st << static_cast<UInt8>(m_answerEnd.valueA);
        st << static_cast<UInt8>(m_answerEnd.valueD);

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
            }
        }

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

        std::map<UInt64, std::list<answerLog>>::iterator iter = m_playerAnswerLog.find(pl->getId());
        if(iter != m_playerAnswerLog.end())
        {
            UInt8 countB = 0;
            if(iter->second.size() > 10)
                countB = 10;
            else
                countB = iter->second.size();
            st << countB;

            UInt8 countC = 0;
            std::list<answerLog>::iterator iterA = iter->second.begin();
            for(iterA; iterA != iter->second.end()&&countC<10; ++iterA)
            {
                answerLog& log = (*iterA);
                if(1 == log.logType)
                    st << log.logType << log.skillId;
                if(2 == log.logType || 3 == log.logType)
                    st << log.logType << log.skillId << log.name;
                else if(4 == log.logType)
                    st << log.answerId << log.value << log.ret;

                countC++;
            }
        }

        st << Stream::eos;
        pl->send(st);
    }
            
    void Answer::SelectAnswer(Player* pl, UInt8 opt)
    {
        if(NULL == pl)
            return;

        if(0 == opt || opt > 4)
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

        GData::NewQuestionsData::newquestionsInfo * info = GData::newquestionsData.getNewQuestionsInfo(m_answer[m_answerId]);
        if(NULL == info)
            return;

        UInt32 status = pl->GetVar(VAR_ANSWER_QUESTIONS_STATUS);
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
    }

    void Answer::AnswerEnd(Player* pl)
    {
        if(NULL == pl)
            return;

        UInt32 status = pl->GetVar(VAR_ANSWER_QUESTIONS_STATUS);
        UInt8 mark = GET_BIT(status, m_answerId);
        if(1 == mark)
        {
            UInt8 awardA[5] = {8, 10, 12, 16, 20};
            UInt8 awardB[5] = {10, 20, 30, 40, 50};

            GData::NewQuestionsData::newquestionsInfo * info = GData::newquestionsData.getNewQuestionsInfo(m_answer[m_answerId]);
            if(NULL == info)
                return;
            
            UInt8 lvl = info->lvl;
            if(0==lvl || lvl>5)
                return;

            UInt8 ret = 0;
            UInt8 value = 0;
            UInt32 pexp = 1000; 
            UInt8 plvl = pl->GetLev();
            UInt32 exp = (plvl - 10) * ((plvl > 99 ? 99 : plvl) / 10) * 5 + 25;
            exp = exp*60/5;

            if(1 == GET_BIT(status, 0))
            {
                value = awardB[lvl];
                status = CLR_BIT(status, 0);
                pl->SetVar(VAR_ANSWER_QUESTIONS_STATUS, status);
                UInt8 num = pl->GetVar(VAR_ANSWER_SUCCESSION_NUM);
                switch(num)
                {
                    case 3:
                        {
                            value += value * 0.1;             
                        }
                        break;
                    case 5:
                        {
                            value += value * 0.2;  
                            SYSMSG_BROADCASTV(5134, pl->getCountry(), pl->getName().c_str());
                        }
                        break;
                    case 7:
                        {
                            value += value * 0.3;  
                        }
                        break;
                    case 10:
                        {
                            value += value * 0.5;  
                            SYSMSG_BROADCASTV(5135, pl->getCountry(), pl->getName().c_str());
                        }
                        break;
                    case 15:
                        {
                            value += value;  
                            SYSMSG_BROADCASTV(5136, pl->getCountry(), pl->getName().c_str());
                        }
                        break;
                     case 20:
                        {
                            SYSMSG_BROADCASTV(5142, pl->getCountry(), pl->getName().c_str());
                        }
                        break;
                }
                if(num>15 && num<=30)
                    value += value;

                UInt32 skillStatus = pl->GetVar(VAR_ANSWER_SKILL_STATUS);
                UInt8 special = GET_BIT_5(skillStatus, 1);
                if(m_answerId == special) //幸运之星技能
                {
                    value += value;
                    exp += exp;
                    pexp += pexp;
                    awardA[lvl] += awardA[lvl];
                }
                pl->AddVar(VAR_ANSWER_LITERARY_VALUE, value);
                m_answerEnd.valueA = value;
                ret = 1;
            }
            else
            {
                exp = exp / 2;
                pexp = pexp / 2;
                awardA[lvl] = awardA[lvl] / 2;
            }
            pl->AddExp(exp);
            pl->AddPExp(pexp);
            pl->getAchievement(awardA[lvl]);
            m_answerEnd.valueB = exp;
            m_answerEnd.valueC = pexp;
            m_answerEnd.valueD = awardA[lvl];

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
        }
    }

    void Answer::AwardEnd(Player* pl)
    {
        if(m_answerId >= 30)
            return;

        m_answerId++;
        m_answerEnd.valueA = 0;
        m_answerEnd.valueB = 0;
        m_answerEnd.valueC = 0;
        m_answerEnd.valueD = 0;

        UInt32 status = pl->GetVar(VAR_ANSWER_QUESTIONS_STATUS);
        status = CLR_BIT(status, 0);
        pl->SetVar(VAR_ANSWER_QUESTIONS_STATUS, status);
        SendAnswerInfo(pl);
    }

    void Answer::UseSkill(Player* pl, UInt8 skillId, UInt64 otherId)
    {
        if(NULL == pl)
            return;

        if(0==skillId || skillId>5)
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
            };
            skilldata sd = {pos, nextId};
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

            answerLog logA;
            logA.logType = 2;
            logA.skillId = skillId;
            logA.name = other->getName();
            std::map<UInt64, std::list<answerLog>>::iterator iterA = m_playerAnswerLog.find(other->getId());
            if(iterA == m_playerAnswerLog.end())
            {
                std::list<answerLog> _answerLog;
                _answerLog.push_front(logA);
                m_playerAnswerLog.insert(std::make_pair(other->getId(), _answerLog));
            }
            else
                iterA->second.push_front(logA);

            answerLog logB;
            logB.logType = 3;
            logB.skillId = skillId;
            logB.name = pl->getName();
            std::map<UInt64, std::list<answerLog>>::iterator iterB = m_playerAnswerLog.find(pl->getId());
            if(iterB == m_playerAnswerLog.end())
            {
                std::list<answerLog> _answerLog;
                _answerLog.push_front(logB);
                m_playerAnswerLog.insert(std::make_pair(pl->getId(), _answerLog));
            }
            else
                iterB->second.push_front(logB);
        }
   }
}

