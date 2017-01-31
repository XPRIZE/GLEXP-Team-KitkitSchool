//
//  OrderedPuzzleScene.h -- A puzzle game with "complete one piece, get a new piece" rule.
//  TodoSchool on Aug 29, 2016
//
//  Copyright (c) 2016 Enuma, Inc. All rights reserved.
//  See LICENSE.md for more details.
//


#pragma once

#include "../ChildNodes/GameBoard.h"
#include "../ChildNodes/PlayField.h"
#include "../ChildNodes/ShadowField.h"
#include "../ChildNodes/DeckBase.h"
#include "../Stores/WoodenPuzzleStore.h"
#include "../Utils/WoodenPuzzleNS.h"


BEGIN_NS_WOODENPUZZLES

class OrderedPuzzleScene: public Scene {
    typedef Scene Super;

    Node* TheGameNode;
    GameBoard* TheGameBoard;
    DeckBase* TheDeckBase;
    PlayField* ThePlayField;
    ShadowField* TheShadowField;

    size_t AnswerCount;
    shared_ptr<WoodenPuzzleStore> ThePuzzleStore;
    
public:
    function<void()> OnSuccess;
    function<void()> OnFail;

public:
    CREATE_FUNC(OrderedPuzzleScene);
    OrderedPuzzleScene();
    bool init() override;
    
    void grabPuzzleStore(WoodenPuzzleStore* Store);

private:
    void clearInternals();
    void refreshChildNodes();
    
    void fillWoodPiecesToMaxCount(PlayField* Field);
    void refillWoodPiecesToMaxCount();
    void attachGameLogicToWoodPiece(WoodPiece* Piece);
    
    void handleCorrectAnswer();
    void handleSuccess();
    void handleFail();
};

END_NS_WOODENPUZZLES