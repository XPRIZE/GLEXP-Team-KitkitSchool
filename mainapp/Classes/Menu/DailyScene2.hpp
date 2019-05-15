//
//  DailyScene2.hpp
//  KitkitSchool
//
//  Created by Gunho Lee on 03/26/18.
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


class DailyScene2 : public Node
{
public:
    static cocos2d::Scene* createScene(std::string levelID);
    
    static DailyScene2* create(std::string levelID);
    bool init(std::string levelID);
    
    void refreshData();
    void setupMangoBoard();
    void setupFreechoiceTab();

    
    virtual void onEnter() override;
    void refreshScene();
    
    virtual void onExit() override;
    

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
    
    bool _freeChoiceStartEnabled = true;

    
    class Mango : public Node {
    public:
        CREATE_FUNC(Mango);
        virtual bool init() override;
        
        void setupShape(int birdID, std::string levelID, int day, bool crown, bool quiz);
        
        void setTouched(bool touched);
        //void setStatus(int status, bool animate = false); // 0 = done, 1 = current, 2 = not yet
        //void setStatus(int time, bool isCleared, bool isAvailable, bool withCrown, bool animate = false);
        
        void refresh(bool animate = false);
        
        void setAvailable(bool available) { _isAvailable = available; };
        
    protected:
        int _birdID;
        std::string _levelID;
        int _day;
        bool _isCrown;
        bool _isQuiz;
        bool _isSpecialCourse;
        bool _isAvailable;
        bool _isFirstSetup;
        
        Label *_label;
        Sprite *_body;
        Sprite *_topGlow;
        Sprite *_backGlow;
//        Sprite *_labelGlow;
        Sprite *_cover;
        
        Vector<SpriteFrame*> _spriteFrameForSC;
        Vector<SpriteFrame*> _spriteFrameTopGlowForGC;
        void loadSpriteFrameForSC(bool isCrown); 
    };
    
    
    Node *_mangoBoard;
    Node *_panel;
    
    
    
//    Node *_stage;
//        Bird *_bird;
//        Node *_startBtnNode;
//        Sprite *_startBtnSprite;
//        Label *_startBtnLabel;
//        Sprite *_startBtnGlow;
    
    
    
    CoinTab *_coinTab;
    
    Node *_choiceTab;
    Sprite *_choiceTabArrow;
    bool _choiceUp;
    bool _choiceTabJumpEnabled;
    
    
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
    bool _isSpecialCourse;
    
    
    
    
    bool _touchEnabled;
    

    

};




