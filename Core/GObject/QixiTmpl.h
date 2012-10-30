#ifndef QIXI_TEMPLATE
#define QIXI_TEMPLATE

#include "Script/Script.h"
#include "Mail.h"

namespace GObject
{
    struct ScoreAward
    {
        UInt32 score;
        std::vector<MailPackage::MailItem> items;
    };
    struct LBAward
    {
        UInt32 lbPos;
        std::vector<MailPackage::MailItem> items;
    };

    class QixiTmpl
    {
    public:
        QixiTmpl();
        void load(const char * = NULL);
        inline void setFilename(const char * fn) { _filename = fn; }
        inline void setQixiItem(UInt32 itemId) { _qixiItem = itemId; }
        inline void setTitleItem(UInt32 itemId) { _titleItem = itemId; }
        inline void setDivorceMsgCode(UInt32 code) { _divorceMsgCode = code; }
        inline void setRoamingFunc(const char * func) { _onRoamingFunc = func; }
        inline void setScoreTitle(UInt32 title) { _scoreTitle = title; }
        inline void setScoreContent(UInt32 content) { _scoreContent = content; }
        inline void setMinAwardScore(UInt32 socre) { _minAwardScore = socre; }
        void addScoreAwards(UInt32 socore, UInt32 itemId, UInt32 itemCnt);

        inline void setPartnerTitle(UInt32 title) { _partnerTitle = title; }
        inline void setPartnerContent(UInt32 content) { _partnerContent = content; }
        void addPartnerAwards(UInt32 itemId, UInt32 itemCnt);

        inline void setLBTitle(UInt32 title) { _lbTitle = title; }
        inline void setLBContent(UInt32 content) { _lbContent = content; }
        inline void setMaxLBAwardPos(UInt32 pos) { _maxLBAwardPos = pos; }
        void addLBAwards(UInt32 pos, UInt32 itemId, UInt32 itemCnt);

        UInt32 _qixiItem;
        UInt32 _titleItem;
        UInt32 _divorceMsgCode;
        std::string _onRoamingFunc;

        UInt32 _minAwardScore;
        UInt32 _scoreTitle;
        UInt32 _scoreContent;
        std::vector<ScoreAward> _scoreAwards;

        UInt32 _partnerTitle;
        UInt32 _partnerContent;
        std::vector<MailPackage::MailItem> _partnerAward;

        UInt32 _lbTitle;
        UInt32 _lbContent;
        std::vector<LBAward> _lbAwards;
        UInt32 _maxLBAwardPos;
    private:
        std::string _filename;
    };

    extern QixiTmpl qixiTmpl;
}


namespace Script
{
    class QixiTmplScript:
        public Script
    {
    public:
        QixiTmplScript(GObject::QixiTmpl * qixitmpl);
    };

}

#endif  //QIXI_TEMPLATE
