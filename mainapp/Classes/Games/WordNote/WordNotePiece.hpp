//
//  Piece.hpp
//  KitkitSchool-mobile
//
//  Created by JungJaehun on 21/02/2018.
//

#pragma once
#include "WordNoteHeader.h"

BEGIN_NS_WORDNOTE;

class NoteSlot;
class PlateSlot;
class Piece : public Sprite {
public:
    CREATE_FUNC(Piece);
    virtual bool init() override;
    static int _serialCounter;

    void setLetter(string letter);
    void place(NoteSlot *noteSlot);
    void togglePicked(bool picked);
    
    string getLetter();
    NoteSlot *_placedNoteSlot;
    PlateSlot *_registeredPlateSlot;
    bool _isOnPlateSlot, _correctActionFlag;
    bool _touchEnabled;

    function<void()> onTouchBegan;
    function<void(Touch* touch)> onTouchMoved;
    function<void(Touch* touch)> onTouchEnded;

    int _serialNumber;
    Sprite* _paper;
    vector<Label*> _letters;

    void createShiningParticle();
    void startParticle();
    void stopParticle();
    void fadeOutLetter();
    void setTouchEnable(bool enable);
    Node* _shiningParticleNode;

private:
    string _letter;
    int _fadeOutCount;
};

END_NS_WORDNOTE;
