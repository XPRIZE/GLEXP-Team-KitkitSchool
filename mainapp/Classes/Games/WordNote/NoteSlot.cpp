//
//  NoteSlot.cpp
//  KitkitSchool-mobile
//
//  Created by JungJaehun on 21/02/2018.
//

#include "NoteSlot.hpp"
#include "Piece.hpp"

BEGIN_NS_WORDNOTE;

const Size noteSlotSize = Size(160, 180);

bool NoteSlot::init() {
    if (!Node::init()) return false;
    _isMiddle = false;
    _placedPiece = nullptr;
    _letter = "";
    setContentSize(noteSlotSize);
    //setPosition(getContentSize()/2*-1);
    setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    return true;
}

void NoteSlot::setDisable() {
    _filter = LayerColor::create(Color4B(0, 0, 0, 128), noteSlotSize.width, noteSlotSize.height);
    addChild(_filter);
    _letter = "_";
}

void NoteSlot::setFadeOutFilter() {
    if (_letter == "_") _filter->runAction(FadeOut::create(0.5f));
}

void NoteSlot::setCoordinate(int x, int y) {
    _coordinateX = x;
    _coordinateY = y;
    //drawBoundingBox();
}

void NoteSlot::setMiddle(int previous, int next, int y) {
    _isMiddle = true;
    _previous = previous;
    _next = next;
    _coordinateX = -1;
    _coordinateY = y;
    //drawBoundingBox();
}

bool NoteSlot::isMiddle() { return _isMiddle; }
int NoteSlot::getCoordinateX() { return _coordinateX; }
int NoteSlot::getCoordinateY() { return _coordinateY; }
int NoteSlot::getPrevious() { return _previous; }
int NoteSlot::getNext() { return _next; }
string NoteSlot::getLetter() { return _letter; }
void NoteSlot::setLetter(string letter) { _letter = letter; }

void NoteSlot::drawBoundingBox() {
    Color4F boxColor = _isMiddle ? Color4F::GREEN : Color4F::BLUE;
    auto rectNode = DrawNode::create(); rectNode->drawRect(Vec2(this->getBoundingBox().getMinX(), this->getBoundingBox().getMinY()), Vec2(this->getBoundingBox().getMaxX(), this->getBoundingBox().getMaxY()), boxColor); addChild(rectNode);
}

END_NS_WORDNOTE;
