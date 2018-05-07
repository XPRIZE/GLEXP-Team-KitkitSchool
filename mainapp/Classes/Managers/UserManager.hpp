//
//  UserManager.hpp
//  KitkitSchool
//
//  Created by Seokkwon Lee on 11/4/16.
//
//

#ifndef UserManager_hpp
#define UserManager_hpp

#include <stdio.h>
#include "3rdParty/json/json.h"


#include <string>
#include <vector>

using namespace std;

enum class PretestProgressType {
    required,
    pass,
    fail,
    finish
};

class UserManager {
    
    
    static UserManager* _instance;
public:
    static UserManager* getInstance();
    void init();
    
    bool isDebugMode() { return _debugMode; }
    void setDebugMode(bool isDebug) { _debugMode = isDebug; }
    
    bool isGameTestingMode() { return _gameTestingMode; }
    void setGameTestingMode(bool isTesting) { _gameTestingMode = isTesting; }
    
    bool isResetAllowed() { return _allowReset; }
    
    // First Official Version
    void resetStatus();
    void clearStatus();
    


    void flushData();
    
    string getCurrentLevelIDKey();
    string getCurrentLevelID();
    void setCurrentLevelID(string levelID);
    
    string getCurrentDayKey(string levelID);
    int getCurrentDay(string levelID);
    void setCurrentDay(string levelID, int day);
 
    void setPlayingDay(int day) { _playingDay = day; }
    int getPlayingDay() { return _playingDay; }
    
    string getLevelOpenKey(string levelID);
    bool isLevelOpen(string levelID);
    void setLevelOpen(string levelID, bool isOpen = true);
    bool isLevelCleared(string levelID);
    
    PretestProgressType getPretestProgressType(string levelID);
    void setPretestProgressType(string levelID, PretestProgressType type);
    
    string getDayClearedKey(string levelID, int day);
    bool isDayCleared(string levelID, int day);
    void setDayCleared(string levelID, int day, bool isCleared = true);
    
    bool checkIfNextDayIsAvailable(string levelID, int day);
    void clearDayProgress(string levelID, int day);
    
    
    
    
    int numDayCleared(string levelID);
    float ratioDayCleared(string levelID);
    
    string getGameClearedKey(string levelID, int day, int gameIndex);
    bool isGameCleared(string levelID, int day, int gameIndex);
    void setGameCleared(string levelID, int day, int gameIndex, bool isCleared = true);
    
    //string getProgressDataKey();
    //Json::Value getProgressData();
    //void setProgressData(Json::Value json);
    //std::string getKey();
    
    void refreshUsername();
    const string getCurrentUsername();
    void updateStars(int numStars);
    int getStars();
    
    string getFinishTutorialKey();
    void finishTutorial();
    bool hasPlayedMenuTutorial();
    
    
private:
    
    string _currentLevelID;
    int _currentDay;
    map<string, bool> _levelOpenedMap;
    map<pair<string, int>, bool> _dayClearedMap;
    map<tuple<string, int, int>, bool> _gameClearedMap;
    
    int _playingDay;
    bool _debugMode;
    bool _gameTestingMode;
    bool _allowReset;
    
    string _userName;
    
    
    
    
    
public:
    // TZ test
    //void setCurrentUser(std::string classId, std::string studentId, std::string courseId);
    //std::string getClassId();
    //std::string getStudentId();
    //std::string getCourseId();
    
    //void setCompletedGame(std::string gameName, int level);
    //bool isCompletedGame(std::string gameName, int level);
    
    //void setBirdStatus(int birdType, int status);
    //int getBirdStatus(int birdType);
    
    
public:
    const std::string getAppVersion();
    
    void sendAppToBack();
    
};

#endif /* UserManager_hpp */
