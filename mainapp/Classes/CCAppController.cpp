//
//  CCAppController.cpp
//  KitkitSchool
//
//  Created by Gunho Lee on 11/1/16.
//
//

#include "CCAppController.hpp"
#include "Common/Controls/TodoLoadingScene.hpp"
#include "Common/Controls/TodoSchoolBackButton.hpp"
#include "Common/Controls/TouchEventLogger.h"
#include "Utils/TodoUtil.h"

#include "Common/Basic/KitkitGameInfo.hpp"
#include "Games/SentenceBridge/SentenceBridgeInfo.hpp"
#include "Games/AnimalPuzzle/AnimalPuzzleInfo.hpp"
#include "Games/MultiplicationBoard/MultiplicationBoardInfo.hpp"
#include "Games/WordWindow/WordWindowInfo.h"
#include "Games/WordMatrix/WordMatrixInfo.h"
#include "Games/QuickFacts/QuickFactsInfo.hpp"
#include "Games/WordKicker/WordKickerInfo.hpp"
#include "Games/PlaceValue/PlaceValueInfo.hpp"
#include "Games/Labeling/LabelingInfo.hpp"

#include "Games/NumberTrace/NumberTrace.h"
#include "Games/Counting/CountingScene.hpp"
#include "Games/DoubleDigit/DoubleDigitScene.h"
#include "Games/DoubleDigit/DoubleDigit_ProblemBank.h"
#include "Games/EquationMaker/EquationMakerScene.h"
#include "Games/EquationMaker/EquationMakerProblemBank.h"

#include "Games/NumberTraceExt/NumberTraceExt.h"
#include "Games/WoodenPuzzles/NumberPuzzle.h"
#include "Games/FindTheMatch/FindTheMatch.h"
#include "Games/NumberMatching/NumberMatchingScene.hpp"

#include "Games/MovingInsects/MovingInsects.h"
#include "Games/FishTank/FishTankScene.h"
#include "Games/FishTank/FishTankProblemBank.h"

#include "Games/Books/BookView.hpp"
#include "Games/BookWithQuiz/BookWithQuiz.hpp" // book-with-comprehension

#include "Games/WoodenPuzzles/AlphabetPuzzle.h"
#include "Games/WordMachine/WordMachineScene.hpp"
#include "Games/LetterTrace/LetterTrace.h"
#include "Games/LetterTracingCard/LetterTracingCard.h"
#include "Games/LetterTracingCard/Core/LetterTracingCardScene.h"
#include "Games/WordTrace/WordTrace.h"
#include "Games/StarFall/StarFall.h"
#include "Games/Spelling/Spelling.h"
#include "Games/LetterMatching/LetterMatchingScene.hpp"
#include "Games/Tapping/TappingScene.hpp"
#include "Games/TutorialTrace/TutorialTrace.h"
#include "Games/HundredPuzzle/HundredPuzzleScene.hpp"
#include "Games/ComprehensionTest/Matching/MatchingScene.hpp"
#include "Games/ComprehensionTest/ComprehensionScene.hpp"
#include "Games/SentenceMaker/SentenceMakerScene.hpp"
#include "Games/BirdPhonics/BirdPhonicsScene.hpp"
#include "Games/SoundTrain/SoundTrainScene.hpp"
#include "Games/PatternTrain/PatternTrainScene.hpp"
#include "Games/NumberTrain/NumberTrainScene.hpp"
#include "Games/ShapeMatching/ShapeMatchingScene.hpp"
#include "Games/ThirtyPuzzle/ThirtyPuzzleScene.hpp"
#include "Games/MangoShop/MangoShopScene.hpp"
#include "Games/WordNote/WordNoteScene.hpp"
#include "Games/WhatIsThis/WhatIsThisScene.h"
#include "Games/FeddingTime/FeedingTimeScene.h"
#include "Games/LineMatching/LineMatchingScene.h"
#include "Games/LineMatching/LineMatchingProblemBank.h"
#include "Games/LRComprehension/LRComprehensionScene.hpp"

#include "Games/Video/GameVideoScene.hpp"


#if (CC_TARGET_PLATFORM != CC_PLATFORM_WINRT)
#include "Games/ThirtyPuzzle/ThirtyPuzzleScene.hpp"
#include "Games/EggQuiz/EggQuizScene.hpp"
#include "Games/WordNote/WordNoteScene.hpp"
#include "Games/DigitalQuiz/DigitalQuiz.h"
#include "Games/ReadingBird/ReadingBirdScene.hpp"
#include "Games/MissingNumber/MissingNumberScene.hpp"
#endif //!WINRT

#include "Menu/CoopScene.hpp"
#include "Menu/GradeSelector.hpp"
#include "Menu/BookChoiceScene.hpp"
#include "Menu/Bird.hpp"
#include "Menu/TutorialVideoScene.hpp"


#include "Managers/LanguageManager.hpp"
#include "Managers/BookManager.hpp"
#include "Managers/LogManager.hpp"
#include "Managers/UserManager.hpp"
#include "Managers/GameSoundManager.h"
#include "Managers/CurriculumManager.hpp"
#include "Managers/StrictLogManager.h"

#include "CustomDirector.h"
#include "3rdParty/CCNativeAlert.h"

#include "platform/android/jni/JniHelper.h"

using namespace std;
USING_NS_CC;


namespace
{
    string kBookFolder = "book2/";
}

static CCAppController *instance;

CCAppController *CCAppController::sharedAppController() {
    if (instance == nullptr) {
        instance = new CCAppController;
    }
    return instance;
}

CCAppController::CCAppController() {
    _playTimer = new PlayTimer();
    _playTimer->start();
    
    
    CurriculumManager::getInstance();
    _currentCurrLevelID = "";
    _isFreeChoice = false;
    _allowSkipTutorial = true;
    
    KitkitGameInfo::registerGame(new SentenceBridgeInfo);
    KitkitGameInfo::registerGame(new AnimalPuzzleInfo);
    KitkitGameInfo::registerGame(new MultiplicationBoardInfo);
    KitkitGameInfo::registerGame(new WordWindowInfo);
    KitkitGameInfo::registerGame(new WordMatrixInfo);
    KitkitGameInfo::registerGame(new QuickFactsInfo);
    KitkitGameInfo::registerGame(new WordKickerInfo);
    KitkitGameInfo::registerGame(new MathKickerInfo);
    KitkitGameInfo::registerGame(new PlaceValueInfo);
    KitkitGameInfo::registerGame(new LabelingInfo);
}

CCAppController::~CCAppController() {

    CC_SAFE_DELETE(_playTimer);
    
}

// JNI function to set the parameter "current_screen" in the logged events
void firebase_setCurrentScreen(string screenName, string screenClass) {
    JniMethodInfo t;
    bool getInfo = JniHelper::getMethodInfo(t, "org/cocos2dx/cpp/AppActivity", "firebase_setCurrentScreen", "(Ljava/lang/String;Ljava/lang/String;)V");
    if (getInfo)
    {
        jobject activity = JniHelper::getActivity();
        jstring jScreenName = t.env->NewStringUTF(screenName.c_str());
        jstring jScreenClass = t.env->NewStringUTF(screenClass.c_str());
        t.env->CallVoidMethod(activity, t.methodID, jScreenName, jScreenClass);
        t.env->DeleteLocalRef(jScreenName);
        t.env->DeleteLocalRef(jScreenClass);
    }
}

bool CCAppController::gameExists(std::string gameName)
{
    return this->startGame(gameName, 0, "", true);
    
}

void CCAppController::startGameWithLevelChoice(std::string gameName)
{
    _currentGame = "";
    _currentParam = "";
    _currentLevel = 0;
    
    if(gameName == "Book"){
        Director::getInstance()->pushScene(TransitionFade::create(0.5, TodoLoadingScene::createScene([](){
            return BookChoiceScene::createScene();
        })));
        return;

    } else if (gameName == "BookWithQuiz") {
        Director::getInstance()->pushScene(TransitionFade::create(0.5, TodoLoadingScene::createScene([](){
            return BookChoiceScene::createSceneForBookWithQuiz();
        })));
        return;
        
    } else if (gameName == "DigitalQuiz") {
        startGame(gameName, 0);
        return;
    }
    
    
    auto it = GradeSelector::create();
    it->retain();
    it->setGameName(gameName);
    
    
    auto info = KitkitGameInfo::findGameInfo(gameName);
    if (info) {
        auto ddLevels = info->enumerateLevelIDs();
        it->setChoices(ddLevels);
        
        
        Director::getInstance()->pushScene(TransitionFade::create(0.5, TodoLoadingScene::createScene([it](){
            auto ret = it->minimalSceneByWrapping();
            it->release();
            return ret;
        })));
        return;
    }
    
	if (gameName == "NumberTracing") {
		it->setChoices(NumberTrace().getCandidateLevelIDs());

	} else if (gameName == "Counting") {
		using todoschool::counting::LevelData;
		auto lang = LanguageManager::getInstance()->getCurrentLanguageTag();
		it->setChoices(LevelData::defaultData().levelIDsFor(lang));

	} else if (gameName == "DoubleDigit") {
        DoubleDigit_ProblemBank bank;
        it->setChoices(bank.getCandidateLevelIDs());

	} else if (gameName == "EquationMaker") {
#ifdef IMPORT_TSV_FILE_EQUATION_MAKER
        it->setChoices(EquationMakerProblemBank::getLevelIDs());
#else
        vector<int> ddLevels = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };
        it->setChoices(ddLevels);
#endif

	}else if(gameName == "FishTank"){
        vector<int> fishLevels = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11 };
        it->setChoices(fishLevels);

    } else if(gameName=="Tapping"){
        vector<int> ddLevels = { 1, 2, 3};
        it->setChoices(ddLevels);
        
    } else if(gameName == "TutorialTrace"){
        auto levels = TutorialTrace().getCandidateLevelIDs();
        it->setChoices(levels);
        
    } else if(gameName=="HundredPuzzle"){
        vector<int> ddLevels = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13};
        it->setChoices(ddLevels);
        
    } else if (gameName=="SoundTrain") {
        auto soundTrainProblemBank = new todoschool::soundtrain::SoundTrainProblemBank();
        it->setChoices(soundTrainProblemBank->getLevels());
    } else if (gameName=="PatternTrain") {
        vector<int> ddLevels = { 1, 2, 3, 4, 5};
        it->setChoices(ddLevels);
        
    } else if (gameName=="ShapeMatching") {
        vector<int> ddLevels = { 1, 2, 3, 4, 5, 6, 7 ,8, 9, 10, 11, 12};
        it->setChoices(ddLevels);
        
    } else if (gameName=="NumberTrain") {
        vector<int> ddLevels = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12,13,14,15,16,17,18};
        it->setChoices(ddLevels);
        
    } else if(gameName == "NumberTracingExt"){
        vector<int> ddLevels = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14 };
        it->setChoices(ddLevels);
        
    }else if(gameName == "NumberPuzzle"){
        it->setChoices(NumberPuzzle().getCandidateLevelIDs());
        
    }else if(gameName == "FindTheMatch"){
        it->setChoices(FindTheMatch().getCandidateLevelIDs());
        
    }else if(gameName == "NumberMatching"){
        auto lang = LanguageManager::getInstance()->getCurrentLanguageTag();
        auto data = todoschool::numbermatching::LevelData::defaultData();
        it->setChoices(data.levelIDsFor(lang));

    }else if(gameName == "MovingInsects"){
        it->setChoices(MovingInsects().getCandidateLevelIDs());

    }else if(gameName == "AlphabetPuzzle"){
        it->setChoices(AlphabetPuzzle().getCandidateLevelIDs());

    }else if(gameName == "WordMachine"){
        using todoschool::wordmachine::LevelData;
        auto lang = LanguageManager::getInstance()->getCurrentLanguageTag();
        it->setChoices(LevelData::defaultData().levelIDsFor(lang));

    }else if(gameName == "LetterTrace"){
        it->setChoices(LetterTrace().getCandidateLevelIDs());

    }else if(gameName == "LetterTracingCard"){
#ifdef IMPORT_TSV_FILE_LETTER_TRACING_CARD
        it->setChoices(todoschool::lettertracingcard::LetterTracingCardScene::getLevelIDs());
#else
        vector<int> ddLevels;
        if (LanguageManager::getInstance()->isEnglish())
        {
            ddLevels = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26 };
        }
        else
        {
            ddLevels = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32 };
        }
        it->setChoices(ddLevels);
#endif
        
    }else if(gameName == "WordTracing"){
        it->setChoices(WordTrace().getCandidateLevelIDs());

    }else if(gameName == "Comprehension"){
        vector<int> ddLevels = { 1 };
        it->setChoices(ddLevels);
        
    }else if(gameName == "StarFall"){
        it->setChoices(StarFall().getCandidateLevelIDs());

    }else if(gameName == "Spelling"){
        it->setChoices(Spelling().getCandidateLevelIDs());

    }else if(gameName == "LetterMatching"){
        auto lang = LanguageManager::getInstance()->getCurrentLanguageTag();
        auto data = todoschool::lettermatching::LevelData::defaultData();
        it->setChoices(data.levelIDsFor(lang));
        
    }else if(gameName == "CompMatching"){
        vector<int> ddLevels = { 1 };
        it->setChoices(ddLevels);
        
    }else if(gameName == "SentenceMaker"){
        auto sentenceMakerScene = SentenceMakerScene::create();
        sentenceMakerScene->setLevel(0);
        it->setChoices(sentenceMakerScene->getCandidateLevelIDs());
        
    }else if(gameName == "BirdPhonics"){
        it->setChoices(BirdPhonicsScene::getLevelIDs());
        
    }else if(gameName == "MangoShop"){
        vector<int> ddLevels = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11 };
        it->setChoices(ddLevels);
        
    }else if(gameName == "WhatIsThis"){
        auto whatIsThisLayer = WhatIsThisLayer::create();
        it->setChoices(whatIsThisLayer->getLevels());

    }else if(gameName == "FeedingTime"){
        auto feedingTimeLayer = FeedingTimeLayer::create();
        it->setChoices(feedingTimeLayer->getLevels());
        
    }else if(gameName == "LineMatching"){
        vector<int> ddLevels = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12 };
        it->setChoices(ddLevels);
    }
    else if (gameName == "MissingNumber") {
        it->setChoices(MissingNumber::MissingNumberScene::levelIDsFor());
    }
    else if (gameName == "LRComprehension") {
        it->setChoices(LRComprehensionScene::getLevels());
    }

#if (CC_TARGET_PLATFORM != CC_PLATFORM_WINRT)
    else if(gameName == "EggQuizLiteracy"){
        it->setChoices(EggQuiz::ProblemBank::getInstance()->getLevels("Literacy"));
    }
    
    else if (gameName == "EggQuizMath") {
        it->setChoices(EggQuiz::ProblemBank::getInstance()->getLevels("Math"));
    }

    else if (gameName == "ThirtyPuzzle") {
		it->setChoices(ThirtyPuzzle::ProblemBank::getInstance()->getLevels());
	}
	else if (gameName == "WordNote") {
		it->setChoices(WordNote::ProblemBank::getInstance()->getLevels());
    }
    else if (gameName == "ReadingBird") {
        it->setChoices(ReadingBird::ReadingBirdScene::levelIDsFor());
    }

#endif //WINRT
    //nextScene = it->minimalSceneByWrapping();
    Director::getInstance()->pushScene(TransitionFade::create(0.5, TodoLoadingScene::createScene([it](){
        auto ret = it->minimalSceneByWrapping();
        it->release();
        return ret;
    })));
}

void CCAppController::startCurriculumGame(std::string levelID, int day, int gameIndex)
{
    auto cur = CurriculumManager::getInstance()->findCurriculum(levelID);
    auto dayCur = cur->getDayCurriculum(day);
    auto gameInfo = dayCur->games.at(gameIndex);
    
    _currentCurrLevelID = levelID;
    _currentCurrDay = day;
    _currentCurrGameIndex = gameIndex;
    _isFreeChoice = false;
    
    if (gameInfo.gameLevel ==1 && gameInfo.appearIndex<=2) {
        _allowSkipTutorial = false;
    }
    
    if (gameInfo.gameName=="Book") {
        startBookScene(gameInfo.gameParameter);
    }

    else if (gameInfo.gameName=="BookWithQuiz") {
        startBookWithQuizScene(gameInfo.gameParameter);
    }
    
    else if (gameInfo.gameName=="Comprehension") {
        startComprehensionScene(gameInfo.gameParameter, gameInfo.gameLevel);
    }
    else if (gameInfo.gameName=="Video") {
        startVideoScene(gameInfo.gameParameter);
        
    } else if (gameInfo.gameName == "EggQuizMath" || gameInfo.gameName == "EggQuizLiteracy") {
        startGame(gameInfo.gameName, gameInfo.gameLevel, gameInfo.gameParameter);

    } else {
        startGame(gameInfo.gameName, gameInfo.gameLevel);
    }
}

void CCAppController::startFreeChoiceGame(std::string gameName, int level)
{
    _isFreeChoice = true;
    startGame(gameName, level);

    
}

std::function<Scene*(void)> CCAppController::getGameCreator(std::string gameName, int level, std::string param)
{
    std::function<Scene*(void)> creator = nullptr;
    
    auto info = KitkitGameInfo::findGameInfo(gameName);
    if (info) {
        string levelID = TodoUtil::itos(level);
        if (level==0) levelID = param;
        creator = [levelID, info]() {
            auto scene = info->createScene(levelID);
            scene->setName(info->getSceneName());
            return scene;
        };
    }
    
    if (gameName == "NumberTracing") {
        creator = [level]() {
            NumberTrace builder;
            
            // NB(xenosoz, 2016): Please consult
            //   $ROOT/Resources/Game/NumberTrace/NumberTrace_Levels.tsv
            builder.setLevelID(level);
            auto scene = builder.createScene();
            scene->setName("NumberTracingScene");
            return scene;
        };
        
    } else if (gameName == "Counting") {
        creator = [level]() {
            auto scene = CountingScene::createScene(level);
            scene->setName("CountingScene");
            return scene;
        };
        
    }
    else if (gameName == "DoubleDigit") {
        creator = [level]() {
            auto nextScene = Scene::create();
            auto gameScene = DoubleDigitScene::create();
            
            DoubleDigit_ProblemBank bank;
            auto param = bank.generateParameters(level);
            
            gameScene->prepareNewGameWithLevel(param);
            gameScene->handleStart();
            
            nextScene->addChild(gameScene);
            nextScene->setName("DoubleDigitScene");
            return nextScene;
            
        };
        
    }
    else if (gameName == "EquationMaker") {
        creator = [level]() {
            auto nextScene = Scene::create();
            auto gameScene = EquationMakerScene::create();
            
            EquationMakerProblemBank bank;
            auto param = bank.generateParameters(level);
            
            gameScene->prepareNewGameWithLevel(param);
            gameScene->handleStart();
            
            nextScene->addChild(gameScene);
            nextScene->setName("EquationMakerScene");
            return nextScene;
            
        };
        
    } else if(gameName == "NumberTracingExt"){
        creator = [level]() {
            NumberTraceExt builder;
            auto scene = builder.createScene(level);
            scene->setName("NumberTracingExtScene");
            return scene;
        };
        
    }else if(gameName == "NumberPuzzle"){
        creator = [level]() {
            NumberPuzzle builder;
            
            // NB(xenosoz, 2016): Please consult
            //   $ROOT/Resources/Game/WoodenPuzzles/NumberPuzzle_Levels.tsv
            builder.setLevelID(level);
            
            auto scene = builder.createScene();
            scene->setName("NumberPuzzleScene");
            return scene;
        };
        
    }else if(gameName == "FindTheMatch"){
        creator = [level]() {
            FindTheMatch builder;
            
            // NB(xenosoz, 2016): Please consult
            //   $ROOT/Resources/Game/FindTheMatch/FindTheMatch_Levels.tsv
            builder.setLevelID(level);
            
            builder.setOnSuccess([] { CCLOG("The user finished FindTheMatch."); });
            builder.setOnFail([] { CCLOG("The user failed to finish FindTheMatch."); });
            
            auto scene = builder.createScene();
            scene->setName("FindTheMatchScene");
            return scene;
        };
        
    }else if(gameName == "NumberMatching"){
        creator = [level]() {
            auto scene = NumberMatchingScene::createScene(level);
            scene->setName("NumberMatchingScene");
            return scene;
        };
        
        
    }else if(gameName == "Tapping"){
        creator = [level]() {
            auto scene = TappingScene::createScene(level);
            scene->setName("TappingScene");
            return scene;
        };
        
    }
    else if (gameName == "TutorialTrace") {
        creator = [level]() {
            TutorialTrace builder;
            builder.setLevelID(level);
            builder.setOnSuccess([] { CCLOG("The user finished TutorialTrace."); });
            builder.setOnFail([] { CCLOG("The user failed to finish TutorialTrace."); });
            
            auto scene = builder.createScene();
            scene->setName("TutorialTraceScene");
            return scene;
        };
    }
    else if(gameName == "SoundTrain"){
        creator = [level]() {
            auto scene = todoschool::soundtrain::SoundTrainScene::createScene(level);
            scene->setName("SoundTrainScene");
            return scene;
        };
    } else if(gameName == "PatternTrain"){
        creator = [level]() {
            auto scene = todoschool::patterntrain::PatternTrainScene::createScene(level);
            scene->setName("PatternTrainScene");
            return scene;
        };
    } else if(gameName == "NumberTrain"){
        creator = [level]() {
            auto scene = NumberTrainScene::createScene(level);
            scene->setName("NumberTrainScene");
            return scene;
        };
    }
    else if(gameName == "ThirtyPuzzle"){
        creator = [level]() {
            auto scene = ThirtyPuzzle::ThirtyPuzzleScene::createScene(level);
            scene->setName("ThirtyPuzzle");
            return scene;
        };
    }
    else if(gameName == "MangoShop"){
        creator = [level]() {
            auto scene = MangoShop::MangoShopScene::createScene(level);
            scene->setName("MangoShopScene");
            return scene;
        };
        
    } else if(gameName == "WordNote"){
        creator = [level]() {
            auto scene = WordNote::WordNoteScene::createScene(level);
            scene->setName("WordNoteScene");
            return scene;
        };
        
    } else if(gameName == "ShapeMatching"){
        creator = [level]() {
            auto scene = ShapeMatchingScene::createScene(level);
            scene->setName("ShapeMatchingScene");
            return scene;
        };
    } else if(gameName == "HundredPuzzle"){
        creator = [level]() {
            auto scene = HundredPuzzleScene::createScene(level);
            scene->setName("HundredPuzzleScene");
            return scene;
        };
        
    } else if(gameName == "MovingInsects"){
        creator = [level]() {
            MovingInsects builder;
            
            // NB(xenosoz, 2016): Please consult
            //   $ROOT/Resources/Game/MovingInsects/MovingInsects_Levels.tsv
            builder.setLevelID(level);
            
            builder.setOnSuccess([] { CCLOG("The user finished MovingInsects."); });
            builder.setOnFail([] { CCLOG("The user failed to finish MovingInsects."); });
            
            auto scene = builder.createScene();
            scene->setName("MovingInsectsScene");
            return scene;
        };
        
    }else if(gameName == "FishTank"){
        creator = [level]() {
            auto nextScene = Scene::create();
            auto gameScene = FishTankScene::create();
            
            FishTankProblemBank bank;
            auto param = bank.generateParameters(level);
            
            gameScene->prepareNewGameWithLevel(param);
            gameScene->handleStart();
            
            nextScene->addChild(gameScene);
            nextScene->setName("FishTankScene");
            return nextScene;
            
        };
    }else if(gameName == "Comprehension"){
        creator = [level]() {
            auto scene = ComprehensionScene::createScene("", 0);
            scene->setName("ComprehensionScene");
            return scene;
        };
        
    }else if(gameName == "AlphabetPuzzle"){
        creator = [level]() {
            AlphabetPuzzle builder;
            
            // NB(xenosoz, 2016): Please consult
            //   $ROOT/Resources/Game/WoodenPuzzles/AlphabetPuzzle_Levels.tsv
            builder.setLevelID(level);
            
            builder.setOnSuccess([] { CCLOG("The user finished AlphabetPuzzle."); });
            builder.setOnFail([] { CCLOG("The user failed to finish AlphabetPuzzle."); });
            
            auto scene = builder.createScene();
            scene->setName("AlphabetPuzzleScene");
            return scene;
        };
        
    }else if(gameName == "WordMachine"){
        creator = [level]() {
            auto scene = WordMachineScene::createScene(level);
            scene->setName("WordMachineScene");
            return scene;
        };
        
    }else if(gameName == "LetterTrace"){
        creator = [level]() {
            LetterTrace builder;
            
            // NB(xenosoz, 2016): Please consult
            //   $ROOT/Resources/Game/LetterTrace/LetterTrace_Levels.tsv
            builder.setLevelID(level);
            
            builder.setOnSuccess([] { CCLOG("The user finished LetterTrace."); });
            builder.setOnFail([] { CCLOG("The user failed to finish LetterTrace."); });
            
            auto scene = builder.createScene();
            scene->setName("LetterTraceScene");
            return scene;
        };
        
    }else if(gameName == "LetterTracingCard"){
        creator = [level]() {
            LetterTracingCard builder;
            auto scene = builder.createScene(level);
            scene->setName("LetterTracingCardScene");
            return scene;
        };
        
    }else if(gameName == "WordTracing"){
        creator = [level]() {
            WordTrace builder;
            
            // NB(xenosoz, 2016): Please consult
            //   $ROOT/Resources/Game/WordTrace/WordTrace_Levels.tsv
            builder.setLevelID(level);
            
            builder.setOnSuccess([] { CCLOG("The user finished WordTrace."); });
            builder.setOnFail([] { CCLOG("The user failed to finish WordTrace."); });
            
            auto scene = builder.createScene();
            scene->setName("WordTracingScene");
            return scene;
        };
        
    }else if(gameName == "StarFall"){
        creator = [level]() {
            StarFall builder;
            
            // NB(xenosoz, 2016): Please consult
            //   $ROOT/Resources/Game/StarFall/starfall.sw.json
            builder.setLevelID(level);
            
            builder.setOnSuccess([] { CCLOG("The user finished StarFall."); });
            builder.setOnFail([] { CCLOG("The user failed to finish StarFall."); });
            
            auto scene = builder.createScene();
            scene->setName("StarFallScene");
            return scene;
        };
        
    }else if(gameName == "Spelling"){
        creator = [level]() {
            Spelling builder;
            
            // NB(xenosoz, 2016): Please consult
            //   $ROOT/Resources/Game/Spelling/Spelling_Levels.tsv
            builder.setLevelID(level);
            
            builder.setOnSuccess([] { CCLOG("The user finished Spelling."); });
            builder.setOnFail([] { CCLOG("The user failed to finish Spelling."); });
            
            auto scene = builder.createScene();
            scene->setName("SpellingScene");
            return scene;
        };
        
    }else if(gameName == "LetterMatching"){
        creator = [level]() {
            auto scene = LetterMatchingScene::createScene(level);
            scene->setName("LetterMatchingScene");
            return scene;
        };
    } else if(gameName == "SentenceMaker"){
        creator = [level]() {
            auto scene = SentenceMakerScene::createScene(level);
            scene->setName("SentenceMakerScene");
            return scene;
        };
        
    } else if(gameName == "EggQuizMath"){
        creator = [param]() {
            auto scene = EggQuiz::EggQuizScene::createScene("Math", param);
            scene->setName("EggQuizMathScene");
            return scene;
        };
    } else if (gameName == "EggQuizLiteracy") {
        creator = [param]() {
            auto scene = EggQuiz::EggQuizScene::createScene("Literacy", param);
            scene->setName("EggQuizLiteracyScene");
            return scene;
        };
        
    } else if (gameName == "BirdPhonics") {
        creator = [level]() {
            auto scene = BirdPhonicsScene::createScene(level);
            scene->setName("BirdPhonicsScene");
            return scene;
        };
        
    } else if (gameName == "WhatIsThis") {
        creator = [level]() {
            auto scene = WhatIsThisScene::create();
            scene->setName("WhatIsThisScene");
            scene->SetLevelAndStart(level);
            return scene;
        };
    } else if (gameName == "FeedingTime") {
        creator = [level]() {
            auto scene = FeedingTimeScene::create();
            scene->setName("FeedingTimeScene");
            scene->SetLevelAndStart(level);
            return scene;
        };
    } else if (gameName == "LineMatching") {
        creator = [level]() {
            auto nextScene = Scene::create();
            auto gameScene = LineMatchingScene::create();
            
            LineMatchingProblemBank bank;
            auto param = bank.generateParameters(level);
            
            gameScene->prepareNewGameWithLevel(param);
            gameScene->handleStart();
            
            nextScene->addChild(gameScene);
            nextScene->setName("LineMatchingScene");
            return nextScene;
        };
    } else if (gameName == "MissingNumber") {
        creator = [level]() {
            auto scene = MissingNumber::MissingNumberScene::createScene(level);
            scene->setName("MissingNumberScene");
            return scene;
        };
    } else if (gameName == "LRComprehension") {
        creator = [level]() {
            auto scene = LRComprehensionScene::createScene(level);
            scene->setName("LRComprehensionScene");
            return scene;
        };
    }
    
#if (CC_TARGET_PLATFORM != CC_PLATFORM_WINRT)
    else if (gameName == "DigitalQuiz") {
        creator = [level]() {
            DigitalQuiz builder;
            
            builder.setClassroom("B");
            builder.setStudentNumber(1);
            builder.setCourseKind(DigitalQuiz::CourseKind::PreTest);
            
            auto scene = builder.createScene();
            scene->setName("DigitalQuiz");
            return scene;
        };
    }
    else if (gameName == "ThirtyPuzzle") {
        creator = [level]() {
            auto scene = ThirtyPuzzle::ThirtyPuzzleScene::createScene(level);
            scene->setName("ThirtyPuzzleScene");
            return scene;
        };
    }
    else if (gameName == "WordNote") {
        creator = [level]() {
            auto scene = WordNote::WordNoteScene::createScene(level);
            scene->setName("WordNoteScene");
            return scene;
        };
    }
    else if (gameName == "ReadingBird") {
        creator = [level]() {
            auto scene = ReadingBird::ReadingBirdScene::createScene(level);
            scene->setName("ReadingBirdScene");
            return scene;
        };
    }
    
#endif  // !WINRT
    
    return creator;
    
}



bool CCAppController::startGame(std::string gameName, int level, std::string param, bool checkOnly)
{
    if (!checkOnly)  {
        GameSoundManager::getInstance()->stopBGM();

        // for memory release
        Director::getInstance()->getTextureCache()->removeUnusedTextures();
    }
    
    _currentGame = gameName;
    _currentParam = "";
    _currentLevel = level;

    //Scene* nextScene;
    std::function<Scene*(void)> creator;

    firebase_setCurrentScreen(_currentGame, "");

    if (gameName == "Book") {
        if (checkOnly) return true;
        startBookScene(kBookFolder);
    } else if (gameName == "BookWithQuiz") {
        if (checkOnly) return true;
        startBookWithQuizScene(kBookFolder);
    } else if (gameName == "Video") {
        if (checkOnly) return true;
    } else {
        creator = getGameCreator(gameName, level, param);
    }
    
    
    if (!creator) {
        
        if (checkOnly) return false;
        
        auto emptyScene = GradeSelector::create()->minimalSceneByWrapping();
        
        Director::getInstance()->pushScene(TransitionFade::create(0.5, emptyScene));
        
        NativeAlert::show("Wrong GameName", gameName + " does not exist", "OK");
        return false;
        
    }
    
    if (checkOnly) return true;
    [&] {
        // NB(xenosoz, 2018): Wrap all game with TouchEventLogger
        auto newCreator = [creator, gameName] {
            auto scene = creator();
            if (scene && scene->getName().empty()) {
                scene->setName(string("(Implicit ") + gameName + string(")"));
            }
            return TouchEventLogger::wrapScene(scene);
        };
        creator = newCreator;
    }();
    

    StrictLogManager::shared()->game_Begin(_currentGame, _currentLevel, param);
    _playTimer->start();

    auto allowSkip = _allowSkipTutorial;
    _allowSkipTutorial = true;

    bool firstLevel = (level==1) || ( (level==0) && (atoi(param.c_str()))==1);
    
    if ( firstLevel && TutorialVideoScene::tutorialExists(gameName))
    {
        auto tutorialCreator = [creator, gameName, allowSkip](){
            auto scene = TutorialVideoScene::createScene(gameName, creator, allowSkip);
            return scene;
        };
        Director::getInstance()->pushScene(TransitionFade::create(0.5, TodoLoadingScene::createScene(tutorialCreator)));
    }
    // MangoShop Level #5 : using custom tutorial
    else if (level == 5 && gameName == "MangoShop" && TutorialVideoScene::tutorialExistsWithCustomName("xPrize Tutorial - MangoShop2.m4v"))
    {
        auto tutorialCreator = [creator, gameName](){
            auto scene = TutorialVideoScene::createSceneWithCustomFileName("xPrize Tutorial - MangoShop2.m4v", creator);
            return scene;
        };
        Director::getInstance()->pushScene(TransitionFade::create(0.5, TodoLoadingScene::createScene(tutorialCreator)));
    }
    else
    {
        Director::getInstance()->pushScene(TransitionFade::create(0.5, TodoLoadingScene::createScene(creator)));
    }
    
    return true;

}

// JNI function to log the event "playGame" after a game has been played
void logFirebaseEvent_playGame(std::string game, int level, double duration, bool freechoice, bool completed) {
    JniMethodInfo t;
    bool getInfo = JniHelper::getStaticMethodInfo(t, "org/cocos2dx/cpp/AppActivity", "logFirebaseEvent_playGame", "(Ljava/lang/String;IDZZ)V");
    if (getInfo)
    {
        jstring jGame = t.env->NewStringUTF(game.c_str());
        t.env->CallStaticVoidMethod(t.classID, t.methodID, jGame, level, duration, freechoice, completed);
        t.env->DeleteLocalRef(t.classID);
        t.env->DeleteLocalRef(jGame);
    }
}

void CCAppController::handleGameQuit(bool bImmediately)
{
    if (_currentGame != "") {
        double duration = _playTimer->stop();
        // Log an event for quitting the game without finishing it
        logFirebaseEvent_playGame(_currentGame, _currentLevel, duration, _isFreeChoice, false);
        // Set the parameter "current_screen_name" back to NULL
        firebase_setCurrentScreen("", "");
        _playTimer->start();
        
        if (_currentGame == "__BookView__") {
            StrictLogManager::shared()->book_End_Quit(_currentParam, duration);
        }
        if (_currentGame == "__BookViewWithComprehension__") {
            StrictLogManager::shared()->BookWithQuiz_End_Quit(_currentParam);
        }
        else if (_currentGame == "__Comprehension__") {
            StrictLogManager::shared()->comprehension_End_Quit(_currentParam, _currentLevel, duration);
        }
        else if (_currentGame == "__GameVideo__") {
            StrictLogManager::shared()->video_End_Quit(_currentParam, duration);
        }
        else {
            StrictLogManager::shared()->game_End_Quit(_currentGame, _currentLevel, duration);
        }
    }
    
    _currentGame = "";
    _currentParam = "";
    _currentLevel = 0;
    _isFreeChoice = false;

    if (bImmediately) {
        ((CustomDirector*)Director::getInstance())->popScene();
    } else {
        ((CustomDirector*)Director::getInstance())->popSceneWithTransition<TransitionFade>(0.5);
    }

}

void CCAppController::handleGameComplete(int result)
{
    //UserManager::getInstance()->setCompletedGame(_currentGame, _currentLevel);
    
    if (_currentCurrLevelID!="") {
        if (TodoUtil::endsWith(_currentCurrLevelID, StringUtils::format("_%d", CoopScene::LEVEL_SPECIAL_COURSE))) {
            int currentSpecialGame = UserManager::getInstance()->getSpecialCourseCurrentProgress(_currentCurrLevelID, _currentCurrDay);
            
            UserManager::getInstance()->setSpecialCourseCurrentProgress(_currentCurrLevelID, _currentCurrDay, currentSpecialGame + 1);

        }
    }
    
    
    double duration = _playTimer->stop();
    // Log an event for finishing the game
    logFirebaseEvent_playGame(_currentGame, _currentLevel, duration, _isFreeChoice, true);
    // Set the parameter "current_screen_name" back to NULL
    firebase_setCurrentScreen("", "");
    _playTimer->start();
    
    if (_currentGame == "__BookView__") {
        StrictLogManager::shared()->book_End_Complete(_currentParam, duration, result);
    }
    else if (_currentGame == "__BookViewWithComprehension__") {
        StrictLogManager::shared()->BookWithQuiz_End_Complete(_currentParam, result);
    }
    else if (_currentGame == "__Comprehension__") {
        StrictLogManager::shared()->comprehension_End_Complete(_currentParam, _currentLevel, duration, result);
    }
    else if (_currentGame == "__GameVideo__") {
        StrictLogManager::shared()->video_End_Complete(_currentParam, duration, result);
    }
    else {
        StrictLogManager::shared()->game_End_Complete(_currentGame, _currentLevel, duration, result);
    }
    
    
    _currentGame = "";
    _currentParam = "";
    _currentLevel = 0;
    
    if (_isFreeChoice) {
        auto stars = UserManager::getInstance()->getStars();
        UserManager::getInstance()->updateStars(stars+1);
        _isFreeChoice = false;
    }
    
    
    ((CustomDirector*)Director::getInstance())->popSceneWithTransition<TransitionFade>(0.5);
}


void CCAppController::startBookScene(std::string bookFolder, bool replaceParent, bool checkCompleteCondition)
{
    StrictLogManager::shared()->book_Begin(bookFolder);
    _playTimer->start();
    
    _currentGame = "__BookView__";
    _currentParam = bookFolder;
    _currentLevel = 0;

    BookView::setBookLanguage(LanguageManager::getInstance()->getCurrentLanguageCode());
    
    
    GameSoundManager::getInstance()->stopBGM();
    
    std::function<Scene*(void)> creator = [this, bookFolder, checkCompleteCondition]() {
        auto nextScene = Scene::create();
        
        string bookPath = "Books/BookData/" + bookFolder;
        auto bv = BookView::create(Director::getInstance()->getVisibleSize(), bookPath, checkCompleteCondition);
        if (bv) {
            bv->viewTitle(1.0);
        
            nextScene->addChild(bv);
        } else {
            auto backButton = TodoSchoolBackButton::create();
            backButton->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
            backButton->setPosition(Vec2(25, nextScene->getContentSize().height-25));
            nextScene->addChild(backButton);
            
            auto l = TodoUtil::createLabel(LanguageManager::getInstance()->getLocalizedString("Error"), 200, Size::ZERO, "fonts/Aileron-Regular.otf", Color4B::WHITE);
            l->setPosition(nextScene->getContentSize()/2);
            nextScene->addChild(l);
            
        }

        return nextScene;
    };
    
    if (replaceParent) {
        Director::getInstance()->replaceScene(TransitionFade::create(0.5, TodoLoadingScene::createScene(creator)));
    } else {
        Director::getInstance()->pushScene(TransitionFade::create(0.5, TodoLoadingScene::createScene(creator)));
    }
}

void CCAppController::startBookWithQuizScene(std::string bookFolder)
{
    StrictLogManager::shared()->book_Begin(bookFolder);
    _playTimer->start();
    
    _currentGame = "__BookViewWithComprehension__";
    _currentParam = bookFolder;
    _currentLevel = 0;
    
    auto scene = BookWithQuiz::createScene(bookFolder, 1);
    Director::getInstance()->pushScene(scene);
}

void CCAppController::startComprehensionScene(std::string bookFolder, int set, bool replaceParent, bool checkCompleteCondition)
{
    StrictLogManager::shared()->comprehension_Begin(bookFolder, set);
    _playTimer->start();

    _currentParam = bookFolder;
    _currentLevel = set;
    
    GameSoundManager::getInstance()->stopBGM();

    std::function<Scene*(void)> creator = [bookFolder, set, checkCompleteCondition]() {
        return ComprehensionScene::createScene("Books/BookData/" + bookFolder, set, checkCompleteCondition);
    };
    
    string tutorialName = "";
    if (bookFolder=="sw_101" || bookFolder=="sw_102") tutorialName = "Comprehension Quiz Line";
    if (bookFolder=="sw_201" || bookFolder=="sw_202") tutorialName = "Comprehension Quiz Sound";
    
    if (tutorialName!="" && TutorialVideoScene::tutorialExists(tutorialName)) {
        auto tutorialCreator = [creator, tutorialName](){
            auto scene = TutorialVideoScene::createScene(tutorialName, creator);
            return scene;
            
        };
        
        Director::getInstance()->pushScene(TransitionFade::create(0.5, TodoLoadingScene::createScene(tutorialCreator)));
    } else {
        
        if (replaceParent) {
            Director::getInstance()->replaceScene(TransitionFade::create(0.5, TodoLoadingScene::createScene(creator)));
        } else {
            Director::getInstance()->pushScene(TransitionFade::create(0.5, TodoLoadingScene::createScene(creator)));
        }
    }
    
    
    
}


void CCAppController::startVideoScene(std::string filename, bool replaceParent)
{
    StrictLogManager::shared()->video_Begin(filename);
    _playTimer->start();
    
    _currentGame = "__GameVideo__";
    _currentParam = filename;
    _currentLevel = 0;
    
    GameSoundManager::getInstance()->stopBGM();
    
    std::function<Scene*(void)> creator = [this, filename]() {
        auto nextScene = GameVideoScene::createScene(filename);
        
        return nextScene;
    };
    
    if (replaceParent) {
        Director::getInstance()->replaceScene(TransitionFade::create(0.5, TodoLoadingScene::createScene(creator)));
    } else {
        Director::getInstance()->pushScene(TransitionFade::create(0.5, TodoLoadingScene::createScene(creator)));
    }
}

#if (CC_TARGET_PLATFORM != CC_PLATFORM_WINRT)
void CCAppController::startQuiz(std::string classroom, int studentNumber, std::string courseKind)
{
    
    std::function<Scene*(void)> creator = [classroom, studentNumber, courseKind, this]() {
        DigitalQuiz builder;
        
        builder.setClassroom(classroom);
        builder.setStudentNumber(studentNumber);
        
        if (courseKind == "Pre-Test") {
            builder.setCourseKind(DigitalQuiz::CourseKind::PreTest);
        }
        else {
            builder.setCourseKind(DigitalQuiz::CourseKind::PostTest);
        }
        
        
        StrictLogManager::shared()->game_Begin(_currentGame, _currentLevel, "");
        _playTimer->start();


        auto nextScene = builder.createScene();
        return nextScene;
    };
    
    Director::getInstance()->pushScene(TransitionFade::create(0.5, TodoLoadingScene::createScene(creator)));
    
    
}


#else

void CCAppController::startQuiz(std::string classroom, int studentNumber, std::string courseKind) {}

#endif //WINRT
    /*
void CCAppController::startEggQuiz(char category, int categoryLevel, bool isPreTest, std::function<void(bool)> callback)
{

    auto level = categoryLevel;
    if (category=='M') level+=5;
    if (level<1 || level>10) return;
    
    _currentGame = "EggQuiz";
    _currentParam = "";
    _currentLevel = level;
    
    std::function<Scene*(void)> creator = [category, categoryLevel, isPreTest, callback]() {
        
        auto scene = Scene::create();
        
        auto layer = EggQuizScene::create();
        layer->setLevel(category, categoryLevel, isPreTest);
        layer->onCompleteCallback = callback;
        
        scene->addChild(layer);
        
        return scene;
        
        
    };
    
    
    StrictLogManager::shared()->game_Begin(_currentGame, _currentLevel, "");
    _playTimer->start();
    
    Director::getInstance()->pushScene(TransitionFade::create(0.5, TodoLoadingScene::createScene(creator)));

}
          */
bool CCAppController::isDebug()
{
    return UserManager::getInstance()->isDebugMode();
}
