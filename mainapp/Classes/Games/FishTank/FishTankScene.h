//
//  FishTankScene.h
//  todomath
//
//  Created by Jesun Kim on 2/15/16.
//
//

#ifndef FishTankScene_h
#define FishTankScene_h



#include "FishTankFishObject.h"
#include "FishTankTankObject.h"
#include "FishTankCardObject.h"
#include "FishTankSlotObject.h"

#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
#include "3rdParty/json/json.h"

USING_NS_CC;

using namespace cocostudio;
using namespace cocostudio::timeline;
using namespace ui;

class ProgressIndicator;

class FishTankScene : public Layer {
    
public:
    CREATE_FUNC(FishTankScene);
    
    FishTankScene();
    virtual ~FishTankScene();
    
    virtual void prepareNewGameWithLevel(Json::Value problemParameter) ;
    virtual void handleStart() ;
    virtual void handlePause() ;
    virtual void handleQuit() ;
    virtual void handleResume() ;
    virtual bool init() ;
    virtual void onEnter() ;
    virtual void onExit() ;
    
    virtual void showHint() ;
    virtual void hideHint() ;
    
    virtual void handleAnswerEntered(std::string &answer) ;
    
    virtual bool onTouchBegan(cocos2d::Touch *touch, cocos2d::Event *event) override;
    virtual void onTouchMoved(cocos2d::Touch *touch, cocos2d::Event *event) override;
    virtual void onTouchEnded(cocos2d::Touch *touch, cocos2d::Event *event) override;
    
    virtual void update(float fDelta) override;
    
public:
    enum{
        K_SOUND_CARD_TOUCH = 0,
        K_SOUND_CARD_SLOT_IN,
        K_SOUND_CARD_MISS,
        K_SOUND_FISH_TOUCH,
        K_SOUND_FISH_RELEASE,
        K_SOUND_FISH_RELEASE_1,
        K_SOUND_FISH_BUBBLE,
        K_SOUND_FISH_PLACED,
        K_SOUND_FISH_START,
        K_SOUND_MAX
    };

    
    void initUI();
    
    void setProblem(float);
    void setProblemInit(float);
    void setGameStart();
    
private:
    bool checkIsFishInTank(FishTankTankObject *tankObject);
    void fixFishPosition(FishTankTankObject *tankObject);
    bool checkClearPhase1();
    
    void initFishes(FishTankTankObject *tankObject, int fishCount);
    void drawTank();
    void drawPhase2Move();
    void drawPhase1Problem();
    void drawPhase2Problem();
    std::string findWrongString();
    void checkSlotChar(std::string str);
    
    void checkLevelClear(float);
    void checkLevelClearDelay(float);
    void drawFishBubble(float);
    
    void removeFishes();
    void removeCards();
    void removeTexts();
    
    std::string getSoundName(int type);
    
private:
    Json::Value answers;
    
    Size m_WinSize;
    
    Vector<FishTankFishObject *> m_vecFishObject;
    Vector<FishTankTankObject *> m_vecTankObject;
    Vector<FishTankSlotObject *> m_vecSlotObject;
    Vector<FishTankCardObject *> m_vecCardObject;
    std::vector<std::vector<std::string>> m_vecTotalProblems;
    std::vector<std::string> m_vecNowProblem;
    std::vector<std::string> m_vecWrong;
    
    FishTankFishObject *m_selectedFish;
    FishTankCardObject *m_selectedCard;
    FishTankCardObject *m_hintCard;

    Node *m_mainNode;
    
    Sprite *m_spriteShelf;
    
    bool m_isOnlyShowSubtrahend;
    bool m_isShowAnswer;
    bool m_isMinus;
    bool m_isBlockTouch;
    bool m_isPhase1;
    bool m_isBlockFishMove;
    bool m_isAlreadyFish[7];
    
    int m_currentLevel;
    int m_currentStage;
    int m_selectIndex;
    
    int m_succCount;
    int m_clearCount;
    int m_problemCount;
    
    // phase2 읽어주기
    void readPhase2(float dt);
    int m_readCount;
    // phase2 읽어주기
    
    std::string m_strLevelFormat;
    // foramt 데이터
    int m_topTankCount;
    std::string m_strTopTankName;
    int m_bottomTankCount;
    std::string m_strBottomTankName;
    bool m_isAlreadyPhase2;
    bool m_isReversePhase2;
    bool m_isHideTankText;
//    bool m_isMinus;
    
    ProgressIndicator *_progressBar;

    const char* defaultFont = "fonts/TodoSchoolV2.ttf";
};

#endif /* FishTankScene_h */
