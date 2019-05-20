//
//  PlaceValueScene.cpp
//
//  Created by harunom on 8/31/18.
//
//


#include "PlaceValueScene.hpp"
#include "Common/Basic/SoundEffect.h"
#include "Common/Sounds/CommonSound.hpp"

#include <string>
#include <vector>
#include <numeric>
#include <algorithm>
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



namespace PlaceValueSceneSpace
{
    const char* defaultFont = "fonts/chanakya.ttf";
    const char* andikaFont = "fonts/mukta-bold.ttf";
    const char* kitkitNumbersFont = "fonts/kitkitnumbers.ttf";
    
    const string resourcePath = "PlaceValue/";
    
    const Size gameSize = Size(2560, 1800);
    
    const Color3B numberColor = Color3B(255, 216, 0);
    const Color3B symbolColor = Color3B(0, 181, 8);
    
    const CommonSound CommonSoundPlayer;
    
    SoundEffect pickEffect() { return SoundEffect("Common/Sounds/Effect/matrix_clickblock.m4a"); }
    SoundEffect correctEffect() { return SoundEffect("Common/Sounds/Effect/SFX_Wood_Correct.m4a"); }
    SoundEffect incorrectEffect() { return SoundEffect("Common/Sounds/Effect/SFX_Wood_InCorrect.m4a"); }
    SoundEffect solveEffect() { return SoundEffect("Common/Sounds/Effect/SFX_Counting_Win.m4a"); }
    SoundEffect layEffect() { return SoundEffect("Common/Sounds/Effect/matrix_wrongmove.m4a"); }
    SoundEffect successEffect() { return SoundEffect("Common/Sounds/Effect/success.m4a"); }
}

using namespace PlaceValueSceneSpace;


Scene* PlaceValueScene::createScene(string levelID)
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = PlaceValueScene::create();
    layer->setLevel(TodoUtil::stoi(levelID));
    
    // add layer as a child to scene
    scene->addChild(layer);
    
    // return the scene
    return scene;
}

bool PlaceValueScene::init()
{
    if (!Layer::init())
    {
        return false;
    }
    
    this->scheduleUpdate();
    
    pickEffect().preload();
    correctEffect().preload();
    incorrectEffect().preload();
    solveEffect().preload();
    layEffect().preload();
    successEffect().preload();
    
    auto winSize = getContentSize();
    auto gameScale = winSize.height / gameSize.height;
    
    //하늘
    auto bg = Sprite::create(resourcePath+"bg-sky.png");
    bg->setName("bg");
    bg->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
    bg->setPosition(Vec2(winSize.width/2, 0));
    bg->setVisible(false);
    addChild(bg);

    //구름 위치 잡자
    auto cloud1 = Sprite::create(resourcePath+"image-cloud-01.png");
    auto cloud2 = Sprite::create(resourcePath+"image-cloud-02.png");
    auto cloud3 = Sprite::create(resourcePath+"image-cloud-03.png");
    cloud1_pos = Vec2(gameSize.width - (cloud1->getContentSize().width/2), gameSize.height - (cloud1->getContentSize().height/2)-40);
    cloud2_pos = Vec2(140, gameSize.height - cloud2->getContentSize().height);
    cloud3_pos = Vec2(gameSize.width/2 - cloud3->getContentSize().width, gameSize.height-40);
    
    //게임노드
    _gameNode = Node::create();
    _gameNode->setContentSize(gameSize);
    _gameNode->setScale(gameScale);
    _gameNode->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    _gameNode->setPosition(winSize.width / 2, winSize.height / 2);
    addChild(_gameNode);

    //스프라이트 이미지 미리로딩
    Sprite::create(resourcePath+"image-land.png");
    Sprite::create(resourcePath+"bg-bush.png");
    Sprite::create(resourcePath+"image-cloud-01.png");
    Sprite::create(resourcePath+"image-cloud-02.png");
    Sprite::create(resourcePath+"image-cloud-03.png");
    Sprite::create(resourcePath+"image-water-tank-hundreds.png");
    Sprite::create(resourcePath+"image-hundreds-text-bg.png");
    Sprite::create(resourcePath+"image-water-tank-hundreds-effect.png");
    Sprite::create(resourcePath+"image-water-tank-tens.png");
    Sprite::create(resourcePath+"image-tens-text-bg.png");
    Sprite::create(resourcePath+"image-water-tank-tens-effect.png");
    Sprite::create(resourcePath+"image-water-tank-ones.png");
    Sprite::create(resourcePath+"image-ones-text-bg.png");
    Sprite::create(resourcePath+"image-water-tank-ones-effect.png");
    Sprite::create(resourcePath+"image-pipe-01.png");
    Sprite::create(resourcePath+"image-pipe-02.png");
    Sprite::create(resourcePath+"image-answer-text-bg.png");
    
    Sprite::create(resourcePath+"button-drop-on-land-hundred.png");
    Sprite::create(resourcePath+"button-drop-on-land-hundred-shadow.png");
    Sprite::create(resourcePath+"button-drop-on-land-ten.png");
    Sprite::create(resourcePath+"button-drop-on-land-ten-shadow.png");
    Sprite::create(resourcePath+"button-drop-on-land-one.png");
    Sprite::create(resourcePath+"button-drop-on-land-one-shadow.png");
    Sprite::create(resourcePath+"button-drop-in-tank-hundred.png");
    Sprite::create(resourcePath+"button-drop-in-tank-hundred-shadow.png");
    Sprite::create(resourcePath+"button-drop-in-tank-ten.png");
    Sprite::create(resourcePath+"button-drop-in-tank-ten-shadow.png");
    Sprite::create(resourcePath+"button-drop-in-tank-one.png");
    Sprite::create(resourcePath+"button-drop-in-tank-one-shadow.png");
    Sprite::create(resourcePath+"image-hundred-indication-text-bg.png");
    Sprite::create(resourcePath+"image-ten-indication-text-bg.png");
    Sprite::create(resourcePath+"image-one-indication-text-bg.png");
    
    //공용파티클 미리로딩
    createShiningParticle(Vec2(140, 80), 50);
    createShiningParticle(Vec2(120, 80), 50);
    createShiningParticle(Vec2(80, 80), 50);
    createShiningParticle(Vec2(240, 140), 150);


    auto backButton = TodoSchoolBackButton::create();
    backButton->setName("backButton");
    backButton->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
    backButton->setPosition(Vec2(25, winSize.height-25));
    backButton->setVisible(false);
    addChild(backButton);
    
    
    _progressBar = ProgressIndicator::create();
    _progressBar->setPosition(Vec2(winSize.width/2, winSize.height - _progressBar->getContentSize().height));
    addChild(_progressBar);
    
    if (UserManager::getInstance()->isDebugMode()) {
        auto skip = Button::create();
        skip->setName("skip");
        skip->setTitleFontSize(100);
        skip->setTitleText("Skip");
        skip->setAnchorPoint(Vec2::ANCHOR_TOP_RIGHT);
        skip->setPosition(Vec2(winSize.width-25, winSize.height-25));
        skip->setVisible(false);
        addChild(skip);
        skip->addTouchEventListener([this](Ref*,Widget::TouchEventType e) {
            if (e == Widget::TouchEventType::ENDED)
            {
                if (_isSkip == false)
                {
                    _isSkip = true;
                    
                    setTank100Count(_objectNo / 100);
                    setTank10Count((_objectNo % 100) / 10);
                    setTank1Count(((_objectNo % 100) % 10) / 1);
                    
                    onStageClear();
                    
                }
            }
        });
        
    }

    loadDurationsheet();
    
    return true;
}

void PlaceValueScene::onEnter()
{
    Layer::onEnter();
}

void PlaceValueScene::onEnterTransitionDidFinish()
{

    onStart();

}

void PlaceValueScene::onStart()
{
    loadData(_currentLevel, &_currentSheetNo);
    _currentBoardIndex = 0;
    
    _progressBar->setMax(_boards.size());
    
    showBoard(_currentBoardIndex);
}


void PlaceValueScene::showBoard(int index)
{
    _progressBar->setCurrent(index+1, false);
    createBoard(index);
    
}

void PlaceValueScene::createBoard(int index)
{
    _currentPickedTankPiece = nullptr;
    _currentPickedLandPiece = nullptr;
    
    _gameNode->removeAllChildren();
    
    _tank100Pieces.clear();
    _tank10Pieces.clear();
    _tank1Pieces.clear();
    
    //데이터 읽어오기
    auto info = _boards.at(index);
    _objectNo = info.objectNo;
    _suggestNo = info.suggestNo;
    
    _tank100Count = _suggestNo / 100;
    _tank10Count = (_suggestNo % 100) / 10;
    _tank1Count = ((_suggestNo % 100) % 10) / 1;
    
    auto tank100_title_text = (LanguageManager::getInstance()->isEnglish() ? "Hundreds" : "Mamia");
    auto tank10_title_text = (LanguageManager::getInstance()->isEnglish() ? "Tens" : "Makumi");
    auto tank1_title_text = (LanguageManager::getInstance()->isEnglish() ? "Ones" : "Mamoja");

    //공용파티클 미리로딩
    _tank100Particle = createShiningParticle(Vec2(140, 80), 50);
    _tank10Particle = createShiningParticle(Vec2(120, 80), 50);
    _tank1Particle = createShiningParticle(Vec2(80, 80), 50);
    _answerParticle = createShiningParticle(Vec2(240, 140), 150);
    
    //땅
    auto land = Sprite::create(resourcePath+"image-land.png");
    land->setName("land");
    land->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
    land->setPosition(Vec2(gameSize.width/2, 446));
    _gameNode->addChild(land, 100);
    
    //풀
    auto brush = Sprite::create(resourcePath+"bg-bush.png");
    brush->setName("brush1");
    brush->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    brush->setPosition(Vec2(0, 446));
    _gameNode->addChild(brush);
    
    auto brush2 = Sprite::createWithTexture(brush->getTexture());
    brush2->setName("brush2");
    brush2->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    brush2->setPosition(Vec2(gameSize.width, 446));
    _gameNode->addChild(brush2);

    //구름
    auto cloud1 = Sprite::create(resourcePath+"image-cloud-01.png");
    cloud1->setName("cloud1");
    cloud1->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    cloud1->setPosition(cloud1_pos);
    _gameNode->addChild(cloud1);
    
    auto cloud2 = Sprite::create(resourcePath+"image-cloud-02.png");
    cloud2->setName("cloud2");
    cloud2->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    cloud2->setPosition(cloud2_pos);
    _gameNode->addChild(cloud2);
    
    auto cloud3 = Sprite::create(resourcePath+"image-cloud-03.png");
    cloud3->setName("cloud3");
    cloud3->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    cloud3->setPosition(cloud3_pos);
    _gameNode->addChild(cloud3);
    
    //물탱크 100
    auto tank100 = Sprite::create(resourcePath+"image-water-tank-hundreds.png");
    tank100->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
    tank100->setPosition(Vec2(gameSize.width/2 - 540, gameSize.height - 275));
    tank100->setName("tank100");
    _gameNode->addChild(tank100, 90);
    
    auto tank100_title = Label::createWithTTF(tank100_title_text, andikaFont, 50);
    tank100_title->setColor(Color3B(26, 26, 26));
    tank100_title->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
    tank100_title->setPosition(Vec2(tank100->getPosition().x, gameSize.height - 491 + 30));
    tank100_title->setName("tank100Title");
    _gameNode->addChild(tank100_title, 100);
    
    auto hundredsTextBG = Sprite::create(resourcePath+"image-hundreds-text-bg.png");
    hundredsTextBG->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
    hundredsTextBG->setPosition(Vec2(tank100->getPosition().x, gameSize.height - 600));
    hundredsTextBG->setName("tank100Text");
    _gameNode->addChild(hundredsTextBG, 310);
    
    //파티클
    _tank100Particle->setPosition(Vec2(tank100->getPosition().x, gameSize.height - 600 - (hundredsTextBG->getContentSize().height/2)));
    _gameNode->addChild(_tank100Particle, 95);
    
    auto hundredsText1 = Label::createWithTTF(TodoUtil::itos(_tank100Count), kitkitNumbersFont, 75);
    hundredsText1->setColor(Color3B(255, 197, 61));
    hundredsText1->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    hundredsText1->setPosition(Vec2(31 + (hundredsText1->getContentSize().width/2), hundredsTextBG->getContentSize().height/2));
    hundredsText1->setName("text");
    hundredsTextBG->addChild(hundredsText1);

    auto hundredsText2 = Label::createWithTTF("0", kitkitNumbersFont, 75);
    hundredsText2->setOpacity(255 * 0.5);
    hundredsText2->setColor(Color3B(255, 197, 61));
    hundredsText2->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    hundredsText2->setPosition(Vec2(81 + (hundredsText2->getContentSize().width/2), hundredsTextBG->getContentSize().height/2));
    hundredsText2->setName("text2");
    hundredsTextBG->addChild(hundredsText2);

    auto hundredsText3 = Label::createWithTTF("0", kitkitNumbersFont, 75);
    hundredsText3->setOpacity(255 * 0.5);
    hundredsText3->setColor(Color3B(255, 197, 61));
    hundredsText3->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    hundredsText3->setPosition(Vec2(132 + (hundredsText3->getContentSize().width/2), hundredsTextBG->getContentSize().height/2));
    hundredsText3->setName("text3");
    hundredsTextBG->addChild(hundredsText3);

    //물탱크 100 하이라이트
    auto tank100_effect = Sprite::create(resourcePath+"image-water-tank-hundreds-effect.png");
    tank100_effect->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
    tank100_effect->setPosition(Vec2(tank100->getPosition().x, gameSize.height - 656));
    tank100_effect->setName("tank100_effect");
    _gameNode->addChild(tank100_effect, 100);
    
    tank100_effect->setVisible(false);



    //물탱크 10
    auto tank10 = Sprite::create(resourcePath+"image-water-tank-tens.png");
    tank10->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
    tank10->setPosition(Vec2(tank100->getPosition().x + (tank100->getContentSize().width/2) + (tank10->getContentSize().width/2), gameSize.height - 275));
    tank10->setName("tank10");
    _gameNode->addChild(tank10, 90);

    auto tank10_title = Label::createWithTTF(tank10_title_text, andikaFont, 50);
    tank10_title->setColor(Color3B(26, 26, 26));
    tank10_title->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
    tank10_title->setPosition(Vec2(tank10->getPosition().x, gameSize.height - 491 + 30));
    tank10_title->setName("tank10Title");
    _gameNode->addChild(tank10_title, 100);
    
    auto tensTextBG = Sprite::create(resourcePath+"image-tens-text-bg.png");
    tensTextBG->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
    tensTextBG->setPosition(Vec2(tank10->getPosition().x, gameSize.height - 600));
    tensTextBG->setName("tank10Text");
    _gameNode->addChild(tensTextBG, 310);
    
    //파티클
    _tank10Particle->setPosition(Vec2(tank10->getPosition().x, gameSize.height - 600 - (tensTextBG->getContentSize().height/2)));
    _gameNode->addChild(_tank10Particle, 95);
    
    auto tensText1 = Label::createWithTTF(TodoUtil::itos(_tank10Count), kitkitNumbersFont, 75);
    tensText1->setColor(Color3B(194, 255, 61));
    tensText1->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    tensText1->setPosition(Vec2(37 + (tensText1->getContentSize().width/2), tensTextBG->getContentSize().height/2));
    tensText1->setName("text");
    tensTextBG->addChild(tensText1);
    
    auto tensText2 = Label::createWithTTF("0", kitkitNumbersFont, 75);
    tensText2->setOpacity(255 * 0.5);
    tensText2->setColor(Color3B(194, 255, 61));
    tensText2->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    tensText2->setPosition(Vec2(86 + (tensText2->getContentSize().width/2), tensTextBG->getContentSize().height/2));
    tensText2->setName("text2");
    tensTextBG->addChild(tensText2);
    
    //물탱크 10 하이라이트
    auto tank10_effect = Sprite::create(resourcePath+"image-water-tank-tens-effect.png");
    tank10_effect->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
    tank10_effect->setPosition(Vec2(tank10->getPosition().x, gameSize.height - 656));
    tank10_effect->setName("tank10_effect");
    _gameNode->addChild(tank10_effect, 100);
    
    tank10_effect->setVisible(false);

    

    //물탱크 1
    auto tank1 = Sprite::create(resourcePath+"image-water-tank-ones.png");
    tank1->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
    tank1->setPosition(Vec2(tank10->getPosition().x + (tank10->getContentSize().width/2) + (tank1->getContentSize().width/2), gameSize.height - 275));
    tank1->setName("tank1");
    _gameNode->addChild(tank1, 90);
    
    auto tank1_title = Label::createWithTTF(tank1_title_text, andikaFont, 50);
    tank1_title->setColor(Color3B(26, 26, 26));
    tank1_title->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
    tank1_title->setPosition(Vec2(tank1->getPosition().x, gameSize.height - 491 + 30));
    tank1_title->setName("tank1Title");
    _gameNode->addChild(tank1_title, 100);
    
    auto onesTextBG = Sprite::create(resourcePath+"image-ones-text-bg.png");
    onesTextBG->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
    onesTextBG->setPosition(Vec2(tank1->getPosition().x, gameSize.height - 600));
    onesTextBG->setName("tank1Text");
    _gameNode->addChild(onesTextBG, 310);
    
    //파티클
    _tank1Particle->setPosition(Vec2(tank1->getPosition().x, gameSize.height - 600 - (onesTextBG->getContentSize().height/2)));
    _gameNode->addChild(_tank1Particle, 95);
    
    auto onesText1 = Label::createWithTTF(TodoUtil::itos(_tank1Count), kitkitNumbersFont, 75);
    onesText1->setColor(Color3B(200, 61, 255));
    onesText1->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    onesText1->setPosition(Vec2(onesTextBG->getContentSize().width/2, onesTextBG->getContentSize().height/2));
    onesText1->setName("text");
    onesTextBG->addChild(onesText1);

    //물탱크 1 하이라이트
    auto tank1_effect = Sprite::create(resourcePath+"image-water-tank-ones-effect.png");
    tank1_effect->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
    tank1_effect->setPosition(Vec2(tank1->getPosition().x, gameSize.height - 656));
    tank1_effect->setName("tank1_effect");
    _gameNode->addChild(tank1_effect, 100);
    
    tank1_effect->setVisible(false);



    //긴 파이프
    auto pipe = Sprite::create(resourcePath+"image-pipe-01.png");
    pipe->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
    pipe->setPosition(Vec2(gameSize.width/2, gameSize.height - 275));
    pipe->setName("pipe");
    _gameNode->addChild(pipe);

    //메인 파이프
    auto answerPipe = Sprite::create(resourcePath+"image-pipe-02.png");
    answerPipe->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    answerPipe->setPosition(Vec2(answerPipe->getContentSize().width/2 + 10, gameSize.height - 275 - (pipe->getContentSize().height/2)));
    answerPipe->setName("answerPipe");
    _gameNode->addChild(answerPipe, 290);

    //파이프 글자BG, 글자
    auto answerBG = Sprite::create(resourcePath+"image-answer-text-bg.png");
    answerBG->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    answerBG->setPosition(answerPipe->getPosition());
    answerBG->setName("answer");
    _gameNode->addChild(answerBG, 300);
    
    //파티클
    _answerParticle->setPosition(answerPipe->getPosition());
    _gameNode->addChild(_answerParticle, 295);
    
    auto answer1 = Label::createWithTTF(TodoUtil::itos(_objectNo/100), kitkitNumbersFont, 160);
    answer1->setColor(Color3B(191, 191, 191));
    answer1->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    answer1->setPosition(Vec2(33 + (answer1->getContentSize().width/2), answerBG->getContentSize().height/2));
    answer1->setName("text1");
    if (TodoUtil::stoi(answer1->getString()) == 0)
        answer1->setVisible(false);
    answerBG->addChild(answer1);
    
    auto answer2 = Label::createWithTTF(TodoUtil::itos((_objectNo%100)/10), kitkitNumbersFont, 160);
    answer2->setColor(Color3B(191, 191, 191));
    answer2->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    answer2->setPosition(Vec2(33 + 90 + (answer2->getContentSize().width/2), answerBG->getContentSize().height/2));
    answer2->setName("text2");
    if (TodoUtil::stoi(answer1->getString()) == 0 && TodoUtil::stoi(answer2->getString()) == 0)
        answer2->setVisible(false);
    answerBG->addChild(answer2);

    auto answer3 = Label::createWithTTF(TodoUtil::itos((_objectNo%100)%10), kitkitNumbersFont, 160);
    answer3->setColor(Color3B(191, 191, 191));
    answer3->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    answer3->setPosition(Vec2(33 + 90 + 90 + (answer3->getContentSize().width/2), answerBG->getContentSize().height/2));
    answer3->setName("text3");
    answerBG->addChild(answer3);

    //퀴즈 완료 후 스크롤될 아이
    //땅
    auto nextLand = Sprite::create(resourcePath+"image-land.png");
    nextLand->setName("nextLand");
    nextLand->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
    nextLand->setPosition(Vec2(getContentSize().width + gameSize.width/2, 446));
    _gameNode->addChild(nextLand, 99);
    
    //풀
    auto nextBrush = Sprite::create(resourcePath+"bg-bush.png");
    nextBrush->setName("nextBrush1");
    nextBrush->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    nextBrush->setPosition(Vec2(getContentSize().width + 0, 446));
    _gameNode->addChild(nextBrush);
    
    auto nextBrush2 = Sprite::createWithTexture(brush->getTexture());
    nextBrush2->setName("nextBrush2");
    nextBrush2->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    nextBrush2->setPosition(Vec2(getContentSize().width + gameSize.width, 446));
    _gameNode->addChild(nextBrush2);

    //물탱크 100
    auto nextTank100 = Sprite::create(resourcePath+"image-water-tank-hundreds.png");
    nextTank100->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
    nextTank100->setPosition(Vec2(getContentSize().width + gameSize.width/2 - 540, gameSize.height - 275));
    nextTank100->setName("nextTank100");
    _gameNode->addChild(nextTank100, 90);
    
    auto nextTank100_title = Label::createWithTTF(tank100_title_text, andikaFont, 50);
    nextTank100_title->setColor(Color3B(26, 26, 26));
    nextTank100_title->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
    nextTank100_title->setPosition(Vec2(nextTank100->getPosition().x, gameSize.height - 491 + 30));
    nextTank100_title->setName("nextTank100Title");
    _gameNode->addChild(nextTank100_title, 100);

    auto nextHundredsTextBG = Sprite::create(resourcePath+"image-hundreds-text-bg.png");
    nextHundredsTextBG->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
    nextHundredsTextBG->setPosition(Vec2(nextTank100->getPosition().x, gameSize.height - 600));
    nextHundredsTextBG->setName("nextTank100Text");
    _gameNode->addChild(nextHundredsTextBG, 100);

    //물탱크 10
    auto nextTank10 = Sprite::create(resourcePath+"image-water-tank-tens.png");
    nextTank10->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
    nextTank10->setPosition(Vec2(nextTank100->getPosition().x + (nextTank100->getContentSize().width/2) + (nextTank10->getContentSize().width/2), gameSize.height - 275));
    nextTank10->setName("nextTank10");
    _gameNode->addChild(nextTank10, 90);

    auto nextTank10_title = Label::createWithTTF(tank10_title_text, andikaFont, 50);
    nextTank10_title->setColor(Color3B(26, 26, 26));
    nextTank10_title->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
    nextTank10_title->setPosition(Vec2(nextTank10->getPosition().x, gameSize.height - 491 + 30));
    nextTank10_title->setName("nextTank10Title");
    _gameNode->addChild(nextTank10_title, 100);

    auto nextTensTextBG = Sprite::create(resourcePath+"image-tens-text-bg.png");
    nextTensTextBG->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
    nextTensTextBG->setPosition(Vec2(nextTank10->getPosition().x, gameSize.height - 600));
    nextTensTextBG->setName("nextTank10Text");
    _gameNode->addChild(nextTensTextBG, 100);

    //물탱크 1
    auto nextTank1 = Sprite::create(resourcePath+"image-water-tank-ones.png");
    nextTank1->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
    nextTank1->setPosition(Vec2(nextTank10->getPosition().x + (nextTank10->getContentSize().width/2) + (nextTank1->getContentSize().width/2), gameSize.height - 275));
    nextTank1->setName("nextTank1");
    _gameNode->addChild(nextTank1, 90);

    auto nextTank1_title = Label::createWithTTF(tank1_title_text, andikaFont, 50);
    nextTank1_title->setColor(Color3B(26, 26, 26));
    nextTank1_title->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
    nextTank1_title->setPosition(Vec2(nextTank1->getPosition().x, gameSize.height - 491 + 30));
    nextTank1_title->setName("nextTank1Title");
    _gameNode->addChild(nextTank1_title, 100);

    auto nextOnesTextBG = Sprite::create(resourcePath+"image-ones-text-bg.png");
    nextOnesTextBG->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
    nextOnesTextBG->setPosition(Vec2(nextTank1->getPosition().x, gameSize.height - 600));
    nextOnesTextBG->setName("nextTank1Text");
    _gameNode->addChild(nextOnesTextBG, 100);

    //메인 파이프
    auto nextAnswerPipe = Sprite::create(resourcePath+"image-pipe-02.png");
    nextAnswerPipe->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    nextAnswerPipe->setPosition(Vec2(getContentSize().width + nextAnswerPipe->getContentSize().width/2 + 10, gameSize.height - 275 - (pipe->getContentSize().height/2)));
    nextAnswerPipe->setName("nextAnswerPipe");
    _gameNode->addChild(nextAnswerPipe, 290);

    //파이프 글자BG, 글자
    auto nextAnswerBG = Sprite::create(resourcePath+"image-answer-text-bg.png");
    nextAnswerBG->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    nextAnswerBG->setPosition(nextAnswerPipe->getPosition());
    nextAnswerBG->setName("nextAnswer");
    _gameNode->addChild(nextAnswerBG, 300);

    //바닥 피스 생성
    createWaterDropLandPiece(100);
    createWaterDropLandPiece(10);
    createWaterDropLandPiece(1);
    
    //물탱크 100내 피스 생성
    for (int i = 0; i < _tank100Count; i++)
    {
        createWaterDropTankPiece(100);
    }
    adjustWaterDropInTank100(false);
    
    //물탱크 10내 피스 생성
    for (int i = 0; i < _tank10Count; i++)
    {
        createWaterDropTankPiece(10);
    }
    adjustWaterDropInTank10(false);
    
    //물탱크 1내 피스 생성
    for (int i = 0; i < _tank1Count; i++)
    {
        createWaterDropTankPiece(1);
    }
    adjustWaterDropInTank1(false);

    //모든 초기 로딩 및 셋팅이 끝나고 나면 BG,백버튼,스킵 출력
    getChildByName("bg")->setVisible(true);
    getChildByName("backButton")->setVisible(true);
    
    if (UserManager::getInstance()->isDebugMode())
    {
        getChildByName("skip")->setVisible(true);
    }
    
    _isSkip = false;
}

void PlaceValueScene::onLevelComplete()
{
    _progressBar->setCurrent(_currentBoardIndex+1, true);
    _currentBoardIndex++;

    auto winSize = getContentSize();

    if (_currentBoardIndex>=_boards.size())
    {
        auto CP = CompletePopup::create();
        CP->show(1.5f, [this] {
            CCAppController::sharedAppController()->handleGameComplete(1);
        });
    }
    else
    {
        showBoard(_currentBoardIndex);
    }
}

void PlaceValueScene::setLevel(int level)
{
    _currentLevel = level;
}

void PlaceValueScene::loadData(int level, int *workSheet)
{
    string P = "Games/" + resourcePath + "placevalue_level.tsv";
    string S = FileUtils::getInstance()->getStringFromFile(P);

    auto data = TodoUtil::readTSV(S);
    auto Lang = LanguageManager::getInstance()->getCurrentLanguageTag();

    string LanguageTag;

    int maxWorkSheet = 1;
    int rowIndex = 0;

    //최대 worksheet 값 찾기
    for (int i = 0; i < data.size(); i++)
    {
        //찾으려는 레벨과 다르면 다음으로 통과
        if (TodoUtil::stoi(data[i][1]) != level)
            continue;
        
        int workSheet = TodoUtil::stoi(data[i][2]);
        
        if (workSheet > maxWorkSheet)
            maxWorkSheet = workSheet;
    }

    //worksheet 고르기
    *workSheet = random(1, maxWorkSheet);
    
    _boards.clear();
    
    PlaceValueLevelStruct e;

    while (rowIndex < data.size())
    {
        auto row = data[rowIndex++];
        LanguageTag = row[0];
        e.level = TodoUtil::stoi(row[1]);
        e.worksheet = TodoUtil::stoi(row[2]);
        e.problem = TodoUtil::stoi(row[3]);
        e.objectNo = TodoUtil::stoi(row[4]);
        e.suggestNo = TodoUtil::stoi(row[5]);
        
        if (e.level == level)
        {
            //워크시트 테스트 모드가 아니라면 워크시트중에 랜덤으로 선택한것만 나오자
            if (UserManager::getInstance()->isWorksheetTestMode() == false)
            {
                //워크시트중에 랜덤 선택한것만 하도록
                if (e.worksheet == *workSheet)
                {
                    _boards.push_back(e);
                }
            }
            else
            {
                _boards.push_back(e);
            }
        }
    }
}

void PlaceValueScene::loadDurationsheet() {

    if (_duration.size() != 0) return;
    std::string rawString = cocos2d::FileUtils::getInstance()->getStringFromFile("NumberVoice/Durations.tsv");
    auto data = TodoUtil::readTSV(rawString);

    for (auto row : data) {
        if (TodoUtil::trim(row[0]).size() <= 0) continue;
        if (row.size()==1) continue;
        auto row1Vec = TodoUtil::split(row[1], ':');
        TodoUtil::replaceAll(row1Vec[2], ".", "");
        auto rowDuration = (float)TodoUtil::stoi(row1Vec[2])/100;
        _duration[row[0]] = rowDuration;
        
//        CCLOG("%s, %f", row[0].c_str(), rowDuration);
    }
}

float PlaceValueScene::getDuration(string name) {

    if (_duration.count(name))
        return _duration[name];

    return 2.5;
}

Node* PlaceValueScene::createShiningParticle(Vec2 size, int totalParticles)
{
    Node *particleNode = Node::create();
    
    ParticleSystemQuad* particleEffect = nullptr;
    
    auto createParticleEffect = [&](string name, string plist) {
        particleEffect = ParticleSystemQuad::create(plist);
        particleEffect->setName(name);
        particleEffect->setTotalParticles(totalParticles);
        particleEffect->setPosVar(size);
        particleEffect->setScale(1.0f);
        particleEffect->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        particleEffect->setPosition(Vec2::ZERO);
        particleEffect->stopSystem();
        particleNode->addChild(particleEffect);
    };
    
    createParticleEffect("particle1", "Common/Effects/Particle/shining_particle_blur.plist");
    createParticleEffect("particle2", "Common/Effects/Particle/shining_particle_circle.plist");
    createParticleEffect("particle3", "Common/Effects/Particle/shining_particle_star.plist");
    
    return particleNode;
}

void PlaceValueScene::startParticle(Node *particleNode)
{
    particleNode->getChildByName<ParticleSystemQuad*>("particle1")->resetSystem();
    particleNode->getChildByName<ParticleSystemQuad*>("particle2")->resetSystem();
    particleNode->getChildByName<ParticleSystemQuad*>("particle3")->resetSystem();
}

void PlaceValueScene::stopParticle(Node *particleNode)
{
    particleNode->getChildByName<ParticleSystemQuad*>("particle1")->stopSystem();
    particleNode->getChildByName<ParticleSystemQuad*>("particle2")->stopSystem();
    particleNode->getChildByName<ParticleSystemQuad*>("particle3")->stopSystem();
}

void PlaceValueScene::onStageClear()
{
    //스테이지 클리어시 블럭 터치 안되도록 설정
    for (auto p : _tank100Pieces)
    {
        p->_touchEnabled = false;
    }
    
    for (auto p : _tank10Pieces)
    {
        p->_touchEnabled = false;
    }

    for (auto p : _tank1Pieces)
    {
        p->_touchEnabled = false;
    }
    
    //연출할 텍스트노드 가져오기
    auto tank100Text = _gameNode->getChildByName("tank100Text");
    auto tank10Text = _gameNode->getChildByName("tank10Text");
    auto tank1Text = _gameNode->getChildByName("tank1Text");

    //성공 사운드 플레이
    solveEffect().play();
    
    //성공 파티클
    if (_tank100Count == 0)
    {
        tank100Text->setVisible(false);
    }
    else
    {
        startParticle(_tank100Particle);
    }
    
    if (_tank100Count == 0 && _tank10Count == 0)
    {
        tank10Text->setVisible(false);
    }
    else
    {
        startParticle(_tank10Particle);
    }
    
    startParticle(_tank1Particle);
    
    auto seq = Sequence::create(DelayTime::create(2.0f),//성공 사운드 플레이시간
                                CallFunc::create([this](){
        stopParticle(_tank100Particle);
        stopParticle(_tank10Particle);
        stopParticle(_tank1Particle);
    }),
                                nullptr);
    _gameNode->runAction(seq);
    
    //물방울 하강
    auto dropLand100 = _gameNode->getChildByName("dropLand100");
    if (dropLand100 != nullptr)
        dropLand100->runAction(MoveTo::create(0.5f, Vec2(dropLand100->getPositionX(), 0 - (dropLand100->getContentSize().height/2))));
    
    auto dropLand10 = _gameNode->getChildByName("dropLand10");
    if (dropLand10 != nullptr)
        dropLand10->runAction(MoveTo::create(0.5f, Vec2(dropLand10->getPositionX(), 0 - (dropLand10->getContentSize().height/2))));
    
    auto dropLand1 = _gameNode->getChildByName("dropLand1");
    if (dropLand1 != nullptr)
        dropLand1->runAction(MoveTo::create(0.5f, Vec2(dropLand1->getPositionX(), 0 - (dropLand1->getContentSize().height/2))));
    
    //배관타는 연출 시작
    auto seqPipe = Sequence::create(
                                    DelayTime::create(2.0f),
                                    CallFunc::create([this, tank100Text](){
                                        tank100Text->runAction(
                                            Sequence::create(
                                                MoveTo::create(0.5f, Vec2(tank100Text->getPositionX(), _gameNode->getChildByName("answerPipe")->getPositionY() + (tank100Text->getContentSize().height/2))),
                                                Spawn::create(MoveTo::create(0.5f, Vec2(_gameNode->getChildByName("answerPipe")->getPositionX() - 30, _gameNode->getChildByName("answerPipe")->getPositionY() + (tank100Text->getContentSize().height/2))),
                                                              FadeOut::create(0.5f), nullptr),
                                                nullptr));
        
                                        auto lb = tank100Text->getChildByName("text");
                                        lb->runAction(Sequence::create(DelayTime::create(0.5f), ScaleBy::create(0.5f, 2), Hide::create(), nullptr));
        
                                        if (_tank100Count == 0)
                                        {
                                            lb->runAction(Sequence::create(DelayTime::create(0.5f), FadeOut::create(0.5f), nullptr));
                                        }
        
                                        auto lb2 = tank100Text->getChildByName("text2");
                                        lb2->runAction(Sequence::create(DelayTime::create(0.5f), FadeOut::create(0.5f), nullptr));
        
                                        auto lb3 = tank100Text->getChildByName("text3");
                                        lb3->runAction(Sequence::create(DelayTime::create(0.5f), FadeOut::create(0.5f), nullptr));
                                    }),
                                    DelayTime::create(1.0f),
                                    CallFunc::create([this, tank10Text](){
                                        tank10Text->runAction(
                                            Sequence::create(
                                               MoveTo::create(0.5f, Vec2(tank10Text->getPositionX(), _gameNode->getChildByName("answerPipe")->getPositionY() + (tank10Text->getContentSize().height/2))),
                                               Spawn::create(MoveTo::create(0.5f, Vec2(_gameNode->getChildByName("answerPipe")->getPositionX() - 30 + 60, _gameNode->getChildByName("answerPipe")->getPositionY() + (tank10Text->getContentSize().height/2))),
                                                             FadeOut::create(0.5f), nullptr),
                                               nullptr));
        
                                        auto lb = tank10Text->getChildByName("text");
                                        lb->runAction(Sequence::create(DelayTime::create(0.5f), ScaleBy::create(0.5f, 2), Hide::create(), nullptr));
        
                                        if (_tank100Count == 0 && _tank10Count == 0)
                                        {
                                            lb->runAction(Sequence::create(DelayTime::create(0.5f), FadeOut::create(0.5f), nullptr));
                                        }
        
                                        auto lb2 = tank10Text->getChildByName("text2");
                                        lb2->runAction(Sequence::create(DelayTime::create(0.5f), FadeOut::create(0.5f), nullptr));
                                    }),
                                    DelayTime::create(1.0f),
                                    CallFunc::create([this, tank1Text](){
                                        tank1Text->runAction(
                                            Sequence::create(
                                              MoveTo::create(0.5f, Vec2(tank1Text->getPositionX(), _gameNode->getChildByName("answerPipe")->getPositionY() + (tank1Text->getContentSize().height/2))),
                                              Spawn::create(MoveTo::create(0.5f, Vec2(_gameNode->getChildByName("answerPipe")->getPositionX() - 30 + 60 + 60, _gameNode->getChildByName("answerPipe")->getPositionY() + (tank1Text->getContentSize().height/2))),
                                                            FadeOut::create(0.5f), nullptr),
                                              nullptr));

                                        auto lb = tank1Text->getChildByName("text");
                                        lb->runAction(Sequence::create(DelayTime::create(0.5f), ScaleBy::create(0.5f, 2), Hide::create(), nullptr));
                                    }),
                                    nullptr);
    
    _gameNode->runAction(seqPipe);
    
    //글자가 메인배관 들어갈때 연출
    auto answer1 = _gameNode->getChildByName("answer")->getChildByName("text1");
    auto answer2 = _gameNode->getChildByName("answer")->getChildByName("text2");
    auto answer3 = _gameNode->getChildByName("answer")->getChildByName("text3");
    
    auto seqAnswer = Sequence::create(
                                    DelayTime::create(2.0f),
                                    DelayTime::create(1.0f),
                                    CallFunc::create([this, answer1](){
                                        if (_tank100Count == 0)
                                        {
                                        }
                                        else
                                        {
                                            answer1->runAction(
                                                Sequence::create(
                                                    CallFunc::create([this](){
                                                        this->startParticle(_answerParticle);
                                                        CommonSoundPlayer.cardinalNumber(_tank100Count * 100).play();
                                                    }),
                                                    TintTo::create(0.5f, 255, 197, 61),
                                                    CallFunc::create([this](){
                                                        this->stopParticle(_answerParticle);
                                                    }),
                                                    nullptr));
                                        }
                                    }),
                                    DelayTime::create(1.0f),
                                    CallFunc::create([this, answer2](){
                                        if (_tank100Count == 0 && _tank10Count == 0)
                                        {
                                        }
                                        else
                                        {
                                            answer2->runAction(
                                                Sequence::create(
                                                    CallFunc::create([this](){
                                                        this->startParticle(_answerParticle);
                                                        CommonSoundPlayer.cardinalNumber(_tank10Count * 10).play();
                                                    }),
                                                    TintTo::create(0.5f, 194, 255, 61),
                                                    CallFunc::create([this](){
                                                        this->stopParticle(_answerParticle);
                                                    }),
                                                    nullptr));
                                        }
                                    }),
                                    DelayTime::create(1.0f),
                                    CallFunc::create([this, answer3](){
                                        answer3->runAction(
                                            Sequence::create(
                                                CallFunc::create([this](){
                                                    this->startParticle(_answerParticle);
                                                    CommonSoundPlayer.cardinalNumber(_tank1Count * 1).play();
                                                }),
                                                TintTo::create(0.5f, 200, 61, 255),
                                                CallFunc::create([this](){
                                                    this->stopParticle(_answerParticle);
                                                }),
                                                nullptr));
                                    }),
                                    DelayTime::create(1.0f),
                                    DelayTime::create(0.5f),
                                    CallFunc::create([this, answer1, answer2, answer3](){
                                        answer1->setColor(Color3B(61, 178, 255));
                                        answer2->setColor(Color3B(61, 178, 255));
                                        answer3->setColor(Color3B(61, 178, 255));
        
                                        this->startParticle(_answerParticle);
        
                                        //100의 자리 음성출력
                                        if (_tank100Count == 0){}
                                        else
                                        {
                                            CommonSoundPlayer.cardinalNumber(_tank100Count * 100).play();
                                        }
                                    }),
                                    DelayTime::create(getDurationResult100()),
                                    CallFunc::create([this](){
                                        //스와일리어에서 1의자리가 0일때는 중간에 and 음성 출력해야함
                                        if (_tank100Count == 0 || _tank10Count == 0){}
                                        else
                                        {
                                            if (LanguageManager::getInstance()->isSwahili() && _tank10Count > 0 && _tank1Count == 0)
                                            {
                                                CommonSoundPlayer.aand().play();
                                            }
                                            else{}
                                        }
                                    }),
                                    DelayTime::create(getDurationResultAnd()),
                                    CallFunc::create([this](){
                                          //10의 자리 음성 출력
                                          if (_tank10Count > 0)
                                          {
                                              CommonSoundPlayer.cardinalNumber((_tank10Count * 10) + _tank1Count).play();
                                          }
                                          else if (_tank10Count == 0 && _tank1Count > 0)
                                          {
                                              CommonSoundPlayer.cardinalNumber(_tank1Count).play();
                                          }
                                    }),
                                    DelayTime::create(getDurationResult10()),
                                    CallFunc::create([this](){
                                        this->stopParticle(_answerParticle);
                                    }),
                                    DelayTime::create(1.0f),
                                    //다음 문제 제출 연출
                                    CallFunc::create([this](){
                                        //마지막 문제였다면 바로 종료시키자
                                        if (_currentBoardIndex+1 >= _boards.size())
                                        {
                                            onLevelComplete();
                                        }
                                        else
                                        {
                                            _gameNode->getChildByName("land")->runAction(MoveBy::create(1.0f, Vec2(-getContentSize().width, 0)));
                                            _gameNode->getChildByName("brush1")->runAction(MoveBy::create(1.0f, Vec2(-getContentSize().width, 0)));
                                            _gameNode->getChildByName("brush2")->runAction(MoveBy::create(1.0f, Vec2(-getContentSize().width, 0)));
                                            _gameNode->getChildByName("tank100")->runAction(MoveBy::create(1.0f, Vec2(-getContentSize().width, 0)));
                                            _gameNode->getChildByName("tank100Title")->runAction(MoveBy::create(1.0f, Vec2(-getContentSize().width, 0)));
                                            _gameNode->getChildByName("tank100Text")->runAction(MoveBy::create(1.0f, Vec2(-getContentSize().width, 0)));
                                            _gameNode->getChildByName("tank10")->runAction(MoveBy::create(1.0f, Vec2(-getContentSize().width, 0)));
                                            _gameNode->getChildByName("tank10Title")->runAction(MoveBy::create(1.0f, Vec2(-getContentSize().width, 0)));
                                            _gameNode->getChildByName("tank10Text")->runAction(MoveBy::create(1.0f, Vec2(-getContentSize().width, 0)));
                                            _gameNode->getChildByName("tank1")->runAction(MoveBy::create(1.0f, Vec2(-getContentSize().width, 0)));
                                            _gameNode->getChildByName("tank1Title")->runAction(MoveBy::create(1.0f, Vec2(-getContentSize().width, 0)));
                                            _gameNode->getChildByName("tank1Text")->runAction(MoveBy::create(1.0f, Vec2(-getContentSize().width, 0)));
                                            _gameNode->getChildByName("answerPipe")->runAction(MoveBy::create(1.0f, Vec2(-getContentSize().width, 0)));
                                            _gameNode->getChildByName("answer")->runAction(MoveBy::create(1.0f, Vec2(-getContentSize().width, 0)));

                                            _gameNode->getChildByName("nextLand")->runAction(MoveBy::create(1.0f, Vec2(-getContentSize().width, 0)));
                                            _gameNode->getChildByName("nextBrush1")->runAction(MoveBy::create(1.0f, Vec2(-getContentSize().width, 0)));
                                            _gameNode->getChildByName("nextBrush2")->runAction(MoveBy::create(1.0f, Vec2(-getContentSize().width, 0)));
                                            _gameNode->getChildByName("nextTank100")->runAction(MoveBy::create(1.0f, Vec2(-getContentSize().width, 0)));
                                            _gameNode->getChildByName("nextTank100Title")->runAction(MoveBy::create(1.0f, Vec2(-getContentSize().width, 0)));
                                            _gameNode->getChildByName("nextTank100Text")->runAction(MoveBy::create(1.0f, Vec2(-getContentSize().width, 0)));
                                            _gameNode->getChildByName("nextTank10")->runAction(MoveBy::create(1.0f, Vec2(-getContentSize().width, 0)));
                                            _gameNode->getChildByName("nextTank10Title")->runAction(MoveBy::create(1.0f, Vec2(-getContentSize().width, 0)));
                                            _gameNode->getChildByName("nextTank10Text")->runAction(MoveBy::create(1.0f, Vec2(-getContentSize().width, 0)));
                                            _gameNode->getChildByName("nextTank1")->runAction(MoveBy::create(1.0f, Vec2(-getContentSize().width, 0)));
                                            _gameNode->getChildByName("nextTank1Title")->runAction(MoveBy::create(1.0f, Vec2(-getContentSize().width, 0)));
                                            _gameNode->getChildByName("nextTank1Text")->runAction(MoveBy::create(1.0f, Vec2(-getContentSize().width, 0)));
                                            _gameNode->getChildByName("nextAnswerPipe")->runAction(MoveBy::create(1.0f, Vec2(-getContentSize().width, 0)));
                                            _gameNode->getChildByName("nextAnswer")->runAction(MoveBy::create(1.0f, Vec2(-getContentSize().width, 0)));
            
                                            for (auto piece : _tank100Pieces)
                                            {
                                                piece->runAction(MoveBy::create(1.0f, Vec2(-getContentSize().width, 0)));
                                            }
            
                                            for (auto piece : _tank10Pieces)
                                            {
                                                piece->runAction(MoveBy::create(1.0f, Vec2(-getContentSize().width, 0)));
                                            }
            
                                            for (auto piece : _tank1Pieces)
                                            {
                                                piece->runAction(MoveBy::create(1.0f, Vec2(-getContentSize().width, 0)));
                                            }
                                        }
                                    }),
                                    DelayTime::create(1.0f),
                                    CallFunc::create([this](){
                                        //마지막 문제였다면 바로 종료시키자
                                        if (_currentBoardIndex+1 >= _boards.size())
                                        {
                                        }
                                        else
                                        {
                                            correctEffect().play();
                                            onLevelComplete();
                                        }
                                    }),
                                    nullptr);
    
    _gameNode->runAction(seqAnswer);

}

float PlaceValueScene::getDurationResult100()
{
    float tmpTime = 0;
    
    //100의 자리 음성출력
    if (_tank100Count == 0)
    {
        tmpTime = 0;
    }
    else
    {
        tmpTime = getDuration("d_"+ (TodoUtil::itos(_tank100Count * 100)) + ".m4a");
    }

    return tmpTime;
}

float PlaceValueScene::getDurationResultAnd()
{
    float tmpTime = 0;
    
    //스와일리어에서 1의자리가 0일때는 중간에 and 음성 출력해야함
    if (_tank100Count == 0 || _tank10Count == 0)
    {
        tmpTime = 0;
    }
    else
    {
        if (LanguageManager::getInstance()->isSwahili() && _tank10Count > 0 && _tank1Count == 0)
        {
            tmpTime = getDuration("and.m4a");
        }
        else
        {
            tmpTime = 0;
        }
    }
    
    return tmpTime;
}

float PlaceValueScene::getDurationResult10()
{
    float tmpTime = 0;
    
    //10의 자리 음성 출력
    if (_tank10Count > 0)
    {
        tmpTime = getDuration("d_"+ (TodoUtil::itos((_tank10Count * 10) + _tank1Count) + ".m4a"));
    }
    else if (_tank10Count == 0 && _tank1Count > 0)
    {
        tmpTime = getDuration("d_"+ (TodoUtil::itos(_tank1Count)) + ".m4a");
    }

    return tmpTime;
}


//여기에 아예 카운트도 추가하도록 하자
void PlaceValueScene::setTank100Count(int count)
{
    bool isUp = (_tank100Count < count ? true : false);
    
    auto tankText = _gameNode->getChildByName("tank100Text");
    auto lb = (Label*)(tankText->getChildByName("text"));
    
    auto lbNew = Label::createWithTTF(TodoUtil::itos(count), kitkitNumbersFont, 75);
    lbNew->setColor(Color3B(255, 197, 61));
    lbNew->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    lbNew->setPosition(Vec2(lb->getPositionX(), lb->getPositionY() + (isUp ? -30 : +30)));
    lbNew->setOpacity(0);
    tankText->addChild(lbNew, 10);
    
    //카운트 업데이트
    _tank100Count = count;
    
    //오름일때
    if (isUp)
    {
        lb->runAction(Sequence::create(Spawn::create(MoveBy::create(0.2f, Vec2(0, 30)), FadeOut::create(0.2f), nullptr),
                                       DelayTime::create(0.2f),
                                       CallFunc::create([this, lb](){ lb->setString(TodoUtil::itos(_tank100Count)); }),
                                       MoveBy::create(0.0f, Vec2(0, -30)),
                                       FadeIn::create(0.0f),
                                       nullptr));
        
        lbNew->runAction(Sequence::create(Spawn::create(MoveBy::create(0.2f, Vec2(0, 30)), FadeIn::create(0.2f), nullptr),
                                          DelayTime::create(0.2f),
                                          CallFunc::create([this, tankText, lbNew](){ tankText->removeChild(lbNew); }),
                                          nullptr));
    }
    else
    {
        lb->runAction(Sequence::create(Spawn::create(MoveBy::create(0.2f, Vec2(0, -30)), FadeOut::create(0.2f), nullptr),
                                       DelayTime::create(0.2f),
                                       CallFunc::create([this, lb](){ lb->setString(TodoUtil::itos(_tank100Count)); }),
                                       MoveBy::create(0.0f, Vec2(0, +30)),
                                       FadeIn::create(0.0f),
                                       nullptr));
        
        lbNew->runAction(Sequence::create(Spawn::create(MoveBy::create(0.2f, Vec2(0, -30)), FadeIn::create(0.2f), nullptr),
                                          DelayTime::create(0.2f),
                                          CallFunc::create([this, tankText, lbNew](){ tankText->removeChild(lbNew); }),
                                          nullptr));
    }
}

void PlaceValueScene::setTank10Count(int count)
{
    if (count >= 10)
    {
        //카운트 업데이트
        _tank10Count = count;
        return;
    }

    bool isUp = (_tank10Count < count ? true : false);
    
    auto tankText = _gameNode->getChildByName("tank10Text");
    auto lb = (Label*)(tankText->getChildByName("text"));
    
    auto lbNew = Label::createWithTTF(TodoUtil::itos(count), kitkitNumbersFont, 75);
    lbNew->setColor(Color3B(194, 255, 61));
    lbNew->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    lbNew->setPosition(Vec2(lb->getPositionX(), lb->getPositionY() + (isUp ? -30 : +30)));
    lbNew->setOpacity(0);
    tankText->addChild(lbNew, 10);
    
    //카운트 업데이트
    _tank10Count = count;
    
    //오름일때
    if (isUp)
    {
        lb->runAction(Sequence::create(Spawn::create(MoveBy::create(0.2f, Vec2(0, 30)), FadeOut::create(0.2f), nullptr),
                                       DelayTime::create(0.2f),
                                       CallFunc::create([this, lb](){ lb->setString(TodoUtil::itos(_tank10Count)); }),
                                       MoveBy::create(0.0f, Vec2(0, -30)),
                                       FadeIn::create(0.0f),
                                       nullptr));
        
        lbNew->runAction(Sequence::create(Spawn::create(MoveBy::create(0.2f, Vec2(0, 30)), FadeIn::create(0.2f), nullptr),
                                          DelayTime::create(0.2f),
                                          CallFunc::create([this, tankText, lbNew](){ tankText->removeChild(lbNew); }),
                                          nullptr));
    }
    else
    {
        lb->runAction(Sequence::create(Spawn::create(MoveBy::create(0.2f, Vec2(0, -30)), FadeOut::create(0.2f), nullptr),
                                       DelayTime::create(0.2f),
                                       CallFunc::create([this, lb](){ lb->setString(TodoUtil::itos(_tank10Count)); }),
                                       MoveBy::create(0.0f, Vec2(0, +30)),
                                       FadeIn::create(0.0f),
                                       nullptr));
        
        lbNew->runAction(Sequence::create(Spawn::create(MoveBy::create(0.2f, Vec2(0, -30)), FadeIn::create(0.2f), nullptr),
                                          DelayTime::create(0.2f),
                                          CallFunc::create([this, tankText, lbNew](){ tankText->removeChild(lbNew); }),
                                          nullptr));
    }
}

void PlaceValueScene::setTank1Count(int count)
{
    bool isUp = (_tank1Count < count ? true : false);
    
    auto tankText = _gameNode->getChildByName("tank1Text");
    auto lb = (Label*)(tankText->getChildByName("text"));
    
    auto lbNew = Label::createWithTTF(TodoUtil::itos(count), kitkitNumbersFont, 75);
    lbNew->setColor(Color3B(200, 61, 255));
    lbNew->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    lbNew->setPosition(Vec2(lb->getPositionX(), lb->getPositionY() + (isUp ? -30 : +30)));
    lbNew->setOpacity(0);
    tankText->addChild(lbNew, 10);
    
    //카운트 업데이트
    _tank1Count = count;
    
    //오름일때
    if (isUp)
    {
        lb->runAction(Sequence::create(Spawn::create(MoveBy::create(0.2f, Vec2(0, 30)), FadeOut::create(0.2f), nullptr),
                                       DelayTime::create(0.2f),
                                       CallFunc::create([this, lb](){ lb->setString(TodoUtil::itos(_tank1Count)); }),
                                       MoveBy::create(0.0f, Vec2(0, -30)),
                                       FadeIn::create(0.0f),
                                       nullptr));
        
        lbNew->runAction(Sequence::create(Spawn::create(MoveBy::create(0.2f, Vec2(0, 30)), FadeIn::create(0.2f), nullptr),
                                          DelayTime::create(0.2f),
                                          CallFunc::create([this, tankText, lbNew](){ tankText->removeChild(lbNew); }),
                                          nullptr));
    }
    else
    {
        lb->runAction(Sequence::create(Spawn::create(MoveBy::create(0.2f, Vec2(0, -30)), FadeOut::create(0.2f), nullptr),
                                       DelayTime::create(0.2f),
                                       CallFunc::create([this, lb](){ lb->setString(TodoUtil::itos(_tank1Count)); }),
                                       MoveBy::create(0.0f, Vec2(0, +30)),
                                       FadeIn::create(0.0f),
                                       nullptr));
        
        lbNew->runAction(Sequence::create(Spawn::create(MoveBy::create(0.2f, Vec2(0, -30)), FadeIn::create(0.2f), nullptr),
                                          DelayTime::create(0.2f),
                                          CallFunc::create([this, tankText, lbNew](){ tankText->removeChild(lbNew); }),
                                          nullptr));
    }
}

WaterDropLandPiece *PlaceValueScene::createWaterDropLandPiece(int count)
{
    auto piece = WaterDropLandPiece::create();
    
    //텍스쳐 설정
    piece->setTexture(count);
    piece->_count = count;
    
    //좌표셋팅
    if (count == 100)
    {
        piece->setName("dropLand100");
        piece->_startPos = Vec2(_gameNode->getChildByName("tank100")->getPosition().x, 0 - piece->getContentSize().height);
        piece->_originPos = Vec2(_gameNode->getChildByName("tank100")->getPosition().x, 0 + 14 + (piece->getContentSize().height/2));
        piece->_targetNode = _gameNode->getChildByName("tank100_effect");
        
        //숫자 증가 알림
        auto indicationBG = Sprite::create(resourcePath+"image-hundred-indication-text-bg.png");
        indicationBG->setName("indication");
        indicationBG->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
        piece->addChild(indicationBG);
        
        auto indicationText = Label::createWithTTF("+100", andikaFont, 75);
        indicationText->setColor(Color3B(255, 197, 61));
        indicationText->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        indicationText->setPosition(Vec2(indicationBG->getContentSize().width/2, indicationBG->getContentSize().height/2));
        indicationBG->addChild(indicationText);
        
        indicationBG->setVisible(false);
    }
    else if (count == 10)
    {
        piece->setName("dropLand10");
        piece->_startPos = Vec2(_gameNode->getChildByName("tank10")->getPosition().x, 0 - piece->getContentSize().height);
        piece->_originPos = Vec2(_gameNode->getChildByName("tank10")->getPosition().x, 0 + 14 + (piece->getContentSize().height/2));
        piece->_targetNode = _gameNode->getChildByName("tank10_effect");
        
        //숫자 증가 알림
        auto indicationBG = Sprite::create(resourcePath+"image-ten-indication-text-bg.png");
        indicationBG->setName("indication");
        indicationBG->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
        piece->addChild(indicationBG);
        
        auto indicationText = Label::createWithTTF("+10", andikaFont, 75);
        indicationText->setColor(Color3B(194, 255, 61));
        indicationText->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        indicationText->setPosition(Vec2(indicationBG->getContentSize().width/2, indicationBG->getContentSize().height/2));
        indicationBG->addChild(indicationText);
        
        indicationBG->setVisible(false);
    }
    else if (count == 1)
    {
        piece->setName("dropLand1");
        piece->_startPos = Vec2(_gameNode->getChildByName("tank1")->getPosition().x, 0 - piece->getContentSize().height);
        piece->_originPos = Vec2(_gameNode->getChildByName("tank1")->getPosition().x, 0 + 118 + (piece->getContentSize().height/2));
        piece->_targetNode = _gameNode->getChildByName("tank1_effect");
        
        //숫자 증가 알림
        auto indicationBG = Sprite::create(resourcePath+"image-one-indication-text-bg.png");
        indicationBG->setName("indication");
        indicationBG->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
        piece->addChild(indicationBG);
        
        auto indicationText = Label::createWithTTF("+1", andikaFont, 75);
        indicationText->setColor(Color3B(200, 61, 255));
        indicationText->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        indicationText->setPosition(Vec2(indicationBG->getContentSize().width/2, indicationBG->getContentSize().height/2));
        indicationBG->addChild(indicationText);
        
        indicationBG->setVisible(false);
    }
    
    //터치 관련 설정
    piece->_touchEnabled = true;

    piece->onPicked = [this, piece]()
    {        
    };
    
    piece->onHover = [this, piece]()
    {
        //해당 탱크 하이라이트 켜주자
        piece->_targetNode->setVisible(true);
        
        //숫자 증가 알림
        piece->getChildByName("indication")->setVisible(true);
        piece->getChildByName("indication")->setPosition(Vec2(piece->_touchPos.x/*piece->getContentSize().width/2*/, piece->_touchPos.y + 150));
    };
    
    piece->onHoverOut = [this, piece]()
    {
        //해당 탱크 하이라이트 꺼주자
        piece->_targetNode->setVisible(false);
        
        //숫자 증가 알림 끄기
        piece->getChildByName("indication")->setVisible(false);
    };

    
    piece->onSnapped = [this, piece]()
    {
//        correctEffect().play();
        
        int pcount = piece->_count;
        
        //해당 탱크 하이라이트 꺼주자
        piece->_targetNode->setVisible(false);
        
        //탱크에 피스 추가
        createWaterDropTankPiece(pcount);
    
        if (pcount == 100)
        {
            //탱크내 물방울 정렬
            adjustWaterDropInTank100(false);
            
            //100 카운트 증가
            setTank100Count(_tank100Count + 1);
            
            //숫자 사운드 플레이
            CommonSoundPlayer.cardinalNumber(_tank100Count * 100).play();
        }
        else if (pcount == 10)
        {
            //탱크내 물방울 정렬
            adjustWaterDropInTank10(false);
            
            //받아올림
            if (_tank10Pieces.size() >= 10)
            {
                upgradeTankPiece10();
            }
            else
            {
                //10 카운트 증가
                setTank10Count(_tank10Count + 1);
                
                //숫자 사운드 플레이
                CommonSoundPlayer.cardinalNumber(_tank10Count * 10).play();
            }
        }
        else if (pcount == 1)
        {
            //탱크내 물방울 정렬
            adjustWaterDropInTank1(false);
            
            //받아올림
            if (_tank1Pieces.size() >= 10)
            {
                upgradeTankPiece1();
            }
            else
            {
                //1 카운트 증가
                setTank1Count(_tank1Count + 1);
                
                //숫자 사운드 플레이
                CommonSoundPlayer.cardinalNumber(_tank1Count * 1).play();
            }
        }
        
        //스테이지 클리어 판정
        if ((_tank100Count*100)+(_tank10Count*10)+(_tank1Count) == _objectNo)
        {
            onStageClear();
        }
        else
        {
            //새 바닥 피스 추가
            createWaterDropLandPiece(pcount);
        }
        
        //게임 로그 남기기
        string workPath = [this] {
            stringstream SS;
            SS << "/" << "PlaceValue";
            SS << "/" << "level-" << _currentLevel << "-" << _currentSheetNo;
            SS << "/" << "work-" << _currentBoardIndex;
            return SS.str();
        }();
        
        StrictLogManager::shared()->game_Peek_Answer("PlaceValue", workPath,
                                                     TodoUtil::itos((_tank100Count*100)+(_tank10Count*10)+(_tank1Count)),
                                                     TodoUtil::itos(_objectNo));
        
        //기존 바닥 피스 삭제
        _gameNode->removeChild(piece);

    };

    piece->onLaid = [this, piece]
    {
        //돌아갈곳과의 거리차이가 100px이상일때만 사운드 플레이
        if (piece->getPosition().getDistance(piece->_originPos) > 100)
            layEffect().play();
    };
    
    //첫 시작시 연출
    piece->setPosition(piece->_startPos);
    piece->runAction(EaseIn::create(MoveTo::create(0.5f, piece->_originPos), 3.0f));
    
    _gameNode->addChild(piece, 300);

    
    return piece;
}

void PlaceValueScene::upgradeTankPiece10()
{
    //기존것 삭제
    for (auto p : _tank10Pieces)
    {
        _gameNode->removeChild(p);
    }
    _tank10Pieces.clear();
    
    //100의 자리 생성 및 추가
    auto p = createWaterDropTankPiece(100);
    p->setPosition(_gameNode->getChildByName("tank10")->getPosition().x, _gameNode->getChildByName("tank10_effect")->getPositionY() - (_gameNode->getChildByName("tank10_effect")->getContentSize().height/2));
    
    //숫자 증가 알림 표시
    Sprite *indication = (Sprite*)p->getChildByName("indication");
    Label *indicationText = (Label*)indication->getChildByName("text");
    indicationText->setString("+100");
    indicationText->setColor(Color3B(255, 197, 61));
    indication->setVisible(true);
    indication->setPosition(Vec2(p->getContentSize().width/2, p->getContentSize().height + 150));
    
    //윗자리 사운드 플레이
    CommonSoundPlayer.cardinalNumber(100).play();
    
    //받아올려서 이동할 위치 계산
    Vec2 movePos;
    for (int i = 0; i < 10; i++)
    {
        int c = i % 5;
        int r = i / 5;
        
        if (i == _tank100Pieces.size()-1)
        {
            movePos = Vec2(_gameNode->getChildByName("tank100")->getBoundingBox().getMinX() + 59 + (p->getContentSize().width/2) + (c * 222), _gameNode->getChildByName("tank100")->getPosition().y - 506 - (p->getContentSize().height/2) - (r * 242));
            break;
        }
    }
    
    //100의 자리 포물선 이동후 사라지고 100 탱크 정렬
    auto seq = Sequence::create(CallFunc::create([this](){
                                    //10카운트 0으로 설정
                                    setTank10Count(0);
                                }),
                                DelayTime::create(0.5f),
                                JumpTo::create(0.5f, movePos, 300, 1),
                                CallFunc::create([this, indication, indicationText](){
                                    //숫자 증가 알림 끄기
                                    indication->setVisible(false);
                                    indicationText->setString("-100");
                                    indicationText->setColor(Color3B(255, 61, 116));
        
                                    //탱크내 물방울 정렬
                                    adjustWaterDropInTank100(false);
        
                                    //100 카운트 증가
                                    setTank100Count(_tank100Count + 1);
        
                                    //숫자 사운드 플레이
                                    CommonSoundPlayer.cardinalNumber(_tank100Count * 100).play();
        
                                    //스테이지 클리어 판정
                                    if ((_tank100Count*100)+(_tank10Count*10)+(_tank1Count) == _objectNo)
                                    {
                                        onStageClear();
                                    }
                                }),
                                nullptr);
    p->runAction(seq);
}

void PlaceValueScene::upgradeTankPiece1()
{
    //기존것 삭제
    for (auto p : _tank1Pieces)
    {
        _gameNode->removeChild(p);
    }
    _tank1Pieces.clear();
    
    //10의 자리 생성 및 추가
    auto p = createWaterDropTankPiece(10);
    p->setPosition(_gameNode->getChildByName("tank1")->getPosition().x, _gameNode->getChildByName("tank1_effect")->getPositionY() - (_gameNode->getChildByName("tank1_effect")->getContentSize().height/2));
    
    //숫자 증가 알림 표시
    Sprite *indication = (Sprite*)p->getChildByName("indication");
    Label *indicationText = (Label*)indication->getChildByName("text");
    indicationText->setString("+10");
    indicationText->setColor(Color3B(194, 255, 61));
    indication->setVisible(true);
    indication->setPosition(Vec2(p->getContentSize().width/2, p->getContentSize().height + 150));
    
    //윗자리 사운드 플레이
    CommonSoundPlayer.cardinalNumber(10).play();
    
    //받아올려서 이동할 위치 계산
    Vec2 movePos;
    for (int i = 0; i < 10; i++)
    {
        if (i == _tank10Pieces.size()-1)
        {
            movePos = Vec2(_gameNode->getChildByName("tank10")->getBoundingBox().getMinX() + 48 + (p->getContentSize().width/2) + (i * 68), _gameNode->getChildByName("tank10")->getPosition().y - 505 - (p->getContentSize().height/2));
        }
    }
    
    //10의 자리 포물선 이동후 사라지고 10 탱크 정렬
    auto seq = Sequence::create(CallFunc::create([this](){
                                //1카운트 0으로 설정
                                setTank1Count(0);
                                }),
                                DelayTime::create(0.5f),
                                JumpTo::create(0.5f, movePos, 300, 1),
                                CallFunc::create([this, indication, indicationText](){
                                    //숫자 증가 알림 끄기
                                    indication->setVisible(false);
                                    indicationText->setString("-10");
                                    indicationText->setColor(Color3B(255, 61, 116));
        
                                    //탱크내 물방울 정렬
                                    adjustWaterDropInTank10(false);
        
                                    //10 카운트 증가
                                    setTank10Count(_tank10Count + 1);
        
                                    //숫자 사운드 플레이
                                    CommonSoundPlayer.cardinalNumber(_tank10Count * 10).play();
        
                                    //스테이지 클리어 판정
                                    if ((_tank100Count*100)+(_tank10Count*10)+(_tank1Count) == _objectNo)
                                    {
                                        onStageClear();
                                    }
                                }),
                                DelayTime::create(0.5f),
                                CallFunc::create([this](){
                                    //십의자리 받아올림
                                    if (_tank10Pieces.size() >= 10)
                                    {
                                        upgradeTankPiece10();
                                    }
                                }),
                                nullptr);
    
    p->runAction(seq);
}

WaterDropTankPiece *PlaceValueScene::createWaterDropTankPiece(int count)
{
    auto piece = WaterDropTankPiece::create();
    
    //텍스쳐 설정
    piece->setTexture(count);
    piece->_count = count;
    
    if (count == 100)
    {
        piece->_targetNode = _gameNode->getChildByName("tank100_effect");
        
        //숫자 감소 알림
        auto indicationBG = Sprite::create(resourcePath+"image-hundred-indication-text-bg.png");
        indicationBG->setName("indication");
        indicationBG->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
        piece->addChild(indicationBG);
        
        auto indicationText = Label::createWithTTF("-100", andikaFont, 75);
        indicationText->setName("text");
        indicationText->setColor(Color3B(255, 61, 116));
        indicationText->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        indicationText->setPosition(Vec2(indicationBG->getContentSize().width/2, indicationBG->getContentSize().height/2));
        indicationBG->addChild(indicationText);
        
        indicationBG->setVisible(false);

        //리스트에 추가
        _tank100Pieces.insert(_tank100Pieces.begin(), piece);
    }
    else if (count == 10)
    {
        piece->_targetNode = _gameNode->getChildByName("tank10_effect");
        
        //숫자 감소 알림
        auto indicationBG = Sprite::create(resourcePath+"image-ten-indication-text-bg.png");
        indicationBG->setName("indication");
        indicationBG->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
        piece->addChild(indicationBG);
        
        auto indicationText = Label::createWithTTF("-10", andikaFont, 75);
        indicationText->setName("text");
        indicationText->setColor(Color3B(255, 61, 116));
        indicationText->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        indicationText->setPosition(Vec2(indicationBG->getContentSize().width/2, indicationBG->getContentSize().height/2));
        indicationBG->addChild(indicationText);
        
        indicationBG->setVisible(false);

        //리스트에 추가
        _tank10Pieces.insert(_tank10Pieces.begin(), piece);
    }
    else if (count == 1)
    {
        piece->_targetNode = _gameNode->getChildByName("tank1_effect");
        
        //숫자 감소 알림
        auto indicationBG = Sprite::create(resourcePath+"image-one-indication-text-bg.png");
        indicationBG->setName("indication");
        indicationBG->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
        piece->addChild(indicationBG);
        
        auto indicationText = Label::createWithTTF("-1", andikaFont, 75);
        indicationText->setName("text");
        indicationText->setColor(Color3B(255, 61, 116));
        indicationText->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        indicationText->setPosition(Vec2(indicationBG->getContentSize().width/2, indicationBG->getContentSize().height/2));
        indicationBG->addChild(indicationText);
        
        indicationBG->setVisible(false);
        
        //리스트에 추가
        _tank1Pieces.insert(_tank1Pieces.begin(), piece);
    }
    
    //터치 관련 설정
    piece->_touchEnabled = true;
    
    piece->onPicked = [this, piece]()
    {
    };
    
    piece->onHover = [this, piece]()
    {
        //숫자 감소 알림
        piece->getChildByName("indication")->setVisible(true);
        piece->getChildByName("indication")->setPosition(Vec2(piece->_touchPos.x/*piece->getContentSize().width/2*/, piece->_touchPos.y + 150));
    };
    
    piece->onHoverOut = [this, piece]()
    {
        //숫자 증가 알림 끄기
        piece->getChildByName("indication")->setVisible(false);
    };
    
    piece->onSnapped = [this, piece]()
    {
        incorrectEffect().play();
        
        //물방울 감소 이펙트 출력
        auto animation = Animation::create();
        animation->setDelayPerUnit(0.05f);
        animation->addSpriteFrameWithFile(resourcePath+"minusWaterDrop/effect-subtract-01.png");
        animation->addSpriteFrameWithFile(resourcePath+"minusWaterDrop/effect-subtract-02.png");
        animation->addSpriteFrameWithFile(resourcePath+"minusWaterDrop/effect-subtract-03.png");
        animation->addSpriteFrameWithFile(resourcePath+"minusWaterDrop/effect-subtract-04.png");
        animation->addSpriteFrameWithFile(resourcePath+"minusWaterDrop/effect-subtract-05.png");
        
        auto minusWaterDropEffect = Sprite::create();
        _gameNode->addChild(minusWaterDropEffect, 500);
        
        minusWaterDropEffect->setPosition(piece->getPosition());
        minusWaterDropEffect->runAction(Sequence::create(Animate::create(animation),
                                                         CallFunc::create([this, minusWaterDropEffect](){ _gameNode->removeChild(minusWaterDropEffect); }),
                                                         nullptr) );
        
        int pcount = piece->_count;
        
        if (pcount == 100)
        {
            int removedIndex = 0;
            
            //리스트에서 삭제
            for (vector<WaterDropTankPiece*>::iterator it = _tank100Pieces.begin(); it != _tank100Pieces.end(); )
            {
                if ((*it) == piece)
                {
                    it = _tank100Pieces.erase(it);
                    break;
                }
                else
                    ++it;
                
                removedIndex++;
            }
            
            //탱크내 물방울 정렬
            adjustWaterDropInTank100(true, false, removedIndex);
            
            //100 카운트 감소
            setTank100Count(_tank100Count - 1);
            
            //숫자 사운드 플레이
            CommonSoundPlayer.cardinalNumber(_tank100Count * 100).play();
        }
        else if (pcount == 10)
        {
            int removedIndex = 0;
            
            //리스트에서 삭제
            for (vector<WaterDropTankPiece*>::iterator it = _tank10Pieces.begin(); it != _tank10Pieces.end(); )
            {
                if ((*it) == piece)
                {
                    it = _tank10Pieces.erase(it);
                    break;
                }
                else
                    ++it;
                
                removedIndex++;
            }

            //탱크내 물방울 정렬
            adjustWaterDropInTank10(true, false, removedIndex);
            
            //10 카운트 감소
            setTank10Count(_tank10Count - 1);
            
            //숫자 사운드 플레이
            CommonSoundPlayer.cardinalNumber(_tank10Count * 10).play();
        }
        else if (pcount == 1)
        {
            int removedIndex = 0;
            
            //리스트에서 삭제
            for (vector<WaterDropTankPiece*>::iterator it = _tank1Pieces.begin(); it != _tank1Pieces.end(); )
            {
                if ((*it) == piece)
                {
                    it = _tank1Pieces.erase(it);
                    break;
                }
                else
                    ++it;
                
                removedIndex++;
            }

            //탱크내 물방울 정렬
            adjustWaterDropInTank1(true, false, removedIndex);
            
            //1 카운트 감소
            setTank1Count(_tank1Count - 1);
            
            //숫자 사운드 플레이
            CommonSoundPlayer.cardinalNumber(_tank1Count * 1).play();
        }

        
        //스테이지 클리어 판정
        if ((_tank100Count*100)+(_tank10Count*10)+(_tank1Count) == _objectNo)
        {
            onStageClear();
        }
        else
        {
        }
        
        //게임 로그 남기기
        string workPath = [this] {
            stringstream SS;
            SS << "/" << "PlaceValue";
            SS << "/" << "level-" << _currentLevel << "-" << _currentSheetNo;
            SS << "/" << "work-" << _currentBoardIndex;
            return SS.str();
        }();
        
        StrictLogManager::shared()->game_Peek_Answer("PlaceValue", workPath,
                                                     TodoUtil::itos((_tank100Count*100)+(_tank10Count*10)+(_tank1Count)),
                                                     TodoUtil::itos(_objectNo));

        //기존 탱크 피스 삭제
        _gameNode->removeChild(piece);

    };
    
    piece->onLaid = [this, piece]
    {
    };
    
    //게임노드에 추가
    _gameNode->addChild(piece, 300);
    
    return piece;
}

void PlaceValueScene::adjustWaterDropInTank100(bool playAction, bool isInc, int removedIndex)
{
    for (int i = 0; i < _tank100Pieces.size(); i++)
    {
        int c = i % 5;
        int r = i / 5;
        
        auto p = _tank100Pieces.at(i);
        p->_originPos = Vec2(_gameNode->getChildByName("tank100")->getBoundingBox().getMinX() + 59 + (p->getContentSize().width/2) + (c * 222), _gameNode->getChildByName("tank100")->getPosition().y - 506 - (p->getContentSize().height/2) - (r * 242));
        p->setPosition(p->_originPos);
        
        //액션을 플레이 하지 않을때는 통과
        if (playAction == false)
            continue;
        
        //물방울 추가 연출
        if (isInc)
        {
            if (i == 5)
            {
                p->runAction(Sequence::create(MoveBy::create(0.0f, Vec2(222 * (i-1), 242)),
                                              DelayTime::create(0.05f * i),
                                              MoveBy::create(0.0f, Vec2(-222 * (i-1), -242)),
                                              MoveBy::create(0.0f, Vec2(-222, 0)),
                                              EaseBounceOut::create(MoveBy::create(0.5f, Vec2(222, 0))),
                                              nullptr));
            }
            else
            {
                p->runAction(Sequence::create(MoveBy::create(0.0f, Vec2(-222, 0)),
                                              DelayTime::create(0.05f * i),
                                              EaseBounceOut::create(MoveBy::create(0.5f, Vec2(222, 0))),
                                              nullptr));
            }
        }
        //물방울 감소 연출
        else
        {
            if (i >= removedIndex)
            {
                p->runAction(Sequence::create(MoveBy::create(0.0f, Vec2(+222, 0)),
                                              DelayTime::create(0.05f * i),
                                              EaseBounceOut::create(MoveBy::create(0.5f, Vec2(-222, 0))),
                                              nullptr));
            }
        }
    }
}

void PlaceValueScene::adjustWaterDropInTank10(bool playAction, bool isInc, int removedIndex)
{
    for (int i = 0; i < _tank10Pieces.size(); i++)
    {
        auto p = _tank10Pieces.at(i);
        p->_originPos = Vec2(_gameNode->getChildByName("tank10")->getBoundingBox().getMinX() + 48 + (p->getContentSize().width/2) + (i * 68), _gameNode->getChildByName("tank10")->getPosition().y - 505 - (p->getContentSize().height/2));
        p->setPosition(p->_originPos);
        
        //액션을 플레이 하지 않을때는 통과
        if (playAction == false)
            continue;
        
        //물방울 추가 연출
        if (isInc)
        {
            p->runAction(Sequence::create(MoveBy::create(0.0f, Vec2(-68, 0)),
                                          DelayTime::create(0.05f * i),
                                          EaseBounceOut::create(MoveBy::create(0.5f, Vec2(68, 0))),
                                          nullptr));
        }
        //물방울 감소 연출
        else
        {
            if (i >= removedIndex)
            {
                p->runAction(Sequence::create(MoveBy::create(0.0f, Vec2(+68, 0)),
                                              DelayTime::create(0.05f * i),
                                              EaseBounceOut::create(MoveBy::create(0.5f, Vec2(-68, 0))),
                                              nullptr));
            }
        }
    }
}

void PlaceValueScene::adjustWaterDropInTank1(bool playAction, bool isInc, int removedIndex)
{
    for (int i = 0; i < _tank1Pieces.size(); i++)
    {
        auto p = _tank1Pieces.at(i);
        p->_originPos = Vec2(_gameNode->getChildByName("tank1")->getBoundingBox().getMinX() + 90 + (p->getContentSize().width/2), _gameNode->getChildByName("tank1")->getPosition().y - 964 - (p->getContentSize().height/2) + (i * 58));
        p->setPosition(p->_originPos);
        
        //액션을 플레이 하지 않을때는 통과
        if (playAction == false)
            continue;

        //물방울 추가 연출
        if (isInc)
        {
            p->runAction(Sequence::create(MoveBy::create(0.0f, Vec2(0, -58)),
                                          DelayTime::create(0.05f * i),
                                          EaseBounceOut::create(MoveBy::create(0.5f, Vec2(0, 58))),
                                          nullptr));
        }
        //물방울 감소 연출
        else
        {
            if (i >= removedIndex)
            {
                p->runAction(Sequence::create(MoveBy::create(0.0f, Vec2(0, +58)),
                                              DelayTime::create(0.05f * i),
                                              EaseBounceOut::create(MoveBy::create(0.5f, Vec2(0, -58))),
                                              nullptr));
            }
        }
    }
}

void PlaceValueScene::update(float dt)
{
    auto cloud1 = _gameNode->getChildByName("cloud1");
    if (cloud1 != nullptr)
    {
        if (cloud1->getPosition().x < 0 - (cloud1->getContentSize().width/2))
            cloud1_pos = Vec2(gameSize.width + (cloud1->getContentSize().width/2), cloud1->getPosition().y);
        
        cloud1_pos = Vec2(cloud1_pos.x - 0.5f, cloud1_pos.y);
        
        cloud1->setPosition(cloud1_pos);
    }
    
    auto cloud2 = _gameNode->getChildByName("cloud2");
    if (cloud2 != nullptr)
    {
        if (cloud2->getPosition().x < 0 - (cloud2->getContentSize().width/2))
            cloud2_pos = Vec2(gameSize.width + (cloud2->getContentSize().width/2), cloud2->getPosition().y);
        
        cloud2_pos = Vec2(cloud2_pos.x - 0.5f, cloud2_pos.y);
        
        cloud2->setPosition(cloud2_pos);
    }

    auto cloud3 = _gameNode->getChildByName("cloud3");
    if (cloud3 != nullptr)
    {
        if (cloud3->getPosition().x < 0 - (cloud3->getContentSize().width/2))
            cloud3_pos = Vec2(gameSize.width + (cloud3->getContentSize().width/2), cloud3->getPosition().y);
        
        cloud3_pos = Vec2(cloud3_pos.x - 0.5f, cloud3_pos.y);
        
        cloud3->setPosition(cloud3_pos);
    }

}

////////////////////



bool WaterDropLandPiece::init()
{
    if (!Node::init()) return false;

    _snapped = false;
    _picked = false;
    _touchEnabled = false;
    
    onPicked = nullptr;
    onSnapped = nullptr;
    onLaid = nullptr;
    onHover = nullptr;
    onHoverOut = nullptr;
    
    auto *listener = EventListenerTouchOneByOne::create();
    listener->setSwallowTouches(true);
    listener->onTouchBegan = [this](Touch* T, Event* E) {
        if (_snapped) return false;
        if (!_touchEnabled) return false;
        
        auto P = getParent();
        auto pos = P->convertToNodeSpace(T->getLocation());
        
        if (this->getBoundingBox().containsPoint(pos))
        {
            PlaceValueScene* scene = (PlaceValueScene*)P->getParent();

            if (scene->_currentPickedLandPiece != nullptr || scene->_currentPickedTankPiece != nullptr)
                return false;
            
            this->setPicked(true);
            pickEffect().play();
            
            this->getParent()->reorderChild(this, this->getLocalZOrder());
            
            scene->_currentPickedLandPiece = this;
            
            return true;
        }
        return false;
    };
    
    listener->onTouchMoved = [this](Touch* T, Event* E) {
        if (_snapped) return;
        
        auto P = getParent();
        PlaceValueScene* scene = (PlaceValueScene*)P->getParent();
        
        auto pl = P->convertToNodeSpace(T->getPreviousLocation());
        auto cl = P->convertToNodeSpace(T->getLocation());
        auto delta = cl-pl;
        
        this->setPosition(this->getPosition()+delta);
        this->_touchPos = Vec2(cl.x - (this->getPosition().x - (this->getContentSize().width/2)), cl.y - (this->getPosition().y - (this->getContentSize().height/2)));
        
        //화면밖으로 나가지 않도록 설정
        if (getBoundingBox().getMinX() + 30 < 0 || getBoundingBox().getMaxX() - 30 > P->getBoundingBox().getMaxX() ||
            getBoundingBox().getMinY() + 30 < 0 || getBoundingBox().getMaxY() - 30 > P->getBoundingBox().getMaxY())
        {
            this->setPosition(this->getPosition()-delta);
        }
        
        //탱크에 물방울이 가득찼다면 못들어가게 하자
        if ((_count == 100 && scene->_tank100Count >= 9) ||
            (_count == 10 && (scene->_tank100Count >= 9 && scene->_tank10Count >= 9)) ||
            (_count == 1 && (scene->_tank100Count >= 9 && scene->_tank10Count >= 9 && scene->_tank1Count >= 9)) )
        {
            return;
        }
        
        //호버 영역에 들어오면 hover
        if (_targetNode->getBoundingBox().intersectsRect(getBoundingBox()))
        {
            this->hoverTarget();
        }
        else
        {
            this->hoverOutTarget();
        }
        
    };
    
    listener->onTouchEnded = [this](Touch* T, Event* E) {
        if (_snapped) return;
        setPicked(false);
        
        auto P = getParent();
        PlaceValueScene* scene = (PlaceValueScene*)P->getParent();
        
        scene->_currentPickedLandPiece = nullptr;

        //탱크에 물방울이 가득찼다면 못들어가게 하자
        if ((_count == 100 && scene->_tank100Count >= 9) ||
            (_count == 10 && (scene->_tank100Count >= 9 && scene->_tank10Count >= 9)) ||
            (_count == 1 && (scene->_tank100Count >= 9 && scene->_tank10Count >= 9 && scene->_tank1Count >= 9)) )
        {
            this->layTarget();
            return;
        }
        
        //스냅 영역에 들어오면 snap
        if (_targetNode->getBoundingBox().intersectsRect(getBoundingBox()))
        {
            this->snapTarget();
        }
        else
        {
            this->layTarget();
        }
    };
    
    this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);
    
    return true;
}

void WaterDropLandPiece::setTexture(int count)
{
    if (count == 100)
    {
        _dropLand = Sprite::create(resourcePath+"button-drop-on-land-hundred.png");
        _shadowLand = Sprite::create(resourcePath+"button-drop-on-land-hundred-shadow.png");
    }
    else if (count == 10)
    {
        _dropLand = Sprite::create(resourcePath+"button-drop-on-land-ten.png");
        _shadowLand = Sprite::create(resourcePath+"button-drop-on-land-ten-shadow.png");
    }
    else if (count == 1)
    {
        _dropLand = Sprite::create(resourcePath+"button-drop-on-land-one.png");
        _shadowLand = Sprite::create(resourcePath+"button-drop-on-land-one-shadow.png");
    }
    
    //piece 사이즈 설정
    setContentSize( _dropLand->getContentSize() );
    setAnchorPoint(Vec2::ANCHOR_MIDDLE);

    //스프라이트 위치 조정 및 자식으로 추가
    _dropLand->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    _shadowLand->setAnchorPoint(Vec2::ANCHOR_MIDDLE);

    _dropLand->setPosition(getContentSize()/2);
    _shadowLand->setPosition(getContentSize()/2);

    addChild(_dropLand);
    addChild(_shadowLand);
}

void WaterDropLandPiece::setPicked(bool picked)
{
    Size size = getContentSize();

    _picked = picked;

    if (picked)
    {
        _dropLand->setPosition(size.width/2, size.height/2+5);
        _shadowLand->setPosition(size.width/2+5, size.height/2-5);

        if (onPicked) {
            onPicked();
        }
    }
    else
    {
        _dropLand->setPosition(size.width/2, size.height/2);
        _shadowLand->setPosition(size.width/2, size.height/2);
    }
}

void WaterDropLandPiece::snapTarget()
{
    setPicked(false);
    _shadowLand->setVisible(false);
    _snapped = true;
    _touchEnabled = false;

    if (onSnapped) {
        onSnapped();
    }
}

void WaterDropLandPiece::layTarget()
{
    //원래 자리로 갖다놓자
    runAction(EaseOut::create(MoveTo::create(0.12, _originPos), 2.0));
    
    if (onLaid)
    {
        onLaid();
    }
}

void WaterDropLandPiece::hoverTarget()
{
    if (onHover)
    {
        onHover();
    }
}

void WaterDropLandPiece::hoverOutTarget()
{
    if (onHoverOut)
    {
        onHoverOut();
    }
}


////////////////////



bool WaterDropTankPiece::init()
{
    if (!Node::init()) return false;
    
    _snapped = false;
    _picked = false;
    _touchEnabled = false;
    
    onPicked = nullptr;
    onSnapped = nullptr;
    onLaid = nullptr;
    onHover = nullptr;
    onHoverOut = nullptr;
    
    auto *listener = EventListenerTouchOneByOne::create();
    listener->setSwallowTouches(true);
    listener->onTouchBegan = [this](Touch* T, Event* E) {
        if (_snapped) return false;
        if (!_touchEnabled) return false;
        
        auto P = getParent();
        auto pos = P->convertToNodeSpace(T->getLocation());
        
        if (this->getBoundingBox().containsPoint(pos))
        {
            PlaceValueScene* scene = (PlaceValueScene*)P->getParent();

            if (scene->_currentPickedLandPiece != nullptr || scene->_currentPickedTankPiece != nullptr)
                return false;
            
            this->setPicked(true);
            pickEffect().play();
            
            this->getParent()->reorderChild(this, this->getLocalZOrder());
            
            scene->_currentPickedTankPiece = this;
            
            return true;
        }
        return false;
    };
    
    listener->onTouchMoved = [this](Touch* T, Event* E) {
        if (_snapped) return;
        
        auto P = getParent();
        auto pl = P->convertToNodeSpace(T->getPreviousLocation());
        auto cl = P->convertToNodeSpace(T->getLocation());
        auto delta = cl-pl;
        
        this->setPosition(this->getPosition()+delta);
        this->_touchPos = Vec2(cl.x - (this->getPosition().x - (this->getContentSize().width/2)), cl.y - (this->getPosition().y - (this->getContentSize().height/2)));
        
        if (getBoundingBox().getMinX() + 30 < 0 || getBoundingBox().getMaxX() - 30 > P->getBoundingBox().getMaxX() ||
            getBoundingBox().getMinY() + 30 < 0 || getBoundingBox().getMaxY() - 30 > P->getBoundingBox().getMaxY())
        {
            this->setPosition(this->getPosition()-delta);
        }
        
        //탱크에 완전히 포함되어있으면 안나간거다
        if (_targetNode->getBoundingBox().containsPoint(Vec2(getBoundingBox().getMinX(), getBoundingBox().getMinY())) &&
            _targetNode->getBoundingBox().containsPoint(Vec2(getBoundingBox().getMinX(), getBoundingBox().getMaxY())) &&
            _targetNode->getBoundingBox().containsPoint(Vec2(getBoundingBox().getMaxX(), getBoundingBox().getMinY())) &&
            _targetNode->getBoundingBox().containsPoint(Vec2(getBoundingBox().getMaxX(), getBoundingBox().getMaxY())))
        {
            this->hoverOutTarget();
        }
        else
        {
            this->hoverTarget();
        }

    };
    
    listener->onTouchEnded = [this](Touch* T, Event* E) {
        if (_snapped) return;
        setPicked(false);
        
        auto P = getParent();
        PlaceValueScene* scene = (PlaceValueScene*)P->getParent();
        
        scene->_currentPickedTankPiece = nullptr;
        
        //탱크에 완전히 포함되어있으면 안나간거다
        if (_targetNode->getBoundingBox().containsPoint(Vec2(getBoundingBox().getMinX(), getBoundingBox().getMinY())) &&
            _targetNode->getBoundingBox().containsPoint(Vec2(getBoundingBox().getMinX(), getBoundingBox().getMaxY())) &&
            _targetNode->getBoundingBox().containsPoint(Vec2(getBoundingBox().getMaxX(), getBoundingBox().getMinY())) &&
            _targetNode->getBoundingBox().containsPoint(Vec2(getBoundingBox().getMaxX(), getBoundingBox().getMaxY())))
        {
            this->layTarget();
        }
        else
        {
            this->snapTarget();
        }
    };
    
    this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);
    
    return true;
}


void WaterDropTankPiece::setTexture(int count)
{
    if (count == 100)
    {
        _dropTank = Sprite::create(resourcePath+"button-drop-in-tank-hundred.png");
        _shadowTank = Sprite::create(resourcePath+"button-drop-in-tank-hundred-shadow.png");
    }
    else if (count == 10)
    {
        _dropTank = Sprite::create(resourcePath+"button-drop-in-tank-ten.png");
        _shadowTank = Sprite::create(resourcePath+"button-drop-in-tank-ten-shadow.png");
    }
    else if (count == 1)
    {
        _dropTank = Sprite::create(resourcePath+"button-drop-in-tank-one.png");
        _shadowTank = Sprite::create(resourcePath+"button-drop-in-tank-one-shadow.png");
    }

    //piece 사이즈 설정
    setContentSize( _dropTank->getContentSize() );
    setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    
    //스프라이트 위치 조정 및 자식으로 추가
    _dropTank->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    _shadowTank->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    
    _dropTank->setPosition(getContentSize()/2);
    _shadowTank->setPosition(getContentSize()/2);
    
    _dropTank->setName("dropTank");
    _shadowTank->setName("shadowTank");
    
    addChild(_dropTank);
    addChild(_shadowTank);
    
}

void WaterDropTankPiece::setPicked(bool picked)
{
    Size size = getContentSize();

    _picked = picked;

    if (picked)
    {
        _dropTank->setPosition(size.width/2, size.height/2+5);
        _shadowTank->setPosition(size.width/2+5, size.height/2-5);

        if (onPicked) {
            onPicked();
        }
    }
    else
    {
        _dropTank->setPosition(size.width/2, size.height/2);
        _shadowTank->setPosition(size.width/2, size.height/2);
    }
}

void WaterDropTankPiece::snapTarget()
{
    setPicked(false);
    _shadowTank->setVisible(false);
    _snapped = true;
    _touchEnabled = false;

    if (onSnapped) {
        onSnapped();
    }
}

void WaterDropTankPiece::layTarget()
{
    //원래 자리로 갖다놓자
    runAction(EaseOut::create(MoveTo::create(0.12, _originPos), 2.0));
    
    if (onLaid)
    {
        onLaid();
    }
}

void WaterDropTankPiece::hoverTarget()
{
    if (onHover)
    {
        onHover();
    }
}

void WaterDropTankPiece::hoverOutTarget()
{
    if (onHoverOut)
    {
        onHoverOut();
    }
}

