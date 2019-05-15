//
//  NumberTrainProblemBank.cpp
//  todomath
//
//  Created by Gunho Lee on 2017. 3. 21..
//
//

#include <cocos/cocos2d.h>
#include <vector>
#include "NumberTrainProblemBank.hpp"

using namespace cocos2d;
using namespace std;

NumberTrainProblemBank::NumberTrainProblemBank()
{
    
}

NumberTrainProblemBank::~NumberTrainProblemBank()
{
    
}



vector<int> pickNumbers(int level, int index)
{
    vector<int> ret;
    
    switch (level) {
        case 1: {
            ret = { 1, 2, 3, 4, 5};
            random_shuffle(ret.begin(), ret.end());
            break;
        }
        case 2: {
            ret = {6, 7, 8, 9, 10};
            random_shuffle(ret.begin(), ret.end());
            break;
        }
        case 3: {
            auto a = random<int>(2, 6);
            ret = {a, a+1, a+2, a+3, a+4};
            random_shuffle(ret.begin(), ret.end());
            
            break;
        }
        case 4: {
            ret = {2, 4, 6, 8, 10};
            random_shuffle(ret.begin(), ret.end());
            break;
        }
        case 5: {
            while (ret.size()<5) {
                auto a = random<int>(1, 10);
                if (find(ret.begin(), ret.end(), a)==ret.end()) {
                    ret.push_back(a);
                }
            }
            break;
        }
        case 6: {
            auto a = random<int>(11, 16);
            ret = {a, a+1, a+2, a+3, a+4};
            random_shuffle(ret.begin(), ret.end());
            break;
        }
        case 7: {
            auto a = random<int>(6, 16);
            ret = {a, a+1, a+2, a+3, a+4};
            random_shuffle(ret.begin(), ret.end());
            break;
        }
        case 8: {
            while (ret.size()<5) {
                auto a = random<int>(1, 20);
                if (find(ret.begin(), ret.end(), a)==ret.end()) {
                    ret.push_back(a);
                }
            }
            break;
        }
        case 9: {
            auto a = random<int>(10, 12);
            ret = {a, a+2, a+4, a+6, a+8};
            random_shuffle(ret.begin(), ret.end());
            break;
        }
        case 10: {
            auto mul = 3;
            //1~20
            const int minNum = 1;
            const int maxNum = 20;
            int minBase = (int)ceil((float)minNum/mul);
            int maxBase = (maxNum/mul)-4;
            
            auto base = random<int>(minBase, maxBase);
            for (int i=0; i<5; i++) ret.push_back((base+i)*mul);
            random_shuffle(ret.begin(), ret.end());
            break;
        }
        case 11: {
            auto a = random<int>(20, 46);
            ret = {a, a+1, a+2, a+3, a+4};
            random_shuffle(ret.begin(), ret.end());
            break;
        }
        case 12: {
            auto mul = 5;
            const int minNum = 20;
            const int maxNum = 50;
            int minBase = (int)ceil((float)minNum/mul);
            int maxBase = (maxNum/mul)-4;
            
            auto base = random<int>(minBase, maxBase);
            for (int i=0; i<5; i++) ret.push_back((base+i)*mul);
            random_shuffle(ret.begin(), ret.end());
            break;
        }
        case 13: {
            auto mul = 3;
            const int minNum = 20;
            const int maxNum = 50;
            int minBase = (int)ceil((float)minNum/mul);
            int maxBase = (maxNum/mul)-4;
            
            auto base = random<int>(minBase, maxBase);
            for (int i=0; i<5; i++) ret.push_back((base+i)*mul);
            random_shuffle(ret.begin(), ret.end());
            break;
        }
        case 14: {
            auto mul = 10;
            const int minNum = 1;
            const int maxNum = 100;
            int minBase = (int)ceil((float)minNum/mul);
            int maxBase = (maxNum/mul)-4;
            
            auto base = random<int>(minBase, maxBase);
            for (int i=0; i<5; i++) ret.push_back((base+i)*mul);
            random_shuffle(ret.begin(), ret.end());
            break;
        }
        case 15: {
            while (ret.size()<5) {
                auto a = random<int>(1, 50);
                if (find(ret.begin(), ret.end(), a)==ret.end()) {
                    ret.push_back(a);
                }
            }
            break;
        }
        case 16: {
            auto a = random<int>(60, 96);
            ret = {a, a+1, a+2, a+3, a+4};
            random_shuffle(ret.begin(), ret.end());
            break;
        }
        case 17: {
            auto mul = 5;
            const int minNum = 1;
            const int maxNum = 100;
            int minBase = (int)ceil((float)minNum/mul);
            int maxBase = (maxNum/mul)-4;
            
            auto base = random<int>(minBase, maxBase);
            for (int i=0; i<5; i++) ret.push_back((base+i)*mul);
            random_shuffle(ret.begin(), ret.end());
            break;
        }
        case 18: {
            while (ret.size()<5) {
                auto a = random<int>(60, 100);
                if (find(ret.begin(), ret.end(), a)==ret.end()) {
                    ret.push_back(a);
                }
            }
            break;
        }
        default:
            break;
            
    }
    
    
    
    return ret;
}

Json::Value NumberTrainProblemBank::generateParameters(int level) {
    
    Json::Value a(Json::objectValue);
    Json::Value problems(Json::arrayValue);
    
    
    for (int i=0; i<2; i++) {
        
        vector<int> candidates;
        candidates = pickNumbers(level, i);
        
        Json::Value choices(Json::arrayValue);
        Json::Value answers(Json::arrayValue);
        
        for (auto c : candidates) {
            choices.append(c);
        }
        
        sort(candidates.begin(), candidates.end());
        
        
        if (i == 0) {
            problems[i]["type"] = "ascending";
            for (int j=0; j<candidates.size(); j++) answers.append(candidates[j]);
        } else if (i == 1) {
            problems[i]["type"] = "largest";
            answers.append(candidates.back());
            answers.append(candidates.front());
        }
        
        problems[i]["choices"] = choices;
        problems[i]["answers"] = answers;
        
    }
    
    
    a["level"] = level;
    a["problems"] = problems;
    a["problemCount"] = 2;

    Json::StyledWriter writer;
    CCLOG("%s", writer.write(a).c_str());

    return a;
}
