//
//  CurriculumManager.hpp
//  KitkitSchool
//
//  Created by Gunho Lee on 12/23/16.
//
//

#ifndef CurriculumManager_hpp
#define CurriculumManager_hpp

#include <string>
#include <vector>
#include <map>

using namespace std;

struct LevelCurriculum;

class CurriculumManager {
    
    
    static CurriculumManager* _instance;
public:

    static CurriculumManager* getInstance();
    void init();

    string makeLevelID(string lang, char category, int level);
    
    LevelCurriculum* findCurriculum(char category, int level);
    LevelCurriculum* findCurriculum(string levelID);
    
    map<string, LevelCurriculum> levels;
    
    void loadData();
    

};

struct GameDescription
{
    string gameName;
    int gameLevel;
    string gameParameter;
    
    // ephemeral
    int appearIndex;
};


struct DayCurriculum {
    int dayOrder;
    int numGames;
    bool isEggQuiz;
    bool isMiniQuiz;
    vector<GameDescription> games;
    
};

struct LevelTestDescription {
    
};

class LevelCurriculum {
public:
    string levelID;
    
    string lang;
    string levelTitle;
    char category; // M for Math, L for Literacy
    int categoryLevel; // 0 for Pre-K, 1~
    int numDays;
    
    LevelTestDescription levelTest;
    
    vector<DayCurriculum> days;
    
    DayCurriculum* getDayCurriculum(int day) {
        if (day<=0 || day>days.size()) return nullptr;
        return &(days[day-1]);
    }
    

};


#endif /* CurriculumManager_hpp */
