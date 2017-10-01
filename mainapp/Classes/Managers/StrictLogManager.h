//
//  StrictLogManager.hpp -- A log manager with more strict rules than LogManager
//  TodoSchool on Sep 21, 2017
//
//  Copyright (c) 2016 Enuma, Inc. All rights reserved.
//  See LICENSE.md for more details.
//

// NB(xenosoz, 2017): What we're trying to achieve is to make analyze process easier at the cost of project building speed.
//   ((That means, frequent rebuilding may occur as we change this header/class.))


#pragma once

#include <string>


class StrictLogManager {
public:
    // NB(xenosoz, 2017): Curriculum choice (Menu/MainScene)
    void curriculumChoice_Begin();
    void curriculumChoice_End();
    void curriculumChoice_TouchCoop();
    
    
    // NB(xenosoz, 2017): Course choice (Menu/CoopScene)
    //   [ ] ShowNewEgg 작성하기
    void courseChoice_Begin();
    void courseChoice_End();
    //void courseChoice_ShowNewEgg();  // [ ] 알이 출현한 것: 언제 무엇이  // DailyScene::showClear을 보면 될 것 같지만.. 알이 두 개 이상 생겨나는 때가 있는데  // dayChoice_End_Complete로 추정가능?
    void courseChoice_ShowNewAnimal(const std::string& levelID);
    void courseChoice_TouchAnimal(const std::string& levelID);
    
    
    // NB(xenosoz, 2017): Day choice (Menu/DailyScene)
    void dayChoice_Begin(const std::string& levelID);
    void dayChoice_End_Quit(const std::string& levelID);
    void dayChoice_End_Complete(const std::string& levelID);
    
    void dayChoice_OpenFreeChoiceMenu();
    void dayChoice_CloseFreeChoiceMenu();
    void dayChoice_OpenFreeChoiceLevelPopup(const std::string& gameName, int openCount, int targetCount);
    void dayChoice_CloseFreeChoiceLevelPopup();
    void dayChoice_ChooseFreeChoiceGame(const std::string& gameName, int gameLevel);
    

    // NB(xenosoz, 2017): Daily game choice (Menu/GameSelectScene)
    void dailyGameChoice_Begin(const std::string& levelID, int dayID);
    void dailyGameChoice_End_Quit(const std::string& levelID, int dayID);
    void dailyGameChoice_End_Complete(const std::string& levelID, int dayID);
    void dailyGameChoice_ChooseDailyGame(const std::string& levelID, int dayID, int choiceIndex);


    // NB(xenosoz, 2017): Daily/FreeChoice game
    //   [ ] Quit의 경우 스테이지까지 남으면 좋음
    void game_Begin(const std::string& gameName, int gameLevel);
    void game_End_Quit(const std::string& gameName, int gameLevel, double duration);
    void game_End_Complete(const std::string& gameName, int gameLevel, double duration, int result);
    
    void video_Begin(const std::string& videoName);
    void video_End_Quit(const std::string& videoName, double duration);
    void video_End_Complete(const std::string& videoName, double duration, int result);
    
    void book_Begin(const std::string& bookName);
    void book_End_Quit(const std::string& bookName, double duration);
    void book_End_Complete(const std::string& bookName, double duration, int result);
    
    void comprehension_Begin(const std::string& bookName, int problemSet);
    void comprehension_End_Quit(const std::string& bookName, int problemSet, double duration);
    void comprehension_End_Complete(const std::string& bookName, int problemSet, double duration, int result);
    void comprehension_Solve(const std::string& bookName, int problemSet, int problemIndex);
    
    
    // NB(xenosoz, 2017): EggQuiz (Games/EggQuiz)
    void eggQuiz_CorrectAnswer(int problemIndex, const std::string& answer, int answerIndex);
    void eggQuiz_WrongAnswer(int problemIndex, const std::string& correctAnswer, const std::string& myAnswer, int answerIndex);
    

    // NB(xenosoz, 2017): Stars
    void starStat_UpdateStarsInKitKitSchool(int oldStars, int newStars);
    
    
    // NB(xenosoz, 2017): Launcher (XXX)
    // 런처 시작(언제)
    // 킷킷 스쿨 시작 버튼 누름
    // 라이브러리 메뉴 버튼 누름
    // 툴 메뉴 버튼 누름
    // 런처 종료(언제) -> 슬립모드 돌입을 알 수 있나? 1분마다 heartbeat을 찍는것이 나을지도?
    
    
    // NB(xenosoz, 2017): Tool (XXX)
    // 툴 구매 타이밍(무엇을 언제)
    // 각 툴(드럼, 마림바...) 시작
    // 각 툴 큇(얼마나 했나)
    // back 버튼으로 툴 메뉴 나감(언제)
    
    
    // NB(xenosoz, 2017): Library (XXX)
    // 비디오 시작(무엇을)
    // 비디오 클리어(끝까지 봄)
    // 책 시작(무엇을)
    // 책 클리어(끝까지 봄)
    // back버튼으로 라이브러리 메뉴 나감
    
    
public:
    static StrictLogManager* shared();
};
