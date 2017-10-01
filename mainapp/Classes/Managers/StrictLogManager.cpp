//
//  StrictLogManager.cpp -- A log manager with more strict rules than LogManager
//  TodoSchool on Sep 21, 2017
//
//  Copyright (c) 2016 Enuma, Inc. All rights reserved.
//  See LICENSE.md for more details.
//

// NB(xenosoz, 2017): What we're trying to achieve is to make analyze process easier at the cost of project building speed


#include "StrictLogManager.h"
#include "LogManager.hpp"
#include "UserManager.hpp"

#include "Utils/PlayTimer.hpp"
#include "cocos2d.h"
#include <ctime>

namespace
{
    void logJson(Json::Value json) {
        LogManager::getInstance()->logEventJson("StrictLogManager", json);
    }

    std::string datetime()
    {
        time_t now;
        std::time(&now);
        
        char buf[sizeof "2017-09-22T06:58:00Z"];
        std::strftime(buf, sizeof buf, "%FT%TZ", std::gmtime(&now));

        return buf;
    }

    double localTimestamp() {
        PlayTimer timer;
        return timer.getCurrentTimeOfDouble();
    }

    std::string datetimeKey() { return "!datetime"; }
    std::string funcKey() { return ".func"; }
    std::string stampKey() { return "_localTimestamp"; }
    std::string userNameKey() { return "_userName"; }

    Json::Value basicLog(const std::string& fnName) {
        Json::Value it;

        it[datetimeKey()] = datetime();
        it[funcKey()] = fnName;
        
        it[stampKey()] = localTimestamp();
        it[userNameKey()] = UserManager::getInstance()->getCurrentUsername();
        
        return it;
    }
}  // unnamed namespace


void StrictLogManager::curriculumChoice_Begin() {
    Json::Value it = basicLog("curriculumChoice_Begin");
    
    logJson(it);
}

void StrictLogManager::curriculumChoice_End() {
    Json::Value it = basicLog("curriculumChoice_End");
    
    logJson(it);
}

void StrictLogManager::curriculumChoice_TouchCoop() {
    Json::Value it = basicLog("curriculumChoice_TouchCoop");

    logJson(it);
}

void StrictLogManager::courseChoice_Begin() {
    Json::Value it = basicLog("courseChoice_Begin");
    
    logJson(it);
}

void StrictLogManager::courseChoice_End() {
    Json::Value it = basicLog("courseChoice_End");
    
    logJson(it);
}

//void StrictLogManager::courseChoice_ShowNewEgg() {
//    Json::Value it = basicLog("courseChoice_ShowNewEgg");
//
//    logJson(it);
//}

void StrictLogManager::courseChoice_ShowNewAnimal(const std::string& levelID) {
    Json::Value it = basicLog("courseChoice_ShowNewAnimal");

    it["0.levelID"] = levelID;
    logJson(it);
}

void StrictLogManager::courseChoice_TouchAnimal(const std::string& levelID) {
    Json::Value it = basicLog("courseChoice_TouchAnimal");
    
    it["0.levelID"] = levelID;
    logJson(it);
}

void StrictLogManager::dayChoice_Begin(const std::string& levelID) {
    Json::Value it = basicLog("dayChoice_Begin");
    
    it["0.levelID"] = levelID;
    logJson(it);
}

void StrictLogManager::dayChoice_End_Quit(const std::string& levelID) {
    Json::Value it = basicLog("dayChoice_End_Quit");

    it["0.levelID"] = levelID;
    logJson(it);
}

void StrictLogManager::dayChoice_End_Complete(const std::string& levelID) {
    Json::Value it = basicLog("dayChoice_End_Complete");

    it["0.levelID"] = levelID;
    logJson(it);
}

void StrictLogManager::dayChoice_OpenFreeChoiceMenu() {
    Json::Value it = basicLog("dayChoice_OpenFreeChoiceMenu");
    
    logJson(it);
}

void StrictLogManager::dayChoice_CloseFreeChoiceMenu() {
    Json::Value it = basicLog("dayChoice_CloseFreeChoiceMenu");
    
    logJson(it);
}

void StrictLogManager::dayChoice_OpenFreeChoiceLevelPopup(const std::string& gameName, int openCount, int targetCount) {
    Json::Value it = basicLog("dayChoice_OpenFreeChoiceLevelPopup");
    
    it["0.gameName"] = gameName;
    it["1.openCount"] = openCount;
    it["2.targetCount"] = targetCount;
    logJson(it);
}

void StrictLogManager::dayChoice_CloseFreeChoiceLevelPopup() {
    Json::Value it = basicLog("dayChoice_CloseFreeChoiceLevelPopup");
    
    logJson(it);
}

void StrictLogManager::dayChoice_ChooseFreeChoiceGame(const std::string& gameName, int gameLevel) {
    Json::Value it = basicLog("dayChoice_ChooseFreeChoiceGame");

    it["0.gameName"] = gameName;
    it["1.gameLevel"] = gameLevel;
    logJson(it);
}

void StrictLogManager::dailyGameChoice_Begin(const std::string& levelID, int dayID) {
    Json::Value it = basicLog("dailyGameChoice_Begin");
    
    it["0.levelID"] = levelID;
    it["1.dayID"] = dayID;
    logJson(it);
}

void StrictLogManager::dailyGameChoice_End_Quit(const std::string& levelID, int dayID) {
    Json::Value it = basicLog("dailyGameChoice_End_Quit");

    it["0.levelID"] = levelID;
    it["1.dayID"] = dayID;
    logJson(it);
}

void StrictLogManager::dailyGameChoice_End_Complete(const std::string& levelID, int dayID) {
    Json::Value it = basicLog("dailyGameChoice_End_Complete");
    
    it["0.levelID"] = levelID;
    it["1.dayID"] = dayID;
    logJson(it);
}

void StrictLogManager::dailyGameChoice_ChooseDailyGame(const std::string& levelID, int dayID, int choiceIndex) {
    Json::Value it = basicLog("dailyGameChoice_ChooseDailyGame");
    
    it["0.levelID"] = levelID;
    it["1.dayID"] = dayID;
    it["2.choiceIndex"] = choiceIndex;
    logJson(it);
}

void StrictLogManager::game_Begin(const std::string& gameName, int gameLevel) {
    Json::Value it = basicLog("game_Begin");

    it["0.gameName"] = gameName;
    it["1.gameLevel"] = gameLevel;
    
    // NB(xenosoz, 2017): Please consider reviving code fragment below (from the original place)
    //vl["class"] = UserManager::getInstance()->getClassId();
    //vl["student"] = UserManager::getInstance()->getStudentId();
    //vl["course"] = UserManager::getInstance()->getCourseId();
    
    logJson(it);
}

void StrictLogManager::game_End_Quit(const std::string& gameName, int gameLevel, double duration) {
    Json::Value it = basicLog("game_End_Quit");

    it["0.gameName"] = gameName;
    it["1.gameLevel"] = gameLevel;
    it["2.duration"] = duration;
    logJson(it);
}

void StrictLogManager::game_End_Complete(const std::string& gameName, int gameLevel, double duration, int result) {
    Json::Value it = basicLog("game_End_Complete");
    
    it["0.gameName"] = gameName;
    it["1.gameLevel"] = gameLevel;
    it["2.duration"] = duration;
    it["3.result"] = result;
    logJson(it);
}

void StrictLogManager::video_Begin(const std::string& videoName) {
    Json::Value it = basicLog("video_Begin");
    
    it["0.videoName"] = videoName;
    logJson(it);
}

void StrictLogManager::video_End_Quit(const std::string& videoName, double duration) {
    Json::Value it = basicLog("video_End_Quit");
    
    it["0.videoName"] = videoName;
    it["1.duration"] = duration;
    logJson(it);
}

void StrictLogManager::video_End_Complete(const std::string& videoName, double duration, int result) {
    Json::Value it = basicLog("video_End_Complete");

    it["0.videoName"] = videoName;
    it["1.duration"] = duration;
    it["2.result"] = result;
    logJson(it);
}

void StrictLogManager::book_Begin(const std::string& bookName) {
    Json::Value it = basicLog("book_Begin");
    
    it["0.bookName"] = bookName;
    logJson(it);
}

void StrictLogManager::book_End_Quit(const std::string& bookName, double duration) {
    Json::Value it = basicLog("book_End_Quit");
    
    it["0.bookName"] = bookName;
    it["1.duration"] = duration;
    logJson(it);
}

void StrictLogManager::book_End_Complete(const std::string& bookName, double duration, int result) {
    Json::Value it = basicLog("book_End_Complete");
    
    it["0.bookName"] = bookName;
    it["1.duration"] = duration;
    it["2.result"] = result;
    logJson(it);
}

void StrictLogManager::comprehension_Begin(const std::string& bookName, int problemSet) {
    Json::Value it = basicLog("comprehension_Begin");
    
    it["0.bookName"] = bookName;
    it["1.problemSet"] = problemSet;
    logJson(it);
}

void StrictLogManager::comprehension_End_Quit(const std::string& bookName, int problemSet, double duration) {
    Json::Value it = basicLog("comprehension_End_Quit");
    
    it["0.bookName"] = bookName;
    it["1.problemSet"] = problemSet;
    it["2.duration"] = duration;
    logJson(it);
}

void StrictLogManager::comprehension_End_Complete(const std::string& bookName, int problemSet, double duration, int result) {
    Json::Value it = basicLog("comprehension_End_Complete");
    
    it["0.bookName"] = bookName;
    it["1.problemSet"] = problemSet;
    it["2.duration"] = duration;
    it["3.result"] = result;
    logJson(it);
}

void StrictLogManager::comprehension_Solve(const std::string& bookName, int problemSet, int problemIndex) {
    Json::Value it = basicLog("comprehension_Solve");
    
    it["0.bookName"] = bookName;
    it["1.problemSet"] = problemSet;
    it["2.problemIndex"] = problemIndex;
    logJson(it);
}


void StrictLogManager::eggQuiz_CorrectAnswer(int problemIndex, const std::string& answer, int answerIndex) {
    Json::Value it = basicLog("eggQuiz_CorrectAnswer");
    
    it["0.problemIndex"] = problemIndex;
    it["1.answer"] = answer;
    it["2.answerIndex"] = answerIndex;
    logJson(it);
}

void StrictLogManager::eggQuiz_WrongAnswer(int problemIndex, const std::string& correctAnswer, const std::string& myAnswer, int answerIndex) {
    Json::Value it = basicLog("eggQuiz_WrongAnswer");
    
    it["0.problemIndex"] = problemIndex;
    it["1.correctAnswer"] = correctAnswer;
    it["2.myAnswer"] = myAnswer;
    it["3.answerIndex"] = answerIndex;
    logJson(it);
}

void StrictLogManager::starStat_UpdateStarsInKitKitSchool(int oldStars, int newStars) {
    Json::Value it = basicLog("starStat_UpdateStarInKitKitSchool");
    
    it["0.oldStars"] = oldStars;
    it["1.newStars"] = newStars;
    logJson(it);
}


StrictLogManager* StrictLogManager::shared()
{
    static StrictLogManager* _shared = nullptr;
    
    if (!_shared) {
        _shared = new (std::nothrow) StrictLogManager();
        CCASSERT(_shared, "FATAL: Not enough memory in StrictLogManger::shared()");
    }
    
    return _shared;
}
