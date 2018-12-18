//
//  BookWithQuiz.hpp
//  KitkitSchool
//
//  Created by HyeonGyu Yu on 24/07/2018.
//

#ifndef BookWithQuiz_hpp
#define BookWithQuiz_hpp

#include <stdio.h>
#include "cocos2d.h"
#include "ui/CocosGUI.h"

#include <vector>
#include <string>
#include <functional>
#include <algorithm>

USING_NS_CC;
using namespace cocos2d::ui;
using namespace std;

class BookWithQuiz : public Layer
{
public:
    enum class SceneState
    {
        StandBy = 0,
        Book,
        ComprehensionTest,
        NeedToPopThisScene,
    };
    
    CREATE_FUNC(BookWithQuiz);
    static Scene* createScene(string bookFolder, int comprehensionTestLevel);
    
    virtual bool init() override;
    virtual void onEnter() override;
    virtual void onEnterTransitionDidFinish() override;
    virtual void onExit() override;

    Node *_gameNode;
    
    static void setSceneState(SceneState toState);
    
private:
    static SceneState _currentState;
    
    string _bookFolder;
    int _comprehensionTestLevel;
    Size _winSize, _gameSize;
    
    void nextState();
    void startBook();
    void startComprehensionTest();
};


#endif /* BookWithQuiz_hpp */
