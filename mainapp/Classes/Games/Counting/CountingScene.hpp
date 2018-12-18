//
//  CountingScene.hpp
//
//  Created by Gunho Lee on 6/26/16.
//
//

#ifndef CountingScene_hpp
#define CountingScene_hpp

#include <stdio.h>
#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "ChildNodes/AnswerPadMulti.hpp"
#include "ChildNodes/CountingObject.hpp"
#include "Models/CountingLevelData.hpp"
#include "Models/CountingWorksheet.hpp"
#include "Models/CountingProblem.hpp"
#include <vector>
#include "Common/Controls/ProgressIndicator.hpp"

USING_NS_CC;
using namespace cocos2d::ui;

class CountingScene : Layer
{
public:
    static Scene* createScene(int levelID);
    virtual bool init() override;
    CREATE_FUNC(CountingScene);


    virtual void onEnter() override;
    virtual void onEnterTransitionDidFinish() override;
    void onStart();
    void onSolve();
    void onMiss();
    
    
    void setLevel(int level);
    void setTouchEnabled(bool enabled);
    
private:
    void putObjects(float& appearTime);
    
private:
    Node *_gameNode;
    AnswerPadMulti *_answerPad;
    Node *_objectNode;
    
    Point _answerPadPos, _answerPadOutPos;

    std::vector<CountingObject*> _objects;
    std::vector<Node*> _numbers;
    
    // NB(xenosoz, 2016): count(objects) vs sum(weights of objects)
    int _touchedObjects;
    int _touchedValue;
    
    
    int _currentLevel;
    int _currentSheetID;
    int _currentProblemID;
    todoschool::counting::Worksheet _currentWorksheet;
    todoschool::counting::Problem _currentProblem;
    
    bool _touchEnabled;
    
    ProgressIndicator *_progressBar;
    
    
    
    
 
};



#endif /* CountingScene_hpp */
