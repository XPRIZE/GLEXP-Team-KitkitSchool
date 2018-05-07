//
//  EggQuizMathScene.cpp
//  KitkitSchool-mobile
//
//  Created by JungJaehun on 26/03/2018.
//

#include "EggQuizMathScene.hpp"

#include "CCAppController.hpp"
#include "Managers/GameSoundManager.h"
#include "Managers/UserManager.hpp"
#include "Managers/LanguageManager.hpp"
#include "Managers/StrictLogManager.h"
#include "Common/Controls/TodoSchoolBackButton.hpp"
#include "Common/Controls/CompletePopup.hpp"

BEGIN_NS_EGGQUIZMATH;

const char* solveEffect = "Common/Sounds/Effect/UI_Star_Collected.m4a";
const char* missEffect = "Common/Sounds/Effect/Help.m4a";
const char* pageTurnEffect = "Common/Sounds/Effect/Card_Move_Right.m4a";
const char* touchEffect = "Common/Sounds/Effect/paneltouch.m4a";
const char* defaultFont = "fonts/Andika-R.ttf";
const char* boldFont = "fonts/AndikaNewBasic-B.ttf";
const Color4B defaultColor = Color4B(74, 77, 82,255);
const Color4B fontColor1 = Color4B(24, 115, 160,255);

const int choiceTypeImage = 1;

const int tagSoundButtonNormal = 1001;
const int tagSoundButtonActive = 1002;
const int tagSoundButtonNode = 1003;

const int tagCardBody = 2001;
const int tagCardShadow = 2002;
const int tagCardOriginalPos = 2004;
const int tagCardSnapped = 2006;
const int tagSlotActive = 2005;

const int tagAutoSequence = 3001;
const int tagStopSoundButtonOff = 3002;

map<string, string> _textEn;
map<string, string> _textSw;

auto getFilenameFromText = [](string input){
    TodoUtil::replaceAll(input, "?", "");
    TodoUtil::replaceAll(input, ".", "");
    TodoUtil::replaceAll(input, " ", "_");
    return "EggQuizMath/Sounds/"+input+".m4a";
};

Scene* EggQuizMathScene::createScene(string level)
{
    auto scene = Scene::create();
    auto layer = EggQuizMathScene::create();
    layer->setLevel(level);
    layer->setCascadeOpacityEnabled(true);
    scene->addChild(layer);
    return scene;
}

void EggQuizMathScene::setLevel(string level) {
    _currentLevel = level;
}

bool EggQuizMathScene::init() {
    if (!Layer::init()) return false;
    
    onCompleteCallback = nullptr;
    
    initUI();
    initTextMap();
    return true;
}

void EggQuizMathScene::initUI() {
    
    _winSize = getContentSize();
    _currentPageGridZ = 0;
        
    auto bg = Sprite::create("EggQuizLiteracy/Common/pretest_layout_bg.jpg");
    auto bgSize = bg->getContentSize();
    auto bgScale = MAX(_winSize.width / bgSize.width, _winSize.height / bgSize.height);
    bg->setScale(bgScale);
    bg->setPosition(_winSize/2);
    bg->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    addChild(bg);
    
    _pageNode = Node::create();
    _pageNode->setContentSize(_winSize);
    addChild(_pageNode);
    _currentPageGridZ = 0;
    
    _gameSize = Size(2560, 1800);
    //_gameSize = Size(3200, 1800);
    _gameScale = _winSize.height / 1800;
    
    auto backButton = TodoSchoolBackButton::create();
    backButton->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
    backButton->setPosition(Vec2(25, _winSize.height-25));
    addChild(backButton);
    
    _progressBar = ProgressIndicator::create();
    _progressBar->setPosition(Vec2(_winSize.width/2, _winSize.height - _progressBar->getContentSize().height));
    addChild(_progressBar);
    
    auto paperTop = Sprite::create("EggQuizLiteracy/Common/pretest_layout_page.png");
    paperTop->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
    paperTop->setPosition(_winSize.width / 2, 0.f);
    addChild(paperTop);
    _paperTop = paperTop;
    
    auto problemNode = Node::create();
    problemNode->setContentSize(_gameSize);
    problemNode->setScale(_gameScale);
    problemNode->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    problemNode->setPosition(Vec2(_winSize.width/2, _winSize.height/2));
    
    addChild(problemNode);
    _problemNode = problemNode;

    if (UserManager::getInstance()->isDebugMode()) {
        auto pass = Button::create();
        pass->setTitleText("Pass");
        pass->setTitleFontName(defaultFont);
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
        fail->setTitleFontName(defaultFont);
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
        next->setTitleFontName(defaultFont);
        next->setTitleFontSize(50);
        next->setTitleColor(Color3B::BLACK);
        next->setPosition(Vec2(_winSize.width-800, _winSize.height-50));
        next->addClickEventListener([this](Ref*) {
            this->onSolve();
        });
        addChild(next);
    }

}

void EggQuizMathScene::onEnter(){
    LOGFN();
    Layer::onEnter();
    
    setTouchEnabled(true);
    _problems = ProblemBank::getInstance()->loadData(_currentLevel);
    _duration = ProblemBank::getInstance()->loadDurationsheet();
    
    _currentProblemIndex = 0;
    _totalProblemCount = _problems.size();

    _answerResult.clear();
    for (int i=0; i<_totalProblemCount; i++) {
        _answerResult.push_back(false);
    }
    
    _progressBar->setMax(_problems.size(), true);
    _progressBar->setCurrent(_currentProblemIndex + 1);

    showProblem();
}

void EggQuizMathScene::showProblem() {
    LOGFN();

    _problem = _problems.at(_currentProblemIndex);
    _answer = _problem.answerRange;
    _answerInt = TodoUtil::stoi(_answer);
    
    auto problemNode = _problemNode;
    auto paperTop = _paperTop;
    
    if (_problem.templateName == "singledigitnumbers") {
        auto questionView = createSingleDigitNumbersView();
        questionView->setPosition(_gameSize.width/2, 1060);
        problemNode->addChild(questionView);
        vector<string> choices;
        for (int i=1;i<=9;i++) choices.push_back(TodoUtil::itos(i));
        auto choiceView = createAnswerMultipleChoiceView(choices);
        choiceView->setPosition(_gameSize.width/2, 380);
        problemNode->addChild(choiceView);
        
    } else if (_problem.templateName == "2digitnumbers") {
        auto questionView = create2DigitNumbersView();
        questionView->setPosition(_gameSize.width/2, 1060);
        problemNode->addChild(questionView);
        
        auto choiceNumbers = TodoUtil::split(_problem.answerOption1, ',');
        auto choiceView = createAnswerMultipleChoiceView(choiceNumbers);
        choiceView->setPosition(_gameSize.width/2, 380);
        problemNode->addChild(choiceView);

    } else if (_problem.templateName == "recognizenumber") {
        auto questionView = createRecognizeNumberView();
        questionView->setPosition(_gameSize.width/2, 1060);
        problemNode->addChild(questionView);
        
        auto choiceNumbers = TodoUtil::split(_problem.answerOption1, ',');
        auto choiceView = createAnswerMultipleChoiceView(choiceNumbers);
        choiceView->setPosition(_gameSize.width/2, 380);
        problemNode->addChild(choiceView);
        
    } else if (_problem.templateName == "missingnumber") {
        auto questionView = createMissingNumberView();
        questionView->setPosition(_gameSize.width/2-500, 800);
        problemNode->addChild(questionView);
        
        auto numberPadView = createAnswerNumberPadView();
        numberPadView->setPosition(_gameSize.width/2+100, 180);
        problemNode->addChild(numberPadView);
        
    } else if (_problem.templateName == "operationswithobjects") {
        auto questionView = createOperationsWithObjects();
        questionView->setPosition(_gameSize.width/2-1030, _gameSize.height/2-728);
        problemNode->addChild(questionView);

        auto numberPadView = createAnswerNumberPadView();
        numberPadView->setPosition(_gameSize.width/2+100, 180);
        problemNode->addChild(numberPadView);
        
    } else if (_problem.templateName == "operationswithoutobjects") {
        auto questionView = createOperationsWithoutObjects();
        questionView->setPosition(_gameSize.width/2-1030, paperTop->getContentSize().height/2-16);
        problemNode->addChild(questionView);
        
        auto numberPadView = createAnswerNumberPadView();
        numberPadView->setPosition(_gameSize.width/2+100, 180);
        problemNode->addChild(numberPadView);
        
    } else if (_problem.templateName == "biggestandsmallest") {
        string labelStr = _problem.questionOption1 == "biggest" ? "Which is the biggest?" : "Which is the smallest?";
        
        auto soundButton = createSoundButton(getFilenameFromText(labelStr), 2.f, 200.f, true);
        soundButton->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        soundButton->setPosition(_gameSize.width/2-paperTop->getContentSize().width/2+218, paperTop->getContentSize().height-200);
        problemNode->addChild(soundButton);

        labelStr = getText(labelStr);
        auto label = TodoUtil::createLabel(labelStr, 130, Size::ZERO, defaultFont, Color4B(70, 70, 70, 255));
        label->setPosition(_gameSize.width/2, _gameSize.height/2 + 200);
        problemNode->addChild(label);
        
        auto choices = TodoUtil::split(_problem.answerOption1, ',');
        auto choiceView = createAnswerMultipleChoiceView(choices);
        choiceView->setPosition(_gameSize.width/2, 380);
        problemNode->addChild(choiceView);
        
    } else if (_problem.templateName == "biggerandsmaller") {
        auto gameView = createBiggerAndSmaller();
        problemNode->addChild(gameView);
        
    } else if (_problem.templateName == "comparenumbermagnitudes") {
        auto gameView = createCompareNumberMagnitudes();
        gameView->setContentSize(_gameSize);
        gameView->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
        gameView->setPosition(_gameSize.width/2, 0);
        problemNode->addChild(gameView);
        
    } else if (_problem.templateName == "usemagnitudesymbols") {
        auto gameView = createUseMagnitudeSymbols();
        gameView->setContentSize(_gameSize);
        gameView->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
        gameView->setPosition(_gameSize.width/2, 0);
        problemNode->addChild(gameView);
        
    } else if (_problem.templateName == "shapes") {
        auto gameView = createShapes();
        gameView->setContentSize(_gameSize);
        gameView->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
        gameView->setPosition(_gameSize.width/2, 0);
        problemNode->addChild(gameView);
        
    } else if (_problem.templateName == "3digitnumbers") {
        auto gameView = create3DigitNumbersView();
        gameView->setContentSize(_gameSize);
        gameView->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
        gameView->setPosition(_gameSize.width/2, 0);
        problemNode->addChild(gameView);
    }
}

Node* EggQuizMathScene::createSoundButton(string fileName, float duration, float size, bool autoStart) {
    Node *node = Node::create();
    auto fileNameVec = TodoUtil::split(fileName, '/');
    auto playFileName = fileNameVec.back();
    
    if (_duration.count(playFileName)) duration = _duration[playFileName];
    
    auto soundNormal = Sprite::create("EggQuizLiteracy/Common/pretest_speaker_normal.png");
    soundNormal->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    node->addChild(soundNormal);
    soundNormal->setTag(tagSoundButtonNormal);
    
    auto soundActive = Sprite::create("EggQuizLiteracy/Common/pretest_speaker_active.png");
    soundActive->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    soundActive->setVisible(false);
    node->addChild(soundActive);
    soundActive->setTag(tagSoundButtonActive);
    
    node->setContentSize(soundNormal->getContentSize());
    soundNormal->setPosition(soundNormal->getContentSize()/2);
    soundActive->setPosition(soundActive->getContentSize()/2);
    
    node->setScale((float)size/node->getContentSize().width);
    
    auto onTouch = [this, node, duration, fileName](){
        GameSoundManager::getInstance()->playEffectSoundVoiceOnly(fileName);
        node->setTag(tagSoundButtonNode);
        node->getChildByTag(tagSoundButtonActive)->setVisible(true);
        node->getChildByTag(tagSoundButtonNormal)->setVisible(false);
        
        auto sequence = Sequence::create(
                                         DelayTime::create(duration),
                                         CallFunc::create([node](){
            node->setTag(0);
            node->getChildByTag(tagSoundButtonActive)->setVisible(false);
            node->getChildByTag(tagSoundButtonNormal)->setVisible(true);
        }),
                                         nullptr
                                         );
        sequence->setTag(tagStopSoundButtonOff);
        this->runAction(sequence);
    };
    
    auto *listener = EventListenerTouchOneByOne::create();
    listener->setSwallowTouches(true);
    listener->onTouchBegan = [this, node, duration, fileName, onTouch](Touch* T, Event* E) {
        auto P = node->getParent();
        auto pos = P->convertToNodeSpace(T->getLocation());
        
        if (node->getBoundingBox().containsPoint(pos)) {
            // if (node->getTag() == tagSoundButtonNode) return false;
            GameSoundManager::getInstance()->stopAllEffects();
            this->stopActionByTag(tagAutoSequence);
            this->stopActionByTag(tagStopSoundButtonOff);
            onTouch();
        }
        return false;
    };
    
    this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);
    
    if (autoStart) {
        auto autoSequence = Sequence::create(
                                             DelayTime::create(0.5f),
                                             CallFunc::create([onTouch](){
            onTouch();
        }),
                                             //DelayTime::create(duration+1.f),
                                             //CallFunc::create([onTouch](){ onTouch(); }),
                                             nullptr
                                             );
        autoSequence->setTag(tagAutoSequence);
        this->runAction(autoSequence);
    }
    
    return node;
};

Node* EggQuizMathScene::createShapes() {
    Node *node = Node::create();

    auto choices = TodoUtil::split(_problem.answerOption1, ',');
    TodoUtil::replaceAll(_problem.questionOption1, "_", " ");
    _problem.questionOption1[0] = toupper(_problem.questionOption1[0]);
    _problem.questionOption1 += "?";
    
    if (
        _problem.questionOption1 == "Hili ni umbo gani?"
        || _problem.questionOption1 == "What is this shape?"
        ) {
        
        for (int i=0; i<choices.size(); i++) {
            TodoUtil::replaceAll(choices[i], "_", "\n");
        }

        TodoUtil::replaceAll(_answer, "_", "\n");
        
        auto choiceView = createAnswerMultipleChoiceView(choices);
        choiceView->setPosition(_gameSize.width/2, 380);
        node->addChild(choiceView);
        
        auto background = Sprite::create();
        background->setContentSize(Size(1890,300));
        background->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
        background->setPosition(_gameSize.width/2, 1282);
        node->addChild(background);
        
        auto questionLabel = TodoUtil::createLabel(_problem.questionOption1, 80, Size::ZERO, defaultFont, defaultColor, TextHAlignment::LEFT);
        questionLabel->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
        questionLabel->setPosition(background->getContentSize().width/2-730, background->getContentSize().height/2);
        background->addChild(questionLabel);
        
        auto soundButton = createSoundButton(getFilenameFromText(_problem.questionOption1), 2.5f, 200, true);
        soundButton->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        soundButton->setPosition(100, background->getContentSize().height/2);
        background->addChild(soundButton);
        
        auto sample = Sprite::create("EggQuizMath/Images/" + _problem.answerRange + ".png");
        sample->setPosition(_gameSize.width/2, _gameSize.height/2+150);
        node->addChild(sample);

        
    } else {
        
        vector<string> paths = {};
        
        for (auto it : choices) {
            paths.push_back("EggQuizMath/Images/" + it + ".png");
        }
        
        auto choiceView = createAnswerMultipleChoiceView(choices, paths, choiceTypeImage);
        choiceView->setPosition(_gameSize.width/2, 380);
        node->addChild(choiceView);
        
        // auto background = ui::Scale9Sprite::create("EggQuizLiteracy/Common/pretest_questionbox.png");
        // background->setPreferredSize(Size(1890,300));
        auto background = Sprite::create();
        background->setContentSize(Size(1890,300));
        background->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
        background->setPosition(_gameSize.width/2, 982);
        node->addChild(background);
        
        auto questionLabel = TodoUtil::createLabel(_problem.questionOption1, 80, Size::ZERO, defaultFont, defaultColor, TextHAlignment::LEFT);
        questionLabel->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
        questionLabel->setPosition(background->getContentSize().width/2-730, background->getContentSize().height/2);
        background->addChild(questionLabel);
        
        auto soundButton = createSoundButton(getFilenameFromText(_problem.questionOption1), 2.5f, 200, true);
        soundButton->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        soundButton->setPosition(100, background->getContentSize().height/2);
        background->addChild(soundButton);

    }

    return node;
}

Node* EggQuizMathScene::create3DigitNumbersView() {
    Node *node = Node::create();
    
    auto background = ui::Scale9Sprite::create("EggQuizLiteracy/Common/pretest_questionbox.png");
    background->setPreferredSize(Size(940,1226));
    background->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
    background->setPosition(_gameSize.width/2-920, 182);
    node->addChild(background);
    
    auto soundButton = createSoundButton("EggQuizMath/Sounds/howmany.m4a", 1.f, 200.f, true);
    soundButton->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    soundButton->setPosition(0, background->getContentSize().height);
    background->addChild(soundButton);
    
    auto numberPadView = createAnswerNumberPadView();
    numberPadView->setPosition(_gameSize.width/2+1020, 182);
    numberPadView->setAnchorPoint(Vec2::ANCHOR_BOTTOM_RIGHT);
    node->addChild(numberPadView);
    
    float posX = 0;
    float posY = 0;
    float space = -50;
    
    auto ballNode = Node::create();
    ballNode->setPosition(_gameSize.width/2-770,1100);
    ballNode->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
    for (int i=0; i<100; i++) {
        auto blueBall = Sprite::create("EggQuizLiteracy/Common/pretest_math_dot_blue.png");

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
        auto blueBall = Sprite::create("EggQuizLiteracy/Common/pretest_math_dot_blue.png");
        blueBall->setPosition(posX, posY);
        blueBall->setScale(0.5);
        posY -= blueBall->getContentSize().height + space;
        ballNode->addChild(blueBall);
    }
    
    node->addChild(ballNode);

    return node;
}

Node* EggQuizMathScene::createUseMagnitudeSymbols() {
    Node *node = Node::create();
    
    int number1 = TodoUtil::stoi(_problem.questionOption1);
    int number2 = TodoUtil::stoi(_problem.questionOption2);
    
    if (number1 > number2) _answer = ">";
    else if (number1 < number2) _answer = "<";
    else _answer = "=";

    
    string label = "Fill in the blank.";
    
    auto background = ui::Scale9Sprite::create("EggQuizLiteracy/Common/pretest_questionbox.png");
    background->setPreferredSize(Size(1890,760));
    background->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
    background->setPosition(_gameSize.width/2, 682);
    node->addChild(background);
    
    auto soundButton = createSoundButton(getFilenameFromText(label), 1.5f, 200.f, true);
    soundButton->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    soundButton->setPosition(0, background->getContentSize().height);
    background->addChild(soundButton);
    
    auto questionLabel = TodoUtil::createLabel(getText(label), 69, Size::ZERO, defaultFont, defaultColor, TextHAlignment::LEFT);
    questionLabel->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
    questionLabel->setPosition(100, 568);
    background->addChild(questionLabel);
    
    auto numberLabel1 = TodoUtil::createLabel(_problem.questionOption1, 180, Size::ZERO, boldFont, defaultColor, TextHAlignment::CENTER);
    numberLabel1->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    numberLabel1->setPosition(background->getContentSize().width/2-400, 370);
    background->addChild(numberLabel1);
    
    auto numberLabel2 = TodoUtil::createLabel(_problem.questionOption2, 180, Size::ZERO, boldFont, defaultColor, TextHAlignment::CENTER);
    numberLabel2->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    numberLabel2->setPosition(background->getContentSize().width/2+400, 370);
    background->addChild(numberLabel2);
    
    auto slot = Sprite::create("EggQuizLiteracy/Common/pretest_math_emptyslot.png");
    slot->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    slot->setPosition(background->getContentSize().width/2, background->getContentSize().height/2-30);
    slot->setScale(0.8);
    background->addChild(slot);

    vector<string> choices = {">", "<", "="};
    auto choiceView = createAnswerMultipleChoiceView(choices);
    choiceView->setPosition(_gameSize.width/2, 380);
    node->addChild(choiceView);

    return node;
}

bool sortSlotNumbersAscending(const int i, const int j) { return (i < j); };
bool sortSlotNumbersDescending(const int i, const int j) { return (i > j); };

Node* EggQuizMathScene::createCompareNumberMagnitudes() {
    Node *node = Node::create();
    
    auto background = ui::Scale9Sprite::create("EggQuizLiteracy/Common/pretest_questionbox.png");
    background->setPreferredSize(Size(1890,760));
    background->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
    background->setPosition(_gameSize.width/2, 682);
    node->addChild(background);
    
    vector<int> slotNumbers;
    vector<int> cardNumbers;
    _slots.clear();
    _cards.clear();
    auto answerRangeVec = TodoUtil::split(_problem.answerRange, ',');
    bool ascending = _problem.questionOption1 == "ascending" ? true : false;
    
    string question = ascending ? "Order the numbers from the smallest to the largest." : "Order the numbers from the largest to the smallest.";
    auto questionLabel = TodoUtil::createLabel(getText(question), 69, Size::ZERO, defaultFont, defaultColor, TextHAlignment::CENTER);
    questionLabel->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
    background->addChild(questionLabel);
    questionLabel->setPosition(background->getContentSize().width/2, 618);
    
    auto soundButton = createSoundButton(getFilenameFromText(question), 5.f, 200.f, true);
    soundButton->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    soundButton->setPosition(0, background->getContentSize().height);
    background->addChild(soundButton);

    for (auto it : answerRangeVec) {
        slotNumbers.push_back(TodoUtil::stoi(it));
        cardNumbers.push_back(TodoUtil::stoi(it));
    }
    
    sort(slotNumbers.begin(), slotNumbers.end(), ascending ? sortSlotNumbersAscending : sortSlotNumbersDescending);
    
    auto createSlot = [](int number) {
        auto slot = Sprite::create("EggQuizLiteracy/Common/pretest_math_emptyslot.png");
        slot->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
        slot->setTag(number);
        
        auto slotActive = Sprite::create("EggQuizLiteracy/Common/pretest_math_emptyslot_active.png");
        slotActive->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
        slotActive->setVisible(false);
        slotActive->setTag(tagSlotActive);
        
        slot->addChild(slotActive);

        return slot;
    };
    
    auto setPickedToggle = [](Node* card, bool picked) {
        auto cardBody = card->getChildByTag(tagCardBody);
        auto cardShadow = card->getChildByTag(tagCardShadow);
        if (picked) {
            cardBody->setPosition(cardBody->getPositionX(), cardBody->getPositionY()+20);
            cardShadow->setPosition(cardBody->getPositionX(), cardBody->getPositionY()-20);
        } else {
            cardBody->setPosition(cardBody->getPositionX(), cardBody->getPositionY()-20);
            cardShadow->setPosition(cardBody->getPositionX(), cardBody->getPositionY()+20);
        }
    };
    
    auto checkAnswer = [this]() {
        bool isCorrect = true;
        bool isComplete = true;
        for (auto it : _cards) {
            CCLOG("card:%d", it->getTag());
            CCLOG("slot:%d", it->getChildByName("slot")->getTag());
            if (it->getTag() != it->getChildByName("slot")->getTag()) isCorrect = false;
            if (it->getChildByName("slot")->getTag() < 1) isComplete = false;
        }
        if (isComplete) {
            _answerResult[_currentProblemIndex] = isCorrect ? true : false;
            onSolve();
        }
    };
    
    auto moveToOriginalPos = [this](Node* card, string posInfo) {
        card->getChildByName("slot")->setTag(0);
        auto posVec = TodoUtil::split(posInfo, ',');
        card->runAction(
                        Sequence::create(
                            MoveTo::create(0.1, Vec2(TodoUtil::stoi(posVec[0]), TodoUtil::stoi(posVec[1]))),
                            CallFunc::create([this, card](){
                                card->getChildByTag(tagCardSnapped)->setName("false");
                            }),
                        nullptr)
        );
    };
    
    auto createCard = [this, setPickedToggle, checkAnswer, moveToOriginalPos](int number) {
        
        auto cardShadow = ui::Scale9Sprite::create("EggQuizLiteracy/Common/pretest_english_format1a_answer_normal_shade.png");
        cardShadow->setPreferredSize(Size(356,457));
        cardShadow->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
        cardShadow->setTag(tagCardShadow);
        
        auto cardBody = ui::Scale9Sprite::create("EggQuizLiteracy/Common/pretest_english_format1a_answer_normal.png");
        cardBody->setPreferredSize(Size(356,457));
        cardBody->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
        cardBody->setTag(tagCardBody);
        
        auto cardSlot = Node::create();
        cardSlot->setName("slot");
        
        auto originalPos = Node::create();
        originalPos->setTag(tagCardOriginalPos);

        Size cardSize = cardBody->getContentSize();
        
        auto card = Node::create();
        card->setContentSize(cardBody->getContentSize());
        
        auto snapped = Node::create();
        snapped->setTag(tagCardSnapped);
        
        card->addChild(cardShadow);
        card->addChild(cardBody);
        card->addChild(cardSlot);
        card->addChild(originalPos);
        card->addChild(snapped);
        card->setTag(number);
        
        auto label = TodoUtil::createLabel(TodoUtil::itos(number), 160, Size::ZERO, defaultFont, fontColor1, TextHAlignment::CENTER);
        cardBody->addChild(label);
        label->setPosition(cardBody->getContentSize().width/2, 191+117/2);
        
        auto *listener = EventListenerTouchOneByOne::create();
        listener->setSwallowTouches(true);
        listener->onTouchBegan = [this, card, setPickedToggle](Touch* T, Event* E) {
            if (card->getChildByTag(tagCardSnapped)->getName() == "true") return false;
            card->setLocalZOrder(_maxCardIndex++);

            auto P = card->getParent();
            auto pos = P->convertToNodeSpace(T->getLocation());
            
            if (card->getBoundingBox().containsPoint(pos)) {
                if (card->getChildByTag(tagCardOriginalPos)->getName() == "") {
                    card->getChildByTag(tagCardOriginalPos)->setName(TodoUtil::itos((int)card->getPositionX())+","+TodoUtil::itos((int)card->getPositionY()));
                }
                card->getChildByName("slot")->setTag(0);
                setPickedToggle(card, true);
                card->getChildByTag(tagCardSnapped)->setName("true");
                return true;
            }
            return false;
        };
        
        listener->onTouchMoved = [this, card](Touch* T, Event* E) {
            auto P = card->getParent();
            auto pl = P->convertToNodeSpace(T->getPreviousLocation());
            auto cl = P->convertToNodeSpace(T->getLocation());
            auto delta = cl-pl;
            card->setPosition(card->getPosition()+delta);
            for (auto it : _slots) {
                if (it->getBoundingBox().containsPoint(cl)) {
                    it->getChildByTag(tagSlotActive)->setVisible(true);
                } else it->getChildByTag(tagSlotActive)->setVisible(false);
            }
        };
        
        listener->onTouchEnded = [this, card, setPickedToggle, checkAnswer, moveToOriginalPos](Touch* T, Event* E) {
            setPickedToggle(card, false);

            auto P = card->getParent();
            auto pos = P->convertToNodeSpace(T->getLocation());
            
            bool inSlot = false;

            for (auto it : _slots) {
                if (it->getBoundingBox().containsPoint(pos)) {
                    inSlot = true;
                    for (auto c : _cards) {
                        if (c->getChildByName("slot")->getTag() == it->getTag()) {
                            moveToOriginalPos(c, c->getChildByTag(tagCardOriginalPos)->getName());
                        }
                    }
                    card->runAction(MoveTo::create(0.1, Vec2(it->getPositionX()+23, it->getPositionY()+2)));
                    card->getChildByTag(tagCardSnapped)->setName("false");
                    // CCLOG("slot %d - x:%f y:%f in!", it->getTag(), it->getPositionX(), it->getPositionY());
                    card->getChildByName("slot")->setTag(it->getTag());
                }
                
            }
            if (!inSlot) {
                moveToOriginalPos(card, card->getChildByTag(tagCardOriginalPos)->getName());
            }
            checkAnswer();
        };

        card->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, card);
        
        return card;
    };
    
    float slotWidth = 402;
    float slotSpace = 40;
    float slotStartX = _gameSize.width/2 - slotWidth*2 - slotSpace*1.5;
    float slotY = 800;

    for (auto it : slotNumbers) {
        auto slot = createSlot(it);
        slot->setPosition(slotStartX, slotY);
        node->addChild(slot);
        _slots.push_back(slot);
        slotStartX += slotWidth + slotSpace;
    }
    
    float cardWidth = 356;
    float cardSpace = 80;
    float cardStartX = _gameSize.width/2 - cardWidth*2 - cardSpace*1.5;
    float cardY = 143;

    for (auto it : cardNumbers) {
        auto card = createCard(it);
        card->setPosition(cardStartX, cardY);
        node->addChild(card);
        _cards.push_back(card);
        cardStartX += cardWidth + cardSpace;
    }
    
    return node;
}

Node* EggQuizMathScene::createBiggerAndSmaller() {
    Node *node = Node::create();
    
    bool withImage = _problem.questionOption2 == "image" ? true : false;
    auto options = TodoUtil::split(_problem.answerOption1, ',');
    
    string label = "";
    if (_problem.questionOption1 == "bigger") {
        label = withImage ? "Which group is bigger?" : "Which is bigger?";
    } else {
        label = withImage ? "Which group is smaller?" : "Which is smaller?";
    }

    auto questionLabel = TodoUtil::createLabel(getText(label), 100, Size::ZERO, defaultFont, defaultColor, TextHAlignment::LEFT);
    questionLabel->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
    questionLabel->setPosition(_gameSize.width/2-756, 1400);
    node->addChild(questionLabel);
    
    auto soundButton = createSoundButton(getFilenameFromText(label), 2.f, 200.f, true);
    soundButton->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
    soundButton->setPosition(-220, 80);
    questionLabel->addChild(soundButton);
    
    
    auto addButton = [](){
        auto button = ui::Scale9Sprite::create("EggQuizLiteracy/Common/pretest_english_format1a_answer_normal.png");
        button->setPreferredSize(Size(930,930));
        
        auto buttonHighlight = ui::Scale9Sprite::create("EggQuizLiteracy/Common/pretest_english_format1a_answer_active.png");
        buttonHighlight->setPosition(button->getContentSize()/2);
        buttonHighlight->setPreferredSize(Size(930,930));
        buttonHighlight->setTag(1);
        buttonHighlight->setVisible(false);
        button->addChild(buttonHighlight);
        return button;
    };
    
    auto addImageToButton = [this](Scale9Sprite* button, int count, string imagePath, float scale = 1){
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
    
    auto addTextToButton = [](Scale9Sprite* button, string number){
        auto label = TodoUtil::createLabel(number, 237, Size::ZERO, boldFont, defaultColor, TextHAlignment::CENTER);
        label->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        label->setPosition(button->getContentSize()/2);
        button->addChild(label);
    };
    
    auto addListenerToButton = [this](Scale9Sprite* button, string answer) {
        auto *listener = EventListenerTouchOneByOne::create();
        listener->setSwallowTouches(true);
        listener->onTouchBegan = [this, button, answer](Touch* T, Event* E) {
            auto pos = button->getParent()->convertToNodeSpace(T->getLocation());
            if (button->getBoundingBox().containsPoint(pos)) {
                button->getChildByTag(1)->setVisible(true);
                return true;
            }
            return false;
        };
        
        listener->onTouchEnded = [this, button, answer](Touch* T, Event* E){
            button->getChildByTag(1)->setVisible(false);
            
            auto pos = button->getParent()->convertToNodeSpace(T->getLocation());
            if (button->getBoundingBox().containsPoint(pos)) {
                CCLOG("answer:%s", _answer.c_str());
                CCLOG("try:%s", answer.c_str());
                if (_answer == answer) this->onCorrect(_answer, 0);
                else this->onIncorrect(_answer, answer, 0);
            }
        };
        button->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);
    };

    auto button1 = addButton();
    button1->setAnchorPoint(Vec2::ANCHOR_BOTTOM_RIGHT);
    button1->setPosition(_gameSize.width/2-28, 166);
    node->addChild(button1);
    
    auto button2 = addButton();
    button2->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
    button2->setPosition(_gameSize.width/2+28, 166);
    node->addChild(button2);
    
    addListenerToButton(button1, options[0]);
    addListenerToButton(button2, options[1]);
    
    if (withImage) {
        addImageToButton(button1, TodoUtil::stoi(options[0]), "EggQuizLiteracy/Common/pretest_math_dot_yellow.png");
        addImageToButton(button2, TodoUtil::stoi(options[1]), "EggQuizLiteracy/Common/pretest_math_acorn.png", 0.65);
    } else {
        addTextToButton(button1, options[0]);
        addTextToButton(button2, options[1]);
    }
    return node;

}

Node* EggQuizMathScene::createSingleDigitNumbersView() {
    Node *node = Node::create();
    node->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    
    auto viewSize = Size(1890, 760);
    node->setContentSize(viewSize);
    
    auto bg = ui::Scale9Sprite::create("EggQuizLiteracy/Common/pretest_questionbox.png");
    bg->setPreferredSize(viewSize);
    bg->setPosition(viewSize/2);
    node->addChild(bg);
    
    auto soundButton = createSoundButton("EggQuizMath/Sounds/howmany.m4a", 1.f, 200.f, true);
    soundButton->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    soundButton->setPosition(0, bg->getContentSize().height);
    bg->addChild(soundButton);

    vector<Sprite*> stones;
    for (int i=0; i<_answerInt; i++) {
        auto stone = Sprite::create("EggQuizLiteracy/Common/pretest_math_acorn.png");
        Size margin = stone->getContentSize();
        stone->setRotation(random(-60.0, 60.0));
        
        Vec2 bestPos;
        float bestMinDist = -1;
        
        for (int j=0; j<10; j++) {
            Vec2 pos = Vec2(random(margin.width, viewSize.width-margin.width), random(margin.height, viewSize.height-margin.height));
            
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
        node->addChild(stone);
    }
    
    return node;
}

Node* EggQuizMathScene::create2DigitNumbersView() {
    Node *node = Node::create();
    node->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    
    auto viewSize = Size(1890, 760);
    node->setContentSize(viewSize);
    
    auto bg = ui::Scale9Sprite::create("EggQuizLiteracy/Common/pretest_questionbox.png");
    bg->setPreferredSize(viewSize);
    bg->setPosition(viewSize/2);
    node->addChild(bg);
    
    auto soundButton = createSoundButton("EggQuizMath/Sounds/howmany.m4a", 1.f, 200.f, true);
    soundButton->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    soundButton->setPosition(0, bg->getContentSize().height);
    bg->addChild(soundButton);
    
    vector<Sprite*> stones;
    int numSection = (_answerInt+9) / 10;
    auto sectionSize = Size(1518, 132);
    auto space = 10;
    
    auto tView = Node::create();
    tView->setContentSize(viewSize);
    tView->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    tView->setPosition(viewSize/2);
    float sectionHeight = sectionSize.height*numSection + space*(numSection+1);
    if (sectionHeight>viewSize.height) {
        auto scale = viewSize.height / sectionHeight;
        tView->setScale(scale);
    }
    node->addChild(tView);
    
    auto occ = space*(numSection-1)+sectionSize.height*numSection;
    auto top = viewSize.height - (viewSize.height-occ)/2;
    auto left = sectionSize.width - ( 112*10 + 34*9 );
    
    for (int i=0; i<numSection; i++) {
        int numInRow = 10;
        if (i==numSection-1) numInRow = _answerInt - (10*i);
        
        auto section = Sprite::create("EggQuizLiteracy/Common/pretest_math_10set_outline.png");
        section->setPosition(viewSize.width/2, top - i*(sectionSize.height+space) - sectionSize.height/2);
        tView->addChild(section);
        
        for (int j=0; j<numInRow; j++) {
            auto dot = Sprite::create("EggQuizLiteracy/Common/pretest_math_dot_yellow.png");
            dot->setPosition(left + (112+34)*j, sectionSize.height/2);
            section->addChild(dot);
        }
    }
    return node;
}

Node* EggQuizMathScene::createRecognizeNumberView() {
    Node *questionView;
    std::string fileName = GameSoundManager::getInstance()->getAdultVoiceName(_answerInt);
    //std::string prefix = "Common/AdultVoice/" + LanguageManager::getInstance()->getCurrentLanguageCode()+"/";
    auto soundPath = LanguageManager::getInstance()->findLocalizedResource("NumberVoice/"+fileName);
    
    auto soundButton = createSoundButton(soundPath, 1.f, 500.f, true);
    soundButton->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    questionView = soundButton;
    
    questionView->setPosition(_gameSize.width/2, 1000);
    return questionView;
}

Node* EggQuizMathScene::createMissingNumberView() {
    
    auto blank = TodoUtil::stoi(_problem.questionOption2) - 1;
    auto answerRangeVec = TodoUtil::split(_problem.answerRange, ',');
    
    Node *node = Node::create();
    node->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    
    const Size btnSize = Size(434, 566);
    const float space = 64;
    Size viewSize = Size(btnSize.width*4+space*3, btnSize.height);
    
    node->setContentSize(viewSize);
    node->setScale(0.5);
    
    for (int i=0; i<answerRangeVec.size(); i++) {
        auto it = answerRangeVec[i];
        
        auto btn = Node::create();
        btn->setContentSize(btnSize);
        btn->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        btn->setPosition(Vec2((btnSize.width+space)*i+btnSize.width/2, viewSize.height/2));
        node->addChild(btn);
        
        auto sp = ui::Scale9Sprite::create("EggQuizLiteracy/Common/pretest_questionbox.png");
        sp->setPreferredSize(btnSize);
        sp->setPosition(btnSize/2);
        btn->addChild(sp);
        
        if (i == blank) {
            auto q = Sprite::create("EggQuizLiteracy/Common/pretest_math_emptyanswer_dotted_large.png");
            q->setPosition(btnSize/2);
            btn->addChild(q);
            _answer = it;
            _answerInt = TodoUtil::stoi(it);
        } else {
            auto l = TodoUtil::createLabel(it, 150, Size::ZERO, defaultFont, Color4B(70, 70, 70, 255));
            l->setPosition(btnSize/2);
            btn->addChild(l);
        }
    }
    
    return node;
}

Node* EggQuizMathScene::createOperationsWithObjects() {
    
    Node *node = Node::create();
    
    auto viewSize = Size(1030, 1245);
    node->setContentSize(viewSize);
    
    auto bg = ui::Scale9Sprite::create("EggQuizLiteracy/Common/pretest_questionbox.png");
    bg->setPreferredSize(viewSize);
    bg->setPosition(viewSize/2);
    node->addChild(bg);

    auto problem = _problems.at(_currentProblemIndex);
    auto fomulaV = ProblemBank::getInstance()->parseFomula(problem.questionOption1);

    float fomulaSpace = 40;
    float ObjectSpace = 40;

    Node *fomulaNode = Node::create();
    float fomulaNodeWidth = 0;
    float fomulaNodeHeight = 150;
    
    float objectNodeFitWidth = 800;

    for (auto it : fomulaV) {
        if (it[0] == '?') {
            auto q = Sprite::create("EggQuizLiteracy/Common/pretest_math_emptyanswer_dotted_small.png");
            fomulaNode->addChild(q);
            q->setPosition(fomulaNodeWidth + q->getContentSize().width/2, -10);
            fomulaNodeWidth += q->getContentSize().width + fomulaSpace;
        } else {
            auto l = TodoUtil::createLabel(it, fomulaNodeHeight, Size::ZERO, defaultFont, Color4B(70, 70, 70, 255));
            fomulaNode->addChild(l);
            l->setPosition(fomulaNodeWidth + l->getContentSize().width/2, 0);
            fomulaNodeWidth += l->getContentSize().width + fomulaSpace;
            CCLOG("width:%f", l->getContentSize().width);
            CCLOG("objectsNodeWidth:%f", fomulaNodeWidth);
        }
    }
    fomulaNode->setContentSize(Size(fomulaNodeWidth - fomulaSpace, fomulaNodeHeight));
    fomulaNode->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    node->addChild(fomulaNode);
    
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
            case 'y': b = Sprite::create("EggQuizLiteracy/Common/pretest_math_dot_yellow.png"); break;
            case 'd': b = Sprite::create("EggQuizLiteracy/Common/pretest_math_dot_yellow_empty.png"); break;
            case 'b': b = Sprite::create("EggQuizLiteracy/Common/pretest_math_dot_blue.png"); break;
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

    node->addChild(objectsNode);
    
    fomulaNode->setPosition(viewSize.width/2,viewSize.height/2-200);
    objectsNode->setPosition(viewSize.width/2,viewSize.height/2+240);

    return node;
}

Node* EggQuizMathScene::createOperationsWithoutObjects() {
    
    Node *node = Node::create();
    
    auto problem = _problems.at(_currentProblemIndex);
    auto fomulaV = ProblemBank::getInstance()->parseFomula(problem.questionOption1);

    if (problem.questionOption3 == "vertical") {
        if (fomulaV.size() != 5) return node;
        auto viewSize = Size(1030, 1234);
        node->setContentSize(viewSize);
        node->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);

        auto bg = ui::Scale9Sprite::create("EggQuizLiteracy/Common/pretest_questionbox.png");
        bg->setPreferredSize(viewSize);
        bg->setPosition(viewSize.width/2, viewSize.height/2);
        node->addChild(bg);
        
        Node *fomulaNode = Node::create();
        
        float fomulaNodeX = 0;
        float fomulaNodeY = 300;
        float lineHeight = 270;
        
        auto num1 = TodoUtil::createLabel(fomulaV[0], 210, Size(600,210), defaultFont, Color4B(70, 70, 70, 255), TextHAlignment::RIGHT);
        num1->setPosition(fomulaNodeX, fomulaNodeY);
        fomulaNode->addChild(num1);
        
        auto num2 = TodoUtil::createLabel(fomulaV[2], 210, Size(600,210), defaultFont, Color4B(70, 70, 70, 255), TextHAlignment::RIGHT);
        num2->setPosition(num1->getPositionX(), num1->getPositionY()-lineHeight);
        fomulaNode->addChild(num2);
        
        auto op = TodoUtil::createLabel(fomulaV[1], 210, Size(600,210), defaultFont, Color4B(70, 70, 70, 255), TextHAlignment::RIGHT);
        op->setPosition(num1->getPositionX()-560, num1->getPositionY()-lineHeight);
        fomulaNode->addChild(op);

        auto stroke = LayerColor::create(Color4B(70, 70, 70, 255), 700, 12);
        stroke->setPosition(-400,-150);
        fomulaNode->addChild(stroke);

        auto questionBox = Sprite::create("EggQuizLiteracy/Common/pretest_math_emptyanswer_dotted_large.png");
        fomulaNode->addChild(questionBox);
        questionBox->setAnchorPoint(Vec2::ANCHOR_TOP_RIGHT);
        questionBox->setPosition(290, -228);
        questionBox->setScale(0.8);

        fomulaNode->setContentSize(Size(fomulaNodeX, lineHeight*3));
        fomulaNode->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        fomulaNode->setPosition(viewSize.width/2,viewSize.height/2 + lineHeight*3/2);
        node->addChild(fomulaNode);
        
    } else if (problem.questionOption3 == "horizontal") {
        
        auto viewSize = Size(1030, 445);
        node->setContentSize(viewSize);
        node->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
        
        auto bg = ui::Scale9Sprite::create("EggQuizLiteracy/Common/pretest_questionbox.png");
        bg->setPreferredSize(viewSize);
        bg->setPosition(viewSize.width/2, viewSize.height/2);
        node->addChild(bg);
        
        float fomulaSpace = 40;
        
        Node *fomulaNode = Node::create();
        float fomulaNodeWidth = 0;
        float fomulaNodeHeight = 150;
        
        for (auto it : fomulaV) {
            if (it[0] == '?') {
                auto q = Sprite::create("EggQuizLiteracy/Common/pretest_math_emptyanswer_dotted_small.png");
                fomulaNode->addChild(q);
                q->setPosition(fomulaNodeWidth + q->getContentSize().width/2, -10);
                fomulaNodeWidth += q->getContentSize().width + fomulaSpace;
            } else {
                auto l = TodoUtil::createLabel(it, fomulaNodeHeight, Size::ZERO, defaultFont, Color4B(70, 70, 70, 255));
                fomulaNode->addChild(l);
                l->setPosition(fomulaNodeWidth + l->getContentSize().width/2, 0);
                fomulaNodeWidth += l->getContentSize().width + fomulaSpace;
            }
        }
        fomulaNode->setContentSize(Size(fomulaNodeWidth - fomulaSpace, fomulaNodeHeight));
        fomulaNode->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        node->addChild(fomulaNode);
        
        auto fomulaScale = 800/fomulaNodeWidth;
        fomulaNode->setScale(fomulaScale < 3 ? fomulaScale : 3);
        
        fomulaNode->setPosition(viewSize.width/2,viewSize.height/2 + fomulaNodeHeight/2);
    }
    
    return node;
}

Node* EggQuizMathScene::createAnswerMultipleChoiceView(vector<string>& choices, vector<string> paths, int choiceType)
{
    _buttons.clear();
    Node *node = Node::create();
    node->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    float maxLabelWidth = 0;
    float maxLabelHeight = 0;

    const Size btnSize = choices.size()==9 ? Size(185, 458) : Size(438, 458);
    const float space = 30;
    Size viewSize = Size(btnSize.width*choices.size()+space*(choices.size()-1), btnSize.height);
    node->setContentSize(viewSize);
    
    for (int i=0; i<choices.size(); i++) {
        
        auto a = EggQuizMathButton::create();
        _buttons.push_back(a);
        if (choiceType == choiceTypeImage) {
            a->setImage(paths[i]);
        } else {
            a->setWord(choices[i], btnSize);
        }
        if (choiceType != choiceTypeImage && maxLabelWidth < a->getLabelSize().width) maxLabelWidth = a->getLabelSize().width;
        if (choiceType != choiceTypeImage && maxLabelHeight < a->getLabelSize().height) maxLabelHeight = a->getLabelSize().height;
        a->setPosition(Vec2((btnSize.width+space)*i+btnSize.width/2, viewSize.height/2));
        node->addChild(a);
        
        bool correctAnswer = (choices[i]==_answer);
        auto cp = _currentProblemIndex;
        
        a->addClickEventListener([this, correctAnswer, choices, cp, i](Ref*) {
            if (!isTouchEnabled()) return;
            if (cp!=_currentProblemIndex) return;
            
            if (correctAnswer) {
                this->onCorrect(_answer, i);
            } else {
                this->onIncorrect(_answer, choices[i], i);
            }
        });
    }
    
    float maxSize = MAX(maxLabelWidth, maxLabelHeight);
    if (maxSize > 400) {
        for(auto it : _buttons) {
            it->setLabelScaleManual(maxSize);
        }
    }
    return node;
}

Node* EggQuizMathScene::createAnswerNumberPadView() {

    auto answerPad = Node::create();
    auto answerPadBg = Sprite::create("EggQuizLiteracy/Common/pretest_math_calculator_bg.png");
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
    auto answerLabelPos = Point(answerPadSize.width/2, 1040);
    
    const auto normalColor = fontColor1;
    const auto highlightColor = fontColor1;
    _answerString = "";
    
    Label* answerLabel = TodoUtil::createLabel(_answerString, 220, Size(750, 190), defaultFont, Color4B::WHITE, TextHAlignment::RIGHT);
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
        
        
        if (i >= 0) {
            btn = ui::Button::create("EggQuizLiteracy/Common/pretest_math_calculator_button_normal.png",
                                     "EggQuizLiteracy/Common/pretest_math_calculator_button_active.png");
            btnLabel = TodoUtil::createLabel(TodoUtil::itos(i), fontSize, Size::ZERO, defaultFont, normalColor);
            if (i==0) {
                btnPos = panelPos(rowCount - 1, 1);
            } else {
                btnPos = panelPos((9-i)/colCount, colCount - (9-i)%colCount - 1);
            }
        } else if (i==-1) {
            btn = ui::Button::create("EggQuizLiteracy/Common/pretest_math_calculator_button_normal.png",
                                     "EggQuizLiteracy/Common/pretest_math_calculator_button_active.png");
            btnLabel = Label::create();
            auto backsp = Sprite::create("EggQuizLiteracy/Common/pretest_math_calculator_back.png");
            backsp->setPosition(panelSize/2 + Size(0, 8));
            btn->addChild(backsp);
            btnPos = panelPos(rowCount - 1, 0);
        } else if (i==-2) {
            btn = ui::Button::create("EggQuizLiteracy/Common/pretest_math_calculator_button_enter_normal.png",
                                     "EggQuizLiteracy/Common/pretest_math_calculator_button_enter_active.png");
            btnLabel = Label::create();
            btnPos = panelPos(rowCount - 1, 2);
        }
        btnLabel->setPosition(btn->getContentSize()/2 + Size(0, 8));
        btn->addChild(btnLabel);
        
        btn->setZoomScale(0);
        btn->setPosition(btnPos);
        btn->setTag(i);
        btn->addTouchEventListener([btn, btnLabel, normalColor, highlightColor, this](Ref* sender,Widget::TouchEventType event) {
            if (!_touchEnabled) return;
            btnLabel->setTextColor(btn->isHighlighted() ? highlightColor : normalColor);
            
        });
        
        btn->addClickEventListener([btn, i, this, answerLabel](Ref* sender) {
            if (!_touchEnabled) return;
            
            GameSoundManager::getInstance()->playEffectSound(touchEffect);
            auto ans = btn->getTag();
            if (ans==-2) {
                //enter
                if (_answerString.length()==0) return;
                
                if (_answerString == _answer) {
                    this->onCorrect(_answer, i);
                } else {
                    this->onIncorrect(_answer, _answerString, i);
                }
                
            } else if (ans==-1) {
                //back
                auto l = _answerString.length();
                _answerString = _answerString.substr(0, l-1);
                answerLabel->setString(_answerString);
                
            } else {
                if (_answerString.size() <= 4) {
                    _answerString += TodoUtil::itos(ans);
                    answerLabel->setString(_answerString);
                }
            }
        });
        
        answerPad->addChild(btn);
    }
    return answerPad;
}

void EggQuizMathScene::updateProgressBar(bool setCurrent) {
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

void EggQuizMathScene::onCorrect(const string& answer, int answerIndex)
{
    LOGFN();
    StrictLogManager::shared()->eggQuiz_CorrectAnswer(_currentProblemIndex, answer, answerIndex);
    
    _answerResult[_currentProblemIndex] = true;
    onSolve();
}

void EggQuizMathScene::onIncorrect(const string& correctAnswer, const string& myAnswer, int answerIndex)
{
    LOGFN();
    StrictLogManager::shared()->eggQuiz_WrongAnswer(_currentProblemIndex, correctAnswer, myAnswer, answerIndex);
    
    _answerResult[_currentProblemIndex] = false;
    onSolve();
}

void EggQuizMathScene::onSolve()
{
    LOGFN();
    GameSoundManager::getInstance()->stopAllEffects();
    GameSoundManager::getInstance()->stopBGM();

    stopActionByTag(tagAutoSequence);
    stopActionByTag(tagStopSoundButtonOff);

    setTouchEnabled(false);
    Director::getInstance()->getEventDispatcher()->removeEventListenersForTarget(this);

    _currentProblemIndex++;
    updateProgressBar(true);

    if (_currentProblemIndex == _problems.size()) {
        onComplete();
        return;
    }
    
    _problemNode->runAction(
        Sequence::create(
            DelayTime::create(0.5f),
            CallFunc::create([this](){ _problemNode->removeAllChildren();}),
            DelayTime::create(0.1f),
            CallFunc::create([this](){
                this->setTouchEnabled(true);
                showProblem();
            }),
        nullptr)
    );
    
    /*
    GameSoundManager::getInstance()->playEffectSound(pageTurnEffect);
    
    this->runAction(Sequence::create(
        DelayTime::create(0.8),
        CallFunc::create([this](){
            updateProgressBar();
            this->setTouchEnabled(true);
        }),
        nullptr
    ));
    
    previousPageGrid->runAction(Sequence::create(
        DelayTime::create(0.1),
        PageTurn3D::create(1.5, Size(100,100)),
        CallFunc::create([this, previousPageGrid](){
        previousPageGrid->removeFromParent();
        }),
        nullptr
    ));
     */
}

void EggQuizMathScene::onComplete() {
    
    int numCorrect = 0;
    for (auto s : _answerResult) {
        if (s) numCorrect++;
    }
    bool cleared = (numCorrect>=_problems.size()*0.8);
    
    if (cleared) {
        passTest();
        
    } else {
        failTest();
    }
    
}
void EggQuizMathScene::passTest()
{
    auto popup = EggQuizPopup::create(this, _winSize);
    GameSoundManager::getInstance()->playEffectSoundForAutoStart("Common/Sounds/Effect/pretest_success.m4a");
    
    auto levelVec = TodoUtil::split(_currentLevel, '_');
    if (levelVec.size()>1) {
        popup->setPostCompleted('M', TodoUtil::stoi(levelVec[1]));
    }
    
    /*
    if (_isPreTest) {
        //popup->setPreCompleted('M', _currentLevel);
    } else {
        //popup->setPostCompleted('M', _currentLevel);
    }
     */
    
    this->runAction(Sequence::create(DelayTime::create(1.0),
                                     CallFunc::create([](){SoundEffect::wowEffect().play();}),
                                     nullptr
                                     ));
    
    popup->show(this, true, CallFunc::create([this](){
        CCAppController::sharedAppController()->handleGameComplete(1);
        
        if (onCompleteCallback) {
            onCompleteCallback(true);
        }
    }));
    
    
}

void EggQuizMathScene::failTest()
{
    auto popup = EggQuizPopup::create(this,_winSize);
    
    GameSoundManager::getInstance()->playEffectSoundForAutoStart("Common/Sounds/Effect/pretest_fail1.m4a");
    
    auto levelVec = TodoUtil::split(_currentLevel, '_');
    if (levelVec.size()>1) {
        popup->setPostFailed('M', TodoUtil::stoi(levelVec[1]));
    }

    /* if (_isPreTest) {
        popup->setPreFailed('M', _currentLevel);
    } else {
        popup->setPostFailed('M', _currentLevel);
    }
     */
    
    popup->show(this, true, CallFunc::create([this](){
        
        CCAppController::sharedAppController()->handleGameQuit();
        if (onCompleteCallback) {
            onCompleteCallback(false);
        }
    }));
}

void EggQuizMathScene::onExit() {
    LOGFN();
    Layer::onExit();
    
    this->removeAllChildren();
    this->unscheduleAllCallbacks();
    GameSoundManager::getInstance()->stopAllEffects();
    SpriteFrameCache::getInstance()->removeUnusedSpriteFrames();
    Director::getInstance()->getTextureCache()->removeUnusedTextures();
    
}

string EggQuizMathScene::getText(string key) {
    if (LanguageManager::getInstance()->getCurrentLanguageCode() == "en") {
        return _textEn[key];
    } else if (LanguageManager::getInstance()->getCurrentLanguageCode() == "sw") {
        return _textSw[key];
    } else return "";
}

void EggQuizMathScene::initTextMap() {
    
    _textEn["Which is the biggest?"] = "Which is the biggest?";
    _textSw["Which is the biggest?"] = "Ipi ni kubwa kabisa?";
    
    _textEn["Which is the smallest?"] = "Which is the smallest?";
    _textSw["Which is the smallest?"] = "Ipi ni ndogo kabisa?";

    _textEn["Which group is bigger?"] = "Which group is bigger?";
    _textSw["Which group is bigger?"] = "Kundi gani ni kubwa zaidi?";
    
    _textEn["Which is bigger?"] = "Which is bigger?";
    _textSw["Which is bigger?"] = "Ipi ni kubwa zaidi?";

    _textEn["Which group is smaller?"] = "Which group is smaller?";
    _textSw["Which group is smaller?"] = "Kundi gani ni dogo zaidi?";
    
    _textEn["Which is smaller?"] = "Which is smaller?";
    _textSw["Which is smaller?"] = "Ipi ni ndogo zaidi?";
    
    _textEn["Fill in the blank."] = "Fill in the blank.";
    _textSw["Fill in the blank."] = "Jaza nafasi.";
    
    _textEn["Order the numbers from the smallest to the largest."] = "Order the numbers from the smallest to the largest.";
    _textSw["Order the numbers from the smallest to the largest."] = "Panga namba kutoka ndogo zaidi mpaka kubwa zaidi.";
    
    _textEn["Order the numbers from the largest to the smallest."] = "Order the numbers from the largest to the smallest.";
    _textSw["Order the numbers from the largest to the smallest."] = "Panga namba kutoka kubwa zaidi mpaka ndogo zaidi.";
    
}

END_NS_EGGQUIZMATH;

