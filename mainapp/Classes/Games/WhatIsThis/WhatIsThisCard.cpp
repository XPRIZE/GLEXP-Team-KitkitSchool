/////////////////////////////////////////////////////////////////////////////////////////
/****************************************************************************************
 Created        :    2018.02.13
 
 Author        :    Mikey.p
 
 Purpose        :    WhatIsThis card
 
 ****************************************************************************************/
/////////////////////////////////////////////////////////////////////////////////////////

#include "WhatIsThisCard.h"

#include "ui/CocosGUI.h"
#include "Managers/GameSoundManager.h"
#include "WhatIsThisScene.h"
#include "Utils/TodoUtil.h"

#define FLIP_TIME 0.35f
#define FLIP_MOVE 300.f
#define WAIT_TIME 2.f
#define THROW_TIME 0.35f
#define CONTENTS_SIZE_MODIFIER_X 0.85f
#define CONTENTS_SIZE_MODIFIER_Y 0.85f

/////////////////////////////////////////////////////////////////////////////////////////
bool WhatIsThisCard::init() {
	if (!Node::init())
	{
		return false;
	}

	return true;
}

bool STTopCard::init()
{
	if (!getChildByName("ImageNode"))
	{
		auto imageNode = Node::create();
		addChild(imageNode, 0, "ImageNode");
		std::string bgImageFilename, maskFilename, bgWrongFilename;

		bgImageFilename = "WhatIsThis/Images/showandtel_image_card-large_for-image.png";
		bgWrongFilename = "WhatIsThis/Images/showandtel_image_card-large_for-image_wrong.png";
		maskFilename = "WhatIsThis/Images/showandtel_image_card-large_for-image-mask.png";

		if (auto sprite = Sprite::create(bgImageFilename))
		{
			imageNode->addChild(sprite, 0, "BG");
		}

		if (auto sprite = Sprite::create(bgWrongFilename))
		{
			sprite->setVisible(false);
			imageNode->addChild(sprite, 0, "BGWrong");
		}

		if (auto sprite = Sprite::create(maskFilename))
		{
			auto mask = ClippingNode::create(sprite);
			mask->setAlphaThreshold(0.5f);
			imageNode->addChild(mask, 0, "mask");
		}
	}

	if (!getChildByName("textNode"))
	{
		auto textNode = Node::create();
		addChild(textNode, 0, "textNode");
		std::string bgTextFilename, bgTextWrongFileName;

		bgTextFilename = "WhatIsThis/Images/showandtel_image_card-question-long_for-text.png";
		bgTextWrongFileName = "WhatIsThis/Images/showandtel_image_card-question-long_for-text_wrong.png";

		if (auto sprite = Sprite::create(bgTextFilename))
		{
			textNode->addChild(sprite, 0, "BG");
		}

		if (auto sprite = Sprite::create(bgTextWrongFileName))
		{
			sprite->setVisible(false);
			textNode->addChild(sprite, 0, "BGWrong");
		}

		if (auto label = Label::createWithTTF("", "fonts/mukta-bold.ttf", 130))
		{
			label->setAlignment(TextHAlignment::CENTER, TextVAlignment::CENTER);
			label->setTextColor(Color4B(81, 53, 24, 230));
			label->setContentSize(Size(2038, 0));
			label->setDimensions(2038, 0);
			label->setOverflow(Label::Overflow::NONE);
			textNode->addChild(label, 1, "labelBig");
		}

		if (auto label = Label::createWithTTF("", "fonts/mukta-bold.ttf", 100))
		{
			label->setAlignment(TextHAlignment::LEFT, TextVAlignment::CENTER);
			label->setTextColor(Color4B(81, 53, 24, 230));
			label->setContentSize(Size(2038, 0));
			label->setDimensions(2038, 0);
			label->setOverflow(Label::Overflow::RESIZE_HEIGHT);
			textNode->addChild(label, 1, "labelSmall");
		}
	}

	std::function<void(Node*)> SetCascade = [&SetCascade](Node* node)
	{
		node->setCascadeColorEnabled(true);
		node->setCascadeOpacityEnabled(true);

		for (auto& child : node->getChildren())
			SetCascade(child);
	};

	SetCascade(this);

	return true;
}

void STTopCard::ThrowIn()
{
	auto visibleSize = Director::getInstance()->getVisibleSize();
	setPosition(_basePos + Vec2(-visibleSize.width, 0.f));
	setScale(1.f);
	setRotation3D(Vec3::ZERO);
	runAction(EaseBackOut::create(MoveTo::create(THROW_TIME, _basePos)));
}

void STTopCard::ThrowOut()
{
	auto visibleSize = Director::getInstance()->getVisibleSize();
	runAction(EaseBackIn::create(MoveTo::create(THROW_TIME, _basePos + Vec2(visibleSize.width, 0.f))));
}

void STTopCard::SetCard(const WhatIsThisCardData & data, const CardType & cardType)
{
	_cardType = cardType;

	if (auto imageNode = getChildByName("ImageNode"))
	{
		if (_cardType == CardType::Image)
		{
			imageNode->setVisible(true);
			if (auto mask = dynamic_cast<ClippingNode*>(imageNode->getChildByName("mask")))
			{
				mask->removeAllChildren();
				if (auto sprite = Sprite::create("WhatIsThis/Images/" + data.pic))
				{
					Size size = mask->getStencil()->getContentSize();
					size.width *= CONTENTS_SIZE_MODIFIER_X;
					size.height *= CONTENTS_SIZE_MODIFIER_Y;
					sprite->setContentSize(size);
					mask->addChild(sprite, 0, "image");
				}
			}
		}
		else
			imageNode->setVisible(false);
	}

	if (auto textNode = getChildByName("textNode"))
	{
		if (_cardType == CardType::Text)
		{
			textNode->setVisible(true);
            textNode->removeChildByName("label1");

            auto label1 = TodoUtil::createLabel(data.answer, 120, Size(2038,0), "fonts/mukta-bold.ttf", Color4B(81, 53, 24, 230), TextHAlignment::CENTER);
            label1->setName("label1");
            textNode->addChild(label1);
            
            if (label1->getStringNumLines() > 1) {
                label1->setAlignment(cocos2d::TextHAlignment::LEFT);
                label1->setScale(100.f/120.f);
            }

            /*
			if (auto label = dynamic_cast<Label*>(textNode->getChildByName("labelBig")))
			{
				label->setString(data.answer);
                label->setContentSize(Size(2038, 0));
                label->setDimensions(2038, 0);


				bool oneLine = label->getStringNumLines() == 1;
                
                CCLOG("lines:%d", label->getStringNumLines());
				if (oneLine)
				{
					label->setVisible(true);
					if (auto smallLabel = dynamic_cast<Label*>(textNode->getChildByName("labelSmall")))
						smallLabel->setVisible(false);
				}
				else
				{
					label->setVisible(false);
					if (auto smallLabel = dynamic_cast<Label*>(textNode->getChildByName("labelSmall")))
					{

						smallLabel->setVisible(true);
						smallLabel->setString(data.answer);
                        smallLabel->setContentSize(Size(2038, 0));
                        smallLabel->setDimensions(2038, 0);

					}
				}
			}
             */
		}
		else
			textNode->setVisible(false);
	}
}

void STTopCard::SetFail(const bool & fail)
{
	if (auto imageNode = getChildByName("ImageNode"))
	{
		if (auto pNode = imageNode->getChildByName("BG"))
			pNode->setVisible(!fail);
		if (auto pNode = imageNode->getChildByName("BGWrong"))
			pNode->setVisible(fail);
	}

	if (auto textNode = getChildByName("textNode"))
	{
		if (auto pNode = textNode->getChildByName("BG"))
			pNode->setVisible(!fail);
		if (auto pNode = textNode->getChildByName("BGWrong"))
			pNode->setVisible(fail);
	}

}

void STDownCard::loadDurationsheet() {
    
    if (_duration.size() != 0) return;
    std::string rawString = cocos2d::FileUtils::getInstance()->getStringFromFile("WhatIsThis/Sounds/Durations.tsv");
    auto data = TodoUtil::readTSV(rawString);

    for (auto row : data) {
        if (TodoUtil::trim(row[0]).size() <= 0) continue;
        if (row.size()==1) continue;
        auto row1Vec = TodoUtil::split(row[1], ':');
        TodoUtil::replaceAll(row1Vec[2], ".", "");
        auto rowDuration = (float)TodoUtil::stoi(row1Vec[2])/100;
        _duration[row[0]] = rowDuration;
    }
}

bool STDownCard::init()
{
	if (!getChildByName("ImageNode"))
	{
		auto imageNode = Node::create();
		addChild(imageNode, 0, "ImageNode");
		std::string bgImageFilename, bgWrongFilename, maskFilename;

		bgImageFilename = "WhatIsThis/Images/showandtel_image_card-answer_for-image_front.png";
		bgWrongFilename = "WhatIsThis/Images/showandtel_image_card-answer_for-image_front_wrong.png";
		maskFilename = "WhatIsThis/Images/showandtel_image_card-answer_for-image-mask.png";

		if (auto sprite = Sprite::create(bgImageFilename))
		{
			imageNode->addChild(sprite, 0, "BG");
		}

		if (auto sprite = Sprite::create(bgWrongFilename))
		{
			sprite->setOpacity(0);
			imageNode->addChild(sprite, 0, "BGWrong");
		}

		if (auto sprite = Sprite::create(maskFilename))
		{
			auto mask = ClippingNode::create(sprite);
			mask->setAlphaThreshold(0.5f);
			imageNode->addChild(mask, 0, "mask");
		}
	}

	if (!getChildByName("textNode"))
	{
		auto textNode = Node::create();
		addChild(textNode, 0, "textNode");
		std::string bgTextFilename, bgTextWrongFilename;

		bgTextFilename = "WhatIsThis/Images/showandtel_image_card-answer-long_for-text_front.png";
		bgTextWrongFilename = "WhatIsThis/Images/showandtel_image_card-answer-long_for-text_front_wrong.png";

		if (auto sprite = Sprite::create(bgTextFilename))
		{
			textNode->addChild(sprite, 0, "BG");
		}

		if (auto sprite = Sprite::create(bgTextWrongFilename))
		{
			sprite->setOpacity(0);
			textNode->addChild(sprite, 0, "BGWrong");
		}

		if (auto label = Label::createWithTTF("Hello World", "fonts/mukta-bold.ttf", 82))
		{
			label->setAlignment(TextHAlignment::LEFT, TextVAlignment::CENTER);
			label->setTextColor(Color4B(81, 53, 24, 230));
			label->setContentSize(Size(2208, 200));
			label->setDimensions(0, 0);
			label->setOverflow(Label::Overflow::SHRINK);
			textNode->addChild(label, 1, "label");
		}
	}

	if (!getChildByName("ImageBackNode"))
	{
		if (auto sprite = Sprite::create("WhatIsThis/Images/showandtel_image_card-answer_for-image_back.png"))
		{
			sprite->setRotation3D({ 0, 180, 0 });
			addChild(sprite, 0, "ImageBackNode");
		}
	}

	if (!getChildByName("textBackNode"))
	{
		if (auto sprite = Sprite::create("WhatIsThis/Images/showandtel_image_card-answer-long_for-text_back.png"))
		{
			sprite->setRotation3D({ 180, 0, 0 });
			addChild(sprite, 0, "textBackNode");
		}
	}

	// 터치 영역 최초 생성
	if (!getChildByName("touchNode"))
	{
		auto layout = ui::Layout::create();
		layout->setTouchEnabled(true);
		layout->addClickEventListener([this](Ref*)
		{
			if (_clickEvent)
				_clickEvent();
		});

		addChild(layout, 0, "touchNode");
	}

	std::function<void(Node*)> SetCascade = [&SetCascade](Node* node)
	{
		node->setCascadeColorEnabled(true);
		node->setCascadeOpacityEnabled(true);

		for (auto& child : node->getChildren())
			SetCascade(child);
	};
	
	SetCascade(this);
    
    loadDurationsheet();

	return true;
}

void STDownCard::SetCard(const WhatIsThisCardData & data, const CardType & cardType)
{
	_cardType = cardType;
	_soundFile = data.sound;
	_soundDuration = data.soundDuration;
    
    if (_duration.count(_soundFile)) _soundDuration = _duration[_soundFile];

	if (auto imageNode = getChildByName("ImageNode"))
	{
		auto imageBackNode = getChildByName("ImageBackNode");

		if (_cardType == CardType::Image)
		{
			imageNode->setVisible(true);
			imageBackNode->setVisible(false);

			_frontNode = imageNode;
			_backNode = imageBackNode;

			if (auto mask = dynamic_cast<ClippingNode*>(imageNode->getChildByName("mask")))
			{
				mask->removeAllChildren();
				if (auto sprite = Sprite::create("WhatIsThis/Images/" + data.pic))
				{
					Size size = mask->getStencil()->getContentSize();
					size.width *= CONTENTS_SIZE_MODIFIER_X;
					size.height *= CONTENTS_SIZE_MODIFIER_Y;
					sprite->setContentSize(size);
					mask->addChild(sprite, 0, "image");
				}
			}
		}
		else
		{
			imageNode->setVisible(false);
			imageBackNode->setVisible(false);
		}
	}

	if (auto textNode = getChildByName("textNode"))
	{
		auto textBackNode = getChildByName("textBackNode");

		if (_cardType == CardType::Text)
		{
			textNode->setVisible(true);
			textBackNode->setVisible(false);

			_frontNode = textNode;
			_backNode = textBackNode;

			if (auto label = dynamic_cast<Label*>(textNode->getChildByName("label")))
			{
				label->setString(data.answer);
			}
		}
		else
		{
			textNode->setVisible(false);
			textBackNode->setVisible(false);
		}
	}

	if (auto touchLayout = dynamic_cast<ui::Layout*>(getChildByName("touchNode")))
	{
		auto touchSize = _frontNode->getChildByName("BG")->getContentSize();

		touchLayout->setContentSize(touchSize);
		touchLayout->setTouchEnabled(true);
		touchLayout->setPosition(touchSize / -2.f);
	}
}

void STDownCard::Flip(const bool& success, const std::function<void()>& cbf)
{
	GameSoundManager::getInstance()->playEffectSound("WhatIsThis/Sounds/BlockSlotIn.m4a");

	Vector<FiniteTimeAction*> actions;

	if (success)
	{
		actions.pushBack(CallFunc::create([] { GameSoundManager::getInstance()->playEffectSound("WhatIsThis/Sounds/showandtell_sfx_answer_right.m4a"); }));
		float scale = _cardType == CardType::Image ? 1.1f : 1.05f;
		actions.pushBack(Spawn::create(EaseOut::create(ScaleTo::create(FLIP_TIME, scale, scale), 1.5), EaseOut::create(MoveBy::create(FLIP_TIME / 2.f, Vec2(0, 0.f)), 2.f), nullptr));
		if (!_soundFile.empty())
			actions.pushBack(CallFunc::create([this] { GameSoundManager::getInstance()->playEffectSound("WhatIsThis/Sounds/" + _soundFile); }));
		actions.pushBack(DelayTime::create(MAX(WAIT_TIME, _soundDuration)));
	}
	else
	{
		actions.pushBack(EaseInOut::create(ActionFloat::create(FLIP_TIME, 0.f, 180.f, [this](float value)
		{
			if (_cardType == CardType::Image)
				setRotation3D(Vec3(0.f, value, 0.f));
			else
				setRotation3D(Vec3(value, 0.f, 0.f));

			if (value > 90.f)
			{
				_frontNode->setVisible(false);
				_backNode->setVisible(true);
			}

		}), 2.f));
	}

	if (cbf)
		actions.pushBack(CallFunc::create(cbf));
	runAction(Sequence::create(actions));
	return;
}

void STDownCard::Fail(const bool & hit, const std::function<void()>& cbf)
{
	GameSoundManager::getInstance()->playEffectSound("WhatIsThis/Sounds/BlockSlotIn.m4a");

	Vector<FiniteTimeAction*> actions;

	if (hit)
	{
		auto basePosition = getPosition();
        
		runAction(ActionFloat::create(FLIP_TIME, 0.f, 1.f, [this, basePosition](float rate)
		{
			auto value = sinf(M_PI * 4.f * rate) * sinf(M_PI * rate) * 25.f;
			setPosition(basePosition + Vec2(value, 0));
		}));

		actions.pushBack(CallFunc::create([] { GameSoundManager::getInstance()->playEffectSound("WhatIsThis/Sounds/SFX_Wood_Incorrect.m4a"); }));

		if (!_soundFile.empty())
			actions.pushBack(CallFunc::create([this] { GameSoundManager::getInstance()->playEffectSoundVoiceOnly("WhatIsThis/Sounds/" + _soundFile); }));
		actions.pushBack(DelayTime::create(MAX(WAIT_TIME, _soundDuration)));

		Node* targetNode = nullptr;

		if (_cardType == CardType::Image)
			targetNode = getChildByName("ImageNode");
		else
			targetNode = getChildByName("textNode");

		if (targetNode)
		{
			if (auto pNode = targetNode->getChildByName("BG"))
				pNode->runAction(Sequence::create(
					FadeOut::create(0.3f),
					DelayTime::create(MAX(WAIT_TIME-0.6, _soundDuration-0.6)),
					FadeIn::create(0.3f),
					nullptr
				));
			if (auto pNode = targetNode->getChildByName("BGWrong"))
				pNode->runAction(Sequence::create(
					FadeIn::create(0.3f),
                    DelayTime::create(MAX(WAIT_TIME-0.6, _soundDuration-0.6)),
					FadeOut::create(0.3f),
					nullptr
				));

		}

		if (cbf)
			actions.pushBack(CallFunc::create(cbf));
		runAction(Sequence::create(actions));
	}
	/*
	else
	{
		actions.pushBack(TintTo::create(FLIP_TIME, 150, 150, 150));
		actions.pushBack(DelayTime::create(WAIT_TIME - FLIP_TIME * 2.f));
		actions.pushBack(TintTo::create(FLIP_TIME, 255, 255, 255));
	}
	*/


	return;
}


void STDownCard::ThrowIn()
{
	setPosition(_basePos);
	setScale(1.f);
	setRotation3D(Vec3::ZERO);
	setOpacity(0.f);
	setVisible(true);
	runAction(FadeIn::create(0.1f));
}

void STDownCard::ThrowOut()
{
	runAction(Sequence::create(FadeOut::create(0.1f), Hide::create(), nullptr));
}
