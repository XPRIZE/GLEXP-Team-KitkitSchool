//
//  WordMachineScene.hpp
//  KitkitSchool
//
//  Created by timewalker on 7/1/16.
//
//

#ifndef WordMachineScene_hpp
#define WordMachineScene_hpp

#include <stdio.h>

#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
#include "Models/WordMachineLevelData.hpp"
#include "Models/WordMachineWorksheet.hpp"
#include "LetterWheel.hpp"
#include <Common/Basic/CreateFunc.h>
#include "Common/Controls/ProgressIndicator.hpp"
#include <vector>


USING_NS_CC;

using namespace cocos2d::ui;
std::string _speakMachineWord;
class WordMachineScene : Layer
{
public:
    enum class WordType
    {
        Consonant,
        Vowel,
    };
    
    static Scene* createScene(int levelID);
    virtual bool init(int levelID);
    CREATE_FUNC(WordMachineScene);


private:
    Node* _gameNode;
    Size _gameSize;
    
    Node* _leftCard;
    Node* _rightCard;
    
    unsigned int _levelID;
    int _worksheetID;
    todoschool::wordmachine::Worksheet _worksheet;
    unsigned int _currentProblemID;
    todoschool::wordmachine::Problem _currentProblem;    
    unsigned int _validWheelCount;

    std::vector<LetterWheel*> _wheels;
    
    // NB(xenosoz, 2016): I myself wasn't involved in implementing this game too much.
    //   But I know those word/image vector were used in pre-leveling era.
    //   I commented them but didn't cut it out because someone else might want to go back in time.
    //std::vector<std::string> _legitWords;
    //std::vector<std::string> _legitImages;
    //std::vector<std::string> _madeupWords;

    int _numSpinningWheels;
    std::string _targetWord;
    bool _targetIsLegit;
    
    Button *_startButton;
    ProgressIndicator *_progressBar;
    
    
    
    void startMachine();
    LetterWheel* CreateWheel(WordType wordType, bool playTick);
        
    void initWheels(int numWheels, std::string types);
    //void onStart();
    void onWheelsStop();
    void handleCorrectAnswer();
    void handleMadeupWordDone();
    void handleSuccess();
    
    void showImageCard();
    
};

#endif /* WordMachineScene_hpp */
