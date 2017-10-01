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
#include "ui/UIButton.h"

USING_NS_CC;


class Bird;

class MainScene: public Layer {
public:
    static cocos2d::Scene* createScene();
    
    
    virtual bool init() override;
    virtual void onExitTransitionDidStart() override;
    
    CREATE_FUNC(MainScene);


    
    virtual void onEnter() override;
    


    
    
private:
    
    void zoomCoop();

    
    


    
    
    Vector<Sprite*> clouds;
    
 
    
    Node* _rootNode;
    float _rootScale;
    Node* _frameNode;
    
    
    
    Node *_skyNode;
    Node *_groundNode;
    Node *_leavesLeft, *_leavesRight;

    
    ui::Button *_quitButton;
    
    
    
    Node *_coopNode;
    
    Node *_debugView;
    
    bool _transitionBegins;
    

};

#endif /* MainScene_hpp */
