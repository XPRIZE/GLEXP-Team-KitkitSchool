//
//  ProblemBank.cpp
//  KitkitSchool-mobile
//
//  Created by JungJaehun on 26/03/2018.
//

#include "ProblemBank.hpp"
#include "3rdParty/CCNativeAlert.h"

BEGIN_NS_EGGQUIZMATH;

ProblemBank* ProblemBank::_instance = 0;

ProblemBank* ProblemBank::getInstance()
{
    if(!_instance) {
        _instance = new ProblemBank();
        _instance->init();
    }
    return _instance;
}

void ProblemBank::init() {
}

map<string,float> ProblemBank::loadDurationsheet() {
    std::string rawString = cocos2d::FileUtils::getInstance()->getStringFromFile("EggQuizMath/Sounds/Durations.tsv");
    auto data = TodoUtil::readTSV(rawString);
    
    map<string,float> rt;
    rt.clear();
    
    for (auto row : data) {
        if (TodoUtil::trim(row[0]).size() <= 0) continue;
        if (row.size()==1) continue;
        auto row1Vec = TodoUtil::split(row[1], ':');
        TodoUtil::replaceAll(row1Vec[2], ".", "");
        auto rowDuration = (float)TodoUtil::stoi(row1Vec[2])/100;
        transform(row[0].begin(), row[0].end(), row[0].begin(), ::tolower);
        rt[row[0]] = rowDuration;
    }
    return rt;
}

vector<string> ProblemBank::getLevels() {
    _levels.clear();
    loadData("");
    return _levels;
}

string ProblemBank::checkRandom(string buf) {
    auto bufVec = TodoUtil::split(buf, '~');
    if (bufVec.size() == 1) return buf;
    return TodoUtil::itos(random<int>(TodoUtil::stoi(bufVec[0]), TodoUtil::stoi(bufVec[1])));
}

vector<string> ProblemBank::parseFomula(string fomula) {
    vector<string> ret;
    ret.clear();
    
    string buf = "";
    for (int i=0; i<fomula.size(); i++) {
        if (fomula[i] == '+' || fomula[i] == '-' || fomula[i] == '=') {
            ret.push_back(checkRandom(buf));
            ret.push_back({fomula[i]});
            buf = "";
            continue;
        }
        buf += fomula[i];
    }
    ret.push_back(checkRandom(buf));
    return ret;
}

int ProblemBank::executeFomula(vector<string> fomula) {
    int rt = 0;
    for (int i=0; i<fomula.size(); i++) {
        if (fomula.at(i) == "+") {
            rt += TodoUtil::stoi(fomula.at(i+1));
            i++;
            continue;
        } else if (fomula.at(i) == "-") {
            rt -= TodoUtil::stoi(fomula.at(i+1));
            i++;
            continue;
        } else if (fomula.at(i) == "=") {
            break;
        }
        rt = TodoUtil::stoi(fomula.at(i));
    }
    return rt;
}

string ProblemBank::getFomulaString(vector<string> fomula) {
    string rt = "";
    for (auto it : fomula) {
        rt += it;
    }
    return rt;
}

string ProblemBank::getJoinString(vector<int> numbers) {
    string rt = "";
    for (auto it : numbers) {
        if (rt != "") rt += ",";
        rt += TodoUtil::itos(it);
    }
    return rt;
}

string ProblemBank::getJoinString(vector<string> strings) {
    string rt = "";
    for (auto it : strings) {
        if (rt != "") rt += ",";
        rt += it;
    }
    return rt;
}


bool ProblemBank::checkRegrouping(int number1, char op, int resultCandidate) {
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

bool ProblemBank::hasSameAnswer(string templateName, string questionOption1, string answerRange) {
    for (auto it : _problems) {
        if (templateName == it.templateName && questionOption1 == it.questionOption1 && it.answerRange == answerRange) return true;
    }
    return false;
    
}

bool ProblemBank::checkReviewFail(EggQuizMathTemplateStruct type) {
    _reviewCount++;
    if (_reviewCount == 10000) {
        NativeAlert::show("problem review failed", TodoUtil::itos(type.type) + "," + type.templateName + "," + type.questionOption1 + "," + type.questionOption2 + "," + type.questionOption3 + "," + type.answerRange + "," + type.answerOption1, "OK");
        _reviewCount = 0;
        return true;
    }
    return false;
}

vector<EggQuizMathTemplateStruct> ProblemBank::loadData(string level)
{
    transform(level.begin(), level.end(), level.begin(), ::tolower);

    std::string rawString = cocos2d::FileUtils::getInstance()->getStringFromFile("EggQuizMath/EggQuizMath_Levels.tsv");
    auto data = TodoUtil::readTSV(rawString);
    
    map<int, EggQuizMathTemplateStruct> types;
    vector<EggQuizMathLevelStruct> levels;
    vector<string> sequence;
    
    _problems.clear();
    types.clear();
    levels.clear();
    
    for (auto row : data) {
        if (row.size() < 2) continue;
        if (row[0][0] == '#') continue;
        bool blankCheck = true;
        for (int i=0; i<row.size(); i++) {
            row[i] = TodoUtil::removeSpaces(TodoUtil::trim(row[i]));
            transform(row[i].begin(), row[i].end(), row[i].begin(), ::tolower);
            if (row[i] != "") blankCheck = false;
        }
        if (blankCheck) continue;
        
        if (row[0] != "") {
            string rawLevel = row[0];
            string rawLevelTypeSequence = row[1];
            levels.push_back({rawLevel, rawLevelTypeSequence});
            _levels.push_back(rawLevel);
        } else {
            int rawType = TodoUtil::stoi(row[2]);
            string templateName = row[3];
            //transform(templateName.begin(), templateName.end(), templateName.begin(), ::tolower);
            string questionOption1 = row[4];
            string questionOption2 = row[5];
            string questionOption3 = row[6];
            string answerRange = row[7];
            string answerOption1 = row[8];
            types[rawType] = {rawType, templateName, questionOption1, questionOption2, questionOption3, answerRange, answerOption1};
        }
    }
    
    for (auto it : levels) {
        if (it.level != level) continue;
        sequence = TodoUtil::split(it.levelTypeSequence, ',');
    }
    
    /*
     for (int i=0; i<sequence.size(); i++) {
     sequence[i] = checkRandom(sequence[i]);
     }
     */
    _reviewCount = 0;
    
    for (auto it : sequence) {
        auto typeRange = checkRandom(it);
        auto type = types[TodoUtil::stoi(typeRange)];
        if (type.templateName == "singledigitnumbers"
            || type.templateName == "3digitnumbers"
            ) {
            while(1) {
                auto newAnswer = checkRandom(type.answerRange);;
                if (hasSameAnswer(type.templateName, type.questionOption1, newAnswer)) continue;
                type.answerRange = newAnswer;
                break;
            }
        }
        else if (type.templateName == "2digitnumbers"
                 || type.templateName == "recognizenumber"
                 ) {
            string newAnswer;
            while(1) {
                newAnswer = checkRandom(type.answerRange);
                if (hasSameAnswer(type.templateName, type.questionOption1, newAnswer)) continue;
                break;
            }
            vector<string> choices = {newAnswer};
            while(1) {
                auto choice = checkRandom(type.answerRange);
                if (find(choices.begin(), choices.end(), choice) != choices.end()) continue;
                choices.push_back(choice);
                if (choices.size() == 4) break;
            }
            random_shuffle(choices.begin(), choices.end(), [](int n) { return rand() % n; });
            
            type.answerRange = newAnswer;
            type.answerOption1 = getJoinString(choices);

        }
        else if (type.templateName == "operationswithobjects" || type.templateName == "operationswithoutobjects") {
            while(1) {
                auto fomula = parseFomula(type.questionOption1);
                if (type.answerOption1 == "fixed10") {
                    int num = random<int>(1,9);
                    fomula[0] = TodoUtil::itos(num);
                    fomula[1] = "+";
                    fomula[2] = TodoUtil::itos(10-num);
                }
                auto result = executeFomula(fomula);
                auto answerRange = TodoUtil::split(type.answerRange, '~');
                if (answerRange.size()==1) answerRange.push_back(answerRange[0]);
                if (result >= TodoUtil::stoi(answerRange[0]) && result <= TodoUtil::stoi(answerRange[1])) {
                    if (fomula.size()==5) {
                        if (type.questionOption2 == "regroupingtrue" && !checkRegrouping(TodoUtil::stoi(fomula[0]), fomula[1][0], result)) continue;
                        if (type.questionOption2 == "regroupingfalse" && checkRegrouping(TodoUtil::stoi(fomula[0]), fomula[1][0], result)) continue;
                    }
                    
                    auto questionOption1Candidate = getFomulaString(fomula);
                    auto answerRangeCandidate = TodoUtil::itos(result);
                    if (hasSameAnswer(type.templateName, questionOption1Candidate, answerRangeCandidate)) continue;
                    type.questionOption1 = questionOption1Candidate;
                    type.answerRange = answerRangeCandidate;
                    //CCLOG("%s, %s, %s", type.templateName.c_str(), type.questionOption1.c_str(), type.answerRange.c_str());
                    break;
                }
                if (checkReviewFail(type)) break;
            }
        }
        else if (type.templateName == "biggestandsmallest") {
            auto questionTypes = TodoUtil::split(type.questionOption1, ',');
            random_shuffle(questionTypes.begin(), questionTypes.end(), [](int n) { return rand() % n; });
            type.questionOption1 = questionTypes[0];
            
            auto answerRange = TodoUtil::split(type.answerRange, '~');
            vector<int> choices;
            choices.clear();
            int answerInt = 0;
            
            while(1) {
                int newChoice = random<int>(TodoUtil::stoi(answerRange[0]),TodoUtil::stoi(answerRange[1]));
                if (find(choices.begin(), choices.end(), newChoice) != choices.end()) continue;
                choices.push_back(newChoice);
                if (choices.size() == 4) break;
            }
            random_shuffle(choices.begin(), choices.end(), [](int n) { return rand() % n; });
            string choiceStr = "";
            
            for (auto it : choices) {
                if (answerInt == 0) answerInt = it;
                else if (type.questionOption1 == "biggest" && answerInt < it) answerInt = it;
                else if (type.questionOption1 == "smallest" && answerInt > it) answerInt = it;
                if (choiceStr != "") choiceStr += ",";
                choiceStr += TodoUtil::itos(it);
            }
            type.answerRange = TodoUtil::itos(answerInt);
            type.answerOption1 = choiceStr;
        }
        else if (type.templateName == "biggerandsmaller") {
            
            auto questionTypes = TodoUtil::split(type.questionOption1, ',');
            random_shuffle(questionTypes.begin(), questionTypes.end(), [](int n) { return rand() % n; });
            type.questionOption1 = questionTypes[0];
            
            if (type.questionOption2 == "") type.questionOption2 = "number,image";
            auto questionTypes2 = TodoUtil::split(type.questionOption2, ',');
            random_shuffle(questionTypes2.begin(), questionTypes2.end(), [](int n) { return rand() % n; });
            type.questionOption2 = questionTypes2[0];
            
            auto answerRange = TodoUtil::split(type.answerRange, '~');
            vector<int> choices;
            choices.clear();
            int answerInt = 0;
            
            while(1) {
                int newChoice = random<int>(TodoUtil::stoi(answerRange[0]),TodoUtil::stoi(answerRange[1]));
                if (find(choices.begin(), choices.end(), newChoice) != choices.end()) continue;
                choices.push_back(newChoice);
                if (choices.size() == 2) break;
            }
            random_shuffle(choices.begin(), choices.end(), [](int n) { return rand() % n; });
            string choiceStr = "";
            
            for (auto it : choices) {
                if (answerInt == 0) answerInt = it;
                else if (type.questionOption1 == "bigger" && answerInt < it) answerInt = it;
                else if (type.questionOption1 == "smaller" && answerInt > it) answerInt = it;
                if (choiceStr != "") choiceStr += ",";
                choiceStr += TodoUtil::itos(it);
            }
            type.answerRange = TodoUtil::itos(answerInt);
            type.answerOption1 = choiceStr;
            
        } else if (type.templateName == "comparenumbermagnitudes") {
            auto questionTypes = TodoUtil::split(type.questionOption1, ',');
            random_shuffle(questionTypes.begin(), questionTypes.end(), [](int n) { return rand() % n; });
            type.questionOption1 = questionTypes[0];
            
            auto answerRangeVec = TodoUtil::split(type.answerRange, '~');
            vector<int> cards;
            
            while(1) {
                cards.clear();
                
                if (type.answerOption1 == "") {
                    while(1) {
                        int newCard = random<int>(TodoUtil::stoi(answerRangeVec[0]),TodoUtil::stoi(answerRangeVec[1]));
                        if (find(cards.begin(), cards.end(), newCard) != cards.end()) continue;
                        cards.push_back(newCard);
                        if (cards.size() == 4) break;
                    }
                } else {
                    while(1) {
                        auto newAnswer = checkRandom(type.answerRange);;
                        auto step = TodoUtil::stoi(type.answerOption1);
                        int max = TodoUtil::stoi(answerRangeVec[1]);
                        int newAnswerInt = TodoUtil::stoi(newAnswer);
                        if (newAnswerInt > max-step*3) {
                            if (checkReviewFail(type)) break;
                            continue;
                        }
                        cards = {newAnswerInt, newAnswerInt+step, newAnswerInt+step*2, newAnswerInt+step*3};
                        random_shuffle(cards.begin(), cards.end(), [](int n) { return rand() % n; });
                        break;
                    }
                }
                string answerRange = "";
                for (auto it : cards) {
                    if (answerRange != "") answerRange += ",";
                    answerRange += TodoUtil::itos(it);
                }
                type.answerRange = answerRange;
                if (!hasSameAnswer(type.templateName, type.questionOption1, answerRange)) break;
                else {
                    if (checkReviewFail(type)) break;
                }
            }
            
        } else if (type.templateName == "usemagnitudesymbols") {
            while(1) {
                string questionOption1 = checkRandom(type.questionOption1);
                string questionOption2 = checkRandom(type.questionOption2);
                string result = TodoUtil::itos(TodoUtil::stoi(questionOption1) + TodoUtil::stoi(questionOption2));
                if (hasSameAnswer(type.templateName, questionOption1, result)) {
                    if (checkReviewFail(type)) break;
                    continue;
                }
                type.answerRange = result;
                type.questionOption1 = questionOption1;
                type.questionOption2 = questionOption2;
                break;
            }
            
        } else if (type.templateName == "shapes") {
            while(1) {
                auto answerRange = TodoUtil::split(type.answerRange, ',');
                random_shuffle(answerRange.begin(), answerRange.end(), [](int n) { return rand() % n; });
                type.answerRange = answerRange[0];
                CCLOG("%s, %s, %s", type.templateName.c_str(), type.questionOption1.c_str(), type.answerRange.c_str());
                if (hasSameAnswer(type.templateName, type.questionOption1, type.answerRange)) {
                    if (checkReviewFail(type)) break;
                    typeRange = checkRandom(it);
                    type = types[TodoUtil::stoi(typeRange)];
                    continue;
                }
                break;
            }
            auto answerOption1 = TodoUtil::split(type.answerOption1, ',');
            random_shuffle(answerOption1.begin(), answerOption1.end(), [](int n) { return rand() % n; });
            
            vector<string> choices = {type.answerRange};
            for (auto it : answerOption1) {
                if (it == type.answerRange) continue;
                choices.push_back(it);
                if (choices.size()==4) break;
            }
            
            random_shuffle(choices.begin(), choices.end(), [](int n) { return rand() % n; });
            
            string buffer = "";
            for (auto it : choices) {
                if (buffer != "") buffer += ",";
                buffer += it;
            }
            type.answerOption1 = buffer;
        } else if (type.templateName == "missingnumber") {
            string answerRange = "";
            int multiple = 0;
            if (type.answerOption1 != "") multiple = TodoUtil::stoi(type.answerOption1);
            int position = (type.questionOption2 == "") ? random<int>(1,4) : TodoUtil::stoi(type.questionOption2);
            
            vector<string> ascOrDesc = {"ascending", "descending"};
            if (type.questionOption3 == "") type.questionOption3 = ascOrDesc.at(random<int>(0,1));
            
            while(1) {
                auto answerRangeVec = TodoUtil::split(type.answerRange, '~');
                int min = TodoUtil::stoi(answerRangeVec[0]);
                int max = TodoUtil::stoi(answerRangeVec[1]);
                
                int answerCandidate = TodoUtil::stoi(checkRandom(type.answerRange));
                int step = TodoUtil::stoi(type.questionOption1);
                
                if (type.questionOption3 == "ascending") {
                    if (answerCandidate + step*3 > max) continue;
                    else {
                        vector<int> answerVec = {answerCandidate, answerCandidate+step*1, answerCandidate+step*2, answerCandidate+step*3};
                        if (multiple != 0 && answerVec.at(position-1) % multiple !=0) continue;
                        answerRange = getJoinString(answerVec);
                        if (hasSameAnswer(type.templateName, type.questionOption1, answerRange)) continue;
                        break;
                    }
                } else {
                    if (answerCandidate - step*3 < min) continue;
                    else {
                        vector<int> answerVec = {answerCandidate, answerCandidate-step*1, answerCandidate-step*2, answerCandidate-step*3};
                        if (multiple != 0 && answerVec.at(position-1) % multiple !=0) continue;
                        answerRange = getJoinString(answerVec);
                        if (hasSameAnswer(type.templateName, type.questionOption1, answerRange)) continue;
                        break;
                    }
                    
                }
            }
            type.answerRange = answerRange;
            type.questionOption2 = TodoUtil::itos(position);
        }
        
        CCLOG("template type %d done! : %s", type.type, type.answerRange.c_str());
        _problems.push_back(type);
    }
    return _problems;
}

END_NS_EGGQUIZMATH;

