//
//  LRDividedTypeFillTheBlanksScene.hpp
//  KitkitSchool
//
//  Created by Gunho on 13/12/2016.
//
//

#pragma once

#include "LRDividedTypeFillTheBlanksHeader.hpp"
#include "../LRComprehensionScene.hpp"
#include "ProblemData.hpp"
#include "WordBlock.hpp"

BEGIN_NS_LR_DIVIDED_TYPE_FILL_THE_BLANKS;

class LRDividedTypeFillTheBlanksScene : Layer
{
public:
    bool init() override;
    CREATE_FUNC(LRDividedTypeFillTheBlanksScene);
    static cocos2d::Scene* createScene();
    static cocos2d::Layer* createLayer(LRComprehensionScene *parent);
    LRDividedTypeFillTheBlanksScene();
    void onEnter() override;
    void onExit() override;
    bool isSolved();
    void onSolve();
    
private:
    LRComprehensionScene* _comprehensionScene;
    
    Size _maxWordSize;
    float _pageScale;
    
    Node *_gameNode;
    vector<WordBlock*> _slots;
    vector<Point> _randomPoints;
    
    Size calculateMaxWordSize();
    Node* createPage(float pageWidth);
    Node* createWordBlock(std::string word);

    void initData();
    void drawFixedResources();
    void createRandomAreas();
    
    void writePlayLog();
    std::string makeWorkPath();
};

END_NS_LR_DIVIDED_TYPE_FILL_THE_BLANKS;
