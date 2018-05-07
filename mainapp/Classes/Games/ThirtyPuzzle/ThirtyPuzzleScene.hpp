//
//  ThirtyPuzzleScene.hpp
//  KitkitSchool-mobile
//
//  Created by JungJaehun on 07/02/2018.
//

#pragma once
#include "ThirtyPuzzleHeader.h"
#include "ProblemBank.hpp"
#include "Piece.hpp"

#include "Common/Controls/ProgressIndicator.hpp"

BEGIN_NS_THIRTYPUZZLE;

class ThirtyPuzzleScene : public Layer {
public:
    CREATE_FUNC(ThirtyPuzzleScene);
    static Scene* createScene(int levelID);

    virtual bool init() override;
    virtual void onEnter() override;
    virtual void onExit() override;

    void setLevel(int level);
    void goNextProblem();
    void setProblem();
    void checkAnswer(bool force=false);
    void writeActionLog(Piece& piece);
    
    vector<Piece*> getOverlappedPieces(Piece &piece, float distance = 200.f);
    void cleanUpOverlappedPieces(Piece &piece);
    void releaseColoredPiece(Piece &piece);
    void loadData(int level);
    void removeObjects();

private:
    int _currentLevel, _currentProblemIndex, _totalProblemCount;
    Node *_gameNode;
    Size _winSize, _gameSize;
    float _gameScale;
    vector<ThirtyPuzzleLevelStruct> _problems;
    vector<Piece*> _pieces;

    ProgressIndicator *_progressBar;

};

END_NS_THIRTYPUZZLE;
