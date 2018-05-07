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
    
    virtual bool init() override;
    CREATE_FUNC(BookChoiceScene);
    

private:

    
};

#endif /* BookChoiceScene_hpp */
