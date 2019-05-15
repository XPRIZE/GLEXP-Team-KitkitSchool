//
//  NodeBlockMain.cpp
//  KitkitSchool
//
//  Created by MilliSoftGames on 7/26/18.
//

#include "NodeBlockMain.hpp"
#include "NodeBlock.hpp"


BEGIN_NS_SENTENCEBRIDGE;

bool NodeBlockMain::init()
{
	if (!Node::init()) return false;
	_blockNodes.clear();
	return true;
}

void NodeBlockMain::removeObjects()
{
	onBlockTouchBegan = nullptr;
	onBlockTouchMoved = nullptr;
	onBlockTouchEnded = nullptr;
    onBlockTouchEnded_cancel = nullptr;

	_capslockButton = nullptr;
	for (auto block : _blockNodes)
	{
		block->removeObjects();
	}
	_blockNodes.clear();
	removeAllChildren();
}

void NodeBlockMain::createBlockList()
{
	onBlockTouchBegan = nullptr;
	onBlockTouchMoved = nullptr;
	onBlockTouchEnded = nullptr;
    onBlockTouchEnded_cancel = nullptr;

	makeCapslockButton();

	string strBlocks = getKeyboardLayout();
	int blockPerLine = strBlocks.size() / 2;
	int startX = - (blockPerLine * 154) / 2 + 154 / 2;
	for (int i = 0; i < strBlocks.size(); i++)
	{
		int xIdx = i % blockPerLine;
		int yIdx = 1 - (i / blockPerLine);

		NodeBlock* newBlock = NodeBlock::create();
		newBlock->createBlock(strBlocks.substr(i,1));
		newBlock->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
		newBlock->setPosition(startX + xIdx * 154, 240 / 2 + yIdx * 240);
		addChild(newBlock);
		_blockNodes.push_back(newBlock);

		processBlockTouch(newBlock);
	}
}

void NodeBlockMain::onStartStage(bool showUpperCase, bool hasAlphabet)
{
    if (!showUpperCase)
    {
        _capslockButton->setVisible(false);
    }
    else
    {
        _capslockButton->setVisible(true);
        
        turnoffCapslock();
    }
    
    for (int i = 0; i < _blockNodes.size(); i++)
    {
        _blockNodes[i]->showAlphabet(hasAlphabet);
    }
}

void NodeBlockMain::showCapslockBlinkAnimation(float fDelay)
{
    if (_capslockButton->isVisible())
    {
        /*
        _capslockButton->runAction(Sequence::create(
                                                    DelayTime::create(fDelay),
                                                    FadeOut::create(0.1),
                                                    
                                                    FadeIn::create(0.1),
                                                    DelayTime::create(0.2),
                                                    FadeOut::create(0.1),
                                                    FadeIn::create(0.1),
                                                    DelayTime::create(0.2),
                                                    FadeOut::create(0.1),
                                                    FadeIn::create(0.1),
                                                    DelayTime::create(0.2),
                                                    FadeOut::create(0.1),
                                                    
                                                    FadeIn::create(0.1),
                                                    nullptr));
         */
        _capslockButton->runAction(Sequence::create(
                                                    DelayTime::create(fDelay),
                                                    EaseInOut::create(ScaleTo::create(0.5f, 1.5f), 2),
                                                    EaseInOut::create(ScaleTo::create(0.5f, 1.0f), 2),
                                                    EaseInOut::create(ScaleTo::create(0.5f, 1.5f), 2),
                                                    EaseInOut::create(ScaleTo::create(0.5f, 1.0f), 2),
                                                    nullptr));
    }
}

void NodeBlockMain::turnoffCapslock()
{
    if (_isCapslockOn)
    {
        toggleCapslock();
    }
}

void NodeBlockMain::processBlockTouch(NodeBlock* blockNode)
{
	blockNode->onTouchBegan = [this]() {
		if (onBlockTouchBegan) {
			return onBlockTouchBegan();
		}
		return false;
	};

	blockNode->onTouchMoved = [this, blockNode](Touch* touch) {
		if (onBlockTouchMoved) {
			onBlockTouchMoved(blockNode->_strLetter, touch);
		}
	};

	blockNode->onTouchEnded = [this, blockNode](Touch* touch) {
		if (onBlockTouchEnded) {
			return onBlockTouchEnded(blockNode, touch);
		}
		return false;
	};
    
    blockNode->onTouchEnded_cancel = [this, blockNode](Touch* touch) {
        if (onBlockTouchEnded_cancel) {
            return onBlockTouchEnded_cancel(blockNode, touch);
        }
        return false;
    };
}

string NodeBlockMain::getKeyboardLayout()
{

	if (!_isCapslockOn)
	{
		if (LanguageManager::getInstance()->getCurrentLanguageTag() == "sw-TZ")
		{
			return "abcdefghijklmnoprstuvwyz '!?,.";
		}
		return "abcdefghijklmnopqrstuvwxyz '!?,.";
	}
	if (LanguageManager::getInstance()->getCurrentLanguageTag() == "sw-TZ")
	{
		return "ABCDEFGHIJKLMNOPRSTUVWYZ '!?,.";
	}
	return "ABCDEFGHIJKLMNOPQRSTUVWXYZ '!?,.";
}

void NodeBlockMain::makeCapslockButton()
{
	_isCapslockOn = false;

	_capslockButton = Sprite::create(resourcePath + "button-small.png");
	_capslockButton->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	_capslockButton->setPosition(-1020, 580);
	addChild(_capslockButton);

	auto *listener = EventListenerTouchOneByOne::create();
	listener->setSwallowTouches(true);
	listener->onTouchBegan = [this](Touch* T, Event* E) {
        if (_capslockButton->isVisible()) {
            auto pos = convertToNodeSpace(T->getLocation());
            if (_capslockButton->getBoundingBox().containsPoint(pos)) {
                if (onBlockTouchBegan) {
                    if (onBlockTouchBegan()) {
                        toggleCapslock();
                        onBlockTouchEnded_cancel(nullptr, nullptr);
                        
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
                        GameSoundManager::getInstance()->playEffectSound("Common/Sounds/Effect/SFX_Wood_SlideOut.mp3");
#else
                        GameSoundManager::getInstance()->playEffectSound("Common/Sounds/Effect/SFX_Wood_SlideOut.m4a");
#endif
                        
                        return true;
                    }
                }
                return false;
            }
            return false;
        }
	};
	_capslockButton->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);
}

void NodeBlockMain::toggleCapslock()
{
	_isCapslockOn = !_isCapslockOn;

	if (!_isCapslockOn)
	{
		_capslockButton->setTexture(resourcePath + "button-small.png");
	}
	else
	{
		_capslockButton->setTexture(resourcePath + "button-capital.png");
	}

	string strBlocks = getKeyboardLayout();
	for (int i = 0; i < _blockNodes.size(); i++)
	{
		_blockNodes[i]->changeLetter(strBlocks.substr(i, 1));
	}
}

END_NS_SENTENCEBRIDGE;
