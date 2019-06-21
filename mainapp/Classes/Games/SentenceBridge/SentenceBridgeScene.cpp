//
//  SentenceBridgeScene.cpp
//  KitkitSchool
//
//  Created by MilliSoftGames on 7/26/18.
//
//

#include <Managers/VoiceMoldManager.h>
#include "SentenceBridgeScene.hpp"
#include "NodeBridgeMain.hpp"
#include "NodeBlockMain.hpp"
#include "NodeBlock.hpp"

#include "Utils/TodoUtil.h"

#include "Common/Controls/TodoSchoolBackButton.hpp"
#include "Common/Controls/CompletePopup.hpp"


#include "CCAppController.hpp"


using namespace cocos2d::ui;
using namespace std;


BEGIN_NS_SENTENCEBRIDGE;


Scene* SentenceBridgeScene::createScene(string levelID)
{
    auto level = TodoUtil::stoi(levelID);
    
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = SentenceBridgeScene::create();
    layer->setLevel(TodoUtil::stoi(levelID));
    
    // add layer as a child to scene
    scene->addChild(layer);
    
    // return the scene
    return scene;
}

bool SentenceBridgeScene::init()
{
    if (!Layer::init())
    {
        return false;
    }

	std::srand((unsigned)time(nullptr));
    
	_gameNode = nullptr;
	_speakerButton = nullptr;
	_shiningParticleNode = nullptr;

	_touchEnabled = false;
	_speakerPlaying = false;
    _blockMoving = false;
    
    SoundEffect("Common/Sounds/Effect/cardrive.m4a").preload();
    SoundEffect("Common/Sounds/Effect/cardrive_gone.m4a").preload();
    
    auto winSize = getContentSize();
	auto gameScale = winSize.height / gameSize.height;
    
    auto bg = Sprite::create(resourcePath+"sky.png");
    bg->setScale(gameScale);
	bg->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
    bg->setPosition(winSize.width/2, -200);
    addChild(bg);
    
	_gameNode = Node::create();
	//_gameNode->setContentSize(gameSize);
	_gameNode->setContentSize(Size::ZERO);
	_gameNode->setScale(gameScale);
	_gameNode->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	_gameNode->setPosition(winSize.width / 2, winSize.height / 2);
	addChild(_gameNode);

	_bridgeMainNode = NodeBridgeMain::create();
	_gameNode->addChild(_bridgeMainNode);

	_blockMainNode = NodeBlockMain::create();
	_blockMainNode->setContentSize(Size::ZERO);
	_blockMainNode->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
	_blockMainNode->setPosition(0, -winSize.height / 2);
	_gameNode->addChild(_blockMainNode);
	_blockMainNode->createBlockList();

	makeSpeakerButton(winSize);

    auto backButton = TodoSchoolBackButton::create();
    backButton->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
    backButton->setPosition(Vec2(25, winSize.height-25));
    addChild(backButton);
    
    
    _progressBar = ProgressIndicator::create();
    _progressBar->setPosition(Vec2(winSize.width/2, winSize.height - _progressBar->getContentSize().height));
    addChild(_progressBar);




    
    if (UserManager::getInstance()->isDebugMode()) {
        auto skip = Button::create();
        skip->setTitleFontSize(100);
        skip->setTitleText("Skip");
        skip->setAnchorPoint(Vec2::ANCHOR_TOP_RIGHT);
        skip->setPosition(Vec2(winSize.width-25, winSize.height-25));
        addChild(skip);
        skip->addTouchEventListener([this](Ref*,Widget::TouchEventType e) {
            if (e == Widget::TouchEventType::ENDED) {
				if (_touchEnabled)
				{
					_bridgeMainNode->debugSolveAll();
					onBridgeComplete();
				}
            }
        });
        
    }
    
    return true;
}

void SentenceBridgeScene::onEnter()
{
    Layer::onEnter();

	startGame();
}

void SentenceBridgeScene::onEnterTransitionDidFinish()
{
	Layer::onEnterTransitionDidFinish();
}

void SentenceBridgeScene::onExit()
{
	Layer::onExit();

	_speakerButton = nullptr;
	_shiningParticleNode->removeAllChildren();
	_shiningParticleNode = nullptr;
	_blockMainNode->removeObjects();
	_bridgeMainNode->removeObjects();
	_gameNode->removeAllChildren();
	this->removeAllChildren();
	this->unscheduleAllCallbacks();
	GameSoundManager::getInstance()->stopAllEffects();
	SpriteFrameCache::getInstance()->removeUnusedSpriteFrames();
	Director::getInstance()->getTextureCache()->removeUnusedTextures();
}



void SentenceBridgeScene::setLevel(int level)
{
	_currentLevel = level;
}

void SentenceBridgeScene::startGame()
{
    loadData(_currentLevel, &_currentWorkSheet);
    _currentProblemIndex = 0;

	loadDurationsheet();

	_bridgeMainNode->createBridgeList(_problems, getContentSize());
    _progressBar->setMax(_problems.size());
    showBridge(_currentProblemIndex);
    _blockMainNode->showCapslockBlinkAnimation(getCurrentDuration() + 0.5f);
	showSpeakerButton();

	_touchEnabled = true;
processBlockTouch();
touchSpeakerButton(0.5f);
createShiningParticle();
	//startParticle();
}

void SentenceBridgeScene::makeSpeakerButton(Size winSize)
{
	_speakerButton = Sprite::create(resourcePath + "button-speaker-normal.png");
	_speakerButton->setPosition(0, winSize.height / 2 + 300);
	_gameNode->addChild(_speakerButton);
	
	auto *listener = EventListenerTouchOneByOne::create();
	listener->setSwallowTouches(true);
	listener->onTouchBegan = [this](Touch* T, Event* E) {
		if (_touchEnabled && !_speakerPlaying)
		{
		    auto pos = _speakerButton->getParent()->convertToNodeSpace(T->getLocation());
			if (_speakerButton->getBoundingBox().containsPoint(pos)) {
				 touchSpeakerButton(0);
			}
		}
		return false;
	};
	_speakerButton->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);
}
// Implementation of tts for this module
bool SentenceBridgeScene::touchSpeakerButton(float delayTime)
{
	_speakerPlaying = true;
	_speakerButton->setTexture(resourcePath + "button-speaker-playing.png");

	string strSoundFileName = _problems[_currentProblemIndex].SentenceSound;
	float fSoundFileDuration = getDuration(strSoundFileName);
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
	TodoUtil::replace(strSoundFileName, ".m4a", ".mp3");
#endif

	if (delayTime > 0)
	{
		runAction(Sequence::create(
			DelayTime::create(delayTime),
			CallFunc::create([this, strSoundFileName]() {
				VoiceMoldManager::shared()->speak(strSoundFileName);
			}),
			nullptr
		));
	}
	else
	{
		VoiceMoldManager::shared()->speak(strSoundFileName);
	}
	


	auto duration = delayTime + fSoundFileDuration;

	this->runAction(Sequence::create(
		DelayTime::create(duration),
		CallFunc::create([this]() {
		_speakerPlaying = false;
		_speakerButton->setTexture(resourcePath + "button-speaker-normal.png");
	}), nullptr));
	
	return true;
}

void SentenceBridgeScene::processBlockTouch()
{
	_blockMainNode->onBlockTouchBegan = [this]() {
        if (_touchEnabled && !_blockMoving) {
            touchSpeakerButton(0.5f);
            _blockMoving = true;
            return  true;
        }
        return false;
	};

	_blockMainNode->onBlockTouchMoved = [this](string letter, Touch* touch) {
		_bridgeMainNode->blockTouchMoved(letter, touch);
	};

	_blockMainNode->onBlockTouchEnded = [this](NodeBlock* blockNode, Touch* touch) {
        _blockMoving = false;
		auto workPath = [&] {
			stringstream ss;
			ss << "/" << "SentenceBridge";
			ss << "/" << "level-" << _currentLevel << "-" << _currentWorkSheet;
			ss << "/" << "work-" << (_currentProblemIndex+1);
			return ss.str();
		}();

		auto foundSentenceIdx = _bridgeMainNode->blockTouchEnded(blockNode->_strLetter, touch, workPath);
		if (foundSentenceIdx < 0)
		{
			return false;
		}
		onCorrectAnswer(blockNode, foundSentenceIdx);
		return true;
	};
    
    _blockMainNode->onBlockTouchEnded_cancel = [this](NodeBlock* blockNode, Touch* touch) {
        _blockMoving = false;
        return true;
    };
}

void SentenceBridgeScene::showSpeakerButton()
{
	_speakerButton->setPosition(0, gameSize.height / 2 + 300);
	_speakerButton->runAction(
		EaseBackInOut::create(MoveTo::create(0.5f, Vec2(0, gameSize.height / 2 - 300)))//, 2)
	);
}

void SentenceBridgeScene::hideSpeakerButton()
{
	_speakerButton->setPosition(0, gameSize.height / 2 - 300);
	_speakerButton->runAction(
		EaseBackInOut::create(MoveTo::create(0.5f, Vec2(0, gameSize.height / 2 + 300)))//, 2)
	);
}

void SentenceBridgeScene::showBridge(int index)
{
    _progressBar->setCurrent(index+1, false);
    //createPuzzle(index);
    
    _blockMainNode->onStartStage(_problems[index].UpperCase, _problems[index].HasAlphabet);
}

void SentenceBridgeScene::onCorrectAnswer(NodeBlock* blockNode, int foundSentenceIdx)
{
	_touchEnabled = false;
	startParticle(blockNode->getBlockWorldPosition());
	_bridgeMainNode->showAnswer(blockNode, foundSentenceIdx, [this]() {
        _blockMainNode->turnoffCapslock();
		stopParticle();
		if (_bridgeMainNode->areAllProblemsSolved())
		{
			onBridgeComplete();
		}
		else
		{
			_touchEnabled = true;
		}
	});
}

void SentenceBridgeScene::onBridgeComplete()
{
	_touchEnabled = false;
	hideSpeakerButton();
	touchSpeakerButton(0.5f);

	if (_currentProblemIndex < (int)_problems.size() - 1)
	{
		_bridgeMainNode->moveCarToNext(_currentProblemIndex + 1, 5, false);
	}
	else
	{
		_bridgeMainNode->moveCarToNext(_currentProblemIndex + 2, 5, true);
	}

	runAction(Sequence::create(
		DelayTime::create(5),
		CallFunc::create([this]() {onBridgeComplete_step2(); }),
		nullptr
	));
}

void SentenceBridgeScene::onBridgeComplete_step2()
{
	if (_currentProblemIndex < (int)_problems.size() - 1)
	{
		_currentProblemIndex++;
		showBridge(_currentProblemIndex);
        _bridgeMainNode->moveToNextProblem(_currentProblemIndex, [this]() {
            _blockMainNode->showCapslockBlinkAnimation(getCurrentDuration() + 0.5f);
            showSpeakerButton();
            touchSpeakerButton(0.5f);
            _touchEnabled = true;
        });

#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
		GameSoundManager::getInstance()->playEffectSound("Common/Sounds/Effect/success.mp3");
#else
		GameSoundManager::getInstance()->playEffectSound("Common/Sounds/Effect/success.m4a");
#endif
	}
	else
	{
		auto CP = CompletePopup::create();
		CP->show(0, [this] {
			CCAppController::sharedAppController()->handleGameComplete(1);
		});
	}
}





void SentenceBridgeScene::createShiningParticle()
{
	_shiningParticleNode = Node::create();

	ParticleSystemQuad* particleEffect = nullptr;


	auto createParticleEffect = [&](std::string name, std::string plist) {
		particleEffect = ParticleSystemQuad::create(plist);
		particleEffect->setName(name);
		particleEffect->setScale(1);
		particleEffect->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
		particleEffect->setPosition(Vec2::ZERO);
		particleEffect->setGlobalZOrder(100);
		particleEffect->setColor(Color3B::YELLOW);
		particleEffect->stopSystem();
		_shiningParticleNode->addChild(particleEffect);
	};

	createParticleEffect("particle1", "common/effects/particle/shining_particle_blur.plist");
	createParticleEffect("particle2", "common/effects/particle/shining_particle_circle.plist");
	createParticleEffect("particle3", "common/effects/particle/shining_particle_star.plist");

	_shiningParticleNode->setPosition(Vec2::ZERO);
	_gameNode->addChild(_shiningParticleNode);

}

void SentenceBridgeScene::startParticle(Vec2 worldPos)
{
	Vec2 nodePos = _shiningParticleNode->getParent()->convertToNodeSpace(worldPos);
	_shiningParticleNode->setPosition(nodePos);
	_shiningParticleNode->getChildByName<ParticleSystemQuad*>("particle1")->resetSystem();
	_shiningParticleNode->getChildByName<ParticleSystemQuad*>("particle2")->resetSystem();
	_shiningParticleNode->getChildByName<ParticleSystemQuad*>("particle3")->resetSystem();
}

void SentenceBridgeScene::stopParticle()
{
	_shiningParticleNode->getChildByName<ParticleSystemQuad*>("particle1")->stopSystem();
	_shiningParticleNode->getChildByName<ParticleSystemQuad*>("particle2")->stopSystem();
	_shiningParticleNode->getChildByName<ParticleSystemQuad*>("particle3")->stopSystem();
}

void SentenceBridgeScene::loadData(int level, int *worksheetID)
{
    string P = "Games/" + resourcePath + "sentencebridge_level.tsv";
    string S = FileUtils::getInstance()->getStringFromFile(P);

    auto data = TodoUtil::readTSV(S);
   
    auto Lang = LanguageManager::getInstance()->getCurrentLanguageTag();
    

    string tsv_LanguageTag;
    int tsv_Level;
    int tsv_Worksheet;
	vector<int> worksheets;
	worksheets.clear();
	for (auto row : data)
	{
		if (row.size() < 1) continue;
		if (TodoUtil::trim(row[0]).size() <= 0) continue;
		if (row[0][0] == '#') continue;

		tsv_LanguageTag = row[0];
		tsv_Level = TodoUtil::stoi(row[1]);
		tsv_Worksheet = TodoUtil::stoi(row[2]);

		if (tsv_LanguageTag == Lang && tsv_Level == level)
		{
			if (find(worksheets.begin(), worksheets.end(), tsv_Worksheet) == worksheets.end())
			{
				worksheets.push_back(tsv_Worksheet);
			}
		}
	}

	*worksheetID = worksheets[random<int>(0, worksheets.size() - 1)];
	worksheets.clear();


	_problems.clear();
    
	SentenceBridgeProblemStruct problem;

	for (auto row : data)
	{
		if (row.size() < 1) continue;
		if (TodoUtil::trim(row[0]).size() <= 0) continue;
		if (row[0][0] == '#') continue;

		tsv_LanguageTag = row[0];
		tsv_Level = TodoUtil::stoi(row[1]);
		tsv_Worksheet = TodoUtil::stoi(row[2]);

		if (tsv_LanguageTag == Lang && tsv_Level == level && (UserManager::getInstance()->isWorksheetTestMode() || tsv_Worksheet == *worksheetID))
		{
			string strSentence = row[4];
			problem.Sentence = "";
			problem.SentenceSound = row[5];
            problem.UpperCase = row[6] == "y";

			problem.ProblemSentenceIndices.clear();
			problem.multiLineStartPos = -1;
            problem.HasAlphabet = false;

			bool bIsAnswer = false;
			for (char sentenceChar : strSentence)
			{
				switch (sentenceChar)
				{
				case '[':
					bIsAnswer = true;
					break;
				case ']':
					bIsAnswer = false;
					break;
				case '@':
					problem.multiLineStartPos = (int)problem.Sentence.length();
					break;
				default:
					if (bIsAnswer)
					{
						problem.ProblemSentenceIndices.push_back((int)problem.Sentence.length());
                        
                        if (!problem.HasAlphabet)
                        {
                            if (isalpha(sentenceChar))
                            {
                                problem.HasAlphabet = true;
                            }
                        }
					}
					problem.Sentence += sentenceChar;
					break;
				}
			}

			_problems.push_back(problem);
		}
	}
}

void SentenceBridgeScene::loadDurationsheet() {

	if (_duration.size() != 0) return;
	std::string rawString = cocos2d::FileUtils::getInstance()->getStringFromFile("SentenceBridge/Sound/Durations.tsv");
	auto data = TodoUtil::readTSV(rawString);

	for (auto row : data) {
		if (TodoUtil::trim(row[0]).size() <= 0) continue;
		if (row.size() == 1) continue;
		auto row1Vec = TodoUtil::split(row[1], ':');
		TodoUtil::replaceAll(row1Vec[2], ".", "");
		auto rowDuration = (float)TodoUtil::stoi(row1Vec[2]) / 100;
		_duration[row[0]] = rowDuration;
	}
}

float SentenceBridgeScene::getDuration(string name)
{
	if (_duration.count(name))
		return _duration[name];

	return 2.5;
}

float SentenceBridgeScene::getCurrentDuration()
{
    return getDuration(_problems[_currentProblemIndex].SentenceSound);
}



END_NS_SENTENCEBRIDGE;
