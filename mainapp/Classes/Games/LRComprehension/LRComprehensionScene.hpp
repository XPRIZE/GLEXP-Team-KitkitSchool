//
//  LRComprehensionScene.hpp
//  KitkitSchool
//
//  Created by HyeonGyu Yu on 07/08/2018.
//

#ifndef LRComprehensionScene_hpp
#define LRComprehensionScene_hpp

#include "cocos2d.h"
#include <string>
#include "Common/Controls/ProgressIndicator.hpp"
#include "LRGuideView.hpp"
#include "LRLevelData.hpp"

USING_NS_CC;
using namespace std;
using namespace LRComprehension;

class LRTitlePageView;

class LRComprehensionScene : Layer
{
    typedef pair<string, vector<string>> ComprehensionProblem;
    
private:
public:
    LRComprehensionScene();
    static Scene* createScene(int levelID);
    virtual bool init() override;
    virtual void onEnter() override;
    virtual void onEnterTransitionDidFinish() override;
    virtual void onExit() override;
    CREATE_FUNC(LRComprehensionScene);
    
private:
    LRGuideView *guideView = nullptr;
    
    Node *_pageNode;
    NodeGrid *_pageGrid;
    int _currentPageGridZ;
    Node *_gameNode;
    Node *_paperBottom;
    
    Sprite* _questionHighlight;
    Sprite* _guideHighlight;
    
    Size _winSize;
    int _currentLevel;
    int _currentProblem;
    ProgressIndicator *_progressBar;
    EventListenerTouchOneByOne *_blocker;
    
    Vec2 _boardPositionActive;
    Vec2 _boardPositionInactive;
    Vec2 _boardPositionHidden;
    
    string _bookFolder;
    int _problemSetIndex = 1;
    int _progressBarIndex = 0;
    
    NodeGrid* createPageGrid();
    void selectProblem();
    void showProblem();
    
    void drawQuestionTitle(string titleText, Node* parentNode, bool useSoundButton, string soundPath, float addLeftPadding);
    void drawGuideTitle(string titleText);
    
public:
    vector<LRProblem*> problemSet;
    int getCurrentProblem();
    void onStart();
    void onSolve(bool playSound = true);
    void onSolveWithDelay(float delayTime, bool playSound = true);
    
    void setLevel(int level);
    static vector<int> getLevels();
    void drawQuestionTitleWithSoundButton(string titleText, string soundPath, Node* parentNode);
    void drawQuestionTitle(string titleText, Node* parentNode);
    
    std::string getBookName() { return ""; }
    
    NodeGrid* GetPageGrid() { return _pageGrid; }
    
    void drawGuideView(bool isText, string textOrSoundPath);
};

#endif /* LRComprehensionScene_hpp */

