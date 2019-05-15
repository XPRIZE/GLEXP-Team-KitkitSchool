//
//  LRDividedTypeReorderingScene.hpp
//  KitkitSchool
//
//  Created by HyeonGyu Yu on 14/08/2018.
//

#pragma once

#include "LRDividedTypeReorderingHeader.hpp"
#include "../LRComprehensionScene.hpp"
#include "ImageBlock.hpp"
#include "TextBlock.hpp"

BEGIN_NS_LR_DIVIDED_TYPE_REORDERING;

class LRDividedTypeReorderingScene : Layer
{
public:
    bool init() override;
    CREATE_FUNC(LRDividedTypeReorderingScene);
    static cocos2d::Layer* createLayer(GameType gameType, LRComprehensionScene *parent);
    void onEnter() override;
    
private:
    LRComprehensionScene* _comprehensionScene;
    //string _questionText;
    vector<string> _itemVector;
    Node* _gameNode;
    vector<ImageBlock*> _imageSlots;
    vector<TextBlock*> _textSlots;
    GameType _currentType;
    LRProblem* _problem;
        
    void initData();
    void createImageSlots();
    void createTextSlots(float wrapperNodeCorrectionY, float scaleFactor);
    Node* createImageBlock(string imageFile);
    Node* createTextBlock(int index, string text, float scaleFactor);
    bool isSolved();
    void onSolve();
    void createFixedResources();
    void determineItemType();
    void drawBlocksByGameType();
    
    void writePlayLog();
    string makeWorkPath();
};

END_NS_LR_DIVIDED_TYPE_REORDERING;
