//
//  EggQuizScene.cpp
//  KitkitSchool
//
//  Created by Jaehun Jung on 29/08/2018.
//

#include <Managers/VoiceMoldManager.h>
#include "EggQuizScene.hpp"
#include "PushButton.hpp"
#include "DragButton.hpp"
#include "Slot.hpp"
#include "Speaker.hpp"
#include "EggQuizPopup.hpp"
#include "FishQuizPopup.hpp"

#include "Common/Basic/SoundEffect.h"
#include "Common/Controls/TodoSchoolBackButton.hpp"
#include "Managers/GameSoundManager.h"
#include "Managers/UserManager.hpp"
#include "Managers/LanguageManager.hpp"
#include "Managers/StrictLogManager.h"
#include "Menu/CoopScene.hpp"
#include "CCAppController.hpp"

BEGIN_NS_EGGQUIZ;

static int _tryCountFishTest;

Scene* EggQuizScene::createScene(string course, string level)
{
    
    auto scene = Scene::create();
    auto layer = EggQuizScene::create();
    layer->setCourse(course);
    layer->setLevel(level);
    scene->addChild(layer);
    return scene;
}

void EggQuizScene::setCourse(string course) { _course = course; }
void EggQuizScene::setLevel(string level) { _level = level; }

bool EggQuizScene::init() {
    if (!Layer::init()) return false;
    
    _winSize = getContentSize();
    auto bg = Sprite::create(partsPath + "pretest-layout-bg.png");
    auto bgSize = bg->getContentSize();
    bg->setScale(_winSize.height / bgSize.height);
    bg->setPosition(_winSize/2);
    bg->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    addChild(bg);

    auto backButton = TodoSchoolBackButton::create();
    backButton->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
    backButton->setPosition(Vec2(25, _winSize.height-25));
    addChild(backButton);
    
    _gameSize = Size(2560, 1800);
    _gameScale = _winSize.height / 1800;
    _gameNode = Node::create();
    _gameNode->setContentSize(_gameSize);
    _gameNode->setScale(_gameScale);
    _gameNode->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    _gameNode->setPosition(Vec2(_winSize.width/2, _winSize.height/2));
    addChild(_gameNode);
    
    if (UserManager::getInstance()->isDebugMode()) {
        auto pass = Button::create();
        pass->setTitleText("Pass");
        pass->setTitleFontName(FONT_NORMAL);
        pass->setTitleFontSize(50);
        pass->setTitleColor(Color3B::BLACK);
        pass->setPosition(Vec2(_winSize.width-200, _winSize.height-50));
        pass->addClickEventListener([this](Ref*) {
            for (int i=0; i<_problems.size(); i++) {
                _answerResult[i] = true;
            }
            this->onComplete();
        });
        addChild(pass);
        
        auto fail = Button::create();
        fail->setTitleText("Fail");
        fail->setTitleFontName(FONT_NORMAL);
        fail->setTitleFontSize(50);
        fail->setTitleColor(Color3B::BLACK);
        fail->setPosition(Vec2(_winSize.width-500, _winSize.height-50));
        fail->addClickEventListener([this](Ref*) {
            for (int i=0; i<_problems.size(); i++) {
                _answerResult[i] = false;
            }
            this->onComplete();
        });
        addChild(fail);
        
        auto next = Button::create();
        next->setTitleText("Next");
        next->setTitleFontName(FONT_NORMAL);
        next->setTitleFontSize(50);
        next->setTitleColor(Color3B::BLACK);
        next->setPosition(Vec2(_winSize.width-800, _winSize.height-50));
        next->addClickEventListener([this](Ref*) {
            checkAnswer(_answer);
        });
        addChild(next);
    }

    return true;
}

void EggQuizScene::onEnter() {
    LOGFN();
    Layer::onEnter();
    
    _currentProblemIndex = 0;
    _problems.clear();
    
    _problems = ProblemBank::getInstance()->loadProblemData(_course, _level, &_sheetNo);
    ProblemBank::getInstance()->loadDurationsheet();

    _totalProblemCount = (int)_problems.size();

    _answerResult.clear();
    for (int i=0; i<_totalProblemCount; i++) {
        _answerResult.push_back(false);
    }

    _progressBar = ProgressIndicator::create();
    _progressBar->setPosition(Vec2(_winSize.width/2, _winSize.height - _progressBar->getContentSize().height));
    _progressBar->setMax((int)_problems.size(), true);
    addChild(_progressBar);
    
    showProblem();
}

// math template

void EggQuizScene::createSingleDigitNumbersView() {
    
    auto questionBox = Scale9Sprite::create(partsPath+"pretest-questionbox.png");
    questionBox->setPreferredSize(Size(2360,1012));
    questionBox->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
    questionBox->setPosition(_gameNode->getContentSize().width/2,468);
    _gameNode->addChild(questionBox);

    createSmallSpeakerView(_problem.questionOption1);
    
    vector<Sprite*> stones;
    for (int i=0; i<_answerInt; i++) {
        auto stone = Sprite::create(partsPath+"pretest_math_acorn.png");
        Size margin = stone->getContentSize();
        stone->setRotation(random(-60.0, 60.0));
        
        Vec2 bestPos;
        float bestMinDist = -1;
        
        for (int j=0; j<10; j++) {
            Vec2 pos = Vec2(random(margin.width+300, questionBox->getContentSize().width-margin.width), random(margin.height, questionBox->getContentSize().height-margin.height));
            
            float minDist = -1;
            for (auto s : stones) {
                auto dist = s->getPosition().distance(pos);
                if (minDist<0 || dist<minDist) minDist = dist;
            }
            
            if (bestMinDist<0 || bestMinDist < minDist) {
                bestMinDist = minDist;
                bestPos = pos;
            }
        }
        stones.push_back(stone);
        stone->setPosition(bestPos);
        questionBox->addChild(stone);
    }
    
    createPushButton09Template();
}

void EggQuizScene::createRecognizeNumberView() {
    
    auto questionBox = Scale9Sprite::create(partsPath+"pretest-questionbox.png");
    questionBox->setPreferredSize(Size(2360,800));
    questionBox->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
    questionBox->setPosition(_gameNode->getContentSize().width/2,680);
    _gameNode->addChild(questionBox);
    
    auto speaker = Speaker::create(SpeakerSize::Big);
    speaker->setFileName( _problem.answer);
    speaker->setPosition(questionBox->getContentSize()/2);
    questionBox->addChild(speaker);
    speaker->setPressed(autoStartDelay);
    
    Size buttonSize = Size(542,516);
    float startX = _gameSize.width/2-638 - buttonSize.width/2;
    float posY = 92 + buttonSize.height/2;
    float space = 64;
    
    auto choices = TodoUtil::split(_problem.answerOption1, ',');
    
    for (auto it : choices) {
        it = TodoUtil::trim(it);
        auto button = PushButton::create(buttonSize, 224, it, [this](string answer){ checkAnswer(answer); });
        button->setPosition(startX, posY);
        _gameNode->addChild(button);
        startX += space + buttonSize.width;
    }
}

void EggQuizScene::create2DigitNumbersView() {

    auto questionBox = Scale9Sprite::create(partsPath+"pretest-questionbox.png");
    questionBox->setPreferredSize(Size(2360,800));
    questionBox->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
    questionBox->setPosition(_gameNode->getContentSize().width/2,680);
    _gameNode->addChild(questionBox);
    
    if (_problem.questionOption1!="") createSmallSpeakerView(_problem.questionOption1);
    
    {
        vector<Sprite*> stones;
        int numSection = (_answerInt+9) / 10;
        auto sectionSize = Size(1518, 132);
        auto space = 10;
        auto tView = Node::create();
        
        Size viewSize = questionBox->getContentSize();
        tView->setContentSize(viewSize);
        tView->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        tView->setPosition(viewSize/2);
        float sectionHeight = sectionSize.height*numSection + space*(numSection+1);
        if (sectionHeight>viewSize.height) {
            auto scale = (viewSize.height-100) / sectionHeight;
            tView->setScale(scale);
        }
        questionBox->addChild(tView);
        
        auto occ = space*(numSection-1)+sectionSize.height*numSection;
        auto top = viewSize.height - (viewSize.height-occ)/2;
        auto left = sectionSize.width - ( 112*10 + 34*9 );
        
        for (int i=0; i<numSection; i++) {
            int numInRow = 10;
            if (i==numSection-1) numInRow = _answerInt - (10*i);
            
            auto section = Sprite::create(partsPath+"pretest_math_10set_outline.png");
            section->setPosition(viewSize.width/2, top - i*(sectionSize.height+space) - sectionSize.height/2);
            tView->addChild(section);
            
            for (int j=0; j<numInRow; j++) {
                auto dot = Sprite::create(partsPath+"pretest_math_dot_yellow.png");
                dot->setPosition(left + (112+34)*j, sectionSize.height/2);
                section->addChild(dot);
            }
        }
    }
    
    {
        Size buttonSize = Size(542,516);
        float startX = _gameSize.width/2-638 - buttonSize.width/2;
        float posY = 92 + buttonSize.height/2;
        float space = 64;
        
        auto choices = TodoUtil::split(_problem.answerOption1, ',');
        
        for (auto it : choices) {
            it = TodoUtil::trim(it);
            auto button = PushButton::create(buttonSize, 224, it, [this](string answer){ checkAnswer(answer); });
            button->setPosition(startX, posY);
            _gameNode->addChild(button);
            startX += space + buttonSize.width;
        }
    }
    
}
    
void EggQuizScene::createBiggerAndSmallerView() {

    bool withImage = _problem.questionOption2 == "image" ? true : false;
    auto choices = TodoUtil::split(_problem.answerOption1, ',');
    Size buttonSize;
    Scale9Sprite* questionBox;
    
    if (withImage) {
        questionBox = Scale9Sprite::create(partsPath+"pretest-questionbox.png");
        questionBox->setPreferredSize(Size(2360,324));
        questionBox->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
        questionBox->setPosition(_gameNode->getContentSize().width/2,1156);
        _gameNode->addChild(questionBox);
        
        buttonSize = Size(1148,1000);
    } else {
        questionBox = Scale9Sprite::create(partsPath+"pretest-questionbox.png");
        questionBox->setPreferredSize(Size(2360,800));
        questionBox->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
        questionBox->setPosition(_gameNode->getContentSize().width/2,680);
        _gameNode->addChild(questionBox);
        
        buttonSize = Size(1148,516);
    }
    
    createSmallSpeakerView(_problem.questionOption5);
    
    auto label = TodoUtil::createLabel(_problem.questionOption4, 100, Size::ZERO, FONT_BOLD, FONT_COLOR);
    label->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    label->setPosition(questionBox->getContentSize()/2);
    questionBox->addChild(label);

    
    auto addImageToButton = [this](PushButton* button, int count, string imagePath, float scale = 1){
        vector<Sprite*> items;
        bool lineup = _problem.questionOption3 == "lineup" ? true : false;
        
        float startX = 36*4, startY = 192*4;
        float posX = startX, posY = startY;
        float space = 40*4;
        
        for (int i=0; i<count; i++) {
            auto newItem = Sprite::create(imagePath);
            
            if (lineup) {
                if (i && i%5 == 0) {
                    posX = startX;
                    posY -= space;
                }
                newItem->setPosition(posX, posY);
                button->addChild(newItem);
                newItem->setScale(scale);
                items.push_back(newItem);
                posX += space;
            } else {
                int j=0;
                while(1) {
                    newItem->setPosition(Vec2(random<int>(150,780),random<int>(150,780)));
                    
                    bool near = false;
                    for (auto item : items) {
                        if (item->getPosition().distance(newItem->getPosition()) < 120) {
                            near = true;
                            break;
                        }
                    }
                    if (j++!=10000 && near) continue;
                    button->addChild(newItem);
                    newItem->setScale(scale);
                    items.push_back(newItem);
                    break;
                }
            }
        }
    };
    
    auto button1 = PushButton::create(buttonSize, 224, choices[0], [this](string answer){ checkAnswer(answer); });
    button1->setPosition(_gameSize.width/2-32-buttonSize.width/2, 92+buttonSize.height/2);
    _gameNode->addChild(button1);
    
    auto button2 = PushButton::create(buttonSize, 224, choices[1], [this](string answer){ checkAnswer(answer); });
    button2->setPosition(_gameSize.width/2+32+buttonSize.width/2, 92+buttonSize.height/2);
    _gameNode->addChild(button2);

    
    if (withImage) {
        button1->_label->setVisible(false);
        button2->_label->setVisible(false);
        addImageToButton(button1, TodoUtil::stoi(choices[0]), partsPath+"pretest_math_dot_yellow.png");
        addImageToButton(button2, TodoUtil::stoi(choices[1]), partsPath+"pretest_math_acorn.png", 0.65);
    }
    
}

void EggQuizScene::createMissingNumberView() {
    
    auto questionBox = Scale9Sprite::create(partsPath+"pretest-questionbox.png");
    questionBox->setPreferredSize(Size(1136,1380));
    questionBox->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
    questionBox->setPosition(_gameSize.width/2-44-1136/2,100);
    _gameNode->addChild(questionBox);
    
    auto blank = TodoUtil::stoi(_problem.questionOption2) - 1;
    auto answerRangeVec = TodoUtil::split(_problem.answer, ',');
    
    float space = 128;
    float posX = questionBox->getContentSize().width/2 - space*1.5 - 133*1.5;
    float posY = questionBox->getContentSize().height/2;
    
    for (int i=0; i<answerRangeVec.size(); i++) {
        auto it = answerRangeVec[i];
        
        if (i == blank) {
            auto q = Sprite::create(partsPath+"pretest-math-emptyanswer-dotted-large.png");
            q->setScale(0.5);
            q->setPosition(posX,posY);
            questionBox->addChild(q);
            _answer = it;
            _answerInt = TodoUtil::stoi(it);
        } else {
            auto label = TodoUtil::createLabel(it, 128, Size::ZERO, FONT_NORMAL, FONT_COLOR);
            label->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
            label->setPosition(posX,posY);
            questionBox->addChild(label);
        }
        posX += 133 + space;

    }

    createAnswerNumberPadView();

}

void EggQuizScene::createOperationsWithObjectsView() {

    auto questionBox = Scale9Sprite::create(partsPath+"pretest-questionbox.png");
    questionBox->setPreferredSize(Size(1136,1380));
    questionBox->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
    questionBox->setPosition(_gameSize.width/2-44-1136/2,100);
    _gameNode->addChild(questionBox);
    
    auto fomulaV = ProblemBank::getInstance()->parseFomula(_problem.questionOption1);
    
    float fomulaSpace = 40;
    float ObjectSpace = 40;
    
    Node *fomulaNode = Node::create();
    float fomulaNodeWidth = 0;
    float fomulaNodeHeight = 150;
    
    float objectNodeFitWidth = 800;
    
    for (auto it : fomulaV) {
        if (it[0] == '?') {
            auto q = Sprite::create(partsPath+"pretest-math-emptyanswer-dotted-large.png");
            q->setScale(0.6);
            fomulaNode->addChild(q);
            q->setPosition(fomulaNodeWidth + q->getContentSize().width/2, -10);
            fomulaNodeWidth += q->getContentSize().width + fomulaSpace;
        } else {
            auto l = TodoUtil::createLabel(it, fomulaNodeHeight, Size::ZERO, FONT_NORMAL, FONT_COLOR);
            fomulaNode->addChild(l);
            l->setPosition(fomulaNodeWidth + l->getContentSize().width/2, 0);
            fomulaNodeWidth += l->getContentSize().width + fomulaSpace;
            CCLOG("width:%f", l->getContentSize().width);
            CCLOG("objectsNodeWidth:%f", fomulaNodeWidth);
        }
    }
    fomulaNode->setContentSize(Size(fomulaNodeWidth - fomulaSpace, fomulaNodeHeight));
    fomulaNode->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    questionBox->addChild(fomulaNode);
    
    Node *objectsNode = Node::create();
    
    vector<char> ballBox;
    
    int yellowBall = 0;
    int yellowDottedBall = 0;
    int blueBall = 0;
    
    int num1 = TodoUtil::stoi(fomulaV[0]);
    string op = fomulaV[1];
    int num2 = TodoUtil::stoi(fomulaV[2]);
    
    if (op == "+") {
        yellowBall = num1;
        blueBall = num2;
    } else {
        yellowBall = num1-num2;
        yellowDottedBall = num2;
    }
    
    while (yellowBall--) ballBox.push_back('y');
    while (yellowDottedBall--) ballBox.push_back('d');
    while (blueBall--) ballBox.push_back('b');
    
    
    float objectsNodeX = 0.f;
    float objectsNodeY = 0.f;
    float objectsNodeWidth = 0.f;
    float objectsNodeHeight = 0.f;
    
    for (int i=0; i<ballBox.size(); i++) {
        auto it = ballBox.at(i);
        Sprite* b;
        switch(it) {
            case 'y': b = Sprite::create(partsPath+"pretest_math_dot_yellow.png"); break;
            case 'd': b = Sprite::create(partsPath+"pretest_math_dot_yellow_empty.png"); break;
            case 'b': b = Sprite::create(partsPath+"pretest_math_dot_blue.png"); break;
        }
        b->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
        if (i==1) objectsNodeHeight = b->getContentSize().height;
        if (i!=0 && i%10 == 0) {
            objectsNodeX = 0;
            objectsNodeY -= b->getContentSize().height + ObjectSpace;
            objectsNodeHeight += b->getContentSize().height + ObjectSpace;
        }
        
        b->setPosition(objectsNodeX, objectsNodeY);
        objectsNodeX += b->getContentSize().width + ObjectSpace;
        objectsNode->addChild(b);
        
        if (objectsNodeX > objectsNodeWidth) objectsNodeWidth = objectsNodeX;
    }
    
    objectsNode->setContentSize(Size(objectsNodeWidth - ObjectSpace, -objectsNodeHeight));
    objectsNode->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    auto objectScale = objectNodeFitWidth/objectsNodeWidth;
    objectsNode->setScale(objectScale);
    
    questionBox->addChild(objectsNode);

    auto viewSize = questionBox->getContentSize();
    fomulaNode->setPosition(viewSize.width/2,viewSize.height/2-200);
    objectsNode->setPosition(viewSize.width/2,viewSize.height/2+240);
    
    createAnswerNumberPadView();
}

void EggQuizScene::createOperationsWithoutObjectsView() {

    auto questionBox = Scale9Sprite::create(partsPath+"pretest-questionbox.png");
    questionBox->setPreferredSize(Size(1136,1380));
    questionBox->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
    questionBox->setPosition(_gameSize.width/2-44-1136/2,100);
    _gameNode->addChild(questionBox);
    
    auto fomulaV = ProblemBank::getInstance()->parseFomula(_problem.questionOption1);
    
    if (_problem.questionOption3 == "vertical") {
        if (fomulaV.size() != 5) return;

        Node *fomulaNode = Node::create();
        
        float fomulaNodeX = 0;
        float fomulaNodeY = 300;
        float lineHeight = 270;
        
        auto num1 = TodoUtil::createLabel(fomulaV[0], 210, Size(600,210), FONT_NORMAL, FONT_COLOR, TextHAlignment::RIGHT);
        num1->setPosition(fomulaNodeX, fomulaNodeY);
        fomulaNode->addChild(num1);
        
        auto num2 = TodoUtil::createLabel(fomulaV[2], 210, Size(600,210), FONT_NORMAL, FONT_COLOR, TextHAlignment::RIGHT);
        num2->setPosition(num1->getPositionX(), num1->getPositionY()-lineHeight);
        fomulaNode->addChild(num2);
        
        auto op = TodoUtil::createLabel(fomulaV[1], 210, Size(600,210), FONT_NORMAL, FONT_COLOR, TextHAlignment::RIGHT);
        op->setPosition(num1->getPositionX()-560, num1->getPositionY()-lineHeight);
        fomulaNode->addChild(op);
        
        auto stroke = LayerColor::create(Color4B(70, 70, 70, 255), 700, 12);
        stroke->setPosition(-400,-150);
        fomulaNode->addChild(stroke);
        
        auto qb = Sprite::create(partsPath+"pretest-math-emptyanswer-dotted-large.png");
        fomulaNode->addChild(qb);
        qb->setAnchorPoint(Vec2::ANCHOR_TOP_RIGHT);
        qb->setPosition(290, -228);
        qb->setScale(0.8);
        
        fomulaNode->setContentSize(Size(fomulaNodeX, lineHeight*3));
        fomulaNode->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        fomulaNode->setPosition(questionBox->getContentSize().width/2,questionBox->getContentSize().height/2 + lineHeight*3/2);
        questionBox->addChild(fomulaNode);
        
    } else if (_problem.questionOption3 == "horizontal") {
        
        float fomulaSpace = 40;
        
        Node *fomulaNode = Node::create();
        float fomulaNodeWidth = 0;
        float fomulaNodeHeight = 150;
        
        for (auto it : fomulaV) {
            if (it[0] == '?') {
                auto q = Sprite::create(partsPath+"pretest-math-emptyanswer-dotted-large.png");
                q->setScale(0.8);
                fomulaNode->addChild(q);
                q->setPosition(fomulaNodeWidth + q->getContentSize().width/2, -10);
                fomulaNodeWidth += q->getContentSize().width + fomulaSpace;
            } else {
                auto l = TodoUtil::createLabel(it, fomulaNodeHeight, Size::ZERO, FONT_NORMAL, FONT_COLOR);
                fomulaNode->addChild(l);
                l->setPosition(fomulaNodeWidth + l->getContentSize().width/2, 0);
                fomulaNodeWidth += l->getContentSize().width + fomulaSpace;
            }
        }
        fomulaNode->setContentSize(Size(fomulaNodeWidth - fomulaSpace, fomulaNodeHeight));
        fomulaNode->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        questionBox->addChild(fomulaNode);
        
        auto fomulaScale = 800/fomulaNodeWidth;
        fomulaNode->setScale(fomulaScale < 3 ? fomulaScale : 3);
        fomulaNode->setPosition(questionBox->getContentSize().width/2,questionBox->getContentSize().height/2 + fomulaNodeHeight/2);
    }
    
    createAnswerNumberPadView();
}

void EggQuizScene::createBiggestAndSmallestView() {

    auto questionBox = Scale9Sprite::create(partsPath+"pretest-questionbox.png");
    questionBox->setPreferredSize(Size(2360,800));
    questionBox->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
    questionBox->setPosition(_gameNode->getContentSize().width/2,680);
    _gameNode->addChild(questionBox);
    
    createSmallSpeakerView(_problem.questionOption3);
    
    auto label = TodoUtil::createLabel(_problem.questionOption2, 128, Size::ZERO, FONT_BOLD, FONT_COLOR);
    label->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    label->setPosition(questionBox->getContentSize()/2);

    questionBox->addChild(label);
    
    Size buttonSize = Size(542,516);
    float startX = _gameSize.width/2-638 - buttonSize.width/2;
    float posY = 92 + buttonSize.height/2;
    float space = 64;
    
    auto choices = TodoUtil::split(_problem.answerOption1, ',');
    
    for (auto it : choices) {
        it = TodoUtil::trim(it);
        auto button = PushButton::create(buttonSize, 224, it, [this](string answer){ checkAnswer(answer); });
        button->setPosition(startX, posY);
        _gameNode->addChild(button);
        startX += space + buttonSize.width;
    }

}

void EggQuizScene::createCompareNumberMagnitudesView() {
    
    auto questionBox = Scale9Sprite::create(partsPath+"pretest-questionbox.png");
    questionBox->setPreferredSize(Size(2360,800));
    questionBox->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
    questionBox->setPosition(_gameNode->getContentSize().width/2,680);
    _gameNode->addChild(questionBox);

    auto speaker = Speaker::create(SpeakerSize::Small);
    speaker->setFileName(_problem.questionOption3);
    speaker->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    speaker->setPosition(_gameSize.width/2-1032-200/2,1311+220/2);
    _gameNode->addChild(speaker);
    speaker->setPressed(autoStartDelay);

    auto label = TodoUtil::createLabel(_problem.questionOption2, 68, Size::ZERO, FONT_BOLD, FONT_COLOR);
    label->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    label->setPosition(_gameSize.width/2, 1325+71/2);
    _gameNode->addChild(label);
    
    float cardPosX, slotPosX, cardPosY, slotPosY;
    cardPosX = slotPosX = _gameSize.width/2 - 561 - 484/2;
    cardPosY = slotPosY = 744 + 512/2;
    
    auto answerVec = TodoUtil::split(_problem.answer, ',');
    for (auto it : answerVec) {
        auto slot = Slot::create(SlotSize::Big);
        slot->setAnswer(it);
        slot->setPosition(slotPosX, slotPosY);
        _gameNode->addChild(slot);
        _slots.push_back(slot);
        slotPosX += slot->getContentSize().width + 51;
    }
    
    cardPosY -= 130 + 512;
    
    function<void()> onTouchMoved = [this](){
        checkDragButtonOver();
    };
    
    function<void()> onTouchEnded = [this](){
        checkDragButton();
        checkSlotFull();
    };
    
    auto cardVec = TodoUtil::split(_problem.questionOption1, ',');
    for (auto it : cardVec) {
        auto button = DragButton::create(it, Size(472,503), 224, _slots, onTouchMoved, onTouchEnded);
        button->setOriginalPos(Vec2(cardPosX,cardPosY));
        button->disableDuplicate();
        _gameNode->addChild(button);
        cardPosX += button->getContentSize().width + 64;
    }
}

void EggQuizScene::createUseMagnitudeSymbolsView() {

    int number1 = TodoUtil::stoi(_problem.questionOption1);
    int number2 = TodoUtil::stoi(_problem.questionOption2);
    
    if (number1 > number2) _answer = ">";
    else if (number1 < number2) _answer = "<";
    else _answer = "=";

    auto questionBox = Scale9Sprite::create(partsPath+"pretest-questionbox.png");
    questionBox->setPreferredSize(Size(2360,800));
    questionBox->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
    questionBox->setPosition(_gameNode->getContentSize().width/2,680);
    _gameNode->addChild(questionBox);
    
    createSmallSpeakerView(_problem.questionOption4);

    auto label = TodoUtil::createLabel(_problem.questionOption3, 100, Size::ZERO, FONT_BOLD, FONT_COLOR);
    label->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
    label->setPosition(_gameSize.width/2-224-652, 1292+80/2);
    _gameNode->addChild(label);
    
    auto numberLabel1 = TodoUtil::createLabel(_problem.questionOption1, 224, Size::ZERO, FONT_NORMAL, FONT_COLOR);
    numberLabel1->setAnchorPoint(Vec2::ANCHOR_MIDDLE_RIGHT);
    numberLabel1->setPosition(_gameSize.width/2-238, 908+166/2);
    _gameNode->addChild(numberLabel1);
    
    auto numberLabel2 = TodoUtil::createLabel(_problem.questionOption2, 224, Size::ZERO, FONT_NORMAL, FONT_COLOR);
    numberLabel2->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
    numberLabel2->setPosition(_gameSize.width/2+238, 908+166/2);
    _gameNode->addChild(numberLabel2);
    
    auto qb = Sprite::create(partsPath+"pretest-math-emptyanswer-dotted-large.png");
    qb->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
    qb->setPosition(_gameSize.width/2, 831);
    _gameNode->addChild(qb);
    
    vector<string> operators = {">", "<", "="};
    Size buttonSize = Size(744,516);
    
    float posX = _gameSize.width/2 - 436 - buttonSize.width/2;
    float posY = 92 + buttonSize.height/2;
    
    for (auto it : operators) {
        auto button = PushButton::create(buttonSize, 224, it, [this](string answer){ checkAnswer(answer); });
        button->setPosition(posX, posY);
        _gameNode->addChild(button);
        posX += 64 + buttonSize.width;
    }
}

void EggQuizScene::createShapesView() {
        
    auto questionBox = Scale9Sprite::create(partsPath+"pretest-questionbox.png");
    questionBox->setPreferredSize(Size(2360,800));
    questionBox->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
    questionBox->setPosition(_gameNode->getContentSize().width/2,680);
    _gameNode->addChild(questionBox);
    
    createSmallSpeakerView(_problem.questionOption3);
    
    auto label = TodoUtil::createLabel(_problem.questionOption2, 100, Size::ZERO, FONT_BOLD, FONT_COLOR);
    label->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    label->setPosition(questionBox->getContentSize()/2);
    questionBox->addChild(label);
    
    if (_problem.questionOption1 == "what") {
        
        label->setPosition(questionBox->getContentSize()/2+Size(0,250));
        
        auto sample = Sprite::create(imagesPath + _problem.answer + ".png");
        sample->setPosition(questionBox->getContentSize()/2+Size(0,-150));
        questionBox->addChild(sample);

    } else {
        
    }
    
    Size buttonSize = Size(542,516);
    float startX = _gameSize.width/2-638 - buttonSize.width/2;
    float posY = 92 + buttonSize.height/2;
    float space = 64;
    
    auto choices = TodoUtil::split(_problem.answerOption1, ',');
    
    if (_problem.questionOption1 == "what") {

        vector<string> labels;
        for (auto it : choices) {
            labels.push_back(it);
        }
        float fontSize = getMinFontSize(labels, 100, buttonSize);

        for (int i=0; i<choices.size(); i++) {
            auto it = TodoUtil::trim(choices[i]);
            TodoUtil::replaceAll(it, "_", "\n");
            auto button = PushButton::create(buttonSize, fontSize, it, [this](string answer){ TodoUtil::replaceAll(answer, "\n", "_"); checkAnswer(answer); });
            button->setPosition(startX, posY);
            _gameNode->addChild(button);
            startX += space + buttonSize.width;
        }

    } else {
        
        for (auto it : choices) {
            auto button = PushButton::create(buttonSize, 100, it, [this](string answer){ checkAnswer(answer); });
            button->setPosition(startX, posY);
            button->setImage();
            _gameNode->addChild(button);
            startX += space + buttonSize.width;
        }
        
    }
    
}

void EggQuizScene::create3DigitNumbersView() {
    
    auto questionBox = Scale9Sprite::create(partsPath+"pretest-questionbox.png");
    questionBox->setPreferredSize(Size(1136,1380));
    questionBox->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
    questionBox->setPosition(_gameSize.width/2-44-1136/2,100);
    _gameNode->addChild(questionBox);
    
    createSmallSpeakerView(_problem.questionOption1);

    float posX = 0;
    float posY = 0;
    float space = -50;
    
    auto ballNode = Node::create();
    ballNode->setPosition(355,1006);
    ballNode->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
    for (int i=0; i<100; i++) {
        auto blueBall = Sprite::create(partsPath+"pretest_math_dot_blue.png");
        
        if (i && i%10 == 0) {
            posX = 0;
            posY -= blueBall->getContentSize().height + space;
        }
        
        blueBall->setPosition(posX, posY);
        blueBall->setScale(0.5);
        posX += blueBall->getContentSize().width + space;
        ballNode->addChild(blueBall);
        
    }
    
    int addBallCount = _answerInt - 100;
    posX = 670; posY = 0;
    
    for (int i=0; i<addBallCount; i++) {
        auto blueBall = Sprite::create(partsPath+"pretest_math_dot_blue.png");
        blueBall->setPosition(posX, posY);
        blueBall->setScale(0.5);
        posY -= blueBall->getContentSize().height + space;
        ballNode->addChild(blueBall);
    }
    
    _gameNode->addChild(ballNode);
    
    createAnswerNumberPadView();

}

void EggQuizScene::createNumberIdentificationDragView() {
    
    auto questionBox = Scale9Sprite::create(partsPath+"pretest-questionbox.png");
    questionBox->setPreferredSize(Size(2360,800));
    questionBox->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
    questionBox->setPosition(_gameNode->getContentSize().width/2,680);
    _gameNode->addChild(questionBox);
    
    createSmallSpeakerView(soundsNumberPath+_problem.answer);
        
    Size slotSize = Slot::create(SlotSize::Small)->getContentSize();
    float space = 44;
    float startX = _gameSize.width/2 - (slotSize.width*(_problem.answer.size()-1)+space*(_problem.answer.size()-1))/2;
    
    for (int i=0; i<_problem.answer.size(); i++) {
        auto slot1 = Slot::create(SlotSize::Small);
        slot1->setAnswer(_problem.answer.substr(i,1));
        slot1->setPosition(startX, 818+slotSize.height/2);
        _gameNode->addChild(slot1);
        _slots.push_back(slot1);
        startX += slotSize.width + space;
    }
    
    createDragButton09Template();
}

void EggQuizScene::createMissingNumberDragView() {
    
    auto mode = missingNumberMode::Normal;
    auto numbers = TodoUtil::split(_problem.answer, ',');
    for (auto it : numbers) {
        it = TodoUtil::trim(it);
        if (it.size() == 3) mode = missingNumberMode::Expanded;
    }
    
    Size boxSize, slotBoxSize;
    
    if (mode == missingNumberMode::Normal) {
        boxSize = Size(542,1012);
        slotBoxSize = boxSize;
    } else if (mode == missingNumberMode::Expanded) {
        boxSize = Size(462,1012);
        slotBoxSize = Size(782,1012);
    }
    
    float startX = _gameSize.width/2 - (718 + 462);
    float posY = 468 + boxSize.height/2;
    
    for (int i=0; i<numbers.size(); i++) {
        auto it = TodoUtil::trim(numbers.at(i));
        bool isSlot = i==TodoUtil::stoi(_problem.questionOption2)-1 ? true : false;
        auto currentBoxSize = isSlot ? slotBoxSize : boxSize;
        auto box = Scale9Sprite::create(partsPath+"pretest-questionbox.png");
        box->setPreferredSize(currentBoxSize);
        box->setPosition(startX + currentBoxSize.width/2, posY);
        _gameNode->addChild(box);
        
        if (isSlot) {
            
            float slotStartX = 0;
            switch(it.size()) {
                case 1: slotStartX = slotBoxSize.width/2; break;
                case 2: slotStartX = slotBoxSize.width/2 - 22 - 102; break;
                case 3: slotStartX = slotBoxSize.width/2 - 44 - 204; break;
            }
            
            for (int i=0; i < it.size(); i++) {
                auto slotValue = _problem.answer.substr(i,1);
                auto slot = Slot::create(SlotSize::Small);
                slot->setAnswer(slotValue);
                slot->setPosition(Vec2(slotStartX, 350+312/2) + box->getPosition() - box->getContentSize()/2);
                _gameNode->addChild(slot);
                _slots.push_back(slot);
                
                slotStartX += 204 + 44;
            }

            _answer = it;
            _answerInt = TodoUtil::stoi(it);

            
        } else {
            auto label = TodoUtil::createLabel(it, 224, Size::ZERO, FONT_NORMAL, FONT_COLOR);
            label->setPosition(currentBoxSize/2);
            box->addChild(label);
        }
        startX += currentBoxSize.width + 64;
    }
    
    createDragButton09Template();
    
}

void EggQuizScene::createOperationsDragView() {
    
    auto questionBox = Scale9Sprite::create(partsPath+"pretest-questionbox.png");
    questionBox->setPreferredSize(Size(2360,1012));
    questionBox->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
    questionBox->setPosition(_gameNode->getContentSize().width/2,468);
    _gameNode->addChild(questionBox);
        
    if (_problem.questionOption3 == "horizontal") {
        auto fomulaVec = TodoUtil::split(_problem.questionOption1, '?');
        auto fomula = TodoUtil::trim(fomulaVec.at(0));
        
        auto label = TodoUtil::createLabel(fomula, 224, Size::ZERO, FONT_NORMAL, FONT_COLOR);
        _gameNode->addChild(label);
        
        float totalFomulaWidth = label->getContentSize().width + _problem.answer.size()*SlotSizeSmall.width + (_problem.answer.size()-1)*24 + 98;
        label->setPosition(_gameSize.width/2 - totalFomulaWidth/2 + label->getContentSize().width/2, 891 + 166/2);
        
        float slotStartX = label->getPositionX()+label->getContentSize().width/2 + 98 + SlotSizeSmall.width/2;
        
        for (int i=0; i < _problem.answer.size(); i++) {
            auto slotValue = _problem.answer.substr(i,1);
            auto slot = Slot::create(SlotSize::Small);
            slot->setAnswer(slotValue);
            slot->setPosition(slotStartX, 818 + SlotSizeSmall.height/2);
            _gameNode->addChild(slot);
            _slots.push_back(slot);
            
            slotStartX += SlotSizeSmall.width + 24;
        }
        
    } else if (_problem.questionOption3 == "vertical") {
        char delimiter = '+';
        if (_problem.questionOption1.find("-") != string::npos) delimiter = '-';
        auto fomulaVec = TodoUtil::split(_problem.questionOption1, '=');
        auto numberVec = TodoUtil::split(fomulaVec[0], delimiter);
        
        float startY = 766+166/2;
        
        for (auto it : numberVec) {
            it = TodoUtil::trim(it);
            float startX = 1343+111/2;
            
            for (int i=(int)it.size()-1; i>-1; i--) {
                auto it2 = it.substr(i,1);
                auto label = TodoUtil::createLabel(it2, 224, Size::ZERO, FONT_NORMAL, FONT_COLOR);
                label->setPosition(startX, startY);
                questionBox->addChild(label);
                startX -= 111 + 63;
            }
            startY -= 98 + 166;
            
        }
        string delimiterStr = {delimiter};
        
        auto stroke = LayerColor::create(FONT_COLOR, 730, 16);
        stroke->setPosition(_gameSize.width/2 - stroke->getContentSize().width/2, 896);
        _gameNode->addChild(stroke);
        
        auto delimiterLabel = TodoUtil::createLabel(delimiterStr, 224, Size::ZERO, FONT_NORMAL, FONT_COLOR);
        delimiterLabel->setPosition(Vec2(857,529)+Vec2(49,49));
        questionBox->addChild(delimiterLabel);
        
        float slotStartX = 1409 + SlotSizeSmall.width/2;
        if (_problem.answer.size()>1) slotStartX -= (SlotSizeSmall.width + 37) * (_problem.answer.size()-1);
        
        for (int i=0; i<_problem.answer.size(); i++) {
            
            auto slotValue = _problem.answer.substr(i,1);
            auto slot = Slot::create(SlotSize::Small);
            slot->setAnswer(slotValue);
            slot->setPosition(slotStartX, 522 + SlotSizeSmall.height/2);
            _gameNode->addChild(slot);
            _slots.push_back(slot);
            
            slotStartX += SlotSizeSmall.width + 44;
        }
    }
    
    createDragButton09Template();
    
}

void EggQuizScene::createWordProblemView() {
    auto questionBox = Scale9Sprite::create(partsPath+"pretest-questionbox.png");
    questionBox->setPreferredSize(Size(2360,1012));
    questionBox->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
    questionBox->setPosition(_gameNode->getContentSize().width/2,468);
    _gameNode->addChild(questionBox);
    
    createSmallSpeakerView(_problem.questionOption2);
        
    auto label = TodoUtil::createLabel(_problem.questionOption1, 68, Size(1945,0), FONT_BOLD, FONT_COLOR);
    label->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
    label->setPosition(questionBox->getPositionX()-questionBox->getContentSize().width/2+294+20, 1244+158/2);
    _gameNode->addChild(label);
    
    auto labelHeight = label->getContentSize().height;
    float limit = 158;
    if (labelHeight > limit) {
        label->setPosition(label->getPosition() - Vec2(0, (labelHeight-limit)/2));
    }
    
    float totalSlotWidth = _problem.answer.size()*SlotSizeSmall.width + (_problem.answer.size()-1)*24;
    float startX = _gameSize.width/2 - totalSlotWidth/2 + SlotSizeSmall.width/2;
    
    for (int i=0; i < _problem.answer.size(); i++) {
        auto slotValue = _problem.answer.substr(i,1);
        auto slot = Slot::create(SlotSize::Small);
        slot->setAnswer(slotValue);
        slot->setPosition(startX, 694 + SlotSizeSmall.height/2);
        _gameNode->addChild(slot);
        _slots.push_back(slot);
        
        startX += SlotSizeSmall.width + 24;
    }
    
    createDragButton09Template();
    
}

void EggQuizScene::createSoundonlyWordView() {
    
    auto questionBox = Scale9Sprite::create(partsPath+"pretest-questionbox.png");
    questionBox->setPreferredSize(Size(2360,800));
    questionBox->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
    questionBox->setPosition(_gameNode->getContentSize().width/2,680);
    _gameNode->addChild(questionBox);
    
    auto speaker = Speaker::create(SpeakerSize::Big);
    speaker->setFileName( _problem.questionOption5);
    speaker->setPosition(questionBox->getContentSize()/2);
    questionBox->addChild(speaker);
    speaker->setPressed(autoStartDelay);
    
    createPushButtonC3WTemplate();    
}

void EggQuizScene::createWordWordView() {

    auto questionBox = Scale9Sprite::create(partsPath+"pretest-questionbox.png");
    questionBox->setPreferredSize(Size(2360,800));
    questionBox->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
    questionBox->setPosition(_gameNode->getContentSize().width/2,680);
    _gameNode->addChild(questionBox);
    
    if (!_problem.questionOption5.empty()) {
        createSmallSpeakerView(_problem.questionOption5);
    }
    
    Label* questionLabel = TodoUtil::createLabel(_problem.questionOption2, 224, Size::ZERO, FONT_NORMAL, FONT_COLOR);
    questionLabel->setPosition(questionBox->getContentSize()/2);
    questionBox->addChild(questionLabel);

    createPushButtonC3WTemplate();    
    
}

void EggQuizScene::createImageWordView() {
    auto questionBox = Scale9Sprite::create(partsPath+"pretest-questionbox.png");
    questionBox->setPreferredSize(Size(2360,800));
    questionBox->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
    questionBox->setPosition(_gameNode->getContentSize().width/2,680);
    _gameNode->addChild(questionBox);
    
    auto image = Sprite::create(imagesPath+_problem.questionOption2);
    image->setPosition(questionBox->getContentSize()/2);
    questionBox->addChild(image);
    Size imageSize = image->getContentSize();
    
    if (!_problem.questionOption1.empty()) {

        float margin = 100;
        float widthLimit = 400;
        image->setScale(widthLimit/imageSize.width);
        Label* questionLabel = TodoUtil::createLabel(_problem.questionOption1, 152, Size::ZERO, FONT_NORMAL, FONT_COLOR);
        questionBox->addChild(questionLabel);
        
        auto contentWidth = widthLimit + margin + questionLabel->getContentSize().width;
        auto posX = questionBox->getContentSize().width/2 - contentWidth/2;
        auto posY = questionBox->getContentSize().height/2;
        
        image->setPosition(posX+widthLimit/2, posY);
        
        questionLabel->setPosition(posX+widthLimit+margin+questionLabel->getContentSize().width/2, posY);
        
    } else {

        Size questionBoxLimitSize = questionBox->getContentSize()*0.8;
        image->setScale(MIN(questionBoxLimitSize.width/imageSize.width, questionBoxLimitSize.height/imageSize.height));
        
    }

    createPushButtonC3WTemplate(100.f);    
}

void EggQuizScene::createSentenceWordView() {
    
    Label* questionLabel;
    auto questionBox = Scale9Sprite::create(partsPath+"pretest-questionbox.png");
    questionBox->setPreferredSize(Size(2360,808));
    questionBox->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
    questionBox->setPosition(_gameNode->getContentSize().width/2,680);
    _gameNode->addChild(questionBox);
    
    createSmallSpeakerView(_problem.questionOption5);

    if (_problem.questionOption2.back() == '.'|| _problem.questionOption2.back() == '?'|| _problem.questionOption2.back() == '!'|| _problem.questionOption2.back() == '_') {
        questionLabel = TodoUtil::createLabelMultilineToFitWidth(_problem.questionOption2, 100, Size(1600,600), FONT_NORMAL, FONT_COLOR, TextHAlignment::CENTER);
    } else {
        questionLabel = TodoUtil::createLabelMultilineToFitWidth(_problem.questionOption2, 100, Size(1600,600), FONT_BOLD, FONT_COLOR, TextHAlignment::CENTER);
    }
    questionLabel->setPosition(questionBox->getContentSize()/2);
    questionBox->addChild(questionLabel);

    createPushButtonC3WTemplate(100.f);    
}

void EggQuizScene::createSentenceSentenceView() {

    Label* questionLabel;
    auto questionBox = Scale9Sprite::create(partsPath+"pretest-questionbox.png");
    questionBox->setPreferredSize(Size(2360,620));
    questionBox->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
    questionBox->setPosition(_gameNode->getContentSize().width/2,860);
    _gameNode->addChild(questionBox);

    createSmallSpeakerView(_problem.questionOption5);

    if (_problem.questionOption2.back() == '.'|| _problem.questionOption2.back() == '?'|| _problem.questionOption2.back() == '"'|| _problem.questionOption2.back() == '_') {
        questionLabel = TodoUtil::createLabelMultilineToFitWidth(_problem.questionOption2, 100, Size(1600,600), FONT_NORMAL, FONT_COLOR, TextHAlignment::CENTER);
    } else {
        questionLabel = TodoUtil::createLabelMultilineToFitWidth(_problem.questionOption2, 100, Size(1600,600), FONT_BOLD, FONT_COLOR, TextHAlignment::CENTER);
    }
    questionLabel->setPosition(questionBox->getContentSize()/2);
    questionBox->addChild(questionLabel);
    
    createPushButtonC3STemplate();
}

void EggQuizScene::createImageSentenceView() {

    auto questionBox = Scale9Sprite::create(partsPath+"pretest-questionbox.png");
    questionBox->setPreferredSize(Size(2360,620));
    questionBox->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
    questionBox->setPosition(_gameNode->getContentSize().width/2,860);
    _gameNode->addChild(questionBox);

    createSmallSpeakerView(_problem.questionOption5);
    
    auto image = Sprite::create(imagesPath+_problem.questionOption2);
    image->setPosition(questionBox->getContentSize()/2);
    questionBox->addChild(image);
    Size imageSize = image->getContentSize();
    Size questionBoxLimitSize = questionBox->getContentSize()*0.8;
    image->setScale(MIN(questionBoxLimitSize.width/imageSize.width, questionBoxLimitSize.height/imageSize.height));

    createPushButtonC3STemplate();    
}

void EggQuizScene::createSoundonlySentenceView() {

    auto questionBox = Scale9Sprite::create(partsPath+"pretest-questionbox.png");
    questionBox->setPreferredSize(Size(2360,620));
    questionBox->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
    questionBox->setPosition(_gameNode->getContentSize().width/2,860);
    _gameNode->addChild(questionBox);
    
    auto speaker = Speaker::create(SpeakerSize::Big);
    speaker->setFileName( _problem.questionOption5);
    speaker->setPosition(questionBox->getContentSize()/2);
    questionBox->addChild(speaker);
    speaker->setPressed(autoStartDelay);
    
    createPushButtonC3STemplate();    
}

void EggQuizScene::createParagraphSentenceView() {

    Size buttonSize = Size(1137, 320);
    float posX = _gameSize.width/2+43+buttonSize.width/2;
    float paragraphWeight = 0;
    float buttonWeight = 0;
    float questionLabelWeight = 0;

    auto questionBox = Scale9Sprite::create(partsPath+"pretest-questionbox.png");
    questionBox->setPreferredSize(Size(1136,1380));
    questionBox->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
    questionBox->setPosition(_gameNode->getContentSize().width/2-44-questionBox->getContentSize().width/2,100);
    _gameNode->addChild(questionBox);

    if (!_problem.questionOption5.empty()) {
        
        if (_problem.questionOption5.find(",")!=string::npos) {
            auto soundFiles = TodoUtil::split(_problem.questionOption5, ',');
            vector<string> outputV;
            for (auto it : soundFiles) {
                outputV.push_back(it);
            }
            createSmallSpeakerView(ProblemBank::getInstance()->getJoinString(outputV), Vec2(_gameSize.width/2+200+35, 1237));
            questionLabelWeight += 140;

        } else {
            createSmallSpeakerView(_problem.questionOption5);
            paragraphWeight -= 120;
        }
    }
    
    if (_problem.questionOption1.empty()) {
        buttonWeight += 175;
    }

    Label* paragraphLabel = TodoUtil::createLabelMultilineToFit(_problem.questionOption2, 80, Size(questionBox->getContentSize().width*0.9, questionBox->getContentSize().height*0.8), FONT_NORMAL, FONT_COLOR, TextHAlignment::LEFT);
    paragraphLabel->setPosition(questionBox->getContentSize()/2 + Size(0, paragraphWeight));
    questionBox->addChild(paragraphLabel);
    
    Label* questionLabel = TodoUtil::createLabelMultilineToFitWidth(_problem.questionOption1, questionLabelWeight ? 88 : 100, Size(1060-questionLabelWeight, 400), FONT_NORMAL, FONT_COLOR, questionLabelWeight ? TextHAlignment::LEFT :TextHAlignment::CENTER);
    questionLabel->setPosition(questionLabelWeight ? posX+questionLabelWeight/2 : posX, 1245+215/2);
    _gameNode->addChild(questionLabel);
    
    auto choices = TodoUtil::split(_problem.answerOption1, '^');
    float posY = 812 + buttonSize.height/2 + buttonWeight;
    
    for (auto it : choices) {
        it = TodoUtil::trim(it);
        auto button = PushButton::create(buttonSize, 68, it, [this](string answer){ checkAnswer(answer); });
        button->setPosition(posX, posY);
        _gameNode->addChild(button);
        posY -= 40 + buttonSize.height;
    }
    
}

void EggQuizScene::createImageseqSentenceView() {
    
    auto questionBox = Scale9Sprite::create(partsPath+"pretest-questionbox.png");
    questionBox->setPreferredSize(Size(2360,620));
    questionBox->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
    questionBox->setPosition(_gameNode->getContentSize().width/2,860);
    _gameNode->addChild(questionBox);

    if (!_problem.questionOption5.empty()) createSmallSpeakerView(_problem.questionOption5);
    if (_problem.questionOption3.empty()) _problem.questionOption3 = "?";
    vector<string> sequenceV;
    sequenceV.push_back(_problem.questionOption2);
    sequenceV.push_back(_problem.questionOption3);
    sequenceV.push_back(_problem.questionOption4);
    
    createSequenceTemplate(sequenceV, 1000+336/2);
    createPushButtonC3STemplate(68.f);

}

void EggQuizScene::createImageseqImageView() {

    auto questionBox = Scale9Sprite::create(partsPath+"pretest-questionbox.png");
    questionBox->setPreferredSize(Size(2360,800));
    questionBox->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
    questionBox->setPosition(_gameNode->getContentSize().width/2,680);
    _gameNode->addChild(questionBox);

    if (!_problem.questionOption5.empty()) createSmallSpeakerView(_problem.questionOption5);

    if (_problem.questionOption3.empty()) _problem.questionOption3 = "?";
    vector<string> sequenceV;
    sequenceV.push_back(_problem.questionOption2);
    sequenceV.push_back(_problem.questionOption3);
    sequenceV.push_back(_problem.questionOption4);

    createSequenceTemplate(sequenceV, 891+377/2);
    createPushButtonC3WTemplate();
    
}

void EggQuizScene::createOrderingSentenceView() {
    
    auto questionBox = Scale9Sprite::create(partsPath+"pretest-questionbox.png");
    questionBox->setPreferredSize(Size(2360,810));
    questionBox->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
    questionBox->setPosition(_gameNode->getContentSize().width/2,670);
    _gameNode->addChild(questionBox);
    
    auto title = TodoUtil::createLabel(_problem.questionOption1, 100, Size::ZERO, FONT_BOLD, FONT_COLOR);
    title->setPosition(_gameSize.width/2, 1354+40);
    _gameNode->addChild(title);
    
    auto speaker = Speaker::create(SpeakerSize::Small);
    speaker->setFileName(_problem.questionOption2);
    speaker->setAnchorPoint(Vec2::ANCHOR_BOTTOM_RIGHT);
    speaker->setPosition(_gameSize.width/2-1032+20, 1311);
    _gameNode->addChild(speaker);
    speaker->setPressed(autoStartDelay);
    
    float slotStartY = 1148+152/2;
    auto answerVec = TodoUtil::split(_problem.answer, '^');
    for (auto answer : answerVec) {
        answer = TodoUtil::trim(answer);
        auto slot = Slot::create(SlotSize::Bar);
        slot->setAnswer(answer);
        slot->setPosition(_gameSize.width/2, slotStartY);
        _gameNode->addChild(slot);
        _slots.push_back(slot);
        slotStartY -= SlotSizeBar.height + 60;
        
    }
    
    float labelStartY = 1188 + 49/2;
    auto numberVec = {1,2,3};
    for (auto number : numberVec) {
        auto numberLabel = TodoUtil::createLabel(TodoUtil::itos(number), 68, Size::ZERO, FONT_BOLD, FONT_COLOR);
        numberLabel->setPosition(_gameSize.width/2 - 1071 - 31/2, labelStartY);
        _gameNode->addChild(numberLabel);
        labelStartY -= 155 + 49;
    }
    
    float arrowStartY = 1070 + 81/2;
    for (int i=0; i<2; i++) {
        auto arrow = Sprite::create(partsPath+"pretest_arrow.png");
        arrow->setRotation(90);
        arrow->setPosition(_gameSize.width/2 - 1048 - 72/2, arrowStartY);
        _gameNode->addChild(arrow);
        arrowStartY -= 124 + 81;
    }
    
    function<void()> onTouchMoved = [this](){
        checkDragButtonOver();
    };
    
    function<void()> onTouchEnded = [this](){
        checkDragButton();
        checkSlotFull(SlotType::Sentence);
    };
    
    Size buttonSize = Size(2001, 160);
    float buttonStartY = 456 + buttonSize.height/2;
    auto choiceVec = TodoUtil::split(_problem.answerOption1, '^');
    vector<string> labels;
    for (auto choice : choiceVec) {
        labels.push_back(choice);
    }
    float fontSize = getMinFontSize(labels, 68, buttonSize);
    
    random_shuffle(choiceVec.begin(), choiceVec.end(), [](int n) { return rand() % n; });
    
    for (auto choice : choiceVec) {
        choice = TodoUtil::trim(choice);
        auto button = DragButton::create(choice, buttonSize, fontSize, _slots, onTouchMoved,  onTouchEnded);
        button->setOriginalPos(Vec2(_gameSize.width/2, buttonStartY));
        button->disableDuplicate();
        _gameNode->addChild(button);
        buttonStartY -= 28 + buttonSize.height;
    }
}

void EggQuizScene::createListeningcompSentenceView() {

    Size buttonSize = Size(1436, 328);
    float posX = _gameSize.width/2-256+buttonSize.width/2;

    auto questionBox = Scale9Sprite::create(partsPath+"pretest-questionbox.png");
    questionBox->setPreferredSize(Size(860,1380));
    questionBox->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
    questionBox->setPosition(_gameSize.width/2-320-860/2,100);
    _gameNode->addChild(questionBox);
    
    auto speaker = Speaker::create(SpeakerSize::Big);
    speaker->setFileName( _problem.questionOption1);
    speaker->setPosition(questionBox->getContentSize()/2);
    questionBox->addChild(speaker);
    speaker->setPressed(autoStartDelay);

    Label* questionLabel = TodoUtil::createLabelMultilineToFitWidth(_problem.questionOption2, 100, Size(1060, 600), FONT_NORMAL, FONT_COLOR, TextHAlignment::CENTER);
    questionLabel->setPosition(posX, 1245+215/2);
    _gameNode->addChild(questionLabel);
    
    auto choices = TodoUtil::split(_problem.answerOption1, '^');
    float posY = 812 + buttonSize.height/2;
    
    for (auto it : choices) {
        it = TodoUtil::trim(it);
        auto button = PushButton::create(buttonSize, 100, it, [this](string answer){ checkAnswer(answer); });
        button->setPosition(posX, posY);
        _gameNode->addChild(button);
        if (it.find(".png")!=string::npos) button->setImage();
        posY -= 40 + buttonSize.height;
    }

    
}

void EggQuizScene::showProblem() {
    LOGFN();
    
    if (_currentProblemIndex == _problems.size()) {
        onComplete();
        return;
    }
    
    _problem = _problems.at(_currentProblemIndex);
    _answer = _problem.answer;
    _answerInt = TodoUtil::stoi(_answer);
    _slots.clear();

    updateProgressBar(true);
    
    CCLOG("show problem:%s", _problem.templateName.c_str());
    
    if (_problem.templateName == "single_digit_numbers") { createSingleDigitNumbersView(); }
    else if (_problem.templateName == "recognize_number") { createRecognizeNumberView(); }
    else if (_problem.templateName == "2digit_numbers") { create2DigitNumbersView(); }
    else if (_problem.templateName == "bigger_and_smaller") { createBiggerAndSmallerView(); }
    else if (_problem.templateName == "missing_number") { createMissingNumberView(); }
    else if (_problem.templateName == "operations_with_objects") { createOperationsWithObjectsView(); }
    else if (_problem.templateName == "operations_without_objects") { createOperationsWithoutObjectsView(); }
    else if (_problem.templateName == "biggest_and_smallest") { createBiggestAndSmallestView(); }
    else if (_problem.templateName == "compare_number_magnitudes") { createCompareNumberMagnitudesView(); }
    else if (_problem.templateName == "use_magnitude_symbols") { createUseMagnitudeSymbolsView(); }
    else if (_problem.templateName == "shapes") { createShapesView(); }
    else if (_problem.templateName == "3digit_numbers") { create3DigitNumbersView(); }
    else if (_problem.templateName == "number_identification") { createNumberIdentificationDragView(); }
    else if (_problem.templateName == "missing_number_drag") { createMissingNumberDragView(); }
    else if (_problem.templateName == "operations_drag") { createOperationsDragView(); }
    else if (_problem.templateName == "word_problem") { createWordProblemView(); }

    else if (
             _problem.templateName == "soundonly_word"
             || _problem.templateName == "soundonly_image"
             ) { createSoundonlyWordView(); }
    else if (_problem.templateName == "word_word") { createWordWordView(); }
    else if (_problem.templateName == "image_word") { createImageWordView(); }
    else if (_problem.templateName == "sentence_word") { createSentenceWordView(); }
    else if (_problem.templateName == "sentence_sentence") { createSentenceSentenceView(); }
    else if (_problem.templateName == "image_sentence") { createImageSentenceView(); }
    else if (_problem.templateName == "soundonly_sentence") { createSoundonlySentenceView(); }
    else if (_problem.templateName == "paragraph_sentence") { createParagraphSentenceView(); }
    else if (_problem.templateName == "imageseq_sentence") { createImageseqSentenceView(); }
    else if (_problem.templateName == "imageseq_image") { createImageseqImageView(); }
    else if (_problem.templateName == "ordering_sentence") { createOrderingSentenceView(); }
    else if (_problem.templateName == "listeningcomp_sentence") { createListeningcompSentenceView(); }
    
    fadeInAll(_gameNode);
        
}

void EggQuizScene::checkAnswer(string answer) {
    
    CCLOG("choice: %s / answer: %s", answer.c_str(), _answer.c_str());

    if (answer == _answer) {
        _answerResult[_currentProblemIndex] = true;
        StrictLogManager::shared()->eggQuiz_CorrectAnswer(_currentProblemIndex, answer, 0);
    } else {
        _answerResult[_currentProblemIndex] = false;
        StrictLogManager::shared()->eggQuiz_WrongAnswer(_currentProblemIndex, _answer, answer, 0);
    }
    
    _currentProblemIndex++;
    updateProgressBar();
    
    confirmAnswer([this](){
        fadeOutAll(_gameNode, [this](){
            showProblem();
        });
    });
    
}

void EggQuizScene::confirmAnswer(function<void()> onCompleteCallback) {
    setPressDisabledAll(_gameNode);
    GameSoundManager::getInstance()->stopBGM();

    for (auto it : _gameNode->getChildren()) {
        if (it->getName() == "Slot") {
            auto slot = dynamic_cast<Slot*>(it);
            if (slot->_targetButton) slot->_targetButton->setActive();
        }
    }
    
    runAction(Sequence::create(
        DelayTime::create(1),
        CallFunc::create([this, onCompleteCallback](){
            onCompleteCallback();
        }),
        nullptr
    ));
}

void EggQuizScene::onComplete() {
    
    int numCorrect = 0;
    for (auto s : _answerResult) {
        if (s) numCorrect++;
    }
    bool cleared = (numCorrect>=_problems.size()*0.8);

    if (TodoUtil::startsWith(_level, "fishtest")) {
        completedFishTest((int)((float)numCorrect / _problems.size() * 100));
        
    } else {
        auto popup = EggQuizPopup::create(this, _winSize);
        auto levelVec = TodoUtil::split(_level, '_');
        auto course = levelVec[0];
        std::transform(course.begin(), course.end(), course.begin(), ::tolower);

        if (course == "specialtest") popup->_isSpecialTest = true;
        
        if (cleared) {
            GameSoundManager::getInstance()->playEffectSoundForAutoStart("Common/Sounds/Effect/pretest_success.m4a");
            
            if (levelVec.size()>1) popup->setPostCompleted(_course=="Math"?'M':'L', TodoUtil::stoi(levelVec[1]));
            this->runAction(Sequence::create(DelayTime::create(1.0), CallFunc::create([](){SoundEffect::wowEffect().play();}), nullptr ));
            popup->show(this, true, CallFunc::create([this](){
                CCAppController::sharedAppController()->handleGameComplete(1);
            }));
            
        } else {
            
            GameSoundManager::getInstance()->playEffectSoundForAutoStart("Common/Sounds/Effect/pretest_fail1.m4a");
            if (levelVec.size()>1) popup->setPostFailed(_course=="Math"?'M':'L', TodoUtil::stoi(levelVec[1]));
            popup->show(this, true, CallFunc::create([this](){
                CCAppController::sharedAppController()->handleGameQuit();
            }));
        }
    }
}

void EggQuizScene::completedFishTest(int score) {
    auto popup = FishQuizPopup::create(this, _winSize);
    
    auto lang = LanguageManager::getInstance()->getCurrentLanguageTag();
    auto levelID = CurriculumManager::getInstance()->makeLevelID(lang, _course=="Math"?'M':'L', CoopScene::LEVEL_FISH_PRESENT);
    bool gameTestMode = UserManager::getInstance()->isGameTestingMode();
    
    int progressLevelIndex;
    int progressIndex;
    
    if (gameTestMode) {
        auto levelVec = TodoUtil::split(_level, '_');
        progressLevelIndex = TodoUtil::stoi(levelVec[1]) - 1;
        progressIndex = TodoUtil::stoi(levelVec[2]) - 1;
        
    } else {
        progressLevelIndex = UserManager::getInstance()->getFishPresentCurrentProgressLevel(levelID);
        progressIndex = UserManager::getInstance()->getFishPresentCurrentProgressIndex(levelID, progressLevelIndex);
        
    }

    popup->setFishCompleted(_course=="Math"?'M':'L', progressLevelIndex, progressIndex, score);
    popup->show(this, true, CallFunc::create([this, levelID, score, gameTestMode](){
        if (gameTestMode == false && score <= 60 && EggQuiz::EggQuizScene::getTryCountFishTest() == 1) {
            // Retry
            CCAppController::sharedAppController()->handleGameQuit(true);
            
            string gameName = (this->_course == "Math") ? "EggQuizMath" : "EggQuizLiteracy";
            int progressLevel = UserManager::getInstance()->getFishPresentCurrentProgressLevel(levelID);
            auto levels = EggQuiz::ProblemBank::getInstance()->getLevels(this->_course);
            int setCount = 0;
            auto prefix = "fishtest_" + TodoUtil::itos(progressLevel + 1) + "_";
            for (auto level : levels) {
                if (TodoUtil:: TodoUtil::startsWith(level, prefix)) {
                    ++setCount;
                }
            }

            int progressIndex = UserManager::getInstance()->getFishPresentCurrentProgressIndex(levelID, progressLevel);

            string param = StringUtils::format("fishtest_%d_%d", progressLevel + 1, (progressIndex % setCount) + 1);
            CCAppController::sharedAppController()->startGame(gameName, CoopScene::LEVEL_FISH_PRESENT, param);
            
        } else {
            CCAppController::sharedAppController()->handleGameQuit();
            
        }
    }));
}

void EggQuizScene::fadeInAll(Node* node) {

    node->setCascadeOpacityEnabled(true);
    node->setOpacity(0);
    
    node->runAction(FadeIn::create(0.5));
    for (auto it : node->getChildren()) {
        fadeInAll(it);
    }
}

void EggQuizScene::fadeOutAll(Node* node, function<void()> onCompleteCallback) {
    node->runAction(FadeOut::create(0.5));
    for (auto it : node->getChildren()) {
        fadeOutAll(it, nullptr);
    }
    
    if (onCompleteCallback) {
        runAction(Sequence::create(
            DelayTime::create(0.6),
            CallFunc::create([this, onCompleteCallback](){
                _gameNode->removeAllChildren();
                onCompleteCallback();
            }),
            nullptr
        ));
    }
}

void EggQuizScene::setPressDisabledAll(Node* node) {
    Director::getInstance()->getEventDispatcher()->removeEventListenersForTarget(node);
    if (node->getName()=="speaker") node->stopAllActions();
    for (auto it : node->getChildren()) {
        setPressDisabledAll(it);
    }
}


// sub template

void EggQuizScene::updateProgressBar(bool setCurrent) {
    string progressStatus = "";
    for (int i=0; i<_problems.size(); i++) {
        string c = _answerResult[i] ? "o" : "x";
        if (setCurrent && i==_currentProblemIndex) c = "c";
        else if (i>=_currentProblemIndex) c = "-";
        progressStatus += c;
    }
    CCLOG("%s", progressStatus.c_str());
    _progressBar->setStatus(progressStatus);
}

void EggQuizScene::createPushButton09Template() {
    
    float startX = _gameSize.width/2-988 - BUTTON_SIZE_SMALL.width/2;
    float posY = 78 + BUTTON_SIZE_SMALL.height/2;
    
    for(int i=0; i<10; i++) {
        auto choice = TodoUtil::itos(i);
        auto button = PushButton::create(BUTTON_SIZE_SMALL, 200, choice, [this, choice](string answer){ checkAnswer(answer); });
        button->setPosition(Vec2(startX,posY));
        _gameNode->addChild(button);
        startX += 49 + button->getContentSize().width;
    }
}

void EggQuizScene::createDragButton09Template() {
    
    function<void()> onTouchMoved = [this](){
        checkDragButtonOver();
    };
    
    function<void()> onTouchEnded = [this](){
        checkDragButton();
        checkSlotFull(SlotType::LinkedNumber);
    };
    
    float startX = _gameSize.width/2-988 - BUTTON_SIZE_SMALL.width/2;
    float posY = 78 + BUTTON_SIZE_SMALL.height/2;
    
    for(int i=0; i<10; i++) {
        auto button = DragButton::create(TodoUtil::itos(i), BUTTON_SIZE_SMALL, 200, _slots, onTouchMoved, onTouchEnded);
        button->setOriginalPos(Vec2(startX,posY));
        _gameNode->addChild(button);
        startX += 49 + button->getContentSize().width;
    }
}


void EggQuizScene::createAnswerNumberPadView() {
    
    auto answerPad = Node::create();
    auto answerPadBg = Sprite::create(partsPath+"pretest-math-calculator-bg.png");
    auto answerPadSize = answerPadBg->getContentSize();
    answerPad->setContentSize(answerPadSize);
    answerPadBg->setPosition(answerPadSize/2);
    answerPad->addChild(answerPadBg);
    
    auto panelSize = Size(240, 178);
    
    const int rowCount = 4;
    const int colCount = 3;
    
    auto panelAreaSize = Size(panelSize.width*colCount + 10.f*(colCount-1),
                              panelSize.height*rowCount + 10.f*(rowCount-1));
    float panelAreaMargin = (answerPadSize.width-panelAreaSize.width)/2.f;
    // float answerAreaHeight = answerPadSize.height - 2*panelAreaMargin - panelAreaSize.height;
    auto answerLabelPos = Point(answerPadSize.width/2, 1180);
    
    const auto normalColor = FONT_COLOR;
    const auto highlightColor = FONT_COLOR;
    _padNumber = "";
    
    Label* answerLabel = TodoUtil::createLabel(_padNumber, 220, Size(750, 190), FONT_NORMAL, FONT_COLOR, TextHAlignment::RIGHT);
    answerLabel->setPosition(answerLabelPos);
    answerPad->addChild(answerLabel);
    
    auto panelPos = [panelSize, rowCount, colCount, panelAreaMargin](int row, int col)
    {
        float x = col * (panelSize.width + 10.f) + panelSize.width/2 + panelAreaMargin;
        float y = (10.f + panelSize.height) * (rowCount - row-1) + panelSize.height/2 + panelAreaMargin;
        
        return Point(x, y);
    };
    
    for (int i = -2; i <= 9; i++) {
        Point btnPos;
        int fontSize = 120;
        
        ui::Button *btn;
        Label *btnLabel;
        
        if (i==-1) {
            btn = ui::Button::create(partsPath+"pretest_math_calculator_button_normal.png", partsPath+"pretest_math_calculator_button_active.png");
            btnLabel = Label::create();
            auto backsp = Sprite::create(partsPath+"pretest_math_calculator_back.png");
            backsp->setPosition(panelSize/2 + Size(0, 8));
            btn->addChild(backsp);
            btnPos = panelPos(rowCount - 1, 0);
        } else if (i==-2) {
            btn = ui::Button::create(partsPath+"pretest_math_calculator_button_enter_normal.png", partsPath+"pretest_math_calculator_button_enter_active.png");
            btnLabel = Label::create();
            btnPos = panelPos(rowCount - 1, 2);
        } else {
            btn = ui::Button::create(partsPath+"pretest_math_calculator_button_normal.png", partsPath+"pretest_math_calculator_button_active.png");
            btnLabel = TodoUtil::createLabel(TodoUtil::itos(i), fontSize, Size::ZERO, FONT_NORMAL, FONT_COLOR);
            if (i==0) {
                btnPos = panelPos(rowCount - 1, 1);
            } else {
                btnPos = panelPos((9-i)/colCount, colCount - (9-i)%colCount - 1);
            }
        }
        btnLabel->setPosition(btn->getContentSize()/2 + Size(0, 8));
        btn->addChild(btnLabel);
        
        btn->setZoomScale(0);
        btn->setPosition(btnPos);
        btn->setTag(i);
        btn->addTouchEventListener([btn, btnLabel, normalColor, highlightColor, this](Ref* sender,Widget::TouchEventType event) {
            btnLabel->setTextColor(btn->isHighlighted() ? highlightColor : normalColor);
            
        });
        
        btn->addClickEventListener([btn, i, this, answerLabel](Ref* sender) {
            
            //GameSoundManager::getInstance()->playEffectSound(touchEffect);
            auto ans = btn->getTag();
            if (ans==-2) {
                //enter
                if (_padNumber.length()==0) return;
                checkAnswer(_padNumber);
                
            } else if (ans==-1) {
                //back
                auto l = _padNumber.length();
                _padNumber = _padNumber.substr(0, l-1);
                answerLabel->setString(_padNumber);
                
            } else {
                if (_padNumber.size() <= 4) {
                    _padNumber += TodoUtil::itos(ans);
                    answerLabel->setString(_padNumber);
                }
            }
        });
        
        answerPad->addChild(btn);
    }
    _gameNode->addChild(answerPad);
    answerPad->setPosition(_gameSize.width/2+43, 100);
    
}
void EggQuizScene::createPushButtonC3WTemplate(float fontSize) {
    Size buttonSize = Size(744,516);
    float startX = _gameSize.width/2-436 - buttonSize.width/2;
    float posY = 92 + buttonSize.height/2;
    float space = 64;
    
    auto choices = TodoUtil::split(_problem.answerOption1, '^');
    
    for (auto it : choices) {
        it = TodoUtil::trim(it);
        auto button = PushButton::create(buttonSize, fontSize, it, [this](string answer){ checkAnswer(answer); });
        button->setPosition(startX, posY);
        _gameNode->addChild(button);
        startX += space + buttonSize.width;
        
        if (it.find("+") != string::npos) {
            button->_label->setVisible(false);
            
            auto chars = TodoUtil::split(it, '+');
            Size frameSize = Size(146,222);
            
            float posX = buttonSize.width/2 - 12 - frameSize.width/2;
            float posY = 161 + frameSize.height/2;
            
            for(auto ch : chars) {
                auto frame = Sprite::create(partsPath+"egg_quiz_cards_blank.png");
                auto label = TodoUtil::createLabel(ch, 152, Size::ZERO, FONT_NORMAL, FONT_COLOR);
                label->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
                label->setPosition(frame->getContentSize()/2);
                frame->setPosition(posX, posY);
                button->addChild(frame);
                frame->addChild(label);
                posX += frameSize.width + 24;
            }
            
        } else if (it.find(".png") != string::npos) {
            button->_label->setVisible(false);
            auto image = Sprite::create(imagesPath+it);
            Size imageSize = image->getContentSize();
            Size buttonLimitSize = button->getContentSize()*0.8;
            image->setScale(MIN(buttonLimitSize.width/imageSize.width, buttonLimitSize.height/imageSize.height));
            button->addChild(image);
            image->setPosition(button->getContentSize()/2);
        }
    }
}

void EggQuizScene::createPushButtonC3STemplate(float fontSize) {
    auto choices = TodoUtil::split(_problem.answerOption1, '^');
    Size buttonSize = Size(2360, 208);
    float posX = _gameSize.width/2;
    float posY = 588 + buttonSize.height/2;
    
    vector<string> labels;
    for (auto it : choices) {
        labels.push_back(it);
    }
    fontSize = getMinFontSize(labels, fontSize, buttonSize);
    
    for (auto it : choices) {
        it = TodoUtil::trim(it);
        auto button = PushButton::create(buttonSize, fontSize, it, [this](string answer){ checkAnswer(answer); });
        button->setPosition(posX, posY);
        _gameNode->addChild(button);
        posY -= 40 + buttonSize.height;
    }
}

void EggQuizScene::createSmallSpeakerView(string fileName, Vec2 position) {
    if (!position.x && !position.y) {
        position = Vec2(_gameSize.width/2-932+32,1211-16);
    }
    
    auto speaker = Speaker::create(SpeakerSize::Small);
    speaker->setFileName(fileName);
    speaker->setAnchorPoint(Vec2::ANCHOR_BOTTOM_RIGHT);
    speaker->setPosition(position);
    _gameNode->addChild(speaker);
    speaker->setPressed(autoStartDelay);
    
}

void EggQuizScene::createSequenceTemplate(vector<string> sequenceV, float posY) {
    
    float posX = _gameSize.width/2 - 360 - 518/2;
    
    for (auto sequence : sequenceV) {
        sequence = TodoUtil::trim(sequence);
        auto frame = Sprite::create(partsPath+"pretest_picture_withoutbg.png");
        frame->setPosition(posX, posY);
        _gameNode->addChild(frame);
        posX += frame->getContentSize().width+101;
        
        if (sequence == "?") {
            auto blank = Sprite::create(partsPath+"pretest_filltheblank.png");
            blank->setPosition(frame->getPosition());
            blank->setScale(0.95);
            _gameNode->addChild(blank);
            frame->setVisible(false);
        } else {
            
            if (sequence.find(".png")!=string::npos) {
                auto image = Sprite::create(imagesPath+sequence);
                image->setPosition(frame->getContentSize()/2);
                image->setScale(MIN(frame->getContentSize().width/image->getContentSize().width, frame->getContentSize().height/image->getContentSize().height)*0.88);
                frame->addChild(image);
                
            } else {
                auto text = TodoUtil::createLabelMultilineToFit(sequence, 64*0.7, Size(438,288), FONT_NORMAL, FONT_COLOR);
                text->setPosition(frame->getPosition());
                _gameNode->addChild(text);
                
            }
        }
    }
    
    posX = _gameSize.width/2 - 269 - 81/2 + 10;
    for (int i=0; i<2; i++) {
        auto arrow = Sprite::create(partsPath+"pretest_arrow.png");
        arrow->setPosition(posX, posY);
        _gameNode->addChild(arrow);
        posX += 538 + arrow->getContentSize().width;
    }
    
}

void EggQuizScene::checkDragButton() {
    map<string, bool> buttonMap;
    for (auto it : _gameNode->getChildren()) {
        if (it->getName() == "dragButton") {
            auto button = dynamic_cast<DragButton*>(it);
            auto label = button->_label->getString();
            if (button->_targetSlot) continue;
            if (button->_holding) continue;
            if (button->_willBeDisappear) continue;
            if (buttonMap.count(label) > 0) {
                button->_willBeDisappear = true;
                button->fadeOut();
                continue;
            }
            buttonMap[button->_label->getString()] = true;
        }
    }
}
void EggQuizScene::checkDragButtonOver() {
    for (int i=0; i<_slots.size(); i++) {
        auto it = _slots.at(i);
        bool disableFlag = true;
        
        for (auto child : _gameNode->getChildren()) {
            if (child->getName() != "dragButton") continue; 
            
            if (it->getBoundingBox().containsPoint(child->getPosition())) {
                if (it->_targetButton) continue;
                it->enableOver();
                disableFlag = false;
            }
        }
        if (disableFlag) it->disableOver();
    }
}

void EggQuizScene::checkSlotFull(SlotType type) {
    vector<string> answers;
    for (auto it : _slots) {
        if (!it->_targetButton) return;
        answers.push_back(it->_targetButton->_label->getString());
    }    
    auto answer = ProblemBank::getInstance()->getJoinString(answers, "^");
    switch(type) {
        case SlotType::LinkedNumber:
            TodoUtil::replaceAll(answer, "^", "");
            break;
        case SlotType::Sentence:
            break;
        case SlotType::Normal:
        default: 
            TodoUtil::replaceAll(answer, "^", ",");
            break;
    }
    checkAnswer(answer);
    
}

float EggQuizScene::getMinFontSize(vector<string> labels, float fontSize, Size boxSize) {
    float minSize = fontSize;
    for (auto it : labels) {
        TodoUtil::replaceAll(it, "_", "\n");
        auto label = Label::createWithTTF(it, FONT_NORMAL, fontSize, Size(boxSize.width*0.95,0), TextHAlignment::CENTER, cocos2d::TextVAlignment::CENTER);
        
        if (label->getContentSize().width > boxSize.width) minSize = MIN(minSize, fontSize * boxSize.width / label->getContentSize().width * 0.8);
        if (label->getContentSize().height > boxSize.height) minSize = MIN(minSize, fontSize * boxSize.height / label->getContentSize().height * 0.8);
        //CCLOG("minSize: %f",minSize);
    }
    
    return minSize;
}

void EggQuizScene::onExit() {
    LOGFN();
    Layer::onExit();
    VoiceMoldManager::shared()->speak(" "); //To stop TTS
}

void EggQuizScene::resetTryCountFishTest() {
    _tryCountFishTest = 0;
}

void EggQuizScene::increaseTryCountFishTest() {
    ++_tryCountFishTest;
}

int EggQuizScene::getTryCountFishTest() {
    return _tryCountFishTest;
}

END_NS_EGGQUIZ;
