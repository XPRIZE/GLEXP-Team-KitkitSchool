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
    string P = LanguageManager::getInstance()->findLocalizedResource("CurriculumData.tsv");
    string S = FileUtils::getInstance()->getStringFromFile(P);
    
    todoschool::CommentStream IS(S);
    
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
    map<string, int> occurrenceMap;
    
    
    

    auto loadGame = [&](GameDescription &game) {
        string entryType;
        
        string gameName, gameLevel, gameParam;
        
        bool ret = (IS >> entryType >> gameName >> gameLevel >> gameParam);
        if (!ret || entryType!="game") {
            CCLOGERROR("Unexpected input stream error in loadGame");
        }
        
        game.gameName = gameName;
       
		int gameLevelInt = 0;
        if (gameName=="LetterTracingCard") {
#if (CC_TARGET_PLATFORM != CC_PLATFORM_WINRT)
            gameLevelInt = LetterTracingCardUtility::getLevelByLetter(gameLevel, LanguageManager::getInstance()->isEnglish());
#endif //WINRT
            
        } else {
            gameLevelInt = TodoUtil::stoi(gameLevel);
        }
        
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
            
            if (!CCAppController::sharedAppController()->gameExists(game.gameName)) {
                if (find(wrongGameNames.begin(), wrongGameNames.end(), game.gameName)==wrongGameNames.end())
                    wrongGameNames.push_back(game.gameName);
                //string err = "Wrong game name : "+ game.gameName;
                //CCLOGERROR("%s", err.c_str());
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
    
    
    if (UserManager::getInstance()->isDebugMode() && (wrongGameNames.size()>0)) {
        string files;
        for (auto file : wrongGameNames) {
            files += file + "\n";
        }
        MessageBox("Wrong Game Name in the curriculum", files.c_str() );

    }
    
}
