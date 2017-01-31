//
//  CCAppController.cpp
//  enumaXprize
//
//  Created by Gunho Lee on 11/1/16.
//
//

#include "CCAppController.hpp"
#include "Common/Controls/TodoLoadingScene.hpp"
#include "Common/Controls/TodoSchoolBackButton.hpp"
#include "Games/NumberTrace/NumberTrace.h"
#include "Games/WoodenPuzzles/NumberPuzzle.h"
#include "Games/FindTheMatch/FindTheMatch.h"
#include "Games/NumberMatching/NumberMatchingScene.hpp"
#include "Games/Counting/CountingScene.hpp"
#include "Games/MovingInsects/MovingInsects.h"
#include "Games/FishTank/FishTankScene.h"
#include "Games/FishTank/FishTankProblemBank.h"
#include "Games/DoubleDigit/DoubleDigitScene.h"
#include "Games/DoubleDigit/DoubleDigit_ProblemBank.h"
#include "Games/EquationMaker/EquationMakerScene.h"
#include "Games/EquationMaker/EquationMakerProblemBank.h"
#include "Games/Books/BookView.hpp"
#include "Games/WoodenPuzzles/AlphabetPuzzle.h"
#include "Games/WordMachine/WordMachineScene.hpp"
#include "Games/LetterTrace/LetterTrace.h"
#include "Games/WordTrace/WordTrace.h"
#include "Games/StarFall/StarFall.h"
#include "Games/Spelling/Spelling.h"
#include "Games/LetterMatching/LetterMatchingScene.hpp"
#include "Games/DigitalQuiz/DigitalQuiz.h"
#include "Games/Tapping/TappingScene.hpp"
#include "Games/TutorialTrace/TutorialTrace.h"
#include "Games/AnimalPuzzle/AnimalPuzzleScene.hpp"
#include "Games/HundredPuzzle/HundredPuzzleScene.hpp"
#include "Games/ComprehensionTest/Matching/MatchingScene.hpp"
#include "Games/ComprehensionTest/ComprehensionScene.hpp"
#include "Games/SentenceMaker/SentenceMakerScene.hpp"
#include "Games/EggQuiz/EggQuizScene.hpp"

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

#include "CustomDirector.h"
#include "3rdParty/CCNativeAlert.h"

using namespace std;
USING_NS_CC;


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
    
    //BookManager::getInstance()->addBookInfo("We are", LanguageManager::ENGLISH, "Books/We are/", "Books/book2/page/");
    BookManager::getInstance()->addBookInfo("book2", LanguageManager::SWAHILI, "book2/");
    BookManager::getInstance()->addBookInfo("en_504", LanguageManager::SWAHILI, "en_504/");
    //BookManager::getInstance()->addBookInfo("Wanyama (Test)", LanguageManager::SWAHILI, "Books/book_landscape_test/");
    
    CurriculumManager::getInstance();
    _currentCurrLevelID = "";
    
    
}

CCAppController::~CCAppController() {

    CC_SAFE_DELETE(_playTimer);
    
}

bool CCAppController::gameExists(std::string gameName)
{
    return this->startGame(gameName, 0, true);
    
}


BookView* CCAppController::createBookView(std::string bookName)
{
    
    auto info = BookManager::getInstance()->findBookInfo(bookName);
    if (!info) return nullptr;
    
    
    auto bv = BookView::create(Director::getInstance()->getVisibleSize(), info->bookPath);

    if (info->bookImagePath!="") {
        bv->getBook()->imagePrefix = info->bookImagePath;
    }
    
    bv->viewTitle(1.0);
    
    return bv;
}





void CCAppController::startGameWithLevelChoice(std::string gameName)
{
    _currentGame = "";
    _currentLevel = 0;
    
    // bluecloud games
    
    if(gameName == "Book"){
        
        //startGame(gameName, 0);
        
        //auto* nextScene = BookChoiceScene::createScene();
        
        Director::getInstance()->pushScene(TransitionFade::create(0.5, TodoLoadingScene::createScene([](){
            return BookChoiceScene::createScene();
        })));
        return;
    } else if (gameName == "DigitalQuiz") {
        startGame(gameName, 0);
        return;
    }
    
    
    Scene* nextScene;
    auto it = GradeSelector::create();
    it->retain();
    it->setGameName(gameName);
    
    if(gameName == "FishTank"){
        vector<int> fishLevels = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11 };
        it->setChoices(fishLevels);

    }else if (gameName=="DoubleDigit") {
        vector<int> ddLevels = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15 };
        it->setChoices(ddLevels);
        
    }else if (gameName=="EquationMaker") {
        vector<int> ddLevels = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
        it->setChoices(ddLevels);
        
    } else if (gameName=="Tapping") {
        vector<int> ddLevels = { 1, 2, 3};
        it->setChoices(ddLevels);
    }
    else if (gameName == "TutorialTrace") {
        auto levels = TutorialTrace().getCandidateLevelIDs();
        it->setChoices(levels);
    }
    else if (gameName=="AnimalPuzzle") {
        vector<int> ddLevels = { 1, 2, 3, 4, 5, 6, 7 };
        it->setChoices(ddLevels);

    } else if (gameName=="HundredPuzzle") {
        vector<int> ddLevels = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13};
        it->setChoices(ddLevels);
        
    } else if(gameName == "NumberTracing"){
        
        it->setChoices(NumberTrace().getCandidateLevelIDs());

    }else if(gameName == "NumberPuzzle"){

        it->setChoices(NumberPuzzle().getCandidateLevelIDs());
        
    }else if(gameName == "FindTheMatch"){

        it->setChoices(FindTheMatch().getCandidateLevelIDs());
        
    }else if(gameName == "NumberMatching"){
        
        auto lang = LanguageManager::getInstance()->getCurrentLanguageTag();
        auto data = todoschool::numbermatching::LevelData::defaultData();
        
        it->setChoices(data.levelIDsFor(lang));

    }else if(gameName == "Counting"){
        
        using todoschool::counting::LevelData;
        
        auto lang = LanguageManager::getInstance()->getCurrentLanguageTag();
        it->setChoices(LevelData::defaultData().levelIDsFor(lang));

    }else if(gameName == "MovingInsects"){

        it->setChoices(MovingInsects().getCandidateLevelIDs());

    }else if(gameName == "AlphabetPuzzle"){
        
        it->setChoices(AlphabetPuzzle().getCandidateLevelIDs());

    }else if(gameName == "WordMachine"){

        using todoschool::wordmachine::LevelData;
        
        auto lang = LanguageManager::getInstance()->getCurrentLanguageTag();
        
        it->setChoices(LevelData::defaultData().levelIDsFor(lang));

    }else if(gameName == "LetterTracing"){

        it->setChoices(LetterTrace().getCandidateLevelIDs());

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
        
    }else if (gameName == "CompMatching") {
        
        vector<int> ddLevels = { 1 };
        it->setChoices(ddLevels);
        
    }else if (gameName == "SentenceMaker") {
        
        vector<int> ddLevels = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17 };
        it->setChoices(ddLevels);
    } else if (gameName == "EggQuiz") {
        vector<int> ddLevels = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 12, 13, 14, 15, 17, 18, 19, 20 };
        it->setChoices(ddLevels);
    }
    
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
    

    
    if (gameInfo.gameName=="Book") {
     
        startBookScene(gameInfo.gameParameter+"/");
        
    } else if (gameInfo.gameName=="Comprehension") {
        
        startComprehensionScene(gameInfo.gameParameter, gameInfo.gameLevel);
        
    } else {
        startGame(gameInfo.gameName, gameInfo.gameLevel);
    }
    
}


bool CCAppController::startGame(std::string gameName, int level, bool checkOnly)
{
    GameSoundManager::getInstance()->stopBGM();
    
    // for memory release
    Director::getInstance()->getTextureCache()->removeUnusedTextures();

    
    _currentGame = gameName;
    _currentLevel = level;
    
    //Scene* nextScene;
    std::function<Scene*(void)> creator;
    
    
    if(gameName == "NumberTracing"){
        creator = [level]() {
            NumberTrace builder;
            
            // NB(xenosoz, 2016): Please consult
            //   $ROOT/Resources/Game/NumberTrace/NumberTrace_Levels.tsv
            builder.setLevelID(level);
            
            return builder.createScene();
        };
        

    }else if(gameName == "NumberPuzzle"){
        creator = [level]() {
            NumberPuzzle builder;
            
            // NB(xenosoz, 2016): Please consult
            //   $ROOT/Resources/Game/WoodenPuzzles/NumberPuzzle_Levels.tsv
            builder.setLevelID(level);
            
            return builder.createScene();
        };

    }else if(gameName == "FindTheMatch"){
        creator = [level]() {
            FindTheMatch builder;
            
            // NB(xenosoz, 2016): Please consult
            //   $ROOT/Resources/Game/FindTheMatch/FindTheMatch_Levels.tsv
            builder.setLevelID(level);
            
            builder.setOnSuccess([] { CCLOG("The user finished FindTheMatch."); });
            builder.setOnFail([] { CCLOG("The user failed to finish FindTheMatch."); });
            
            return builder.createScene();
        };

    }else if(gameName == "NumberMatching"){

        creator = [level]() {
            return NumberMatchingScene::createScene(level);
        };
        

    }else if(gameName == "Counting"){
        creator = [level]() {
            return CountingScene::createScene(level);
        };

    }else if(gameName == "Tapping"){
        creator = [level]() {
            return TappingScene::createScene(level);
        };
        
    }
    else if (gameName == "TutorialTrace") {
        creator = [level]() {
            TutorialTrace builder;
            builder.setLevelID(level);
            builder.setOnSuccess([] { CCLOG("The user finished TutorialTrace."); });
            builder.setOnFail([] { CCLOG("The user failed to finish TutorialTrace."); });
            return builder.createScene();
        };
    }
    else if(gameName == "AnimalPuzzle"){
        creator = [level]() {
            return AnimalPuzzleScene::createScene(level);
        };
        
    } else if(gameName == "HundredPuzzle"){
        creator = [level]() {
            return HundredPuzzleScene::createScene(level);
        };
        
    } else if(gameName == "MovingInsects"){
        creator = [level]() {
            MovingInsects builder;
            
            // NB(xenosoz, 2016): Please consult
            //   $ROOT/Resources/Game/MovingInsects/MovingInsects_Levels.tsv
            builder.setLevelID(level);
            
            builder.setOnSuccess([] { CCLOG("The user finished MovingInsects."); });
            builder.setOnFail([] { CCLOG("The user failed to finish MovingInsects."); });
            
            return builder.createScene();
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
            return nextScene;
            
        };
    }else if(gameName == "DoubleDigit"){
        creator = [level]() {
            auto nextScene = Scene::create();
            auto gameScene = DoubleDigitScene::create();
            
            DoubleDigit_ProblemBank bank;
            auto param = bank.generateParameters(level);
            
            gameScene->prepareNewGameWithLevel(param);
            gameScene->handleStart();
            
            nextScene->addChild(gameScene);
            return nextScene;
            
        };
        
    }else if(gameName == "EquationMaker"){
        creator = [level]() {
            auto nextScene = Scene::create();
            auto gameScene = EquationMakerScene::create();
            
            EquationMakerProblemBank bank;
            auto param = bank.generateParameters(level);
            
            gameScene->prepareNewGameWithLevel(param);
            gameScene->handleStart();
            
            nextScene->addChild(gameScene);
            return nextScene;
            
        };
        
    }else if(gameName == "Book"){
        
       
        if (checkOnly) return true;
        
        startBookScene("book2/");
        
    }else if(gameName == "Comprehension"){
        creator = [level]() {
            return ComprehensionScene::createScene("", 0);
            //return ComprehensionScene::createScene("book2", 1);
        };
        
    }else if(gameName == "AlphabetPuzzle"){
        creator = [level]() {
            AlphabetPuzzle builder;
            
            // NB(xenosoz, 2016): Please consult
            //   $ROOT/Resources/Game/WoodenPuzzles/AlphabetPuzzle_Levels.tsv
            builder.setLevelID(level);
            
            builder.setOnSuccess([] { CCLOG("The user finished AlphabetPuzzle."); });
            builder.setOnFail([] { CCLOG("The user failed to finish AlphabetPuzzle."); });
            
            return builder.createScene();
        };
        
    }else if(gameName == "WordMachine"){
        creator = [level]() {
            return WordMachineScene::createScene(level);
        };

    }else if(gameName == "LetterTracing"){
        creator = [level]() {
            LetterTrace builder;
            
            // NB(xenosoz, 2016): Please consult
            //   $ROOT/Resources/Game/LetterTrace/LetterTrace_Levels.tsv
            builder.setLevelID(level);
            
            builder.setOnSuccess([] { CCLOG("The user finished LetterTrace."); });
            builder.setOnFail([] { CCLOG("The user failed to finish LetterTrace."); });
            
            return builder.createScene();
        };

    }else if(gameName == "WordTracing"){
        creator = [level]() {
            WordTrace builder;
            
            // NB(xenosoz, 2016): Please consult
            //   $ROOT/Resources/Game/WordTrace/WordTrace_Levels.tsv
            builder.setLevelID(level);
            
            builder.setOnSuccess([] { CCLOG("The user finished WordTrace."); });
            builder.setOnFail([] { CCLOG("The user failed to finish WordTrace."); });
            
            return builder.createScene();
        };

    }else if(gameName == "StarFall"){
        creator = [level]() {
            StarFall builder;
            
            // NB(xenosoz, 2016): Please consult
            //   $ROOT/Resources/Game/StarFall/starfall.sw.json
            builder.setLevelID(level);
            
            builder.setOnSuccess([] { CCLOG("The user finished StarFall."); });
            builder.setOnFail([] { CCLOG("The user failed to finish StarFall."); });
            
            return builder.createScene();
        };
        
    }else if(gameName == "Spelling"){
        creator = [level]() {
            Spelling builder;
            
            // NB(xenosoz, 2016): Please consult
            //   $ROOT/Resources/Game/Spelling/Spelling_Levels.tsv
            builder.setLevelID(level);
            
            builder.setOnSuccess([] { CCLOG("The user finished Spelling."); });
            builder.setOnFail([] { CCLOG("The user failed to finish Spelling."); });
            
            return builder.createScene();
        };

    }else if(gameName == "LetterMatching"){
        creator = [level]() {
            return LetterMatchingScene::createScene(level);
        };
    } else if (gameName == "DigitalQuiz") {
        creator = [level]() {
            DigitalQuiz builder;
            
            builder.setClassroom("B");
            builder.setStudentNumber(1);
            builder.setCourseKind(DigitalQuiz::CourseKind::PreTest);
            
            return builder.createScene();
        };
    }else if(gameName == "SentenceMaker"){
        creator = [level]() {
            return SentenceMakerScene::createScene(level);
        };
    } else if (gameName == "EggQuiz") {
        creator = [level]() {
            return EggQuizScene::createScene(level);
        };
        
    }
    
    if (!creator) {

        if (checkOnly) return false;
        
        auto emptyScene = GradeSelector::create()->minimalSceneByWrapping();
        
        Director::getInstance()->pushScene(TransitionFade::create(0.5, emptyScene));
        
        NativeAlert::show("Wrong GameName", gameName + " does not exist", "OK");
        return false;
        
    }
    
    if (checkOnly) return true;
    
    _playTimer->start();
    
    
    Json::Value vl;
    vl["class"] = UserManager::getInstance()->getClassId();
    vl["student"] = UserManager::getInstance()->getStudentId();
    vl["course"] = UserManager::getInstance()->getCourseId();
    vl["timestamp"] = _playTimer->getCurrentTimeOfDouble();
    vl["game"] = _currentGame;
    vl["level"] = _currentLevel;
    
    LogManager::getInstance()->logEventJson("gameStart", vl);
    


    
    if (level==1 && TutorialVideoScene::tutorialExists(gameName)) {
        auto tutorialCreator = [creator, gameName](){
            auto scene = TutorialVideoScene::createScene(gameName, creator);
            return scene;
            
        };
        
        Director::getInstance()->pushScene(TransitionFade::create(0.5, TodoLoadingScene::createScene(tutorialCreator)));
    } else {
        Director::getInstance()->pushScene(TransitionFade::create(0.5, TodoLoadingScene::createScene(creator)));
    }
    
    return true;

}

void CCAppController::handleGameQuit()
{
    if(_currentGame != ""){
        Json::Value vl;
        vl["class"] = UserManager::getInstance()->getClassId();
        vl["student"] = UserManager::getInstance()->getStudentId();
        vl["course"] = UserManager::getInstance()->getCourseId();
        vl["timestamp"] = _playTimer->getCurrentTimeOfDouble();
        vl["duration"] = _playTimer->stop(); _playTimer->start();
        vl["game"] = _currentGame;
        vl["level"] = _currentLevel;
        
        LogManager::getInstance()->logEventJson("gameQuit", vl);
    }
    
    _currentGame = "";
    _currentLevel = 0;
    
    ((CustomDirector*)Director::getInstance())->popSceneWithTransition<TransitionFade>(0.5);
}


void CCAppController::startBookScene(std::string bookFolder, bool replaceParent)
{
    
    GameSoundManager::getInstance()->stopBGM();
    
    std::function<Scene*(void)> creator = [this, bookFolder]() {
        auto nextScene = Scene::create();
        
        string bookPath = "Books/BookData/" + bookFolder;
        auto bv = BookView::create(Director::getInstance()->getVisibleSize(), bookPath);
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
            
            if (_currentCurrLevelID!="") {
                UserManager::getInstance()->setGameCleared(_currentCurrLevelID, _currentCurrDay, _currentCurrGameIndex);
            }
            
        }

        return nextScene;
    };
    
    if (replaceParent) {
        Director::getInstance()->replaceScene(TransitionFade::create(0.5, TodoLoadingScene::createScene(creator)));
    } else {
        Director::getInstance()->pushScene(TransitionFade::create(0.5, TodoLoadingScene::createScene(creator)));
    }
}

void CCAppController::startComprehensionScene(std::string bookFolder, int set, bool replaceParent)
{

    
    GameSoundManager::getInstance()->stopBGM();

    std::function<Scene*(void)> creator = [bookFolder, set]() {
        return ComprehensionScene::createScene(bookFolder, set);
    };
    
    if (replaceParent) {
        Director::getInstance()->replaceScene(TransitionFade::create(0.5, TodoLoadingScene::createScene(creator)));
    } else {
        Director::getInstance()->pushScene(TransitionFade::create(0.5, TodoLoadingScene::createScene(creator)));
    }
}



void CCAppController::handleGameComplete(int result)
{
    UserManager::getInstance()->setCompletedGame(_currentGame, _currentLevel);
    
    if (_currentCurrLevelID!="") {
        UserManager::getInstance()->setGameCleared(_currentCurrLevelID, _currentCurrDay, _currentCurrGameIndex);
    }
    
    Json::Value vl;
    vl["class"] = UserManager::getInstance()->getClassId();
    vl["student"] = UserManager::getInstance()->getStudentId();
    vl["course"] = UserManager::getInstance()->getCourseId();
    vl["timestamp"] = _playTimer->getCurrentTimeOfDouble();
    vl["duration"] = _playTimer->stop(); _playTimer->start();
    vl["game"] = _currentGame;
    vl["level"] = _currentLevel;
    vl["result"] = result;
    
    LogManager::getInstance()->logEventJson("gameComplete", vl);
    
    _currentGame = "";
    _currentLevel = 0;

    
    ((CustomDirector*)Director::getInstance())->popSceneWithTransition<TransitionFade>(0.5);
}

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
        
        
        _playTimer->start();
        
        
        Json::Value vl;
        vl["class"] = UserManager::getInstance()->getClassId();
        vl["student"] = UserManager::getInstance()->getStudentId();
        vl["course"] = UserManager::getInstance()->getCourseId();
        vl["timestamp"] = _playTimer->getCurrentTimeOfDouble();
        vl["game"] = _currentGame;
        vl["level"] = _currentLevel;
        
        LogManager::getInstance()->logEventJson("gameStart", vl);
        
        auto nextScene = builder.createScene();
        return nextScene;
    };
    
    Director::getInstance()->pushScene(TransitionFade::create(0.5, TodoLoadingScene::createScene(creator)));
    
    
}

void CCAppController::startEggQuiz(char category, int categoryLevel, bool isPreTest, std::function<void(bool)> callback)
{

    auto level = categoryLevel;
    if (category=='M') level+=5;
    if (level<1 || level>10) return;
    
    _currentGame = "EggQuiz";
    _currentLevel = level;
    
    std::function<Scene*(void)> creator = [category, categoryLevel, isPreTest, callback]() {

        auto scene = Scene::create();

        auto layer = EggQuizScene::create();
        layer->setLevel(category, categoryLevel, isPreTest);
        layer->onCompleteCallback = callback;

        scene->addChild(layer);
        
        return scene;
        
        
    };
    
    _playTimer->start();
    
    
    Json::Value vl;
    vl["class"] = UserManager::getInstance()->getClassId();
    vl["student"] = UserManager::getInstance()->getStudentId();
    vl["course"] = UserManager::getInstance()->getCourseId();
    vl["timestamp"] = _playTimer->getCurrentTimeOfDouble();
    vl["game"] = _currentGame;
    vl["level"] = _currentLevel;
    
    LogManager::getInstance()->logEventJson("gameStart", vl);
    
    Director::getInstance()->pushScene(TransitionFade::create(0.5, TodoLoadingScene::createScene(creator)));
}


bool CCAppController::isDebug()
{
    return UserManager::getInstance()->isDebugMode();
}
