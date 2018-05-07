//
//  PlateSlot.hpp
//  KitkitSchool-mobile
//
//  Created by JungJaehun on 21/02/2018.
//

#pragma once
#include "WordNoteHeader.h"

BEGIN_NS_WORDNOTE;

class Piece;
class PlateSlot : public Node {
public:
    CREATE_FUNC(PlateSlot);
    virtual bool init() override;
    void setSlotSize(int letterLength);
    void registerPiece(Piece* piece);
    void move(Vec2 pos);
    void cleanPiecesOnPlateSlot();
    void setDisable();
    int getLetterLength();
    vector<Piece*> _registeredPieces;
private:
    int _letterLength;
};

END_NS_WORDNOTE;
