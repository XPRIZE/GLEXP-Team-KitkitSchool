//
//  UserManager.cpp
//  KitkitSchool
//
//  Created by Seokkwon Lee on 11/4/16.
//
//

#include "UserManager.hpp"
#include "CurriculumManager.hpp"
#include "StrictLogManager.h"

#include "cocos2d.h"
#include "Utils/TodoUtil.h"

#include "Menu/CoopScene.hpp"

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

#if (CC_TARGET_PLATFORM == CC_PLATFORM_MAC)
    _debugMode = true;
    _gameTestingMode = true;
#else
    _debugMode = false;
    _gameTestingMode = false;
    
#endif
    _worksheetTestMode = false;
    _allowReset = false;
    _userName = "";
    
    refreshUsername();



}

void UserManager::clearStatus()
{
    _currentLevelID = "";

    _currentDay = 0;
    _levelOpenedMap.clear();
    _dayClearedMap.clear();
    _gameClearedMap.clear();

}


void UserManager::resetStatus()
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    JniHelper::callStaticVoidMethod("org/cocos2dx/cpp/AppActivity", "deleteAllUserDefault");
#else
    for (auto l : CurriculumManager::getInstance()->levels) {
        auto levelID = l.first;
        UserDefault::getInstance()->setBoolForKey(getLevelOpenKey(levelID).c_str(), false);
        
        for (auto d : l.second.days) {
            UserDefault::getInstance()->setBoolForKey(getDayClearedKey(levelID, d.dayOrder).c_str(), false);
            
            for (int i=0; i<d.games.size(); i++) {
                UserDefault::getInstance()->setBoolForKey(getGameClearedKey(levelID, d.dayOrder, i).c_str(), false);
            }
            
            if (l.second.categoryLevel == CoopScene::LEVEL_SPECIAL_COURSE) {
                UserDefault::getInstance()->setIntegerForKey(getSpecialCourseCurrentProgressKey(levelID, d.dayOrder).c_str(), 0);
            }
        }
        
        if (l.second.categoryLevel == CoopScene::LEVEL_FISH_PRESENT) {
            UserDefault::getInstance()->setIntegerForKey(getFishPresentCurrentProgressLevelKey(levelID).c_str(), 0);
            for (int i = 0; i < CoopScene::LEVEL_COUNT_REGULAR_EGG; ++i) {
                UserDefault::getInstance()->setIntegerForKey(getFishPresentCurrentProgressIndexKey(levelID, i).c_str(), 0);
            }
            UserDefault::getInstance()->setBoolForKey(getFishPresentEnableKey(levelID).c_str(), true);
        }
        
        string key = getCurrentDayKey(levelID);
        UserDefault::getInstance()->setIntegerForKey(key.c_str(), 0);
        
        setPretestProgressType(levelID, PretestProgressType::required);
    }
    setGuideCoopStatus(guideCoopType::visitFirst);
    setGuideDayStatus(guideDayType::touchFirst);
    UserDefault::getInstance()->setBoolForKey(getFinishTutorialKey().c_str(), false);
    UserDefault::getInstance()->setBoolForKey(getSpecialCourseLightOnKey('L').c_str(), false);
    UserDefault::getInstance()->setBoolForKey(getSpecialCourseLightOnKey('M').c_str(), false);
    UserDefault::getInstance()->setBoolForKey(getFishPresentLightOnKey('L').c_str(), false);
    UserDefault::getInstance()->setBoolForKey(getFishPresentLightOnKey('M').c_str(), false);
    UserDefault::getInstance()->setBoolForKey(getWelcomeVideoPlayedKey().c_str(), false);
    
#endif
    _currentDay = 0;
    setCurrentLevelID("");
    UserDefault::getInstance()->flush();

    _levelOpenedMap.clear();
    _dayClearedMap.clear();
    _gameClearedMap.clear();
    
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    JniHelper::callStaticVoidMethod("org/cocos2dx/cpp/AppActivity", "setUnlockFishBowl", false);
#endif
}

string UserManager::getWelcomeVideoPlayedKey()
{
    return _userName + "_welcomeVideoPlayed";
}

string UserManager::getCurrentLevelIDKey()
{
    return _userName + "_currentLevelID";
}


string UserManager::getCurrentLevelID() {
    if (_currentLevelID.length()==0) {
        _currentLevelID = UserDefault::getInstance()->getStringForKey(getCurrentLevelIDKey().c_str(), "");
    }
    return _currentLevelID;
}

void UserManager::setCurrentLevelID(string levelID)
{
    if (_currentLevelID!=levelID) {
        _currentLevelID = levelID;
        _currentDay = 0;
        UserDefault::getInstance()->setStringForKey(getCurrentLevelIDKey().c_str(), levelID);
        UserDefault::getInstance()->flush();
    }
}

string UserManager::getCurrentDayKey(string levelID)
{
    return _userName+"_currentDay_"+levelID;

}


int UserManager::getCurrentDay(string levelID)
{
    if (levelID==_currentLevelID && _currentDay>0) {
        return _currentDay;
    }

    string key = getCurrentDayKey(levelID);
    _currentDay = UserDefault::getInstance()->getIntegerForKey(key.c_str(), 1);

    return _currentDay;
}

void UserManager::setCurrentDay(string levelID, int day)
{
    if (_currentDay!=day) {

//        if (_currentDay>0 && !isDayCleared(levelID, _currentDay)) {
//            clearDayProgress(levelID, _currentDay);
//        }

        _currentDay = day;
        if (levelID.length()>0) {
            string key = getCurrentDayKey(levelID);
            UserDefault::getInstance()->setIntegerForKey(key.c_str(), day);
            UserDefault::getInstance()->flush();
        }
    }
}

string UserManager::getLevelOpenKey(string levelID)
{
    return _userName + "_levelOpen_"+levelID;
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
    if (cur->days.size() == 0) {
        return false;
    }
    //int num = 0;

    auto lastDayCur = cur->days.back();

    return (isDayCleared(levelID, lastDayCur.dayOrder));



    /*
    for (auto d : cur->days) {
        if (isDayCleared(levelID, d.dayOrder)) num++;
    }
    return (num==cur->days.size());
     */


}

PretestProgressType UserManager::getPretestProgressType(string levelID) {
    return PretestProgressType::finish;
}

void UserManager::setPretestProgressType(string levelID, PretestProgressType type) {
    auto cur = CurriculumManager::getInstance()->findCurriculum(levelID);
    if (cur->categoryLevel > 0 && cur->categoryLevel <= 4) {
        UserDefault::getInstance()->setIntegerForKey((_userName+"pretest_progress_"+levelID).c_str(), (int)type);
    }
    return;
}

guideCoopType UserManager::getGuideCoopStatus() {
    return (guideCoopType)UserDefault::getInstance()->getIntegerForKey((_userName+"guide_coop_flag").c_str(), (int)guideCoopType::visitFirst);
}

void UserManager::setGuideCoopStatus(guideCoopType type) {
    UserDefault::getInstance()->setIntegerForKey((_userName+"guide_coop_flag").c_str(), (int)type);

}

guideDayType UserManager::getGuideDayStatus() {
    return (guideDayType)UserDefault::getInstance()->getIntegerForKey((_userName+"guide_day_flag").c_str(), (int)guideDayType::touchFirst);
}

void UserManager::setGuideDayStatus(guideDayType type) {
    UserDefault::getInstance()->setIntegerForKey((_userName+"guide_day_flag").c_str(), (int)type);
}

string UserManager::getDayClearedKey(string levelID, int day)
{
    return _userName + "_dayCleared_"+levelID+"_"+TodoUtil::itos(day);
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

bool UserManager::checkIfNextDayIsAvailable(string levelID, int day)
{
    int clearedNumOfGamesInDay = 0, numOfGamesInDay = 0;

    auto cur = CurriculumManager::getInstance()->findCurriculum(levelID);
    auto dayCurr = cur->getDayCurriculum(day);
    if (!dayCurr) return false;

    numOfGamesInDay = dayCurr->numGames;

    for (int num = 0; num <= numOfGamesInDay; num++) {
        if(UserManager::getInstance()->isGameCleared(levelID, _currentDay, num)) clearedNumOfGamesInDay++;
    }
    if (clearedNumOfGamesInDay >= numOfGamesInDay-1) return true;

    return false;
}

void UserManager::clearDayProgress(string levelID, int day)
{
    auto cur = CurriculumManager::getInstance()->findCurriculum(levelID);
    auto dayCurr = cur->getDayCurriculum(day);
    if (!dayCurr) return;

    for (int i=0; i<dayCurr->numGames; i++) {
        UserManager::getInstance()->setGameCleared(levelID, day, i, false);
    }


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
    return _userName + "_gameCleared_"+levelID+"_"+TodoUtil::itos(day)+"_"+TodoUtil::itos(gameIndex);
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
    ///////////////////////////////
    // store the number of games cleared
    string gamesClearedInTotalKey = _userName + "_gamesClearedInTotal_" + levelID.substr(0, 7);
    bool presentGameCleared = isGameCleared(levelID, day, gameIndex);
    int gamesClearedInTotal = UserDefault::getInstance()->getIntegerForKey(gamesClearedInTotalKey.c_str(), 0);
    if (!presentGameCleared) {
        gamesClearedInTotal++;
        UserDefault::getInstance()->setIntegerForKey(gamesClearedInTotalKey.c_str(), gamesClearedInTotal);
    }
    ///////////////////////////////
    auto key = make_tuple(levelID, day, gameIndex);
    _gameClearedMap[key] = isCleared;
    UserDefault::getInstance()->setBoolForKey(getGameClearedKey(levelID, day, gameIndex).c_str(), isCleared);
    UserDefault::getInstance()->flush();
}

string UserManager::getSpecialCourseCurrentProgressKey(string levelID, int day) {
    return _userName + "_SCCurrentProgress_"+levelID+"_"+TodoUtil::itos(day);
}

void UserManager::setSpecialCourseCurrentProgress(string levelID, int day, int gameIndex) {
    UserDefault::getInstance()->setIntegerForKey(getSpecialCourseCurrentProgressKey(levelID, day).c_str(), gameIndex);
    UserDefault::getInstance()->flush();
}

int UserManager::getSpecialCourseCurrentProgress(string levelID, int day) {
    return UserDefault::getInstance()->getIntegerForKey(getSpecialCourseCurrentProgressKey(levelID, day).c_str());
}

string UserManager::getSpecialCourseLightOnKey(char course) {
    return _userName + "_SCLight_" + course;
}

void UserManager::setSpecialCourseLightOn(char course) {
    UserDefault::getInstance()->setBoolForKey(getSpecialCourseLightOnKey(course).c_str(), true);
    UserDefault::getInstance()->flush();
}

bool UserManager::getSpecialCourseLightOn(char course) {
    return UserDefault::getInstance()->getBoolForKey(getSpecialCourseLightOnKey(course).c_str());
}

string UserManager::getFishPresentCurrentProgressLevelKey(string levelID) {
    return _userName + "_FPCurrentProgressLevel_" + levelID;
}

void UserManager::setFishPresentCurrentProgressLevel(string levelID, int levelIndex) {
    if (_gameTestingMode) {
        return;
    }
    UserDefault::getInstance()->setIntegerForKey(getFishPresentCurrentProgressLevelKey(levelID).c_str(), levelIndex);
    UserDefault::getInstance()->flush();
}

int UserManager::getFishPresentCurrentProgressLevel(string levelID) {
    return UserDefault::getInstance()->getIntegerForKey(getFishPresentCurrentProgressLevelKey(levelID).c_str());
}

string UserManager::getFishPresentCurrentProgressIndexKey(string levelID, int levelIndex) {
    return _userName + "_FPCurrentProgressIndex_" + levelID + "_" + TodoUtil::itos(levelIndex);
}

void UserManager::setFishPresentCurrentProgressIndex(string levelID, int levelIndex, int index) {
    if (_gameTestingMode) {
        return;
    }

    UserDefault::getInstance()->setIntegerForKey(getFishPresentCurrentProgressIndexKey(levelID, levelIndex).c_str(), index);
    UserDefault::getInstance()->flush();
}

string UserManager::getFishPresentLightOnKey(char course) {
    return _userName + "_FPLight_" + course;
}

void UserManager::setFishPresentLightOn(char course) {
    UserDefault::getInstance()->setBoolForKey(getFishPresentLightOnKey(course).c_str(), true);
    UserDefault::getInstance()->flush();
}

bool UserManager::getFishPresentLightOn(char course) {
    return UserDefault::getInstance()->getBoolForKey(getFishPresentLightOnKey(course).c_str());
}

int UserManager::getFishPresentCurrentProgressIndex(string levelID, int levelIndex) {
    return UserDefault::getInstance()->getIntegerForKey(getFishPresentCurrentProgressIndexKey(levelID, levelIndex).c_str());
}

string UserManager::getFishPresentEnableKey(string levelID) {
    return _userName + "_FPEnable_" + levelID;
}

void UserManager::setFishPresentEnable(string levelID, bool isEnable) {
    UserDefault::getInstance()->setBoolForKey(getFishPresentEnableKey(levelID).c_str(), isEnable);
    UserDefault::getInstance()->flush();
}

bool UserManager::isFishPresentEnable(string levelID) {
    return UserDefault::getInstance()->getBoolForKey(getFishPresentEnableKey(levelID).c_str(), true);
}

/*
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
    //std::string key = getKey();
    Json::Value json = getProgressData();

    json[_userName][gameName.append("_").append(TodoUtil::itos(level))] = "1";

    setProgressData(json);
}

bool UserManager::isCompletedGame(std::string gameName, int level)
{
    //std::string key = getKey();
    Json::Value json = getProgressData();

    Json::Value result = json[_userName][gameName.append("_").append(TodoUtil::itos(level))];

    return !result.empty();
}


string UserManager::getProgressDataKey()
{
        return _userName + "_progressData";
}


Json::Value UserManager::getProgressData()
{
    std::string progressString = UserDefault::getInstance()->getStringForKey(getProgressDataKey().c_str());

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

    UserDefault::getInstance()->setStringForKey(getProgressDataKey().c_str(), progressString);
}
 */

/*
 std::string UserManager::getKey()
 {
 return getClassId().append("_").append(getStudentId()).append("_").append(getCourseId());
 }
 */



/*
void UserManager::setBirdStatus(int birdType, int status)
{
    std::string key = StringUtils::format("%s_BirdStatus_%d", _userName.c_str(), birdType);
    UserDefault::getInstance()->setIntegerForKey(key.c_str(), status);
    UserDefault::getInstance()->flush();
}
int UserManager::getBirdStatus(int birdType)
{
    std::string key = StringUtils::format("%s_BirdStatus_%d", _userName.c_str(), birdType);
    return UserDefault::getInstance()->getIntegerForKey(key.c_str(),0);
}
*/

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID || CC_TARGET_PLATFORM == CC_PLATFORM_WINRT)
const std::string UserManager::getAppVersion()
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    JniMethodInfo t;
    std:: string a("");
    if (JniHelper::getStaticMethodInfo(t, "org/cocos2dx/cpp/AppActivity", "getAppVersion", "()Ljava/lang/String;"))
    {
        jstring jstr = (jstring)t.env->CallStaticObjectMethod(t.classID, t.methodID);
        a = JniHelper::jstring2string(jstr);
        t.env->DeleteLocalRef(jstr);
        return a;
    }
#endif
    return "";
}
#endif



void UserManager::refreshUsername()
{
    auto user = getCurrentUsername();
    if (user!=_userName) {
        clearStatus();
    }
    _userName = getCurrentUsername();
}

const std::string UserManager::getCurrentUsername()
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    JniMethodInfo t;
    std:: string a("");
    if (JniHelper::getStaticMethodInfo(t, "org/cocos2dx/cpp/AppActivity", "getCurrentUsername", "()Ljava/lang/String;"))
    {
        jstring jstr = (jstring)t.env->CallStaticObjectMethod(t.classID, t.methodID);
        a = JniHelper::jstring2string(jstr);
        t.env->DeleteLocalRef(jstr);
        return a;
    }
    return "username";
#else  // iOS
    return "username";
#endif

}



void UserManager::updateStars(int numStars)
{
    int oldStars = getStars();
    StrictLogManager::shared()->starStat_UpdateStarsInKitKitSchool(oldStars, numStars);

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    JniMethodInfo t;
    if (JniHelper::getStaticMethodInfo(t, "org/cocos2dx/cpp/AppActivity", "updateStars", "(I)V"))
    {
        t.env->CallStaticVoidMethod(t.classID, t.methodID, numStars);
        t.env->DeleteLocalRef(t.classID);
    }
#else // iOS
    UserDefault::getInstance()->setIntegerForKey("coinNum", numStars);
    UserDefault::getInstance()->flush();
#endif
}



int UserManager::getStars()
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    JniMethodInfo t;
    jint a;
    if (JniHelper::getStaticMethodInfo(t, "org/cocos2dx/cpp/AppActivity", "getStars", "()I"))
    {
        a = t.env->CallStaticIntMethod(t.classID, t.methodID);
        t.env->DeleteLocalRef(t.classID);
        int ret = (int)a;
        return ret;
    }
    return 0;
#else // iOS
    auto coins = UserDefault::getInstance()->getIntegerForKey("coinNum", 0);
    return coins;

#endif

}


string UserManager::getFinishTutorialKey()
{
    return _userName + "hasPlayedMenuTutorial";
}

void UserManager::finishTutorial()
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)

    JniMethodInfo t;
    if (JniHelper::getStaticMethodInfo(t, "org/cocos2dx/cpp/AppActivity", "finishTutorial", "()V"))
    {
        t.env->CallStaticVoidMethod(t.classID, t.methodID);
        t.env->DeleteLocalRef(t.classID);
    }
#endif
    UserDefault::getInstance()->setBoolForKey(getFinishTutorialKey().c_str(), true);
    UserDefault::getInstance()->flush();

}

bool UserManager::hasPlayedMenuTutorial()
{
    auto ret = UserDefault::getInstance()->getBoolForKey(getFinishTutorialKey().c_str(), false);
    return ret;
}

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
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_WINRT)
	auto uri = ref new Windows::Foundation::Uri("kitkitlauncher:");
	concurrency::create_task(Windows::System::Launcher::LaunchUriAsync(uri)).then([](bool launchResult)
	{

	});

#else
    exit(0);
#endif
}
