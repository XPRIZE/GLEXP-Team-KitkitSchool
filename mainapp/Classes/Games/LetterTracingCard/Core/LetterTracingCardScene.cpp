//
//  LetterTracingCardScene.cpp -- A game scene for LetterTracingCard
//  TodoSchool on May 25, 2016
//
//  Copyright (c) 2016 Enuma, Inc. All rights reserved.
//  See LICENSE.md for more details.
//

#include "LetterTracingCardScene.h"
#include "../Utils/MainDepot.h"

#include <Managers/LanguageManager.hpp>
#include "CCAppController.hpp"

BEGIN_NS_LETTERTRACINGCARD

using namespace todoschool::tracefield;
using namespace cocos2d;
using namespace std;


namespace
{

}

LetterTracingCardScene::LetterTracingCardScene()
:TheGameNode(nullptr)
,_currentProblemIndex(0)
{
    
}

bool LetterTracingCardScene::init()
{
    if (!Super::init()) { return false; }
    initData();
    return true;
}

void LetterTracingCardScene::initData()
{
    _levelDataEn = {
        { { make_tuple("a", "anga.png", "animals"), make_tuple("a", "5.4_foxjump.png", "ax"), make_tuple("a", "5.4_foxjump.png", "apple"), } },
        { { make_tuple("b", "5.4_foxjump.png", "bear"), make_tuple("b", "5.4_foxjump.png", "bat"), make_tuple("b", "5.4_foxjump.png", "banjo"), } },
        { { make_tuple("c", "5.4_foxjump.png", "coconut"), make_tuple("c", "5.4_foxjump.png", "camel"), make_tuple("c", "5.4_foxjump.png", "cow"), } },
        { { make_tuple("d", "5.4_foxjump.png", "donkey"), make_tuple("d", "5.4_foxjump.png", "drum"), make_tuple("d", "5.4_foxjump.png", "duck"), } },
        { { make_tuple("e", "5.4_foxjump.png", "egg"), make_tuple("e", "5.4_foxjump.png", "ear"), make_tuple("e", "5.4_foxjump.png", "elephant"), } },
        { { make_tuple("f", "5.4_foxjump.png", "flower"), make_tuple("f", "5.4_foxjump.png", "frog"), make_tuple("f", "5.4_foxjump.png", "fish"), } },
        { { make_tuple("g", "5.4_foxjump.png", "garlic"), make_tuple("g", "5.4_foxjump.png", "grape"), make_tuple("g", "5.4_foxjump.png", "gift"), } },
        { { make_tuple("h", "5.4_foxjump.png", "heart"), make_tuple("h", "5.4_foxjump.png", "hyena"), make_tuple("h", "5.4_foxjump.png", "hat"), } },
        { { make_tuple("i", "5.4_foxjump.png", "impala"), make_tuple("i", "5.4_foxjump.png", "iguana"), make_tuple("i", "5.4_foxjump.png", "iron"), } },
        { { make_tuple("j", "5.4_foxjump.png", "jellyfish"), make_tuple("j", "5.4_foxjump.png", "jar"), make_tuple("j", "5.4_foxjump.png", "jersey"), } },
        { { make_tuple("k", "5.4_foxjump.png", "kettle"), make_tuple("k", "5.4_foxjump.png", "key"), make_tuple("k", "5.4_foxjump.png", "kitten"), } },
        { { make_tuple("l", "5.4_foxjump.png", "leopard"), make_tuple("l", "5.4_foxjump.png", "leaf"), make_tuple("l", "5.4_foxjump.png", "lemon"), } },
        { { make_tuple("m", "5.4_foxjump.png", "mango"), make_tuple("m", "5.4_foxjump.png", "milk"), make_tuple("m", "5.4_foxjump.png", "market"), } },
        { { make_tuple("n", "5.4_foxjump.png", "neck"), make_tuple("n", "5.4_foxjump.png", "net"), make_tuple("n", "5.4_foxjump.png", "nest"), } },
        { { make_tuple("o", "5.4_foxjump.png", "office"), make_tuple("o", "5.4_foxjump.png", "octopus"), make_tuple("o", "5.4_foxjump.png", "orange"), } },
        { { make_tuple("p", "5.4_foxjump.png", "purple"), make_tuple("p", "5.4_foxjump.png", "pineapple"), make_tuple("p", "5.4_foxjump.png", "pumpkin"), } },
        { { make_tuple("q", "5.4_foxjump.png", "quail"), make_tuple("q", "5.4_foxjump.png", "quilt"), make_tuple("q", "5.4_foxjump.png", "quiet"), } },
        { { make_tuple("r", "5.4_foxjump.png", "rooster"), make_tuple("r", "5.4_foxjump.png", "rabbit"), make_tuple("r", "5.4_foxjump.png", "radio"), } },
        { { make_tuple("s", "5.4_foxjump.png", "sky"), make_tuple("s", "5.4_foxjump.png", "soap"), make_tuple("s", "5.4_foxjump.png", "snake"), } },
        { { make_tuple("t", "5.4_foxjump.png", "turtle"), make_tuple("t", "5.4_foxjump.png", "truck"), make_tuple("t", "5.4_foxjump.png", "toilet"), } },
        { { make_tuple("u", "5.4_foxjump.png", "umbrella"), make_tuple("u", "5.4_foxjump.png", "unicorn"), make_tuple("u", "5.4_foxjump.png", "up"), } },
        { { make_tuple("v", "5.4_foxjump.png", "vulture"), make_tuple("v", "5.4_foxjump.png", "vase"), make_tuple("v", "5.4_foxjump.png", "vest"), } },
        { { make_tuple("w", "5.4_foxjump.png", "worm"), make_tuple("w", "5.4_foxjump.png", "water"), make_tuple("w", "5.4_foxjump.png", "wheel"), } },
        { { make_tuple("x", "5.4_foxjump.png", "x-ray"), make_tuple("x", "5.4_foxjump.png", "box"), make_tuple("x", "5.4_foxjump.png", "fox"), } },
        { { make_tuple("y", "5.4_foxjump.png", "yam"), make_tuple("y", "5.4_foxjump.png", "yellow"), make_tuple("y", "5.4_foxjump.png", "yolk"), } },
        { { make_tuple("z", "5.4_foxjump.png", "zebra"), make_tuple("z", "5.4_foxjump.png", "zipper"), make_tuple("z", "5.4_foxjump.png", "zigzag"), } },
    };
    
    _levelDataSw = {
        { { make_tuple("a", "anga.png", "anga"), make_tuple("a", "5.4_foxjump.png", "adabu"), make_tuple("a", "5.4_foxjump.png", "ardhi"), } },
        { { make_tuple("b", "5.4_foxjump.png", "baba"), make_tuple("b", "5.4_foxjump.png", "boga"), make_tuple("b", "5.4_foxjump.png", "bega"), } },
        { { make_tuple("ch", "5.4_foxjump.png", "chura"), make_tuple("ch", "5.4_foxjump.png", "chakula"), } }, //make_tuple("ch", "5.4_foxjump.png", "chungwa"), } },
        { { make_tuple("d", "5.4_foxjump.png", "dubu"), make_tuple("d", "5.4_foxjump.png", "dafu"), make_tuple("d", "5.4_foxjump.png", "debe"), } },
        { { make_tuple("e", "5.4_foxjump.png", "embe"), make_tuple("e", "5.4_foxjump.png", "elfu"), make_tuple("e", "5.4_foxjump.png", "elimu"), } },
        { { make_tuple("f", "5.4_foxjump.png", "fisi"), make_tuple("f", "5.4_foxjump.png", "fua"), make_tuple("f", "5.4_foxjump.png", "farasi"), } },
        { { make_tuple("g", "5.4_foxjump.png", "gogo"), make_tuple("g", "5.4_foxjump.png", "gurudumu"), make_tuple("g", "5.4_foxjump.png", "gauni"), } },
        { { make_tuple("h", "5.4_foxjump.png", "homa"), make_tuple("h", "5.4_foxjump.png", "hesabu"), make_tuple("h", "5.4_foxjump.png", "hadithi"), } },
        { { make_tuple("i", "5.4_foxjump.png", "injini"), make_tuple("i", "5.4_foxjump.png", "imba"), make_tuple("i", "5.4_foxjump.png", "idadi"), } },
        { { make_tuple("j", "5.4_foxjump.png", "jino"), make_tuple("j", "5.4_foxjump.png", "jani"), make_tuple("j", "5.4_foxjump.png", "jogoo"), } },
        { { make_tuple("k", "5.4_foxjump.png", "kofia"), make_tuple("k", "5.4_foxjump.png", "kiti"), make_tuple("k", "5.4_foxjump.png", "kuni"), } },
        { { make_tuple("l", "5.4_foxjump.png", "leso"), make_tuple("l", "5.4_foxjump.png", "lori"), make_tuple("l", "5.4_foxjump.png", "lala"), } },
        { { make_tuple("m", "5.4_foxjump.png", "maziwa"), make_tuple("m", "5.4_foxjump.png", "mpira"), make_tuple("m", "5.4_foxjump.png", "maji"), } },
        { { make_tuple("n", "5.4_foxjump.png", "neno"), make_tuple("n", "5.4_foxjump.png", "ndizi"), make_tuple("n", "5.4_foxjump.png", "nanasi"), } },
        { { make_tuple("o", "5.4_foxjump.png", "oa"), make_tuple("o", "5.4_foxjump.png", "ogelea"), make_tuple("o", "5.4_foxjump.png", "ofisi"), } },
        { { make_tuple("p", "5.4_foxjump.png", "popo"), make_tuple("p", "5.4_foxjump.png", "pira"), make_tuple("p", "5.4_foxjump.png", "papai"), } },
        { { make_tuple("r", "5.4_foxjump.png", "rafiki"), make_tuple("r", "5.4_foxjump.png", "redio"), make_tuple("r", "5.4_foxjump.png", "rangi"), } },
        { { make_tuple("s", "5.4_foxjump.png", "samaki"), make_tuple("s", "5.4_foxjump.png", "sabuni"), make_tuple("s", "5.4_foxjump.png", "sikio"), } },
        { { make_tuple("t", "5.4_foxjump.png", "takataka"), make_tuple("t", "5.4_foxjump.png", "tausi"), make_tuple("t", "5.4_foxjump.png", "tembo"), } },
        { { make_tuple("u", "5.4_foxjump.png", "ua"), make_tuple("u", "5.4_foxjump.png", "uji"), make_tuple("u", "5.4_foxjump.png", "ubao"), } },
        { { make_tuple("v", "5.4_foxjump.png", "viti"), make_tuple("v", "5.4_foxjump.png", "viazi"), make_tuple("v", "5.4_foxjump.png", "viatu"), } },
        { { make_tuple("w", "5.4_foxjump.png", "wanyama"), make_tuple("w", "5.4_foxjump.png", "wino"), make_tuple("w", "5.4_foxjump.png", "watoto"), } },
        { { make_tuple("y", "5.4_foxjump.png", "yaya"), make_tuple("y", "5.4_foxjump.png", "yai"), make_tuple("y", "5.4_foxjump.png", "yavuyavu"), } },
        { { make_tuple("z", "5.4_foxjump.png", "zeze"), make_tuple("z", "5.4_foxjump.png", "zabibu"), make_tuple("z", "5.4_foxjump.png", "zambarau"), } },
        { { make_tuple("sh", "5.4_foxjump.png", "shoka"), make_tuple("sh", "5.4_foxjump.png", "shamba"), } }, //make_tuple("sh", "5.4_foxjump.png", "shingo"), } },
        { { make_tuple("ng'", "5.4_foxjump.png", "ng'ombe"), make_tuple("ng'", "5.4_foxjump.png", "ng'amba"), } }, //make_tuple("ng'", "5.4_foxjump.png", "ng'oa"), } },
        { { make_tuple("ng", "5.4_foxjump.png", "ngamia"), make_tuple("ng", "5.4_foxjump.png", "ngoma"), } }, //make_tuple("ng", "5.4_foxjump.png", "ngozi"), } },
        //{ { make_tuple("ny", "5.4_foxjump.png", "nyama"), make_tuple("ny", "5.4_foxjump.png", "nyumba"), } }, //make_tuple("ny", "5.4_foxjump.png", "nyota"), } },
        { { make_tuple("th", "5.4_foxjump.png", "thumu"), make_tuple("th", "5.4_foxjump.png", "thelathini"), } }, //make_tuple("th", "5.4_foxjump.png", "theluji"), } },
        { { make_tuple("gh", "5.4_foxjump.png", "ghala"), make_tuple("gh", "5.4_foxjump.png", "gharika"), } }, //make_tuple("gh", "5.4_foxjump.png", "gharama"), } },
        { { make_tuple("dh", "5.4_foxjump.png", "dhau"), make_tuple("dh", "5.4_foxjump.png", "dhifa"), } }, // make_tuple("dh", "5.4_foxjump.png", "dhahabu"), } },
    };
    
    
}

std::vector<std::vector<std::vector<std::tuple<string, string, string>>>> LetterTracingCardScene::getLevelData()
{
    if (LanguageManager::getInstance()->isEnglish())
    {
        return _levelDataEn;
    }
    else
    {
        return _levelDataSw;
    }
}

void LetterTracingCardScene::initUI()
{
    MainDepot Depot;

    removeAllChildren();
    
    Background = ([&] {
        Sprite* It = Depot.createBackgroundSprite();
        addChild(It);
        return It;
    }());
    
    TheGameNode = ([&] {
        Node* It = Depot.createGameNode();
        addChild(It);
        return It;
    }());
    
//    TheProgressBar = ([&] {
//        ProgressIndicator* It = ProgressIndicator::create();
//        It->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
//        It->setPosition(Point(Depot.windowSize().width / 2.f, Depot.windowSize().height - It->getContentSize().height));
//        It->setMax(getLevelData()[_currentLevel - 1].size());
//        addChild(It);
//        return It;
//    }());
    
    [&] {
        auto It = TodoSchoolBackButton::create();
        It->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
        It->setPosition(Point(25.f, Depot.windowSize().height - 25.f));
        It->addClickEventListener([this](Ref*) {
            handleFail();
        });
        addChild(It);
        return It;
    }();
}

void LetterTracingCardScene::initProblem()
{
    _cardVector.clear();
    TheGameNode->removeAllChildren();
//    TheProgressBar->setCurrent(_currentProblemIndex + 1, false);
    
    auto card = Card::create();

    // (s)temp
    _currentProblem = new Problem();
    _currentProblem->characters = std::get<0>(getLevelData()[_currentLevel - 1][_currentProblemIndex][0]);
    _currentProblem->imageName = std::get<1>(getLevelData()[_currentLevel- 1][_currentProblemIndex][0]);
    _currentProblem->word = std::get<2>(getLevelData()[_currentLevel- 1][_currentProblemIndex][0]);
    card->drawCardWithProblem(_currentProblem);
    
    // (e)temp
    
    int currentProblemSize = getLevelData()[_currentLevel - 1][_currentProblemIndex].size();
    Size cardSize = card->getContentSize();
    auto rootNode = Node::create();
    rootNode->setContentSize(Size(cardSize.width * currentProblemSize, cardSize.height));
    for (int i = 0; i < currentProblemSize; i++)
    {
        _currentProblem = new Problem();
        _currentProblem->characters = std::get<0>(getLevelData()[_currentLevel - 1][_currentProblemIndex][i]);
        _currentProblem->imageName = std::get<1>(getLevelData()[_currentLevel - 1][_currentProblemIndex][i]);
        _currentProblem->word = std::get<2>(getLevelData()[_currentLevel - 1][_currentProblemIndex][i]);
        
        auto card = Card::create();
        card->drawCardWithProblem(_currentProblem);
        _cardVector.push_back(card);
        card->onComplete = [this]() {
            for (auto c : _cardVector)
            {
                c->traceWord->Enabled.update(true);
            }
            
            for (auto c : _cardVector)
            {
                if (c->isComplete == false)
                    return;
            }
            handleSuccess();
        };
        card->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        card->setPosition(cardSize.width / 2 + cardSize.width * i, cardSize.height / 2);
        rootNode->addChild(card);
        
        card->traceWord->OnBeginEditing = [this, card](TraceField*) {
            for (auto c : _cardVector)
            {
                if (c != card)
                {
                    c->traceWord->Enabled.update(false);
                }
            }
        };
    }
    rootNode->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    rootNode->setPosition(TheGameNode->getContentSize() / 2.f);
    TheGameNode->addChild(rootNode);
}

void LetterTracingCardScene::handleSuccess()
{
//    TheProgressBar->setCurrent(_currentProblemIndex + 1, true);
    
    if (_currentProblemIndex + 1 < getLevelData()[_currentLevel - 1].size())
    {
        _currentProblemIndex++;
        this->runAction(Sequence::create(DelayTime::create(0.5f), CallFunc::create([this]() {
            initProblem();
        }), nullptr));
        
    }
    else
    {
        auto CP = CompletePopup::create();
        CP->show(1.f, [this] {
            auto Guard = NodeScopeGuard(this);
            
            if (OnSuccess)
                OnSuccess();
            
            CCAppController::sharedAppController()->handleGameComplete(1);
        });
    }
}


void LetterTracingCardScene::handleFail()
{
    auto Guard = NodeScopeGuard(this);
    if (OnFail)
        OnFail();
}

void LetterTracingCardScene::onEnter()
{
    Super::onEnter();
}

void LetterTracingCardScene::onStart()
{
    initData();
    initUI();
    initProblem();
}

void LetterTracingCardScene::onExitTransitionDidStart()
{
    Super::onExitTransitionDidStart();
}

void LetterTracingCardScene::setCurrentLevel(int level)
{
    _currentLevel = level;
}

END_NS_LETTERTRACINGCARD
