//
//  EquationMakerProblemBank.h
//  todomath
//
//  Created by Hye Kyoung Hong on 7/23/15.
//
//

#ifndef __todomath__EquationMakerProblemBank__
#define __todomath__EquationMakerProblemBank__

#include "Base/ProblemBank.h"
#include <vector>
#include <string>

class EquationMakerProblemBank: public ProblemBank {
public:
//    enum eObjectL5{
//        L5None, fish_1, fish_2, fish_3,
//        fish_4, fish_5, fish_6
//    };
    
    
    const std::vector<std::string> L5Objects = {
        "fish_1", "fish_2", "fish_3", "fish_4", "fish_5", "fish_6"
    };
    
//    enum eObjectL4{
//        L4PlusNone, cookie, cupcake01, cupcake02, cupcake03, cupcake04, cupcake05,
//        donuts01, donuts02, donuts03, donuts04, donuts05, L4PlusMax,
//        
//        L4MinusNone, icecream01, icecream02, icecream03, icecream04, icecream05,
//        juice01, juice02, milk, L4MinusMax,
//    };
    const std::vector<std::string> L4MinusObjects = {
        "cup_grapejuice", "cup_milk", "cup_tea",
        "icecream_1", "icecream_2", "icecream_3", "icecream_4", "icecream_5", "icecream_6",
    };
    //enum eObjectL3{
    //    L3None, apple1, grape1, corn1, acorn, mushroom, strawberry1, L3Max,
    //};
    
    const std::vector<std::string> L3Objects = {
        "carrot", "corn", "eggplant", "mushroom", "pepper", "potato", "pumpkin", "sweetpotato", "tomato",
        "ball", "cracker", "egg", "peanut", "pencil",
        "apple", "grape", "lemon", "mango", "orange", "pear", "strawberry",
    };
    
    
    
    struct sData{
        int type;
        int rowType;
        
        bool isSlotIn;
        bool isSlotLineHide;
        
        sData(int _type, int _row, bool _isSlotIn = false, bool _hide = true):
        type(_type), rowType(_row), isSlotIn(_isSlotIn), isSlotLineHide(_hide){}
    };
    
    struct sProblem{
        std::string file;
        std::string otherFile;
        std::vector<int> vecDummy;
        std::vector<sData> vecData;
        
        sProblem(){
            file = "";
            otherFile = "";
        }
    };
    
public:
    EquationMakerProblemBank();
    ~EquationMakerProblemBank() {}
    
    virtual Json::Value generateParameters(int level);
    
    
public:
    //std::string getL3FileName(int _objectType);
    //std::string getL4FileName(int _objectType);
    //std::string getL5FileName(int _objectType);
    
    int getMathSign(int _curLevel);
    
    int getLevelType(int _curLevel);
    int getRepNumber(int _curLevel);
    int getTargetCount(int _curLevel);

    int getStartNumber(int _curLevel);
    int getMaxNumber(int _curLevel);
    
    int getUpAnswerCount(int _curLevel, int _max);
    int getAnswerCount(int _curLevel, int _levelType);
    
    int getPanelCount(int _curLevel);
    
};

#endif 
