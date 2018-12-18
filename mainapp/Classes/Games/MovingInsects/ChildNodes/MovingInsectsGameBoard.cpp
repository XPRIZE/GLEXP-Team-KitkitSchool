//
//  GameBoard.cpp on Aug 4, 2016
//  TodoSchool
//
//  Copyright (c) 2016 Enuma, Inc. All rights reserved.
//  See LICENSE.md for more details.
//


#include "MovingInsectsGameBoard.h"
#include "../Utils/MovingInsectsMainDepot.h"


BEGIN_NS_MOVINGINSECTS

namespace {
    string contentSkin() { return MainDepot().assetPrefix() + "/GameBoard/moving_image_board.png"; }
    Size contentSize() { return Size(1116.f, 453.f); }
    string questionMarkSkin() { return MainDepot().assetPrefix() + "/GameBoard/question_mark.png"; }
    
    string defaultFont() { return MainDepot().defaultFont(); }
    float defaultFontSize() { return 270.f; }
    Color4B defaultFontColor() { return Color4B(216, 209, 183, 255); }

    float durationForCardMove() { return .3f; }
}  // unnamed namespace


GameBoard::GameBoard()
    : LeftExprLabel(nullptr)
    , OperatorLabel(nullptr)
    , RightExprLabel(nullptr)
    , EqualsLabel(nullptr)
    , QuestionMarkSprite(nullptr)
{
}

bool GameBoard::init() {
    if (!Super::init()) { return false; }
    
    clearInternals();
    refreshChildNodes();

    return true;
}

void GameBoard::showLeftExpr() {
    if (!LeftExprLabel) { return; }
    LeftExprLabel->setVisible(true);
}

void GameBoard::showOperator() {
    if (!OperatorLabel) { return; }
    OperatorLabel->setVisible(true);
}

void GameBoard::showRightExpr() {
    if (!RightExprLabel) { return; }
    RightExprLabel->setVisible(true);
}

void GameBoard::showEquals() {
    if (!EqualsLabel) { return; }
    EqualsLabel->setVisible(true);
}

void GameBoard::showQuestionMark() {
    if (!QuestionMarkSprite) { return; }
    QuestionMarkSprite->setVisible(true);
}


FiniteTimeAction* GameBoard::actionForPullCardToAnswerSlot(Card* C) {
    if (!C || !C->getParent()) { return CallFunc::create([]{}); }
    
    Point PointInHere((836.f + 1066.f) / 2.f, (66.f + 412.f - 6.f) / 2.f);
    Point PointInWorld = convertToWorldSpace(PointInHere);
    Point PointInCardParent = C->getParent()->convertToNodeSpace(PointInWorld);
    
    return MoveTo::create(durationForCardMove(), PointInCardParent);
}

void GameBoard::pullCardToAnswerSlot(Card* C) {
    if (!C || !C->getParent()) { return; }
    
    C->stopAllActions();
    C->runAction(C->movementGuard(actionForPullCardToAnswerSlot(C)));
}

Size GameBoard::defaultSize() {
    return contentSize();
}


void GameBoard::clearInternals() {
    setContentSize(contentSize());
    
    LeftExpr.OnValueUpdate = [this](string&) {
        refreshChildNodes();
    };
    Operator.OnValueUpdate = [this](string&) {
        refreshChildNodes();
    };
    RightExpr.OnValueUpdate = [this](string&) {
        refreshChildNodes();
    };
}

void GameBoard::refreshChildNodes() {
    removeAllChildren();
    Size CS = getContentSize();
    
    [&] {
        auto It = Sprite::create(contentSkin());
        It->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        It->setPosition(CS / 2.f);

        addChild(It);
        return It;
    }();

    
    LeftExprLabel = ([&] {
        auto It = Label::createWithTTF(LeftExpr(), defaultFont(), defaultFontSize());
        It->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
        It->setTextColor(defaultFontColor());
        It->setVisible(false);
        
        addChild(It);
        return It;
    }());
    
    OperatorLabel = ([&] {
        auto It = Label::createWithTTF(Operator(), defaultFont(), defaultFontSize());
        It->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
        It->setTextColor(defaultFontColor());
        It->setVisible(false);
        
        addChild(It);
        return It;
    }());
    
    RightExprLabel = ([&] {
        auto It = Label::createWithTTF(RightExpr(), defaultFont(), defaultFontSize());
        It->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
        It->setTextColor(defaultFontColor());
        It->setVisible(false);
        
        addChild(It);
        return It;
    }());
    
    EqualsLabel = ([&] {
        auto It = Label::createWithTTF("=", defaultFont(), defaultFontSize());
        It->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
        It->setTextColor(defaultFontColor());
        It->setVisible(false);
        
        addChild(It);
        return It;
    }());
    
    QuestionMarkSprite = ([&] {
        auto It = Sprite::create(questionMarkSkin());
        It->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        It->setPosition(Point(951.f, 240.f));
        It->setVisible(false);
        
        addChild(It);
        return It;
    }());
    
    if (true) {
        array<Label*, 4> LabelElems = {LeftExprLabel, OperatorLabel, RightExprLabel, EqualsLabel};
        
        float LabelNetWidth = 0.f;
        for (auto LE : LabelElems)
            LabelNetWidth += LE->getContentSize().width;

        float InterLabelWidth = (675.f - LabelNetWidth) / 3.f;
        float X = 100.f;
        
        for (auto LE : LabelElems) {
            LE->setPosition(Point(X, CS.height / 2.f - 25.f));
            X += LE->getContentSize().width + InterLabelWidth;
        }
    }
}

END_NS_MOVINGINSECTS
