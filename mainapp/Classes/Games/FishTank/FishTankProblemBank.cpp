//
//  FishTankProblemBank.cpp
//  todomath
//
//  Created by Jesun Kim on 2/25/16.
//
//

#include "FishTankProblemBank.h"
#include "cocos2d.h"

USING_NS_CC;
#include <string>

using namespace std;


FishTankProblemBank::FishTankProblemBank()
{
    
}

FishTankProblemBank::~FishTankProblemBank()
{
    
}

Json::Value FishTankProblemBank::generateParameters(int level) {
    Json::Value a(Json::objectValue);
    Json::Value temp(Json::arrayValue);
    std::vector<std::vector<std::string>> vecProblems;

    char buf[128];
    sprintf(buf, "FishTank/Problems/FT_%d.json", level);
    string problemsString = FileUtils::getInstance()->getStringFromFile(buf);

    if(problemsString.empty() == false){
        Json::Value root;
        Json::Reader reader;
        
        if(reader.parse(problemsString, root)){
            Json::Value& problems = root["Problems"];
            bool isRandom = root["IsRandom"].asBool();
            bool isMinus = root["IsMinus"].asBool();
            bool isShowAnswer = root["ShowAnswer"].asBool();
            bool isOnlyShowSubtrahend = root["OnlyShowSubtrahend"].asBool();
            std::string strLevelFormat = root["Format"].asString();
            int size = problems.size();
            for(int i = 0; i < size ; ++i){
                Json::Value& problem = problems[i];
                std::vector<std::string> strVector;
                for (int j=0; j<problem.size(); j++) {
                    strVector.push_back(problem[j].asString());
                }
                vecProblems.push_back(strVector);
            }
            
            if(vecProblems.size() > 0){
                // random일 경우에만 shuffle
                if (isRandom) {
                    std::random_shuffle(vecProblems.begin(), vecProblems.end());
                }
                int problemCount = 5;
                for(int i = 0; i < problemCount; ++i){
                    Json::Value temp2(Json::arrayValue);
                    for (int j=0; j<vecProblems.at(i).size(); j++) {
                        temp2[j] = vecProblems.at(i).at(j);
                    }
                    temp[i] = temp2;
                }
                // format 데이터
                sprintf(buf, "FishTank/Problems/FT_foramt_%s.json", strLevelFormat.c_str());
                string formatString = FileUtils::getInstance()->getStringFromFile(buf);
                Json::Value formatRoot;
                Json::Reader formatReader;
                if (formatReader.parse(formatString, formatRoot)) {
                    a["TopTankCount"] = formatRoot["TopTankCount"].asInt();
                    a["TopTankName"] = formatRoot["TopTankName"].asString();
                    a["BottomTankCount"] = formatRoot["BottomTankCount"].asInt();
                    a["BottomTankName"] = formatRoot["BottomTankName"].asString();
                    a["IsAlreadyPhase2"] = formatRoot["IsAlreadyPhase2"].asBool();
                    a["IsHideTankText"] = formatRoot["IsHideTankText"].asBool();
                    a["IsReversePhase2"] = formatRoot["IsReversePhase2"].asBool();
                    a["IsMinus"] = isMinus;
                    a["ShowAnswer"] = isShowAnswer;
                    a["OnlyShowSubtrahend"] = isOnlyShowSubtrahend;
                }
                
                // wrong 데이터
                string wrongString = FileUtils::getInstance()->getStringFromFile("FishTank/Problems/FT_Wrong.json");
                Json::Value wrongRoot;
                Json::Reader wrongReader;
                Json::Value tempWrong(Json::arrayValue);
                if (wrongReader.parse(wrongString, wrongRoot)) {
                    Json::Value& problems = wrongRoot["Problems"];
                    int size = problems.size();
                    for (int i=0; i<size; i++) {
                        tempWrong[i] = problems[i].asString();
                    }
                }

                a["level"] = level;
                a["answers"] = temp;
                a["format"] = strLevelFormat;
                a["wrong"] = tempWrong;
            }
        }
    }
    
    return a;
}
