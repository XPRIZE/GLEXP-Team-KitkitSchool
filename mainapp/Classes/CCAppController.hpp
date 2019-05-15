//
//  CCAppController.hpp
//  KitkitSchool
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



    
    bool gameExists(std::string gameName);
    
    
    std::function<Scene*(void)> getGameCreator(std::string gameName, int level, std::string param="");
    
    void startGameWithLevelChoice(std::string gameName);
    bool startGame(std::string gameName, int level, std::string param="", bool checkOnly = false);
    void startFreeChoiceGame(std::string gameName, int level);
    void startCurriculumGame(std::string levelID, int day, int gameIndex);
    void startBookScene(std::string bookFolder, bool replaceParent = false, bool checkCompleteCondition = true);
    void startBookWithQuizScene(std::string bookFolder); // book-with-comprehension

    void startComprehensionScene(std::string bookFolder, int set, bool replaceParent = false,  bool checkCompleteCondition = true);
	void startVideoScene(std::string filename, bool replaceParent = false);
    
    void startQuiz(std::string classroom, int studentNumber, std::string courseKind);
   // void startEggQuiz(char category, int categoryLevel, bool isPreTest, std::function<void(bool)> callback);
    
    void handleGameQuit(bool bImmediately = false);
    void handleGameComplete(int result);
    
    bool isDebug();

    
private:
    PlayTimer* _playTimer;
    std::string _currentGame;
    std::string _currentParam;
    int _currentLevel;
    bool _isFreeChoice;
    bool _allowSkipTutorial;
    
    
    std::string _currentCurrLevelID;
    int _currentCurrDay;
    int _currentCurrGameIndex;
    
    
    
};

#endif /* CCAppController_hpp */
