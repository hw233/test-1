#ifndef _ANSWER_H_
#define _ANSWER_H_
#include "Config.h"
#include "Player.h"
#include "Common/Stream.h"

namespace GObject
{
    #define MAX_ANSWER_NUM      30
    #define MAX_QUESTION_NUM    891

    enum
    {
        ANSWER_SKILL_A = 1,   // 一战成名命运之手技能 
        ANSWER_SKILL_B,       // 一战成名幸运之星技能 
        ANSWER_SKILL_C,       // 一战成名复苏之风技能 
        ANSWER_SKILL_D,       // 一战成名乌烟瘴气技能 
        ANSWER_SKILL_E,       // 一战成名颠倒乾坤技能 
    };

    struct answerNum
    {
        UInt32 retANum;      // A选项支持人数
        UInt32 retBNum;      // B选项支持人数
        UInt32 retCNum;      // C选项支持人数
        UInt32 retDNum;      // D选项支持人数

        answerNum() : retANum(0), retBNum(0), retCNum(0), retDNum(0) {}
    };

    struct answerEnd
    {
        UInt32 valueA;       // 本题所获经验值 
        UInt32 valueB;       // 本题所获修为值
        UInt32 valueC;       // 本题所获文彩值
        UInt32 valueD;       // 本题所获荣誉值

        answerEnd() : valueA(0), valueB(0), valueC(0), valueD(0) {}
    };

    struct answerLog
    {
        UInt8 logType;
        UInt8 skillId;
        std::string name;
        UInt8 answerId;
        UInt8 value;
        UInt8 ret;
        answerLog() : logType(0), skillId(0), name(""), answerId(0), value(0), ret(0) {}
    };

    class Player;

    class Answer
    {
    public:
        Answer();
        ~Answer();

        
    public:
        void RandQuestions();
        void AddQuestionsFromDB(UInt8 answerId, UInt16 questionsId);
        void InitAnswerId(UInt8 answerId);
        void SendAnswerInfo(Player* pl);
        void SelectAnswer(Player* pl, UInt8 opt);
        void AnswerEnd(Player* pl);
        void AwardEnd(Player* pl);
        void UseSkill(Player* pl, UInt8 skillId, UInt64 otherId);
    private:
        UInt8 m_answer[30];                             // 本次活动题目
        UInt8 m_answerId;                               // 当前题号

        std::map<UInt64, answerEnd> m_playerAnswerEnd;  // 玩家答题信息
        std::map<UInt64, std::list<answerLog>> m_playerAnswerLog;
        answerEnd m_answerEnd;
        answerNum m_answerNum;

    };
    
    extern Answer* answerManager;
}

#endif
