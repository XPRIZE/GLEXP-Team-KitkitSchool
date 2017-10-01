//
//  GameSelectScene.hpp
//  enumaXprize
//
//  Created by Gunho Lee on 09/12/17.
//
//

#pragma once


#include "cocos2d.h"
#include "ui/UIButton.h"
#include "Common/Controls/TodoSchoolBackButton.hpp"

USING_NS_CC;


class Bird;
class CoinTab;

class GameSelectScene: public Layer {
public:
    static cocos2d::Scene* createScene();
    
    GameSelectScene();
    
    virtual bool init() override;
    virtual void onExitTransitionDidStart() override;
    
    CREATE_FUNC(GameSelectScene);


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
    
    std::string _levelID;
    int _day;
    
    Node* _rootNode;
    float _rootScale;
    Node* _frameNode;
    
    
    CoinTab *_coinTab;
    
    
    Node *_skyNode;
    Node *_groundNode;
    Node *_leavesLeft, *_leavesRight;
    Node *_panelNode, *_dayNode;
    
    TodoSchoolBackButton *_backButton;
    
  
    Node *_debugView;
    
    bool _transitionBegins;
    
   //    EventListenerTouchOneByOne* birdEventListener;
};


