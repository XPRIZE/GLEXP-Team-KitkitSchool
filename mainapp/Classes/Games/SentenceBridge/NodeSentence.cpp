//
//  NodeSentence.cpp
//  KitkitSchool
//
//  Created by MilliSoftGames on 7/26/18.
//


#include "NodeSentence.hpp"

#include "Utils/TodoUtil.h"


BEGIN_NS_SENTENCEBRIDGE;

bool NodeSentence::init()
{
	if (!Node::init()) return false;
	return true;
}

void NodeSentence::removeObjects()
{
	_sentencePieceLabel = nullptr;
	_sentencePieceSprite_bg = nullptr;
	_sentencePieceSprite_slot = nullptr;
	_sentencePieceSprite_slotEffect = nullptr;

	removeAllChildren();
}

void NodeSentence::createSentence(int problemSentenceIdx, string strSentence, int multilineY)
{
	/*
	if (LanguageManager::getInstance()->getCurrentLanguageTag() == "sw-TZ")
	{
		TodoUtil::replaceAll(strSentence, "#", "Ch");
		TodoUtil::replaceAll(strSentence, "$", "ch");
	}
	*/

	_problemSentenceIdx = problemSentenceIdx;
	_strSentence = strSentence;
	_isSymbolProblem = (problemSentenceIdx>=0) && !isalpha(strSentence[0]);
	_multilineY = multilineY;

	_sentencePieceLabel = nullptr;
	_sentencePieceSprite_bg = nullptr;
	_sentencePieceSprite_slot = nullptr;
	_sentencePieceSprite_slotEffect = nullptr;

	_isEffectPlaying = false;

	float multiLinePosY = 0;
	if (_multilineY == -1)
	{
		multiLinePosY = 108;
	}
	else if (_multilineY == 1)
	{
		multiLinePosY = -100;
	}

	if (_problemSentenceIdx < 0)
	{
		if (_strSentence == " ")
		{
			_strSentence = "   ";
		}
		else if (_strSentence.size() == 2)
		{
			if (_strSentence[0] == ' ')
			{
				_strSentence = "  " + _strSentence;
			}
			else if (_strSentence[1] == ' ')
			{
				_strSentence = _strSentence + "  ";
			}
		}

		_sentencePieceLabel = TodoUtil::createLabel(_strSentence, fontSize, Size::ZERO, fontName, Color4B(245, 246, 211, 255), TextHAlignment::CENTER);
		_sentencePieceLabel->setAnchorPoint(Vec2::ANCHOR_MIDDLE);

		addChild(_sentencePieceLabel);
		_sentencePieceLabel->setPosition(0, -20 + multiLinePosY);
	}
	else
	{
		if (_multilineY == -1)
		{
			_sentencePieceSprite_bg = Sprite::create(resourcePath + "slot-2-row-above-common-bg.png");
			_sentencePieceSprite_bg->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
			_sentencePieceSprite_bg->setPosition(0, 40 + 125);
		}
		else if (_multilineY == 1)
		{
			_sentencePieceSprite_bg = Sprite::create(resourcePath + "slot-2-row-bottom-common-bg.png");
			_sentencePieceSprite_bg->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
			_sentencePieceSprite_bg->setPosition(0, 40 - 188);
		}
		else
		{
			_sentencePieceSprite_bg = Sprite::create(resourcePath + "slot-1-row-general-bg.png");
			_sentencePieceSprite_bg->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
			_sentencePieceSprite_bg->setPosition(0, 40);
		}
		addChild(_sentencePieceSprite_bg);

		if (!_isSymbolProblem)
		{
            if (isupper(strSentence[0]))
            {
                Sprite* sprCapitalSlot = Sprite::create(resourcePath + "capital-mark.png");
                sprCapitalSlot->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
                addChild(sprCapitalSlot);
                sprCapitalSlot->setPosition(0, -30 + multiLinePosY);
                sprCapitalSlot->setGlobalZOrder(1);
            }
			_sentencePieceSprite_slot = Sprite::create(resourcePath + "slot-common-general-normal.png");
			_sentencePieceSprite_slotEffect = Sprite::create(resourcePath + "slot-common-general-selected-effect.png");
		}
		else

		{
			_sentencePieceSprite_slot = Sprite::create(resourcePath + "slot-etc-normal.png");
			_sentencePieceSprite_slotEffect = Sprite::create(resourcePath + "slot-common-etc-selected-effect.png");
		}
		_sentencePieceSprite_slot->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
		addChild(_sentencePieceSprite_slot);
		_sentencePieceSprite_slot->setPosition(0, -30 + multiLinePosY);
		_sentencePieceSprite_slot->setGlobalZOrder(1);

		_sentencePieceSprite_slotEffect->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
		addChild(_sentencePieceSprite_slotEffect);
		_sentencePieceSprite_slotEffect->setPosition(0, -30 + multiLinePosY);
		_sentencePieceSprite_slotEffect->setOpacity(0);

		_sentencePieceSprite_slotEffect->setGlobalZOrder(2);
	}
}

float NodeSentence::getSentenceWidth()
{
	if (_problemSentenceIdx < 0)
	{
		return _sentencePieceLabel->getContentSize().width;
	}
	return _sentencePieceSprite_bg->getContentSize().width;
}

bool NodeSentence::isTouchedProblemSentence(string letter, Touch* touch)
{
	if (_problemSentenceIdx < 0)
	{
		return false;
	}

	if (_isSymbolProblem != !isalpha(letter[0]))
	{
		return false;
	}

	auto pos = convertToNodeSpace(touch->getLocation());
	if (_sentencePieceSprite_slot->getBoundingBox().containsPoint(pos))
	{
		return true;
	}
	return false;
}

void NodeSentence::showSelectedEffect()
{
	if (_problemSentenceIdx < 0)
	{
		return;
	}
	if (_isEffectPlaying)
	{
		return;
	}

	_isEffectPlaying = true;

	_sentencePieceSprite_slotEffect->stopAllActions();
	_sentencePieceSprite_slotEffect->runAction(FadeIn::create(0.01f));
	_sentencePieceSprite_slotEffect->runAction(
		RepeatForever::create(
			Sequence::create(
				DelayTime::create(0.1f),
				FadeTo::create(0.2f, 200),
				FadeTo::create(0.2f, 255),
				nullptr
			)
		)
	);
}

void NodeSentence::hideSelectedEffect()
{
	if (_problemSentenceIdx < 0)
	{
		return;
	}
	if (!_isEffectPlaying)
	{
		return;
	}

	_isEffectPlaying = false;
	_sentencePieceSprite_slotEffect->stopAllActions();
	_sentencePieceSprite_slotEffect->runAction(FadeOut::create(0.2f));
}

Vec2 NodeSentence::getSlotWorldPos()
{
	return convertToWorldSpace(_sentencePieceSprite_slot->getPosition());
}

void NodeSentence::hideSentenceSpriteBG(float duration)
{
	_sentencePieceSprite_bg->runAction(FadeOut::create(duration));
}


END_NS_SENTENCEBRIDGE;
