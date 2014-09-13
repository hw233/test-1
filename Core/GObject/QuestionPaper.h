#ifndef QUESTIONPAPER_H_
#define QUESTIONPAPER_H_

#include "Config.h"
#include <vector>
#include <map>
#include "Common/Singleton.h"

using namespace std;

namespace GData
{
    class DBTitlePaper;
}

namespace GObject
{
    enum EAnswerType
    {
        ANSWER_NORMAL = 0,      //单选
        ANSWER_MULTIPLE,        //多选
        ANSWER_INPUT           //可输入
    };

    class Player;
    class DBQuestionPaper;

    struct TitleInfo            //题目
    {
        UInt8 title_idx;                 //序号
        //string title_str;                //题目
        //vector<string> vec_answer;       //答案选项
        EAnswerType eAnswerType;         //答题类型 
        map<UInt32,UInt16> m_award;      //该题奖励
        UInt8 answer_num;                //答案选项数

        TitleInfo(UInt8 idx = 0,EAnswerType eTmp = ANSWER_NORMAL,UInt8 num = 0)
        {
            title_idx = idx;
            eAnswerType = eTmp;
            answer_num = num;
        }
        bool checkAnswerCorrect(UInt16 answer,string str); 
    };

    struct AnswerInfo           //答题
    {
        UInt8 title_idx;
        UInt16 answer_option;//选项 0 - 未答题 FFFF - 备注
        string answer_str;

        AnswerInfo(UInt8 idx,UInt16 option,string str)
        {
            title_idx = idx;
            answer_option = option;
            answer_str = str;
        }
    };
    
    struct PaperPlayerInfo
    {
        Player* pl;
        UInt8 cur_idx;//当前答题序号 初始0 答完 FE 答完领完奖 FF
        map<UInt8,AnswerInfo*> m_answerList;//答案列表 <问题id,16位多选答案>
        map<UInt32,UInt16> m_awardSum;//奖励集合
        UInt32 beg_answerTime;
        PaperPlayerInfo(Player* pltmp = NULL,UInt8 idx = 0,UInt32 time = 0)
        {
            pl = pltmp;
            cur_idx = idx;
            beg_answerTime = time;
        }
    };
    
    typedef map<UInt8,TitleInfo*> TitleMap;
    typedef map<UInt64,PaperPlayerInfo*> PaperInfoMap;
    typedef map<UInt8,AnswerInfo*> AnswerInfoMap;

    class QuestionPaper : public Singleton<QuestionPaper>
    {
        public:
            QuestionPaper();
            ~QuestionPaper();

            void loadTitlePaper(GData::DBTitlePaper dbtp);
            void loadQuestionPaper(DBQuestionPaper dbqp);
            PaperPlayerInfo* getPlayerInfo(Player* pl);
            void AnswerQuestion(Player* pl,UInt16 option,string ext_answer);//回答问题
            void RetPlAnswerInfo(Player* pl,bool b_next = false);//返回答题信息 
            void EnterPaper(Player* pl);//进入答题
            //void QuitPaper(Player* pl);//退出答题
            void ReceiveAward(Player* pl);//领取奖励


        private:
            TitleMap m_titleList;//题目列表
            PaperInfoMap m_PaperPlayerList;//所有参与活动玩家信息
            UInt8 titleSum;//题目总数

    };


}


#endif

