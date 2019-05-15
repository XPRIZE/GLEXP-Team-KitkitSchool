/////////////////////////////////////////////////////////////////////////////////////////
/****************************************************************************************
 Created        :    2018.02.13
 
 Author        :    Mikey.p
 
 Purpose        :    WhatIsThis card
 
 ****************************************************************************************/
/////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "cocos2d.h"

USING_NS_CC;

struct WhatIsThisCardData;

class WhatIsThisCard : public Node
{
public:
    

	enum class CardType
    {
        None = -1,
        Image,
        Text
    };
    
    virtual bool init() override;

	virtual void ThrowIn() {};
	virtual void ThrowOut() {};

	void SetBasePos(const Vec2& basePos) { _basePos = basePos; };
	Vec2 GetBasePos() const { return _basePos; }

protected:
	CardType _cardType = CardType::None;
	Vec2 _basePos = Vec2::ZERO;
};

class STTopCard : public WhatIsThisCard
{
public:
	CREATE_FUNC(STTopCard);
	virtual bool init() override ;

	virtual void ThrowIn();
	virtual void ThrowOut();

	void SetCard(const WhatIsThisCardData& data, const CardType& cardType);
  void SetFail(const bool& fail);

};

class STDownCard : public WhatIsThisCard
{
public:
	CREATE_FUNC(STDownCard);
	virtual bool init() override;

	void SetCard(const WhatIsThisCardData& data, const CardType& cardType);
	void Flip(const bool& success, const std::function<void()>& cbf);
	void Fail(const bool& hit, const std::function<void()>& cbf);

	virtual void ThrowIn();
	virtual void ThrowOut();

    void loadDurationsheet();
    std::map<std::string,float> _duration;
    float _soundDuration = 0.f;

	void SetClickEvent(const std::function<void()>& clickEvent) { _clickEvent = clickEvent; }

protected:

	Node* _frontNode = nullptr;
	Node* _backNode = nullptr;
	std::string _soundFile;

	std::function<void()> _clickEvent = nullptr;
};
