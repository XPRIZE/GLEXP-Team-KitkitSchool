//
//  EquationMakerProblemBank.cpp
//  todomath
//
//  Created by Hye Kyoung Hong on 7/23/15.
//
//

#include "EquationMakerProblemBank.h"
#include "EquationMakerScene.h"
#include <Common/Basic/CommentStream.h>
#include <Managers/LanguageManager.hpp>

#ifndef MAKE_STRING
#   define MAKE_STRING(_Text) "" #_Text
#endif

EquationMakerProblemBank::EquationMakerProblemBank() {
    
}

Json::Value EquationMakerProblemBank::generateParameters(int level) {
    Json::Value a(Json::objectValue);
    Json::Value temp(Json::arrayValue);
    
#ifdef IMPORT_TSV_FILE_EQUATION_MAKER
    auto p = LanguageManager::getInstance()->findLocalizedResource("Games/EquationMaker/EquationMaker_Levels.tsv");
    std::string s = FileUtils::getInstance()->getStringFromFile(p);
    CCLOG("levels file : %s", s.c_str());
    todoschool::CommentStream stream(s);
    
    int Level, MinNumber, MaxNumber, Repeat, Line1BlankCount, PannelCount;
    std::string Language, Type, MathSign;
    
    while (stream >> Language >> Level >> Type >> MathSign >> MinNumber >> MaxNumber >> Repeat >> Line1BlankCount >> PannelCount)
    {
        if (level == Level)
        {
            mItemTSV.level = Level;
            mItemTSV.type = 3;
            if (Type == "B")
            {
                mItemTSV.type = 4;
            }
            else if (Type == "C")
            {
                mItemTSV.type = 5;
            }
            
            mItemTSV.mathSign = EquationMakerScene::K_TYPE_SIGN_PLUS;
            if (MathSign == "M")
            {
                mItemTSV.mathSign = EquationMakerScene::K_TYPE_SIGN_MINUS;
            }
            else if (MathSign == "R")
            {
                mItemTSV.mathSign = EquationMakerScene::K_TYPE_SIGN_RANDOM;
            }
            mItemTSV.minNumber = MinNumber;
            mItemTSV.maxNumber = MaxNumber;
            mItemTSV.repeatCount = Repeat;
            mItemTSV.maxLine1BlankCount = Line1BlankCount;
            mItemTSV.panelCount = PannelCount;
        }
    }
#endif
    
    int levelType = getLevelType(level);
    
    
    std::vector<sProblem> vecProblemList;
    
    switch (levelType) {
        case 3:
        {
            std::vector<int> vecRow;
            /* problem row type setting */
            vecRow.push_back(EquationMakerScene::K_OBJECT_TYPE_NUMBER);
            vecRow.push_back(EquationMakerScene::K_OBJECT_TYPE_OBJECT);
            
            if(level >= 3 && level <= 4){
                int temp = vecRow[0];
                vecRow[0] = vecRow[1];
                vecRow[1] = temp;
            }
            /* problem row type setting End */

            
            
            /* problem Number Setting */
            std::vector<int> vecList;

            int mathSign = getMathSign(level);
            int targetValueStart = getStartNumber(level);
            int targetValueMax = getMaxNumber(level);

            int problemCount = 6;

            for(int i = targetValueStart; i < targetValueMax; ++i)
                vecList.push_back(i);
            
            int prevFirst = 0;
            for(int i = 0; i < problemCount; ++i){
                sProblem problem;
                
                std::random_shuffle(vecList.begin(), vecList.end());
                int first = vecList[0];
                if(first == prevFirst) first = vecList[1];

                prevFirst = first;
                
                int second = 0;
                int right = 0;
                int subSign = mathSign;
                
                if(mathSign == EquationMakerScene::K_TYPE_SIGN_RANDOM)
                    subSign = rand()%2;
                
                for(unsigned int j = 0; j < vecList.size(); ++j){
                    int sum = first+vecList[j];
                    
                    if(sum <= targetValueMax){
                        bool isFind = true;
                        if(subSign == EquationMakerScene::K_TYPE_SIGN_MINUS){
                            if(first == vecList[j]) isFind = false;
                        }
                        
                        if(isFind){
                            second = vecList[j];
                            break;
                        }
                    }
                }
                
                right = first+second;

                if(subSign == EquationMakerScene::K_TYPE_SIGN_MINUS){
                    int temp = first;
                    first = right;
                    right = temp;
                }
                
                int rowType = vecRow[0];
                problem.vecData.push_back(sData(first, rowType));
                problem.vecData.push_back(sData(subSign, rowType, true));
                problem.vecData.push_back(sData(second, rowType));
                problem.vecData.push_back(sData(EquationMakerScene::K_TYPE_SIGN_EQ, rowType, true));
                problem.vecData.push_back(sData(right, rowType));
                /* problem Number Setting End */
                
                
                /* target Setting */
                std::vector<sData> vecDownData;
                for(unsigned int j = 0; j < problem.vecData.size(); ++j){
                    rowType = vecRow[1];
                    
                    sData data = problem.vecData[j];
                    data.rowType = rowType;
                    vecDownData.push_back(data);
                }
                
                int targetCount = getAnswerCount(level, levelType);
                int up = getUpAnswerCount(level, targetCount-1);

                if(up > 0){
                    std::vector<int> vecProblemSelect;
                    for(int j = 0; j < problem.vecData.size(); ++j){
                        if(problem.vecData[j].isSlotIn == false)
                            vecProblemSelect.push_back(j);
                    }
                    
                    std::random_shuffle(vecProblemSelect.begin(), vecProblemSelect.end());
                    
                    for(int j = 0; j < up; ++j){
                        int index = vecProblemSelect[j];
                        problem.vecData[index].isSlotIn = false;
                        problem.vecData[index].isSlotLineHide = false;
                        
                        vecDownData[index].isSlotIn = true;
                        vecDownData[index].isSlotLineHide = true;
                    }
                    
                    int selectSize = (int)vecProblemSelect.size();
                    int sizeMax = MIN(targetCount, selectSize);
                    for(int j = up; j < sizeMax; ++j){
                        int index = vecProblemSelect[j];
                        
                        vecDownData[index].isSlotIn = false;
                        vecDownData[index].isSlotLineHide = false;
                        
                        problem.vecData[index].isSlotIn = true;
                        problem.vecData[index].isSlotLineHide = true;
                    }
                }
                else{
                    int forSize = MAX(targetCount, (int)vecDownData.size());
                    for(int j = 0; j < forSize; ++j){
                        if(vecDownData[j].isSlotIn == false){
                            vecDownData[j].isSlotIn = false;
                            vecDownData[j].isSlotLineHide = false;
                        }
                    }
                    
                    for(int j = 0; j < problem.vecData.size(); ++j){
                        problem.vecData[j].isSlotIn = true;
                        problem.vecData[j].isSlotLineHide = true;
                    }
                }
                /* target Setting End */
                
                
                /* dummy Setting */
                int findArray[3] = {first, second, right};
                std::random_shuffle(vecList.begin(), vecList.end());
                for(unsigned int j = 0; j < vecList.size(); ++j){
                    int num = vecList[j];
                    
                    bool isFind = false;
                    for(unsigned int k = 0; k < 3; ++k){
                        int destType = findArray[k];
                        if(destType == num){
                            isFind = true;
                            break;
                        }
                    }
                    
                    if(isFind == false){
                        problem.vecDummy.push_back(num);
                        if(problem.vecDummy.size() > 1) break;
                    }
                }
                /* dummy Setting End */
                
                
                /* target Merge */
                for(unsigned int j = 0; j < vecDownData.size(); ++j){
                    sData data = vecDownData[j];
                    problem.vecData.push_back(data);
                }
                /* target Merge End */
                
                std::vector<std::string> vecObjectList;
                
                //for(int i = apple1; i < L3Max; ++i)
                for (auto s : L3Objects) {
                    vecObjectList.push_back(s);
                }
                
                std::random_shuffle(vecObjectList.begin(), vecObjectList.end());
                auto object = vecObjectList[0];
                auto objectOther = vecObjectList[1];
                
                problem.file = object; //getL3FileName(object);
                problem.otherFile = objectOther; //getL3FileName(objectOther);
                vecProblemList.push_back(problem);
            }
           
            break;
        }
        case 4:
        {
            int mathSign = getMathSign(level);
            std::vector<int> vecRow;
            /* problem row type setting */
            vecRow.push_back(EquationMakerScene::K_OBJECT_TYPE_OBJECT_NON_SLOT);
            vecRow.push_back(EquationMakerScene::K_OBJECT_TYPE_NUMBER);
            /* problem row type setting End */
            
            
            /* problem target Setting */
            int startNumber = getStartNumber(level);
            int maxNumber = getMaxNumber(level);
            
            std::vector<int> vecList;
            for(int i = startNumber; i < maxNumber; ++i) vecList.push_back(i);
            
            int prevFirst = 0;
            int problemCount = 6;
            for(int i = 0; i < problemCount; ++i){
                sProblem problem;

                int subSign = mathSign;
                if(mathSign == EquationMakerScene::K_TYPE_SIGN_RANDOM)
                    subSign = EquationMakerScene::K_TYPE_SIGN_PLUS + (rand()%2);
                
                std::random_shuffle(vecList.begin(), vecList.end());
                
                int first = vecList[0];
                if(first == prevFirst) first = vecList[1];
                
                prevFirst = first;

                int maxNum = maxNumber-first;
                std::vector<int> vecSecond;

                for(int j = 1; j <= maxNum; ++j) vecSecond.push_back(j);
                std::random_shuffle(vecSecond.begin(), vecSecond.end());
                
                int second = vecSecond[0];
                int right = first + second;

                if(subSign == EquationMakerScene::K_TYPE_SIGN_MINUS){
                    int temp = first;
                    first = right;
                    right = temp;
                }
                
                problem.vecData.push_back(sData(first, vecRow[0], true));
                problem.vecData.push_back(sData(subSign, vecRow[0], true));
                problem.vecData.push_back(sData(second, vecRow[0], true));
                
                
                problem.vecData.push_back(sData(first, vecRow[1], false, false));
                problem.vecData.push_back(sData(subSign, vecRow[1], false, false));
                problem.vecData.push_back(sData(second, vecRow[1], false, false));
                problem.vecData.push_back(sData(EquationMakerScene::K_TYPE_SIGN_EQ, vecRow[1], false, false));
                problem.vecData.push_back(sData(right, vecRow[1], false, false));
                
                
                /* dummy Setting */
                int findArray[3] = {first, second, right};
                std::random_shuffle(vecList.begin(), vecList.end());
                for(unsigned int j = 0; j < vecList.size(); ++j){
                    int num = vecList[j];
                    
                    bool isFind = false;
                    for(unsigned int k = 0; k < 3; ++k){
                        int destType = findArray[k];
                        if(destType == num){
                            isFind = true;
                            break;
                        }
                    }
                    
                    if(isFind == false){
                        problem.vecDummy.push_back(num);
                        if(problem.vecDummy.size() > 1) break;
                    }
                }
                /* dummy Setting End */
                
                
                /* problem Object Setting */
//                int objectStart = cookie;
//                int objectEnd = L4PlusMax;
//                if(subSign == EquationMakerScene::K_TYPE_SIGN_MINUS){
//                    objectStart = icecream01;
//                    objectEnd = L4MinusMax;
//                }
                
                
                std::vector<std::string> vecObjectList;
                if(subSign == EquationMakerScene::K_TYPE_SIGN_MINUS){
                    for (auto s : L4MinusObjects) {
                        vecObjectList.push_back(s);
                    }
                } else {
                    for (auto s : L3Objects) {
                        vecObjectList.push_back(s);
                    }
                }
//                
//                for(int i = objectStart; i < objectEnd; ++i)
//                    vecObjectList.push_back(i);
                
                std::random_shuffle(vecObjectList.begin(), vecObjectList.end());
                auto object = vecObjectList[0];
                auto objectOther = vecObjectList[1];
                
                problem.file = object;//getL4FileName(object);
                problem.otherFile = objectOther;//getL4FileName(objectOther);
                /* problem Object Setting End*/
                
                
                vecProblemList.push_back(problem);
            }
            /* problem Target Setting */
            
            
            break;
        }
        case 5:
        {
            int repNum = getRepNumber(level);
            int targetValueStart = 2;
            int targetValueEnd = 10;
            int prevObjectIndex = -1;
            int mathSign = EquationMakerScene::K_TYPE_SIGN_PLUS;
            int rowType = EquationMakerScene::K_OBJECT_TYPE_OBJECT_NUMBER;
            
            std::vector<int> vecList;
            for(int i = targetValueStart; i <= targetValueEnd; ++i) vecList.push_back(i);
            
            std::vector<std::string> vecObjectList;
            for (auto s : L5Objects) vecObjectList.push_back(s);
            //for(int i = fish_1; i <= fish_6; ++i) vecObjectList.push_back(i);
            
            for(unsigned int i = 0; i < vecList.size(); ++i){
                sProblem problem;
                
                std::random_shuffle(vecObjectList.begin(), vecObjectList.end());
                
                int objectIndex = 0; //vecObjectList[0];
                if(objectIndex == prevObjectIndex){
                    for(unsigned int j = 1; j < vecObjectList.size(); ++j){
                        objectIndex = j;
                        if(objectIndex != prevObjectIndex) break;
                    }
                }
                
                int num = vecList[i];
                for(int i = 0; i < repNum; ++i){
                    problem.vecData.push_back(sData(num, rowType, false, false));
                    if(i < (repNum-1))
                        problem.vecData.push_back(sData(mathSign, rowType, true));
                }
                
                //right number
                problem.vecData.push_back(sData(EquationMakerScene::K_TYPE_SIGN_EQ, rowType, true));
                problem.vecData.push_back(sData(repNum*num, rowType, true, false));
                
                problem.file = vecObjectList.at(objectIndex);// getL5FileName(object);
                
                vecProblemList.push_back(problem);
            }
            
            break;
        }
      
        default:{
            break;
        }
    }
    
    for(unsigned int i = 0; i < vecProblemList.size(); ++i){
        sProblem& problem = vecProblemList[i];

        temp[i]["file"] = problem.file;
        temp[i]["fileOther"] = problem.otherFile;
        
        Json::Value objectList;
        
        for(unsigned int j = 0; j < problem.vecData.size(); ++j){
            sData data = problem.vecData[j];
            objectList[j]["type"] = data.type;
            objectList[j]["isSlotIn"] = data.isSlotIn;
            objectList[j]["isSlotLineHide"] = data.isSlotLineHide;
            objectList[j]["rowType"] = data.rowType;
        }
        
        Json::Value dummyData;
        for(unsigned int j = 0; j < problem.vecDummy.size(); ++j){
            dummyData[j] = problem.vecDummy[j];
        }
        
        temp[i]["objectList"] = objectList;
        temp[i]["dummyList"] = dummyData;
        temp[i]["panelCount"] = getPanelCount(level);
    }
    
    a["level"] = level;
    a["levelType"] = levelType;
    a["answers"] = temp;
    a["problemCount"] = (int)vecProblemList.size();
    
    return a;
}

int EquationMakerProblemBank::getPanelCount(int _curLevel){
#ifdef IMPORT_TSV_FILE_EQUATION_MAKER
    return mItemTSV.panelCount;
    
#else
    int ret = 0;
    
    if(_curLevel <= 2) ret = 4;
    else if(_curLevel >= 3 && _curLevel <= 7) ret = 5;
    else if(_curLevel == 8) ret = 2;
    else if(_curLevel == 9) ret = 3;
    else ret = 4;
    
    return ret;
#endif
}

//std::string EquationMakerProblemBank::getL5FileName(int _objectType){
//    std::string ret = "";
//    
//    switch (_objectType) {
//    
//        case fish_1: ret = MAKE_STRING(fish_1); break;
//        case fish_2: ret = MAKE_STRING(fish_2); break;
//        case fish_3: ret = MAKE_STRING(fish_3); break;
//        case fish_4: ret = MAKE_STRING(fish_4); break;
//        case fish_5: ret = MAKE_STRING(fish_5); break;
//        case fish_6: ret = MAKE_STRING(fish_6); break;
//            
//        default: break;
//    }
//    
//    return ret;
//}
//

//std::string EquationMakerProblemBank::getL3FileName(int _objectType){
//    std::string ret = "";
//    
//    const std::string names[] = {
//        "L3None", "carrot", "corn", "eggplant", "mushroom", "pepper", "potato", "pumpkin", "sweetpotato", "tomato",
//        "ball", "cracker", "egg", "peanut", "pencil",
//        "apple", "grape", "lemon", "mango", "orange", "pear", "strawberry",
//        "L3MAX"
//    };
//    
//    return names[_objectType];
//    /*
//    switch (_objectType) {
//        case apple1: ret = MAKE_STRING(apple1); break;
//        case grape1: ret = MAKE_STRING(grape1); break;
//        case corn1: ret = MAKE_STRING(corn1); break;
//        case acorn: ret = MAKE_STRING(acorn); break;
//        case mushroom: ret = MAKE_STRING(mushroom); break;
//        case strawberry1: ret = MAKE_STRING(strawberry1); break;
//        default: break;
//    }
//    
//    return ret;
//     */
//}

//std::string EquationMakerProblemBank::getL4FileName(int _objectType){
//    std::string ret = "";
//    
//    switch (_objectType) {
//        case cookie: ret = MAKE_STRING(cookie); break;
//        case cupcake01: ret = MAKE_STRING(cupcake01); break;
//        case cupcake02: ret = MAKE_STRING(cupcake02); break;
//        case cupcake03: ret = MAKE_STRING(cupcake03); break;
//        case cupcake04: ret = MAKE_STRING(cupcake04); break;
//        case cupcake05: ret = MAKE_STRING(cupcake05); break;
//            
//        case donuts01: ret = MAKE_STRING(donuts01); break;
//        case donuts02: ret = MAKE_STRING(donuts02); break;
//        case donuts03: ret = MAKE_STRING(donuts03); break;
//        case donuts04: ret = MAKE_STRING(donuts04); break;
//        case donuts05: ret = MAKE_STRING(donuts05); break;
//            
//        case icecream01: ret = MAKE_STRING(icecream01); break;
//        case icecream02: ret = MAKE_STRING(icecream02); break;
//        case icecream03: ret = MAKE_STRING(icecream03); break;
//        case icecream04: ret = MAKE_STRING(icecream04); break;
//        case icecream05: ret = MAKE_STRING(icecream05); break;
//
//        case juice01: ret = MAKE_STRING(juice01); break;
//        case juice02: ret = MAKE_STRING(juice02); break;
//
//        case milk: ret = MAKE_STRING(milk); break;
//
//
//        default: break;
//    }
//    
//    return ret;
//}

int EquationMakerProblemBank::getMathSign(int _curLevel){
#ifdef IMPORT_TSV_FILE_EQUATION_MAKER
    return mItemTSV.mathSign;
    
#else
    int ret = EquationMakerScene::K_TYPE_SIGN_PLUS;
    
    if(_curLevel == 2 || _curLevel == 4 || _curLevel == 6)
        ret =  EquationMakerScene::K_TYPE_SIGN_MINUS;
    
    else if(_curLevel == 7) ret = EquationMakerScene::K_TYPE_SIGN_RANDOM;
    
    return ret;
#endif
}

int EquationMakerProblemBank::getLevelType(int _curLevel){
#ifdef IMPORT_TSV_FILE_EQUATION_MAKER
    return mItemTSV.type;
    
#else
    if(_curLevel >= 5 && _curLevel <= 7) return 4;
    else if(_curLevel >= 8) return 5;
    
    return 3;
#endif
}

int EquationMakerProblemBank::getRepNumber(int _curLevel){
#ifdef IMPORT_TSV_FILE_EQUATION_MAKER
    return mItemTSV.repeatCount;
    
#else
    int ret = 2;
    
    if(_curLevel == 9) ret = 3;
    else if(_curLevel == 10) ret = 4;
    
    return ret;
#endif
}

int EquationMakerProblemBank::getTargetCount(int _curLevel){
    int ret = 3;
    
    if(_curLevel >= 5 && _curLevel <= 7) ret = 5;
    
    return ret;
}

int EquationMakerProblemBank::getStartNumber(int _curLevel){
#ifdef IMPORT_TSV_FILE_EQUATION_MAKER
    return mItemTSV.minNumber;
    
#else
    int ret = 1;
    
    if(_curLevel == 7) ret = 10;
    
    return ret;
#endif
}

int EquationMakerProblemBank::getMaxNumber(int _curLevel){
#ifdef IMPORT_TSV_FILE_EQUATION_MAKER
    return mItemTSV.maxNumber;
#else
    int ret = 5;
    
    if(_curLevel >= 3 && _curLevel <= 6) ret = 10;
    else if(_curLevel == 7) ret = 20;
    
    return ret;
#endif
}

int EquationMakerProblemBank::getUpAnswerCount(int _curLevel, int _max){
#ifdef IMPORT_TSV_FILE_EQUATION_MAKER
    if (mItemTSV.maxLine1BlankCount == 0)
    {
        return 0;
    }
    
    return MIN(((rand() % mItemTSV.maxLine1BlankCount)+1), mItemTSV.maxLine1BlankCount);
#else
    int ret = 0;
    
    if(_curLevel == 3 || _curLevel == 4) ret = MIN(((rand()%_max)+1), _max);
    
    return ret;
#endif
}

int EquationMakerProblemBank::getAnswerCount(int _curLevel, int _levelType){
#ifdef IMPORT_TSV_FILE_EQUATION_MAKER
    return 3;
    
#else
    int ret = 3;
    
    if(_levelType == 4) return 5;
    else if(_levelType == 5){
        if(_curLevel == 8) return 2;
        else if(_curLevel == 9) return 3;
        else if(_curLevel == 5) return 4;
    }
    
    return ret;
#endif
}

#ifdef IMPORT_TSV_FILE_EQUATION_MAKER
std::vector<int> EquationMakerProblemBank::getLevelIDs()
{
    std::vector<int> result;
    
    auto p = LanguageManager::getInstance()->findLocalizedResource("Games/EquationMaker/EquationMaker_Levels.tsv");
    std::string s = FileUtils::getInstance()->getStringFromFile(p);
    todoschool::CommentStream stream(s);
    
    int Level, MinNumber, MaxNumber, Repeat, Line1BlankCount, PannelCount;
    std::string Language, Type, MathSign;
    
    while (stream >> Language >> Level >> Type >> MathSign >> MinNumber >> MaxNumber >> Repeat >> Line1BlankCount >> PannelCount)
    {
        result.push_back(Level);
    }
    
    return result;
}
#endif
