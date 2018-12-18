//
//  EggQuizPopup.hpp
//  KitkitSchool
//
//  Created by Gunho Lee on 1/10/17.
//
//

#pragma once

#include <stdio.h>
#include "cocos2d.h"
#include "Common/Controls/PopupBase.hpp"
#include "Managers/CurriculumManager.hpp"
#include "EggQuizHeader.hpp"

BEGIN_NS_EGGQUIZ;

class EggQuizPopup : public PopupBase
{
public:
    static EggQuizPopup* create(Node* parent);
    static EggQuizPopup* create(Node* parent, Size size);
    
protected:
    void setCommon(char category, int level);
    
    LevelCurriculum *_cur;
    
    
    
public:
    
    void setConfirmQuit(char category, int level);
    void setPreCompleted(char category, int level);
    void setPostCompleted(char category, int level);
    
    void setPreFailed(char category, int level);
    void setPostFailed(char category, int level);
    
    bool _isSpecialTest = false;
    
};

END_NS_EGGQUIZ;
