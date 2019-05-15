//
//  NoteSlot.hpp
//  KitkitSchool-mobile
//
//  Created by JungJaehun on 21/02/2018.
//

#pragma once
#include "WordNoteHeader.h"

BEGIN_NS_WORDNOTE;
class Piece;
class NoteSlot : public Node {
public:
    CREATE_FUNC(NoteSlot);
    virtual bool init() override;
    void setCoordinate(int x, int y);
    void setMiddle(int previous, int next, int y);
    void drawBoundingBox();
    bool isMiddle();
    int getCoordinateX();
    int getCoordinateY();
    int getPrevious();
    int getNext();
    Piece* _placedPiece;
    void setLetter(string letter);
    string getLetter();
    void setDisable();
    void setFadeOutFilter();

private:
    string _letter;
    bool _isMiddle;
    int _coordinateX, _coordinateY;
    int _previous, _next;
    LayerColor* _filter;
};

END_NS_WORDNOTE;
