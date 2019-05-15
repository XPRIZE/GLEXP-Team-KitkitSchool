//
//  DailyScene.hpp
//  KitkitSchool
//
//  Created by Gunho Lee on 09/10/17.
//
//

#pragma once


#include <cocos/ui/CocosGUI.h>
#include <functional>
#include <string>
#include "Common/Effects/LightBall.hpp"

USING_NS_CC;


class LevelCurriculum;
class Bird;
class CoinTab;


class DailyScene : public Node
{
public:
    static cocos2d::Scene* createScene(std::string levelID);
    
    static DailyScene* create(std::string levelID);
    bool init(std::string levelID);
    
    void refreshData();
    void setupMangoBoard();
    void setupFreechoiceTab();

    
    virtual void onEnter() override;
    void refreshScene();
    
    virtual void onExit() override;
    

    void setStartButtonPressed(bool pressed);
    void setStartButtonText(int day);
    void showChoiceTab(bool show);
    void showClear(int day);
    
    void dayChosen(int day);
    void dismiss();
    
    

private:
    
    void lightFall(LightBall *l, bool first);

    void showFreechoicePopup(std::string gameName, int maxPlayable, int maxAvailable);
    

    LevelCurriculum* _cur;
    
    //Node *_backView;
    Node *_bgView;
    Node *_mainView;

    
    class Mango : public Node {
    public:
        CREATE_FUNC(Mango);
        virtual bool init() override;
        
        void setupShape(int day, bool crown);
        
        void setTouched(bool touched);
        void setStatus(int status, bool animate = false); // 0 = done, 1 = current, 2 = not yet
        
        
    protected:
        bool _isCrown;
        int _currentStatus;
        
        Label *_label;
        Sprite *_body;
        Sprite *_backGlow;
        Sprite *_labelGlow;
        Sprite *_cover;
        
        
        
        
    };
    
    
    Node *_mangoBoard;
    Node *_panel;
    
    
    
    Node *_stage;
        Bird *_bird;
        //Sprite *_birdShadow;
        Node *_startBtnNode;
        Sprite *_startBtnSprite;
        Label *_startBtnLabel;
        Sprite *_startBtnGlow;
    
    
    
    CoinTab *_coinTab;
    
    Node *_choiceTab;
    Sprite *_choiceTabArrow;
    bool _choiceUp;
    
    
    // < gameName <maxAvailable, maxShow>>
    struct FreeChoiceInfo {
        int maxAvailable;
        int maxShow;
        bool isNew;
    };
    
    std::map<std::string, FreeChoiceInfo> _freechoiceGames;
    
    
    std::string _levelID;
    int _currentDay;
    bool _doneClearedDay;
    int _maxAvailableDay;
    
    
    
    
    bool _touchEnabled;
    

    

};




