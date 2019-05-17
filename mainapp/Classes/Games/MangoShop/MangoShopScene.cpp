//
//  MangoShopScene.cpp
//  KitkitSchool
//
//  Created by HyeonGyu Yu on 3/6/18.
//

#include "MangoShopScene.hpp"
#include "Common/Basic/DeviceSpec.h"
#include "Common/Controls/TodoSchoolBackButton.hpp"
#include "CCAppController.hpp"
#include "Common/Controls/CompletePopup.hpp"
#include "Utils/TodoUtil.h"
#include "Managers/GameSoundManager.h"
#include "Managers/UserManager.hpp"
#include "Managers/StrictLogManager.h"


#define MANGOSHOP_TUTORIAL_LEVEL 5
#define MANGO_MOVE_ENALBLE [](){Mango::_isOtherMovingCard=false;}
namespace MangoShop
{
    const char* kDataFile = "MangoShop/Mangoshop_Level.tsv";
    const Size _gameSize = Size(2560.f, 1800.f);

    MangoShopScene::MangoShopScene():
    _isSumExpression(false),
    _isLandscape(false),
    _currentProblemIndex(0),
    boxRootNode(nullptr),
    numberCardsRoot(nullptr),
    numberExpressionRoot(nullptr),
    _tutorialHand(nullptr),
    _tutorialLayer1(nullptr),
    _tutorialLayer2(nullptr)
    {
        
    }
    
    Scene* MangoShopScene::createScene(int levelID)
    {
        // 'scene' is an autorelease object
        auto scene = Scene::create();
        
        // 'layer' is an autorelease object
        auto layer = MangoShopScene::create();
        layer->setLevel(levelID);
        
        // add layer as a child to scene
        scene->addChild(layer);
        
        // return the scene
        return scene;
    }

    bool MangoShopScene::init()
    {
        if (!Layer::init())
        {
            return false;
        }
        
        initUI();
        
        return true;
    }

    void MangoShopScene::onEnter()
    {
        Layer::onEnter();
        
        initData(&_currentSheetNo);
    }

    void MangoShopScene::onEnterTransitionDidFinish()
    {
        onStart();
    }


    // *****************************
    // Custom Methods
    // *****************************
    void MangoShopScene::initData(int *workSheetNo)
    {
        *workSheetNo = 1;

        std::string rawString = cocos2d::FileUtils::getInstance()->getStringFromFile(kDataFile);
        
        std::vector<Problem*> result;
        _problems.clear();
        auto data = TodoUtil::readTSV(rawString);
        
        for (auto row : data)
        {
            if (row.size() < 1)
                continue;
            
            if (TodoUtil::trim(row[0]).size() <= 0)
                continue;
            
            if (row[0][0] == '#')
                continue;
            
            if (TodoUtil::stoi(row[1]) != _currentLevel)
                continue;
            
            auto p = new Problem();
            p->level = row[1];
            p->worksheet = row[2];
            p->problem = row[3];
            p->category = row[4];
            p->question = row[5];
            p->answer = row[6];
            p->isRandom = row[7];
            
            result.push_back(p);
        }
        
        int maxWorksheetNumber = 0;
        //bool isRandom = result[0]->isRandom == "TRUE" ? true : false;
        for (int i = 0; i < result.size(); i++)
        {
            int currentWorksheetNumber = TodoUtil::stoi(result[i]->worksheet);
            if (maxWorksheetNumber < currentWorksheetNumber)
            {
                maxWorksheetNumber = currentWorksheetNumber;
            }
        }
        
        if (maxWorksheetNumber > 1)
        {
            *workSheetNo = random(1, maxWorksheetNumber);
            for (int i = 0; i < result.size(); i++)
            {
                if (*workSheetNo != TodoUtil::stoi(result[i]->worksheet))
                    continue;
                
                _problems.push_back(result[i]);
            }
        }
        else
        {
            random_shuffle(result.begin(), result.end());
            for (int i = 0; i < 5; i++)
            {
                _problems.push_back(result[i]);
            }
        }
    }
    
    void MangoShopScene::initUI()
    {
        const Size winSize = this->getContentSize();
        
        {
            auto background = Sprite::create("MangoShop/mango-shop_image_background.png");
            background->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
            background->setPosition(winSize / 2);
            this->addChild(background);
            
            auto leftDeco = Sprite::create("MangoShop/mango-shop_image_background-element_left.png");
            leftDeco->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
            leftDeco->setPosition(0.f, winSize.height);
            this->addChild(leftDeco);
            
            auto rightDeco = Sprite::create("MangoShop/mango-shop_image_background-element_right.png");
            rightDeco->setAnchorPoint(Vec2::ANCHOR_TOP_RIGHT);
            rightDeco->setPosition(winSize.width, winSize.height);
            this->addChild(rightDeco);
        }

        {
            auto backButton = TodoSchoolBackButton::create();
            backButton->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
            backButton->setPosition(Point(25.f, winSize.height - 25.f));
            backButton->addClickEventListener([this](Ref*) {
            
            });
            this->addChild(backButton);
        }
        
        _gameNode = Node::create();
        _gameNode->setContentSize(_gameSize);
        _gameNode->setScale(MIN(winSize.width/_gameSize.width, winSize.height/_gameSize.height));
        _gameNode->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        _gameNode->setPosition(this->getContentSize() / 2);
        this->addChild(_gameNode);
        
        boxRootNode = Node::create();
        boxRootNode->setPosition(_gameNode->getContentSize().width / 2, _gameNode->getContentSize().height / 2 + 300.f);
        _gameNode->addChild(boxRootNode);
        
        _progressBar = ProgressIndicator::create();
        _progressBar->setPosition(Vec2(winSize.width/2, winSize.height - _progressBar->getContentSize().height));
        addChild(_progressBar);
        
        if (UserManager::getInstance()->isDebugMode()) {
            auto skip = ui::Button::create();
            skip->setTitleFontSize(100);
            skip->setTitleText("Skip");
            skip->setAnchorPoint(Vec2::ANCHOR_TOP_RIGHT);
            skip->setPosition(Vec2(winSize.width-25, winSize.height-25));
            addChild(skip);
            skip->addTouchEventListener([this](Ref*,ui::Widget::TouchEventType e) {
                if (e == ui::Widget::TouchEventType::ENDED) {
                    this->onSolve();
                }
            });
        }
    }

    void MangoShopScene::drawTutorialLayerStep1()
    {
        if (MANGOSHOP_TUTORIAL_LEVEL != _currentLevel || _currentProblemIndex != 0)
            return;
        
        _tutorialLayer1 = Node::create();
        _tutorialLayer1->setContentSize(_gameNode->getContentSize());
        _tutorialLayer1->setScale(_gameNode->getScale());
        _tutorialLayer1->setAnchorPoint(_gameNode->getAnchorPoint());
        _tutorialLayer1->setPosition(_gameNode->getPosition());
        this->addChild(_tutorialLayer1);
        
        _tutorialHand = Sprite::create("LineMatching/tutorial_image_guideHand_normal.png");
        _tutorialHand->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
        _tutorialHand->setPosition(_tutorialLayer1->convertToNodeSpace(getFistLeftBoxWorldPositionForTutorial()));
        _tutorialLayer1->addChild(_tutorialHand);
        
        auto rf = RepeatForever::create(Sequence::create(DelayTime::create(0.5f), CallFunc::create([this](){
            _tutorialHand->setTexture("LineMatching/tutorial_image_guideHand_touch.png");
        }), DelayTime::create(0.5f), CallFunc::create([this](){
            _tutorialHand->setTexture("LineMatching/tutorial_image_guideHand_normal.png");
        }), nullptr));
        _tutorialLayer1->runAction(rf);
    }
    
    void MangoShopScene::drawTutorialLayerStep2()
    {
        if (MANGOSHOP_TUTORIAL_LEVEL != _currentLevel || _currentProblemIndex != 0)
            return;
        
        _tutorialLayer2 = Node::create();
        _tutorialLayer2->setContentSize(_gameNode->getContentSize());
        _tutorialLayer2->setScale(_gameNode->getScale());
        _tutorialLayer2->setAnchorPoint(_gameNode->getAnchorPoint());
        _tutorialLayer2->setPosition(_gameNode->getPosition());
        this->addChild(_tutorialLayer2);
        
        auto lp = _tutorialLayer2->convertToNodeSpace(getLeftWorldPositionForTutorial());
        auto rp = _tutorialLayer2->convertToNodeSpace(getRightWorldPositionForTutorial());
        
        _tutorialHand = Sprite::create("LineMatching/tutorial_image_guideHand_normal.png");
        _tutorialHand->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
        _tutorialHand->setPosition(lp);
        _tutorialLayer2->addChild(_tutorialHand);
        
        const float moveDelay = 0.8f;
        const float moveAfterDelay = 0.3f;
        
        auto rf = RepeatForever::create(Sequence::create(DelayTime::create(0.3f), CallFunc::create([this, rp](){
            _tutorialHand->setTexture("LineMatching/tutorial_image_guideHand_touch.png");
        }), DelayTime::create(0.2f), CallFunc::create([this, rp, moveDelay](){
            _tutorialHand->runAction(MoveTo::create(moveDelay, rp));
        }), DelayTime::create(moveDelay + moveAfterDelay), CallFunc::create([this](){
            _tutorialHand->setTexture("LineMatching/tutorial_image_guideHand_normal.png");
        }), DelayTime::create(0.2f), CallFunc::create([this, lp](){
            _tutorialHand->setPosition(lp);
        }), nullptr));
        _tutorialLayer2->runAction(rf);
    }
    
    Vec2 MangoShopScene::getFistLeftBoxWorldPositionForTutorial()
    {
        for (const auto &box : boxes)
        {
            if (box->isOnTray)
                continue;
            
            return box->getParent()->convertToWorldSpace(box->getPosition());
        }
        return Vec2::ZERO;
    }
    
    Vec2 MangoShopScene::getLeftWorldPositionForTutorial()
    {
        vector<Box*> boxVector;
        for (const auto &box : boxes)
        {
            if (box->isOnTray)
                continue;
            boxVector.push_back(box);
        }
        
        for (const auto &box : boxVector)
        {
            if (box->isFull())
                continue;
            
            int index = 0;
            for (const auto &s : box->getSlots())
            {
                if (s->targetMango != nullptr)
                    continue;
                
                if (index++ == 4)
                    return s->getParent()->convertToWorldSpace(s->getPosition());
            }
        }
        
        return Vec2::ZERO;
    }
    
    Vec2 MangoShopScene::getRightWorldPositionForTutorial()
    {
        vector<Box*> boxVector;
        for (const auto &box : boxes)
        {
            if (box->isOnTray == false)
                continue;
            boxVector.push_back(box);
        }
        
        for (const auto &box : boxVector)
        {
            if (box->isFull())
                continue;
            
            if (box->isShowCover())
                continue;
            
            for (const auto &s : box->getSlots())
            {
                if (s->targetMango != nullptr)
                    continue;
                
                return s->getParent()->convertToWorldSpace(s->getPosition());
            }
        }
        
        return Vec2::ZERO;
    }
    
    void MangoShopScene::setLevel(int level)
    {
        _currentLevel = level;
    }

    void MangoShopScene::onStart()
    {
        // 데이터 설정
        setProblem();
        
        // 오브젝트 설정
        setBoxes();
        setNumberExpression();
        setNumberCards();
        
        createShiningParticle();
        
        drawTutorialLayerStep1();
        
        NumberCard::_isOtherMovingCard = false;
        Mango::_isOtherMovingCard = false;
        
        // 프로그레스 바
        _progressBar->setMax(_problems.size());
        _progressBar->setCurrent(_currentProblemIndex + 1);
    }
    
    void MangoShopScene::setPreventTouches(bool value)
    {
        this->getEventDispatcher()->setEnabled(!value);
    }
    
    void MangoShopScene::setProblem()
    {
        _problemNumberStrings.clear();
        auto p = _problems[_currentProblemIndex];
        if (p->category == "SUB")
        {
            _problemSignString = "-";
            _problemNumberStrings = TodoUtil::split(p->question, '-');
            _isLandscape = true;
            
            int solutionNumber = TodoUtil::stoi(_problemNumberStrings[0]);
            for (int i = 1; i < _problemNumberStrings.size(); i++)
            {
                solutionNumber -= TodoUtil::stoi(_problemNumberStrings[i]);
            }
            _solutionNumberString = TodoUtil::itos(solutionNumber);
        }
        else
        {
            _problemSignString = "+";
            _problemNumberStrings = TodoUtil::split(p->question, '+');
            _isLandscape = _problemNumberStrings.size() > 2 ? false : true;
            
            int solutionNumber = TodoUtil::stoi(_problemNumberStrings[0]);
            for (int i = 1; i < _problemNumberStrings.size(); i++)
            {
                solutionNumber += TodoUtil::stoi(_problemNumberStrings[i]);
            }
            _solutionNumberString = TodoUtil::itos(solutionNumber);
        }
        //_solutionNumberString = p->answer;
    }
    
    bool MangoShopScene::addMango(Box* box)
    {
        for (const auto &s : box->getSlots())
        {
            if (s->targetMango != nullptr)
                continue;
            auto mango = Mango::create();
            mango->initWithOrientation(_isLandscape);
            mango->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
            auto wp = s->getParent()->convertToWorldSpace(s->getPosition());
            mango->setPosition(_gameNode->convertToNodeSpace(wp));
            bindingMangoEvent(mango);
            _gameNode->addChild(mango);
            s->targetMango = mango;
            mango->origin = mango->getPosition();
            mangos.push_back(mango);
            return true;
        }
        return false;
    }
    
    void MangoShopScene::addMangos(Box* box, int count, bool isAutoShowCover = true)
    {
        if (count == 10 && isAutoShowCover == true)
        {
            box->showCover(true, false);
            return;
        }
        
        for (int i = 0; i < count; i++)
        {
            if (addMango(box) == false)
                break;
        }
    }
    
    void MangoShopScene::setBoxes()
    {
        if (boxRootNode != nullptr)
        {
            boxRootNode->removeAllChildren();
        }
        boxes.clear();
        
        for (const auto &m : mangos)
        {
            m->removeFromParent();
        }
        mangos.clear();
        
        float kLeftOrRightSpace = 550.f;
        float kUpOrDownSpace = 250.f;
        
        Box* box;
        // Box 레이아웃 : 가로 형태
        if (_isLandscape)
        {
            CCASSERT(_problemNumberStrings.size() == 2, "The problem number strings size must be 2.");
            
            int leftValue = TodoUtil::stoi(_problemNumberStrings[0]);
            int rightValue = TodoUtil::stoi(_problemNumberStrings[1]);
                           
            // 왼쪽 영역
            if (leftValue > 10)
            {
                box = Box::create();
                box->initWithOrientation(_isLandscape);
                box->initMangoSlots(10, false);
                addMangos(box, 10);
                box->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
                box->setPosition(-kLeftOrRightSpace, kUpOrDownSpace);
                box->origin = box->getPosition();
                box->isBoxMovable = false;
                boxRootNode->addChild(box);
                boxes.push_back(box);
                bindingBoxEvent(box);
                
                box = Box::create();
                box->initWithOrientation(_isLandscape);
                box->initMangoSlots(10, false);
                box->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
                box->setPosition(-kLeftOrRightSpace, -kUpOrDownSpace);
                box->origin = box->getPosition();
                box->isBoxMovable = false;
                boxRootNode->addChild(box);
                boxes.push_back(box);
                bindingBoxEvent(box);
                addMangos(box, leftValue - 10);
            }
            else
            {
                box = Box::create();
                box->initWithOrientation(_isLandscape);
                box->initMangoSlots(10, false);
                box->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
                box->setPosition(-kLeftOrRightSpace, 0.f);
                box->origin = box->getPosition();
                box->isBoxMovable = false;
                boxRootNode->addChild(box);
                boxes.push_back(box);
                bindingBoxEvent(box);
                addMangos(box, leftValue);
            }
            
            // 오른쪽 영역 (+)
            if (_problemSignString == "+")
            {
                if (rightValue > 10)
                {
                    box = Box::create();
                    box->initWithOrientation(_isLandscape);
                    box->initMangoSlots(10, false);
                    box->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
                    box->setPosition(kLeftOrRightSpace, kUpOrDownSpace);
                    box->origin = box->getPosition();
                    box->isBoxMovable = false;
                    boxRootNode->addChild(box);
                    boxes.push_back(box);
                    bindingBoxEvent(box);
                    addMangos(box, 10);
                    
                    box = Box::create();
                    box->initWithOrientation(_isLandscape);
                    box->initMangoSlots(10, false);
                    box->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
                    box->setPosition(kLeftOrRightSpace, -kUpOrDownSpace);
                    box->origin = box->getPosition();
                    box->isBoxMovable = true;
                    boxRootNode->addChild(box);
                    boxes.push_back(box);
                    bindingBoxEvent(box);
                    addMangos(box, rightValue - 10);
                }
                else
                {
                    box = Box::create();
                    box->initWithOrientation(_isLandscape);
                    box->initMangoSlots(10, false);
                    box->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
                    box->setPosition(kLeftOrRightSpace, 0.f);
                    box->origin = box->getPosition();
                    box->isBoxMovable = false;
                    boxRootNode->addChild(box);
                    boxes.push_back(box);
                    bindingBoxEvent(box);
                    addMangos(box, rightValue);
                }
            }
            // 오른쪽 영역 (-)
            else
            {
                _tray = Tray::create();
                _tray->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
                _tray->setPosition(-50.f, -50.f);
                boxRootNode->addChild(_tray);
                boxes.push_back(box);
                
                if (rightValue > 10)
                {
                    box = Box::create();
                    box->initMinusLayoutWithOrientation(_isLandscape, true);
                    box->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
                    box->setPosition(kLeftOrRightSpace, kUpOrDownSpace);
                    box->origin = box->getPosition();
                    box->isBoxMovable = false;
                    box->isOnTray = true;
                    box->isSlot = true;
                    boxRootNode->addChild(box);
                    boxes.push_back(box);
                    bindingBoxEvent(box);
                    
                    box = Box::create();
                    box->initMinusLayoutWithOrientation(_isLandscape, false);
                    box->initMangoSlots(rightValue - 10, true);
                    box->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
                    box->setPosition(kLeftOrRightSpace, -kUpOrDownSpace);
                    box->origin = box->getPosition();
                    box->isBoxMovable = false;
                    box->isOnTray = true;
                    boxRootNode->addChild(box);
                    boxes.push_back(box);
                    bindingBoxEvent(box);
                }
                else
                {
                    box = Box::create();
                    box->initMinusLayoutWithOrientation(_isLandscape, false);
                    box->initMangoSlots(rightValue, true);
                    box->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
                    box->setPosition(kLeftOrRightSpace, 0.f);
                    box->origin = box->getPosition();
                    box->isBoxMovable = false;
                    box->isOnTray = true;
                    boxRootNode->addChild(box);
                    boxes.push_back(box);
                    bindingBoxEvent(box);
                }
            }
        }
        // Box 레이아웃 : 세로 형태 (덧셈만 가능)
        else
        {
            CCASSERT(_problemNumberStrings.size() == 3, "The problem number strings size must be 3.");
            CCASSERT(_problemSignString == "+", "The sign is must be '+'.");
            
            int leftValue = TodoUtil::stoi(_problemNumberStrings[0]);
            int middleValue = TodoUtil::stoi(_problemNumberStrings[1]);
            int rightValue = TodoUtil::stoi(_problemNumberStrings[2]);
            
            box = Box::create();
            box->initWithOrientation(_isLandscape);
            box->initMangoSlots(10, false);
            box->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
            box->setPosition(-600.f, 0.f);
            box->origin = box->getPosition();
            box->isBoxMovable = false;
            boxRootNode->addChild(box);
            boxes.push_back(box);
            bindingBoxEvent(box);
            addMangos(box, leftValue);
            
            box = Box::create();
            box->initWithOrientation(_isLandscape);
            box->initMangoSlots(10, false);
            box->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
            box->setPosition(0.f, 0.f);
            box->origin = box->getPosition();
            box->isBoxMovable = false;
            boxRootNode->addChild(box);
            boxes.push_back(box);
            bindingBoxEvent(box);
            addMangos(box, middleValue);
            
            box = Box::create();
            box->initWithOrientation(_isLandscape);
            box->initMangoSlots(10, false);
            box->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
            box->setPosition(600.f, 0.f);
            box->origin = box->getPosition();
            box->isBoxMovable = false;
            boxRootNode->addChild(box);
            boxes.push_back(box);
            bindingBoxEvent(box);
            addMangos(box, rightValue);
        }
    }
    
    void MangoShopScene::setNumberExpression()
    {
        float kGap = 50.f;
        float kFontSize = 220.f;
        float totalWidth = 0.f;
        
        _blankSlots.clear();
        
        if (numberExpressionRoot != nullptr)
        {
            numberExpressionRoot->removeAllChildren();
        }
        
        numberExpressionRoot = Node::create();
        
        // (s) one-pass
        bool isFirstTime = true;
        vector<Label*> labels;
        for (const auto &number : _problemNumberStrings)
        {
            if (!isFirstTime)
            {
                auto sign = TodoUtil::createLabel(_problemSignString, kFontSize, Size::ZERO, "fonts/mukta-bold.ttf", Color4B(255, 196, 0, 255 * 1.f));
                totalWidth += kGap;
                totalWidth += sign->getContentSize().width;
                labels.push_back(sign);
            }
            
            auto label = TodoUtil::createLabel(number, kFontSize, Size::ZERO, "fonts/mukta-bold.ttf", Color4B(255, 196, 0, 255 * 1.f));
            totalWidth += label->getContentSize().width;
            labels.push_back(label);
            
            isFirstTime = false;
        }
        
        auto sign = TodoUtil::createLabel("=", kFontSize, Size::ZERO, "fonts/mukta-bold.ttf", Color4B(255, 196, 0, 255 * 1.f));
        totalWidth += kGap;
        totalWidth += sign->getContentSize().width;
        
        auto blankSlot = BlankSlot::create();
        float blankSlotWidth = blankSlot->getContentSize().width * _solutionNumberString.size();
        
        numberExpressionRoot->setContentSize(Size(totalWidth + blankSlotWidth, blankSlot->getContentSize().height));
        // (e) one-pass
        
        // (s) two-pass
        float cursorX = 0.f;
        isFirstTime = true;
        for (const auto& label : labels)
        {
            if (isFirstTime == false)
            {
                cursorX += kGap;
            }
            label->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
            label->setPosition(cursorX + label->getContentSize().width / 2, numberExpressionRoot->getContentSize().height / 2 - 20.f);
            numberExpressionRoot->addChild(label);
            cursorX += label->getContentSize().width;
            isFirstTime = false;
        }
        
        cursorX += kGap;
        auto equalSign = TodoUtil::createLabel("=", kFontSize, Size::ZERO, "fonts/chanakya.ttf", Color4B(255, 196, 0, 255 * 1.f));
        equalSign->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        equalSign->setPosition(cursorX + equalSign->getContentSize().width / 2, numberExpressionRoot->getContentSize().height / 2 - 20.f);
        numberExpressionRoot->addChild(equalSign);
        cursorX += equalSign->getContentSize().width;
        
        cursorX += kGap;
        for (int i = 0; i < _solutionNumberString.size(); i++)
        {
            auto bs = BlankSlot::create();
            bs->numberString = _solutionNumberString.at(i);
            bs->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
            bs->setPosition(cursorX + bs->getContentSize().width / 2, numberExpressionRoot->getContentSize().height / 2);
            numberExpressionRoot->addChild(bs);
            cursorX += bs->getContentSize().width;
            _blankSlots.push_back(bs);
        }
        // (e) two-pass
        
        numberExpressionRoot->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        numberExpressionRoot->setPosition(_gameNode->getContentSize().width / 2, 500.f);
        _gameNode->addChild(numberExpressionRoot);
    }
    
    void MangoShopScene::setNumberCards()
    {
        if (numberCardsRoot != nullptr)
        {
            numberCardsRoot->removeAllChildren();
        }
        _numberCards.clear();
        
        numberCardsRoot = Node::create();
        
        _dummyNumberCard = NumberCard::create();
        _dummyNumberCard->setOpacity(255 * 0.5f);
        _dummyNumberCard->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        _dummyNumberCard->initWithNumberString("", 0.8f);
        _dummyNumberCard->setVisible(false);
        numberCardsRoot->addChild(_dummyNumberCard);
        
        float totalWidth = 0.f;
        float totalHeight = 0.f;

        for (int i = 0; i < 10; i++)
        {
            auto card = NumberCard::create();
            card->initWithNumberString(TodoUtil::itos(i), 0.8f);
            
            totalHeight = card->getContentSize().height;
            
            card->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
            card->setPosition(totalWidth + card->getContentSize().width / 2, totalHeight / 2);
            numberCardsRoot->addChild(card);
            
            totalWidth += card->getContentSize().width;
            if (i > 0) { totalWidth += 8.f; }
            
            _numberCards.push_back(card);
            
            bindingCardEvent(card);
        }
        
        numberCardsRoot->setContentSize(Size(totalWidth, totalHeight));
        numberCardsRoot->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
        numberCardsRoot->setPosition(_gameNode->getContentSize().width / 2, 50.f);
        _gameNode->addChild(numberCardsRoot);
    }
    
    void MangoShopScene::bindingBoxEvent(Box *box)
    {
        box->onStartParticle = [this](Vec2 pos) {
            startParticle(pos);
        };
        
        box->onStopParticle = [this]() {
            stopParticle();
        };
        
        box->onTouchBegan = [this, box]() {
            if (box->isShowCover())
            {
                box->retain();
                auto p = box->getParent()->convertToWorldSpace(box->getPosition());
                box->removeFromParent();
                box->setPosition(_gameNode->convertToNodeSpace(p));
                _gameNode->addChild(box);
                box->release();
            }
        };
        
        box->onTouchEnded = [this, box]() {
            box->retain();
            auto p = box->getParent()->convertToWorldSpace(box->getPosition());
            box->removeFromParent();
            box->setPosition(boxRootNode->convertToNodeSpace(p));
            boxRootNode->addChild(box);
            box->release();
            
            if (box->origin == box->getPosition() && box->isShowCover() == true)
            {
//                box->showCover(false, false);
//                addMangos(box, 10, false);
                return;
            }
            
            Box* targetBox = nullptr;
            for (const auto &b : boxes)
            {
                if (b->isSlot == false)
                    continue;
                
                if (b->getPosition().distance(box->getPosition()) < 400.f)
                {
                    targetBox = b;
                }
            }
            
            if (targetBox != nullptr)
            {
                animateMove(box, 0.05f, targetBox->getPosition(), nullptr);
                box->isBoxMovable = false;
                box->isActive = false;
                box->isOnTray = true;
                targetBox->isSlotIsFull = true;
                
                if (trayIsFull())
                {
                    moveTray();
                }
            }
            else
            {
                animateMove(box, 0.1f, box->origin, nullptr);
            }
        };
        
        box->onCloseButtonTouchBegan = [this, box]() {
            box->animateCloseButton(true);
        };
        
        box->onCloseButtonTouchEnded = [this, box]() {
            box->animateCloseButton(false);
            box->isActive = false;
            this->runAction(Sequence::create(DelayTime::create(0.5f), CallFunc::create([this, box](){
                if (_tutorialLayer1 != nullptr)
                {
                    _tutorialLayer1->stopAllActions();
                    _tutorialLayer1->removeFromParent();
                    _tutorialLayer1 = nullptr;
                    drawTutorialLayerStep2();
                }
                box->showCover(false, true);
                this->addMangos(box, 10, false);
            }), nullptr));
        };
    }
    
    void MangoShopScene::bindingMangoEvent(Mango *mango)
    {
        mango->onTouchBegan = [this, mango]() {
            GameSoundManager::getInstance()->playEffectSound("Common/Sounds/Effect/Mango_Pick.m4a");
            mango->retain();
            mango->removeFromParent();
            _gameNode->addChild(mango);
            mango->release();
        };
        
        mango->onTouchMoved = [this, mango](Vec2 delta) {
            if (_tutorialHand != nullptr)
            {
                _tutorialHand->stopAllActions();
                _tutorialHand->removeFromParent();
                _tutorialHand = nullptr;
            }
            
            if (_tutorialLayer2 != nullptr)
            {
                _tutorialLayer2->stopAllActions();
                _tutorialLayer2->removeFromParent();
                _tutorialLayer2 = nullptr;
            }
        };
        
        mango->onTouchEnded = [this, mango]() {
            Box *targetBox = nullptr;
            float justBeforeDistance = 9999.f;
            MangoSlot *currentSlot = nullptr;
            GameSoundManager::getInstance()->playEffectSound("Common/Sounds/Effect/Mango_Drop.m4a");
            
            auto convertToWorldSpaceRect = [](Node* node) {
                Rect rect = Rect(Point::ZERO, node->getContentSize());
                auto origin = node->convertToWorldSpace(rect.origin);
                auto diagonal = node->convertToWorldSpace(rect.origin + rect.size);
                return Rect(origin, Size(diagonal - origin));
            };
            
            for (const auto& b : boxes)
            {
                Rect rectBox = convertToWorldSpaceRect(b);
                Rect rectMango = convertToWorldSpaceRect(mango);
                
                if (rectBox.intersectsRect(rectMango))
                {
                    auto wmp = mango->getParent()->convertToWorldSpace(mango->getPosition());
                    auto wbp = b->getParent()->convertToWorldSpace(b->getPosition());
                    float d = wbp.distance(wmp);
                    
                    if (targetBox != nullptr)
                    {
                        if (d < justBeforeDistance)
                        {
                            targetBox = b;
                            justBeforeDistance = d;
                        }
                    }
                    else
                    {
                        targetBox = b;
                        justBeforeDistance = d;
                    }
                }
                
                for (const auto& s : b->getSlots())
                {
                    if (s->targetMango == mango)
                    {
                        currentSlot = s;
                    }
                }
            }
            
            if (targetBox != nullptr && targetBox->isShowCover() == false)
            {
                for (const auto& s : targetBox->getSlots())
                {
                    // 다시 넣어야 할 박스가 원래 꺼냈던 박스이면
                    if (s->targetMango == mango)
                    {
                        animateMove(mango, 0.1f, mango->origin, MANGO_MOVE_ENALBLE);
                        return;
                    }
                    // 넣어야 할 박스에 빈자리가 있으면
                    else if (s->targetMango == nullptr)
                    {
                        // 빈자리 슬릇의 위치에 망고를 넣는다.
                        auto wp = s->getParent()->convertToWorldSpace(s->getPosition());
                        animateMove(mango, 0.05f, _gameNode->convertToNodeSpace(wp), MANGO_MOVE_ENALBLE);
                        mango->origin = _gameNode->convertToNodeSpace(wp);
                        s->targetMango = mango;

                        // 원래 슬롯은 비움
                        currentSlot->targetMango = nullptr;
//                        auto currentBox = dynamic_cast<Box*>(currentSlot->getParent());
//                        if (currentBox->isEmpty() && currentBox->isOnTray)
//                        {
//                            currentBox->isTransparency(true);
//                        }
                        
                        // 트레이 위의 박스일 경우 (뚜껑을 덮는 상황은 없으므로 바로 종료)
                        if (targetBox->isOnTray == true)
                        {
                            if (trayIsFull())
                            {
                                moveTray();
                                return;
                            }
                        }
                        
                        // 망고를 넣고나서 꽉 찼을 경우, 뚜껑을 덮어야 함
                        if (targetBox->getSlots().size() == 10 && targetBox->isFull())
                        {
                            // 뚜껑을 덮기 전에, 슬롯을 모두 비움
                            for (const auto &s : targetBox->getSlots())
                            {
                                s->targetMango->removeFromParent();
                                for(auto iter = mangos.begin(); iter != mangos.end();)
                                {
                                    if((*iter) == s->targetMango)
                                        iter = mangos.erase(iter);
                                    else
                                        ++iter;
                                }
                                s->targetMango = nullptr; // 이 코드의 위치가 루프문 위에 있으면 안됨
                            }
                            
                            targetBox->retain();
                            targetBox->removeFromParent();
                            boxRootNode->addChild(targetBox);
                            targetBox->release();
                            
                            targetBox->showCover(true, true);
                        }
                        setPreventTouches(false);
                        return;
                    }
                }
            }
            else
            {
                animateMove(mango, 0.1f, mango->origin, MANGO_MOVE_ENALBLE);
                return;
            }
            
            animateMove(mango, 0.1f, mango->origin, MANGO_MOVE_ENALBLE);
            return;
        };
    }
    
    void MangoShopScene::bindingCardEvent(NumberCard *card)
    {
        card->onTouchBegan = [this, card]() {
            GameSoundManager::getInstance()->playEffectSound("NumberTrain/Sounds/Matrix_ClickBlock.m4a");
            _dummyNumberCard->setNumberString(card->getNumberString());
            _dummyNumberCard->setPosition(card->getPosition());
            _dummyNumberCard->setVisible(true);
            
            card->retain();
            card->removeFromParent();
            numberCardsRoot->addChild(card);
            card->release();
        };
        
        card->onTouchMoved = [this, card]() {
            float justBeforeDistance = 9999.f;
            targetBlankSlot = nullptr;
            candidateBlankSlot = nullptr;
            
            auto cp = numberCardsRoot->convertToWorldSpace(card->getPosition());
            for (const auto &bs : _blankSlots)
            {
                if (bs->isDone)
                    continue;
                auto bp = numberExpressionRoot->convertToWorldSpace(bs->getPosition());
                float d = bp.distance(cp);
                if (d < 180.f && d < justBeforeDistance)
                {
                    candidateBlankSlot = bs;
                }
            }
            
            if (candidateBlankSlot == nullptr)
            {
                resetBlankSlotEffect();
                return;
            }
            
            for (const auto &bs : _blankSlots)
            {
                bs->setSelectEffect(bs == candidateBlankSlot);
                if (card->getNumberString() == candidateBlankSlot->numberString)
                {
                    targetBlankSlot = candidateBlankSlot;
                }
            }
        };
        
        card->onTouchEnded = [this, card]() {
            resetBlankSlotEffect();
            float justBeforeDistance = 9999.f;
            BlankSlot* targetBlankSlot = nullptr;
            auto cp = numberCardsRoot->convertToWorldSpace(card->getPosition());
            for (const auto &bs : _blankSlots)
            {
                if (bs->isDone)
                    continue;
                
                auto bp = numberExpressionRoot->convertToWorldSpace(bs->getPosition());
                
                float d = bp.distance(cp);
                if (card->getNumberString() == bs->numberString && d < 180.f && d < justBeforeDistance)
                {
                    targetBlankSlot = bs;
                }
            }
            
            // 맞는 것이 있을 경우
            if (targetBlankSlot != nullptr)
            {
                targetBlankSlot->isDone = true;
                
                auto nc = NumberCard::create();
                nc->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
                nc->initWithNumberString(card->getNumberString(), 0.8f);
                
                //nc->setPosition(targetBlankSlot->getPosition());
                auto p = card->getParent()->convertToWorldSpace(card->getPosition());
                nc->setPosition(numberExpressionRoot->convertToNodeSpace(p));
                nc->runAction(Sequence::create(EaseBackOut::create(MoveTo::create(0.1f, targetBlankSlot->getPosition())), nullptr));
                
                nc->isActive = false;
                if (isSolved())
                {
                    for (int i = 0; i < _numberCards.size(); ++i)
                    {
                        _numberCards.at(i)->isActive = false;
                    }
                    
                    for (int i = 0; i < mangos.size(); ++i)
                    {
                        mangos.at(i)->isActive = false;
                    }
                    
                    for (int i = 0; i < boxes.size(); ++i)
                    {
                        boxes.at(i)->isActive = false;
                    }
                }
                
                numberExpressionRoot->addChild(nc);
                GameSoundManager::getInstance()->playEffectSound("NumberTrain/Sounds/Matrix_RightAnswer.m4a");
                writeGameLog(card, targetBlankSlot);
                
                card->setScale(0.5f);
                card->runAction(Sequence::create(MoveTo::create(0.01f, _dummyNumberCard->getPosition()),
                                                 EaseBackOut::create(ScaleTo::create(0.2f, 1.f)),
                                                 CallFunc::create([this](){
                    
                    NumberCard::_isOtherMovingCard = false;
                    _dummyNumberCard->setVisible(false);
                    
                    if (this->isSolved())
                    {
                        this->onSolve();
                        return;
                    }
                    
                }), nullptr));
            }
            // 맞는 것이 없을 경우
            else
            {
                if (card->getPosition().distance(_dummyNumberCard->getPosition()) > 100.f)
                {
                    GameSoundManager::getInstance()->playEffectSound("FishTank/Sounds/Matrix_WrongMove.m4a");
                    writeGameLog(card, candidateBlankSlot);
                }
                
                card->runAction(Sequence::create(EaseExponentialOut::create(MoveTo::create(0.18f, _dummyNumberCard->getPosition())), CallFunc::create([this](){
                    NumberCard::_isOtherMovingCard = false;
                    _dummyNumberCard->setVisible(false);
                }), nullptr));
            }
        };
    }
    
    void MangoShopScene::moveTray()
    {
        this->runAction(Sequence::create(DelayTime::create(0.4f), CallFunc::create([](){
            GameSoundManager::getInstance()->playEffectSound("Common/Sounds/Effect/Result_RightAnswer.m4a");
        }), nullptr));
        
        
        auto outAction = [&](Node& n) {
            n.runAction(Sequence::create(DelayTime::create(0.4f), EaseCubicActionInOut::create(MoveBy::create(0.5f, Vec2(1500.f, 0.f))), nullptr));
        };
        
        outAction(*_tray);
        
        for (const auto &b : boxes)
        {
            if (b->isOnTray == false)
                continue;
            
            outAction(*b);
        
            for (const auto &s : b->getSlots())
            {
                if (s->targetMango != nullptr)
                {
                    outAction(*(s->targetMango));
                }
            }
        }
    }
    
    bool MangoShopScene::trayIsFull()
    {
        for (const auto &b : boxes)
        {
            if (b->isSlot == true && b->isSlotIsFull == false)
                return false;
            if (b->isOnTray && b->isShowCover() == false)
            {
                for (const auto &s : b->getSlots())
                {
                    if (s->targetMango == nullptr)
                        return false;
                }
            }
        }
        return true;
    }
    
    void MangoShopScene::resetBlankSlotEffect()
    {
        for (const auto &bs : _blankSlots)
        {
            bs->setSelectEffect(false);
        }
    }
    
    bool MangoShopScene::isTrayFull()
    {
        for (const auto &b : boxes)
        {
            if (b->isShowCover() == true)
                continue;
            
            if (b->isOnTray == false)
                continue;
            
            for (const auto &s : b->getSlots())
            {
                if (s->targetMango == nullptr)
                    return false;
            }
        }
        return true;
    }
    
    bool MangoShopScene::isSolved()
    {
        for (const auto &bs : _blankSlots)
        {
            if (bs->isDone == false)
                return false;
        }
        return true;
    }
    
    void MangoShopScene::onSolve()
    {
        auto f1 = [this](){
            GameSoundManager::getInstance()->playEffectSound("Common/Sounds/Effect/UI_Star_Collected.m4a");
        };
        
        auto f2_1 = [this](){
            auto CP = CompletePopup::create();
            CP->show(0.5f, [this] {
                CCAppController::sharedAppController()->handleGameComplete(1);
            });
            return;
        };
        
        auto f2_2 = [this](){
            _currentProblemIndex++;
            onStart();
        };
        
        // 문제를 모두 풀었을 경우
        if (_currentProblemIndex + 1 == _problems.size())
        {
            this->runAction(Sequence::create(DelayTime::create(0.5f), CallFunc::create(f1), DelayTime::create(1.3f), CallFunc::create(f2_1), nullptr));
        }
        else
        {
            this->runAction(Sequence::create(DelayTime::create(0.5f), CallFunc::create(f1), DelayTime::create(1.3f), CallFunc::create(f2_2), nullptr));
        }
    }
    
    void MangoShopScene::animateMove(Node *target, float duration, Vec2 destination, function<void()> onComplete)
    {
        setPreventTouches(true);
        target->runAction(Sequence::create(EaseExponentialOut::create(MoveTo::create(duration, destination)), CallFunc::create([this]() {
            setPreventTouches(false);
        }), CallFunc::create(onComplete), nullptr));
    }
    
    void MangoShopScene::writeGameLog(NumberCard* card, BlankSlot* candidateSlot)
    {
        auto workPath = [this] {
            stringstream ss;
            ss << "/" << "MangoShop";
            ss << "/" << "level-" << _currentLevel << "-" << _currentSheetNo;
            ss << "/" << "work-" << _currentProblemIndex;
            return ss.str();
        }();
        
        auto userAnswer = [this, card, candidateSlot] {
            stringstream ss;
            for (BlankSlot* slot : _blankSlots) {
                if (candidateSlot != nullptr && candidateSlot == slot) {
                    ss << card->getNumberString();
                    continue;
                }
                if (!slot || !slot->isDone) {
                    ss << "_";
                    continue;
                }
                ss << slot->numberString;
            }
            return ss.str();
        }();
        
        auto correctAnswer = [this] {
            stringstream ss;
            for (BlankSlot* slot : _blankSlots) {
                ss << slot->numberString;
            }
            return ss.str();
        }();
        
        StrictLogManager::shared()->game_Peek_Answer("MangoShop", workPath, userAnswer, correctAnswer);
    }
    
    void MangoShopScene::createShiningParticle()
    {
        _shiningParticleNode = Node::create();
        ParticleSystemQuad* particleEffect = nullptr;
        auto createParticleEffect = [&](std::string name, std::string plist) {
            particleEffect = ParticleSystemQuad::create(plist);
            particleEffect->setName(name);
            particleEffect->setScale(4.f);
            particleEffect->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
            particleEffect->setPosition(Vec2::ZERO);
            particleEffect->stopSystem();
            _shiningParticleNode->addChild(particleEffect);
        };
        createParticleEffect("mangoshopParticle", "common/effects/particle/star_particle.plist");
        this->addChild(_shiningParticleNode);
    }
    
    void MangoShopScene::startParticle(Vec2 worldPosition)
    {
        _shiningParticleNode->setPosition(this->convertToNodeSpace(worldPosition));
        _shiningParticleNode->getChildByName<ParticleSystemQuad*>("mangoshopParticle")->resetSystem();
    }
    
    void MangoShopScene::stopParticle()
    {
        _shiningParticleNode->getChildByName<ParticleSystemQuad*>("mangoshopParticle")->stopSystem();
    }
    
} // end namespace
