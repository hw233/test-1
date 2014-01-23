#include "Config.h"
#include "Common/Stream.h" 
#include "Player.h"
#include "Mail.h"
#include "Var.h"
namespace GObject
{
#define QuestionMax 48
#define OneTime 300

    class MarryBoard
    {
        public:
            MarryBoard():_man(NULL),_woman(NULL),finder(NULL),_boardTime(0)
            {
                resetData(); 
            }
            bool CreateMarry(Player * man , Player * woman ,UInt8, UInt32 mTime) ;
            void MarryBoard_Timer();
            bool sendMarryBoardInfo(Player * pl , UInt8 type = 0);
            void answerTheQuestionOn2(Player* pl ,UInt8 ans);
            void selectDoor(Player * pl ,UInt8 door);
            UInt32 wrapTheKey(UInt32 plKey);
            UInt32 unWrapTheOutKey(UInt32 key);
            void setDoorMax();
            void resetData();
            void SetQuestionOnMarryBoard();
            UInt8 getTheMaxSupport();
            void SendPreMarryPresent(Player * man,Player * woman ,UInt8 norms );
            void sendTodayMarryInfo(Player * pl ,UInt8 flag = 0);
            void CheckTheAnswerBoard2();
            bool sendAward();
            static MarryBoard & instance()
            {
                static MarryBoard s;
                return s;
            }
        public:
            static void * _marryBoardTimer ;
            static UInt8 answers[QuestionMax];
            static std::string norms[3];
            static std::string doorName[8];
            Player * _man;
            Player * _woman;
            Player * finder;

            UInt8 _norms;
            UInt8 _type;
            UInt32 _atTime;
            UInt32 _boardTime ;
            //第一阶段
            UInt32 _lively;
            UInt32 _YHlively;
            //第二阶段
            UInt8 _askNum;
            UInt8 _questionId[10];
            UInt8 _answers[10];
            UInt32 _support[4];
            UInt8 _mAnswer;
            //第三阶段
            //第四阶段
            UInt32 _door[8];
            UInt8 _rightDoor;
            UInt32 doorMax;
            UInt32 plNum;
    };
}
