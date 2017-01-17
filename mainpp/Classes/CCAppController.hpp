//
//  CCAppController.hpp
//  enumaXprize
//
//  Created by Gunho Lee on 11/1/16.
//
//

#ifndef CCAppController_hpp
#define CCAppController_hpp

#include <stdio.h>
#include <string>
#include <functional>

#include "Games/Books/BookView.hpp"
#include "Utils/PlayTimer.hpp"

class CCAppController {
public:

    static CCAppController *sharedAppController();
    
    CCAppController();
    ~CCAppController();
    
    
    PlayTimer* getPlayTimer() { return _playTimer; }


    BookView* createBookView(std::string bookName);

    
    bool gameExists(std::string gameName);
    
    void startGameWithLevelChoice(std::string gameName);
    bool startGame(std::string gameName, int level, bool checkOnly = false);
    void startCurriculumGame(std::string levelID, int day, int gameIndex);
    void startBookScene(std::string bookFolder, bool replaceParent = false);
    void startComprehensionScene(std::string bookFolder, int set, bool replaceParent = false);
    
    void startQuiz(std::string classroom, int studentNumber, std::string courseKind);
    void startEggQuiz(char category, int categoryLevel, bool isPreTest, std::function<void(bool)> callback);
    
    
    void handleGameQuit();
    void handleGameComplete(int result);
    
    bool isDebug();
    
    
private:
    PlayTimer* _playTimer;
    std::string _currentGame;
    int _currentLevel;
    
    std::string _currentCurrLevelID;
    int _currentCurrDay;
    int _currentCurrGameIndex;
    
    
    
};

#endif /* CCAppController_hpp */
