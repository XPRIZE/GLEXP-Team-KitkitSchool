//
//  WordKicker_NodeBirdKicker.cpp
//  KitkitSchool
//
//  Created by MilliSoftGames on 8/25/18.
//


#include "WordKicker_NodeBirdKicker.hpp"

#include "Utils/TodoUtil.h"


BEGIN_NS_WORDKICKER;

bool WordKicker_NodeBirdKicker::init()
{
	if (!Node::init()) return false;
	return true;
}

void WordKicker_NodeBirdKicker::removeObjects()
{
	_spriteShadow = nullptr;
	_spriteBirdKicker = nullptr;

	_node->removeAllChildren();
	_node = nullptr;

	removeAllChildren();
}

void WordKicker_NodeBirdKicker::createSprites()
{
	_node = Node::create();
	_node->setPosition(kicker_x, kicker_y);
	addChild(_node);

	_spriteShadow = Sprite::create(resourcePath + "shadow.png");
	_spriteShadow->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	_spriteShadow->setPosition(0, 0);
	_node->addChild(_spriteShadow);

	_spriteBirdKicker = Sprite::create(resourcePath + "gif/kicker/1/kicker_1_1-7.png");
	_spriteBirdKicker->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
	_spriteBirdKicker->setPosition(0, -25);
	_node->addChild(_spriteBirdKicker);

	startNewProblem();
}

void WordKicker_NodeBirdKicker::startNewProblem()
{
	_node->setPosition(kicker_x, kicker_y);

	_curAnimation = -1;
	setAnimation_1_idle();
}

void WordKicker_NodeBirdKicker::stopAnimation()
{
	if (_currentActionAnimation != nullptr)
	{
		_spriteBirdKicker->stopAction(_currentActionAnimation);
	}
	_currentActionAnimation = nullptr;
	_curAnimation = -1;
}

void WordKicker_NodeBirdKicker::setAnimation_1_idle()
{
	if (_curAnimation == 1)
	{
		return;
	}
	stopAnimation();
	_curAnimation = 1;

	Rect textureRect = _spriteBirdKicker->getTextureRect();
	Vector<SpriteFrame*> animFrames;

	animFrames.clear();
	animFrames.pushBack(SpriteFrame::create(resourcePath + "gif/kicker/1/kicker_1_1-7.png", textureRect));
	animFrames.pushBack(SpriteFrame::create(resourcePath + "gif/kicker/1/kicker_1_2.png", textureRect));
	animFrames.pushBack(SpriteFrame::create(resourcePath + "gif/kicker/1/kicker_1_3.png", textureRect));
	animFrames.pushBack(SpriteFrame::create(resourcePath + "gif/kicker/1/kicker_1_4.png", textureRect));
	animFrames.pushBack(SpriteFrame::create(resourcePath + "gif/kicker/1/kicker_1_5.png", textureRect));
	animFrames.pushBack(SpriteFrame::create(resourcePath + "gif/kicker/1/kicker_1_6.png", textureRect));
	animFrames.pushBack(SpriteFrame::create(resourcePath + "gif/kicker/1/kicker_1_1-7.png", textureRect));
	Animation* animation = Animation::createWithSpriteFrames(animFrames, 0.1f);
	animation->setLoops(-1);

	_currentActionAnimation = _spriteBirdKicker->runAction(Animate::create(animation));
}

void WordKicker_NodeBirdKicker::setAnimation_2_kick_success()
{
	/*
	if (_curAnimation == 2)
	{
		return;
	}
	*/
	stopAnimation();
	_curAnimation = 2;

	Rect textureRect = _spriteBirdKicker->getTextureRect();
	Vector<SpriteFrame*> animFrames;

	animFrames.clear();
	animFrames.pushBack(SpriteFrame::create(resourcePath + "gif/kicker/2/kicker_2_1-4.png", textureRect));
	animFrames.pushBack(SpriteFrame::create(resourcePath + "gif/kicker/2/kicker_2_2.png", textureRect));
	animFrames.pushBack(SpriteFrame::create(resourcePath + "gif/kicker/2/kicker_2_3.png", textureRect));
	animFrames.pushBack(SpriteFrame::create(resourcePath + "gif/kicker/2/kicker_2_4.png", textureRect));
	animFrames.pushBack(SpriteFrame::create(resourcePath + "gif/kicker/2/kicker_2_5.png", textureRect));
	animFrames.pushBack(SpriteFrame::create(resourcePath + "gif/kicker/2/kicker_2_6.png", textureRect));
	animFrames.pushBack(SpriteFrame::create(resourcePath + "gif/kicker/2/kicker_2_7.png", textureRect));
	animFrames.pushBack(SpriteFrame::create(resourcePath + "gif/kicker/2/kicker_2_8.png", textureRect));
	animFrames.pushBack(SpriteFrame::create(resourcePath + "gif/kicker/2/kicker_2_9.png", textureRect));
	animFrames.pushBack(SpriteFrame::create(resourcePath + "gif/kicker/2/kicker_2_10.png", textureRect));
	Animation* animation = Animation::createWithSpriteFrames(animFrames, 0.1f);
	animation->setLoops(1);

	Vector<SpriteFrame*> animFrames2;
	animFrames2.clear();
	animFrames2.pushBack(SpriteFrame::create(resourcePath + "gif/kicker/2/kicker_2_1-4.png", textureRect));
	animFrames2.pushBack(SpriteFrame::create(resourcePath + "gif/kicker/2/kicker_2_2.png", textureRect));
	animFrames2.pushBack(SpriteFrame::create(resourcePath + "gif/kicker/2/kicker_2_3.png", textureRect));
	animFrames2.pushBack(SpriteFrame::create(resourcePath + "gif/kicker/2/kicker_2_2.png", textureRect));
	Animation* animation2 = Animation::createWithSpriteFrames(animFrames2, 0.1f);
	animation2->setLoops(-1);

	_currentActionAnimation = _spriteBirdKicker->runAction(
		Sequence::create(
			Animate::create(animation),
			Animate::create(animation2),
			nullptr
		)
	);
}

void WordKicker_NodeBirdKicker::setAnimation_3_kick_fail()
{
	/*
	if (_curAnimation == 3)
	{
		return;
	}
	*/
	stopAnimation();
	_curAnimation = 3;

	Rect textureRect = _spriteBirdKicker->getTextureRect();
	Vector<SpriteFrame*> animFrames;

	animFrames.clear();
	animFrames.pushBack(SpriteFrame::create(resourcePath + "gif/kicker/3/kicker_3_1-5.png", textureRect));
	animFrames.pushBack(SpriteFrame::create(resourcePath + "gif/kicker/3/kicker_3_2.png", textureRect));
	animFrames.pushBack(SpriteFrame::create(resourcePath + "gif/kicker/3/kicker_3_3.png", textureRect));
	animFrames.pushBack(SpriteFrame::create(resourcePath + "gif/kicker/3/kicker_3_4.png", textureRect));
	animFrames.pushBack(SpriteFrame::create(resourcePath + "gif/kicker/3/kicker_3_1-5.png", textureRect));
	Animation* animation = Animation::createWithSpriteFrames(animFrames, 0.1f);
	animation->setLoops(1);

	Vector<SpriteFrame*> animFrames2;
	animFrames2.clear();
	animFrames2.pushBack(SpriteFrame::create(resourcePath + "gif/kicker/1/kicker_1_1-7.png", textureRect));
	animFrames2.pushBack(SpriteFrame::create(resourcePath + "gif/kicker/1/kicker_1_2.png", textureRect));
	animFrames2.pushBack(SpriteFrame::create(resourcePath + "gif/kicker/1/kicker_1_3.png", textureRect));
	animFrames2.pushBack(SpriteFrame::create(resourcePath + "gif/kicker/1/kicker_1_4.png", textureRect));
	animFrames2.pushBack(SpriteFrame::create(resourcePath + "gif/kicker/1/kicker_1_5.png", textureRect));
	animFrames2.pushBack(SpriteFrame::create(resourcePath + "gif/kicker/1/kicker_1_6.png", textureRect));
	animFrames2.pushBack(SpriteFrame::create(resourcePath + "gif/kicker/1/kicker_1_1-7.png", textureRect));
	Animation* animation2 = Animation::createWithSpriteFrames(animFrames2, 0.1f);
	animation2->setLoops(-1);

	_currentActionAnimation = _spriteBirdKicker->runAction(
		Sequence::create(
			Animate::create(animation),
			Animate::create(animation2),
			nullptr
		)
	);
}

void WordKicker_NodeBirdKicker::setAnimation_debug_next()
{
	if (_curAnimation == 1)
	{
		setAnimation_2_kick_success();
	}
	else if (_curAnimation == 2)
	{
		setAnimation_3_kick_fail();
	}
	else
	{
		setAnimation_1_idle();
	}
}

END_NS_WORDKICKER;