//
//  GradeSelector.hpp -- Temporary grade selector for debugging
//  TodoSchool on Oct 20, 2016
//
//  Copyright (c) 2016 Enuma, Inc. All rights reserved.
//  See LICENSE.md for more details.
//


#pragma once

#include <cocos/cocos2d.h>
#include <vector>
#include <functional>


class GradeSelector: public cocos2d::Node {
public:
    typedef cocos2d::Node Super;
    typedef std::vector<int> Choices;
    typedef std::function<cocos2d::Scene*(int)> Creator;

private:
    std::string gameName_;
    Choices choices_;
    //Creator creator_;

public:
    CREATE_FUNC(GradeSelector);
    GradeSelector();
    bool init() override;

    void setGameName(std::string gameName);
    void setChoices(const Choices& choices);
    //void setCreator(Creator creator);
    
    cocos2d::Scene* minimalSceneByWrapping();
    
public:
    // NB(xenosoz, 2016): Create a minimal scene containing a GradeSelector.
    static cocos2d::Scene* createScene();

private:
    void clearInternals();
    void refreshChildNodes();
    
    void handleSelection(int choice);
};
