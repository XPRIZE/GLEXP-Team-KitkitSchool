//
//  FreePuzzleScene.h -- A puzzle game with all-pieces, free movement
//  TodoSchool on Aug 11, 2016
//
//  Copyright (c) 2016 Enuma, Inc. All rights reserved.
//  See LICENSE.md for more details.
//


#pragma once

#include "../ChildNodes/GameBoard.h"
#include "../ChildNodes/PlayField.h"
#include "../ChildNodes/ShadowField.h"
#include "../Stores/WoodenPuzzleStore.h"
#include "../Utils/WoodenPuzzleNS.h"

BEGIN_NS_WOODENPUZZLES;


class FreePuzzleScene : public Scene {
    typedef Scene Super;

    Node* TheGameNode;
    GameBoard* TheGameBoard;
    PlayField* ThePlayField;
    ShadowField* TheShadowField;

    size_t AnswerCount;
    shared_ptr<WoodenPuzzleStore> ThePuzzleStore;

public:
    function<void()> OnSuccess;
    function<void()> OnFail;

public:
    CREATE_FUNC(FreePuzzleScene);
    FreePuzzleScene();
    bool init() override;
    
    void grabPuzzleStore(WoodenPuzzleStore* Store);
    
private:
    void clearInternals();
    void refreshChildNodes();
    
    void attachGameLogicToWoodPiece(WoodPiece* Piece);
    
    void handleCorrectAnswer();
    void handleSuccess();
    void handleFail();
};


END_NS_WOODENPUZZLES;