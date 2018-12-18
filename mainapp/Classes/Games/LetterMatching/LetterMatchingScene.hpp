//
//  GameScene.hpp
//  Matching
//
//  Created by sugar on 5/18/16.
//
//

#ifndef LetterMatchingScene_hpp
#define LetterMatchingScene_hpp

#include <stdio.h>
#include "LetterMatchingCard.hpp"

#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"

#include "Models/LetterMatchingLevelData.hpp"
#include "Models/LetterMatchingWorksheet.hpp"
#include "Common/Controls/ProgressIndicator.hpp"


USING_NS_CC;
using namespace cocos2d::ui;

class LetterMatchingScene : Layer
{
public:
    static Scene* createScene(int levelID);
    virtual bool init() override;

    virtual void onEnter() override;
    virtual void onEnterTransitionDidFinish() override;
    void setLevel(int levelID);
    
    CREATE_FUNC(LetterMatchingScene);


private:
    float _defaultScaleFactor;
    float _upScaleFactor;

    int _currentLevelID;
    int _currentSheetID;
    todoschool::lettermatching::Worksheet _currentWorksheet;
    int _currentProblemID;
    todoschool::lettermatching::Problem _currentProblem;

    ProgressIndicator *_progressBar;
    Node* _gameNode;
    int _currentLevelMaxCardCount = 0;
    int _matchCount = 0;
    int _zOrder = 10;

    cocos2d::ui::Button* _nextButton;
    
    LetterMatchingCard* createMatchingCard(int zOrder, Point pos,
                                           int level, int id, int type,
                                           const std::string& cardImageName);
    void bindingEvents(LetterMatchingCard *card);
    void initCardList();
    void addStarParticle(Node* targetNode);

    Point safePointForBoundary(Point point);
    void stompByNode(Node* node);
    
    bool _nextTransition;
    
public:
    std::vector<LetterMatchingCard*> matchingCardList;
    
    void onStart();
    void onSolve();
    void advanceToNextProblem();

};
#endif /* LetterMatchingScene_hpp */
