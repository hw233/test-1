#include "Config.h"
#include "Common/Stream.h"
#include "QuestionPaper.h"
#include "Common/TimeUtil.h"
#include "Player.h"
#include "MsgID.h"
#include "Server/SysMsg.h"
#include "Package.h"
#include "GData/GDataDBExecHelper.h"
#include "GObjectDBExecHelper.h"
#include "Common/StringTokenizer.h"
#include <algorithm>
#include "Common/Itoa.h"

namespace GObject
{

bool TitleInfo::checkAnswerCorrect(UInt16 answer,string str)
{
    if(answer == 0)
        return false;
    if(eAnswerType == ANSWER_NORMAL || eAnswerType == ANSWER_INPUT)
    {
        UInt8 true_num = 0;
        UInt8 bit_num = 0;
        while(answer != 0)
        {
            if(GET_BIT(answer,0))
                true_num ++;
            answer = answer >> 1;
            bit_num ++;
        }
        if(bit_num > answer_num)
            return false;
        if(true_num > 1)
            return false;
    }
    if(eAnswerType != ANSWER_INPUT)
    {
        if(!str.empty())
            return false;
    }

    return true;
}

QuestionPaper::QuestionPaper() : titleSum(0)
{
}

QuestionPaper::~QuestionPaper()
{
}

void QuestionPaper::loadTitlePaper(GData::DBTitlePaper dbtp)
{
    TitleMap::iterator it = m_titleList.find(dbtp.id);
    if(it != m_titleList.end())
        return;
    TitleInfo* ti = new TitleInfo(dbtp.id,static_cast<EAnswerType>(dbtp.type),dbtp.option_num);

    const std::string str_award = dbtp.award;
    StringTokenizer tokenizer(str_award, ";");
    for(StringTokenizer::Iterator it = tokenizer.begin(); it != tokenizer.end(); ++ it)
    {
        const std::string& s = *it;
        size_t p = s.find(':');
        if(p != std::string::npos)
        {
            std::string v = std::string(s.begin(), s.begin() + p);
            UInt32 t1 = atoi(v.c_str());
            UInt16 t2 = atoi(s.c_str() + p + 1);
            ti->m_award.insert(std::make_pair(t1,t2));
        }
    }
    m_titleList.insert(std::make_pair(dbtp.id,ti));
    titleSum ++;
}

void QuestionPaper::loadQuestionPaper(DBQuestionPaper dbqp)
{
    Player* pl = globalPlayers[dbqp.playerId];
    if(pl == NULL)
        return;
    PaperPlayerInfo* tmpPlayerInfo = new PaperPlayerInfo(pl,dbqp.cur_idx,dbqp.begtime);
    
    const std::string str_answer = dbqp.answer; 
    StringTokenizer tokenizer1(str_answer, "|");
    UInt8 idx = 1;
    for(StringTokenizer::Iterator i = tokenizer1.begin(); i != tokenizer1.end(); ++ i)
    {
        const std::string& s = *i;
        size_t p = s.find(';');
        if(p != std::string::npos)
        {
            std::string v = std::string(s.begin(), s.begin() + p);
            std::string w = std::string(s.begin() + p + 1,s.end());
            UInt16 t1 = atoi(v.c_str());
            AnswerInfo* ai = new AnswerInfo(idx,t1,w); 
            tmpPlayerInfo->m_answerList.insert(std::make_pair(idx,ai));
        }
        idx ++;
    }
    
    const std::string str_award = dbqp.award;
    StringTokenizer tokenizer(str_award, ";");
    for(StringTokenizer::Iterator it = tokenizer.begin(); it != tokenizer.end(); ++ it)
    {
        const std::string& s = *it;
        size_t p = s.find(':');
        if(p != std::string::npos)
        {
            std::string v = std::string(s.begin(), s.begin() + p);
            UInt32 t1 = atoi(v.c_str());
            UInt16 t2 = atoi(s.c_str() + p + 1);
            tmpPlayerInfo->m_awardSum.insert(std::make_pair(t1,t2));
        }
    }
    
    m_PaperPlayerList.insert(make_pair(dbqp.playerId,tmpPlayerInfo));

}


PaperPlayerInfo* QuestionPaper::getPlayerInfo(Player* pl)
{
    if(pl == NULL)
        return NULL;
    PaperInfoMap::iterator it = m_PaperPlayerList.find(pl->getId());
    if(it == m_PaperPlayerList.end())
        return NULL;
    return it->second;
}

void QuestionPaper::AnswerQuestion(Player* pl,UInt16 option,string ext_answer)
{
    PaperPlayerInfo* tmpPlayerInfo = NULL;
    tmpPlayerInfo = getPlayerInfo(pl);
    if(tmpPlayerInfo == NULL)
        return;
    
    if(tmpPlayerInfo->cur_idx == 0xFE || tmpPlayerInfo->cur_idx == 0xFF)
        return;
    
    UInt8 idx = tmpPlayerInfo->cur_idx;
    if(tmpPlayerInfo->cur_idx != 0)
    {
    
        AnswerInfoMap::iterator it = tmpPlayerInfo->m_answerList.find(idx);//检查是否已经答过了
        if(it != tmpPlayerInfo->m_answerList.end())
            return;
       
        TitleMap::iterator it_title = m_titleList.find(idx); 
        if(it_title == m_titleList.end())
            return;
        if(!it_title->second->checkAnswerCorrect(option,ext_answer))
            return ;

        AnswerInfo* ai = new AnswerInfo(idx,option,ext_answer); 
        tmpPlayerInfo->m_answerList.insert(std::make_pair(idx,ai));   

        for(map<UInt32,UInt16>::iterator i = it_title->second->m_award.begin(); i != it_title->second->m_award.end(); i++) 
        {
            map<UInt32,UInt16>::iterator m = tmpPlayerInfo->m_awardSum.find(i->first); 
            if(m == tmpPlayerInfo->m_awardSum.end())
                tmpPlayerInfo->m_awardSum.insert(std::make_pair(i->first,i->second));
            else
                m->second += i->second;
        }
        //TODO DB 
        //DB4().PushUpdateData("REPLACE INTO `questionpaper`(`playerid`,`begtime`,`cur_idx`,`answer`,`award`,`totaltime`  FROM `questionpaper`) VALUES(%" I64_FMT "u, %u,%u,'%s','%s',%u)", pl->getId(),   );
        std::string str_answer;
        std::string str_award; 
        for(AnswerInfoMap::iterator ait = tmpPlayerInfo->m_answerList.begin();ait != tmpPlayerInfo->m_answerList.end(); ait++)
        {
            if(ait != tmpPlayerInfo->m_answerList.begin())
                str_answer += "|";
            str_answer += Itoa(ait->second->answer_option);
            str_answer += ";";
            str_answer += ait->second->answer_str;
        }
        for(map<UInt32,UInt16>::iterator bit = tmpPlayerInfo->m_awardSum.begin();bit != tmpPlayerInfo->m_awardSum.end();bit++)
        {
            if(bit != tmpPlayerInfo->m_awardSum.begin())
                str_award += ";";
            str_award += Itoa(bit->first);
            str_award += ":";
            str_award += Itoa(bit->second);
        }
        
        DB4().PushUpdateData("UPDATE `questionpaper` SET `answer` = '%s', `award` = '%s' WHERE `playerid` = %" I64_FMT "u", str_answer.c_str(), str_award.c_str(),pl->getId());
        char str[32] = {0};
        sprintf(str, "F_140912_%d",(idx));
        pl->udpLog("yonghudiaoyan", str, "", "", "", "", "act");
        UInt16 bit_num = 0;
        while(option != 0)
        {
            if(GET_BIT(option,0))
            {
                char str1[32] = {0};
                sprintf(str1, "F_140912_%d_%d",(idx),(bit_num+1));
                pl->udpLog("yonghudiaoyan", str1, "", "", "", "", "act");
            }
            option = option >> 1;
            bit_num ++;
        }
    }
    idx++;

    tmpPlayerInfo->cur_idx = idx;
    if(tmpPlayerInfo->cur_idx > titleSum)
        tmpPlayerInfo->cur_idx = 0xFE;
    RetPlAnswerInfo(pl,false);
    DB4().PushUpdateData("UPDATE `questionpaper` SET `cur_idx` = %u WHERE `playerid` = %" I64_FMT "u", tmpPlayerInfo->cur_idx, pl->getId());
    
}

void QuestionPaper::RetPlAnswerInfo(Player* pl,bool b_next)
{
    PaperPlayerInfo* tmpPlayerInfo = NULL;
    tmpPlayerInfo = getPlayerInfo(pl);
    if(tmpPlayerInfo == NULL)
        return;

    Stream st(REP::EXTEND_PROTOCAOL);
    st <<static_cast<UInt8>(0x05) << static_cast<UInt8>(0x02);
    if(b_next)
        st << (tmpPlayerInfo->cur_idx + 1); 
    else
        st << tmpPlayerInfo->cur_idx; 
    st << Stream::eos;
    pl->send(st);
}

void QuestionPaper::EnterPaper(Player* pl)
{
    PaperPlayerInfo* tmpPlayerInfo = NULL;
    tmpPlayerInfo = getPlayerInfo(pl);
    if(tmpPlayerInfo == NULL)
    {
        UInt32 now = TimeUtil::Now();
        tmpPlayerInfo = new PaperPlayerInfo(pl,0,now);
        m_PaperPlayerList.insert(make_pair(pl->getId(),tmpPlayerInfo));
        //TODO DB
        DB4().PushUpdateData("REPLACE INTO `questionpaper`(`playerid`,`begtime`,`cur_idx`) VALUES(%" I64_FMT "u, %u, %u)", pl->getId(), now, 0);
        pl->udpLog("yonghudiaoyan", "F_140912_1", "", "", "", "", "act");
    }

    RetPlAnswerInfo(pl);
}

void QuestionPaper::ReceiveAward(Player* pl)
{
    PaperPlayerInfo* tmpPlayerInfo = NULL;
    tmpPlayerInfo = getPlayerInfo(pl);
    if(tmpPlayerInfo == NULL)
        return;
     
    if(tmpPlayerInfo->cur_idx != 0xFE)
        return;

    if(tmpPlayerInfo->m_awardSum.size() == 0)
        return; 
    for(map<UInt32,UInt16>::iterator it = tmpPlayerInfo->m_awardSum.begin(); it != tmpPlayerInfo->m_awardSum.end(); it++)
    {
        if(it->first == 499)
            pl->getCoupon(it->second);
        else
            pl->GetPackage()->Add(it->first, it->second, true, false, FromQuestionPaper);
    }
    tmpPlayerInfo->cur_idx = 0xFF;
    UInt32 totaltime = TimeUtil::Now() - tmpPlayerInfo->beg_answerTime;
    //TODO DB
    DB4().PushUpdateData("UPDATE `questionpaper` SET `totaltime` = %u, `cur_idx` = %u WHERE `playerid` = %" I64_FMT "u", totaltime, tmpPlayerInfo->cur_idx, pl->getId());
    pl->udpLog("yonghudiaoyan", "F_140912_2", "", "", "", "", "act");
    RetPlAnswerInfo(pl);
}






}



