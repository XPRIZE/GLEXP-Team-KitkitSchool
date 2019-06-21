//
//  MainScene.hpp
//  KitkitSchool
//
//  Created by Sungwoo Kang on 6/20/16.
//
//

#ifndef MainScene_hpp
#define MainScene_hpp

#include "cocos2d.h"
#include "ui/UIButton.h"
#include <string>

USING_NS_CC;


class Bird;
class CoinTab;

class MainScene: public Layer {
public:
    static cocos2d::Scene* createScene();
    
    
    virtual bool init() override;
    virtual void onExitTransitionDidStart() override;
    virtual void resume() override;
    CREATE_FUNC(MainScene);


    
    virtual void onEnter() override;
    static const int MENU_SCENE_ID = 250;
    
private:
    
    void zoomCoop(Node *coop);
    void confirmDebug();
    void manageCache();
    void playLogoFall(float dt);
    void openAllLevels();
    
    void shakeCoop(Node *coop, int times = 100);
    void setDisableTouchEventForCoopGuide(bool disable);
    
    Vector<Sprite*> clouds;
    
 
    
    Node* _rootNode;
    float _rootScale;
    Node* _frameNode;
    
    
    
    Node *_skyNode;
    Node *_groundNode;
    Node *_leavesLeft, *_leavesRight;
    Node *_highlight;
    std::string _debugCommand;
    int _debugCoopNo;
    

    
    ui::Button *_quitButton;
    ui::Button *_coop1Node, *_coop2Node;
    
    Node *_debugView;
    
    bool _transitionBegins;
    
    
    // demo
    ui::Button *_resetBtn, *_openAllBtn, *_addCoinsBtn;
    Label * _reviewModeLabel;
    Label * _signLanguageModeLabel;
    CoinTab *_coinTab;
    

};

#endif /* MainScene_hpp */
