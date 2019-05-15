//
//  WordKickerScene.cpp
//  KitkitSchool
//
//  Created by MilliSoftGames on 8/14/18.
//
//

#include "WordKickerScene.hpp"
#include "WordKicker_NodeBall.hpp"
#include "WordKicker_NodeBirdGoalkeeper.hpp"
#include "WordKicker_NodeBirdKicker.hpp"
#include "WordKicker_NodeGoalpost.hpp"

#include "Utils/TodoUtil.h"

#include "Common/Controls/TodoSchoolBackButton.hpp"
#include "Common/Controls/CompletePopup.hpp"


#include "CCAppController.hpp"

using namespace cocos2d::ui;
using namespace std;


BEGIN_NS_WORDKICKER;


Scene* WordKickerScene::createScene(string levelID, GAMETYPE type)
{
    if (type == MATHKICKER) {
        gameName = "MathKicker";
        resourcePath = "mathkicker/";
    } else {
		gameName = "WordKicker";
		resourcePath = "wordkicker/";

	}
    
    auto level = TodoUtil::stoi(levelID);
    
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = WordKickerScene::create();
    layer->setLevel(TodoUtil::stoi(levelID));
    
    // add layer as a child to scene
    scene->addChild(layer);
    
    // return the scene
    return scene;
}

bool WordKickerScene::init()
{
    if (!Layer::init())
    {
        return false;
    }
    
	std::srand((unsigned)time(nullptr));

	_gameNode = nullptr;

	_touchEnabled = false;
	_speakerPlaying = false;
    
    auto winSize = getContentSize();
	auto gameScale = winSize.height / gameSize.height;
    
    auto bg = Sprite::create(resourcePath+"bg.png");
    bg->setScale(gameScale);
	bg->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
    bg->setPosition(winSize.width/2, 0);
    addChild(bg);
    
	_gameNode = Node::create();
	//_gameNode->setContentSize(gameSize);
	_gameNode->setContentSize(Size::ZERO);
	_gameNode->setScale(gameScale);
	_gameNode->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	_gameNode->setPosition(winSize.width / 2, winSize.height / 2);
	addChild(_gameNode);

	_speachballoonNode = Node::create();
	_speachballoonNode->setContentSize(Size::ZERO);
	_speachballoonNode->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	_speachballoonNode->setPosition(0, 527);
	_speachballoonNode->setVisible(false);
	_gameNode->addChild(_speachballoonNode);


	_nodeGoalpost = WordKicker_NodeGoalpost::create();
	_nodeGoalpost->createSprites();
	_gameNode->addChild(_nodeGoalpost);
	
	_nodeBirdGoalkeeper = WordKicker_NodeBirdGoalkeeper::create();
	_nodeBirdGoalkeeper->createSprites();
	_gameNode->addChild(_nodeBirdGoalkeeper);

	_nodeBirdKicker = WordKicker_NodeBirdKicker::create();
	_nodeBirdKicker->createSprites();
	_gameNode->addChild(_nodeBirdKicker);

	_nodeBall = WordKicker_NodeBall::create();
	_nodeBall->createSprites();
	_gameNode->addChild(_nodeBall);


	_examplesNode = Node::create();
	_examplesNode->setContentSize(Size::ZERO);
	_examplesNode->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	_examplesNode->setPosition(0, -630);
	_examplesNode->setVisible(false);
	_gameNode->addChild(_examplesNode);

	makeExampleButtons();



	//_speachballoonSprite = Scale9Sprite::create(CCRectMake(100,35,80,90), resourcePath + "speachballoon.png");
	_speachballoonSprite = Scale9Sprite::create(Rect(200, 0, 21, 380), resourcePath + "WardKicker_image_yellow.png");
	_speachballoonSprite->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	_speachballoonSprite->setContentSize(Size(1200, 380));
	_speachballoonSprite->setPosition(0, 0);
	_speachballoonNode->addChild(_speachballoonSprite);

	makeSpeakerButton();

	_node_speachballoon_labels = Node::create();
	_node_speachballoon_labels->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
	_node_speachballoon_labels->setPosition(76 + _sprite_speaker->getContentSize().width + 74, 380/2);
	_speachballoonSprite->addChild(_node_speachballoon_labels);
	


	_maxBalloonLabelWidth = getContentSize().width - (72 + (76 + _sprite_speaker->getContentSize().width + 74 + 100) + 72);




	
    auto backButton = TodoSchoolBackButton::create();
    backButton->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
    backButton->setPosition(Vec2(25, winSize.height-25));
    addChild(backButton);
    
    
    _progressBar = ProgressIndicator::create();
    _progressBar->setPosition(Vec2(winSize.width/2, winSize.height - _progressBar->getContentSize().height));
    addChild(_progressBar);




	_label_debug_timer = nullptr;
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
					onExampleSelected(_sprite_example_answer, false);
				}
            }
        });
        

		_label_debug_timer = TodoUtil::createLabel("0", 80, Size::ZERO, fontName, colorLabel, TextHAlignment::CENTER);
		_label_debug_timer->setAnchorPoint(Vec2::ANCHOR_TOP_RIGHT);
		_label_debug_timer->setPosition(Vec2(winSize.width - 25, winSize.height - 25 - 100));
		addChild(_label_debug_timer);
    }
    
    return true;
}

void WordKickerScene::onEnter()
{
    Layer::onEnter();

	startGame();
}

void WordKickerScene::onEnterTransitionDidFinish()
{
	Layer::onEnterTransitionDidFinish();
}

void WordKickerScene::onExit()
{
	Layer::onExit();

	_node_speachballoon_labels->removeAllChildren();
	_speachballoonNode->removeAllChildren();
	_examplesNode->removeAllChildren();
	_gameNode->removeAllChildren();
	this->removeAllChildren();
	this->unscheduleAllCallbacks();
	GameSoundManager::getInstance()->stopAllEffects();
	SpriteFrameCache::getInstance()->removeUnusedSpriteFrames();
	Director::getInstance()->getTextureCache()->removeUnusedTextures();
}



void WordKickerScene::setLevel(int level)
{
	_currentLevel = level;
}

void WordKickerScene::startGame()
{
	loadData(_currentLevel, &_currentWorkSheet);
	loadDurationsheet();
	_progressBar->setMax(_problems.size());

	_currentProblemIndex = 0;
	startGame_show();
}

void WordKickerScene::startGame_show()
{
	resetExampleButtons(true);

	if (_currentProblemIndex >= (int)_problems.size())
	{
		_touchEnabled = false;

		_nodeGoalpost->resetGlobalZOrder();

		auto CP = CompletePopup::create();
		CP->show(1.0f, [this] {
			CCAppController::sharedAppController()->handleGameComplete(1);
		});
	}
	else
	{
		_progressBar->setCurrent(_currentProblemIndex + 1, false);

		_gameState = eGameState::WORDKICKER_PLAYING;
		_gameStateTime = 0;

		_touchEnabled = false;


		_nodeGoalpost->startNewProblem();
		_nodeBall->startNewProblem();
		_nodeBirdGoalkeeper->startNewProblem();
		_nodeBirdKicker->startNewProblem();

		resetBirdMarkInBalloon();

		showProblem();



		_speachballoonNode->setPosition(0, 527 + 900);
		_examplesNode->setPosition(0, -630 - 900);

		_speachballoonNode->runAction(Sequence::create(
			DelayTime::create(0.2f),
			EaseBackOut::create(MoveTo::create(0.5f, Vec2(0, 527))),
			nullptr
		));
		
		_examplesNode->runAction(Sequence::create(
			DelayTime::create(0.5f),
			EaseBackOut::create(MoveTo::create(0.5f, Vec2(0, -630))),
			CallFunc::create([this]() {
				_touchEnabled = true;
				startTimer();
				touchSpeakerButton(false, true);
			}),
			DelayTime::create(0.05f),
			nullptr
		));
	}
}

void WordKickerScene::startGame_NextProblem()
{
	if (_currentProblemIndex < (int)_problems.size() - 1)
	{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
		GameSoundManager::getInstance()->playEffectSound("Common/Sounds/Effect/success.mp3");
#else
		GameSoundManager::getInstance()->playEffectSound("Common/Sounds/Effect/success.m4a");
#endif
	}

	_speachballoonNode->runAction(MoveTo::create(0.2f, Vec2(0, 527 + 900)));

	_examplesNode->runAction(Sequence::create(
		MoveTo::create(0.2f, Vec2(0, -630 - 900)),
		CallFunc::create([this]() {
			_currentProblemIndex++;
			startGame_show();
		}),
		nullptr
	));
}


void WordKickerScene::startTimer()
{
	/*
	auto problemLength = _problems[_currentProblemIndex].Sentence.length();
	if (problemLength < 16) _remainTime = 5;
	else if (problemLength < 24) _remainTime = 8;
	else _remainTime = 10;
	*/
	_remainTime = _problems[_currentProblemIndex].Time;
	if (_remainTime <= 0)
	{
		_remainTime = 99999;
	}

	if (_label_debug_timer != nullptr)
	{
		_label_debug_timer->setString(TodoUtil::itos(_remainTime));
	}

	if (_remainTime < 99999)
	{
		schedule(schedule_selector(WordKickerScene::onTimerTick), 1.0f);
	}
}

void WordKickerScene::stopTimer()
{
	unschedule(schedule_selector(WordKickerScene::onTimerTick));
}

void WordKickerScene::onTimerTick(float fDeltaTime)
{
	_remainTime--;
	if (_remainTime <= 0)
	{
		_remainTime = 0;
		unschedule(schedule_selector(WordKickerScene::onTimerTick));

		if (_gameState == eGameState::WORDKICKER_PLAYING)
		{
			start_timeover();
		}
	}

	if (_label_debug_timer != nullptr)
	{
		_label_debug_timer->setString(TodoUtil::itos(_remainTime));
	}
}

void WordKickerScene::onExampleSelected(Scale9Sprite* spriteParent, bool bMakeLog)
{
	auto workPath = [&] {
		stringstream ss;
		ss << "/" << gameName;
		ss << "/" << "level-" << _currentLevel << "-" << _currentWorkSheet;
		ss << "/" << "work-" << (_currentProblemIndex + 1);
		return ss.str();
	}();
	string userSelected;
	if (spriteParent == _sprite_example_1) userSelected = _exampleStringList[0];
	else if (spriteParent == _sprite_example_2) userSelected = _exampleStringList[1];
	else if (spriteParent == _sprite_example_3) userSelected = _exampleStringList[2];
	else userSelected = _exampleStringList[3];
	StrictLogManager::shared()->game_Peek_Answer(gameName, workPath, userSelected, _problems[_currentProblemIndex].Answer);

	if (spriteParent == _sprite_example_answer)
	{
		if (_currentProblemIndex >= ((int)_problems.size() - 1))
		{
			start_correctanswer_final();
		}
		else
		{
			if (random<int>(0, 1) == 0)
			{
				start_correctanswer_1();
				//start_correctanswer_2();
			}
			else
			{
				start_correctanswer_2();
				//start_correctanswer_1();
			}
		}
	}
	else
	{
		start_wronganswer(spriteParent);
	}
}

void WordKickerScene::start_wronganswer(Scale9Sprite* wrongAnswer)
{
	_touchEnabled = false;
	_gameState = eGameState::WORDKICKER_WRONGANSWER;
	_gameStateTime = 0;
	schedule(schedule_selector(WordKickerScene::onTick_wronganswer));

	_nodeBirdKicker->setAnimation_3_kick_fail();
	_nodeBirdGoalkeeper->setAnimation_5_idle();
	wrongAnswer->setTexture(resourcePath + "card_off.png");
	wrongAnswer->setContentSize(Size(_curExampleButtonWidth, 190));

#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
	GameSoundManager::getInstance()->playEffectSound("Common/Sounds/Effect/wrong.mp3");
#else
	GameSoundManager::getInstance()->playEffectSound("Common/Sounds/Effect/wrong.m4a");
#endif
}

void WordKickerScene::start_timeover()
{
	_touchEnabled = false;
	_gameState = eGameState::WORDKICKER_TIMEOVER;
	_gameStateTime = 0;
	schedule(schedule_selector(WordKickerScene::onTick_timeover));

	resetExampleButtons(false);
	_sprite_example_answer->setTexture(resourcePath + "card_on_down.png");
	_sprite_example_answer->setContentSize(Size(_curExampleButtonWidth, 190));

	showTimeoverBalloon();

	_nodeBirdKicker->setAnimation_1_idle();
	touchSpeakerButton(false, false);
}

void WordKickerScene::start_correctanswer_1()
{
	_touchEnabled = false;
	_gameState = eGameState::WORDKICKER_CORRECTANSWER_1;
	_gameStateTime = 0;
	schedule(schedule_selector(WordKickerScene::onTick_correctanswer_1));

	_nodeBirdGoalkeeper->setAnimation_7_1_goal();
	showAnswerBalloon();
	touchSpeakerButton(true, false);
	stopTimer();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
	GameSoundManager::getInstance()->playEffectSound("Common/Sounds/Effect/right1.mp3");
#else
	GameSoundManager::getInstance()->playEffectSound("Common/Sounds/Effect/right1.m4a");
#endif
}

void WordKickerScene::start_correctanswer_2()
{
	_touchEnabled = false;
	_gameState = eGameState::WORDKICKER_CORRECTANSWER_2;
	_gameStateTime = 0;
	schedule(schedule_selector(WordKickerScene::onTick_correctanswer_2));

	//_nodeBirdGoalkeeper->setAnimation_7_2_goal();
	_nodeBirdGoalkeeper->setAnimation_7_1_goal();
	showAnswerBalloon();
	touchSpeakerButton(true, false);
	stopTimer();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
	GameSoundManager::getInstance()->playEffectSound("Common/Sounds/Effect/right2.mp3");
#else
	GameSoundManager::getInstance()->playEffectSound("Common/Sounds/Effect/right2.m4a");
#endif
}

void WordKickerScene::start_correctanswer_final()
{
	_touchEnabled = false;
	_gameState = eGameState::WORDKICKER_CORRECTANSWER_FINAL;
	_gameStateTime = 0;
	schedule(schedule_selector(WordKickerScene::onTick_correctanswer_final));

	_nodeBirdGoalkeeper->setAnimation_5_idle();
	showAnswerBalloon();
	touchSpeakerButton(true, false);
	stopTimer();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
	GameSoundManager::getInstance()->playEffectSound("Common/Sounds/Effect/right3.mp3");
#else
	GameSoundManager::getInstance()->playEffectSound("Common/Sounds/Effect/right3.m4a");
#endif
}

void WordKickerScene::onTick_wronganswer(float fDeltaTime)
{
	if (_gameState != eGameState::WORDKICKER_WRONGANSWER) return;

	_gameStateTime += fDeltaTime;

	if (_nodeBall->processBall_wronganswer(_gameStateTime, fDeltaTime))
	{
		unschedule(schedule_selector(WordKickerScene::onTick_wronganswer));

		resetExampleButtons(true);
		if (_remainTime > 0)
		{
			_gameState = eGameState::WORDKICKER_PLAYING;
			_gameStateTime = 0;

			_nodeBall->startNewProblem();

			_nodeBirdKicker->setAnimation_1_idle();
			_nodeBirdGoalkeeper->setAnimation_5_idle();

			_touchEnabled = true;
		}
		else
		{
			_nodeBall->startNewProblem_notmove();

			start_timeover();
		}
	}
}

void WordKickerScene::onTick_timeover(float fDeltaTime)
{
	if (_gameState != eGameState::WORDKICKER_TIMEOVER) return;

	_gameStateTime += fDeltaTime;

	if (_nodeBall->processBall_timeover(_gameStateTime, fDeltaTime))
	{
		if (!_speakerPlaying)
		{
			unschedule(schedule_selector(WordKickerScene::onTick_timeover));

			startGame_NextProblem();
		}
	}
}

void WordKickerScene::onTick_correctanswer_1(float fDeltaTime)
{
	if (_gameState != eGameState::WORDKICKER_CORRECTANSWER_1) return;

	_gameStateTime += fDeltaTime;

	if (_nodeBall->processBall_correctanswer_1(_gameStateTime, fDeltaTime))
	{
		if (!_speakerPlaying)
		{
			unschedule(schedule_selector(WordKickerScene::onTick_correctanswer_1));

			startGame_NextProblem();
		}
	}
	else
	{
		_nodeBirdGoalkeeper->setEyesTarget(_nodeBall->GetBallPosition_world());
	}
}

void WordKickerScene::onTick_correctanswer_2(float fDeltaTime)
{
	if (_gameState != eGameState::WORDKICKER_CORRECTANSWER_2) return;

	float fPrevTime = _gameStateTime;
	_gameStateTime += fDeltaTime;

	Vec2 vOldBallPos = _nodeBall->GetBallPosition_local();

	if (_nodeBall->processBall_correctanswer_2(_gameStateTime, fDeltaTime))
	{
		if (!_speakerPlaying)
		{
			unschedule(schedule_selector(WordKickerScene::onTick_correctanswer_2));

			startGame_NextProblem();
		}
	}
	else
	{
		_nodeBirdGoalkeeper->setEyesTarget_onlyX(_nodeBall->GetBallPosition_world());

		Vec2 vNewBallPos = _nodeBall->GetBallPosition_local();

		if (vOldBallPos.x < 600.0f && vNewBallPos.x >= 600.0f)
		{
			_nodeBirdGoalkeeper->setAnimation_feather();
			_nodeBirdGoalkeeper->doJump();
		}

		if (vOldBallPos.x < 1300.0f && vNewBallPos.x >= 1300.0f)
		{
			_nodeGoalpost->startGoalAnimation();
		}
		if (fPrevTime < 2.9f && _gameStateTime >= 2.9f)
		{
			_nodeGoalpost->stopGoalAnimation();
		}
	}
}

void WordKickerScene::onTick_correctanswer_final(float fDeltaTime)
{
	if (_gameState != eGameState::WORDKICKER_CORRECTANSWER_FINAL) return;

	float fPrevTime = _gameStateTime;
	_gameStateTime += fDeltaTime;

	Vec2 vOldBallPos = _nodeBall->GetBallPosition_local();

	if (_nodeBall->processBall_correctanswer_final(_gameStateTime, fDeltaTime))
	{
		if (!_speakerPlaying)
		{
			unschedule(schedule_selector(WordKickerScene::onTick_correctanswer_final));

			startGame_NextProblem();
		}
	}
	else
	{
		Vec2 vNewBallPos = _nodeBall->GetBallPosition_local();

		if (vOldBallPos.x < 600.0f && vNewBallPos.x >= 600.0f)
		{
			_nodeBirdGoalkeeper->setAnimation_feather();
			_nodeBirdGoalkeeper->doFly();
		}

		if (vOldBallPos.x < 1300.0f && vNewBallPos.x >= 1300.0f)
		{
			_nodeGoalpost->startGoalAnimation();
		}
		if (fPrevTime < 2.9f && _gameStateTime >= 2.9f)
		{
			_nodeGoalpost->stopGoalAnimation();
		}
	}
}



/*
void WordKickerScene::showAnimation_kicker_comein()
{
	_speachballoonNode->setVisible(false);


	float animTime = 3;

	Vector<FiniteTimeAction*> vectorKickerSequence;
	vectorKickerSequence.pushBack(Place::create(Vec2(kicker_x_start, kicker_y)));
	int stepCount = 6;
	//float fIncX = (kicker_x_end - kicker_x_start) / (float)stepCount;
	Vec2 vMoveTo = Vec2(kicker_x_start, kicker_y);
	float fHeightMin = 80;
	float fHeightMaxAdd = 120;
	for (int i = 0; i < stepCount; i++)
	{
		float fMoveRatio = (float)i / (float)(stepCount - 1);
		//fMoveRatio = fMoveRatio * (2.0f - fMoveRatio);
		fMoveRatio = powf(fMoveRatio, 0.6f);
		vMoveTo.x = kicker_x_start + (kicker_x_end - kicker_x_start) * fMoveRatio;
		vectorKickerSequence.pushBack(JumpTo::create(animTime / (float)stepCount, vMoveTo, fHeightMin + fHeightMaxAdd / (stepCount - 1) * (stepCount - 1 - i), 1));
	}
	vectorKickerSequence.pushBack(
		CallFunc::create([this]() {
		showProblem();
	}));
	_nodeBirdKicker->runAction(Sequence::create(vectorKickerSequence));
	//_birdKickerSprite->runAction(EaseOut::create(Sequence::create(vectorKickerSequence), 1.2f));


	animTime = 3.4f;

	Vector<FiniteTimeAction*> vectorBallSequence;
	vectorBallSequence.pushBack(Place::create(Vec2(ball_x_start, ball_y)));
	stepCount = 12;
	//fIncX = (ball_x_end - ball_x_start) / (float)stepCount;
	vMoveTo = Vec2(ball_x_start, ball_y);
	fHeightMin = 30;
	fHeightMaxAdd = 70;
	for (int i = 0; i < stepCount; i++)
	{
		float fMoveRatio = (float)i / (float)(stepCount - 1);
		//fMoveRatio = fMoveRatio * (2.0f - fMoveRatio);
		fMoveRatio = powf(fMoveRatio, 0.7f);
		vMoveTo.x = ball_x_start + (ball_x_end - ball_x_start) * fMoveRatio;
		int jumpCount = 1;// (i == stepCount - 1) ? 3 : 2;
		vectorBallSequence.pushBack(JumpTo::create(animTime / (float)stepCount, vMoveTo, fHeightMin + fHeightMaxAdd / (stepCount - 1) * (stepCount - 1 - i), jumpCount));
		//vectorBallSequence.pushBack(EaseOut::create(JumpTo::create(animTime / (float)stepCount, vMoveTo, fHeightMin + fHeightMaxAdd / (stepCount - 1) * (stepCount - 1 - i), 2), 1.1f));
	}
	_nodeBall->runAction(Sequence::create(vectorBallSequence));
	//_ballSprite->runAction(EaseOut::create(Sequence::create(vectorBallSequence), 1.2f));
}
*/

void WordKickerScene::makeSpeakerButton()
{
	_actionSpeakerPlaying = nullptr;
	_speakerSoundID = 0;

	_sprite_speaker = Sprite::create(resourcePath + "WardKicker_button_speaker_normal.png");
	_sprite_speaker->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	_speachballoonSprite->addChild(_sprite_speaker);
	_sprite_speaker->setPosition(76 + _sprite_speaker->getContentSize().width / 2, 380 / 2);

	_sprite_balloonMark_kicker = Sprite::create(resourcePath + "WardKicker_image_thumbnail_kicker.png");
	_sprite_balloonMark_kicker->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	_speachballoonSprite->addChild(_sprite_balloonMark_kicker);
	_sprite_balloonMark_kicker->setPosition(76 + _sprite_speaker->getContentSize().width / 2, 380 / 2);
	_sprite_balloonMark_kicker->setVisible(false);

	_sprite_balloonMark_goalkeeper = Sprite::create(resourcePath + "WardKicker_image_thumbnail_keeper.png");
	_sprite_balloonMark_goalkeeper->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	_speachballoonSprite->addChild(_sprite_balloonMark_goalkeeper);
	_sprite_balloonMark_goalkeeper->setPosition(76 + _sprite_speaker->getContentSize().width / 2, 380 / 2);
	_sprite_balloonMark_goalkeeper->setVisible(false);


	auto *listener = EventListenerTouchOneByOne::create();
	listener->setSwallowTouches(true);
	listener->onTouchBegan = [this](Touch* T, Event* E) {
		if (_touchEnabled && !_speakerPlaying)
		{
			auto pos = _sprite_speaker->getParent()->convertToNodeSpace(T->getLocation());
			if (_sprite_speaker->getBoundingBox().containsPoint(pos)) {
				_sprite_speaker->setTexture(resourcePath + "wardkicker_button_speaker_playing.png");
				return true;
			}
		}
		return false;
	};
	listener->onTouchMoved = [this](Touch* T, Event* E) {
		if (_touchEnabled)
		{
			auto pos = _sprite_speaker->getParent()->convertToNodeSpace(T->getLocation());
			if (_sprite_speaker->getBoundingBox().containsPoint(pos)) {
				_sprite_speaker->setTexture(resourcePath + "wardkicker_button_speaker_playing.png");
			}
			else {
				_sprite_speaker->setTexture(resourcePath + "wardkicker_button_speaker_normal.png");
			}
		}
	};
	listener->onTouchEnded = [this](Touch* T, Event* E) {
		_sprite_speaker->setTexture(resourcePath + "wardkicker_button_speaker_normal.png");
		if (_touchEnabled)
		{
			auto pos = _sprite_speaker->getParent()->convertToNodeSpace(T->getLocation());
			if (_sprite_speaker->getBoundingBox().containsPoint(pos)) {
				touchSpeakerButton(false, true);
			}
		}
	};
	listener->onTouchCancelled = [this](Touch* T, Event* E) {
		_sprite_speaker->setTexture(resourcePath + "wardkicker_button_speaker_normal.png");
	};
	_sprite_speaker->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);
}

void WordKickerScene::resetBirdMarkInBalloon()
{
	if (!_problems[_currentProblemIndex].SentenceSound.empty())
	{
		_sprite_speaker->setVisible(true);
		_sprite_balloonMark_goalkeeper->setVisible(false);
		_sprite_balloonMark_kicker->setVisible(false);
	}
	else
	{
		_sprite_speaker->setVisible(false);
		_sprite_balloonMark_goalkeeper->setVisible(true);
		_sprite_balloonMark_kicker->setVisible(false);
	}
}

bool WordKickerScene::touchSpeakerButton(bool bIsKicker, bool bResetBirdMark)
{
	_speakerPlaying = true;
	_sprite_speaker->setVisible(false);
	if (!bIsKicker)
	{
		_sprite_balloonMark_kicker->setVisible(false);
		_sprite_balloonMark_goalkeeper->setVisible(true);

		_nodeBirdGoalkeeper->setAnimation_6_speak();
	}
	else
	{
		_sprite_balloonMark_kicker->setVisible(true);
		_sprite_balloonMark_goalkeeper->setVisible(false);

		_nodeBirdKicker->setAnimation_2_kick_success();
	}

	string strSoundFileName = _problems[_currentProblemIndex].SentenceSound;
	float fSoundFileDuration = getDuration(strSoundFileName);
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
	TodoUtil::replace(strSoundFileName, ".m4a", ".mp3");
#endif


	auto duration = fSoundFileDuration;

	if (_speakerSoundID > 0)
	{
		GameSoundManager::getInstance()->stopEffectSound(_speakerSoundID);
		_speakerSoundID = 0;
	}

	if (bIsKicker)
	{
		_speakerSoundID = GameSoundManager::getInstance()->playEffectSound(resourcePath + "sound/" + strSoundFileName, false, 1.2f);
		duration /= 1.2f;
	}
	else
	{
		//_speakerSoundID = GameSoundManager::getInstance()->playEffectSound(resourcePath + "sound/" + strSoundFileName);
		GameSoundManager::getInstance()->playEffectSoundVoiceOnly("wordkicker/sound/" + strSoundFileName);

	}


	if (_actionSpeakerPlaying != nullptr)
	{
		this->stopAction(_actionSpeakerPlaying);
		_actionSpeakerPlaying = nullptr;
	}

	_actionSpeakerPlaying = this->runAction(Sequence::create(
		DelayTime::create(duration),
		CallFunc::create([this, bIsKicker, bResetBirdMark]() {
			_speakerPlaying = false;
			_speakerSoundID = 0;
			//_sprite_speaker->setTexture(resourcePath + "WardKicker_button_speaker_normal.png");
			if (bResetBirdMark)
			{
				resetBirdMarkInBalloon();
			}

			if (!bIsKicker)
			{
				_nodeBirdGoalkeeper->setAnimation_5_idle();
			}
			else
			{
				_nodeBirdKicker->setAnimation_1_idle();
			}
		}),
		nullptr
	));

	return true;
}

void WordKickerScene::makeExampleButtons()
{
	//_minExampleLabelWidth = 664 - 72 - 72;
	_minExampleLabelWidth = 892 - 72 - 72;
	_maxExampleLabelWidth = getContentSize().width / 2 - 72 - 72 - 72 - 36;
	//_curExampleButtonWidth = _minExampleLabelWidth + 144;
	_curExampleButtonWidth = _minExampleLabelWidth + 144;

	_sprite_example_1 = Scale9Sprite::create(Rect(70, 0, 88, 190), resourcePath + "card_on.png");
	_sprite_example_2 = Scale9Sprite::create(Rect(70, 0, 88, 190), resourcePath + "card_on.png");
	_sprite_example_3 = Scale9Sprite::create(Rect(70, 0, 88, 190), resourcePath + "card_on.png");
	_sprite_example_4 = Scale9Sprite::create(Rect(70, 0, 88, 190), resourcePath + "card_on.png");

	_label_example_1 = TodoUtil::createLabel("", fontSize_normal, Size::ZERO, fontName, colorLabel, TextHAlignment::CENTER);
	_label_example_2 = TodoUtil::createLabel("", fontSize_normal, Size::ZERO, fontName, colorLabel, TextHAlignment::CENTER);
	_label_example_3 = TodoUtil::createLabel("", fontSize_normal, Size::ZERO, fontName, colorLabel, TextHAlignment::CENTER);
	_label_example_4 = TodoUtil::createLabel("", fontSize_normal, Size::ZERO, fontName, colorLabel, TextHAlignment::CENTER);

	float fX = (_maxExampleLabelWidth + 144) / 2 + 36;

	makeExampleButtons_sub(_sprite_example_1, _label_example_1, -fX, 100);
	makeExampleButtons_sub(_sprite_example_2, _label_example_2, fX, 100);
	makeExampleButtons_sub(_sprite_example_3, _label_example_3, -fX, -100);
	makeExampleButtons_sub(_sprite_example_4, _label_example_4, fX, -100);
}

void WordKickerScene::makeExampleButtons_sub(Scale9Sprite* spriteParent, Label* labelExample, float fPosX, float fPosY)
{
	spriteParent->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	spriteParent->setContentSize(Size(_curExampleButtonWidth, 190));
	spriteParent->setPosition(fPosX, fPosY);
	_examplesNode->addChild(spriteParent);

	labelExample->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	labelExample->setPosition(fPosX, fPosY + 25);
	_examplesNode->addChild(labelExample);


	auto *listener = EventListenerTouchOneByOne::create();
	listener->setSwallowTouches(true);
	listener->onTouchBegan = [this, spriteParent](Touch* T, Event* E) {
		if (_touchEnabled)
		{
			auto pos = spriteParent->getParent()->convertToNodeSpace(T->getLocation());
			if (spriteParent->getBoundingBox().containsPoint(pos))
			{
				spriteParent->setTexture(resourcePath + "card_on_down.png");
				spriteParent->setContentSize(Size(_curExampleButtonWidth, 190));
				return true;
			}
		}
		return false;
	};
	listener->onTouchMoved = [this, spriteParent](Touch* T, Event* E) {
		if (_touchEnabled)
		{
			auto pos = spriteParent->getParent()->convertToNodeSpace(T->getLocation());
			if (spriteParent->getBoundingBox().containsPoint(pos))
			{
				spriteParent->setTexture(resourcePath + "card_on_down.png");
				spriteParent->setContentSize(Size(_curExampleButtonWidth, 190));
			}
			else
			{
				spriteParent->setTexture(resourcePath + "card_on.png");
				spriteParent->setContentSize(Size(_curExampleButtonWidth, 190));
			}
		}
	};
	listener->onTouchEnded = [this, spriteParent](Touch* T, Event* E) {
		if (_touchEnabled)
		{
			spriteParent->setTexture(resourcePath + "card_on.png");
			spriteParent->setContentSize(Size(_curExampleButtonWidth, 190));

			auto pos = spriteParent->getParent()->convertToNodeSpace(T->getLocation());
			if (spriteParent->getBoundingBox().containsPoint(pos))
			{
				onExampleSelected(spriteParent, true);
			}
		}
	};
	listener->onTouchCancelled = [this, spriteParent](Touch* T, Event* E) {
		spriteParent->setTexture(resourcePath + "card_on.png");
		spriteParent->setContentSize(Size(_curExampleButtonWidth, 190));
	};


	spriteParent->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);
}




void WordKickerScene::showProblem()
{
	_speachballoonNode->setVisible(true);
	_node_speachballoon_labels->removeAllChildren();
	
	float fWidth1 = 0;
	float fWidth2 = 0;
	if (_problems[_currentProblemIndex].multiLineStartPos <= 0)
	{
		fWidth1 = makeProblemLabels(_problems[_currentProblemIndex].Sentence,
			_problems[_currentProblemIndex].AnswerStartPos,
			_problems[_currentProblemIndex].AnswerLength,
			0, false, false);
	}
	else
	{
		if (_problems[_currentProblemIndex].AnswerStartPos < _problems[_currentProblemIndex].multiLineStartPos)
		{
			fWidth1 = makeProblemLabels(_problems[_currentProblemIndex].Sentence.substr(0, _problems[_currentProblemIndex].multiLineStartPos),
				_problems[_currentProblemIndex].AnswerStartPos,
				_problems[_currentProblemIndex].AnswerLength,
				70, false, false);
			fWidth2 = makeProblemLabels(_problems[_currentProblemIndex].Sentence.substr(_problems[_currentProblemIndex].multiLineStartPos, _problems[_currentProblemIndex].Sentence.length() - _problems[_currentProblemIndex].multiLineStartPos),
				-1,
				0,
				-70, false, false);
		}
		else
		{
			fWidth1 = makeProblemLabels(_problems[_currentProblemIndex].Sentence.substr(0, _problems[_currentProblemIndex].multiLineStartPos),
				-1,
				0,
				70, false, false);
			fWidth2 = makeProblemLabels(_problems[_currentProblemIndex].Sentence.substr(_problems[_currentProblemIndex].multiLineStartPos, _problems[_currentProblemIndex].Sentence.length() - _problems[_currentProblemIndex].multiLineStartPos),
				_problems[_currentProblemIndex].AnswerStartPos - _problems[_currentProblemIndex].multiLineStartPos,
				_problems[_currentProblemIndex].AnswerLength,
				-70, false, false);
		}
	}

	

	_speachballoonSprite->setTexture(resourcePath + "WardKicker_image_yellow.png");

	makeExamples();


	if (_problems[_currentProblemIndex].SentenceSound.empty())
	{
		float fBalloonSize = _curExampleButtonWidth * 2 + 72;
		calcBalloonSize(fBalloonSize - 76 - _sprite_speaker->getContentSize().width - 175);
	}
	else
	{
		calcBalloonSize(max(fWidth1, fWidth2));
	}
}

void WordKickerScene::showAnswerBalloon()
{
	_speachballoonNode->setVisible(true);
	_node_speachballoon_labels->removeAllChildren();

	float fWidth1 = 0;
	float fWidth2 = 0;
	if (_problems[_currentProblemIndex].multiLineStartPos <= 0)
	{
		fWidth1 = makeProblemLabels(_problems[_currentProblemIndex].Sentence,
			_problems[_currentProblemIndex].AnswerStartPos,
			_problems[_currentProblemIndex].AnswerLength,
			0, true, false);
	}
	else
	{
		if (_problems[_currentProblemIndex].AnswerStartPos < _problems[_currentProblemIndex].multiLineStartPos)
		{
			fWidth1 = makeProblemLabels(_problems[_currentProblemIndex].Sentence.substr(0, _problems[_currentProblemIndex].multiLineStartPos),
				_problems[_currentProblemIndex].AnswerStartPos,
				_problems[_currentProblemIndex].AnswerLength,
				70, true, false);
			fWidth2 = makeProblemLabels(_problems[_currentProblemIndex].Sentence.substr(_problems[_currentProblemIndex].multiLineStartPos, _problems[_currentProblemIndex].Sentence.length() - _problems[_currentProblemIndex].multiLineStartPos),
				-1,
				0,
				-70, true, false);
		}
		else
		{
			fWidth1 = makeProblemLabels(_problems[_currentProblemIndex].Sentence.substr(0, _problems[_currentProblemIndex].multiLineStartPos),
				-1,
				0,
				70, true, false);
			fWidth2 = makeProblemLabels(_problems[_currentProblemIndex].Sentence.substr(_problems[_currentProblemIndex].multiLineStartPos, _problems[_currentProblemIndex].Sentence.length() - _problems[_currentProblemIndex].multiLineStartPos),
				_problems[_currentProblemIndex].AnswerStartPos - _problems[_currentProblemIndex].multiLineStartPos,
				_problems[_currentProblemIndex].AnswerLength,
				-70, true, false);
		}
	}

	_speachballoonSprite->setTexture(resourcePath + "WardKicker_image_green.png");

	resetExampleButtons(false);
	_sprite_example_answer->setTexture(resourcePath + "card_on_down.png");
	_sprite_example_answer->setContentSize(Size(_curExampleButtonWidth, 190));

	if (_problems[_currentProblemIndex].SentenceSound.empty())
	{
		float fBalloonSize = _curExampleButtonWidth * 2 + 72;
		calcBalloonSize(fBalloonSize - 76 - _sprite_speaker->getContentSize().width - 175);
	}
	else
	{
		calcBalloonSize(max(fWidth1, fWidth2));
	}
}

void WordKickerScene::showTimeoverBalloon()
{
	_speachballoonNode->setVisible(true);
	_node_speachballoon_labels->removeAllChildren();

	float fWidth1 = 0;
	float fWidth2 = 0;
	if (_problems[_currentProblemIndex].multiLineStartPos <= 0)
	{
		fWidth1 = makeProblemLabels(_problems[_currentProblemIndex].Sentence,
			_problems[_currentProblemIndex].AnswerStartPos,
			_problems[_currentProblemIndex].AnswerLength,
			0, true, true);
	}
	else
	{
		if (_problems[_currentProblemIndex].AnswerStartPos < _problems[_currentProblemIndex].multiLineStartPos)
		{
			fWidth1 = makeProblemLabels(_problems[_currentProblemIndex].Sentence.substr(0, _problems[_currentProblemIndex].multiLineStartPos),
				_problems[_currentProblemIndex].AnswerStartPos,
				_problems[_currentProblemIndex].AnswerLength,
				70, true, true);
			fWidth2 = makeProblemLabels(_problems[_currentProblemIndex].Sentence.substr(_problems[_currentProblemIndex].multiLineStartPos, _problems[_currentProblemIndex].Sentence.length() - _problems[_currentProblemIndex].multiLineStartPos),
				-1,
				0,
				-70, true, true);
		}
		else
		{
			fWidth1 = makeProblemLabels(_problems[_currentProblemIndex].Sentence.substr(0, _problems[_currentProblemIndex].multiLineStartPos),
				-1,
				0,
				70, true, true);
			fWidth2 = makeProblemLabels(_problems[_currentProblemIndex].Sentence.substr(_problems[_currentProblemIndex].multiLineStartPos, _problems[_currentProblemIndex].Sentence.length() - _problems[_currentProblemIndex].multiLineStartPos),
				_problems[_currentProblemIndex].AnswerStartPos - _problems[_currentProblemIndex].multiLineStartPos,
				_problems[_currentProblemIndex].AnswerLength,
				-70, true, true);
		}
	}

	_speachballoonSprite->setTexture(resourcePath + "WardKicker_image_yellow.png");

	if (_problems[_currentProblemIndex].SentenceSound.empty())
	{
		float fBalloonSize = _curExampleButtonWidth * 2 + 72;
		calcBalloonSize(fBalloonSize - 76 - _sprite_speaker->getContentSize().width - 175);
	}
	else
	{
		calcBalloonSize(max(fWidth1, fWidth2));
	}
}

void WordKickerScene::calcBalloonSize(float fLabelsWidth)
{
	if (fLabelsWidth > _maxBalloonLabelWidth)
	{
		fLabelsWidth = _maxBalloonLabelWidth;
	}

	Size balloonSize = Size(76 + _sprite_speaker->getContentSize().width + fLabelsWidth + 175, 380);
	_speachballoonSprite->setContentSize(balloonSize);

	float fLabelsRealWidth = 0;
	auto labelNodes = _node_speachballoon_labels->getChildren();
	for (auto iter = labelNodes.begin(); iter != labelNodes.end(); ++iter)
	{
		float fWidth = (*iter)->getPositionX() + (*iter)->getContentSize().width;
		if (fWidth > fLabelsRealWidth)
		{
			fLabelsRealWidth = fWidth;
		}
	}

	float scaleX = fLabelsWidth / fLabelsRealWidth;
	if (scaleX > 1.0f)
	{
		scaleX = 1.0f;
	}
	_node_speachballoon_labels->setScale(scaleX);
	//}
}

float WordKickerScene::makeProblemLabels(string strSentence, int nAnswerStartPos, int nAnswerLength, float fLabelPosY, bool bShowAnswer, bool bIsTimeover)
{
	float fWidth = 0;
	float fRetWidth = 0;

	float fAnswerWidth = 600;
	float fontSize = fontSize_normal;
	if (_problems[_currentProblemIndex].SentenceSound.empty())
	{
		fAnswerWidth = 300;
		fontSize = fontSize_math;
	}

	if (nAnswerStartPos < 0)
	{
		Label* problemLabel = TodoUtil::createLabel(strSentence, fontSize, Size::ZERO, fontName, colorLabel, TextHAlignment::CENTER);
		problemLabel->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
		problemLabel->setPosition(fWidth, fLabelPosY);
		_node_speachballoon_labels->addChild(problemLabel);

		fWidth += problemLabel->getContentSize().width;
		fRetWidth += problemLabel->getContentSize().width;
	}
	else
	{
		if (nAnswerStartPos > 0)
		{
			Label* problemLabel = TodoUtil::createLabel(strSentence.substr(0, nAnswerStartPos), fontSize, Size::ZERO, fontName, colorLabel, TextHAlignment::CENTER);
			problemLabel->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
			problemLabel->setPosition(fWidth, fLabelPosY);
			_node_speachballoon_labels->addChild(problemLabel);

			fWidth += problemLabel->getContentSize().width;
			fRetWidth += problemLabel->getContentSize().width;
		}

		{
			// answer
			if (bShowAnswer)
			{
				Label* problemLabel = TodoUtil::createLabel(strSentence.substr(nAnswerStartPos, nAnswerLength), fontSize, Size::ZERO, fontName, bIsTimeover ? colorTimeoverAnswerLabel : colorAnswerLabel, TextHAlignment::CENTER);
				problemLabel->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
				problemLabel->setPosition(fWidth, fLabelPosY);
				_node_speachballoon_labels->addChild(problemLabel);

				fWidth += problemLabel->getContentSize().width;
				fRetWidth += fAnswerWidth;
			}
			else
			{
				Scale9Sprite* spriteAnswer = Scale9Sprite::create(Rect(20, 0, 164, 18), resourcePath + "WardKicker_image_line.png");
				spriteAnswer->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
				spriteAnswer->setContentSize(Size(fAnswerWidth, 18));
				spriteAnswer->setPosition(fWidth, fLabelPosY - 60);
				_node_speachballoon_labels->addChild(spriteAnswer);

				fWidth += fAnswerWidth;
				fRetWidth += fAnswerWidth;
			}
		}

		if (nAnswerStartPos + nAnswerLength < (int)strSentence.length())
		{
			Label* problemLabel = TodoUtil::createLabel(strSentence.substr(nAnswerStartPos + nAnswerLength, strSentence.length() - (nAnswerStartPos + nAnswerLength)), fontSize, Size::ZERO, fontName, colorLabel, TextHAlignment::CENTER);
			problemLabel->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
			problemLabel->setPosition(fWidth, fLabelPosY);
			_node_speachballoon_labels->addChild(problemLabel);

			fWidth += problemLabel->getContentSize().width;
			fRetWidth += problemLabel->getContentSize().width;
		}

	}

	return fRetWidth;
}

void WordKickerScene::makeExamples()
{
	_examplesNode->setVisible(true);

	_exampleStringList.clear();
	_exampleStringList.push_back(_problems[_currentProblemIndex].Word1);
	_exampleStringList.push_back(_problems[_currentProblemIndex].Word2);
	_exampleStringList.push_back(_problems[_currentProblemIndex].Word3);
	_exampleStringList.push_back(_problems[_currentProblemIndex].Answer);

	_sprite_example_answer = nullptr;

	std::random_shuffle(_exampleStringList.begin(), _exampleStringList.end());

	_label_example_1->setString(_exampleStringList[0]);
	_label_example_2->setString(_exampleStringList[1]);
	_label_example_3->setString(_exampleStringList[2]);
	_label_example_4->setString(_exampleStringList[3]);

	if (_problems[_currentProblemIndex].Answer == _exampleStringList[0]) _sprite_example_answer = _sprite_example_1;
	else if (_problems[_currentProblemIndex].Answer == _exampleStringList[1]) _sprite_example_answer = _sprite_example_2;
	else if (_problems[_currentProblemIndex].Answer == _exampleStringList[2]) _sprite_example_answer = _sprite_example_3;
	else if (_problems[_currentProblemIndex].Answer == _exampleStringList[3]) _sprite_example_answer = _sprite_example_4;


	float fLabelWidth_1 = _label_example_1->getContentSize().width;
	float fLabelWidth_2 = _label_example_2->getContentSize().width;
	float fLabelWidth_3 = _label_example_3->getContentSize().width;
	float fLabelWidth_4 = _label_example_4->getContentSize().width;
	float fLabelWidth_max = fLabelWidth_1;
	if (fLabelWidth_2 > fLabelWidth_max) fLabelWidth_max = fLabelWidth_2;
	if (fLabelWidth_3 > fLabelWidth_max) fLabelWidth_max = fLabelWidth_3;
	if (fLabelWidth_4 > fLabelWidth_max) fLabelWidth_max = fLabelWidth_4;

	if (fLabelWidth_max < _minExampleLabelWidth)
	{
		fLabelWidth_max = _minExampleLabelWidth;
	}
	else if (fLabelWidth_max > _maxExampleLabelWidth)
	{
		fLabelWidth_max = _maxExampleLabelWidth;
	}
	_curExampleButtonWidth = fLabelWidth_max + 144;
	resetExampleButtons(true);

	if (fLabelWidth_1 < fLabelWidth_max) _label_example_1->setScale(1);
	else _label_example_1->setScale(fLabelWidth_max / fLabelWidth_1);
	
	if (fLabelWidth_2 < fLabelWidth_max) _label_example_2->setScale(1);
	else _label_example_2->setScale(fLabelWidth_max / fLabelWidth_2);

	if (fLabelWidth_3 < fLabelWidth_max) _label_example_3->setScale(1);
	else _label_example_3->setScale(fLabelWidth_max / fLabelWidth_3);

	if (fLabelWidth_4 < fLabelWidth_max) _label_example_4->setScale(1);
	else _label_example_4->setScale(fLabelWidth_max / fLabelWidth_4);

	float fX = _curExampleButtonWidth / 2 + 36;
	
	_sprite_example_1->setPosition(-fX, 100);
	_sprite_example_2->setPosition(fX, 100);
	_sprite_example_3->setPosition(-fX, -100);
	_sprite_example_4->setPosition(fX, -100);

	_label_example_1->setPosition(-fX, 100 + 25);
	_label_example_2->setPosition(fX, 100 + 25);
	_label_example_3->setPosition(-fX, -100 + 25);
	_label_example_4->setPosition(fX, -100 + 25);
}


void WordKickerScene::resetExampleButtons(bool bOn)
{
	string textureName = resourcePath + (bOn ? "card_on.png" : "card_off.png");

	_sprite_example_1->setTexture(textureName);
	_sprite_example_1->setContentSize(Size(_curExampleButtonWidth, 190));

	_sprite_example_2->setTexture(textureName);
	_sprite_example_2->setContentSize(Size(_curExampleButtonWidth, 190));

	_sprite_example_3->setTexture(textureName);
	_sprite_example_3->setContentSize(Size(_curExampleButtonWidth, 190));

	_sprite_example_4->setTexture(textureName);
	_sprite_example_4->setContentSize(Size(_curExampleButtonWidth, 190));
}


void WordKickerScene::loadData(int level, int *worksheetID)
{
    string P = "Games/" + resourcePath + gameName + "_level.tsv";
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
    
	WordKickerProblemStruct problem;

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
			problem.Sentence = row[4];
			problem.SentenceSound = row[5];
			problem.Word1 = row[6];
			problem.Word2 = row[7];
			problem.Word3 = row[8];
			problem.Time = TodoUtil::stoi(row[9]);

			auto answerStartPos = problem.Sentence.find('[');
			auto answerEndPos = problem.Sentence.find(']');

			if (answerStartPos != string::npos && answerEndPos != string::npos && answerEndPos > answerStartPos)
			{
				problem.AnswerStartPos = answerStartPos;
				problem.AnswerLength = answerEndPos - answerStartPos - 1;

				problem.Sentence = problem.Sentence.erase(answerEndPos, 1).erase(answerStartPos,1);
				problem.Answer = problem.Sentence.substr(problem.AnswerStartPos, problem.AnswerLength);

				problem.multiLineStartPos = problem.Sentence.find('@');
				if (problem.multiLineStartPos > -1)
				{
					problem.Sentence = problem.Sentence.erase(problem.multiLineStartPos, 1);
					if (problem.AnswerStartPos > problem.multiLineStartPos)
					{
						problem.AnswerStartPos--;
					}
				}

				_problems.push_back(problem);
			}
		}
	}
}

void WordKickerScene::loadDurationsheet() {

	if (_duration.size() != 0) return;
	std::string rawString = cocos2d::FileUtils::getInstance()->getStringFromFile(resourcePath + "Sound/Durations.tsv");
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

float WordKickerScene::getDuration(string name)
{
	if (_duration.count(name))
		return _duration[name];

	return 2.5;
}




END_NS_WORDKICKER;
