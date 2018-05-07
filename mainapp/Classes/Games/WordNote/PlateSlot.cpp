//
//  PlateSlot.cpp
//  KitkitSchool-mobile
//
//  Created by JungJaehun on 21/02/2018.
//

#include "Piece.hpp"
#include "PlateSlot.hpp"

BEGIN_NS_WORDNOTE;

bool PlateSlot::init() {
    if (!Node::init()) return false;
    _registeredPieces.clear();
    return true;
}

void PlateSlot::setSlotSize(int letterLength) {
    Size slotSize;
    switch(letterLength) {
        case 1: slotSize = slotSingleSize; break;
        case 2: slotSize = slotDoubleSize; break;
        case 3: slotSize = slotTripleSize; break;
        case 4: slotSize = slotQuadrupleSize; break;
    }
    setContentSize(slotSize);
    setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    //setPosition(getContentSize()/2*-1);

    _letterLength = letterLength;

    //auto rectNode = DrawNode::create(); rectNode->drawRect(Vec2(this->getBoundingBox().getMinX(), this->getBoundingBox().getMinY()), Vec2(this->getBoundingBox().getMaxX(), this->getBoundingBox().getMaxY()), Color4F::RED); addChild(rectNode);
    
}

void PlateSlot::registerPiece(Piece *piece) {
    piece->_isOnPlateSlot = true;
    piece->_registeredPlateSlot = this;
    _registeredPieces.push_back(piece);
}

void PlateSlot::cleanPiecesOnPlateSlot() {
    int remainPieceFlag = true;
    //int maxZOrder = 0;
    
    auto it = _registeredPieces.begin();
    while (it != _registeredPieces.end()) {
        auto piece = (*it);
        if (piece->_isOnPlateSlot) {
            if (remainPieceFlag) {
                remainPieceFlag = false;
                it++; continue;
            }
        } else { it++; continue; }
        _registeredPieces.erase(it);
        piece->runAction(Sequence::create(
            FadeOut::create(0.5),
            CallFunc::create([piece](){ piece->removeFromParent(); }),
            nullptr
        ));

    }

}

void PlateSlot::setDisable() {
    for (auto piece : _registeredPieces) {
        piece->_touchEnabled = false;
    }
}

void PlateSlot::move(Vec2 pos) {
    this->setPosition(pos);
    for(auto piece : _registeredPieces) {
        piece->setPosition(pos);
    }
}

int PlateSlot::getLetterLength() { return _letterLength; }

END_NS_WORDNOTE;
