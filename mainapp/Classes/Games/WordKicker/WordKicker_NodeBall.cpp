//
//  WordKicker_NodeBall.cpp
//  KitkitSchool
//
//  Created by MilliSoftGames on 8/25/18.
//


#include "WordKicker_NodeBall.hpp"

#include "Utils/TodoUtil.h"


BEGIN_NS_WORDKICKER;

bool WordKicker_NodeBall::init()
{
	if (!Node::init()) return false;
	return true;
}

void WordKicker_NodeBall::removeObjects()
{
	_spriteEffectList.clear();
	_spriteShadow = nullptr;
	_spriteBall = nullptr;

	_nodeBall->removeAllChildren();
	_nodeBall = nullptr;

	_node->removeAllChildren();
	_node = nullptr;

	removeAllChildren();
}

void WordKicker_NodeBall::createSprites()
{
	_node = Node::create();
	_node->setPosition(ball_x, ball_y);
	addChild(_node);

	_spriteShadow = Sprite::create(resourcePath + "shadow.png");
	_spriteShadow->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	_spriteShadow->setPosition(0, 0);
	_spriteShadow->setScaleX(0.75f);
	_node->addChild(_spriteShadow);

	_nodeBall = Node::create();
	_nodeBall->setPosition(0, 0);
	_node->addChild(_nodeBall);

	_spriteBall = Sprite::create(resourcePath + "ball.png");
	_spriteBall->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	_ballBaseY = _spriteBall->getContentSize().height / 2;
	_spriteBall->setPosition(0, _ballBaseY);
	_nodeBall->addChild(_spriteBall);

	
	Sprite* spriteEffect;
	
	spriteEffect = Sprite::create(resourcePath + "gif/ball/a/ball_a_1.png");
	_nodeBall->addChild(spriteEffect);
	spriteEffect->setAnchorPoint(Vec2::ANCHOR_MIDDLE_RIGHT);
	spriteEffect->setPosition(_spriteBall->getContentSize().width / 2, _ballBaseY);
	spriteEffect->setVisible(false);
	spriteEffect->setLocalZOrder(0);
	_spriteEffectList.push_back(spriteEffect);


	spriteEffect = Sprite::create(resourcePath + "gif/ball/b/ball_b_1.png");
	_nodeBall->addChild(spriteEffect);
	spriteEffect->setAnchorPoint(Vec2::ANCHOR_MIDDLE_RIGHT);
	spriteEffect->setPosition(_spriteBall->getContentSize().width * 0.2f, _ballBaseY);
	spriteEffect->setVisible(false);
	spriteEffect->setLocalZOrder(-1);
	_spriteEffectList.push_back(spriteEffect);

	spriteEffect = Sprite::create(resourcePath + "gif/ball/c/ball_c_1.png");
	_nodeBall->addChild(spriteEffect);
	spriteEffect->setAnchorPoint(Vec2::ANCHOR_MIDDLE_RIGHT);
	spriteEffect->setPosition(_spriteBall->getContentSize().width * 0.2f, _ballBaseY);
	spriteEffect->setVisible(false);
	spriteEffect->setLocalZOrder(-1);
	_spriteEffectList.push_back(spriteEffect);
}

void WordKicker_NodeBall::startNewProblem()
{
	_node->setPosition(ball_x, ball_y);
	_spriteShadow->setPosition(0, 0);
	_spriteBall->setRotation(0);

	_spriteShadow->runAction(FadeIn::create(0.1f));
	_spriteBall->runAction(FadeIn::create(0.1f));

	_nodeBall->setPosition(0, 50);
	_nodeBall->runAction(MoveTo::create(0.1f, Vec2::ZERO));
}

void WordKicker_NodeBall::startNewProblem_notmove()
{
	_node->setPosition(ball_x, ball_y);
	_spriteShadow->setPosition(0, 0);
	_spriteBall->setRotation(0);

	_spriteShadow->runAction(FadeIn::create(0.1f));
	_spriteBall->runAction(FadeIn::create(0.1f));

	_nodeBall->setPosition(0, 0);
}

Vec2 WordKicker_NodeBall::GetBallPosition_world()
{
	Vec2 vLocal = _nodeBall->getPosition();
	Vec2 vWorld = _nodeBall->getParent()->convertToWorldSpace(vLocal);
	return vWorld;
}

Vec2 WordKicker_NodeBall::GetBallPosition_local()
{
	Vec2 vLocal = _nodeBall->getPosition();
	return vLocal;
}

void WordKicker_NodeBall::showEffect(int nEffect)
{
	_spriteEffectList[nEffect]->setVisible(true);

	_spriteEffectList[nEffect]->stopAllActions();
	Rect textureRect = _spriteEffectList[nEffect]->getTextureRect();
	Vector<SpriteFrame*> animFrames;
	animFrames.clear();
	if (nEffect == 0)
	{
		animFrames.pushBack(SpriteFrame::create(resourcePath + "gif/ball/a/ball_a_1.png", textureRect));
		animFrames.pushBack(SpriteFrame::create(resourcePath + "gif/ball/a/ball_a_2.png", textureRect));
		animFrames.pushBack(SpriteFrame::create(resourcePath + "gif/ball/a/ball_a_3.png", textureRect));
	}
	else if (nEffect == 1)
	{
		animFrames.pushBack(SpriteFrame::create(resourcePath + "gif/ball/b/ball_b_1.png", textureRect));
		animFrames.pushBack(SpriteFrame::create(resourcePath + "gif/ball/b/ball_b_2.png", textureRect));
		animFrames.pushBack(SpriteFrame::create(resourcePath + "gif/ball/b/ball_b_3.png", textureRect));
	}
	else
	{
		animFrames.pushBack(SpriteFrame::create(resourcePath + "gif/ball/c/ball_c_1.png", textureRect));
		animFrames.pushBack(SpriteFrame::create(resourcePath + "gif/ball/c/ball_c_2.png", textureRect));
		animFrames.pushBack(SpriteFrame::create(resourcePath + "gif/ball/c/ball_c_3.png", textureRect));
	}
	Animation* animation = Animation::createWithSpriteFrames(animFrames, 0.1f);
	animation->setLoops(-1);
	_spriteEffectList[nEffect]->runAction(Animate::create(animation));
}

void WordKicker_NodeBall::stopEffect()
{
	for (auto sprite : _spriteEffectList)
	{
		sprite->stopAllActions();
		sprite->setVisible(false);
		sprite->setOpacity(255);
	}
}

float WordKicker_NodeBall::getRatio(float fTime, float fFrom, float fTo)
{
	return (fTime - fFrom) / (fTo - fFrom);
}

bool WordKicker_NodeBall::processBall_wronganswer(float fTime, float fDeltaTime)
{
	if (fTime >= 3.0f)
	{
		fTime = 3.0f;
	}

	if (fTime < 0.3f)
	{
		_nodeBall->setPosition(0, 0);
		_spriteShadow->setPosition(0, 0);
	}
	else if (fTime < 1.9f)
	{
		float fRatio = getRatio(fTime, 0.3f, 1.9f);

		float fX = 0  + 400.0f * sinf(fRatio * 0.5f * M_PI);
		float fY = 0 + 100.0f * cosf(fRatio * 0.5f * M_PI) * fabsf(sinf(fRatio * 4.0f * M_PI));

		_nodeBall->setPosition(fX, fY);
		_spriteShadow->setPosition(fX, 0);

		_spriteBall->setRotation(360.0f * sinf(fRatio * 0.5f * M_PI));
	}
	else if (fTime < 2.9f)
	{
		_nodeBall->setPosition(400, 0);
		_spriteShadow->setPosition(400, 0);
	}
	else
	{
		float fRatio = 1.0f - getRatio(fTime, 2.9f, 3.0f);

		_nodeBall->setPosition(400, 0);
		_spriteShadow->setPosition(400, 0);

		_spriteShadow->setOpacity(255 * fRatio);
		_spriteBall->setOpacity(255 * fRatio);
	}


	return fTime >= 3.0f;
}

bool WordKicker_NodeBall::processBall_timeover(float fTime, float fDeltaTime)
{
	if (fTime >= 3.0f)
	{
		fTime = 3.0f;
	}

	if (fTime < 0.3f)
	{
		_nodeBall->setPosition(0, 0);
		_spriteShadow->setPosition(0, 0);
	}
	else if (fTime < 2.9f)
	{
		float fRatio = getRatio(fTime, 0.3f, 2.9f);

		float fX = 0 + 300.0f * sinf(fRatio * 0.5f * M_PI);
		float fY = 0;// +100.0f * cosf(fRatio * 0.5f * M_PI) * fabsf(sinf(fRatio * 4.0f * M_PI));

		_nodeBall->setPosition(fX, fY);
		_spriteShadow->setPosition(fX, 0);

		_spriteBall->setRotation(360.0f * sinf(fRatio * 0.5f * M_PI));
	}
	else
	{
		float fRatio = 1.0f - getRatio(fTime, 2.9f, 3.0f);

		_nodeBall->setPosition(300.0f, 0);
		_spriteShadow->setPosition(300.0f, 0);

		_spriteShadow->setOpacity(255 * fRatio);
		_spriteBall->setOpacity(255 * fRatio);
	}


	return fTime >= 3.0f;
}

bool WordKicker_NodeBall::processBall_correctanswer_1(float fTime, float fDeltaTime)
{
	int nEffect = 0;
	float fBallMaxX = 1270.0f;
	float fBallMaxY = 350.0f;
	float fBallRotateSpeed_1 = 720.0f;
	float fBallRotateSpeed_2 = 180.0f;

	if (fTime >= 3.0f)
	{
		fTime = 3.0f;
	}

	if (fTime < 0.9f)
	{
		_nodeBall->setPosition(0, 0);
		_spriteShadow->setPosition(0, 0);

		_spriteEffectList[nEffect]->setVisible(false);
	}
	else if (fTime < 1.5f)
	{
		if (!_spriteEffectList[nEffect]->isVisible())
		{
			showEffect(nEffect);
		}

		float fRatio = getRatio(fTime, 0.9f, 1.5f);

		//float fX = 0 + fBallMaxX * fRatio;// sinf(fRatio * 0.5f * M_PI);
		float fX = 0 + fBallMaxX * sinf(fRatio * 0.5f * M_PI);
		float fY = 0 + fBallMaxY * fabsf(sinf(fRatio * M_PI));

		Vec2 vOldPos = _nodeBall->getPosition();
		
		_nodeBall->setPosition(fX, fY);
		_spriteShadow->setPosition(fX, 0);

		Vec2 vNewPos = _nodeBall->getPosition();
		float fDegree = -CC_RADIANS_TO_DEGREES((vNewPos - vOldPos).getAngle());
		_nodeBall->setRotation(fDegree);

		_spriteBall->setRotation(_spriteBall->getRotation() + fBallRotateSpeed_1 * fDeltaTime);

		if (fRatio >= 0.65f)
		{
			if (_spriteEffectList[nEffect]->isVisible())
			{
				_nodeBall->setRotation(0);
				stopEffect();
			}
		}
	}
	else if (fTime < 2.9f)
	{
		if (_spriteEffectList[nEffect]->isVisible())
		{
			stopEffect();
		}

		float fRatio = getRatio(fTime, 1.5f, 2.9f);
		float fY = 0 + 200.0f * cosf(fRatio * 0.5f * M_PI) * fabsf(sinf(fRatio * 5.0f * M_PI));

		_nodeBall->setPosition(fBallMaxX, fY);
		_nodeBall->setRotation(0);
		_spriteBall->setRotation(_spriteBall->getRotation() + fBallRotateSpeed_2 * fDeltaTime);
		_spriteShadow->setPosition(fBallMaxX, 0);
	}
	else
	{
		float fRatio = 1.0f - getRatio(fTime, 2.9f, 3.0f);

		_nodeBall->setPosition(fBallMaxX, 0);
		_nodeBall->setRotation(0);
		_spriteShadow->setPosition(fBallMaxX, 0);


		_spriteShadow->setOpacity(255 * fRatio);
		_spriteBall->setOpacity(255 * fRatio);
	}


	return fTime >= 3.0f;
}

bool WordKicker_NodeBall::processBall_correctanswer_2(float fTime, float fDeltaTime)
{
	int nEffect = 1;
	float fBallMaxX = 1520.0f;
	float fBallMaxY = 180.0f;
	float fBallRotateSpeed = 2000.0f;

	if (fTime >= 3.0f)
	{
		fTime = 3.0f;
	}

	if (fTime < 0.9f)
	{
		_nodeBall->setPosition(0, 0);
		_spriteShadow->setPosition(0, 0);

		_spriteEffectList[nEffect]->setVisible(false);
	}
	else if (fTime < 1.5f)
	{
		if (!_spriteEffectList[nEffect]->isVisible())
		{
			showEffect(nEffect);
		}

		float fRatio = getRatio(fTime, 0.9f, 1.5f);

		float fX = 0 + fBallMaxX * fRatio;// sinf(fRatio * 0.5f * M_PI);
		//float fX = 0 + fBallMaxX * sinf(fRatio * 0.5f * M_PI);
		float fY = 0 + fBallMaxY * (1.0f - pow(1.0f-fRatio, 5.0f));

		Vec2 vOldPos = _spriteBall->getPosition();

		_nodeBall->setPosition(fX, fY);
		_spriteShadow->setPosition(fX, 0);

		_spriteBall->setRotation(_spriteBall->getRotation() + fBallRotateSpeed * fDeltaTime);
	}
	else if (fTime < 2.9f)
	{
		_nodeBall->setPosition(fBallMaxX, fBallMaxY);
		_spriteShadow->setPosition(fBallMaxX, 0);
		_spriteBall->setRotation(_spriteBall->getRotation() + fBallRotateSpeed * fDeltaTime);
	}
	else
	{
		if (fTime >= 3.0f)
		{
			stopEffect();
		}

		_nodeBall->setPosition(fBallMaxX, fBallMaxY);
		_spriteShadow->setPosition(fBallMaxX, 0);
		_spriteBall->setRotation(_spriteBall->getRotation() + fBallRotateSpeed * fDeltaTime);


		float fRatio = 1.0f - getRatio(fTime, 2.9f, 3.0f);

		_spriteShadow->setOpacity(255 * fRatio);
		_spriteBall->setOpacity(255 * fRatio);
		_spriteEffectList[nEffect]->setOpacity(255 * fRatio);
	}


	return fTime >= 3.0f;
}

bool WordKicker_NodeBall::processBall_correctanswer_final(float fTime, float fDeltaTime)
{
	int nEffect = 2;
	float fBallMaxX = 1520.0f;
	float fBallMaxY = 180.0f;
	float fBallRotateSpeed = 2000.0f;

	if (fTime >= 3.0f)
	{
		fTime = 3.0f;
	}

	if (fTime < 0.9f)
	{
		_nodeBall->setPosition(0, 0);
		_spriteShadow->setPosition(0, 0);

		_spriteEffectList[nEffect]->setVisible(false);
	}
	else if (fTime < 1.5f)
	{
		if (!_spriteEffectList[nEffect]->isVisible())
		{
			showEffect(nEffect);
		}

		float fRatio = getRatio(fTime, 0.9f, 1.5f);

		float fX = 0 + fBallMaxX * fRatio;// sinf(fRatio * 0.5f * M_PI);
										  //float fX = 0 + fBallMaxX * sinf(fRatio * 0.5f * M_PI);
		float fY = 0 + fBallMaxY * (1.0f - pow(1.0f - fRatio, 5.0f));

		Vec2 vOldPos = _spriteBall->getPosition();

		_nodeBall->setPosition(fX, fY);
		_spriteShadow->setPosition(fX, 0);

		_spriteBall->setRotation(_spriteBall->getRotation() + fBallRotateSpeed * fDeltaTime);
	}
	else if (fTime < 2.9f)
	{
		_nodeBall->setPosition(fBallMaxX, fBallMaxY);
		_spriteShadow->setPosition(fBallMaxX, 0);
		_spriteBall->setRotation(_spriteBall->getRotation() + fBallRotateSpeed * fDeltaTime);
	}
	else
	{
		_nodeBall->setPosition(fBallMaxX, fBallMaxY);
		_spriteShadow->setPosition(fBallMaxX, 0);
		_spriteBall->setRotation(_spriteBall->getRotation() + fBallRotateSpeed * fDeltaTime);


		float fRatio = 1.0f - getRatio(fTime, 2.9f, 3.0f);

		_spriteShadow->setOpacity(255 * fRatio);
		_spriteBall->setOpacity(255 * fRatio);
		_spriteEffectList[nEffect]->setOpacity(255 * fRatio);

		if (fTime >= 3.0f)
		{
			stopEffect();
		}
	}


	return fTime >= 3.0f;
}
END_NS_WORDKICKER;