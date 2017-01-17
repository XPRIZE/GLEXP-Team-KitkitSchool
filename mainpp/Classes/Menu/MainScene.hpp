//
//  MainScene.hpp
//  enumaXprize
//
//  Created by Sungwoo Kang on 6/20/16.
//
//

#ifndef MainScene_hpp
#define MainScene_hpp

#include "cocos2d.h"
#include "ui/uiButton.h"

USING_NS_CC;


class Bird;

class MainScene: public Layer {
public:
    static cocos2d::Scene* createScene();
    
    MainScene();
    
    virtual bool init() override;
    virtual void onExitTransitionDidStart() override;
    
    CREATE_FUNC(MainScene);


    void setCurrentBird(Bird* bird);
    
    virtual void onEnter() override;
    
    void spawnGameEggs(float delay);
    

    
    
private:
    
    void checkDayClear();
    
    
    void clearGameButton();
    void addDebugGameButton(Node *parent, std::string gameName, Vector<ui::Button*>& iconVector);
    void addAllDebugGameButtons(char category, Vector<ui::Button*>& iconVector);
    
    void addGameButton(Node *parent, std::string levelID, int day, int gameIndex, Vector<ui::Button*>& iconVector);
    void addDayGameButtons(std::string levelID, int day, Vector<ui::Button*>& iconVector);
    void layoutGameButton(Node *parent, Vector<ui::Button*>& iconVector);
    
    
    Vector<Sprite*> clouds;
    
    ui::Button* matchingButton;
    
    Vector<ui::Button*> gameIcons;
    Bird* _bird;
    Sprite* _birdShadow;
    
    int _day;
    
    Node* _rootNode;
    float _rootScale;
    Node* _frameNode;
    
    
    
    Node *_skyNode;
    Node *_groundNode;
    Node *_leavesLeft, *_leavesRight;
    Node *_panelNode, *_dayNode;
    
    ui::Button *_quitButton;
    
    
    
    Node *_coopNode;
    
    Node *_debugView;
    
    bool _transitionBegins;
    
    Sprite* _coopHighlight;
//    EventListenerTouchOneByOne* birdEventListener;
};

#endif /* MainScene_hpp */
