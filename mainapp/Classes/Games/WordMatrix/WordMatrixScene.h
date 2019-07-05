#ifndef WordMatrixScene_h
#define WordMatrixScene_h

#include <stdio.h>
#include <unistd.h>
#include <algorithm>
#include <string>
#include <vector>
#include <map>

#include "cocos2d.h"
#include "ui/CocosGUI.h"

#include "Common/Basic/SoundEffect.h"
#include "Common/Controls/TodoSchoolBackButton.hpp"
#include "Common/Controls/ProgressIndicator.hpp"

USING_NS_CC;
using namespace std;
using namespace cocos2d::ui;

#define WM_MIN_FONT_SIZE 46.0f
#define WM_MAX_FONT_SIZE 60.0f
#define WM_TSV_PATH "games/wordmatrix/wordmatrix_level.tsv"

typedef enum eMoveBlockType_{
    TYPE_1, TYPE_2,
    TYPE_3, TYPE_4, TYPE_5,
    TYPE_SLOT
}eMoveBlockType;

typedef struct WORD_MATRIX_DATA_{
    int _quesionNo;
    vector<string> _verticalWords;
    vector<string> _horizontalWords;
}WORD_MATRIX_DATA;

typedef struct RESOURCES_DATA_{
    Size _gameSize = Size(2560, 1800);
    
    string _bgPath = "wordmatrix/bg.png";
    string _verBoxPath = "wordmatrix/btn_vertical.png";
    string _horBoxPath = "wordmatrix/btn_horizontal.png";
    
    string _boardPath = "wordmatrix/board.png";
    string _slotPath = "wordmatrix/slot_area.png";
    string _slotBottomPath = "wordmatrix/slot_area_bottom.png";
    
    string _fontName = "fonts/mukta-bold.ttf";
    string _tsvPath = WM_TSV_PATH;
}RESOURCES_DATA;

typedef struct TEXT_INFO_{
    string _word;
    Color3B _color = Color3B::WHITE;
}TEXT_INFO;

class WordMatrixBlock : public Sprite
{
public:
    WordMatrixBlock();
    virtual ~WordMatrixBlock(void){}
    virtual void onEnter();
    virtual void onExit();
    virtual void touchDelegateRetain();
    virtual void touchDelegateRelease();
    
    static WordMatrixBlock *create(string fileName, vector<TEXT_INFO> textInfos, bool swallowTouch = true);
    
    void init(string fileName, vector<TEXT_INFO> textInfos);
    void setShowText(bool enable);
    void onOffEffectImg(bool onOff);
    
public:
    TEXT_INFO _textInfo;
    
protected:
    virtual bool onTouchBegan(Touch *pTouch, Event *pEvent);
    
    bool setSound(const char* fileName);
    void setLabel(vector<TEXT_INFO> textInfos);
    Rect rect();
    bool containsTouchLocation(Touch* touch);
    
protected:
    Sprite *_effectImg;
    
    Label *_wordLabel1;
    Label *_wordLabel2;
    
    bool playSound;
    int _soundIdx;
    vector<SoundEffect *> _wordSounds;
    SoundEffect *_currentSound;
    string _speakWordSound;
    vector<std::string> _speakWord;
    vector<float> _soundDurations;
    
    bool _swallowTouch;
};

class WordMatrixAnswerBlock : public WordMatrixBlock
{
public:
    WordMatrixAnswerBlock();
    virtual ~WordMatrixAnswerBlock(void){}
    virtual void onEnter();
    virtual void onExit();
    
    static WordMatrixAnswerBlock *create(eMoveBlockType blockType, vector<TEXT_INFO> textInfos, bool swallowTouch = true);
    
    void init(eMoveBlockType blockType, vector<TEXT_INFO> textInfos);
    void setPrevPos();
    void autoMove();
    void playWordSound();
    
public:
    bool _isAutoMove;
    
protected:
    virtual bool onTouchBegan(Touch *pTouch, Event *pEvent);
    virtual void onTouchMoved(Touch *pTouch, Event *pEvent);
    virtual void onTouchEnded(Touch *pTouch, Event *pEvent);
    virtual void onTouchCancelled(Touch *pTouch, Event *pEvent);
    
private:
    void checkEndTouch();
    
private:
    Sprite *_midImg;
    Sprite *_shadowImg;
    
    Vec2 _prevPos;
    
    SoundEffect *_wrongSound = NULL;
    
    bool _isTouched;
    bool _isCanMove;
    bool _swallowTouch;
};

class WordMatrixMoveBlockLine : public Sprite
{
public:
    WordMatrixMoveBlockLine();
    virtual ~WordMatrixMoveBlockLine(void){}
    virtual void onEnter();
    virtual void onExit();
    
    static WordMatrixMoveBlockLine *create(int maxBlockCnt);
    
    void init(eMoveBlockType blockType);
    eMoveBlockType getBlockType();
    bool canAddBlock();
    void addWordBlock(WordMatrixAnswerBlock *answerBlock);
    void setAutoPosition();         //must be called after add all word
    void showLine();
    bool removeWordBlock(WordMatrixAnswerBlock *answerBlock);
    int getBlockCnt();
    
private:
    eMoveBlockType _blockType;
    __Array *_childBlockArray = NULL;
    int _maxBlockCnt;
    SoundEffect *_showSound;
};

class WordMatrixSlotBlock : public Sprite
{
public:
    WordMatrixSlotBlock();
    virtual ~WordMatrixSlotBlock(void){}
    
    static WordMatrixSlotBlock *create(bool showImg, bool isBottom, TEXT_INFO textInfo);
    void init(bool showImg, bool isBottom, TEXT_INFO textInfo);
    
    void setConnectedBlock(WordMatrixBlock *verBlock, WordMatrixBlock *horBlock);
    void onOffConnectedBlockEffectImg(bool onOff);
    
public:
    TEXT_INFO _textInfo;
    int _zOrder;
  
private:
    Sprite *_selectedImg;
    WordMatrixBlock *_connectedVerBlock;
    WordMatrixBlock *_connectedHorBlock;
};

class WordMatrixBlockBoard : public Sprite
{
public:
    WordMatrixBlockBoard();
    virtual ~WordMatrixBlockBoard(void){}
    virtual void onEnter();
    virtual void onExit();
    
    static WordMatrixBlockBoard *create(int verCnt, int horCnt);
    
    void init(int verCnt, int horCnt);
    void addVerWord(WordMatrixBlock *verWord);
    void addHorWord(WordMatrixBlock *horWord);
    void makeBlocks();       //must be called after add all word
    
public:
    __Array *_answerBlockArray = NULL;
    int _verCnt;
private:
    Scale9Sprite *_boardPan;
    vector<WordMatrixBlock *> _verBlocks;
    vector<WordMatrixBlock *> _horBlocks;
};

class WordMatrixScene : public Layer
{
public:
    static Scene* createScene(string levelID);
    virtual bool init();
    virtual void onEnter();
    virtual void onEnterTransitionDidFinish();
    virtual void onExit();
    CREATE_FUNC(WordMatrixScene);
    
    void setLevel(int level);
    
    void checkEnterAnswerBlock(Vec2 currentPos);
    void checkAnswerWord(Vec2 touchPos);
    
private:
    void setScreenSize();
    void loadStageData(int *sheetNum);
    void makeStage();
    
    void startGame();
    void gameUpdate(float dt);
    bool checkCollideWall(WordMatrixAnswerBlock *block);
    
    void showParticle(bool onOff, int zOrder = 0, Vec2 pos = Vec2::ZERO);
    void showMoveBlockPan();
    void endStage();
    void setNextStage();
    
private:
    Node *_gamePan;
    Node *_uiPan;
    float _screenW, _screenH;
    float _gameScale;
    
    vector<WORD_MATRIX_DATA> _selectedStageInfos;
    int _currentLevel;
    int _currentSheetID;
    int _currentStageIdx;
    
    WordMatrixBlockBoard *_blockBoard;
    __Array *_moveBlockPanArray = NULL;
    WordMatrixMoveBlockLine *_currentMoveBlockPan;
    
    SoundEffect *_clearSound = NULL;
    
    Node *_particleNode = NULL;
    
    TodoSchoolBackButton *_backButton;
    ProgressIndicator *_progressBar;
};

#endif /* WordMatrixScene_h */
