//
//  BookChoiceScene.hpp
//  KitkitSchool
//
//  Created by Gunho Lee on 11/3/16.
//
//

#ifndef BookChoiceScene_hpp
#define BookChoiceScene_hpp

#include "cocos2d.h"
#include "ui/UIButton.h"

USING_NS_CC;

class BookChoiceScene: public Layer {
public:
    static cocos2d::Scene* createScene();
    static cocos2d::Scene* createSceneForBookWithQuiz();
    
    CREATE_FUNC(BookChoiceScene);
    
    virtual bool init() override;
    virtual void onEnter() override;
        
    bool forBookWithQuizScene;
    
private:
};

#endif /* BookChoiceScene_hpp */
