//
//  MissingNumberScene.hpp
//  KitkitSchool
//
//  Created by YongSoo Hong on 03/28/18
//

#ifndef MissingNumberScene_hpp
#define MissingNumberScene_hpp

#include <stdio.h>
#include <Common/Controls/ProgressIndicator.hpp>
#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "MissingNumberNumberCard.hpp"
#include "Egg.hpp"
USING_NS_CC;

using namespace cocos2d;
using namespace cocos2d::ui;
using namespace std;

namespace MissingNumber {
    
    struct Data {
        int mLevel;
        int mWorksheet;
        int mProblem;
        string mQuestion;
        string mSuggest;
        string mAnswer;
    };
    
    class MissingNumberScene : Layer
    {
    public:
        static Scene* createScene(int levelID);
        virtual bool init() override;
        virtual void onEnter() override;
        virtual void onExit() override;
        void setLevel(int levelID);
        
        CREATE_FUNC(MissingNumberScene);
        
    private:
        int mLevelID;
        int mWorkSheetID;
        int mCurrentProblemID;
        vector<struct Data> mData;
        vector<Egg*> mEggs;
        vector<NumberCard*> mNumberCards;
        int mRetryCount;
        Node* mBgLight;
        Node* mEggRoot;
        Node* mNumberCardRoot;
        NumberCard* mDummyNumberCard;
        ParticleSnow* mPaticleSnow;
        ProgressIndicator *mProgressBar;
        
        void readLevelData();
        void setProblem(int problem);
        void startJumpAnimationProblem(float startOffset);
        void setEggs();
        void setNumberCard();
        void bindingCardEvent(NumberCard *card);
        bool isSolved();
        void onSolved();
        void setPreventTouches(bool bEnable);
        void setLightPosition();
        
        string makeWorkPath() const;
        void writePlayLog(NumberCard* hitCard, Egg* hitEgg);
        
    public:
        static vector<int> levelIDsFor();
    };
    
}  // namespace MissingNumber
#endif /* MissingNumberScene_hpp */
