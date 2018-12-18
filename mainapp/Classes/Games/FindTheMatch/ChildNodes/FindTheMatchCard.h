//
//  Card.h on Aug 3, 2016
//  TodoSchool
//
//  Copyright (c) 2016 Enuma, Inc. All rights reserved.
//  See LICENSE.md for more details.
//


#pragma once

#include "../Utils/FindTheMatchNS.h"


BEGIN_NS_FINDTHEMATCH


class Card: public Node {
    typedef Node Super;
    bool TouchEventEndedPrematually;

    Button* FrontFace;
    Sprite* TitleSprite;
    Label* TitleLabel;

    Button* BackFace;
    

public:
    Chain<bool> TouchEnabled;
    
    // NB(xenosoz, 2016): The title text is for a prototype only.
    Chain<string> TitleAsset;
    Chain<string> TitleText;

    function<void(Card&)> OnFrontFaceTouchDidBegin;
    function<void(Card&)> OnFrontFaceTouchDidCancel;
    function<void(Card&)> OnFrontFaceTouchDidEnd;
    function<void(Card&)> OnFrontFaceClicked;  // XXX: s/clicked/activated/
    function<void(Card&)> OnBackFaceClicked;


public:
    CREATE_FUNC(Card);
    Card();
    bool init() override;
    
    void setVisible(bool Visible) override;
    void setRotation3D(const Vec3& Rotation) override;

    FiniteTimeAction* movementGuard(FiniteTimeAction* Action);
    FiniteTimeAction* localZOrderGuard(FiniteTimeAction* Action);
    FiniteTimeAction* actionForFlipToFront();
    FiniteTimeAction* actionForFlipToBack();
    FiniteTimeAction* actionForVibration();
    FiniteTimeAction* actionForVibration(Point P);
    FiniteTimeAction* actionForShake();
    FiniteTimeAction* actionForShake(Point P);
    FiniteTimeAction* actionForCleanUpRotation();

    void setToFront();
    void setToBack();
    void flipToFront();
    void flipToBack();
    void shake();
    
public:
    static Size defaultSize();

private:
    void clearInternals();
    void refreshChildNodes();
    void refreshTitleSprite();
};


END_NS_FINDTHEMATCH
