//
//  UserManager.cpp
//  enumaXprize
//
//  Created by Seokkwon Lee on 11/4/16.
//
//

#include "UserManager.hpp"
#include "CurriculumManager.hpp"

#include "cocos2d.h"
#include "Utils/TodoUtil.h"

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
#include <jni.h>
#include "platform/android/jni/JniHelper.h"
#endif


USING_NS_CC;
using namespace std;


UserManager* UserManager::_instance = 0;

UserManager* UserManager::getInstance()
{
    if(!_instance) {
        _instance = new UserManager();
        _instance->init();
    }
    return _instance;
}


void UserManager::init()
{
    auto lastVersion = UserDefault::getInstance()->getStringForKey("lastAppVersion");
    auto currentVersion = getAppVersion();
    if (lastVersion!=currentVersion) {
        
        if (currentVersion=="1.0.2") {
            resetStatus();
        }
        UserDefault::getInstance()->setStringForKey("lastAppVersion", currentVersion);
        
    }
    
    _debugMode = false;
    _gameTestingMode = false;
    
    
    
}



void UserManager::resetStatus()
{
    
    for (auto l : CurriculumManager::getInstance()->levels) {
        auto levelID = l.first;
        UserDefault::getInstance()->setBoolForKey(getLevelOpenKey(levelID).c_str(), false);
        
        for (auto d : l.second.days) {
            UserDefault::getInstance()->setBoolForKey(getDayClearedKey(levelID, d.dayOrder).c_str(), false);
            
            for (int i=0; i<d.games.size(); i++) {
                UserDefault::getInstance()->setBoolForKey(getGameClearedKey(levelID, d.dayOrder, i).c_str(), false);
            }
        }
    }

    setCurrentLevelID("");
    setCurrentDay(0);
    
    UserDefault::getInstance()->flush();

    _levelOpenedMap.clear();
    _dayClearedMap.clear();
    _gameClearedMap.clear();

}

string UserManager::getCurrentLevelID() {
    if (_currentLevelID.length()==0) {
        _currentLevelID = UserDefault::getInstance()->getStringForKey("currentLevelID", "");
    }
    return _currentLevelID;
}

void UserManager::setCurrentLevelID(string levelID)
{
    if (_currentLevelID!=levelID) {
        _currentLevelID = levelID;
        UserDefault::getInstance()->setStringForKey("currentLevelID", levelID);
        UserDefault::getInstance()->flush();
    }
}

int UserManager::getCurrentDay()
{
    if (_currentDay<=0) {
        _currentDay = UserDefault::getInstance()->getIntegerForKey("currentDay", 1);
    }
    return _currentDay;
}

void UserManager::setCurrentDay(int day)
{
    if (_currentDay!=day) {
        _currentDay = day;
        UserDefault::getInstance()->setIntegerForKey("currentDay", day);
        UserDefault::getInstance()->flush();
    }
}

string UserManager::getLevelOpenKey(string levelID)
{
    return "levelOpen_"+levelID;
}
bool UserManager::isLevelOpen(string levelID)
{
    auto it = _levelOpenedMap.find(levelID);
    bool opened = false;
    
    if (it==_levelOpenedMap.end()) {
        opened = UserDefault::getInstance()->getBoolForKey(getLevelOpenKey(levelID).c_str(), false);
        _levelOpenedMap[levelID] = opened;
    }else {
        opened = it->second;
    }
    
    return opened;
}


void UserManager::setLevelOpen(string levelID, bool isOpen)
{
    _levelOpenedMap[levelID] = isOpen;
    UserDefault::getInstance()->setBoolForKey(getLevelOpenKey(levelID).c_str(), isOpen);
    UserDefault::getInstance()->flush();
    
}

bool UserManager::isLevelCleared(string levelID)
{
    auto cur = CurriculumManager::getInstance()->findCurriculum(levelID);
    int num = 0;
    for (auto d : cur->days) {
        if (isDayCleared(levelID, d.dayOrder)) num++;
    }
    return (num==cur->days.size());
    
}

string UserManager::getDayClearedKey(string levelID, int day)
{
    return "dayCleared_"+levelID+"_"+TodoUtil::itos(day);
}

bool UserManager::isDayCleared(string levelID, int day)
{
    auto key = make_pair(levelID, day);
    auto it = _dayClearedMap.find(key);
    bool cleared = false;
    if (it==_dayClearedMap.end()) {
        cleared = UserDefault::getInstance()->getBoolForKey(getDayClearedKey(levelID, day).c_str(), false);
        _dayClearedMap[key] = cleared;
    } else {
        cleared = it->second;
    }
    return cleared;
}


void UserManager::setDayCleared(string levelID, int day, bool isCleared)
{
    auto key = make_pair(levelID, day);
    _dayClearedMap[key] = isCleared;
    UserDefault::getInstance()->setBoolForKey(getDayClearedKey(levelID, day).c_str(), isCleared);
    UserDefault::getInstance()->flush();
}

int UserManager::numDayCleared(string levelID)
{
    auto cur = CurriculumManager::getInstance()->findCurriculum(levelID);
    int num = 0;
    for (auto d : cur->days) {
        if (isDayCleared(levelID, d.dayOrder)) num++;
    }
    return num;
    
    
}

float UserManager::ratioDayCleared(string levelID)
{
    auto cur = CurriculumManager::getInstance()->findCurriculum(levelID);
    if (cur->days.size()<=1) return 1.0;
    
    int num = 0;
    for (auto d : cur->days) {
        if (isDayCleared(levelID, d.dayOrder)) num++;
    }
    
    return ((float)num / (float)(cur->days.size()-1));
}

string UserManager::getGameClearedKey(string levelID, int day, int gameIndex)
{
    return "gameCleared_"+levelID+"_"+TodoUtil::itos(day)+"_"+TodoUtil::itos(gameIndex);
}


bool UserManager::isGameCleared(string levelID, int day, int gameIndex)
{
    auto key = make_tuple(levelID, day, gameIndex);
    auto it = _gameClearedMap.find(key);
    bool cleared = false;
    if (it==_gameClearedMap.end()) {
        cleared = UserDefault::getInstance()->getBoolForKey(getGameClearedKey(levelID, day, gameIndex).c_str(), false);
        _gameClearedMap[key] = cleared;
    }else {
        cleared = it->second;
    }
    return cleared;
}

void UserManager::setGameCleared(string levelID, int day, int gameIndex, bool isCleared)
{
    auto key = make_tuple(levelID, day, gameIndex);
    _gameClearedMap[key] = isCleared;
    UserDefault::getInstance()->setBoolForKey(getGameClearedKey(levelID, day, gameIndex).c_str(), isCleared);
    UserDefault::getInstance()->flush();
}





void UserManager::setCurrentUser(std::string classId, std::string studentId, std::string courseId)
{
    UserDefault::getInstance()->setStringForKey("classId", classId);
    UserDefault::getInstance()->setStringForKey("studentId", studentId);
    UserDefault::getInstance()->setStringForKey("courseId", courseId);
    UserDefault::getInstance()->flush();
}


std::string UserManager::getClassId()
{
    return UserDefault::getInstance()->getStringForKey("classId");
}

std::string UserManager::getStudentId()
{
    return UserDefault::getInstance()->getStringForKey("studentId");
}

std::string UserManager::getCourseId()
{
    return UserDefault::getInstance()->getStringForKey("courseId");
}

void UserManager::setCompletedGame(std::string gameName, int level)
{
    std::string key = getKey();
    Json::Value json = getProgressData();
    
    json[key][gameName.append("_").append(TodoUtil::itos(level))] = "1";
    
    setProgressData(json);
}

bool UserManager::isCompletedGame(std::string gameName, int level)
{
    std::string key = getKey();
    Json::Value json = getProgressData();
    
    Json::Value result = json[key][gameName.append("_").append(TodoUtil::itos(level))];
    
    return !result.empty();
}

Json::Value UserManager::getProgressData()
{
    std::string progressString = UserDefault::getInstance()->getStringForKey("progressData");
    
    Json::Value progressJson;
    
    if(progressString != ""){
        Json::Reader reader;
        reader.parse(progressString, progressJson);
    }
    
    return progressJson;
}

void UserManager::setProgressData(Json::Value json)
{
    Json::FastWriter writer;
    std::string progressString = writer.write(json);
    
    UserDefault::getInstance()->setStringForKey("progressData", progressString);
}

std::string UserManager::getKey()
{
    return getClassId().append("_").append(getStudentId()).append("_").append(getCourseId());
}




void UserManager::setBirdStatus(int birdType, int status)
{
    std::string key = StringUtils::format("%s_BirdStatus_%d", getKey().c_str(), birdType);
    UserDefault::getInstance()->setIntegerForKey(key.c_str(), status);
    UserDefault::getInstance()->flush();
}
int UserManager::getBirdStatus(int birdType)
{
    std::string key = StringUtils::format("%s_BirdStatus_%d", getKey().c_str(), birdType);
    return UserDefault::getInstance()->getIntegerForKey(key.c_str(),0);
}


#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
const std::string UserManager::getAppVersion()
{
    JniMethodInfo t;
    std:: string a("");
    if (JniHelper::getStaticMethodInfo(t, "org/cocos2dx/cpp/AppActivity", "getAppVersion", "()Ljava/lang/String;"))
    {
        jstring jstr = (jstring)t.env->CallStaticObjectMethod(t.classID, t.methodID);
        a = JniHelper::jstring2string(jstr);
        t.env->DeleteLocalRef(jstr);
        return a;
    }
    return "";
}
#endif


void UserManager::sendAppToBack()
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    JniMethodInfo t;

    bool result = JniHelper::getStaticMethodInfo(t, "org/cocos2dx/cpp/AppActivity", "sendToBack", "()V");
    if (result)
    {
        t.env->CallStaticVoidMethod(t.classID, t.methodID);
        t.env->DeleteLocalRef(t.classID);
    }
#else
    exit(0);
#endif
}


