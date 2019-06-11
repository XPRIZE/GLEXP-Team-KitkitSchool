#include "WordWindowScene.h"
#include "Common/Basic/SoundEffect.h"
#include <string>
#include <vector>
#include <numeric>
#include <algorithm>
#include <Managers/VoiceMoldManager.h>
#include "ui/CocosGUI.h"
#include "Managers/LanguageManager.hpp"
#include "Managers/UserManager.hpp"
#include "Managers/GameSoundManager.h"
#include "Managers/StrictLogManager.h"
#include "Utils/TodoUtil.h"
#include "Common/Controls/TodoSchoolBackButton.hpp"
#include "Common/Controls/CompletePopup.hpp"
#include "CCAppController.hpp"

using namespace cocos2d::ui;
using namespace std;
string x;

namespace WordWindowSceneSpace
{
    const char* defaultFont = "fonts/chanakya.ttf";
    
    const string resourcePath = "wordwindow/";
    
    const Size gameSize = Size(2560, 1800);
       
    SoundEffect pickEffect() { return SoundEffect("Common/Sounds/Effect/SFX_Wood_SlideOut.m4a"); }
    SoundEffect snapEffect() { return SoundEffect("Common/Sounds/Effect/SFX_Wood_Correct.m4a"); }
    SoundEffect solveEffect() { return SoundEffect("Common/Sounds/Effect/SFX_Counting_Win.m4a"); }

    
    const string fontName = "fonts/mukta-bold.ttf";
	const string boldFontName = "fonts/andikanewbasic-b.ttf";
    const bool gameDebug = false;    
}

using namespace WordWindowSceneSpace;
WordWindowLevelStruct::WordWindowLevelStruct()
{
	m_languageTag = "";
	m_level = 0;
	m_worksheet = 0;
	m_problemNo = 0;
	m_sequenceType = 0;
	//m_soundFilename = "";
	m_text1="";
	m_text = "";
	for (int i = 0; i < 4; i++)
	{
		m_exampleNumber[i] = "";
		m_exampleWord[i] = "";
		m_objectValue[i] = 0;
		m_objectSource[i] = "";
	}
	m_rightFormula = "";
	m_rightFormulaShadow = "";
	m_rightAnswer = 0;
}

WordWindowRightFormulaText::WordWindowRightFormulaText()
{
	m_isBold = false;
	m_text = "";
}

int WordWindowLevelStruct::getTotalObjectCount()
{
	int totalCount = 0;

	for (int i = 0; i < 4; i++)
	{
		totalCount += m_objectValue[i];
	}

	return totalCount;
}

int WordWindowLevelStruct::getSubtractionResult()
{
	int totalCount = m_objectValue[0];

	for (int i = 1; i < 4; i++)
	{
		totalCount -= m_objectValue[i];
	}

	return totalCount;
}

int WordWindowLevelStruct::getLastObjectCountIndex()
{
	for (int i = 0; i < 4; i++)
	{
		if (m_objectValue[i] == 0)
		{
			return i - 1;
		}
	}

	return 0;
}

WordWindowScene::WordWindowScene()
{
	// 설정 변수들
	m_totalAnimationTime = 7.f;
	m_boardFlipDelayPerUnit = 0.05f;
	m_boardShakeDelayPerUnit = 0.05f;
	m_sequenceBeforeTerm = 1.f;
	m_catDownTime = 0.22f;
	m_catUpTime = 0.15f;
	m_stoneIntervalDelay = 1.f;
	m_stoneSimpleDelay = 0.5f;
	m_stoneDelay = 0.6f;
	m_equationCardFlipTime = 0.1f;
	m_backBoardDelay = 2.0f;
	m_otherCatRandom = 20;
	m_rightEffectTime = 1.5f;
	m_shiningParticleInterval = 700.f;
	m_rightFormulaEffectTime = 1.5f;
	
	m_currentLevel = 0;
	m_currentPuzzleIndex = 0;
	m_noTouchTime = 0.f;

	_gameNode = nullptr;

	resetSequence();
	m_pNodeSequence = nullptr;

	m_pProgressBar = nullptr;
	m_pSpriteBackground = nullptr;
	m_pSpriteBoard = nullptr;
	m_pButtonBoard = nullptr;
	m_pButtonSound = nullptr;
	m_pLabelQuestion = nullptr;
	for (int i = 0; i < 4; i++)
	{
		m_pButtonExample[i] = nullptr;
		m_pLabelExampleTop[i] = nullptr;
		m_pLabelExampleBottom[i] = nullptr;
	}
	m_isFirstDisableExamples = false;
	m_firstDisableExamplesTime = 0.f;

	m_soundDuraton = 0.f;
	m_soundQuestionID = 0;

	m_wrongAnswer = 0;
	m_isRightAnswer = false;
	m_pNodeRightFormula = nullptr;
	m_pRichText = nullptr;
	m_pRichTextShadow = nullptr;

	m_soundEffectPutObjectRow = SoundEffect("games/wordwindow/sounds/put_object_row.m4a");

	m_isTouchBoard = false;
	m_isTouchAnswer = false;
	m_touchAnswerIndex = 0;
}

WordWindowScene::~WordWindowScene()
{

}

Scene* WordWindowScene::createScene(string levelID)
{
    auto level = TodoUtil::stoi(levelID);
    auto scene = Scene::create();
    auto layer = WordWindowScene::create();
    layer->setLevel(TodoUtil::stoi(levelID));
    scene->addChild(layer);
    return scene;
}

bool WordWindowScene::init()
{
    if (!Layer::init())
    {
        return false;
    }
    
	// 미리 리소스 로딩
	preloads();

	auto winSize = getContentSize();

	// 배경 추가
	createBackground();

	// 게임 노드 추가
	_gameNode = Node::create();
	_gameNode->setContentSize(gameSize);
	float gameScale = MIN(winSize.width / gameSize.width, winSize.height / gameSize.height);
	_gameNode->setScale(gameScale);
	_gameNode->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	_gameNode->setPosition(Vec2(winSize.width / 2.f, winSize.height / 2.f));
	addChild(_gameNode);

	// 백버튼 추가
	auto backButton = TodoSchoolBackButton::create();
	backButton->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
	backButton->setPosition(Vec2(25, winSize.height - 25));
	backButton->onBack = [this](){ 
		stopSoundQuestion(); 
	};
	addChild(backButton);

	// 프로그레스바 추가
	m_pProgressBar = ProgressIndicator::create();
	m_pProgressBar->setPosition(Vec2(winSize.width / 2, winSize.height - m_pProgressBar->getContentSize().height));
	addChild(m_pProgressBar);

	// 보드 추가
	createBoard();

	// 디버거 모드에서 skip 버튼 눌렀을 경우 다음 문제로 넘어가게 처리
	if (UserManager::getInstance()->isDebugMode())
	{
		auto skip = Button::create();
		skip->setTitleFontSize(100);
		skip->setTitleText("Skip");
		skip->setAnchorPoint(Vec2::ANCHOR_TOP_RIGHT);
		skip->setPosition(Vec2(winSize.width - 25, winSize.height - 25));
		addChild(skip);
		skip->addTouchEventListener([this](Ref*, Widget::TouchEventType e)
		{
			if (e == Widget::TouchEventType::ENDED)
			{
				// 문제 통과
				onAnswer(m_curLevelStruct.m_rightAnswer);
			}
		});
	}

	// 사운드 시간 쉬트 로드
	loadDurationsheet();

	// update
	scheduleUpdate();

	return true;
}
//speech
//void WordWindowScene::speech1(string input) {
  //  if (LanguageManager::getInstance()->isEnglish()) {
     //   if (!_ttsAlert && CC_TARGET_PLATFORM == CC_PLATFORM_MAC) {
          //  _ttsAlert = true;
          //  NativeAlert::show("TTS engine is not supported in mac version", "", "OK");
       // } else {
            //VoiceMoldManager::shared()->speak(input);

      //  return 0.5f;
 //   } else {
      //  return speechWithFiles(input);
 //   }

//}
//float f = WordWindowScene::speech1(x);


void WordWindowScene::createBackground()
{
	//CCLOG("WordWindowScene::createBackground");

	auto winSize = getContentSize();

	m_pSpriteBackground = Sprite::create(resourcePath + "wordwindow_background.png");
	if (m_pSpriteBackground != NULL)
	{
		auto bgSize = m_pSpriteBackground->getContentSize();
		float bgScale = MAX(winSize.width / bgSize.width, winSize.height / bgSize.height);
		m_pSpriteBackground->setScale(bgScale);
		m_pSpriteBackground->setPosition(winSize / 2);
		addChild(m_pSpriteBackground);
	}
}

void WordWindowScene::preloads()
{
	//CCLOG("WordWindowScene::preloads");

	pickEffect().preload();
	snapEffect().preload();
	solveEffect().preload();
	SoundEffect("Common/Sounds/Effect/blockmiss.m4a").preload();

	Director::getInstance()->getTextureCache()->addImage(resourcePath + "wordwindow_frame_question_0.png");
	Director::getInstance()->getTextureCache()->addImage(resourcePath + "wordwindow_frame_question_1.png");
	Director::getInstance()->getTextureCache()->addImage(resourcePath + "wordwindow_frame_question_2.png");
	Director::getInstance()->getTextureCache()->addImage(resourcePath + "wordwindow_frame_question_3.png");
	Director::getInstance()->getTextureCache()->addImage(resourcePath + "wordwindow_frame_question_4.png");
	Director::getInstance()->getTextureCache()->addImage(resourcePath + "wordwindow_frame_question_5.png");
	Director::getInstance()->getTextureCache()->addImage(resourcePath + "wordwindow_frame_question_6.png");
	Director::getInstance()->getTextureCache()->addImage(resourcePath + "wordwindow_frame_question_7.png");
	Director::getInstance()->getTextureCache()->addImage(resourcePath + "wordwindow_frame_squence_0.png");
	Director::getInstance()->getTextureCache()->addImage(resourcePath + "wordwindow_frame_squence_1.png");
	Director::getInstance()->getTextureCache()->addImage(resourcePath + "wordwindow_frame_squence_2.png");
	Director::getInstance()->getTextureCache()->addImage(resourcePath + "wordwindow_frame_squence_3.png");
	Director::getInstance()->getTextureCache()->addImage(resourcePath + "wordwindow_frame_squence_4.png");
	Director::getInstance()->getTextureCache()->addImage(resourcePath + "wordwindow_frame_squence_5.png");
}

void WordWindowScene::onEnter()
{
	//CCLOG("WordWindowScene::onEnter");

    Layer::onEnter();
}

void WordWindowScene::onEnterTransitionDidFinish()
{
	//CCLOG("WordWindowScene::onEnterTransitionDidFinish");

    onStart();
}

void WordWindowScene::resetTouch()
{
	m_isTouchBoard = false;
	m_isTouchAnswer = false;
	m_touchAnswerIndex = 0;
}

void WordWindowScene::update(float dt)
{
	// 아무런 터치가 없는 시간 계산
	updateNoTouchTime(dt);

	// 사운드 남은 시간 계산
	updateSound(dt);

	updateFirstDisableExamples(dt);

	if (m_isTouchAnswer == true)
	{
		onAnswer(m_touchAnswerIndex);
		resetTouch();
	}
	else if (m_isTouchBoard == true)
	{
		onTouchedBoard();
		resetTouch();
	}
}

void WordWindowScene::updateSound(float dt)
{
	if (m_soundDuraton > 0.f)
	{
		m_soundDuraton -= dt;
		if (m_soundDuraton <= 0.f)
		{
			m_soundDuraton = 0.f;

			enableSoundButton();
		}
	}
}

void WordWindowScene::updateNoTouchTime(float dt)
{
	if (m_isHintSequence == true || m_isRightAnswer == true)
	{
		return;
	}

	m_noTouchTime += dt;
	if (m_noTouchTime >= 10.f)
	{
		playBoardShake();
		m_noTouchTime = 0.f;
	}
}

void WordWindowScene::onStart()
{
	//CCLOG("WordWindowScene::onStart");

     loadData(m_currentLevel);
     m_currentPuzzleIndex = 0;      
	 m_pProgressBar->setMax(m_vecPuzzles.size());
     showPuzzle(m_currentPuzzleIndex);
}

void WordWindowScene::showPuzzle(int index)
{
	//CCLOG("[WordWindowScene::showPuzzle] index : %d", index);

	m_pProgressBar->setCurrent(index + 1, false);
	createPuzzle(index);
}

bool WordWindowScene::isFinishedLevel()
{
	if (m_currentPuzzleIndex + 1 == (int)m_vecPuzzles.size())
	{
		return true;
	}

	return false;
}

void WordWindowScene::onFinishedLevel(float dt)
{
	//CCLOG("[WordWindowScene::onFinishLevel]");

	// 원래 속도로 리셋
	resetGameSpeed();

	onPuzzleComplete();
}

void WordWindowScene::onPuzzleComplete()
{
	//CCLOG("[WordWindowScene::onPuzzleComplete]");

	auto e = m_vecPuzzles[m_currentPuzzleIndex];

	m_pProgressBar->setCurrent(m_currentPuzzleIndex + 1, true);
	m_currentPuzzleIndex++;

	auto winSize = getContentSize();

	if (m_currentPuzzleIndex >= (int)m_vecPuzzles.size())
	{
		auto CP = CompletePopup::create();
		CP->show(1.5f, [this]
		{
			CCAppController::sharedAppController()->handleGameComplete(1);
		});
	}
	else
	{
		auto seq = Sequence::create(
			DelayTime::create(1.f),
			CallFunc::create([this](){ showPuzzle(m_currentPuzzleIndex); }),
			nullptr);
		this->runAction(seq);
	}
}

void WordWindowScene::setLevel(int level)
{
    m_currentLevel = level;
}

void WordWindowScene::createPuzzle(int index)
{
	m_curLevelStruct = m_vecPuzzles[index];

	// 모두 해제
	resetPuzzle();
	releaseSoundButton();
	releaseQuestion();
	releaseExamples();
	releaseBoard();

	_gameNode->removeAllChildren();

	// 칠판 추가
	createBoard();

	// 사운드 버튼 추가
	createSoundButton();

	// 문제 추가
	createQuestion();

	// 예제 추가
	createExamples();
	startFirstDisableExamples();

	// 문장 사운드 플레이
	//playSoundQuestion(m_curLevelStruct.m_soundFilename);
	speech1(m_curLevelStruct.m_text1);

	if (m_curLevelStruct.m_sequenceType == SEQUENCE_TYPE_EQUATION)
	{
		selectStone();
	}
}

void WordWindowScene::resetPuzzle()
{
	//CCLOG("[WordWindowScene::resetPuzzle]");

	m_isRightAnswer = false;
	resetSequence();
}

void WordWindowScene::loadData(int level)
{
	string P = "games/"+ resourcePath + "wordwindow_level.tsv";
	string S = cocos2d::FileUtils::getInstance()->getStringFromFile(P);
	auto data = TodoUtil::readTSV(S);
	auto Lang = LanguageManager::getInstance()->getCurrentLanguageTag();

	std::vector<WordWindowLevelStruct> vecDatas;
	std::set<int> setWorkSheets;
	
	int rowIndex = 0;
	while (rowIndex < (int)data.size())
	{
		auto row = data[rowIndex++];

		WordWindowLevelStruct s;

		s.m_languageTag = row[0];
		s.m_level = TodoUtil::stoi(row[1]);
		if (s.m_level != level)
		{
			continue;
		}

		s.m_worksheet = TodoUtil::stoi(row[2]);
		setWorkSheets.insert(s.m_worksheet);

		s.m_problemNo = TodoUtil::stoi(row[3]);
		s.m_sequenceType = TodoUtil::stoi(row[4]);
		// x = row[5];

       //VoiceMoldManager::shared()->speak(x);
		//s.m_soundFilename = row[5];
		s.m_text1=row[5];
		s.m_text = row[6];
		for (int i = 0; i < 4; i++)
		{
			s.m_exampleNumber[i] = row[7 + i];
		}
		for (int i = 0; i < 4; i++)
		{
			s.m_exampleWord[i] = row[11 + i];
		}
		s.m_rightFormula = row[15];
		s.m_rightAnswer = TodoUtil::stoi(row[16]);
		for (int i = 0; i < 4; i++)
		{
			s.m_objectValue[i] = TodoUtil::stoi(row[17 + i]);
		}

		s.m_equationValueA = TodoUtil::stoi(row[21]);
		s.m_equationValueB = TodoUtil::stoi(row[22]);
		s.m_equationValueSign = row[23];
		s.m_equationUnknown = row[24];

		for (int i = 0; i < 4; i++)
		{
			s.m_objectSource[i] = row[25 + i];
		}

		vecDatas.push_back(s);
	}

	int workSheetCount = setWorkSheets.size();
	int selectedWorkSheetIndex = cocos2d::random() % workSheetCount;
	int selectedWorkSheet = 0;
	auto it = setWorkSheets.find(selectedWorkSheetIndex + 1);
	if (it != setWorkSheets.end())
	{
		selectedWorkSheet = *it;
	}

	for (int i = 0; i < (int)vecDatas.size(); i++)
	{
		if (UserManager::getInstance()->isWorksheetTestMode() == true)
		{
			m_vecPuzzles.push_back(vecDatas[i]);
		}
		else
		{
			if (selectedWorkSheet == vecDatas[i].m_worksheet)
			{
				m_vecPuzzles.push_back(vecDatas[i]);
			}
		}
	}
}

//void WordWindowScene::playSoundQuestion(string name)
//{
//	//CCLOG("[WordWindowScene::playSound] %s", name.c_str());
//	if (name.empty() == true)
//	{
//		return;
//	}
//
//    string path = "games/wordwindow/sound/" + name;
//
//	GameSoundManager::getInstance()->playEffectSoundVoiceOnly(path);
//
//	disableSoundButton();
//
//	m_soundDuraton = getDuration(name);
//}
void WordWindowScene::speech1(string name)
{
	if(name.empty()==true)
	{
		return;
	}
//	string path = "games/wordwindow/sound/" + name;
	//void WordWindowScene::speech1(string input) {
//		 if (LanguageManager::getInstance()->isEnglish()) {
//		 if (!_ttsAlert && CC_TARGET_PLATFORM == CC_PLATFORM_MAC) {
//		  _ttsAlert = true;
//		  NativeAlert::show("TTS engine is not supported in mac version", "", "OK");
		// } else {
		VoiceMoldManager::shared()->speak(name,"hi-IN");
		disableSoundButton();
		m_soundDuraton= getDuration(name);
	}
	//  return 0.5f;
	//   } else {
	//  return speechWithFiles(input);
	//   }

//}
//float f = WordWindowScene::speech1(x);




void WordWindowScene::stopSoundQuestion()
{
	m_soundDuraton = 0.f;
	enableSoundButton();

	GameSoundManager::getInstance()->stopBGM();
}

void WordWindowScene::loadDurationsheet()
{
	if (m_mapSoundDuration.size() != 0)
	{
		return;
	}

    std::string rawString = cocos2d::FileUtils::getInstance()->getStringFromFile("wordwindow/sound/durations.tsv");
    auto data = TodoUtil::readTSV(rawString);

    for (auto row : data)
	{
		if (TodoUtil::trim(row[0]).size() <= 0)
		{
			continue;
		}
		if (row.size() == 1)
		{
			continue;
		}
        auto row1Vec = TodoUtil::split(row[1], ':');
        TodoUtil::replaceAll(row1Vec[2], ".", "");
        auto rowDuration = (float)TodoUtil::stoi(row1Vec[2])/100;
        m_mapSoundDuration[row[0]] = rowDuration;
    }
}

float WordWindowScene::getDuration(string name) 
{
	if (m_mapSoundDuration.count(name))
	{
		return m_mapSoundDuration[name];
	}

    return 2.5;
}

void WordWindowScene::createBoard()
{
	//CCLOG("WordWindowScene::createBoard");

	releaseBoard();

	Size winSize = getContentSize();

	// 칠판 추가
	m_pSpriteBoard = Sprite::create(resourcePath + "wordwindow_frame_question_1.png");
	if (m_pSpriteBoard != nullptr)
	{
		//m_pSpriteBoard->setPosition(Vec2(1280.f, winSize.height - 238.f - (984.f / 2.f)));
		m_pSpriteBoard->setPosition(Vec2(1280.f, 1070.f));
		_gameNode->addChild(m_pSpriteBoard);
	};

	// 투명 버튼 등록
	m_pButtonBoard = Button::create(resourcePath + "wordwindow_frame_question_1.png", resourcePath + "wordwindow_frame_question_1.png");
	if (m_pButtonBoard != nullptr)
	{
		m_pButtonBoard->getRendererNormal()->setVisible(false);
		m_pButtonBoard->getRendererClicked()->setVisible(false);
		m_pButtonBoard->setPosition(m_pSpriteBoard->getPosition());
		_gameNode->addChild(m_pButtonBoard);
		m_pButtonBoard->addTouchEventListener([this](Ref*, Widget::TouchEventType e)
		{
			if (e == Widget::TouchEventType::ENDED)
			{
				m_noTouchTime = 0.f;
				m_isTouchBoard = true;
			}
		});
	}
}

void WordWindowScene::releaseBoard()
{
	//CCLOG("WordWindowScene::releaseBoard");

	if (m_pSpriteBoard != nullptr)
	{
		_gameNode->removeChild(m_pSpriteBoard);
		m_pSpriteBoard = nullptr;
	}

	if (m_pButtonBoard != nullptr)
	{
		_gameNode->removeChild(m_pButtonBoard);
		m_pButtonBoard = nullptr;
	}
}

void WordWindowScene::onTouchedBoard()
{
	//CCLOG("[WordWindowScene::onTouchedBoard]");

	if (m_isRightAnswer == true || m_isHintSequence == true)
	{
		return;
	}

	// 질문효과음 정지
	stopSoundQuestion();

	// 컨텐츠를 가리고
	hideBoardContents();

	// 예제 비활성화
	endFirstDisableExamples();
	showExamplesLabel(false);
	disableExamples();
	transparentExamples();

	// 오답인 경우 disable 이미지 변경
	if (m_wrongAnswer > 0)
	{
		int index = m_wrongAnswer - 1;
		if (m_pButtonExample[index] != nullptr)
		{
			m_pButtonExample[index]->loadTextureDisabled(resourcePath + "wordwindow-btn-answer-wrong.png");
		}
	}

	// 보드판을 플립시킨다.
	playBoardFlip1();

	// 약식시퀀스 실행
	resetSequence();
	m_isHintSequence = true;
	m_isSimpleSequence = true;
}

void WordWindowScene::playBoardShake()
{
	//CCLOG("[WordWindowScene::playBoardShake]");

	m_pSpriteBoard->stopAllActions();

	auto animation = Animation::create();
	animation->addSpriteFrameWithFile(resourcePath + "wordwindow_frame_question_0.png");
	animation->addSpriteFrameWithFile(resourcePath + "wordwindow_frame_question_5.png");
	animation->addSpriteFrameWithFile(resourcePath + "wordwindow_frame_question_0.png");
	animation->addSpriteFrameWithFile(resourcePath + "wordwindow_frame_question_6.png");
	animation->addSpriteFrameWithFile(resourcePath + "wordwindow_frame_question_7.png");
	animation->addSpriteFrameWithFile(resourcePath + "wordwindow_frame_question_6.png");
	animation->addSpriteFrameWithFile(resourcePath + "wordwindow_frame_question_1.png");
	animation->setDelayPerUnit(m_boardShakeDelayPerUnit);
	animation->setRestoreOriginalFrame(false);

	auto action = Animate::create(animation);
	m_pSpriteBoard->runAction(action);

	// 효과음
	playSoundBoardShake();
}

void WordWindowScene::playBoardFlip1()
{
	//CCLOG("WordWindowScene::playBoardFlip1");

	m_pSpriteBoard->stopAllActions();

	auto animation = Animation::create();
	animation->addSpriteFrameWithFile(resourcePath + "wordwindow_frame_question_2.png");
	animation->addSpriteFrameWithFile(resourcePath + "wordwindow_frame_question_3.png");
	animation->addSpriteFrameWithFile(resourcePath + "wordwindow_frame_question_4.png");
	animation->addSpriteFrameWithFile(resourcePath + "wordwindow_frame_squence_5.png");
	animation->addSpriteFrameWithFile(resourcePath + "wordwindow_frame_squence_4.png");
	animation->addSpriteFrameWithFile(resourcePath + "wordwindow_frame_squence_3.png");
	animation->addSpriteFrameWithFile(resourcePath + "wordwindow_frame_squence_2.png");
	animation->addSpriteFrameWithFile(resourcePath + "wordwindow_frame_squence_1.png");
	animation->addSpriteFrameWithFile(resourcePath + "wordwindow_frame_squence_0.png");
	animation->addSpriteFrameWithFile(resourcePath + "wordwindow_frame_squence_1.png");
	animation->setDelayPerUnit(m_boardFlipDelayPerUnit);
	animation->setRestoreOriginalFrame(false);

	auto action = Animate::create(animation);
	auto sequence = Sequence::create(action, CallFunc::create(CC_CALLBACK_0(WordWindowScene::onEndPlayBoardFlip1, this)), nullptr);
	m_pSpriteBoard->runAction(sequence);

	// 효과음
	playSoundBoardFlip();
}

void WordWindowScene::onEndPlayBoardFlip1()
{
	//CCLOG("[WordWindowScene::OnEndPlayBoardFlip1]");

	if (m_isSimpleSequence == true)
	{
		playHintSimpleSequence();
	}
	else
	{
		// 정답 수식 표시
		createRightFormula();

		// 시퀀스 진행
		playHintSequence();
	}
}

void WordWindowScene::calculateStartPosY()
{
	int totalCount = 0;

	if (m_curLevelStruct.m_sequenceType == SEQUENCE_TYPE_ADDITION)
	{
		totalCount = m_curLevelStruct.getTotalObjectCount();
	}
	else if (m_curLevelStruct.m_sequenceType == SEQUENCE_TYPE_SUBTRACTION)
	{
		totalCount = m_curLevelStruct.m_objectValue[0];
	}

	if (totalCount <= 10)
	{
		m_sequenceStartY = 578.f + 495.f + (174.f / 2.f) - 90.f;
	}
	else if (totalCount <= 20)
	{
		m_sequenceStartY = 578.f + 495.f + (174.f / 2.f);
	}
	else if (totalCount <= 30)
	{
		m_sequenceStartY = 578.f + 495.f + (174.f / 2.f) + 90.f;
	}
	else
	{
		m_sequenceStartY = 578.f + 495.f + (174.f / 2.f) + 180.f;
	}
}

bool WordWindowScene::isPackAddition(int objectCount)
{
	// 10개 보다 작으면 일단 불가능
	if (objectCount < 10)
	{
		return false;
	}

	// 현재 라인 남은 갯수
	int remainderAtCurLine = (10 - (m_accumulateCount % 10)) % 10;

	// 현재 라인 남은 갯수를 뺀 나머지 갯수
	int remainder = objectCount - remainderAtCurLine;

	// 10개 묶음 갯수
	int tenPackCount = remainder / 10;

	// 10개 묶음이 한개 이상이라면 팩 가능
	if (tenPackCount > 0)
	{
		return true;
	}

	return false;
}

bool WordWindowScene::isPackSubtraction(int index, int objectCount)
{
	// 10개 보다 작으면 일단 불가능
	if (objectCount < 10)
	{
		return false;
	}

	// 현재 라인 남은 갯수
	int remainderAtCurLine = (m_accumulateCount % 10);

	// 현재 라인 남은 갯수를 뺀 나머지 갯수
	int remainder = objectCount - remainderAtCurLine;

	// 10개 묶음 갯수
	int tenPackCount = remainder / 10;

	// 10개 묶음이 한개 이상이라면 팩 가능
	if (tenPackCount > 0)
	{
		return true;
	}

	return false;
}

void WordWindowScene::playHintSimpleSequence()
{
	//CCLOG("[WordWindowScene::playHintSimpleSequence]");

	m_pNodeSequence = Node::create();
	_gameNode->addChild(m_pNodeSequence);

	// 덧셈 약식 시퀀스
	if (m_curLevelStruct.m_sequenceType == SEQUENCE_TYPE_ADDITION)
	{
		playHintSimpleSequenceAddition();
	}
	// 뺄셈 약식 시퀀스
	else if (m_curLevelStruct.m_sequenceType == SEQUENCE_TYPE_SUBTRACTION)
	{
		playHintSimpleSequenceSubtraction();
	}
	// 곱셈 약식 시퀀스
	else if (m_curLevelStruct.m_sequenceType == SEQUENCE_TYPE_MULTIPLY)
	{
		playHintSimpleSequenceMultiply();
	}
	// 비교 약식 시퀀스
	else if (m_curLevelStruct.m_sequenceType == SEQUENCE_TYPE_COMPARISION)
	{
		playHintSimpleSequenceComparison();
	}
	// 포함 약식 시퀀스
	else if (m_curLevelStruct.m_sequenceType == SEQUENCE_TYPE_INCLUSION)
	{
		playHintSimpleSequenceInclusion();
	}
	// 방정식 약식 시퀀스
	else if (m_curLevelStruct.m_sequenceType == SEQUENCE_TYPE_EQUATION)
	{
		playHintSimpleSequenceEquation();
	}

	// 대기 시간 추가
	m_accumulateDelay += m_backBoardDelay;

	// 보드 플립 연출
	playBoardFlip2(m_accumulateDelay);
}

void WordWindowScene::playHintSimpleSequenceAddition()
{
	//CCLOG("[WordWindowScene::playHintSimpleSequenceAddition]");

	for (int i = 0; i < 4; i++)
	{
		// 첫수가 10보다 크다면 10묶음을 한번에 놓고 나머지를 놓는다. (상단에 10묶음을 놓는다.)
		if (m_curLevelStruct.m_sequenceType != SEQUENCE_TYPE_INCLUSION && 
			i == 0 && m_curLevelStruct.m_objectValue[i] >= 10)
		{
			// 10개 묶음(상단)
			Node* pNodeStonePack = Node::create();
			m_pNodeSequence->addChild(pNodeStonePack);

			for (int k = 0; k < 10; k++)
			{
				// 돌 추가
				Sprite* pSpriteStone = Sprite::create(resourcePath + m_curLevelStruct.m_objectSource[i]);
				Vec2 pos;
				pos.x = 166.f + 217.f + (174.f / 2.f) + (k * 180.f);
				pos.y = 578.f + 495.f + (174.f / 2.f);
				pSpriteStone->setPosition(pos);
				pNodeStonePack->addChild(pSpriteStone);
			}

			// 돌 액션
			Sequence* sequence = Sequence::create(
				DelayTime::create(m_accumulateDelay),
				CallFunc::create([this, pNodeStonePack]()
			{
				pNodeStonePack->setVisible(true);

				// 효과음
				playSoundAddObjectPack();
			}),
				nullptr);
			pNodeStonePack->runAction(sequence);

			m_accumulateCount += 10;
			m_accumulateDelay += m_stoneIntervalDelay;

			// 나머지가 있는 경우 나머지를 놓는 시퀀스 추가
			int remainder = m_curLevelStruct.m_objectValue[i] - 10;
			if (remainder > 0)
			{
				for (int kk = 0; kk < remainder; kk++)
				{
					Sprite* pSpriteStone = Sprite::create(resourcePath + m_curLevelStruct.m_objectSource[i]);
					Vec2 pos;
					pos.x = 166.f + 217.f + (174.f / 2.f) + (kk * 180.f);
					pos.y = 578.f + 495.f + (174.f / 2.f) - 180.f;
					pSpriteStone->setPosition(pos);
					pSpriteStone->setVisible(false);
					m_pNodeSequence->addChild(pSpriteStone);

					Sequence* sequence = Sequence::create(
						DelayTime::create(m_accumulateDelay),
						CallFunc::create([this, pSpriteStone]()
					{ 
						pSpriteStone->setVisible(true); 

						// 효과음
						playSoundAddObject();
					}),
						nullptr);
					pSpriteStone->runAction(sequence);

					m_accumulateDelay += m_stoneSimpleDelay;
				}

				m_accumulateCount += remainder;
			}
		}
		else
		{
			// 하단 10묶음 가능한지 판별
			bool isBottomPack = false;

			// 마지막 수이고 10보다 크면서 총합이 20인 경우만 가능
			if (m_curLevelStruct.m_sequenceType != SEQUENCE_TYPE_INCLUSION && 
				m_curLevelStruct.getLastObjectCountIndex() == i && m_curLevelStruct.m_objectValue[i] >= 10 && m_curLevelStruct.getTotalObjectCount() == 20)
			{
				isBottomPack = true;
			}

			if (isBottomPack == true)
			{
				// 하단 10개 뺀 나머지 배치
				int remainder = m_curLevelStruct.m_objectValue[i] - 10;
				if (remainder > 0)
				{
					for (int kk = 0; kk < remainder; kk++)
					{
						int index = m_accumulateCount + kk;

						Sprite* pSpriteStone = Sprite::create(resourcePath + m_curLevelStruct.m_objectSource[i]);
						Vec2 pos;
						pos.x = 166.f + 217.f + (174.f / 2.f) + ((index % 10) * 180.f);
						pos.y = 578.f + 495.f + (174.f / 2.f) - ((index / 10) * 180.f);
						if (m_curLevelStruct.getTotalObjectCount() <= 10)
						{
							pos.y -= 90.f;
						}
						pSpriteStone->setPosition(pos);
						pSpriteStone->setVisible(false);
						m_pNodeSequence->addChild(pSpriteStone);

						Sequence* sequence = Sequence::create(
							DelayTime::create(m_accumulateDelay),
							CallFunc::create([this, pSpriteStone]()
						{
							pSpriteStone->setVisible(true); 

							// 효과음
							playSoundAddObject();
						}),
							nullptr);
						pSpriteStone->runAction(sequence);

						m_accumulateDelay += m_stoneSimpleDelay;
					}

					m_accumulateCount += remainder;
				}

				// 하단 10개 묶음
				Node* pNodeStonePack = Node::create();
				pNodeStonePack->setVisible(false);
				m_pNodeSequence->addChild(pNodeStonePack);
				for (int k = 0; k < 10; k++)
				{
					Sprite* pSpriteStone = Sprite::create(resourcePath + m_curLevelStruct.m_objectSource[i]);
					Vec2 pos;
					pos.x = 166.f + 217.f + (174.f / 2.f) + (k * 180.f);
					pos.y = 578.f + 495.f + (174.f / 2.f) - 180.f;
					pSpriteStone->setPosition(pos);
					pNodeStonePack->addChild(pSpriteStone);
				}

				Sequence* sequence = Sequence::create(
					DelayTime::create(m_accumulateDelay),
					CallFunc::create([this, pNodeStonePack]()
				{ 
					pNodeStonePack->setVisible(true); 

					// 효과음
					playSoundAddObjectPack();
				}),
					nullptr);
				pNodeStonePack->runAction(sequence);

				m_accumulateCount += 10;
			}
			else
			{
				for (int k = 0; k < m_curLevelStruct.m_objectValue[i]; k++)
				{
					int index = m_accumulateCount + k;

					Sprite* pSpriteStone = Sprite::create(resourcePath + m_curLevelStruct.m_objectSource[i]);
					Vec2 pos;
					pos.x = 166.f + 217.f + (174.f / 2.f) + ((index % 10) * 180.f);
					pos.y = 578.f + 495.f + (174.f / 2.f) - ((index / 10) * 180.f);
					if (m_curLevelStruct.getTotalObjectCount() <= 10)
					{
						pos.y -= 90.f;
					}
					pSpriteStone->setPosition(pos);
					pSpriteStone->setVisible(false);
					m_pNodeSequence->addChild(pSpriteStone);

					// 연출
					Sequence* sequence = Sequence::create(
						DelayTime::create(m_accumulateDelay),
						CallFunc::create([this, pSpriteStone]()
					{
						pSpriteStone->setVisible(true);

						// 효과음
						playSoundAddObject();
					}),
						nullptr);
					pSpriteStone->runAction(sequence);

					m_accumulateDelay += m_stoneSimpleDelay;
				}
			}

			m_accumulateCount += m_curLevelStruct.m_objectValue[i];
		}

		m_accumulateDelay += m_stoneDelay;
	}
}

void WordWindowScene::playHintSimpleSequenceSubtraction()
{
	// 뺄셈은 첫수는 덧셈처럼 놓는 에니메이션을 진행한다.
	// 첫수만 검사, 10보다 크다면 10묶음을 한번에 놓고 나머지를 놓는다.
	if (m_curLevelStruct.m_objectValue[0] == 20)
	{
		// 10개 묶음 위에 한줄
		Node* pNodeStonePack = Node::create();
		m_pNodeSequence->addChild(pNodeStonePack);
		for (int k = 0; k < 10; k++)
		{
			Sprite* pSpriteStone = Sprite::create(resourcePath + m_curLevelStruct.m_objectSource[0]);
			Vec2 pos;
			pos.x = 166.f + 217.f + (174.f / 2.f) + (k * 180.f);
			pos.y = 578.f + 495.f + (174.f / 2.f);
			pSpriteStone->setPosition(pos);
			pNodeStonePack->addChild(pSpriteStone);

			m_pSpriteStone[k] = pSpriteStone;
		}

		// 연출
		Sequence* sequence = Sequence::create(
			DelayTime::create(m_accumulateDelay),
			CallFunc::create([this, pNodeStonePack]()
		{
			pNodeStonePack->setVisible(true); 

			// 효과음
			playSoundAddObjectPack();
		}),
			nullptr);
		pNodeStonePack->runAction(sequence);

		m_accumulateCount += 10;
		m_accumulateDelay += m_stoneIntervalDelay;

		// 10개 묶음 아래에 한줄
		pNodeStonePack = Node::create();
		pNodeStonePack->setVisible(false);
		m_pNodeSequence->addChild(pNodeStonePack);
		for (int k = 0; k < 10; k++)
		{
			Sprite* pSpriteStone = Sprite::create(resourcePath + m_curLevelStruct.m_objectSource[0]);
			Vec2 pos;
			pos.x = 166.f + 217.f + (174.f / 2.f) + (k * 180.f);
			pos.y = 578.f + 495.f + (174.f / 2.f) - 180.f;
			pSpriteStone->setPosition(pos);
			pNodeStonePack->addChild(pSpriteStone);

			m_pSpriteStone[m_accumulateCount + k] = pSpriteStone;
		}

		// 연출
		sequence = Sequence::create(
			DelayTime::create(m_accumulateDelay),
			CallFunc::create([this, pNodeStonePack]()
		{
			pNodeStonePack->setVisible(true);

			// 효과음
			playSoundAddObjectPack();
		}),
			nullptr);
		pNodeStonePack->runAction(sequence);

		m_accumulateCount += 10;
	}
	else if (m_curLevelStruct.m_objectValue[0] >= 10)
	{
		// 10개 묶음 위에 한줄
		Node* pNodeStonePack = Node::create();
		m_pNodeSequence->addChild(pNodeStonePack);
		for (int k = 0; k < 10; k++)
		{
			Sprite* pSpriteStone = Sprite::create(resourcePath + m_curLevelStruct.m_objectSource[0]);
			Vec2 pos;
			pos.x = 166.f + 217.f + (174.f / 2.f) + (k * 180.f);
			pos.y = 578.f + 495.f + (174.f / 2.f);
			pSpriteStone->setPosition(pos);
			pNodeStonePack->addChild(pSpriteStone);
			
			m_pSpriteStone[k] = pSpriteStone;
		}

		Sequence* sequence = Sequence::create(
			DelayTime::create(m_accumulateDelay),
			CallFunc::create([this, pNodeStonePack]()
		{
			pNodeStonePack->setVisible(true); 

			// 효과음
			playSoundAddObjectPack();
		}),
			nullptr);
		pNodeStonePack->runAction(sequence);

		m_accumulateCount += 10;
		m_accumulateDelay += m_stoneIntervalDelay;

		// 나머지가 있는 경우 나머지를 놓는 시퀀스 추가
		m_remainder = m_curLevelStruct.m_objectValue[0] - 10;
		if (m_remainder > 0)
		{
			for (int kk = 0; kk < m_remainder; kk++)
			{
				Sprite* pSpriteStone = Sprite::create(resourcePath + m_curLevelStruct.m_objectSource[0]);
				Vec2 pos;
				pos.x = 166.f + 217.f + (174.f / 2.f) + (kk * 180.f);
				pos.y = 578.f + 495.f + (174.f / 2.f) - 180.f;
				pSpriteStone->setPosition(pos);
				pSpriteStone->setVisible(false);
				m_pNodeSequence->addChild(pSpriteStone);

				Sequence* sequence = Sequence::create(
					DelayTime::create(m_accumulateDelay),
					CallFunc::create([this, pSpriteStone]()
				{
					pSpriteStone->setVisible(true);

					// 효과음
					playSoundAddObject();
				}),
					nullptr);
				pSpriteStone->runAction(sequence);

				m_accumulateDelay += m_stoneSimpleDelay;

				m_pSpriteStone[m_accumulateCount + kk] = pSpriteStone;
			}

			m_accumulateCount += m_remainder;
		}
	}
	else
	{
		for (int k = 0; k < m_curLevelStruct.m_objectValue[0]; k++)
		{
			Sprite* pSpriteStone = Sprite::create(resourcePath + m_curLevelStruct.m_objectSource[0]);
			Vec2 pos;
			pos.x = 166.f + 217.f + (174.f / 2.f) + (k * 180.f);
			pos.y = 578.f + 495.f + (174.f / 2.f) - 90.f;
			pSpriteStone->setPosition(pos);
			pSpriteStone->setVisible(false);
			m_pNodeSequence->addChild(pSpriteStone);
			Sequence* sequence = Sequence::create(
				DelayTime::create(m_accumulateDelay),
				CallFunc::create([this, pSpriteStone]()
			{ 
				pSpriteStone->setVisible(true); 

				// 효과음
				playSoundAddObject();
			}),
				nullptr);
			pSpriteStone->runAction(sequence);

			m_accumulateDelay += m_stoneSimpleDelay;

			m_pSpriteStone[k] = pSpriteStone;
		}

		m_accumulateCount += m_curLevelStruct.m_objectValue[0];
	}

	m_accumulateDelay += m_stoneDelay;

	// 다음 수 부터는 돌을 뺀다.
	for (int i = 1; i < 4; i++)
	{
		// 하단 10개 묶음 한번에 뺄 수 있는 지 검사
		bool canBottomPack = false;
		bool canTopPack = false;
		if (m_curLevelStruct.m_objectValue[i] >= 10 && m_accumulateCount == 20)
		{
			canBottomPack = true;
		}
		// 상단 10묶음 한번에 뺄 수 있는 지 검사
		else if (m_curLevelStruct.getLastObjectCountIndex() == i && m_curLevelStruct.m_objectValue[i] >= 10 && m_curLevelStruct.getSubtractionResult() == 0)
		{
			canTopPack = true;
		}

		if (canBottomPack == true || canTopPack == true)
		{
			if (canBottomPack == true)
			{
				// 10묶음 팩
				Node* pNodeStonePack = Node::create();
				pNodeStonePack->setVisible(false);
				m_pNodeSequence->addChild(pNodeStonePack);
				for (int k = 0; k < 10; k++)
				{
					Sprite* pSpriteEmpty = Sprite::create(resourcePath + "wordwindow-stone-empty.png");
					Vec2 pos;
					pos.x = 166.f + 217.f + (174.f / 2.f) + ((k % 10) * 180.f);
					pos.y = 578.f + 495.f + (174.f / 2.f) - 180.f;
					pSpriteEmpty->setPosition(pos);
					pNodeStonePack->addChild(pSpriteEmpty);
				}

				// 연출
				Sequence* sequence = Sequence::create(
					DelayTime::create(m_accumulateDelay),
					CallFunc::create([this, pNodeStonePack]()
				{
					pNodeStonePack->setVisible(true);
					for (int kk = 0; kk < 10; kk++)
					{
						if (m_pSpriteStone[10 + kk])
						{
							m_pSpriteStone[10 + kk]->setVisible(false);
						}
					}

					// 효과음
					playSoundSubObjectPack();
				}),
					nullptr);
				pNodeStonePack->runAction(sequence);

				m_accumulateDelay += m_stoneSimpleDelay;
				m_accumulateCount -= 10;

				int remainder = m_curLevelStruct.m_objectValue[i] - 10;
				for (int kk = 0; kk < remainder; kk++)
				{
					int index = m_accumulateCount - kk - 1;
					Sprite* pSpriteEmpty = Sprite::create(resourcePath + "wordwindow-stone-empty.png");
					Vec2 pos;
					pos.x = 166.f + 217.f + (174.f / 2.f) + ((index % 10) * 180.f);
					pos.y = 578.f + 495.f + (174.f / 2.f);
					pSpriteEmpty->setPosition(pos);
					pSpriteEmpty->setVisible(false);
					m_pNodeSequence->addChild(pSpriteEmpty);

					// 연출
					Sequence* sequence = Sequence::create(
						DelayTime::create(m_accumulateDelay),
						CallFunc::create([this, pSpriteEmpty, index]()
					{ 
						pSpriteEmpty->setVisible(true); 
						if (m_pSpriteStone[index])
						{
							m_pSpriteStone[index]->setVisible(false);
						}

						// 효과음
						playSoundSubObject();
					}),
						nullptr);
					pSpriteEmpty->runAction(sequence);

					m_accumulateDelay += m_stoneSimpleDelay;
				}

				m_accumulateCount -= remainder;
			}
			else if (canTopPack == true)
			{
				int remainder = m_curLevelStruct.m_objectValue[i] - 10;
				for (int kk = 0; kk < remainder; kk++)
				{
					int index = m_accumulateCount - kk - 1;
					Sprite* pSpriteEmpty = Sprite::create(resourcePath + "wordwindow-stone-empty.png");
					Vec2 pos;
					pos.x = 166.f + 217.f + (174.f / 2.f) + ((index % 10) * 180.f);
					pos.y = 578.f + 495.f + (174.f / 2.f) - 180.f;
					pSpriteEmpty->setPosition(pos);
					pSpriteEmpty->setVisible(false);
					m_pNodeSequence->addChild(pSpriteEmpty);

					// 연출
					Sequence* sequence = Sequence::create(
						DelayTime::create(m_accumulateDelay),
						CallFunc::create([this, pSpriteEmpty, index]()
					{ 
						pSpriteEmpty->setVisible(true); 
						if (m_pSpriteStone[index])
						{
							m_pSpriteStone[index]->setVisible(false);
						}

						// 효과음
						playSoundSubObject();
					}),
						nullptr);
					pSpriteEmpty->runAction(sequence);

					m_accumulateDelay += m_stoneSimpleDelay;
				}

				m_accumulateCount -= remainder;

				Node* pNodeStonePack = Node::create();
				pNodeStonePack->setVisible(false);
				m_pNodeSequence->addChild(pNodeStonePack);
				for (int k = 0; k < 10; k++)
				{
					Sprite* pSpriteEmpty = Sprite::create(resourcePath + "wordwindow-stone-empty.png");
					Vec2 pos;
					pos.x = 166.f + 217.f + (174.f / 2.f) + ((k % 10) * 180.f);
					pos.y = 578.f + 495.f + (174.f / 2.f);
					pSpriteEmpty->setPosition(pos);
					pNodeStonePack->addChild(pSpriteEmpty);
				}

				Sequence* sequence = Sequence::create(
					DelayTime::create(m_accumulateDelay),
					CallFunc::create([this, pNodeStonePack]()
				{
					pNodeStonePack->setVisible(true);
					for (int kk = 0; kk < 10; kk++)
					{
						if (m_pSpriteStone[kk])
						{
							m_pSpriteStone[kk]->setVisible(false);
						}
					}

					// 효과음
					playSoundSubObjectPack();
				}),
					nullptr);
				pNodeStonePack->runAction(sequence);
			}
		}
		else
		{
			for (int k = 0; k < m_curLevelStruct.m_objectValue[i]; k++)
			{
				int index = m_accumulateCount - k - 1;
				Sprite* pSpriteEmpty = Sprite::create(resourcePath + "wordwindow-stone-empty.png");
				Vec2 pos;
				pos.x = 166.f + 217.f + (174.f / 2.f) + ((index % 10) * 180.f);
				pos.y = 578.f + 495.f + (174.f / 2.f) - ((index / 10) * 180.f);
				if (m_curLevelStruct.m_objectValue[0] <= 10)
				{
					pos.y -= 90.f;
				}
				pSpriteEmpty->setPosition(pos);
				pSpriteEmpty->setVisible(false);
				m_pNodeSequence->addChild(pSpriteEmpty);

				Sequence* sequence = Sequence::create(
					DelayTime::create(m_accumulateDelay),
					CallFunc::create([this, pSpriteEmpty, index]()
				{ 
					pSpriteEmpty->setVisible(true);
					if (m_pSpriteStone[index])
					{
						m_pSpriteStone[index]->setVisible(false);
					}

					// 효과음
					playSoundSubObject();
				}),
					nullptr);
				pSpriteEmpty->runAction(sequence);

				m_accumulateDelay += m_stoneSimpleDelay;
			}

			m_accumulateCount -= m_curLevelStruct.m_objectValue[i];
		}

		m_accumulateDelay += m_stoneIntervalDelay;
	}
}

void WordWindowScene::playHintSimpleSequenceMultiply()
{
	//CCLOG("[WordWindowScene::playHintSimpleSequenceMultiply]");

	selectCatType();

	int a = m_curLevelStruct.m_objectValue[1];
	int b = m_curLevelStruct.m_objectValue[0];

	// 짝수인지
	bool isPairX = false;
	if ((a % 2) == 0)
	{
		isPairX = true;
	}
	bool isPairY = false;
	if ((b % 2) == 0)
	{
		isPairY = true;
	}

	int c = a / 2;
	int d = b / 2;
	float startX = 470.f;
	float startY = 1340.f + 25.f;

	// 시작 위치 계산
	startX += (5 - c) * 180.f;
	if (isPairX == false)
	{
		startX -= 90.f;
	}
	startY -= (2 - d) * 180.f;
	if (isPairY == false)
	{
		startY += 90.f;
	}

	float height = 200.f;
	float widthCase = (a * 180.f) + 100.f;
	float heightCase = 256.f;

	Node* pNode = Node::create();
	m_pNodeSequence->addChild(pNode);

	for (int y = 0; y < b; y++)
	{
		float posY = startY - (y * height);

		// 돌
		Node* pNodeStonePack = Node::create();
		pNodeStonePack->setVisible(false);
		pNode->addChild(pNodeStonePack);
		for (int x = 0; x < a; x++)
		{
			Sprite* pSpriteStone = Sprite::create(resourcePath + m_curLevelStruct.m_objectSource[0]);
			Vec2 pos;
			pos.x = startX + (x * 180.f);
			pos.y = posY;
			pSpriteStone->setPosition(pos);
			pNodeStonePack->addChild(pSpriteStone);
		}
		
		// 케이스
		ui::Scale9Sprite *pSpriteCase = ui::Scale9Sprite::create(resourcePath + "wordwindow-stone-case.png");
		pSpriteCase->setContentSize(Size(widthCase, heightCase));
		pSpriteCase->setPosition(Vec2(1280.f, posY + 5.f));
		pSpriteCase->setVisible(false);
		pNode->addChild(pSpriteCase);

		// 액션
		Sequence* sequence = Sequence::create(
			DelayTime::create(m_accumulateDelay),
			CallFunc::create([this, pNodeStonePack, pSpriteCase]()
		{ 
			pNodeStonePack->setVisible(true);
			pSpriteCase->setVisible(true);

			// 효과음
			playSoundAddObjectPack();
		}),
			nullptr);
		pNode->runAction(sequence);

		// 다음돌 대기 시간 추가
		m_accumulateDelay += m_stoneIntervalDelay;
	}
}

void WordWindowScene::playHintSimpleSequenceComparison()
{
	//CCLOG("[WordWindowScene::playHintSimpleSequenceComparison]");

	// 라인
	Sprite* pSpriteLine = Sprite::create(resourcePath + "wordwindow-comparison-line-copy.png");
	Vec2 pos;
	Size winSize = getContentSize();
	pos.x = 1280.f;
	pos.y = 578.f + 495.f;
	pSpriteLine->setPosition(pos);
	m_pNodeSequence->addChild(pSpriteLine);

	bool isTopOver = false;
	int topCount = m_curLevelStruct.m_objectValue[0];
	int bottomCount = m_curLevelStruct.m_objectValue[1];
	int sameCount = 0;
	int remainderCount = 0;
	if (topCount > bottomCount)
	{
		isTopOver = true;
		sameCount = bottomCount;
		remainderCount = topCount - bottomCount;
	}
	else
	{
		sameCount = topCount;
		remainderCount = bottomCount - topCount;
	}

	// 똑같은 수
	playAddStoneAllComparisonSimple(0, sameCount, isTopOver);

	// 다음 돌 놓는 대기 시간 추가
	m_accumulateDelay += m_stoneIntervalDelay;

	// 나머지 수
	playAddStoneAllComparisonSimple(1, remainderCount, isTopOver);
}

void WordWindowScene::playAddStonePackComparisonSimple(int index, bool isTopOver, const std::string& resourceName)
{
	// 10개 묶음(상단)
	if (index == 0)
	{
		Node* pNodeStonePack = Node::create();
		m_pNodeSequence->addChild(pNodeStonePack);
		for (int k = 0; k < 10; k++)
		{
			Sprite* pSpriteStone = Sprite::create(resourcePath + m_curLevelStruct.m_objectSource[0]);
			Vec2 pos;
			pos.x = 166.f + 217.f + (174.f / 2.f) + (k * 180.f);
			pos.y = 578.f + 495.f + (174.f / 2.f) + 20.f + 180.f;
			pSpriteStone->setPosition(pos);
			pNodeStonePack->addChild(pSpriteStone);
		}

		// 연출
		Sequence* sequence = Sequence::create(
			DelayTime::create(m_accumulateDelay),
			CallFunc::create([this, pNodeStonePack]()
		{ 
			pNodeStonePack->setVisible(true); 

			// 효과음
			playSoundAddObjectPack();
		}),
			nullptr);
		pNodeStonePack->runAction(sequence);
	}
	

	// 10개 묶음(하단)
	if (index == 0)
	{
		Node* pNodeStonePack = Node::create();
		m_pNodeSequence->addChild(pNodeStonePack);
		for (int k = 0; k < 10; k++)
		{
			Sprite* pSpriteStone = Sprite::create(resourcePath + m_curLevelStruct.m_objectSource[1]);
			Vec2 pos;
			pos.x = 166.f + 217.f + (174.f / 2.f) + (k * 180.f);
			pos.y = 578.f + 495.f + (174.f / 2.f) - 20.f - 180.f;
			pSpriteStone->setPosition(pos);
			pNodeStonePack->addChild(pSpriteStone);
		}

		// 연출
		Sequence* sequence = Sequence::create(
			DelayTime::create(m_accumulateDelay),
			CallFunc::create([this, pNodeStonePack]()
		{ 
			pNodeStonePack->setVisible(true); 

			// 효과음
			playSoundAddObjectPack();
		}),
			nullptr);
		pNodeStonePack->runAction(sequence);
	}

	// 총 갯수 더해준다.
	addAccumulateCount(10);

	// 대기 시간 추가
	m_accumulateDelay += m_stoneIntervalDelay;
}

void WordWindowScene::playAddStoneComparisonSimple(int index, bool isTopOver, const std::string& resourceName)
{
	// 상단
	if (index == 0 || (index == 1 && isTopOver == true))
	{
		Vec2 posTop;
		Sprite* pSpriteStoneTop = Sprite::create(resourcePath + m_curLevelStruct.m_objectSource[0]);
		posTop.x = 166.f + 217.f + (174.f / 2.f) + ((m_accumulateCount % 10) * 180.f);
		posTop.y = 578.f + 495.f + (174.f / 2.f) + 20.f + 180.f - ((m_accumulateCount / 10) * 180.f);
		pSpriteStoneTop->setPosition(posTop);
		pSpriteStoneTop->setVisible(false);
		m_pNodeSequence->addChild(pSpriteStoneTop);

		Sequence* sequence = Sequence::create(
			DelayTime::create(m_accumulateDelay),
			CallFunc::create([this, pSpriteStoneTop]()
		{
			pSpriteStoneTop->setVisible(true);

			// 효과음
			playSoundAddObject();
		}),
			nullptr);
		pSpriteStoneTop->runAction(sequence);
	}
	

	// 하단
	if (index == 0 || (index == 1 && isTopOver == false))
	{
		Vec2 posBottom;
		Sprite* pSpriteStoneBottom = Sprite::create(resourcePath + m_curLevelStruct.m_objectSource[1]);
		posBottom.x = 166.f + 217.f + (174.f / 2.f) + ((m_accumulateCount % 10) * 180.f);
		posBottom.y = 578.f + 495.f + (174.f / 2.f) - 20.f - 180.f - ((m_accumulateCount / 10) * 180.f);
		pSpriteStoneBottom->setPosition(posBottom);
		pSpriteStoneBottom->setVisible(false);
		m_pNodeSequence->addChild(pSpriteStoneBottom);

		Sequence* sequence = Sequence::create(
			DelayTime::create(m_accumulateDelay),
			CallFunc::create([this, pSpriteStoneBottom]()
		{
			pSpriteStoneBottom->setVisible(true);

			// 효과음
			playSoundAddObject();
		}),
			nullptr);
		pSpriteStoneBottom->runAction(sequence);
	}

	// 총 갯수 더해준다.
	addAccumulateCount(1);

	// 딜레이 추가
	m_accumulateDelay += m_stoneSimpleDelay;
}

void WordWindowScene::playAddStoneAllComparisonSimple(int index, int objectCount, bool isTopOver)
{
	if (isPackAddition(objectCount) == true)
	{
		// 현재 라인 남은 갯수
		int remainderFirst = (10 - (m_accumulateCount % 10)) % 10;

		// 현재 라인 남은 갯수를 뺀 나머지 갯수
		int remainder = objectCount - remainderFirst;

		// 10개 묶음 갯수
		int tenPackCount = remainder / 10;

		// 묶음 갯수를 뺀 최종 나머지 갯수
		int remainderEnd = remainder - (tenPackCount * 10);

		for (int i = 0; i < remainderFirst; i++)
		{
			playAddStoneComparisonSimple(index, isTopOver, m_curLevelStruct.m_objectSource[index]);
		}

		for (int i = 0; i < tenPackCount; i++)
		{
			playAddStonePackComparisonSimple(index, isTopOver, m_curLevelStruct.m_objectSource[index]);
		}

		for (int i = 0; i < remainderEnd; i++)
		{
			playAddStoneComparisonSimple(index, isTopOver, m_curLevelStruct.m_objectSource[index]);
		}
	}
	else
	{
		for (int k = 0; k < objectCount; k++)
		{
			playAddStoneComparisonSimple(index, isTopOver, m_curLevelStruct.m_objectSource[index]);
		}
	}
}

void WordWindowScene::playHintSequence()
{
	//CCLOG("[WordWindowScene::playHintSequence]");

	m_pNodeSequence = Node::create();
	_gameNode->addChild(m_pNodeSequence);

	// 처음 위치를 계산
	calculateStartPosY();

	m_accumulateDelay += m_sequenceBeforeTerm;

	// 덧셈 시퀀스
	if (m_curLevelStruct.m_sequenceType == SEQUENCE_TYPE_ADDITION)
	{
		playHintSequenceAddition();
	}
	// 뺄셈 시퀀스
	else if (m_curLevelStruct.m_sequenceType == SEQUENCE_TYPE_SUBTRACTION)
	{
		playHintSequenceSubtraction();
	}
	// 곱셈 시퀀스
	else if (m_curLevelStruct.m_sequenceType == SEQUENCE_TYPE_MULTIPLY)
	{
		playHintSequenceMultiply();
	}
	// 비교 시퀀스
	else if (m_curLevelStruct.m_sequenceType == SEQUENCE_TYPE_COMPARISION)
	{
		playHintSequenceComparison();
	}
	// 포함 시퀀스
	else if (m_curLevelStruct.m_sequenceType == SEQUENCE_TYPE_INCLUSION)
	{
		playHintSequenceInclusion();
	}
	// 방정식 시퀀스
	else if (m_curLevelStruct.m_sequenceType == SEQUENCE_TYPE_EQUATION)
	{
		playHintSequenceEquation();
	}

	// 모든 에니메이션에 소요되는 시간이 5초 이상이면 5초가 되게 속도를 변경
	float totalAnimationTime = m_accumulateDelay - m_sequenceBeforeTerm;
	if (totalAnimationTime > m_totalAnimationTime)
	{
		float speed = totalAnimationTime / m_totalAnimationTime;
		setGameSpeed(speed);
	}

	// 종료 이벤트 추가
	scheduleOnce(schedule_selector(WordWindowScene::onEndSequence), m_accumulateDelay);
}

void WordWindowScene::onEndSequence(float dt)
{
	//CCLOG("[WordWindowScene::onEndSequence]");

	// 게임 속도 리셋
	resetGameSpeed();

	// 수식 위에 반짝이 연출
	Vec2 pos = Vec2(1280.f - m_shiningParticleInterval, 41.f + (488.f / 2.f));
	createShiningParticle(pos);

	pos = Vec2(1280.f, 41.f + (488.f / 2.f));
	createShiningParticle(pos);

	pos = Vec2(1280.f + m_shiningParticleInterval, 41.f + (488.f / 2.f));
	createShiningParticle(pos);

	scheduleOnce(schedule_selector(WordWindowScene::onStopParticle), m_rightFormulaEffectTime);

	// 정답 효과음
	playSoundSolve();

	// 종료 대기 시간
	playBoardFlip2(m_backBoardDelay);
}

float WordWindowScene::getCurLinePosY()
{
	float posY = m_sequenceStartY - (m_curSequenceLine * 180.f);
	return posY;
}

float WordWindowScene::getTopPosY()
{
	return 1900.f;
}

float WordWindowScene::getBottomPosY()
{
	return -100.f;
}

void WordWindowScene::setSpriteStone(Sprite* pSpriteStone)
{
	m_pSpriteStone[m_spriteStoneIndex] = pSpriteStone;
	m_spriteStoneIndex++;
}

void WordWindowScene::playAddStone(const std::string& resourceName)
{
	//CCLOG("[WordWindowScene::playAddStone]");

	int totalIndex = m_accumulateCount;

	Vec2 pos;
	Vec2 posStart;

	float distance = 0.f;

	// 돌 추가
	Sprite* pSpriteStone = Sprite::create(resourcePath + resourceName);
	pos.x = 166.f + 217.f + (174.f / 2.f) + ((totalIndex % 10) * 180.f);
	pos.y = getCurLinePosY();

	// 화면 상단으로 시작 위치
	posStart = pos;
	posStart.y = getTopPosY();
	distance = posStart.y - pos.y;

	pSpriteStone->setPosition(posStart);
	m_pNodeSequence->addChild(pSpriteStone);

	setSpriteStone(pSpriteStone);

	// 고양이 손 추가
	Sprite* pSpriteCat = Sprite::create(resourcePath + getCatFilename());
	pSpriteCat->setPosition(Vec2(160.f, 580.f));
	// 오른쪽이면 고양이 손 플립
	if ((totalIndex % 10) >= 5)
	{
		pSpriteCat->setFlippedX(true);
		pSpriteCat->setPositionX(pSpriteCat->getPositionX() - 100.f);
	}
	pSpriteStone->addChild(pSpriteCat);

	// 액션
	Sequence* sequence = Sequence::create(
		DelayTime::create(m_accumulateDelay),
		EaseOut::create(MoveTo::create(m_catDownTime, pos), 3.f),
		CallFunc::create([this, pSpriteCat, distance]()
	{
		pSpriteCat->runAction(MoveTo::create(m_catUpTime, Vec2(160.f, 580.f + distance))); 

		// 효과음
		playSoundAddObject();
	}),	
		nullptr);
	pSpriteStone->runAction(sequence);

	// 총 갯수 더해준다.
	addAccumulateCount(1);

	calculateCurLine();

	// 딜레이 추가
	m_accumulateDelay += m_stoneDelay;
}

void WordWindowScene::calculateCurLine()
{
	m_curSequenceLine = (m_accumulateCount / 10);
}

void WordWindowScene::calculateCurLineSub()
{
	if (m_accumulateCount <= 10)
	{
		m_curSequenceLine = 0;
	}
	else if (m_accumulateCount <= 20)
	{
		m_curSequenceLine = 1;
	}
	else if (m_accumulateCount <= 30)
	{
		m_curSequenceLine = 2;
	}
	else if (m_accumulateCount <= 40)
	{
		m_curSequenceLine = 3;
	}
}

void WordWindowScene::addAccumulateCount(int add)
{
	m_accumulateCount += add;
}

void WordWindowScene::playAddStonePack(const std::string& resourceName)
{
	// 현재 라인 높이값
	float curLinePosY = getCurLinePosY();

	// 10개 묶음(상단)
	Node* pNodePack = Node::create();
	m_pNodeSequence->addChild(pNodePack);
	for (int k = 0; k < 10; k++)
	{
		Sprite* pSpriteStone = Sprite::create(resourcePath + resourceName);
		Vec2 pos;
		pos.x = 166.f + 217.f + (174.f / 2.f) + (k * 180.f);
		pos.y = curLinePosY;	// 1160.f
		pSpriteStone->setPosition(pos);
		pNodePack->addChild(pSpriteStone);

		// 스프라이트 세팅
		setSpriteStone(pSpriteStone);
	}

	// 화면 상단 위치
	Vec2 pos = pNodePack->getPosition();
	Vec2 posStart = pos;
	Vec2 winSize = getContentSize();
	float distance = winSize.y - curLinePosY + 340.f; // 940.f;
	posStart.y = distance - 190.f; // 750.f;
	pNodePack->setPosition(posStart);

	Node* pNodePack2 = Node::create();
	pNodePack->addChild(pNodePack2);

	// 케이스
	ui::Scale9Sprite *pSpriteCase = ui::Scale9Sprite::create(resourcePath + "wordwindow-stone-case.png");
	pSpriteCase->setContentSize(Size(1900.f, 250.f));
	pSpriteCase->setPosition(Vec2(1283.f, curLinePosY + 5.f));
	pNodePack2->addChild(pSpriteCase);

	// 고양이 손
	Sprite* pSpriteCatLeft = Sprite::create(resourcePath + getCatFilename());
	pSpriteCatLeft->setPosition(Vec2(400.f, curLinePosY + 480.f));
	pNodePack2->addChild(pSpriteCatLeft);
	Sprite* pSpriteCatRight = Sprite::create(resourcePath + getCatFilename());
	pSpriteCatRight->setPosition(Vec2(2160.f, curLinePosY + 480.f));
	pSpriteCatRight->setFlippedX(true);
	pNodePack2->addChild(pSpriteCatRight);

	// 연출
	Sequence* sequence = Sequence::create(
		DelayTime::create(m_accumulateDelay),
		EaseOut::create(MoveTo::create(m_catDownTime, pos), 3.f),
		CallFunc::create([this, pNodePack2, distance]()
	{ 
		pNodePack2->runAction(MoveTo::create(m_catUpTime, Vec2(0.f, distance))); 

		// 효과음
		playSoundAddObjectPack();
	}), 
		nullptr);
	pNodePack->runAction(sequence);

	// 총 갯수 더해준다.
	addAccumulateCount(10);

	// 딜레이 추가
	m_accumulateDelay += m_catDownTime;
	m_accumulateDelay += m_catUpTime;
	m_accumulateDelay += m_stoneIntervalDelay;

	// 라인 새로 계산
	calculateCurLine();
}

void WordWindowScene::playAddStoneAll(int index, int objectCount)
{
	if (isPackAddition(objectCount) == true)
	{
		// 현재 라인 남은 갯수
		int remainderFirst = (10 - (m_accumulateCount % 10)) % 10;

		// 현재 라인 남은 갯수를 뺀 나머지 갯수
		int remainder = objectCount - remainderFirst;

		// 10개 묶음 갯수
		int tenPackCount = remainder / 10;

		// 묶음 갯수를 뺀 최종 나머지 갯수
		int remainderEnd = remainder - (tenPackCount * 10);

		for (int i = 0; i < remainderFirst; i++)
		{
			playAddStone(m_curLevelStruct.m_objectSource[index]);
		}

		for (int i = 0; i < tenPackCount; i++)
		{
			playAddStonePack(m_curLevelStruct.m_objectSource[index]);
		}

		for (int i = 0; i < remainderEnd; i++)
		{
			playAddStone(m_curLevelStruct.m_objectSource[index]);
		}
	}
	else
	{
		for (int k = 0; k < objectCount; k++)
		{
			playAddStone(m_curLevelStruct.m_objectSource[index]);
		}
	}
}

void WordWindowScene::playHintSequenceAddition()
{
	for (int i = 0; i < 4; i++)
	{
		// 고양이 손 종류 선택
		selectCatType();

		// 동물 효과음
		playSoundAnimal(i, m_accumulateDelay - m_stoneIntervalDelay);

		// 돌 놓는 연출
		playAddStoneAll(i, m_curLevelStruct.m_objectValue[i]);

		// 다음 돌 놓는 딜레이 추가
		m_accumulateDelay += m_stoneIntervalDelay;
	}
}

void WordWindowScene::playSubStoneAll(int index, int objectCount)
{
	if (isPackSubtraction(index, objectCount) == true)
	{
		// 현재 라인 남은 갯수
		int remainderFirst = (m_accumulateCount % 10);

		// 현재 라인 남은 갯수를 뺀 나머지 갯수
		int remainder = objectCount - remainderFirst;

		// 10개 묶음 갯수
		int tenPackCount = remainder / 10;

		// 묶음 갯수를 뺀 최종 나머지 갯수
		int remainderEnd = remainder - (tenPackCount * 10);

		for (int i = 0; i < remainderFirst; i++)
		{
			playSubStone(m_curLevelStruct.m_objectSource[0]);
		}

		for (int i = 0; i < tenPackCount; i++)
		{
			playSubStonePack(m_curLevelStruct.m_objectSource[0]);
		}

		for (int i = 0; i < remainderEnd; i++)
		{
			playSubStone(m_curLevelStruct.m_objectSource[0]);
		}
	}
	else
	{
		for (int k = 0; k < objectCount; k++)
		{
			playSubStone(m_curLevelStruct.m_objectSource[0]);
		}
	}
}

void WordWindowScene::playSubStone(const std::string& resourceName)
{
	int totalIndex = m_accumulateCount - 1;
	Vec2 pos;
	Vec2 posStart;
	float distance = 0.f;
	float curLinePosY = getCurLinePosY();

	// 노드 추가
	Node* pNode = Node::create();
	m_pNodeSequence->addChild(pNode);

	pos.x = 166.f + 217.f + (174.f / 2.f) + ((totalIndex % 10) * 180.f);
	pos.y = curLinePosY;

	// 화면 상단으로 시작 위치
	posStart = pos;
	posStart.y = getTopPosY();
	pNode->setPosition(posStart);
	
	// 돌 추가
	Sprite* pSpriteStone = Sprite::create(resourcePath + resourceName);
	pSpriteStone->setVisible(false);
	pNode->addChild(pSpriteStone);

	// 고양이 손 추가
	Sprite* pSpriteCat = Sprite::create(resourcePath + getCatFilename());
	pSpriteCat->setPosition(Vec2(60.f, 480.f));
	// 오른쪽이면 고양이 손 플립
	if ((totalIndex % 10) >= 5)
	{
		pSpriteCat->setFlippedX(true);
		pSpriteCat->setPositionX(pSpriteCat->getPositionX() - 100.f);
	}
	pNode->addChild(pSpriteCat);

	// 빈 공간 추가
	Sprite* pSpriteEmpty = Sprite::create(resourcePath + "wordwindow-stone-empty.png");
	Vec2 emptyPos;
	emptyPos.x = 166.f + 217.f + (174.f / 2.f) + ((totalIndex % 10) * 180.f);
	emptyPos.y = curLinePosY;
	pSpriteEmpty->setPosition(emptyPos);
	pSpriteEmpty->setVisible(false);
	m_pNodeSequence->addChild(pSpriteEmpty);

	// 액션
	Sequence* sequence = Sequence::create(
		DelayTime::create(m_accumulateDelay),
		EaseOut::create(MoveTo::create(m_catDownTime, pos), 3.f),
		CallFunc::create([this, pNode, pSpriteStone, posStart, pSpriteEmpty, totalIndex]()
	{
		pSpriteStone->setVisible(true);
		pNode->runAction(MoveTo::create(m_catUpTime, posStart));
		pSpriteEmpty->setVisible(true);

		if (m_pSpriteStone[totalIndex])
		{
			m_pSpriteStone[totalIndex]->setVisible(false);
		}

		// 효과음
		playSoundSubObject();
	}),
		nullptr);
	pNode->runAction(sequence);

	// 총 갯수 빼준다.
	addAccumulateCount(-1);

	// 현재 라인수 새로 계산
	calculateCurLineSub();

	// 딜레이 추가
	m_accumulateDelay += m_stoneDelay;
}

void WordWindowScene::playSubStonePack(const std::string& resourceName)
{
	// 현재 라인 높이값
	float curLinePosY = getCurLinePosY();

	// 10개 묶음(상단)
	Node* pNodePack = Node::create();
	m_pNodeSequence->addChild(pNodePack);

	Node* pNodeStonePack = Node::create();
	pNodeStonePack->setVisible(false);
	pNodePack->addChild(pNodeStonePack);

	for (int k = 0; k < 10; k++)
	{
		Sprite* pSpriteStone = Sprite::create(resourcePath + resourceName);
		Vec2 pos;
		pos.x = 166.f + 217.f + (174.f / 2.f) + (k * 180.f);
		pos.y = curLinePosY;	// 1160.f
		pSpriteStone->setPosition(pos);
		pNodeStonePack->addChild(pSpriteStone);
	}

	// 화면 상단 위치
	Vec2 pos = pNodePack->getPosition();
	Vec2 posStart = pos;
	Vec2 winSize = getContentSize();
	float distance = winSize.y - curLinePosY + 340.f; // 940.f;
	posStart.y = distance - 190.f; // 750.f;
	pNodePack->setPosition(posStart);

	// 케이스와 고양이 손 추가
	Node* pNodePack2 = Node::create();
	pNodePack->addChild(pNodePack2);
	ui::Scale9Sprite *pSpriteCase = ui::Scale9Sprite::create(resourcePath + "wordwindow-stone-case.png");
	pSpriteCase->setContentSize(Size(1900.f, 250.f));
	pSpriteCase->setPosition(Vec2(1283.f, curLinePosY + 5.f));
	pNodePack2->addChild(pSpriteCase);
	Sprite* pSpriteCatLeft = Sprite::create(resourcePath + getCatFilename());
	pSpriteCatLeft->setPosition(Vec2(400.f, curLinePosY + 480.f));
	pNodePack2->addChild(pSpriteCatLeft);
	Sprite* pSpriteCatRight = Sprite::create(resourcePath + getCatFilename());
	pSpriteCatRight->setPosition(Vec2(2160.f, curLinePosY + 480.f));
	pSpriteCatRight->setFlippedX(true);
	pNodePack2->addChild(pSpriteCatRight);

	// 빈 공간 추가
	Node* pNodeEmptyPack = Node::create();
	pNodeEmptyPack->setVisible(false);
	m_pNodeSequence->addChild(pNodeEmptyPack);
	for (int k = 0; k < 10; k++)
	{
		Sprite* pSpriteEmpty = Sprite::create(resourcePath + "wordwindow-stone-empty.png");
		Vec2 emptyPos;
		emptyPos.x = 166.f + 217.f + (174.f / 2.f) + (k * 180.f);
		emptyPos.y = curLinePosY;
		pSpriteEmpty->setPosition(emptyPos);
		pNodeEmptyPack->addChild(pSpriteEmpty);
	}

	int oldAccumulateCount = m_accumulateCount;

	// 연출
	Sequence* sequence = Sequence::create(
		DelayTime::create(m_accumulateDelay),
		EaseOut::create(MoveTo::create(m_catDownTime, pos), 3.f),
		CallFunc::create([this, pNodePack, pNodeStonePack, posStart, pNodeEmptyPack, oldAccumulateCount]()
	{ 
		pNodeEmptyPack->setVisible(true);

		for (int i = 0; i < 10; i++)
		{
			int index = oldAccumulateCount - i - 1;
			if (m_pSpriteStone[index] != nullptr)
			{
				m_pSpriteStone[index]->setVisible(false);
			}
		}

		pNodeStonePack->setVisible(true);

		pNodePack->runAction(MoveTo::create(m_catUpTime, posStart));

		// 효과음
		playSoundSubObjectPack();
	}),
		nullptr);
	pNodePack->runAction(sequence);

	// 딜레이 추가
	m_accumulateDelay += m_catDownTime;
	m_accumulateDelay += m_catUpTime;
	m_accumulateDelay += m_stoneIntervalDelay;

	// 총 갯수 빼준다.
	addAccumulateCount(-10);

	// 라인 새로 계산
	calculateCurLineSub();
}

void WordWindowScene::playHintSequenceSubtraction()
{
	//CCLOG("[WordWindowScene::playHintSequenceSubtraction]");

	// 고양이 손 종류 선택
	selectCatType();

	// 동물 효과음
	playSoundAnimal(0, m_accumulateDelay - m_stoneIntervalDelay);

	// 첫번째 수는 돌을 놓는 연출
	playAddStoneAll(0, m_curLevelStruct.m_objectValue[0]);

	// 다음 돌 놓는 딜레이 추가
	m_accumulateDelay += m_stoneIntervalDelay;

	// 빼기용으로 라인 새로 계산
	calculateCurLineSub();

	// 다음 수 부터는 돌을 뺀다.
	for (int i = 1; i < 4; i++)
	{
		// 고양이 손 종류 선택
		selectCatType();

		// 동물 효과음
		playSoundAnimal(i, m_accumulateDelay - m_stoneIntervalDelay);

		// 돌을 빼는 연출
		playSubStoneAll(i, m_curLevelStruct.m_objectValue[i]);

		// 다음 돌 놓는 딜레이 추가
		m_accumulateDelay += m_stoneIntervalDelay;
	}
}

void WordWindowScene::playHintSequenceMultiply()
{
	//CCLOG("[WordWindowScene::playHintSequenceMultiply]");

	// 고양이 손 종류 선택
	selectCatType();

	// 동물 효과음
	playSoundAnimal(0, m_accumulateDelay - m_stoneIntervalDelay);

	int a = m_curLevelStruct.m_objectValue[1];
	int b = m_curLevelStruct.m_objectValue[0];

	// 짝수인지
	bool isPairX = false;
	if ((a % 2) == 0)
	{
		isPairX = true;
	}
	bool isPairY = false;
	if ((b % 2) == 0)
	{
		isPairY = true;
	}

	int c = a / 2;
	int d = b / 2;
	float startX = 470.f;
	float startY = 1340.f + 25.f;

	// 시작 위치 계산
	startX += (5 - c) * 180.f;
	if (isPairX == false)
	{
		startX -= 90.f;
	}
	startY -= (2 - d) * 180.f;
	if (isPairY == false)
	{
		startY += 90.f;
	}

	float height = 200.f;
	float widthCase = (a * 180.f) + 100.f;
	float heightCase = 256.f;

	for (int y = 0; y < b; y++)
	{
		float posY = startY - (y * height);

		// 부모
		Node* pNode = Node::create();
		m_pNodeSequence->addChild(pNode);

		// 화면 상단 위치
		Vec2 pos = pNode->getPosition();
		Vec2 posStart = pos;
		Vec2 winSize = getContentSize();
		float distance = winSize.y - posY + 340.f;
		posStart.y = distance - 190.f;
		pNode->setPosition(posStart);

		// 화면 하단 위치
		if (y == 3)
		{
			distance = posY + 340.f;
			posStart.y = -distance + 190.f;
			pNode->setPosition(posStart);
		}

		// 돌
		Node* pNodeStonePack = Node::create();
		pNode->addChild(pNodeStonePack);
		for (int x = 0; x < a; x++)
		{
			Sprite* pSpriteStone = Sprite::create(resourcePath + m_curLevelStruct.m_objectSource[0]);
			Vec2 pos;
			pos.x = startX + (x * 180.f);
			pos.y = posY;
			pSpriteStone->setPosition(pos);
			pNodeStonePack->addChild(pSpriteStone);
		}

		// 케이스
		ui::Scale9Sprite *pSpriteCase = ui::Scale9Sprite::create(resourcePath + "wordwindow-stone-case.png");
		pSpriteCase->setContentSize(Size(widthCase, heightCase));
		pSpriteCase->setPosition(Vec2(1280.f, posY + 5.f));
		pNode->addChild(pSpriteCase);

		// 고양이 손
		Node* pNodeCatPack = Node::create();
		pNode->addChild(pNodeCatPack);

		if (y < 3)
		{
			Sprite* pSpriteCatLeft = Sprite::create(resourcePath + getCatFilename());
			pSpriteCatLeft->setPosition(Vec2(1280.f - (widthCase / 2.f) + 80.f, posY + 470.f));
			pNodeCatPack->addChild(pSpriteCatLeft);
			Sprite* pSpriteCatRight = Sprite::create(resourcePath + getCatFilename());
			pSpriteCatRight->setPosition(Vec2(1280.f + (widthCase / 2.f) - 80.f, posY + 470.f));
			pSpriteCatRight->setFlippedX(true);
			pNodeCatPack->addChild(pSpriteCatRight);

			// 연출
			Sequence* sequence = Sequence::create(
				DelayTime::create(m_accumulateDelay),
				CallFunc::create([this](){ this->playSoundAddObjectPack(); }),
				EaseOut::create(MoveTo::create(m_catDownTime, pos), 3.f),
				CallFunc::create([this, pNodeCatPack, distance]()
			{ 
				pNodeCatPack->runAction(MoveTo::create(m_catUpTime, Vec2(0.f, distance))); 
			}),
				nullptr);
			pNode->runAction(sequence);
		}
		else
		{
			Sprite* pSpriteCatLeft = Sprite::create(resourcePath + getCatFilename());
			pSpriteCatLeft->setPosition(Vec2(1280.f - (widthCase / 2.f) + 80.f, posY - 460.f));
			pSpriteCatLeft->setFlippedY(true);
			pNodeCatPack->addChild(pSpriteCatLeft);
			Sprite* pSpriteCatRight = Sprite::create(resourcePath + getCatFilename());
			pSpriteCatRight->setPosition(Vec2(1280.f + (widthCase / 2.f) - 80.f, posY - 460.f));
			pSpriteCatRight->setFlippedX(true);
			pSpriteCatRight->setFlippedY(true);
			pNodeCatPack->addChild(pSpriteCatRight);

			// 연출
			Sequence* sequence = Sequence::create(
				DelayTime::create(m_accumulateDelay),
				CallFunc::create([this](){ this->playSoundAddObjectPack(); }),
				EaseOut::create(MoveTo::create(m_catDownTime, pos), 3.f),
				CallFunc::create([this, pNodeCatPack, distance]()
			{ 
				pNodeCatPack->runAction(MoveTo::create(m_catUpTime, Vec2(0.f, -distance))); 
			}),
				nullptr);
			pNode->runAction(sequence);
		}

		// 딜레이 추가
		m_accumulateDelay += m_catDownTime;
		m_accumulateDelay += m_catUpTime;
		m_accumulateDelay += m_stoneIntervalDelay;
	}
}

void WordWindowScene::playHintSequenceComparison()
{
	//CCLOG("WordWindowScene::playHintSequenceComparison");

	// 라인
	Sprite* pSpriteLine = Sprite::create(resourcePath + "wordwindow-comparison-line-copy.png");
	Vec2 pos;
	pos.x = 1280.f;
	pos.y = 578.f + 495.f;
	pSpriteLine->setPosition(pos);
	m_pNodeSequence->addChild(pSpriteLine);

	bool isTopOver = false;
	int topCount = m_curLevelStruct.m_objectValue[0];
	int bottomCount = m_curLevelStruct.m_objectValue[1];
	int sameCount = 0;
	int remainderCount = 0;
	if (topCount > bottomCount)
	{
		isTopOver = true;
		sameCount = bottomCount;
		remainderCount = topCount - bottomCount;
	}
	else
	{
		sameCount = topCount;
		remainderCount = bottomCount - topCount;
	}


	// 상단 고양이 손 선택
	selectCatType();
	int oldSelectedCatType = m_curSelectedCatType;

	// 동물 효과음
	playSoundAnimal(0, m_accumulateDelay - m_stoneIntervalDelay);

	// 하단 고양이 손 선택
	selectCatType();
	m_curSelectedCatType2 = oldSelectedCatType;

	// 똑같은 수
	playAddStoneAllComparison(0, sameCount, isTopOver);

	// 다음 돌 놓는 대기 시간 추가
	m_accumulateDelay += m_stoneIntervalDelay;

	// 나머지 수
	playAddStoneAllComparison(1, remainderCount, isTopOver);
}

void WordWindowScene::playAddStoneAllComparison(int index, int objectCount, bool isTopOver)
{
	if (isPackAddition(objectCount) == true)
	{
		// 현재 라인 남은 갯수
		int remainderFirst = (10 - (m_accumulateCount % 10)) % 10;

		// 현재 라인 남은 갯수를 뺀 나머지 갯수
		int remainder = objectCount - remainderFirst;

		// 10개 묶음 갯수
		int tenPackCount = remainder / 10;

		// 묶음 갯수를 뺀 최종 나머지 갯수
		int remainderEnd = remainder - (tenPackCount * 10);

		for (int i = 0; i < remainderFirst; i++)
		{
			playAddStoneComparison(index, isTopOver, m_curLevelStruct.m_objectSource[index]);
		}

		for (int i = 0; i < tenPackCount; i++)
		{
			playAddStonePackComparison(index, isTopOver, m_curLevelStruct.m_objectSource[index]);
		}

		for (int i = 0; i < remainderEnd; i++)
		{
			playAddStoneComparison(index, isTopOver, m_curLevelStruct.m_objectSource[index]);
		}
	}
	else
	{
		for (int k = 0; k < objectCount; k++)
		{
			playAddStoneComparison(index, isTopOver, m_curLevelStruct.m_objectSource[index]);
		}
	}
}

void WordWindowScene::playAddStoneComparison(int index, bool isTopOver, const std::string& resourceName)
{
	int totalIndex = m_accumulateCount;

	// 상단
	if (index == 0 || (index == 1 && isTopOver == true))
	{
		Vec2 pos;
		Vec2 posStart;

		float distance = 0.f;

		// 돌 추가
		Sprite* pSpriteStone = Sprite::create(resourcePath + m_curLevelStruct.m_objectSource[0]);
		pos.x = 166.f + 217.f + (174.f / 2.f) + ((m_accumulateCount % 10) * 180.f);
		pos.y = 578.f + 495.f + (174.f / 2.f) + 20.f + 180.f - ((m_accumulateCount / 10) * 180.f);

		// 화면 상단으로 시작 위치
		posStart = pos;
		posStart.y = getTopPosY();
		distance = posStart.y - pos.y;

		pSpriteStone->setPosition(posStart);
		m_pNodeSequence->addChild(pSpriteStone);

		// 고양이 손 추가
		Sprite* pSpriteCat = Sprite::create(resourcePath + getCatFilename());
		pSpriteCat->setPosition(Vec2(160.f, 580.f));
		// 오른쪽이면 고양이 손 플립
		if ((totalIndex % 10) >= 5)
		{
			pSpriteCat->setFlippedX(true);
			pSpriteCat->setPositionX(pSpriteCat->getPositionX() - 100.f);
		}
		pSpriteStone->addChild(pSpriteCat);

		// 액션
		Sequence* sequence = Sequence::create(
			DelayTime::create(m_accumulateDelay),
			EaseOut::create(MoveTo::create(m_catDownTime, pos), 3.f),
			CallFunc::create([this, pSpriteCat, distance]()
		{ 
			pSpriteCat->runAction(MoveTo::create(m_catUpTime, Vec2(160.f, 580.f + distance))); 

			// 효과음
			playSoundAddObject();
		}),
			nullptr);
		pSpriteStone->runAction(sequence);
	}
	
	// 하단
	if (index == 0 || (index == 1 && isTopOver == false))
	{
		Vec2 pos;
		Vec2 posStart;

		float distance = 0.f;

		// 돌 추가
		Sprite* pSpriteStone = Sprite::create(resourcePath + m_curLevelStruct.m_objectSource[1]);
		pos.x = 166.f + 217.f + (174.f / 2.f) + ((m_accumulateCount % 10) * 180.f);
		pos.y = 578.f + 495.f + (174.f / 2.f) - 20.f - 180.f - ((m_accumulateCount / 10) * 180.f);

		// 화면 하단으로 시작 위치
		posStart = pos;
		posStart.y = getBottomPosY();
		distance = pos.y - posStart.y;

		pSpriteStone->setPosition(posStart);
		m_pNodeSequence->addChild(pSpriteStone);

		// 고양이 손 추가
		Sprite* pSpriteCat = Sprite::create(resourcePath + getCatFilename2());
		pSpriteCat->setPosition(Vec2(160.f, -405.f));
		pSpriteCat->setFlippedY(true);
		// 오른쪽이면 고양이 손 플립
		if ((totalIndex % 10) >= 5)
		{
			pSpriteCat->setFlippedX(true);
			pSpriteCat->setPositionX(pSpriteCat->getPositionX() - 100.f);
		}
		pSpriteStone->addChild(pSpriteCat);

		bool isSound = (index == 1 && isTopOver == false);

		// 액션
		Sequence* sequence = Sequence::create(
			DelayTime::create(m_accumulateDelay),
			EaseOut::create(MoveTo::create(m_catDownTime, pos), 3.f),
			CallFunc::create([this, pSpriteCat, distance, isSound]()
		{ 
			pSpriteCat->runAction(MoveTo::create(m_catUpTime, Vec2(160.f, -(580.f + distance)))); 

			if (isSound == true)
			{
				// 효과음
				playSoundAddObject();
			}
		}),
			nullptr);
		pSpriteStone->runAction(sequence);
	}

	// 총 갯수 더해준다.
	addAccumulateCount(1);

	// 딜레이 추가
	m_accumulateDelay += m_stoneDelay;
}

void WordWindowScene::playAddStonePackComparison(int index, bool isTopOver, const std::string& resourceName)
{
	// 상단
	if (index == 0)
	{
		float curLinePosY = 578.f + 495.f + (174.f / 2.f) + 20.f + 180.f;

		// 10개 묶음(상단)
		Node* pNodePack = Node::create();
		m_pNodeSequence->addChild(pNodePack);
		for (int k = 0; k < 10; k++)
		{
			Sprite* pSpriteStone = Sprite::create(resourcePath + m_curLevelStruct.m_objectSource[0]);
			Vec2 pos;
			pos.x = 166.f + 217.f + (174.f / 2.f) + (k * 180.f);
			pos.y = curLinePosY;
			pSpriteStone->setPosition(pos);
			pNodePack->addChild(pSpriteStone);

			// 스프라이트 세팅
			setSpriteStone(pSpriteStone);
		}

		// 화면 상단 위치
		Vec2 pos = pNodePack->getPosition();
		Vec2 posStart = pos;
		Vec2 winSize = getContentSize();
		float distance = winSize.y - curLinePosY + 340.f;
		posStart.y = distance - 190.f;
		pNodePack->setPosition(posStart);

		// 케이스
		Node* pNodePack2 = Node::create();
		pNodePack->addChild(pNodePack2);
		ui::Scale9Sprite *pSpriteCase = ui::Scale9Sprite::create(resourcePath + "wordwindow-stone-case.png");
		pSpriteCase->setContentSize(Size(1900.f, 250.f));
		pSpriteCase->setPosition(Vec2(1283.f, curLinePosY + 5.f));
		pNodePack2->addChild(pSpriteCase);

		// 고양이 손
		Sprite* pSpriteCatLeft = Sprite::create(resourcePath + getCatFilename());
		pSpriteCatLeft->setPosition(Vec2(400.f, curLinePosY + 480.f));
		pNodePack2->addChild(pSpriteCatLeft);
		Sprite* pSpriteCatRight = Sprite::create(resourcePath + getCatFilename());
		pSpriteCatRight->setPosition(Vec2(2160.f, curLinePosY + 480.f));
		pSpriteCatRight->setFlippedX(true);
		pNodePack2->addChild(pSpriteCatRight);

		// 연출
		Sequence* sequence = Sequence::create(
			DelayTime::create(m_accumulateDelay),
			EaseOut::create(MoveTo::create(m_catDownTime, pos), 3.f),
			CallFunc::create([this, pNodePack2, distance]()
		{ 
			pNodePack2->runAction(MoveTo::create(m_catUpTime, Vec2(0.f, distance))); 

			// 효과음
			playSoundAddObjectPack();
		}),
			nullptr);
		pNodePack->runAction(sequence);
	}
	

	// 하단
	if (index == 0)
	{
		float curLinePosY = 578.f + 495.f + (174.f / 2.f) - 20.f - 180.f;

		// 10개 묶음(상단)
		Node* pNodePack = Node::create();
		m_pNodeSequence->addChild(pNodePack);
		for (int k = 0; k < 10; k++)
		{
			Sprite* pSpriteStone = Sprite::create(resourcePath + m_curLevelStruct.m_objectSource[1]);
			Vec2 pos;
			pos.x = 166.f + 217.f + (174.f / 2.f) + (k * 180.f);
			pos.y = curLinePosY;
			pSpriteStone->setPosition(pos);
			pNodePack->addChild(pSpriteStone);

			// 스프라이트 세팅
			setSpriteStone(pSpriteStone);
		}

		// 화면 하단 위치
		Vec2 pos = pNodePack->getPosition();
		Vec2 posStart = pos;
		float distance = curLinePosY + 340.f;
		posStart.y = -distance - 190.f;
		pNodePack->setPosition(posStart);

		// 케이스
		Node* pNodePack2 = Node::create();
		pNodePack->addChild(pNodePack2);
		ui::Scale9Sprite *pSpriteCase = ui::Scale9Sprite::create(resourcePath + "wordwindow-stone-case.png");
		pSpriteCase->setContentSize(Size(1900.f, 250.f));
		pSpriteCase->setPosition(Vec2(1283.f, curLinePosY + 5.f));
		pNodePack2->addChild(pSpriteCase);

		// 고양이 손
		Sprite* pSpriteCatLeft = Sprite::create(resourcePath + getCatFilename2());
		pSpriteCatLeft->setPosition(Vec2(400.f, curLinePosY - 405.f));
		pSpriteCatLeft->setFlippedY(true);
		pNodePack2->addChild(pSpriteCatLeft);
		Sprite* pSpriteCatRight = Sprite::create(resourcePath + getCatFilename2());
		pSpriteCatRight->setPosition(Vec2(2160.f, curLinePosY - 405.f));
		pSpriteCatRight->setFlippedY(true);
		pSpriteCatRight->setFlippedX(true);
		pNodePack2->addChild(pSpriteCatRight);

		// 연출
		Sequence* sequence = Sequence::create(
			DelayTime::create(m_accumulateDelay),
			EaseOut::create(MoveTo::create(m_catDownTime, pos), 3.f),
			CallFunc::create([this, pNodePack2, distance]()
		{ 
			pNodePack2->runAction(MoveTo::create(m_catUpTime, Vec2(0.f, -distance))); 

			// 효과음
			//playSoundAddObjectPack();
		}),
			nullptr);
		pNodePack->runAction(sequence);
	}

	// 총 갯수 더해준다.
	addAccumulateCount(10);

	// 딜레이 추가
	m_accumulateDelay += m_catDownTime;
	m_accumulateDelay += m_catUpTime;
	m_accumulateDelay += m_stoneIntervalDelay;
}

void WordWindowScene::releaseSequence()
{
	//CCLOG("WordWindowScene::releaseSequence");

	if (m_pNodeSequence != nullptr)
	{
		_gameNode->removeChild(m_pNodeSequence);
		m_pNodeSequence = nullptr;
	}
}

void WordWindowScene::resetSequence()
{
	//CCLOG("WordWindowScene::resetSequence");

	m_isHintSequence = false;
	m_isSimpleSequence = false;
	m_isStonePack = false;
	m_remainder = 0;
	m_accumulateCount = 0;
	m_accumulateDelay = 0.f;
	m_vecSelectedCat.clear();
	m_curSelectedCatType = 0;
	m_curSelectedCatType2 = 0;
	for (int i = 0; i < 40; i++)
	{
		m_pSpriteStone[i] = nullptr;
	}
	m_spriteStoneIndex = 0;
	m_sequenceStartY = 0.f;
	m_curSequenceLine = 0;
	m_selectedStone = 0;
}

void WordWindowScene::playBoardFlip2(float delay)
{
	//CCLOG("[WordWindowScene::playBoardFlip2] delay : %f", delay);

	if (m_isRightAnswer == true && isFinishedLevel() == true)
	{
		scheduleOnce(schedule_selector(WordWindowScene::onFinishedLevel), delay);
	}
	else
	{
		m_pSpriteBoard->stopAllActions();

		auto animation = Animation::create();
		animation->addSpriteFrameWithFile(resourcePath + "wordwindow_frame_squence_2.png");
		animation->addSpriteFrameWithFile(resourcePath + "wordwindow_frame_squence_3.png");
		animation->addSpriteFrameWithFile(resourcePath + "wordwindow_frame_squence_4.png");
		animation->addSpriteFrameWithFile(resourcePath + "wordwindow_frame_squence_5.png");
		animation->addSpriteFrameWithFile(resourcePath + "wordwindow_frame_question_4.png");
		animation->addSpriteFrameWithFile(resourcePath + "wordwindow_frame_question_3.png");
		animation->addSpriteFrameWithFile(resourcePath + "wordwindow_frame_question_2.png");
		animation->addSpriteFrameWithFile(resourcePath + "wordwindow_frame_question_1.png");
		animation->addSpriteFrameWithFile(resourcePath + "wordwindow_frame_question_0.png");
		animation->addSpriteFrameWithFile(resourcePath + "wordwindow_frame_question_1.png");
		animation->setDelayPerUnit(m_boardFlipDelayPerUnit);
		animation->setRestoreOriginalFrame(false);

		auto action = Animate::create(animation);
		auto sequence = Sequence::create(DelayTime::create(delay), CallFunc::create(CC_CALLBACK_0(WordWindowScene::onStartPlayBoardFlip2, this)), action, CallFunc::create(CC_CALLBACK_0(WordWindowScene::onEndPlayBoardFlip2, this)), nullptr);
		m_pSpriteBoard->runAction(sequence);
	}
}

void WordWindowScene::onStartPlayBoardFlip2()
{
	//CCLOG("[WordWindowScene::OnStartPlayBoardFlip2]");

	// 원래 속도로 리셋
	resetGameSpeed();

	// 시퀀스에 사용된 노드를 해제
	releaseSequence();

	// 정답 해제
	releaseRightFormula();

	// 효과음
	playSoundBoardFlip();
}

void WordWindowScene::onEndPlayBoardFlip2()
{
	//CCLOG("[WordWindowScene::OnEndPlayBoardFlip2]");

	// 시퀀스 리셋
	resetSequence();

	if (m_isRightAnswer == true)
	{
		// 해답 수식 해제
		releaseRightFormula();

		// 퍼즐 완료
		onPuzzleComplete();
	}
	else
	{
		// 컨텐츠를 보이고
		showBoardContents();

		// 예제 활성화
		enableExamples();

		if (m_wrongAnswer > 0)
		{
			int index = m_wrongAnswer - 1;
			if (m_pButtonExample[index] != nullptr)
			{
				m_pButtonExample[index]->loadTextureDisabled(resourcePath + "wordwindow-btn-answer-disabled.png");
			}

			m_wrongAnswer = 0;
		}
	}
}

void WordWindowScene::showBoardContents()
{
	//CCLOG("WordWindowScene::showBoardContents");

	// 보드 버튼 활성화
	if (m_pButtonBoard != nullptr)
	{
		m_pButtonBoard->setVisible(true);
		m_pButtonBoard->getRendererNormal()->setVisible(false);
		m_pButtonBoard->getRendererClicked()->setVisible(false);
	}

	if (m_pButtonSound != nullptr)
	{
		m_pButtonSound->setVisible(true);
	}

	if (m_pLabelQuestion != nullptr)
	{
		m_pLabelQuestion->setVisible(true);
	}
}

void WordWindowScene::hideBoardContents()
{
	//CCLOG("WordWindowScene::hideBoardContents");

	// 보드 버튼 비활성화
	if (m_pButtonBoard != nullptr)
	{
		m_pButtonBoard->setVisible(false);
	}

	if (m_pButtonSound != nullptr)
	{
		m_pButtonSound->setVisible(false);
	}

	if (m_pLabelQuestion != nullptr)
	{
		m_pLabelQuestion->setVisible(false);
	}
}

void WordWindowScene::createSoundButton()
{
	//CCLOG("WordWindowScene::createSoundButton");

	m_pButtonSound = Button::create(resourcePath + "wordwindow-btn-speaker-normal.png", "", resourcePath + "wordwindow-btn-speaker-active.png");
	m_pButtonSound->setZoomScale(-0.1f);
	m_pButtonSound->setPosition(Vec2(335.f + (336.f / 2.f), 901.f + (336.f / 2.f)));
	_gameNode->addChild(m_pButtonSound);
	m_pButtonSound->addTouchEventListener([this](Ref*, Widget::TouchEventType e)
	{
		if (e == Widget::TouchEventType::ENDED)
		{
			//CCLOG("[WordWindowScene::createSoundButton] TouchEventType::ENDED");

			if (m_soundDuraton <= 0.f)
			{
			//	playSoundQuestion(m_curLevelStruct.m_soundFilename);
			//VoiceMoldManager::shared()->speak("helloworld");
			speech1(m_curLevelStruct.m_text1);
			}
		}
	});
}

void WordWindowScene::enableSoundButton()
{
	if (m_pButtonSound != nullptr)
	{
		//m_pButtonSound->setEnabled(true);

		m_pButtonSound->setZoomScale(-0.1f);
		m_pButtonSound->getRendererNormal()->setTexture(resourcePath + "wordwindow-btn-speaker-normal.png");
	}
}

void WordWindowScene::disableSoundButton()
{
	if (m_pButtonSound != nullptr)
	{
		//m_pButtonSound->setEnabled(false);

		m_pButtonSound->setZoomScale(0.f);
		m_pButtonSound->getRendererNormal()->setTexture(resourcePath + "wordwindow-btn-speaker-active.png");
	}
}

void WordWindowScene::releaseSoundButton()
{
	//CCLOG("WordWindowScene::releaseSoundButton");

	if (m_pButtonSound != nullptr)
	{
		_gameNode->removeChild(m_pButtonSound);
		m_pButtonSound = nullptr;
	}
}

void WordWindowScene::createQuestion()
{
	//CCLOG("WordWindowScene::createQuestion");

	std::vector<std::string> vecTexts = componentsSeparatedByString(m_curLevelStruct.m_text, "@");
	std::string newText = "";
	for (int i = 0; i < (int)vecTexts.size(); i++)
	{
		newText += vecTexts[i];

		if (i < (int)(vecTexts.size() - 1))
		{
			newText += "\n";
		}
	}

	m_pLabelQuestion = TodoUtil::createLabelMultilineToFit(newText, 80.f, Size(1424.f, 800.f), defaultFont, Color4B::WHITE, TextHAlignment::LEFT, TextVAlignment::CENTER);
	m_pLabelQuestion->setPosition(Vec2(749.f + (1424.f / 2.f), 901.f + (336.f / 2.f)));
	_gameNode->addChild(m_pLabelQuestion);
}

void WordWindowScene::releaseQuestion()
{
	//CCLOG("WordWindowScene::releaseQuestion");

	if (m_pLabelQuestion != nullptr)
	{
		_gameNode->removeChild(m_pLabelQuestion);
		m_pLabelQuestion = nullptr;
	}
}

void WordWindowScene::createExamples()
{
	//CCLOG("WordWindowScene::createExamples");

	releaseExamples();

	for (int i = 0; i < 4; i++)
	{
		m_pButtonExample[i] = Button::create(resourcePath + "wordwindow-btn-answer-normal.png", resourcePath + "wordwindow-btn-answer-normal.png", resourcePath + "wordwindow-btn-answer-disabled.png");
		m_pButtonExample[i]->setPosition(Vec2(117.f + (580.f / 2.f) + (582.f * i), 41.f + (488.f / 2.f)));
		m_pButtonExample[i]->setTag(i + 1);
		_gameNode->addChild(m_pButtonExample[i]);
		m_pButtonExample[i]->addTouchEventListener([this](Ref* sender, Widget::TouchEventType e)
		{
			if (e == Widget::TouchEventType::ENDED)
			{
				Button* pButton = (Button*)sender;
				m_isTouchAnswer = true;
				m_touchAnswerIndex = pButton->getTag();
			}
		});

		// 상단 숫자
		Label* pLabelTop = TodoUtil::createLabel(m_curLevelStruct.m_exampleNumber[i], 188.f, Size(0.f, 0.f), fontName, Color4B(64, 39, 22, 255), TextHAlignment::CENTER, TextVAlignment::CENTER);
		pLabelTop->setPosition(Vec2(580.f / 2.f, 221.f + (135.f / 2.f)));
		m_pButtonExample[i]->addChild(pLabelTop);
		m_pLabelExampleTop[i] = pLabelTop;

		// 하단 문구
		int fontSize = 80;
		getAutoFontSize(m_curLevelStruct.m_exampleWord[i], fontName, Size(400.f, 100.f), false, fontSize);
		Label* pLabelBottom = TodoUtil::createLabel(m_curLevelStruct.m_exampleWord[i], fontSize, Size(400.f, 100.f), fontName, Color4B(64, 39, 22, 255), TextHAlignment::CENTER, TextVAlignment::CENTER);
		pLabelBottom->setPosition(Vec2(580.f / 2.f, 104.f + (100.f / 2.f)));
		m_pButtonExample[i]->addChild(pLabelBottom);
		m_pLabelExampleBottom[i] = pLabelBottom;
	}
}

void WordWindowScene::releaseExamples()
{
	//CCLOG("[WordWindowScene::releaseExamples]");

	for (int i = 0; i < 4; i++)
	{
		if (m_pButtonExample[i] != nullptr)
		{
			_gameNode->removeChild(m_pButtonExample[i]);
			m_pButtonExample[i] = nullptr;
			m_pLabelExampleTop[i] = nullptr;
			m_pLabelExampleBottom[i] = nullptr;
		}
	}
}

void WordWindowScene::hideExamples()
{
	//CCLOG("[WordWindowScene::hideExamples]");

	for (int i = 0; i < 4; i++)
	{
		if (m_pButtonExample[i] != nullptr)
		{
			m_pButtonExample[i]->setVisible(false);
		}
	}
}

void WordWindowScene::enableExamples()
{
	//CCLOG("[WordWindowScene::enableExamples]");

	for (int i = 0; i < 4; i++)
	{
		if (m_pButtonExample[i] != nullptr)
		{
			m_pButtonExample[i]->setEnabled(true);
			m_pButtonExample[i]->setOpacity(255);
		}
		if (m_pLabelExampleTop[i] != nullptr)
		{
			m_pLabelExampleTop[i]->setOpacity(255);
		}
		if (m_pLabelExampleBottom[i] != nullptr)
		{
			m_pLabelExampleBottom[i]->setOpacity(255);
		}
	}
}

void WordWindowScene::disableExamples()
{
	//CCLOG("WordWindowScene::disableExamples");

	for (int i = 0; i < 4; i++)
	{
		if (m_pButtonExample[i] != nullptr)
		{
			m_pButtonExample[i]->setEnabled(false);
		}
	}
}

void WordWindowScene::transparentExamples()
{
	for (int i = 0; i < 4; i++)
	{
		if (m_pButtonExample[i] != nullptr)
		{
			m_pButtonExample[i]->setOpacity(51);
		}
		if (m_pLabelExampleTop[i] != nullptr)
		{
			m_pLabelExampleTop[i]->setOpacity(51);
		}
		if (m_pLabelExampleBottom[i] != nullptr)
		{
			m_pLabelExampleBottom[i]->setOpacity(51);
		}
	}
}

void WordWindowScene::setOpacityExamples(int index, int opacity)
{
	if (index >= 0 && index <= 3)
	{
		if (m_pButtonExample[index] != nullptr)
		{
			m_pButtonExample[index]->setOpacity(opacity);
		}
		if (m_pLabelExampleTop[index] != nullptr)
		{
			m_pLabelExampleTop[index]->setOpacity(opacity);
		}
		if (m_pLabelExampleBottom[index] != nullptr)
		{
			m_pLabelExampleBottom[index]->setOpacity(opacity);
		}
	}
}

void WordWindowScene::startFirstDisableExamples()
{
	m_isFirstDisableExamples = true;
	m_firstDisableExamplesTime = 5.f;

	disableExamples();
	transparentExamples();
	hideExamplesLabel();
}

void WordWindowScene::endFirstDisableExamples()
{
	m_isFirstDisableExamples = false;
	m_firstDisableExamplesTime = 0.f;
}

void WordWindowScene::updateFirstDisableExamples(float dt)
{
	if (m_isFirstDisableExamples == true)
	{
		m_firstDisableExamplesTime -= dt;
		if (m_firstDisableExamplesTime <= 0.f)
		{
			endFirstDisableExamples();
			enableExamples();
			hideExamplesLabel();
			showExamplesLabel(true);
		}
	}
}

void WordWindowScene::hideExamplesLabel()
{
	for (int i = 0; i < 4; i++)
	{
		if (m_pLabelExampleTop[i] != nullptr)
		{
			m_pLabelExampleTop[i]->setOpacity(0);
		}
		if (m_pLabelExampleBottom[i] != nullptr)
		{
			m_pLabelExampleBottom[i]->setOpacity(0);
		}
	}
}

void WordWindowScene::showExamplesLabel(bool isRunAction)
{
	for (int i = 0; i < 4; i++)
	{
		if (m_pLabelExampleTop[i] != nullptr)
		{
			if (isRunAction == true)
			{
				m_pLabelExampleTop[i]->runAction(FadeIn::create(1.f));
			}
			else
			{
				m_pLabelExampleTop[i]->setOpacity(255);
			}
		}
		if (m_pLabelExampleBottom[i] != nullptr)
		{
			if (isRunAction == true)
			{
				m_pLabelExampleBottom[i]->runAction(FadeIn::create(1.f));
			}
			else
			{
				m_pLabelExampleBottom[i]->setOpacity(255);
			}
		}
	}
}

void WordWindowScene::onAnswer(int index)
{
	//CCLOG("[WordWindowScene::onAnswer] index : %d", index);

	// 질문효과음 정지
	stopSoundQuestion();

	m_noTouchTime = 0.f;

	// 이미 정답을 맞춘 다음에는 패스
	if (m_isRightAnswer == true || m_isHintSequence == true)
	{
		return;
	}

	if (m_curLevelStruct.m_rightAnswer == index)
	{
		m_isRightAnswer = true;

		// 정답
		playRightAnswerEffect(index - 1, m_rightEffectTime);
		scheduleOnce(schedule_selector(WordWindowScene::onFinishAnswer), m_rightEffectTime + 0.5f);
		scheduleOnce(schedule_selector(WordWindowScene::onPlaySoundAnswer), 0.05f);

		// 정답 효과음
		playSoundSolve();
	}
	else
	{
		// 오답
		m_wrongAnswer = index;
		onTouchedBoard();
		setOpacityExamples(index - 1, 255);
	}

	// 로그
	string workPath = [this]
	{
		stringstream SS;
		SS << "/" << "wordwindow";
		SS << "/" << "level-" << m_curLevelStruct.m_level << "-" << m_curLevelStruct.m_worksheet;
		SS << "/" << "work-" << (m_currentPuzzleIndex + 1);
		return SS.str();
	}();
	StrictLogManager::shared()->game_Peek_Answer("workwindow", workPath, TodoUtil::itos(index), TodoUtil::itos(m_curLevelStruct.m_rightAnswer));
}

void WordWindowScene::playSoundSolve()
{
	//CCLOG("[WordWindowScene::playSoundSolve]");

	solveEffect().play();
}

void WordWindowScene::playSoundBoardFlip()
{
	SoundEffect("games/wordwindow/sounds/board_flip.m4a").play();
}

void WordWindowScene::playSoundBoardShake()
{
	SoundEffect("games/wordwindow/sounds/board_fliphint.m4a").play();
}

void WordWindowScene::playSoundAddObject()
{
	SoundEffect("games/wordwindow/sounds/put_object_single.m4a").play();
}

void WordWindowScene::playSoundAddObjectPack()
{
	SoundEffect("games/wordwindow/sounds/put_object_frame.m4a").play();
}

void WordWindowScene::playSoundSubObject()
{
	SoundEffect("games/wordwindow/sounds/take_object_single.m4a").play();
}

void WordWindowScene::playSoundSubObjectPack()
{
	SoundEffect("games/wordwindow/sounds/take_object_frame.m4a").play();
}

void WordWindowScene::playSoundPutObjectRow()
{
	m_soundEffectPutObjectRow.playLoop();
}

void WordWindowScene::stopSoundPutObjectRow()
{
	m_soundEffectPutObjectRow.stop();
}

void WordWindowScene::playSoundCat(float dt)
{
	SoundEffect("games/wordwindow/sounds/turn_cat.m4a").play();
}

void WordWindowScene::playSoundTiger(float dt)
{
	SoundEffect("games/wordwindow/sounds/turn_tiger.m4a").play();
}

void WordWindowScene::playSoundBear(float dt)
{
	SoundEffect("games/wordwindow/sounds/turn_bear.m4a").play();
}

void WordWindowScene::playSoundFlipCard()
{
	playSoundAddObject();
}

void WordWindowScene::playSoundAddCard()
{
	playSoundAddObject();
}

void WordWindowScene::playSoundAnimal(int index, float delay)
{
	// 동물 효과음
	if (m_curLevelStruct.m_objectValue[index] > 0 || m_curLevelStruct.m_sequenceType == SEQUENCE_TYPE_EQUATION)
	{
		if (delay > 0.f)
		{
			if (isCat() == true)
			{
				scheduleOnce(schedule_selector(WordWindowScene::playSoundCat), delay);
			}
			else if (isTiger() == true)
			{
				scheduleOnce(schedule_selector(WordWindowScene::playSoundTiger), delay);
			}
			else if (isBear() == true)
			{
				scheduleOnce(schedule_selector(WordWindowScene::playSoundBear), delay);
			}
		}
		else
		{
			if (isCat() == true)
			{
				playSoundCat();
			}
			else if (isTiger() == true)
			{
				playSoundTiger();
			}
			else if (isBear() == true)
			{
				playSoundBear();
			}
		}
		
	}
}

void WordWindowScene::onPlaySoundAnswer(float dt)
{
	//CCLOG("[WordWindowScene::onPlaySoundAnswer]");
}

void WordWindowScene::onFinishAnswer(float dt)
{
	//CCLOG("[WordWindowScene::onFinishAnswer]");

	// 보기 숨긴다.
	hideExamples();

	// 칠판 내용 숨긴다.
	hideBoardContents();

	// 보드판을 플립시킨다.
	playBoardFlip1();

	// 약식시퀀스 실행
	resetSequence();
	m_isHintSequence = true;
}

void WordWindowScene::selectCatType()
{
	//CCLOG("[WordWindowScene::selectCatType]");

	bool isLoop = true;
	while (isLoop)
	{
		int value = cocos2d::random() % 100;
		int selectedCatType = 0;

		// 10% 확률로 고양이가 아닌 다른 동물
		if (value < m_otherCatRandom)
		{
			selectedCatType = (cocos2d::random() % 2) + 4;
		}
		else
		{
			selectedCatType = cocos2d::random() % 4;
		}

		if (isSelectedCatType(selectedCatType) == false)
		{
			isLoop = false;
			m_curSelectedCatType = selectedCatType;
			//CCLOG("[WordWindowScene::selectCatType] selectedCatType : %d", m_curSelectedCatType);
			m_vecSelectedCat.push_back(m_curSelectedCatType);
			break;
		}
	}
}

bool WordWindowScene::isSelectedCatType(int catType)
{
	for (int i = 0; i < (int)m_vecSelectedCat.size(); i++)
	{
		if (m_vecSelectedCat[i] == catType)
		{
			return true;
		}
	}

	return false;
}

bool WordWindowScene::isCat()
{
	if (m_curSelectedCatType <= 3)
	{
		return true;
	}

	return false;
}

bool WordWindowScene::isTiger()
{
	if (m_curSelectedCatType == 4)
	{
		return true;
	}

	return false;
}

bool WordWindowScene::isBear()
{
	if (m_curSelectedCatType == 5)
	{
		return true;
	}

	return false;
}

std::string WordWindowScene::getCatFilename()
{
	return StringUtils::format("wordwindow-catpaw-%02d.png", m_curSelectedCatType + 1);
}

std::string WordWindowScene::getCatFilename2()
{
	return StringUtils::format("wordwindow-catpaw-%02d.png", m_curSelectedCatType2 + 1);
}

void WordWindowScene::playRightAnswerEffect(int index, float time)
{
	//CCLOG("[WordWindowScene::playRightAnswerEffect]");

	Vec2 pos = Vec2(117.f + (580.f / 2.f) + (582.f * index), 41.f + (488.f / 2.f));
	createShiningParticle(pos);

	scheduleOnce(schedule_selector(WordWindowScene::onStopParticle), time);
}

void WordWindowScene::createRightFormula()
{
	//CCLOG("[WordWindowScene::createRightFormula]");

	releaseRightFormula();

	m_pNodeRightFormula = Node::create();
	_gameNode->addChild(m_pNodeRightFormula);	

	// 방정식에서는 정답 밑에 노란 카드 추가
	if (m_curLevelStruct.m_sequenceType == SEQUENCE_TYPE_EQUATION)
	{
		int value1 = m_curLevelStruct.m_equationValueA;
		int value2 = m_curLevelStruct.m_equationValueB;
		int correctAnswer = 0;
		string sign = m_curLevelStruct.m_equationValueSign;
		if (sign == "+")
		{
			correctAnswer = value1 + value2;
		}
		else if (sign == "-")
		{
			correctAnswer = value1 - value2;
		}
		else if (sign == "*")
		{
			correctAnswer = value1 * value2;
		}
		else if (sign == "/")
		{
			correctAnswer = value1 / value2;
		}

		int unknwonIndex = 0;
		int unknwonValue = 0;
		if (m_curLevelStruct.m_equationUnknown == "A")
		{
			unknwonIndex = 0;
		}
		else if (m_curLevelStruct.m_equationUnknown == "B")
		{
			unknwonIndex = 2;
		}
		else if (m_curLevelStruct.m_equationUnknown == "C")
		{
			unknwonIndex = 4;
		}

		float startX = 1280.f - 600.f;
		float gapX = 300.f;
		float y = 189.f + (213.f / 2.f);
		float y_shadow = y - 8.f;

		for (int i = 0; i < 5; i++)
		{
			//
			std::string text;

			if (i == 0)
			{
				text = TodoUtil::itos(value1);
			}
			else if (i == 1)
			{
				if (sign == "*")
				{
					sign = "x";
				}
				else if (sign == "/")
				{
					sign = "%";
				}
				text = sign;
			}
			else if (i == 2)
			{
				text = TodoUtil::itos(value2);
			}
			else if (i == 3)
			{
				text = "=";
			}
			else if (i == 4)
			{
				text = TodoUtil::itos(correctAnswer);
			}

			if (i == unknwonIndex)
			{
				if (unknwonIndex == 0)
				{
					unknwonValue = value1;
				}
				else if (unknwonIndex == 2)
				{
					unknwonValue = value2;
				}
				else if (unknwonIndex == 4)
				{
					unknwonValue = correctAnswer;
				}

				if (unknwonValue >= 10)
				{
					// 박스 추가
					Size winSize = getContentSize();
					ui::Scale9Sprite *pSpriteCard = ui::Scale9Sprite::create(resourcePath + "wordwindow-equation-xbox.png");
					pSpriteCard->setContentSize(Size(400.f, 360.f));
					pSpriteCard->setPosition(Vec2(startX, y - 20.f));
					m_pNodeRightFormula->addChild(pSpriteCard);

					startX += 20.f;

					Label* pLabel = TodoUtil::createLabel(text, 288.f, Size(0.f, 0.f), boldFontName, Color4B(20, 13, 7, 255), TextHAlignment::CENTER, TextVAlignment::CENTER);
					pLabel->setPosition(Vec2(200.f, 180.f + 20.f));
					pSpriteCard->addChild(pLabel);
				}
				else
				{
					// 박스 추가
					Size winSize = getContentSize();
					Sprite* pSpriteCard = Sprite::create(resourcePath + "wordwindow-equation-xbox.png");
					pSpriteCard->setPosition(Vec2(startX, y - 20.f));
					m_pNodeRightFormula->addChild(pSpriteCard);

					Label* pLabel = TodoUtil::createLabel(text, 288.f, Size(0.f, 0.f), boldFontName, Color4B(20, 13, 7, 255), TextHAlignment::CENTER, TextVAlignment::CENTER);
					pLabel->setPosition(Vec2(180.f, 180.f + 20.f));
					pSpriteCard->addChild(pLabel);
				}
			}
			else
			{
				Label* pLabel = TodoUtil::createLabel(text, 288.f, Size(0.f, 0.f), fontName, Color4B(20, 13, 7, 255), TextHAlignment::CENTER, TextVAlignment::CENTER);
				pLabel->setPosition(Vec2(startX, y));
				m_pNodeRightFormula->addChild(pLabel, 10);

				// 그림자 추가
				Label* pLabelShadow = TodoUtil::createLabel(text, 288.f, Size(0.f, 0.f), fontName, Color4B(216, 191, 163, 255), TextHAlignment::CENTER, TextVAlignment::CENTER);
				pLabelShadow->setPosition(Vec2(startX, y_shadow));
				m_pNodeRightFormula->addChild(pLabelShadow, 5);
			}

			startX += gapX;
		}
	}
	else
	{
		std::string xml_formula;
		std::string xml_shadow;
		std::string formula = m_curLevelStruct.m_rightFormula;
		std::vector<WordWindowRightFormulaText> vecTexts;
		getRightFormulaTexts(formula, vecTexts);

		for (int i = 0; i < (int)vecTexts.size(); i++)
		{
			// 수식
			if (vecTexts[i].m_isBold == true)
			{
				xml_formula += "<font color = '#140d07' face = 'main/fonts/andikanewbasic-b.ttf' size = '288'>";
				xml_formula += vecTexts[i].m_text;
				xml_formula += "</font>";
			}
			else
			{
				xml_formula += "<font color ='#140d07' face = 'main/fonts/mukta-bold.ttf' size = '288'>";
				xml_formula += vecTexts[i].m_text;
				xml_formula += "</font>";
			}

			// 그림자
			if (vecTexts[i].m_isBold == true)
			{
				xml_shadow += "<font color = '#d8bfa3' face = 'main/fonts/andikanewbasic-b.ttf' size = '288'>";
				xml_shadow += vecTexts[i].m_text;
				xml_shadow += "</font>";
			}
			else
			{
				xml_shadow += "<font color ='#d8bfa3' face = 'main/fonts/mukta-bold.ttf' size = '288'>";
				xml_shadow += vecTexts[i].m_text;
				xml_shadow += "</font>";
			}
		}

		// 수식
		m_pRichText = ui::RichText::createWithXML(xml_formula);
		m_pRichText->formatText();
		Size size = m_pRichText->getContentSize();
		float scale = 1.f;
		if (size.width > 2236.f)
		{
			scale = 2236.f / size.width;
		}
		m_pRichText->setPosition(Vec2(1280.f, 189.f + (213.f / 2.f)));
		m_pRichText->setAnchorPoint(Vec2(0.5f, 0.5f));
		m_pRichText->setScale(scale);
		m_pNodeRightFormula->addChild(m_pRichText, 10);

		// 그림자
		m_pRichTextShadow = ui::RichText::createWithXML(xml_shadow);
		m_pRichTextShadow->setPosition(Vec2(1280.f, 181.f + (213.f / 2.f)));
		m_pRichTextShadow->setAnchorPoint(Vec2(0.5f, 0.5f));
		m_pRichTextShadow->setScale(scale);
		m_pNodeRightFormula->addChild(m_pRichTextShadow, 5);
	}
}

void WordWindowScene::getRightFormulaTexts(const std::string& text, std::vector<WordWindowRightFormulaText>& vecOutTexts)
{
	int pos = text.find("<");
	if (pos < 0)
	{
		WordWindowRightFormulaText s;
		s.m_isBold = false;
		s.m_text = text;
		vecOutTexts.push_back(s);
		return;
	}

	if (pos > 0)
	{
		std::string text1 = text.substr(0, pos);
		{
			WordWindowRightFormulaText s;
			s.m_isBold = false;
			s.m_text = text1;
			vecOutTexts.push_back(s);
		}
	}
	
	std::string text2 = text.substr(pos + 1, text.size() - (pos + 1));
	pos = text2.find(">");
	std::string text3 = text2.substr(0, pos);
	{
		WordWindowRightFormulaText s;
		s.m_isBold = true;
		s.m_text = text3;
		vecOutTexts.push_back(s);
	}

	std::string text4 = text2.substr(pos + 1, text2.size() - (pos + 1));
	if (text4.empty() == false)
	{
		getRightFormulaTexts(text4, vecOutTexts);
	}
}

void WordWindowScene::releaseRightFormula()
{
	if (m_pNodeRightFormula != nullptr)
	{
		_gameNode->removeChild(m_pNodeRightFormula);
		m_pNodeRightFormula = nullptr;
	}

	if (m_pRichText != nullptr)
	{
		_gameNode->removeChild(m_pRichText);
		m_pRichText = nullptr;
	}

	if (m_pRichTextShadow != nullptr)
	{
		_gameNode->removeChild(m_pRichTextShadow);
		m_pRichTextShadow = nullptr;
	}
}

void WordWindowScene::createShiningParticle(const Vec2& pos)
{
	//CCLOG("[WordWindowScene::createShiningParticle]");

	Node* pParticleNode = Node::create();

	auto createParticleEffect = [&](std::string name, std::string plist) 
	{
		ParticleSystemQuad* particleEffect = nullptr;
		particleEffect = ParticleSystemQuad::create(plist);
		particleEffect->setName(name);
		particleEffect->setScale(1.8f);
		particleEffect->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
		particleEffect->setPosition(Vec2::ZERO);
		pParticleNode->addChild(particleEffect);
	};

	createParticleEffect("particle1", "main/common/effects/particle/shining_particle_blur.plist");
	createParticleEffect("particle2", "main/common/effects/particle/shining_particle_circle.plist");
	createParticleEffect("particle3", "main/common/effects/particle/shining_particle_star.plist");

	pParticleNode->setPosition(pos);
	_gameNode->addChild(pParticleNode);

	m_vecParticleNodes.push_back(pParticleNode);
}

void WordWindowScene::releaseShiningParticle()
{
	//CCLOG("[WordWindowScene::releaseShiningParticle]");

	for (int i = 0; i < (int)m_vecParticleNodes.size(); i++)
	{
		Node* pParticleNode = m_vecParticleNodes[i];

		if (pParticleNode != nullptr)
		{
			_gameNode->removeChild(pParticleNode);
			pParticleNode = nullptr;
		}
	}
}

void WordWindowScene::onStopParticle(float dt)
{
	//CCLOG("[WordWindowScene::onStopParticle]");

	releaseShiningParticle();
}

std::vector<std::string> WordWindowScene::componentsSeparatedByString(const std::string& str, const std::string& seperator)
{
	std::vector<std::string> ret;

	int numSplits = 0;
	size_t start = 0;
	size_t pos = 0;
	do
	{
		pos = str.find_first_of(seperator.c_str(), start);
		if (pos == start)
		{
			start = pos + 1;
		}
		else if (pos == std::string::npos)
		{
			std::string temp = str.substr(start);
			ret.push_back(temp);
			break;
		}
		else
		{
			std::string temp = str.substr(start, pos - start);
			ret.push_back(temp);

			start = pos + 1;
		}
		start = str.find_first_not_of(seperator.c_str(), start);
		++numSplits;

	} while (pos != std::string::npos);

	return ret;
}

void WordWindowScene::getAutoFontSize(const std::string& strText, const std::string& fontName, const Size boxSize, bool isLineBreakWithoutSpace, int& fontSize)
{
	if (boxSize.width <= 0 || boxSize.height <= 0)
	{
		return;
	}

	if (fontSize == 0)
	{
		return;
	}

	Label* pLabel = Label::createWithTTF(strText, fontName, fontSize);
	pLabel->setLineBreakWithoutSpace(isLineBreakWithoutSpace);
	float fontWidth = pLabel->getBoundingBox().getMaxX() - pLabel->getBoundingBox().getMinX();
	float fontHeight = pLabel->getBoundingBox().getMaxY() - pLabel->getBoundingBox().getMinY();

	if (boxSize.width >= fontWidth && boxSize.height >= fontHeight)
	{
		return;
	}
	else
	{
		fontSize--;
		getAutoFontSize(strText, fontName, boxSize, isLineBreakWithoutSpace, fontSize);
	}
}

void WordWindowScene::setGameSpeed(float speed)
{
	Director::getInstance()->getScheduler()->setTimeScale(speed);
}

void WordWindowScene::resetGameSpeed()
{
	Director::getInstance()->getScheduler()->setTimeScale(1.f);
}

void WordWindowScene::playHintSimpleSequenceInclusion()
{
	//CCLOG("[WordWindowScene::playHintSimpleSequenceInclusion]");

	int totalCount = m_curLevelStruct.getTotalObjectCount();

	// 검은색 돌 깐다.
	Node* pNodeBlackStonePack = Node::create();
	m_pNodeSequence->addChild(pNodeBlackStonePack);
	for (int i = 0; i < totalCount; i++)
	{
		Sprite* pSpriteStone = Sprite::create(resourcePath + "wordwindow-stone-black.png");
		Vec2 pos;
		pos.x = 166.f + 217.f + (174.f / 2.f) + ((i % 10) * 180.f);
		pos.y = 578.f + 495.f + (174.f / 2.f) - ((i / 10) * 180.f);
		if (totalCount <= 10)
		{
			pos.y -= 90.f;
		}
		pSpriteStone->setPosition(pos);
		pNodeBlackStonePack->addChild(pSpriteStone);
	}

	// 대기 시간 추가
	m_accumulateDelay += m_stoneIntervalDelay;

	// 나머지는 더하기 약식 시퀀스와 똑같다.
	playHintSimpleSequenceAddition();
}

void WordWindowScene::playHintSequenceInclusion()
{
	//CCLOG("[WordWindowScene::playHintSequenceInclusion]");

	int totalCount = m_curLevelStruct.getTotalObjectCount();

	// 검은색 돌 깐다.
	Node* pNodeBlackStonePack = Node::create();
	m_pNodeSequence->addChild(pNodeBlackStonePack);
	for (int i = 0; i < totalCount; i++)
	{
		Sprite* pSpriteStone = Sprite::create(resourcePath + "wordwindow-stone-black.png");
		Vec2 pos;
		pos.x = 166.f + 217.f + (174.f / 2.f) + ((i % 10) * 180.f);
		pos.y = 578.f + 495.f + (174.f / 2.f) - ((i / 10) * 180.f);
		if (totalCount <= 10)
		{
			pos.y -= 90.f;
		}
		pSpriteStone->setPosition(pos);
		pNodeBlackStonePack->addChild(pSpriteStone);
	}

	for (int i = 0; i < 4; i++)
	{
		// 고양이 손 종류 선택
		selectCatType();

		// 동물 효과음
		playSoundAnimal(i, m_accumulateDelay - m_stoneIntervalDelay);

		int objectCount = m_curLevelStruct.m_objectValue[i];
		if (objectCount == 0)
		{
			continue;
		}
		std::string resourceName = m_curLevelStruct.m_objectSource[i];

		int startIndex = m_accumulateCount;
		int endIndex = m_accumulateCount + objectCount - 1;

		// 손이 중간에 내려가나?
		bool isDown = false;
		if (m_accumulateCount < 10 && (m_accumulateCount + objectCount) > 10)
		{
			isDown = true;
		}

		Vec2 startPos1;
		Vec2 endPos1;
		Vec2 startPos2;
		Vec2 endPos2;

		// 시작 위치
		startPos1.x = 166.f + 217.f + (174.f / 2.f) + ((startIndex % 10) * 180.f);
		startPos1.y = 578.f + 495.f + (174.f / 2.f) - ((startIndex / 10) * 180.f);
		if (totalCount <= 10)
		{
			startPos1.y -= 90.f;
		}

		if (isDown == true)
		{
			// 끝 위치
			endPos1.x = 166.f + 217.f + (174.f / 2.f) + (9 * 180.f);
			endPos1.y = 578.f + 495.f + (174.f / 2.f) - ((startIndex / 10) * 180.f);

			// 다음 라인

			// 시작 위치
			startPos2.x = 166.f + 217.f + (174.f / 2.f);
			startPos2.y = 578.f + 495.f + (174.f / 2.f) - ((endIndex / 10) * 180.f);

			// 끝 위치
			endPos2.x = 166.f + 217.f + (174.f / 2.f) + ((endIndex % 10) * 180.f);
			endPos2.y = 578.f + 495.f + (174.f / 2.f) - ((endIndex / 10) * 180.f);
		}
		else
		{
			// 끝 위치
			endPos1.x = 166.f + 217.f + (174.f / 2.f) + ((endIndex % 10) * 180.f);
			endPos1.y = 578.f + 495.f + (174.f / 2.f) - ((endIndex / 10) * 180.f);
			if (totalCount <= 10)
			{
				endPos1.y -= 90.f;
			}
		}

		// 고양이 손
		Size winSize = getContentSize();
		Sprite* pSpriteCat = Sprite::create(resourcePath + getCatFilename());
		pSpriteCat->setPosition(startPos1 + Vec2(70.f, 440.f));
		// 화면 상단
		float distance = winSize.height - startPos1.y;
		pSpriteCat->setPositionY(pSpriteCat->getPositionY() + distance + 140.f);
		m_pNodeSequence->addChild(pSpriteCat, 10);

		Sequence* sequence = nullptr;
		Sequence* sequence2 = nullptr;

		// 손 내리는 연출
		sequence = Sequence::create(
			DelayTime::create(m_accumulateDelay),
			EaseOut::create(MoveTo::create(m_catDownTime, startPos1 + Vec2(70.f, 440.f) - Vec2(0.f, 50.f)), 3.f),
			MoveTo::create(0.1f, startPos1 + Vec2(70.f, 440.f)),
			nullptr);
		pSpriteCat->runAction(sequence);

		m_accumulateDelay += m_catDownTime;
		m_accumulateDelay += 0.1f;

		if (isDown == true)
		{
			// 현재라인 이동 연출
			int moveCount1 = 9 - startIndex;
			sequence = Sequence::create(
				DelayTime::create(m_accumulateDelay),
				CallFunc::create([this](){ this->playSoundPutObjectRow(); }),
				MoveTo::create(moveCount1 * 0.1f, endPos1 + Vec2(70.f, 440.f)),
				CallFunc::create([this](){ this->stopSoundPutObjectRow(); }),
				nullptr);
			pSpriteCat->runAction(sequence);

			for (int k = 1; k < moveCount1; k++)
			{
				if (k % 2 == 1)
				{
					sequence = Sequence::create(
						DelayTime::create(m_accumulateDelay + (k * 0.1f)),
						MoveBy::create(0.f, Vec2(0.f, -40.f)),
						nullptr);
					pSpriteCat->runAction(sequence);
				}
				else
				{
					sequence = Sequence::create(
						DelayTime::create(m_accumulateDelay + (k * 0.1f)),
						MoveBy::create(0.f, Vec2(0.f, 40.f)),
						nullptr);
					pSpriteCat->runAction(sequence);
				}
			}

			// 현재 라인 돌
			float startDelay1 = m_accumulateDelay;
			for (int k = startIndex, kk = 0; k < 10; k++, kk++)
			{
				Sprite* pSpriteStone = Sprite::create(resourcePath + resourceName);
				Vec2 pos;
				pos.x = 166.f + 217.f + (174.f / 2.f) + ((k % 10) * 180.f);
				pos.y = 578.f + 495.f + (174.f / 2.f) - ((k / 10) * 180.f);
				pSpriteStone->setPosition(pos);
				pSpriteStone->setVisible(false);
				m_pNodeSequence->addChild(pSpriteStone);

				Sequence* sequence = Sequence::create(
					DelayTime::create(startDelay1 + (kk * 0.1f)),
					CallFunc::create([this, pSpriteStone]()
				{
					pSpriteStone->setVisible(true);
				}),
					nullptr);
				pSpriteStone->runAction(sequence);
			}

			m_accumulateDelay += moveCount1 * 0.1f;

			// 손 올라가는 이동
			sequence = Sequence::create(
				DelayTime::create(m_accumulateDelay),
				MoveTo::create(0.1f, endPos1 + Vec2(70.f, 440.f) - Vec2(0.f, 50.f)),
				EaseOut::create(MoveTo::create(m_catUpTime, endPos1 + Vec2(70.f, 440.f) + Vec2(0.f, distance + 140.f)), 3.f),
				MoveTo::create(0.0f, startPos2 + Vec2(70.f, 440.f) + Vec2(0.f, distance + 140.f)),
				nullptr);
			pSpriteCat->runAction(sequence);

			m_accumulateDelay += 0.1f;
			m_accumulateDelay += (m_catUpTime);

			// 약간의 텀을 둔 다음 내려오게 수정
			m_accumulateDelay += 0.1f;

			Sequence* sequence = nullptr;
			Sequence* sequence2 = nullptr;

			// 손 내리는 연출
			sequence = Sequence::create(
				DelayTime::create(m_accumulateDelay),
				EaseOut::create(MoveTo::create(m_catDownTime, startPos2 + Vec2(70.f, 440.f) - Vec2(0.f, 50.f)), 3.f),
				MoveTo::create(0.1f, startPos2 + Vec2(70.f, 440.f)),
				nullptr);
			pSpriteCat->runAction(sequence);

			// 대기시간 추가
			m_accumulateDelay += m_catDownTime;
			m_accumulateDelay += 0.1f;
			m_accumulateDelay += 0.1f;

			// 다음라인 마지막으로 이동
			int moveCount2 = endIndex - 10;
			sequence = Sequence::create(
				DelayTime::create(m_accumulateDelay),
				CallFunc::create([this](){ this->playSoundPutObjectRow(); }),
				MoveTo::create(moveCount2 * 0.1f, endPos2 + Vec2(70.f, 440.f)),
				CallFunc::create([this](){ this->stopSoundPutObjectRow(); }),
				nullptr);
			pSpriteCat->runAction(sequence);

			for (int k = 1; k < moveCount2; k++)
			{
				if (k % 2 == 1)
				{
					sequence = Sequence::create(
						DelayTime::create(m_accumulateDelay + (k * 0.1f)),
						MoveBy::create(0.f, Vec2(0.f, -40.f)),
						nullptr);
					pSpriteCat->runAction(sequence);
				}
				else
				{
					sequence = Sequence::create(
						DelayTime::create(m_accumulateDelay + (k * 0.1f)),
						MoveBy::create(0.f, Vec2(0.f, 40.f)),
						nullptr);
					pSpriteCat->runAction(sequence);
				}
			}

			// 다음 라인 돌
			float startDelay2 = m_accumulateDelay;
			for (int k = 10, kk = 0; k < endIndex + 1; k++, kk++)
			{
				Sprite* pSpriteStone = Sprite::create(resourcePath + resourceName);
				Vec2 pos;
				pos.x = 166.f + 217.f + (174.f / 2.f) + ((k % 10) * 180.f);
				pos.y = 578.f + 495.f + (174.f / 2.f) - ((k / 10) * 180.f);
				pSpriteStone->setPosition(pos);
				pSpriteStone->setVisible(false);
				m_pNodeSequence->addChild(pSpriteStone);

				Sequence* sequence = Sequence::create(
					DelayTime::create(startDelay2 + (kk * 0.1f)),
					CallFunc::create([this, pSpriteStone]()
				{
					pSpriteStone->setVisible(true);
				}),
					nullptr);
				pSpriteStone->runAction(sequence);
			}

			m_accumulateDelay += moveCount2 * 0.1f;
			m_accumulateDelay += 0.1f;

			// 손 올라가는 이동
			sequence = Sequence::create(
				DelayTime::create(m_accumulateDelay),
				MoveTo::create(0.1f, endPos2 + Vec2(70.f, 440.f) - Vec2(0.f, 50.f)),
				EaseOut::create(MoveTo::create(m_catUpTime, endPos2 + Vec2(70.f, 440.f) + Vec2(0.f, distance + 320.f)), 3.f),
				nullptr);
			pSpriteCat->runAction(sequence);

			m_accumulateDelay += 0.1f;
			m_accumulateDelay += m_catUpTime;
		}
		else
		{
			// 현재라인 이동 연출
			int moveCount1 = endIndex - startIndex;
			sequence = Sequence::create(
				DelayTime::create(m_accumulateDelay),
				CallFunc::create([this](){ this->playSoundPutObjectRow(); }),
				MoveTo::create(moveCount1 * 0.1f, endPos1 + Vec2(70.f, 440.f)),
				CallFunc::create([this](){ this->stopSoundPutObjectRow(); }),
				nullptr);
			pSpriteCat->runAction(sequence);

			for (int k = 1; k < moveCount1; k++)
			{
				if (k % 2 == 1)
				{
					sequence = Sequence::create(
						DelayTime::create(m_accumulateDelay + (k * 0.1f)),
						MoveBy::create(0.f, Vec2(0.f, -40.f)),
						nullptr);
					pSpriteCat->runAction(sequence);
				}
				else
				{
					sequence = Sequence::create(
						DelayTime::create(m_accumulateDelay + (k * 0.1f)),
						MoveBy::create(0.f, Vec2(0.f, 40.f)),
						nullptr);
					pSpriteCat->runAction(sequence);
				}
			}

			// 현재 라인 돌
			float startDelay1 = m_accumulateDelay;
			for (int k = startIndex, kk = 0; k < endIndex + 1; k++, kk++)
			{
				Sprite* pSpriteStone = Sprite::create(resourcePath + resourceName);
				Vec2 pos;
				pos.x = 166.f + 217.f + (174.f / 2.f) + ((k % 10) * 180.f);
				pos.y = 578.f + 495.f + (174.f / 2.f) - ((k / 10) * 180.f);
				if (totalCount <= 10)
				{
					pos.y -= 90.f;
				}
				pSpriteStone->setPosition(pos);
				pSpriteStone->setVisible(false);
				m_pNodeSequence->addChild(pSpriteStone);

				Sequence* sequence = Sequence::create(
					DelayTime::create(startDelay1 + (kk * 0.1f)),
					CallFunc::create([this, pSpriteStone]()
				{
					pSpriteStone->setVisible(true);
				}),
					nullptr);
				pSpriteStone->runAction(sequence);
			}

			m_accumulateDelay += moveCount1 * 0.1f;
			m_accumulateDelay += 0.1f;

			// 손 올라가는 이동
			sequence = Sequence::create(
				DelayTime::create(m_accumulateDelay),
				MoveTo::create(0.1f, endPos1 + Vec2(70.f, 440.f) - Vec2(0.f, 50.f)),
				EaseOut::create(MoveTo::create(m_catUpTime, endPos1 + Vec2(70.f, 440.f) + Vec2(0.f, distance + 140.f)), 3.f),
				nullptr);
			pSpriteCat->runAction(sequence);

			m_accumulateDelay += 0.1f;
			m_accumulateDelay += m_catUpTime;
		}

		// 총 갯수 더해준다.
		addAccumulateCount(objectCount);
		
		// 다음 돌 놓는 딜레이 추가
		m_accumulateDelay += m_stoneIntervalDelay;
	}
}

void WordWindowScene::playHintSimpleSequenceEquation()
{
	//CCLOG("[WordWindowScene::playHintSimpleSequenceEquation]");

	int value1 = m_curLevelStruct.m_equationValueA;
	int value2 = m_curLevelStruct.m_equationValueB;
	int correctAnswer = 0;
	string sign = m_curLevelStruct.m_equationValueSign;
	string signResourceName;
	if (sign == "+")
	{
		correctAnswer = value1 + value2;
		signResourceName = "wordwindow-sign-plus.png";
	}
	else if (sign == "-")
	{
		correctAnswer = value1 - value2;
		signResourceName = "wordwindow-sign-substraction.png";
	}
	else if (sign == "*")
	{
		correctAnswer = value1 * value2;
		signResourceName = "wordwindow-sign-multiplication.png";
	}
	else if (sign == "/")
	{
		correctAnswer = value1 / value2;
		signResourceName = "wordwindow-sign-division.png";
	}

	int unknownIndex = 0;
	int unknownValue = 0;
	if (m_curLevelStruct.m_equationUnknown == "A")
	{
		unknownIndex = 0;
		unknownValue = value1;
	}
	else if (m_curLevelStruct.m_equationUnknown == "B")
	{
		unknownIndex = 2;
		unknownValue = value2;
	}
	else if (m_curLevelStruct.m_equationUnknown == "C")
	{
		unknownIndex = 4;
		unknownValue = correctAnswer;
	}

	Size winSize = getContentSize();
	Vec2 startPos;
	startPos.x = 1280.f - (2228.f / 2.f) + 155.f + (366.f / 2.f);
	startPos.y = winSize.height - 238.f - (984.f / 2.f);
	float gapY = 20.f;

	// 카드 5장
	Sprite* pSpriteUnknwonCard = nullptr;

	for (int i = 0; i < 5; i++)
	{
		Sprite* pSpriteCard = nullptr;
		if (i == unknownIndex)
		{
			pSpriteCard = Sprite::create(resourcePath + "wordwindow-equationcard-x.png");
			pSpriteUnknwonCard = pSpriteCard;
		}
		else if (i == 1 || i == 3)
		{
			pSpriteCard = Sprite::create(resourcePath + "wordwindow-equationcard-symbol.png");
		}
		else
		{
			pSpriteCard = Sprite::create(resourcePath + "wordwindow-equationcard-number.png");
		}
		pSpriteCard->setPosition(startPos + Vec2(388.f * i, 0.f));
		pSpriteCard->setVisible(false);
		m_pNodeSequence->addChild(pSpriteCard);

		Sequence* sequence = Sequence::create(
			DelayTime::create(m_accumulateDelay),
			CallFunc::create([this, pSpriteCard]()
		{ 
			pSpriteCard->setVisible(true); 

			// 효과음
			playSoundAddCard();
		}),
			nullptr);
		pSpriteCard->runAction(sequence);

		// 대기시간 추가
		m_accumulateDelay += m_stoneSimpleDelay;

		if (i == unknownIndex)
		{
			Sprite* pSpriteQuestion = Sprite::create(resourcePath + "wordwindow-sign-question.png");
			pSpriteQuestion->setPosition(Vec2(366.f / 2.f, 496.f / 2.f));
			pSpriteQuestion->setTag(10);
			pSpriteCard->addChild(pSpriteQuestion, 5);
		}
		else if (i == 1)
		{
			Sprite* pSpriteSign = Sprite::create(resourcePath + signResourceName);
			pSpriteSign->setPosition(Vec2(366.f / 2.f, 496.f / 2.f));
			pSpriteCard->addChild(pSpriteSign, 5);
		}
		else if (i == 3)
		{
			Sprite* pSpriteEqual = Sprite::create(resourcePath + "wordwindow-sign-equal.png");
			pSpriteEqual->setPosition(Vec2(366.f / 2.f, 496.f / 2.f));
			pSpriteCard->addChild(pSpriteEqual, 5);
		}
		else
		{
			int count = 0;
			if (i == 0)
			{
				count = value1;
			}
			else if (i == 2)
			{
				count = value2;
			}
			else if (i == 4)
			{
				count = correctAnswer;
			}

			if (count == 1)
			{
				Sprite* pSpriteStone = Sprite::create(resourcePath + getStoneName());
				pSpriteStone->setScale(0.45f);
				pSpriteStone->setPosition(Vec2(366.f / 2.f, (496.f / 2.f)));
				pSpriteCard->addChild(pSpriteStone);
			}
			else
			{
				Vec2 stonePos = Vec2((366.f / 2.f) - 40.f, (496.f / 2.f));

				int lineCount = count / 2;
				if ((count % 2) == 1)
				{
					lineCount += 1;
				}

				// 짝수줄
				bool isPairY = false;
				if ((lineCount % 2) == 0)
				{
					isPairY = true;
				}
				stonePos.y += ((lineCount / 2) * 80.f);
				if (isPairY == true)
				{
					stonePos.y -= 40.f;
				}

				float startY = stonePos.y;

				for (int k = 0; k < count; k++)
				{
					float y = startY - ((k / 2) * 80.f);
					Sprite* pSpriteStone = Sprite::create(resourcePath + getStoneName());
					pSpriteStone->setScale(0.45f);
					pSpriteStone->setPosition((366.f / 2.f) - 40.f + (k % 2) * 80.f, y);
					pSpriteCard->addChild(pSpriteStone);
				}
			}
		}
	}

	m_accumulateDelay += m_stoneIntervalDelay;

	// 정답 카드 뒤집히는 연출
	Sequence* sequence = Sequence::create(
		DelayTime::create(m_accumulateDelay),
		CallFunc::create([this, unknownValue, pSpriteUnknwonCard]()
	{
		// X 글씨 숨긴다.
		pSpriteUnknwonCard->getChildByTag(10)->setVisible(false);

		// 정답 돌 추가
		addStoneAtCard(pSpriteUnknwonCard, unknownValue);

		// 카드 뒤집어 지는 연출
		pSpriteUnknwonCard->setScaleX(0.125f);
		pSpriteUnknwonCard->runAction(ScaleTo::create(m_equationCardFlipTime, 1.f, 1.f));

		// 효과음
		playSoundFlipCard();
	}),
		nullptr);
	pSpriteUnknwonCard->runAction(sequence);
}

void WordWindowScene::playHintSequenceEquation()
{
	//CCLOG("[WordWindowScene::playHintSequenceEquation]");

	int value1 = m_curLevelStruct.m_equationValueA;
	int value2 = m_curLevelStruct.m_equationValueB;
	int correctAnswer = 0;
	string sign = m_curLevelStruct.m_equationValueSign;
	string signResourceName;
	if (sign == "+")
	{
		correctAnswer = value1 + value2;
		signResourceName = "wordwindow-sign-plus.png";
	}
	else if (sign == "-")
	{
		correctAnswer = value1 - value2;
		signResourceName = "wordwindow-sign-substraction.png";
	}
	else if (sign == "*")
	{
		correctAnswer = value1 * value2;
		signResourceName = "wordwindow-sign-multiplication.png";
	}
	else if (sign == "/")
	{
		correctAnswer = value1 / value2;
		signResourceName = "wordwindow-sign-division.png";
	}

	int unknwonIndex = 0;
	int unknwonValue = 0;
	if (m_curLevelStruct.m_equationUnknown == "A")
	{
		unknwonIndex = 0;
		unknwonValue = value1;
	}
	else if (m_curLevelStruct.m_equationUnknown == "B")
	{
		unknwonIndex = 2;
		unknwonValue = value2;
	}
	else if (m_curLevelStruct.m_equationUnknown == "C")
	{
		unknwonIndex = 4;
		unknwonValue = correctAnswer;
	}

	Size winSize = getContentSize();
	Vec2 startPos;
	startPos.x = 1280.f - (2228.f / 2.f) + 155.f + (366.f / 2.f);
	startPos.y = winSize.height - 238.f - (984.f / 2.f);
	float gapY = 20.f;

	selectCatType();

	// 동물 효과음
	playSoundAnimal(0, m_accumulateDelay - m_stoneIntervalDelay);

	Sprite* pSpriteUnknwonCard = nullptr;

	// 카드 5장
	for (int i = 0; i < 5; i++)
	{
		Sprite* pSpriteCard = nullptr;
		if (i == unknwonIndex)
		{
			pSpriteCard = Sprite::create(resourcePath + "wordwindow-equationcard-x.png");
			pSpriteUnknwonCard = pSpriteCard;
		}
		else if (i == 1 || i == 3)
		{
			pSpriteCard = Sprite::create(resourcePath + "wordwindow-equationcard-symbol.png");
		}
		else
		{
			pSpriteCard = Sprite::create(resourcePath + "wordwindow-equationcard-number.png");
		}
		Vec2 topPos = startPos + Vec2(388.f * i, 1000.f);
		Vec2 cardPos = startPos + Vec2(388.f * i, 0.f);
		pSpriteCard->setPosition(topPos);
		m_pNodeSequence->addChild(pSpriteCard);

		// 고양이손
		Sprite* pSpriteCat = Sprite::create(resourcePath + getCatFilename());
		pSpriteCat->setPosition(Vec2(250.f, 680.f));
		pSpriteCard->addChild(pSpriteCat, 10);

		// 연출
		Sequence* sequence = Sequence::create(
			DelayTime::create(m_accumulateDelay),
			EaseOut::create(MoveTo::create(m_catDownTime, cardPos), 3.f),
			CallFunc::create([this, pSpriteCat]()
		{ 
			pSpriteCat->runAction(MoveTo::create(m_catUpTime, Vec2(250.f, 1680.f)));

			// 효과음
			playSoundAddCard();
		}),
			nullptr);
		pSpriteCard->runAction(sequence);

		// 대기시간 추가
		m_accumulateDelay += m_catDownTime;
		m_accumulateDelay += m_catUpTime;
		m_accumulateDelay += m_stoneDelay;

		if (i == unknwonIndex)
		{
			Sprite* pSpriteQuestion = Sprite::create(resourcePath + "wordwindow-sign-question.png");
			pSpriteQuestion->setPosition(Vec2(366.f / 2.f, 496.f / 2.f));
			pSpriteQuestion->setTag(10);
			pSpriteCard->addChild(pSpriteQuestion, 5);
		}
		else if (i == 1)
		{
			Sprite* pSpriteSign = Sprite::create(resourcePath + signResourceName);
			pSpriteSign->setPosition(Vec2(366.f / 2.f, 496.f / 2.f));
			pSpriteSign->setTag(10);
			pSpriteCard->addChild(pSpriteSign, 5);
		}
		else if (i == 3)
		{
			Sprite* pSpriteEqual = Sprite::create(resourcePath + "wordwindow-sign-equal.png");
			pSpriteEqual->setPosition(Vec2(366.f / 2.f, 496.f / 2.f));
			pSpriteEqual->setTag(10);
			pSpriteCard->addChild(pSpriteEqual, 5);
		}
		else
		{
			int count = 0;
			if (i == 0)
			{
				count = value1;
			}
			else if (i == 2)
			{
				count = value2;
			}
			else if (i == 4)
			{
				count = correctAnswer;
			}

			addStoneAtCard(pSpriteCard, count);
		}
	}

	m_accumulateDelay += m_stoneIntervalDelay;

	// 고양이 손 선택
	selectCatType();

	// 동물 효과음
	playSoundAnimal(1, m_accumulateDelay - m_stoneIntervalDelay);

	// 정답 뒤집는 연출
	if (pSpriteUnknwonCard != nullptr)
	{
		// 고양이손
		Sprite* pSpriteCat = Sprite::create(resourcePath + getCatFilename());
		//pSpriteCat->setPosition(Vec2(250.f, 680.f));
		Vec2 catPosTop = startPos + Vec2(388.f * unknwonIndex, 0.f) + Vec2(250.f, 1680.f) - Vec2(366.f / 2.f, 496.f / 2.f);
		Vec2 catPos = startPos + Vec2(388.f * unknwonIndex, 0.f) + Vec2(250.f, 680.f) - Vec2(366.f / 2.f, 496.f / 2.f);
		pSpriteCat->setPosition(catPosTop);
		m_pNodeSequence->addChild(pSpriteCat, 10);

		Node* pChild = pSpriteUnknwonCard->getChildByTag(10);

		// 고양이 손 내려가는 연출
		Sequence* sequence = Sequence::create(
			DelayTime::create(m_accumulateDelay),
			EaseOut::create(MoveTo::create(m_catDownTime, catPos), 3.f),
			CallFunc::create([this, unknwonValue, pSpriteUnknwonCard, pSpriteCat, catPosTop, pChild]()
		{
			// 고양이 손 올라가는 연출
			pSpriteCat->runAction(MoveTo::create(m_catUpTime, catPosTop));
			
			// X 글씨 숨긴다.
			pChild->setVisible(false);

			// 정답 돌 추가
			addStoneAtCard(pSpriteUnknwonCard, unknwonValue);

			// 효과음
			playSoundFlipCard();

			// 카드 뒤집어 지는 연출
			pSpriteUnknwonCard->setScaleX(0.125f);
			pSpriteUnknwonCard->runAction(ScaleTo::create(m_equationCardFlipTime, 1.f, 1.f));
		}),
			nullptr);
		pSpriteCat->runAction(sequence);

		m_accumulateDelay += m_catDownTime;
		m_accumulateDelay += m_catUpTime;
		m_accumulateDelay += m_equationCardFlipTime;
	}

	m_accumulateDelay += m_stoneIntervalDelay;
}

void WordWindowScene::addStoneAtCard(Sprite* pSpriteCard, int count)
{
	if (count == 1)
	{
		Sprite* pSpriteStone = Sprite::create(resourcePath + getStoneName());
		pSpriteStone->setScale(0.45f);
		pSpriteStone->setPosition(Vec2(366.f / 2.f, (496.f / 2.f)));
		pSpriteCard->addChild(pSpriteStone);
	}
	else
	{
		Vec2 stonePos = Vec2((366.f / 2.f) - 40.f, (496.f / 2.f));

		int lineCount = count / 2;
		if ((count % 2) == 1)
		{
			lineCount += 1;
		}

		// 짝수줄
		bool isPairY = false;
		if ((lineCount % 2) == 0)
		{
			isPairY = true;
		}
		stonePos.y += ((lineCount / 2) * 80.f);
		if (isPairY == true)
		{
			stonePos.y -= 40.f;
		}

		float startY = stonePos.y;

		for (int k = 0; k < count; k++)
		{
			float y = startY - ((k / 2) * 80.f);
			Sprite* pSpriteStone = Sprite::create(resourcePath + getStoneName());
			pSpriteStone->setScale(0.45f);
			pSpriteStone->setPosition((366.f / 2.f) - 40.f + (k % 2) * 80.f, y);
			pSpriteCard->addChild(pSpriteStone);
		}
	}
}

void WordWindowScene::selectStone()
{
	m_selectedStone = cocos2d::random() % 3;
}

std::string WordWindowScene::getStoneName()
{
	switch (m_selectedStone)
	{
	case 0:
		return "wordwindow-stone-red.png";
	case 1:
		return "wordwindow-stone-green.png";
	case 2:
		return "wordwindow-stone-blue.png";
	}

	return "wordwindow-stone-green.png";
}