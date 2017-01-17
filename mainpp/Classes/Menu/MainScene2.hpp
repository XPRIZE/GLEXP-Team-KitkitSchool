//
//  MainScene2.hpp
//  enumaXprize
//
//  Created by Sungwoo Kang on 11/3/16.
//
//

#ifndef MainScene2_hpp
#define MainScene2_hpp


#include "cocos2d.h"
#include "ui/uiButton.h"

USING_NS_CC;


class Bird;
class DailyData;

class MainScene2: public Layer {
public:
    
    enum MainSceneMode {
        MODE_SHOW_POPUP = 0,
        MODE_SELECT_COOP,
        MODE_SELECT_GAME
    } _mode;
    
    static cocos2d::Scene* createScene();
    
    MainScene2();
    
    virtual bool init() override;
    virtual void onExitTransitionDidStart() override;
    
    CREATE_FUNC(MainScene2);
    
    
    
    void setCurrentBird(Bird* bird);
    
    virtual void onEnter() override;
    
    void spawnGameEggs(float delay);
    
    
    void layoutGameButton(Node *parent, Vector<ui::Button*>& iconVector);
    
    void setMode(MainSceneMode mode);
private:
//    void addGameButton(Node *parent, std::string gameName, Vector<ui::Button*>& iconVector, std::function<Scene*(void)> creator);
    void addGameButton(Node *parent, std::string gameName, int level, Vector<ui::Button*>& iconVector);


    Vector<Sprite*> clouds;
    
    ui::Button* matchingButton;
    
    std::vector<Vector<ui::Button*>> gameIcons;
    Bird* _bird;
    Sprite* _birdShadow;
    Node* _rootNode;
    
    Node *_skyNode;
    Node *_groundNode;
    Node *_leavesLeft, *_leavesRight;
    
    Node *_coopNode;
    Node *_infoBg;
    
    
    bool _transitionBegins;
    
    Sprite* _coopHighlight;
    
    //    EventListenerTouchOneByOne* birdEventListener;
};

#endif /* MainScene2_hpp */
