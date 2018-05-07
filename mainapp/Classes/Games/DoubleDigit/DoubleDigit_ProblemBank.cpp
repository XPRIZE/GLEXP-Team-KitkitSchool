//
//  DoubleDigit_ProblemBank.cpp
//  todomath
//
//  Created by Sunmi Seol on 12/14/15.
//
//

#include "DoubleDigit_ProblemBank.h"
#include "cocos2d.h"

#include <string>

using namespace std;

DoubleDigit_ProblemBank::DoubleDigit_ProblemBank()
: ProblemBank()
{

}

DoubleDigit_ProblemBank::~DoubleDigit_ProblemBank()
{
}



bool check_already_chosen_vec(std::vector<cocos2d::Point>answerArray,int number, int num_i, int num_j)
{
    
    for (auto i=0; i < answerArray.size(); i++)
    {
        if ((answerArray[i].x == num_i) && (answerArray[i].y == num_j)) return true;
    }
    
    return false;
    
}

int getRandomIntInclusive(int startNum, int endNum)
{
    int ret;
    
    if (startNum > endNum)
    {
        CCLOG("invalid range: %d ~ %d", startNum, endNum);
        std::swap(startNum, endNum);
    }
    
    do {
        ret = rand()/(RAND_MAX/(endNum - startNum + 1)) + startNum;
    } while (ret > endNum);
    
    return ret;
}


cocos2d::Point genTwoNumbers(std::vector<cocos2d::Point>answerArray, int low_limit_answer_val, int high_limit_answer_val, int startnum, int lastnum, bool l_fixed, int l_fixed_num, bool r_fixed, int r_fixed_num, bool isRegroup, bool isMultiples, int md_type)
{
    int curr_lhs, curr_rhs;
    
    bool ok;
    do {
        ok = true;
        
        if(r_fixed == true) {
            
            curr_rhs = r_fixed_num;
            curr_lhs = getRandomIntInclusive(startnum, lastnum);
            
        } else if (l_fixed == true) {
            
            curr_lhs = l_fixed_num;
            curr_rhs = getRandomIntInclusive(startnum, lastnum);
            
        } else if (md_type == 12) { // level 12 for multi-digit
            
            curr_lhs = getRandomIntInclusive(startnum, lastnum);
            curr_rhs = (getRandomIntInclusive(0, 1) == 0)? 10:100;
            
        } else {
            
            curr_lhs = getRandomIntInclusive(startnum, lastnum);
            
            if(curr_lhs < low_limit_answer_val) {
                curr_rhs = getRandomIntInclusive(low_limit_answer_val - curr_lhs, high_limit_answer_val - curr_lhs);
            } else {
                curr_rhs = getRandomIntInclusive(startnum, high_limit_answer_val - curr_lhs);
            }
        }
        
        if((check_already_chosen_vec(answerArray,0 , curr_lhs, curr_rhs)) || ((isRegroup == false) && ((curr_lhs%10)-(curr_rhs%10) < 0))
           || ((isMultiples == false) && (curr_lhs%10==0)) || ((md_type ==10) && (curr_rhs%5 != 0))) {
            ok = false;
        }
        
    } while (!ok);
    
    CCLOG("(two numbers for addition) chosen number:%d %d", curr_lhs, curr_rhs);
    return cocos2d::Point(curr_lhs, curr_rhs);
}

// Generate 2 numbers lhs + rhs = answer
// startnum: low number value which can be chosen as one number
// lastnum: high number value which can be chosen as one number

cocos2d::Point genTwoNumbers4Subtraction(std::vector<cocos2d::Point>answerArray, int low_limit_answer_val, int high_limit_answer_val, int startnum, int lastnum, bool l_fixed, int l_fixed_num, bool r_fixed, int r_fixed_num, bool isRegroup, bool isMultiples, int md_type)
{
    int curr_lhs, curr_rhs;
    
    bool ok;
    do {
        ok = true;
        
        if(r_fixed == true) {
            
            curr_rhs = r_fixed_num;
            curr_lhs = getRandomIntInclusive(startnum, lastnum);
            
        } else if (l_fixed == true) {
            
            curr_lhs = l_fixed_num;
            curr_rhs = getRandomIntInclusive(startnum, lastnum);
            
        } else if (md_type == 12) { // level 12 for multi-digit
            
            curr_lhs = getRandomIntInclusive(startnum, lastnum);
            curr_rhs = (getRandomIntInclusive(0, 1) == 0)? 10:100;
            
        } else {
            curr_lhs = getRandomIntInclusive(startnum, lastnum);
            curr_rhs = getRandomIntInclusive(curr_lhs - high_limit_answer_val, curr_lhs - low_limit_answer_val);
        }
        
        if((check_already_chosen_vec(answerArray,0 , curr_lhs, curr_rhs)) || ((isRegroup == false) && ((curr_lhs%10)-(curr_rhs%10) < 0))|| ((isMultiples == false) && (curr_lhs%10==0)) || ((md_type == 10) && (curr_rhs%5 != 0))) ok = false;
        
    } while (!ok);
    
    CCLOG("(two numbers for subtraction)chosen number:%d %d", curr_lhs, curr_rhs);
    return cocos2d::Point(curr_lhs, curr_rhs);
}

vector<DoubleDigitLevelStruct> DoubleDigit_ProblemBank::loadData(int level)
{
    
    std::string rawString = cocos2d::FileUtils::getInstance()->getStringFromFile("DoubleDigit/DoubleDigit_Levels.tsv");
    auto data = TodoUtil::readTSV(rawString);
    
    vector<DoubleDigitLevelStruct> problemDatas;
    problemDatas.clear();
    
    for (auto row : data) {
        if (row.size() < 1) continue;
        if (TodoUtil::trim(row[0]).size() <= 0) continue;
        if (row[0][0] == '#') continue;
        for (int i=0; i<row.size(); i++) row[i] = TodoUtil::removeSpaces(TodoUtil::trim(row[i]));
        
        int rawLevel = TodoUtil::stoi(row[0]);
        string rawNumber1 = row[1];
        bool rawAddition = row[2] == "TRUE" ? true : false;
        bool rawSubtraction = row[3] == "TRUE" ? true : false;
        string rawNumber2 = row[4];
        string rawResultRange = row[5];
        string rawRegrouping = row[6];
        int rawProblemCount = TodoUtil::stoi(row[7]);

        
        if (_maxLevel < rawLevel) _maxLevel = rawLevel;
        if (rawLevel != level) continue;
        
        problemDatas.push_back({
            rawLevel,
            rawNumber1,
            rawAddition,
            rawSubtraction,
            rawNumber2,
            rawResultRange,
            rawRegrouping,
            rawProblemCount
        });
        
    }
    
    return problemDatas;
}

int DoubleDigit_ProblemBank::getNumber(string rawNumber) {
    if (find(begin(rawNumber), end(rawNumber), '-') != end(rawNumber)) {
        auto numbers = TodoUtil::split(rawNumber, '-');
        return random<int>(TodoUtil::stoi(numbers[0]), TodoUtil::stoi(numbers[1]));
    } else if (find(begin(rawNumber), end(rawNumber), ',') != end(rawNumber)) {
        auto numbers = TodoUtil::split(rawNumber, ',');
        return TodoUtil::stoi(numbers.at(random<int>(0, numbers.size()-1)));
    } else {
        return TodoUtil::stoi(rawNumber);
    }
    
}

bool DoubleDigit_ProblemBank::checkRegrouping(int number1, char op, int resultCandidate) {
    int tempNumber1 = number1;
    int tempResultCandidate = resultCandidate;
    while(1) {
        if (op == '+' && tempNumber1%10 > tempResultCandidate%10) return true;
        else if (op == '-' && tempNumber1%10 < tempResultCandidate%10) return true;
        tempNumber1 /= 10;
        tempResultCandidate /= 10;
        if (!tempNumber1 || !tempResultCandidate) return false;
    }
}

DoubleDigitValidProblem DoubleDigit_ProblemBank::getValidProblem(DoubleDigitLevelStruct &rawProblem) {
    DoubleDigitValidProblem rt;
    vector<char> ops;
    if (rawProblem.addition) ops.push_back('+');
    if (rawProblem.subtraction) ops.push_back('-');
    int i=0;
    while(1) {
        
        bool valid = true;
        int number1 = getNumber(rawProblem.number1);
        int number2 = getNumber(rawProblem.number2);
        random_shuffle(ops.begin(), ops.end(), [](int n) { return rand() % n; });
        int resultCandidate;
        auto op = ops.at(0);

        if (op == '+') resultCandidate = number1 + number2;
        else resultCandidate = number1 - number2;
        
        if (rawProblem.resultRange != "" ) {
            auto resultRanges = TodoUtil::split(rawProblem.resultRange, '-');
            if (resultCandidate < TodoUtil::stoi(resultRanges.at(0)) || resultCandidate > TodoUtil::stoi(resultRanges.at(1))) valid = false;
        }
        
        if (rawProblem.regrouping != "") {
            if (rawProblem.regrouping == "YES") {
                if (!checkRegrouping(number1, op, resultCandidate)) valid = false;
            } else if (rawProblem.regrouping == "NO") {
                if (checkRegrouping(number1, op, resultCandidate)) valid = false;
            }
        }
        
        if (valid) {
            if (find(_answers.begin(), _answers.end(), resultCandidate) == _answers.end()) {
                _answers.push_back(resultCandidate);
                rt.number1 = number1;
                rt.number2 = number2;
                rt.op = op;
                rt.answer = resultCandidate;
                
                CCLOG("answer:%d", resultCandidate);
                //CCLOG("success:%d", i);
                return rt;
            }
        }
        
        if (++i==1000) return rt;
    }
    return rt;
}

vector<int> DoubleDigit_ProblemBank::getCandidateLevelIDs() {
    loadData(1);
    vector<int> ret;
    ret.clear();
    
    for (int i=1; i<=_maxLevel; i++) {
        ret.push_back(i);
    }
    return ret;
}


Json::Value DoubleDigit_ProblemBank::generateParameters(int level)
{
    Json::Value rt(Json::objectValue);
    Json::Value problems(Json::arrayValue);
    
    auto problemDatas = loadData(level);
    auto problemData = problemDatas.at(0);
    
    for (int i=0; i<problemData.problemCount; i++) {

        auto row = getValidProblem(problemData);

        Json::Value problem(Json::objectValue);
        problem["lhs"] = row.number1;
        problem["op"] = row.op;
        problem["rhs"] = row.number2;
        problem["answer"] = row.answer;
        problems.append(problem);
    }
    rt["level"] = level;
    rt["problems"] = problems;
    
    return rt;
    /*
    
    
    for (auto i=0; i<5; ++i) {
        
        Json::Value problem(Json::objectValue);
        int lhs, rhs, answer;
        char op;

        
        switch (level) {
            case 1:
                {
                    cocos2d::Point p = genTwoNumbers(answerArray, 10, 20, 1, 19, false, 0, false, 0, true, true, level);
                    answerArray.push_back(p);
                 
                    lhs = answerArray[i].x;
                    rhs = answerArray[i].y;
                    op = '+';
                    answer = lhs+rhs;
                }
                break;
            case 2:
                {
                    do {
                        lhs = getRandomIntInclusive(1, 9)*10;
                        rhs = getRandomIntInclusive(1, 2)*5;
                        
                    } while (check_already_chosen_vec(answerArray, 0, lhs, rhs));
                    
                    cocos2d::Point p(lhs, rhs);
                    answerArray.push_back(p);
                    op = '+';
                    answer = lhs+rhs;
                }
                break;
            case 3:
                {
                    cocos2d::Point p = genTwoNumbers4Subtraction(answerArray, 1, 10, 11, 20, false, 0, false, 0, false, true, level);
                    answerArray.push_back(p);
                    
                    lhs = answerArray[i].x;
                    rhs = answerArray[i].y;
                    op = '-';
                    answer = lhs-rhs;
                }
                break;
            case 4:
                {
                    do {
                        lhs = getRandomIntInclusive(1, 9)*10;
                        rhs = getRandomIntInclusive(1, 2)*5;
                        
                    } while (check_already_chosen_vec(answerArray, 0, lhs, rhs));
                    
                    cocos2d::Point p(lhs, rhs);
                    answerArray.push_back(p);
                    op = '-';
                    answer = lhs-rhs;
                }
                break;
            case 5:
                {
                    cocos2d::Point p;
                    if (i%2 == 0) { // even
                        p = genTwoNumbers(answerArray, 21, 99, 11, 89, false, 0, true, 10, true, false, level);
                        answerArray.push_back(p);
                        
                        lhs = answerArray[i].x;
                        rhs = answerArray[i].y;
                        op = '+';
                        answer = lhs+rhs;
                    } else { // odd
                        p = genTwoNumbers4Subtraction(answerArray, 1, 89, 11, 99, false, 0, true, 10, true, false, level);
                        answerArray.push_back(p);
                        
                        lhs = answerArray[i].x;
                        rhs = answerArray[i].y;
                        op = '-';
                        answer = lhs-rhs;
                    }

                }
                break;
            case 6:
                {
                    cocos2d::Point p = genTwoNumbers4Subtraction(answerArray, 1, 10, 11, 20, false, 0, false, 0, true, true, level);
                    answerArray.push_back(p);
                    
                    lhs = answerArray[i].x;
                    rhs = answerArray[i].y;
                    op = '-';
                    answer = lhs-rhs;
                }
                break;
            case 7:
                {
                    cocos2d::Point p;
                    if (i%2 == 0) { // even
                        p = genTwoNumbers(answerArray, 10, 20, 1, 9, false, 0, false, 0, true, true, level);
                        answerArray.push_back(p);
                        
                        lhs = answerArray[i].x;
                        rhs = answerArray[i].y;
                        op = '+';
                        answer = lhs+rhs;
                    } else { // odd
                        p = genTwoNumbers4Subtraction(answerArray, 1, 10, 11, 20, false, 0, false, 0, true, true, level);
                        answerArray.push_back(p);
                        
                        lhs = answerArray[i].x;
                        rhs = answerArray[i].y;
                        op = '-';
                        answer = lhs-rhs;
                    }
                    
                }
                break;
            case 8:
                {
                    cocos2d::Point p;
                    if (i%2 == 0) { // even
                        p = genTwoNumbers(answerArray, 20, 50, 10, 40, false, 0, false, 0, true, true, level);
                        answerArray.push_back(p);
                        
                        lhs = answerArray[i].x;
                        rhs = answerArray[i].y;
                        op = '+';
                        answer = lhs+rhs;
                    } else { // odd
                        p = genTwoNumbers4Subtraction(answerArray, 10, 19, 20, 50, false, 0, false, 10, true, true, level);
                        answerArray.push_back(p);
                        
                        lhs = answerArray[i].x;
                        rhs = answerArray[i].y;
                        op = '-';
                        answer = lhs-rhs;
                    }
                    
                }
                break;
            case 9:
                {
                    do {
                        lhs = getRandomIntInclusive(1, 9)*100;
                        rhs = getRandomIntInclusive(1, 2)*200-300;
                        
                    } while (check_already_chosen_vec(answerArray, 0, lhs, rhs));
                    cocos2d::Point p(lhs, rhs);
                    answerArray.push_back(p);
                    
                    if (rhs<0) {
                        rhs = 100;
                        op = '-';
                        answer = lhs-rhs;

                    } else {
                        rhs = 100;
                        op = '+';
                        answer = lhs+rhs;
                    }
                }
                break;
            case 10:
                {
                    cocos2d::Point p;
                    if (i%2 == 0) { // even
                        p = genTwoNumbers(answerArray, 5, 95, 5, 95, true, 100, false, 0, true, true, level);
                        answerArray.push_back(p);
                        
                        lhs = answerArray[i].x;
                        rhs = answerArray[i].y;
                        op = '+';
                        answer = lhs+rhs;
                    } else { // odd
                        p = genTwoNumbers4Subtraction(answerArray, 5, 95, 5, 95, true, 100, false, 0, true, true, level);
                        answerArray.push_back(p);
                        
                        lhs = answerArray[i].x;
                        rhs = answerArray[i].y;
                        op = '-';
                        answer = lhs-rhs;
                    }
                    
                }
                break;
            case 11:
                {
                    cocos2d::Point p;
                    if (i%2 == 0) { // even
                        p = genTwoNumbers(answerArray, 50, 100, 10, 90, false, 0, false, 0, true, true, level);
                        answerArray.push_back(p);
                        
                        lhs = answerArray[i].x;
                        rhs = answerArray[i].y;
                        op = '+';
                        answer = lhs+rhs;
                    } else { // odd
                        p = genTwoNumbers4Subtraction(answerArray, 1, 49, 50, 100, false, 0, false, 0, true, true, level);
                        answerArray.push_back(p);
                        
                        lhs = answerArray[i].x;
                        rhs = answerArray[i].y;
                        op = '-';
                        answer = lhs-rhs;
                    }
                    
                }
                break;
            case 12:
                {
                    cocos2d::Point p;
                    if (i%2 == 0) { // even
                        p = genTwoNumbers(answerArray, 90, 1000, 100, 900, false, 0, false, 0, true, true, level);
                        answerArray.push_back(p);
                        
                        lhs = answerArray[i].x;
                        rhs = answerArray[i].y;
                        op = '+';
                        answer = lhs+rhs;
                    } else { // odd
                        p = genTwoNumbers4Subtraction(answerArray, 10, 890, 100, 900, false, 0, false, 0, true, true, level);
                        answerArray.push_back(p);
                        
                        lhs = answerArray[i].x;
                        rhs = answerArray[i].y;
                        op = '-';
                        answer = lhs-rhs;
                    }
                    
                }
                break;
            case 13:
                {
                    cocos2d::Point p;
                    if (i%2 == 0) { // even
                        p = genTwoNumbers(answerArray, 200, 500, 100, 400, false, 0, false, 0, true, true, level);
                        answerArray.push_back(p);
                        
                        lhs = answerArray[i].x;
                        rhs = answerArray[i].y;
                        op = '+';
                        answer = lhs+rhs;
                    } else { // odd
                        p = genTwoNumbers4Subtraction(answerArray, 1, 100, 200, 500, false, 0, false, 0, true, true, level);
                        answerArray.push_back(p);
                        
                        lhs = answerArray[i].x;
                        rhs = answerArray[i].y;
                        op = '-';
                        answer = lhs-rhs;
                    }
                    
                }
                break;
            case 14:
                {
                    cocos2d::Point p = genTwoNumbers4Subtraction(answerArray, 1, 900, 100, 999, true, 1000, false, 0, true, true, level);
                    answerArray.push_back(p);
                    
                    lhs = answerArray[i].x;
                    rhs = answerArray[i].y;
                    op = '-';
                    answer = lhs-rhs;
                }
                break;
            case 15:
                {
                    cocos2d::Point p;
                    if (i%2 == 0) { // even
                        p = genTwoNumbers(answerArray, 500, 1000, 100, 900, false, 0, false, 0, true, true, level);
                        answerArray.push_back(p);
                        
                        lhs = answerArray[i].x;
                        rhs = answerArray[i].y;
                        op = '+';
                        answer = lhs+rhs;
                    } else { // odd
                        p = genTwoNumbers4Subtraction(answerArray, 100, 499, 500, 1000, false, 0, false, 0, true, true, level);
                        answerArray.push_back(p);
                        
                        lhs = answerArray[i].x;
                        rhs = answerArray[i].y;
                        op = '-';
                        answer = lhs-rhs;
                    }
                    
                }
                break;

            default:
                break;
        }
        problem["lhs"] = lhs;
        problem["op"] = std::string(1, op);
        problem["rhs"] = rhs;
        problem["answer"] = answer;
        problems.append(problem);
        
        
    }
    
    a["level"] = level;
    a["problems"] = problems;
    
    return a;
     */
}



