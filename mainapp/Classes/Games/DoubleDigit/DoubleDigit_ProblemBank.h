//
//  DoubleDigit_ProblemBank.h
//
//  Created by Sunmi Seol on 12/14/15.
//
//

#ifndef __DoubleDigit_ProblemBank__
#define __DoubleDigit_ProblemBank__
#include "Base/ProblemBank.h"
#include "Utils/TodoUtil.h"

using namespace std;

struct DoubleDigitLevelStruct {
    int level;
    string number1;
    bool addition;
    bool subtraction;
    string number2;
    string resultRange;
    string regrouping;
    int problemCount;
};

struct DoubleDigitValidProblem {
    int number1;
    int number2;
    string op;
    int answer;
};

class DoubleDigit_ProblemBank : public ProblemBank
{
    
public:
    
    DoubleDigit_ProblemBank();
    ~DoubleDigit_ProblemBank();
    
    virtual Json::Value generateParameters(int level);
    
public:
    static int getNumberOfProblemsInScreen() { return 6; }
    vector<int> getCandidateLevelIDs();

private:
    vector<DoubleDigitLevelStruct> loadData(int level);
    int _maxLevel = 0;
    DoubleDigitValidProblem getValidProblem(DoubleDigitLevelStruct &rawProblem);
    int getNumber(string rawNumber);
    bool checkRegrouping(int number1, char op, int resultCandidate);
    vector<int> _answers;
    
};


#endif /* defined(__DoubleDigit_ProblemBank__) */
