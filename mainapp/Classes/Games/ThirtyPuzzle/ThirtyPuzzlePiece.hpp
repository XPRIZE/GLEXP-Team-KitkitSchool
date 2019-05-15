//
//  Piece.hpp
//  KitkitSchool-mobile
//
//  Created by JungJaehun on 07/02/2018.
//

#pragma once
#include "ThirtyPuzzleHeader.h"

BEGIN_NS_THIRTYPUZZLE;

class Piece : public Node {
public:
    CREATE_FUNC(Piece);
    static int _maxZIndex;
    
    virtual bool init() override;
    
    void setAttribute(int number, float fontSize, bool fixed, bool inLastLine, int pieceColor = 0);
    void setAnswerPos(Vec2 answerPos);
    void setPicked(bool picked);
    void checkInLastLine();
    void reorderTouch();
    void reorderRelease();

    function<void()> onSnapEnded;
    function<void()> onTouchEnded;
    
    bool _touchEnabled, _snapped, _inLastLine;
    int _number;
    Sprite *_body, *_shadow, *_middle, *_top;
    Label *_label;
    Vec2 _answerPos;

};

END_NS_THIRTYPUZZLE;

