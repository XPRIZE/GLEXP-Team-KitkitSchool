//
//  EquationMakerScene.h
//  todomath
//
//  Created by Hye Koung Hong on 7/23/15.
//
//

#ifndef __todomath__EquationMakerScene__
#define __todomath__EquationMakerScene__

#include "cocos2d.h"
#include "3rdParty/json/json.h"
#include "ui/CocosGUI.h"
#include "extensions/GUI/CCControlExtension/CCControl.h"
#include "Common/Controls/ProgressIndicator.hpp"


USING_NS_CC;
USING_NS_CC_EXT;

class EquationMakerObject;
class EquationMakerScene : public Layer {
public:
    enum eGameState{
        K_TAG_GAME_WAIT = 0,
        K_TAG_GAME_ING,
        K_TAG_GAME_END,
    };
    
    enum{
        K_TYPE_NUM = 0,
        
        K_TYPE_SIGN_PLUS = 100,
        K_TYPE_SIGN_MINUS,
        K_TYPE_SIGN_EQ,
        K_TYPE_SIGN_EQ_KOREAN_E,
        K_TYPE_SIGN_EQ_KOREAN_N,
        K_TYPE_SIGN_RANDOM,
        K_TYPE_SIGN_TIMES,
    };
    
    enum{
        K_OBJECT_TYPE_NONE = 0,
        K_OBJECT_TYPE_OBJECT_NON_SLOT,
        K_OBJECT_TYPE_NUMBER,
        K_OBJECT_TYPE_OBJECT,
        K_OBJECT_TYPE_OBJECT_NUMBER,
    };
    
    enum{
        K_TAG_BACKGROUND = 0,
        K_TAG_OBJECT,
        K_TAG_SELECT_OBJECT,
        
        K_TAG_OBJECT_TAG = 100,
        
        K_TAG_OBJECT_RESULT = 150,
//        K_TAG_OBJECT_CRAB = 200,
//        K_TAG_OBJECT_SEA,
        
        K_TAG_OBJECT_HINT = 1000,
    };
    
    enum{
        K_SOUND_TOUCH = 0,
        K_SOUND_SLOT_IN,
        K_SOUND_MISS,
        K_SOUND_MAX,
    };
    
    struct sSlot{
        Rect rect;
        
        Sprite* m_pEmptySlot;
        
        int type;
        int rowType;
        int correctIndex;
        
        bool isSlotIn;
        bool isSlotLineHide;
        
        sSlot(){ correctIndex = 0; m_pEmptySlot = NULL; }
    };

public:
    CREATE_FUNC(EquationMakerScene);

    EquationMakerScene();
    virtual ~EquationMakerScene();
    
    virtual void prepareNewGameWithLevel(Json::Value problemParameter);
    virtual void handleStart();
    virtual void handlePause();
    virtual void handleQuit();
    virtual void handleResume();
    virtual bool init() override;
    virtual void onEnter() override;
    virtual void onExit() override;
    


    virtual void handleAnswerEntered(std::string &answer);
    
    virtual void onTouchesBegan(const std::vector<Touch*>& touches, Event *unused_event) override;
    virtual void onTouchesMoved(const std::vector<Touch*>& touches, Event *unused_event) override;
    virtual void onTouchesEnded(const std::vector<Touch*>& touches, Event *unused_event) override;
    
    void clear(Ref *ref, Control::EventType event);

    
private:
    void initUI();
    
    void setProblemInit(float);
    void setProblem(float);
    
    void setGameStart(float);
    
    void setAlignPanelObject();
    void createPanelObject(Json::Value& val);
    void createObject(int idx, int levelType, std::string fileName, Point pos, int animationTagIndex);
    
    void setObjectMatchMoveEnd();
    
    Node* createCrabResult(int lhs, int rhs);
    Node* createBaloonResult(int lhs, int rhs);
    void setClearAnimationFlow(float);
    void setClearAnimationFlowEnd();
    void setClearAnimationFlowEndDelay(float);
    void playClearAnimationObjectSound();
    
    void removeAllObject();
    
    float createNoneSlotObject(std::vector<int> vecList, std::string file, std::string fileOther, float _retStartY, float _marginY);

    void createNoneSlotPlus(int first, int second,std::string file, std::string fileOther, float _retStartY, float _marginY);
    void createNoneSlotMinus(int first, int second,std::string file, std::string fileOther, float _retStartY, float _marginY);
    
    
    float getObjectWidth(int lvType);
    float getObjectHeight(int lvType);
    
    bool isCheckTouch(const std::vector<Touch*>& touches);
    bool isSlotIn(int idx, int type, int rowType, Point pos);
    
    std::string getSoundName(int type);
    
    void eraseHint();
    
public:
    float getStartPoint(int size, float mid, float margin, bool isYPos = false);
    
private:
    Json::Value answers;
    eGameState m_GameState;
    
    std::vector<int> m_ClearEffectType;
    std::vector<sSlot> m_vecSlot;
    
    std::vector<EquationMakerObject*> m_vecHintList;
    std::vector<EquationMakerObject*> m_vecObjectList;
    std::vector<Node*> m_vecEraseObjectList;
    
    bool requiredChildSound;
    
private:
    int m_CurrentLevel;
    int m_CurrentStage;
    int m_CurrentLevelType;
    int m_SelectIndex;
    int m_ClearSoundIndex;
    
    int m_StageSuccCount;
    int m_StageClearCount;
    
    int m_ProblemCount;
    int m_AnimationStartIndex;
    
    float m_PanelHeight;
    
    bool m_isMiss;
    bool m_isFirstMiss;
    
    Size m_ObjectSize;
    
    const char* defaultFont = "fonts/TodoSchoolV2.ttf";
    
    Node *m_gameView;
    
    ProgressIndicator *_progressBar;
};


#endif
