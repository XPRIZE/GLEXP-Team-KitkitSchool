//
//  CurriculumManager.cpp
//  KitkitSchool
//
//  Created by Gunho Lee on 12/23/16.
//
//

#include "CurriculumManager.hpp"
#include "cocos2d.h"
#include <Common/Basic/CommentStream.h>
#include <Common/Basic/CustomFacet.h>
#include "Managers/LanguageManager.hpp"
#include "Managers/UserManager.hpp"

#if (CC_TARGET_PLATFORM != CC_PLATFORM_WINRT)
#include "Games/LetterTracingCard/Utils/LetterTracingCardUtility.hpp"
#endif

#include "CCAppController.hpp"

#include "Utils/TodoUtil.h"
#include "Games/Video/GameVideoScene.hpp"

#include <map>

USING_NS_CC;

using namespace std;

CurriculumManager* CurriculumManager::_instance = 0;

CurriculumManager* CurriculumManager::getInstance()
{
    if(!_instance) {
        _instance = new CurriculumManager();
        _instance->init();
    }
    return _instance;
}


void CurriculumManager::init()
{

    loadData();
}

string CurriculumManager::makeLevelID(string lang, char category, int level)
{
    return lang+"_"+string(1, category)+"_"+TodoUtil::itos(level);
}



LevelCurriculum* CurriculumManager::findCurriculum(char category, int level)
{
    auto lang = LanguageManager::getInstance()->getCurrentLanguageTag();
    auto id = makeLevelID(lang, category, level);
    
    
    return findCurriculum(id);
    
}

LevelCurriculum* CurriculumManager::findCurriculum(string levelID)
{
    auto cur = levels.find(levelID);
    if (cur==levels.end()) {
        return nullptr;
    } else {
        return &(cur->second);
        
    }
}


void CurriculumManager::loadData()
{
    /*
    string resourceName = LanguageManager::getInstance()->isSignLanguageMode() ? "curriculumdata_sl.tsv":"curriculumdata.tsv";
    string P = LanguageManager::getInstance()->findLocalizedResource(resourceName);
    string S = FileUtils::getInstance()->getStringFromFile(P);
    */
    
    string dataSheet = "curriculumdata.tsv";
    string dataSheetSL = "curriculumdata_sl.tsv";
    string resourceName = LanguageManager::getInstance()->isSignLanguageMode() ? dataSheetSL : dataSheet;
    string resourcePath = LanguageManager::getInstance()->findLocalizedResource(resourceName);
    if (FileUtils::getInstance()->isFileExist(resourcePath) == false)
        resourcePath = LanguageManager::getInstance()->findLocalizedResource(dataSheet);
    string s = FileUtils::getInstance()->getStringFromFile(resourcePath);
    
    todoschool::CommentStream IS(s);
    
    auto Facet = [&] {
        auto It = new todoschool::CustomFacet(IS.getloc());
        
        // NB(xenosoz, 2016): U+0020 'SPACE' is not a whitespace anymore.
        It->clearSpaceFlagsFor(" ");
        return It;
    }();
    
    
    auto Loc = locale(IS.getloc(), Facet);
    IS.imbue(Loc);
    
    
    string entryType;


    vector<string> wrongGameNames;
    vector<string> wrongVideos;
    vector<string> wrongBooks;
    map<string, int> occurrenceMap;
    
    
    

    auto loadGame = [&](GameDescription &game) {
        string entryType;
        
        string gameName, gameLevel, gameParam;
        
        bool ret = (IS >> entryType >> gameName >> gameLevel >> gameParam);
        if (!ret || entryType!="game") {
            CCLOGERROR("Unexpected input stream error in loadGame");
        }
        
        game.gameName = gameName;
       
		int gameLevelInt = TodoUtil::stoi(gameLevel);
        
        game.gameLevel = gameLevelInt;
        game.gameParameter = gameParam;
        
        if (gameLevelInt==1) {
            if (occurrenceMap.find(gameName) == occurrenceMap.end()) {
                occurrenceMap[gameName] = 0;
            }
            auto occurrence = occurrenceMap[gameName]+1;
            occurrenceMap[gameName] = occurrence;
            game.appearIndex = occurrence;
        }
        
  
        if (UserManager::getInstance()->isDebugMode()) {
            if(gameName == "Book"){
                    string bookPath = "Books/BookData/"+ gameParam + "/bookinfo.csv";
                    if ( !FileUtils::getInstance()->isFileExist(bookPath)) {
                        if (find(wrongBooks.begin(), wrongBooks.end(), gameParam)==wrongBooks.end())
                            wrongBooks.push_back((gameParam));
                    }
            
            } else if (gameName == "Video") {
                if( !GameVideoScene::videoExists(gameParam)) {
                    
                    if (find(wrongVideos.begin(), wrongVideos.end(), gameParam)==wrongVideos.end())
                        wrongVideos.push_back((gameParam));
                }
            } else {
        
                if (!CCAppController::sharedAppController()->gameExists(game.gameName)) {
                    if (find(wrongGameNames.begin(), wrongGameNames.end(), game.gameName)==wrongGameNames.end())
                        wrongGameNames.push_back((game.gameName));
                    //string err = "Wrong game name : "+ game.gameName;
                    //CCLOGERROR("%s", err.c_str());
                }
            }
        }
        
        

    };
    
    
    auto loadDay = [&](DayCurriculum &day) {
        string entryType;
        bool ret = (IS >> entryType >> day.dayOrder >> day.numGames);
        if (ret && entryType=="day") {
            day.isEggQuiz = false;
            day.isMiniQuiz = false;

            for (int i=0; i<day.numGames; i++) {
                GameDescription game;
                loadGame(game);
                day.games.push_back(game);
            }
            
            if (day.numGames==1 && (day.games.at(0).gameName=="EggQuizLiteracy" || day.games.at(0).gameName=="EggQuizMath")) {
                day.isEggQuiz = true;
                if (day.games.at(0).gameParameter.find("MiniTest") != string::npos) {
                    day.isMiniQuiz = true;
                }
            }
            
        } else {
            CCLOGERROR("Unexpected input stream error in loadDay");
        }
    };
    
    auto loadLevel = [&](LevelCurriculum &level) {
        if (IS >> level.lang >> level.levelTitle >> level.category >> level.categoryLevel >> level.numDays) {
            level.levelID = makeLevelID(level.lang, level.category, level.categoryLevel);
            for (int i=0; i<level.numDays; i++) {
                DayCurriculum day;
                loadDay(day);
                level.days.push_back(day);
            }
        } else {
            CCLOGERROR("Unexpected input stream error in loadLevel");
            
        }
    };
    
    while (IS >> entryType)
    {
        
        if (entryType=="level") {
            LevelCurriculum level;
            loadLevel(level);
            
//            if (level.categoryLevel>=1) {
//                level.numDays++;
//                DayCurriculum quiz;
//                quiz.numGames = 1;
//                quiz.dayOrder = level.numDays;
//                quiz.isEggQuiz = true;
//                
//                GameDescription game;
//                game.gameName = "EggQuiz";
//                game.gameLevel = level.categoryLevel;
//                if (level.category=='M') game.gameLevel+=5;
//                
//                quiz.games.push_back(game);
//                
//                
//                level.days.push_back(quiz);
//            }
            
            levels[level.levelID] = level;
        }
    }
    
    
    if (UserManager::getInstance()->isDebugMode()) {
        string files;
        if (wrongGameNames.size()>0) {
            files += "Wrong Game Name\n";
            for (auto file : wrongGameNames) {
                files += file + "\n";
            }
        }
        
        if (wrongBooks.size()>0) {
            files += "Wrong Book Name\n";
            for (auto file : wrongBooks) {
                files += file + "\n";
            }
        }
        
        if (wrongVideos.size()>0) {
            files += "Wrong video Name\n";
            for (auto file : wrongVideos) {
                files += file + "\n";
            }
        }
        if (files.size() > 0)
            MessageBox("Wrong curriculum Data", files.c_str() );

    }
    
}
