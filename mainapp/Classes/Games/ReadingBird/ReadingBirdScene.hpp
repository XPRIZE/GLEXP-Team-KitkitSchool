//
//  ReadingBirdScene.hpp
//
//  Created by YongSoo Hong on 02/09/18
//

#ifndef ReadingBirdScene_hpp
#define ReadingBirdScene_hpp

#include <stdio.h>
#include <Common/Controls/ProgressIndicator.hpp>
#include "cocos2d.h"
#include "ui/CocosGUI.h"
USING_NS_CC;

using namespace cocos2d;
using namespace cocos2d::ui;
using namespace std;

namespace ReadingBird {
    enum class MAIN_STATE {
        none = 0,
        entrance,
        wait,
        record,
        stop_record,
        listen_record,
        listen_original,
        complete
    };
    
    enum class CHARACTER1_STATE {
        none = 0,
        appear,
        idle,
        suggest,
        induceIdle,
        induceHappy,
        look,
        stopLook,
        listen,
        stopListen,
        speakHappy,
        speakUnhappy,
        dancingA
    };
    
    enum class CHARACTER2_STATE {
        none = 0,
        urgeCharacter,
        urgeSentence,
        dancingB
    };
    
    enum class SPEAK_TYPE {
        silent = 0,
        type_1,
        type_2,
        type_3
    };
    
    struct Data {
        int mLevel;
        int mWorksheet;
        int mProblem;
        string mSuggest;
        string mSound;
    };

    const static int DEFAULT_TRIGGER_VOLUME = 110;

    class ReadingBirdScene : Layer
    {
    public:
        static Scene* createScene(int levelID, bool isSdcardData = false);
        virtual bool init() override;
        virtual void onEnter() override;
        virtual void onExit() override;
        void setLevel(int levelID);
        
        CREATE_FUNC(ReadingBirdScene);
        
    private:
        int mLevelID;
        int mWorkSheetID;
        int mCurrentProblemID;
        vector<struct Data> mData;
        map<string, string> mSpeech;
        int mScore;
        bool mbSdcardData;
        
        MAIN_STATE mMainState;
        string mCurrentText;
        bool mbShortCurrentText;
        
        Sprite* mCharacter1;
        CHARACTER1_STATE mCharacter1State;
        SPEAK_TYPE mCharacter1SpeakType;
        
        Sprite* mCharacter2;
        CHARACTER2_STATE mCharacter2State;
        SPEAK_TYPE mCharacter2SpeakType;
        
        Text* mTextQuestion;
        
        bool mIsPronunciation;
        bool mIsBeforePronunciation;
        bool mIsSilent;
        int mTriggerVolume;
        void readLevelData(int *curWorksheet);
        void readSpeechData();
        Vector<SpriteFrame*> getAnimationInFile(const char *format, int startNumber, int endNumber);
        void setCharacter1Sprite(SpriteFrame* frame);
        void setCharacter2Sprite(SpriteFrame* frame);
        void setTextQuestion(string question);
        
        void changeCharacter1State(CHARACTER1_STATE state, std::function<void()> callback = nullptr);
        void changeCharacter2State(CHARACTER2_STATE state, std::function<void()> callback = nullptr);
        void processStopLook();
        
        void onStopRecord(float f);
        void onStopRecordVolume(float f);
        
        string makeWorkPath() const;

        
    public:
        ProgressIndicator *mProgressBar;
        void changeMainState(MAIN_STATE state);
        void checkVolume(int volume);
        void changeNextProblem();
        void changeTalkingAnimation(int volume, bool recursive = false);
        void setScore(int score);
        void setSdcardData(bool bSdcardData);
        void processOnRecordVolume(int volume);
        void processOnRecordScore(int score);
        void processOnStopPlayAudio();
        void processOnPlayVolume(int volume);
        static vector<int> levelIDsFor(bool isSdCard = false, string sdcardPath = "");
    };
}  // namespace ReadingBird
#endif /* ReadingBirdScene_hpp */


