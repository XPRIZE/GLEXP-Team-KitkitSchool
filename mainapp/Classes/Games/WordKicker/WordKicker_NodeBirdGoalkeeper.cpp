//
//  WordKicker_NodeBirdGoalkeeper.cpp
//  KitkitSchool
//
//  Created by MilliSoftGames on 8/25/18.
//


#include "WordKicker_NodeBirdGoalkeeper.hpp"

#include "Utils/TodoUtil.h"


BEGIN_NS_WORDKICKER;

bool WordKicker_NodeBirdGoalkeeper::init()
{
	if (!Node::init()) return false;
	return true;
}

void WordKicker_NodeBirdGoalkeeper::removeObjects()
{
	_spriteShadow = nullptr;
	_spriteBirdGoalkeeper = nullptr;

	_node->removeAllChildren();
	_node = nullptr;

	removeAllChildren();
}

void WordKicker_NodeBirdGoalkeeper::createSprites()
{
	_node = Node::create();
	_node->setPosition(goalkeeper_x, goalkeeper_y);
	addChild(_node);

	_spriteShadow = Sprite::create(resourcePath + "shadow.png");
	_spriteShadow->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	_spriteShadow->setPosition(0, 0);
	_node->addChild(_spriteShadow);

	_spriteFeather = Sprite::create(resourcePath + "gif/goalkeeper/feather/goalkeeper_feather_1.png");
	_spriteFeather->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
	_spriteFeather->setPosition(0, -25);
	_node->addChild(_spriteFeather);
	_spriteFeather->setOpacity(0);

	_spriteBirdGoalkeeper = Sprite::create(resourcePath + "gif/goalkeeper/5/goalkeeper_5_1-4-7-9.png");
	_spriteBirdGoalkeeper->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	_spriteBirdGoalkeeper->setPosition(0, _spriteBirdGoalkeeper->getContentSize().width/2 - 25);
	_node->addChild(_spriteBirdGoalkeeper);

	_spriteEyes = Sprite::create(resourcePath + "gif/goalkeeper/7_1/goalkeeper_7_eyes.png");
	_spriteEyes->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
	_spriteEyes->setPosition(_spriteBirdGoalkeeper->getContentSize().width/2, 0);
	_spriteBirdGoalkeeper->addChild(_spriteEyes);
	_spriteEyes->setOpacity(0);
	


	startNewProblem();
}

void WordKicker_NodeBirdGoalkeeper::startNewProblem()
{
	_node->setPosition(goalkeeper_x, goalkeeper_y);
	_spriteShadow->setPosition(0, 0);
	_spriteBirdGoalkeeper->setPosition(0, _spriteBirdGoalkeeper->getContentSize().width / 2 - 25);
	_spriteBirdGoalkeeper->setGlobalZOrder(0);

	_curAnimation = -1;
	setAnimation_5_idle();
}

void WordKicker_NodeBirdGoalkeeper::stopAnimation()
{
	if (_currentActionAnimation != nullptr)
	{
		_spriteBirdGoalkeeper->stopAction(_currentActionAnimation);
	}
	_currentActionAnimation = nullptr;
	_curAnimation = -1;

	_spriteFeather->setOpacity(0);
	_spriteEyes->setOpacity(0);
}

void WordKicker_NodeBirdGoalkeeper::setAnimation_5_idle()
{
	if (_curAnimation == 5)
	{
		return;
	}
	stopAnimation();
	_curAnimation = 5;

	Rect textureRect = _spriteBirdGoalkeeper->getTextureRect();
	Vector<SpriteFrame*> animFrames;

	animFrames.clear();
	animFrames.pushBack(SpriteFrame::create(resourcePath + "gif/goalkeeper/5/goalkeeper_5_1-4-7-9.png", textureRect));
	animFrames.pushBack(SpriteFrame::create(resourcePath + "gif/goalkeeper/5/goalkeeper_5_2.png", textureRect));
	animFrames.pushBack(SpriteFrame::create(resourcePath + "gif/goalkeeper/5/goalkeeper_5_3.png", textureRect));
	animFrames.pushBack(SpriteFrame::create(resourcePath + "gif/goalkeeper/5/goalkeeper_5_1-4-7-9.png", textureRect));
	animFrames.pushBack(SpriteFrame::create(resourcePath + "gif/goalkeeper/5/goalkeeper_5_5-8.png", textureRect));
	animFrames.pushBack(SpriteFrame::create(resourcePath + "gif/goalkeeper/5/goalkeeper_5_6.png", textureRect));
	animFrames.pushBack(SpriteFrame::create(resourcePath + "gif/goalkeeper/5/goalkeeper_5_1-4-7-9.png", textureRect));
	animFrames.pushBack(SpriteFrame::create(resourcePath + "gif/goalkeeper/5/goalkeeper_5_5-8.png", textureRect));
	animFrames.pushBack(SpriteFrame::create(resourcePath + "gif/goalkeeper/5/goalkeeper_5_1-4-7-9.png", textureRect));
	Animation* animation = Animation::createWithSpriteFrames(animFrames, 0.1f);
	animation->setLoops(-1);

	_currentActionAnimation = _spriteBirdGoalkeeper->runAction(Animate::create(animation));
}

void WordKicker_NodeBirdGoalkeeper::setAnimation_6_speak()
{
	if (_curAnimation == 6)
	{
		return;
	}
	stopAnimation();
	_curAnimation = 6;

	Rect textureRect = _spriteBirdGoalkeeper->getTextureRect();
	Vector<SpriteFrame*> animFrames;

	animFrames.clear();
	animFrames.pushBack(SpriteFrame::create(resourcePath + "gif/goalkeeper/6/goalkeeper_6_1-11.png", textureRect));
	animFrames.pushBack(SpriteFrame::create(resourcePath + "gif/goalkeeper/6/goalkeeper_6_2.png", textureRect));
	animFrames.pushBack(SpriteFrame::create(resourcePath + "gif/goalkeeper/6/goalkeeper_6_3.png", textureRect));
	animFrames.pushBack(SpriteFrame::create(resourcePath + "gif/goalkeeper/6/goalkeeper_6_4-6-8-10.png", textureRect));
	animFrames.pushBack(SpriteFrame::create(resourcePath + "gif/goalkeeper/6/goalkeeper_6_5-7-9.png", textureRect));
	animFrames.pushBack(SpriteFrame::create(resourcePath + "gif/goalkeeper/6/goalkeeper_6_4-6-8-10.png", textureRect));
	animFrames.pushBack(SpriteFrame::create(resourcePath + "gif/goalkeeper/6/goalkeeper_6_5-7-9.png", textureRect));
	animFrames.pushBack(SpriteFrame::create(resourcePath + "gif/goalkeeper/6/goalkeeper_6_4-6-8-10.png", textureRect));
	animFrames.pushBack(SpriteFrame::create(resourcePath + "gif/goalkeeper/6/goalkeeper_6_5-7-9.png", textureRect));
	animFrames.pushBack(SpriteFrame::create(resourcePath + "gif/goalkeeper/6/goalkeeper_6_4-6-8-10.png", textureRect));
	Animation* animation = Animation::createWithSpriteFrames(animFrames, 0.1f);
	animation->setLoops(-1);

	_currentActionAnimation = _spriteBirdGoalkeeper->runAction(Animate::create(animation));
}

void WordKicker_NodeBirdGoalkeeper::setAnimation_7_1_goal()
{
	if (_curAnimation == 71)
	{
		return;
	}
	stopAnimation();
	_curAnimation = 71;

	Rect textureRect = _spriteBirdGoalkeeper->getTextureRect();
	Vector<SpriteFrame*> animFrames;

	animFrames.clear();
	animFrames.pushBack(SpriteFrame::create(resourcePath + "gif/goalkeeper/7_1/goalkeeper_7_1-5.png", textureRect));
	animFrames.pushBack(SpriteFrame::create(resourcePath + "gif/goalkeeper/7_1/goalkeeper_7_2.png", textureRect));
	animFrames.pushBack(SpriteFrame::create(resourcePath + "gif/goalkeeper/7_1/goalkeeper_7_3.png", textureRect));
	animFrames.pushBack(SpriteFrame::create(resourcePath + "gif/goalkeeper/7_1/goalkeeper_7_4.png", textureRect));
	animFrames.pushBack(SpriteFrame::create(resourcePath + "gif/goalkeeper/7_1/goalkeeper_7_1-5.png", textureRect));
	Animation* animation = Animation::createWithSpriteFrames(animFrames, 0.1f);
	animation->setLoops(-1);

	_currentActionAnimation = _spriteBirdGoalkeeper->runAction(Animate::create(animation));
	_spriteEyes->setOpacity(255);
}

void WordKicker_NodeBirdGoalkeeper::setAnimation_7_2_goal()
{
	if (_curAnimation == 72)
	{
		return;
	}
	stopAnimation();
	_curAnimation = 72;

	Rect textureRect = _spriteBirdGoalkeeper->getTextureRect();
	Vector<SpriteFrame*> animFrames;

	animFrames.clear();
	animFrames.pushBack(SpriteFrame::create(resourcePath + "gif/goalkeeper/7/goalkeeper_7_1-5.png", textureRect));
	animFrames.pushBack(SpriteFrame::create(resourcePath + "gif/goalkeeper/7/goalkeeper_7_2.png", textureRect));
	animFrames.pushBack(SpriteFrame::create(resourcePath + "gif/goalkeeper/7/goalkeeper_7_3.png", textureRect));
	animFrames.pushBack(SpriteFrame::create(resourcePath + "gif/goalkeeper/7/goalkeeper_7_4.png", textureRect));
	animFrames.pushBack(SpriteFrame::create(resourcePath + "gif/goalkeeper/7/goalkeeper_7_1-5.png", textureRect));
	Animation* animation = Animation::createWithSpriteFrames(animFrames, 0.1f);
	animation->setLoops(-1);

	_currentActionAnimation = _spriteBirdGoalkeeper->runAction(Animate::create(animation));
}

void WordKicker_NodeBirdGoalkeeper::setAnimation_feather()
{
	Rect textureRect = _spriteFeather->getTextureRect();
	Vector<SpriteFrame*> animFrames;

	animFrames.clear();
	animFrames.pushBack(SpriteFrame::create(resourcePath + "gif/goalkeeper/feather/goalkeeper_feather_1.png", textureRect));
	animFrames.pushBack(SpriteFrame::create(resourcePath + "gif/goalkeeper/feather/goalkeeper_feather_2.png", textureRect));
	animFrames.pushBack(SpriteFrame::create(resourcePath + "gif/goalkeeper/feather/goalkeeper_feather_3.png", textureRect));
	Animation* animation = Animation::createWithSpriteFrames(animFrames, 0.1f);
	animation->setLoops(1);

	_spriteFeather->setOpacity(255);
	_spriteFeather->runAction(
		Sequence::create(
			Animate::create(animation),
			FadeTo::create(0,0),
			nullptr
		)
	);
}

void WordKicker_NodeBirdGoalkeeper::setAnimation_debug_next()
{
	if (_curAnimation == 5)
	{
		setAnimation_6_speak();
	}
	else if (_curAnimation == 6)
	{
		setAnimation_7_1_goal();
	}
	else if (_curAnimation == 71)
	{
		setAnimation_7_2_goal();
	}
	else
	{
		setAnimation_5_idle();
	}
}

void WordKicker_NodeBirdGoalkeeper::setEyesTarget(Vec2 vTargetWorldPos)
{
	Vec2 vLocalPos = _spriteEyes->getParent()->convertToNodeSpace(vTargetWorldPos);
	//vLocalPos.y -= 200;
	vLocalPos.normalize();
	
	_spriteEyes->setPosition(vLocalPos.x * 15 + _spriteBirdGoalkeeper->getContentSize().width / 2, vLocalPos .y * 15);
}

void WordKicker_NodeBirdGoalkeeper::setEyesTarget_onlyX(Vec2 vTargetWorldPos)
{
	Vec2 vLocalPos = _spriteEyes->getParent()->convertToNodeSpace(vTargetWorldPos);
	//vLocalPos.y -= 200;
	vLocalPos.normalize();

	_spriteEyes->setPosition(vLocalPos.x * 15 + _spriteBirdGoalkeeper->getContentSize().width / 2, 0);
}

void WordKicker_NodeBirdGoalkeeper::doJump()
{
	_spriteBirdGoalkeeper->runAction(
		JumpBy::create(0.3f, Vec2::ZERO, 50.0f, 1)
	);
}

void WordKicker_NodeBirdGoalkeeper::doFly()
{
	_spriteBirdGoalkeeper->setGlobalZOrder(3);

	_spriteBirdGoalkeeper->runAction(
		JumpBy::create(2.0f, Vec2(2560, 0), 1200, 1)
	);
	_spriteBirdGoalkeeper->runAction(
		RotateBy::create(2.0f, 1440.0f)
	);
	_spriteShadow->runAction(
		MoveBy::create(2.0f, Vec2(2560, 0))
	);
}

END_NS_WORDKICKER;