//
//  ProblemBank.cpp
//  KitkitSchool-mobile
//
//  Created by Jaehun Jung on 29/08/2018.
//

#include "ProblemBank.hpp"
#include "Managers/UserManager.hpp"

#include "3rdParty/CCNativeAlert.h"

BEGIN_NS_EGGQUIZ;

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
    
    if (_durationSheet.size() != 0) return _durationSheet;
    
    std::string rawString = cocos2d::FileUtils::getInstance()->getStringFromFile(soundsPath+"durations.tsv");
    auto data = TodoUtil::readTSV(rawString);
    
    for (auto row : data) {
        if (TodoUtil::trim(row[0]).size() <= 0) continue;
        if (row.size()==1) continue;
        auto row1Vec = TodoUtil::split(row[1], ':');
        TodoUtil::replaceAll(row1Vec[2], ".", "");
        auto rowDuration = (float)TodoUtil::stoi(row1Vec[2])/100;
        //transform(row[0].begin(), row[0].end(), row[0].begin(), ::tolower);
        _durationSheet[row[0]] = rowDuration;
    }
    return _durationSheet;
}

float ProblemBank::getDuration(string fileName) {
    float duration = 1.f;
    
    fileName = fileName.substr(fileName.find_last_of("/\\") + 1);
    if (fileName.find(".m4a") == string::npos) fileName = fileName + ".m4a";
    if (_durationSheet.count(fileName)) duration = _durationSheet[fileName];
    
    return duration;
}

vector<string> ProblemBank::getLevels(string course) {
    int sheetNo;
    _levels.clear();
    loadProblemData(course, "", &sheetNo);
    return _levels;
}

vector<EggQuizTemplateStruct> ProblemBank::loadProblemData(string course, string level, int *sheetNo) {
    srand((unsigned int)time(NULL));
    
    transform(level.begin(), level.end(), level.begin(), ::tolower);
    
    string rawString = FileUtils::getInstance()->getStringFromFile(tsvPath+"eggquiz"+course+"_levels.tsv");
    auto data = TodoUtil::readTSV(rawString);
    
    map<int, EggQuizTemplateStruct> types;
    vector<EggQuizLevelStruct> levels;
    vector<string> sequence;
    int maxWorksheet = 0;
    
    _problems.clear();
    types.clear();
    levels.clear();

    for (auto row : data) {
        if (row.size() < 2) continue;
        if (row[0][0] == '#') continue;

        bool blankCheck = true;
        for (int i=0; i<row.size(); i++) {
            row[i] = TodoUtil::trim(row[i]);
            //transform(row[i].begin(), row[i].end(), row[i].begin(), ::tolower);
            if (row[i] != "") blankCheck = false;
        }
        if (blankCheck) continue;
        
        if (row[0] != "") {
            string rawLevel = row[0];
            int rawWorkSheet = TodoUtil::stoi(row[1]);
            string rawLevelTypeSequence = row[2];
            transform(rawLevel.begin(), rawLevel.end(), rawLevel.begin(), ::tolower);

            levels.push_back({rawLevel, rawWorkSheet, rawLevelTypeSequence});
            if (rawWorkSheet == 1) _levels.push_back(rawLevel);
            
            if (level == rawLevel) maxWorksheet = MAX(maxWorksheet, rawWorkSheet); 
        } else {
            int rawType = TodoUtil::stoi(row[3]);
            string templateName = row[4];
            //transform(templateName.begin(), templateName.end(), templateName.begin(), ::tolower);
            string questionOption1 = row[5];
            string questionOption2 = row[6];
            string questionOption3 = row[7];
            string questionOption4 = row[8];
            string questionOption5 = row[9];
            string answer = row[10];
            string answerOption1 = row.size()>11 ? row[11] : "";
            types[rawType] = {rawType, templateName, questionOption1, questionOption2, questionOption3, questionOption4, questionOption5, answer, answerOption1};
        }
    }

    *sheetNo = random<int>(1,maxWorksheet);
    
    for (auto it : levels) {
        if (it.level != level) continue;
        
        vector<string> newSequence = TodoUtil::split(it.levelTypeSequence, ',');
        
        if (!UserManager::getInstance()->isWorksheetTestMode()) {
            if (it.workSheet != *sheetNo) continue;
        }
        sequence.insert(sequence.end(), newSequence.begin(), newSequence.end());
    }
    
    _reviewCount = 0;
    
    for (auto it : sequence) {
        auto typeRange = checkRandom(it);
        auto type = types[TodoUtil::stoi(typeRange)];
        
        if (type.templateName == "single_digit_numbers"
            || type.templateName == "3digit_numbers"
            || type.templateName == "number_identification"
            ) {
            while(1) {
                auto fixedAnswer = checkRandom(type.answer);
                if (hasSameAnswer(type.templateName, type.questionOption1, fixedAnswer)) continue;
                type.answer = fixedAnswer;
                break;
            }
        }
        else if (type.templateName == "recognize_number"
                 || type.templateName == "2digit_numbers" 
                 ) {
            string fixedAnswer;
            while(1) {
                fixedAnswer = checkRandom(type.answer);
                if (hasSameAnswer(type.templateName, type.questionOption1, fixedAnswer)) continue;
                break;
            }
            vector<string> choices = {fixedAnswer};
            if (type.answerOption1 == "") type.answerOption1 = type.answer; 
            while(1) {
                auto choice = checkRandom(type.answerOption1);
                if (find(choices.begin(), choices.end(), choice) != choices.end()) continue;
                choices.push_back(choice);
                if (choices.size() == 4) break;
            }
            random_shuffle(choices.begin(), choices.end(), [](int n) { return rand() % n; });

            type.answer = fixedAnswer;
            type.answerOption1 = getJoinString(choices);
        }
        
        else if (type.templateName == "bigger_and_smaller") {
            
            auto questionTypes = TodoUtil::split(type.questionOption1, ',');
            random_shuffle(questionTypes.begin(), questionTypes.end(), [](int n) { return rand() % n; });
            type.questionOption1 = questionTypes[0];
            
            if (type.questionOption2 == "") type.questionOption2 = "number,image";
            auto questionTypes2 = TodoUtil::split(type.questionOption2, ',');
            random_shuffle(questionTypes2.begin(), questionTypes2.end(), [](int n) { return rand() % n; });
            type.questionOption2 = questionTypes2[0];
            
            auto answerVec = TodoUtil::split(type.answer, '~');
            vector<int> choices;
            choices.clear();
            int answerInt = 0;
            
            while(1) {
                int newChoice = random<int>(TodoUtil::stoi(answerVec[0]),TodoUtil::stoi(answerVec[1]));
                if (find(choices.begin(), choices.end(), newChoice) != choices.end()) continue;
                choices.push_back(newChoice);
                if (choices.size() == 2) break;
            }
            random_shuffle(choices.begin(), choices.end(), [](int n) { return rand() % n; });
            
            for (auto it : choices) {
                if (answerInt == 0) answerInt = it;
                else if (type.questionOption1 == "bigger" && answerInt < it) answerInt = it;
                else if (type.questionOption1 == "smaller" && answerInt > it) answerInt = it;
            }
            type.answer = TodoUtil::itos(answerInt);
            type.answerOption1 = getJoinString(choices);
        }
        
        else if (type.templateName == "missing_number"
                 || type.templateName == "missing_number_drag"
                 ) {
            int multiple = 0;
            string answerRange = "";
            if (type.answerOption1 != "") multiple = TodoUtil::stoi(type.answerOption1);
            int position = (type.questionOption2 == "") ? random<int>(1,4) : TodoUtil::stoi(type.questionOption2);
            
            vector<string> ascOrDesc = {"ascending", "descending"};
            if (type.questionOption3 == "") type.questionOption3 = ascOrDesc.at(random<int>(0,1));
            
            while(1) {
                auto answerRangeVec = TodoUtil::split(type.answer, '~');
                int min = TodoUtil::stoi(answerRangeVec[0]);
                int max = TodoUtil::stoi(answerRangeVec[1]);
                
                int answerCandidate = TodoUtil::stoi(checkRandom(type.answer));
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
            type.answer = answerRange;
            type.questionOption2 = TodoUtil::itos(position);
        }
        
        else if (type.templateName == "operations_with_objects"
                 || type.templateName == "operations_without_objects"
                 || type.templateName == "operations_drag"
                 ) {
            while(1) {
                auto fomula = parseFomula(type.questionOption1);
                if (type.answerOption1 == "fixed10") {
                    int num = random<int>(1,9);
                    fomula[0] = TodoUtil::itos(num);
                    fomula[1] = "+";
                    fomula[2] = TodoUtil::itos(10-num);
                }
                auto result = executeFomula(fomula);
                auto answer = TodoUtil::split(type.answer, '~');
                if (answer.size()==1) answer.push_back(answer[0]);
                if (result >= TodoUtil::stoi(answer[0]) && result <= TodoUtil::stoi(answer[1])) {
                    if (fomula.size()==5) {
                        if (type.questionOption2 == "regroupingtrue" && !checkRegrouping(TodoUtil::stoi(fomula[0]), fomula[1][0], result)) continue;
                        if (type.questionOption2 == "regroupingfalse" && checkRegrouping(TodoUtil::stoi(fomula[0]), fomula[1][0], result)) continue;
                    }
                    
                    auto questionOption1Candidate = getFomulaString(fomula);
                    auto answerRangeCandidate = TodoUtil::itos(result);
                    if (hasSameAnswer(type.templateName, questionOption1Candidate, answerRangeCandidate)) continue;
                    type.questionOption1 = questionOption1Candidate;
                    type.answer = answerRangeCandidate;
                    //CCLOG("%s, %s, %s", type.templateName.c_str(), type.questionOption1.c_str(), type.answerRange.c_str());
                    break;
                }
                if (checkReviewFail(type)) break;
            }
        }

        else if (type.templateName == "biggest_and_smallest") {
            auto questionTypes = TodoUtil::split(type.questionOption1, ',');
            random_shuffle(questionTypes.begin(), questionTypes.end(), [](int n) { return rand() % n; });
            type.questionOption1 = questionTypes[0];
            
            auto answer = TodoUtil::split(type.answer, '~');
            vector<int> choices;
            choices.clear();
            int answerInt = 0;
            
            while(1) {
                int newChoice = random<int>(TodoUtil::stoi(answer[0]),TodoUtil::stoi(answer[1]));
                if (find(choices.begin(), choices.end(), newChoice) != choices.end()) continue;
                choices.push_back(newChoice);

                if (answerInt == 0) answerInt = newChoice;
                if (type.questionOption1 == "biggest" && answerInt < newChoice) answerInt = newChoice;
                else if (type.questionOption1 == "smallest" && answerInt > newChoice) answerInt = newChoice;
                if (choices.size() == 4) break;
            }
            random_shuffle(choices.begin(), choices.end(), [](int n) { return rand() % n; });
            
            type.answer = TodoUtil::itos(answerInt);
            type.answerOption1 = getJoinString(choices);
        }
        
        else if (type.templateName == "compare_number_magnitudes") {
            auto questionTypes = TodoUtil::split(type.questionOption1, ',');
            random_shuffle(questionTypes.begin(), questionTypes.end(), [](int n) { return rand() % n; });
            type.questionOption1 = questionTypes[0];
            
            auto answerVec = TodoUtil::split(type.answer, '~');
            int max = TodoUtil::stoi(answerVec[1]);
            
            vector<int> cards;
            vector<int> answers;
            cards.clear();
            int step = 0;
            if (type.answerOption1 != "" ) step = TodoUtil::stoi(type.answerOption1);

            while(1) {
                if (!step) {
                    int cardCandidate = TodoUtil::stoi(checkRandom(type.answer));
                    if (find(cards.begin(), cards.end(), cardCandidate) != cards.end()) continue;
                    cards.push_back(cardCandidate);
                    if (cards.size() == 4) break;
                } else {
                    int cardCandidate = TodoUtil::stoi(checkRandom(type.answer));
                    
                    if (cardCandidate > max-step*3) {
                        if (checkReviewFail(type)) break;
                        continue;
                    }
                    cards = {cardCandidate, cardCandidate+step, cardCandidate+step*2, cardCandidate+step*3};
                    random_shuffle(cards.begin(), cards.end(), [](int n) { return rand() % n; });
                    break;
                }
            }
            
            answers = cards;
            
            if (type.questionOption1 == "ascending") {
                sort(answers.begin(), answers.end());
            } else {
                sort(answers.begin(), answers.end(), [](int i, int j){ return i>j; });
            }
            type.answer = getJoinString(answers);
            type.questionOption1 = getJoinString(cards);
            
        }
        
        else if (type.templateName == "use_magnitude_symbols") {
            while(1) {
                string questionOption1 = checkRandom(type.questionOption1);
                string questionOption2 = checkRandom(type.questionOption2);
                string result = questionOption1 + "," + questionOption2;
                if (hasSameAnswer(type.templateName, questionOption1, result)) {
                    if (checkReviewFail(type)) break;
                    continue;
                }
                type.answer = result;
                type.questionOption1 = questionOption1;
                type.questionOption2 = questionOption2;
                break;
            }
        }
        
        else if (type.templateName == "shapes") {
            while(1) {
                auto answers = TodoUtil::split(type.answer, ',');
                random_shuffle(answers.begin(), answers.end(), [](int n) { return rand() % n; });
                auto answerCandidate = answers[0];
                if (hasSameAnswer(type.templateName, type.questionOption1, answerCandidate)) {
                    if (checkReviewFail(type)) break;
                    continue;
                }
                type.answer = answerCandidate;
                break;
            }
            auto answerOption1 = TodoUtil::split(type.answerOption1, ',');

            random_shuffle(answerOption1.begin(), answerOption1.end(), [](int n) { return rand() % n; });
            
            vector<string> choices = {type.answer};
            for (auto it : answerOption1) {
                if (it == type.answer) continue;
                choices.push_back(it);
                if (choices.size()==4) break;
            }
            
            random_shuffle(choices.begin(), choices.end(), [](int n) { return rand() % n; });
            
            type.answerOption1 = getJoinString(choices);
        }
        
        else if (type.templateName == "word_problem") {
            while(1) {
                typeRange = checkRandom(it);
                type = types[TodoUtil::stoi(typeRange)];

                if (hasSameAnswer(type.templateName, type.questionOption1, type.answer)) {
                    if (checkReviewFail(type)) break;
                    continue;
                }
                break;
            }
        }
        else if (
                 type.templateName == "soundonly_word"
                 || type.templateName == "word_word"
                 || type.templateName == "image_word"
                 || type.templateName == "soundonly_image"
                 || type.templateName == "sentence_word"
                 || type.templateName == "sentence_sentence"
                 || type.templateName == "image_sentence"
                 || type.templateName == "soundonly_sentence"
                 || type.templateName == "paragraph_sentence"
                 || type.templateName == "imageseq_sentence"
                 || type.templateName == "imageseq_image"
                 || type.templateName == "ordering_sentence"
                 || type.templateName == "listeningcomp_sentence"
                 ) {
            
            auto answerOption1 = TodoUtil::split(type.answerOption1, '^');
            random_shuffle(answerOption1.begin(), answerOption1.end(), [](int n) { return rand() % n; });
            type.answerOption1 = getJoinString(answerOption1, "^");
    
        }
        
        else continue;
        
        CCLOG("template type %d done : %s", type.type, type.answer.c_str());
        _problems.push_back(type);
    }

    return _problems;
    
}

string ProblemBank::checkRandom(string buf) {
    
    if (buf.find(',') != string::npos) {
        auto bufV = TodoUtil::split(buf, ',');
        random_shuffle(bufV.begin(), bufV.end(), [](int n) { return rand() % n; });
        return bufV[0];
    }
    
    auto bufVec = TodoUtil::split(buf, '~');
    if (bufVec.size() < 2) return buf;
    return TodoUtil::itos(random<int>(TodoUtil::stoi(bufVec[0]), TodoUtil::stoi(bufVec[1])));
}

vector<string> ProblemBank::parseFomula(string fomula) {
    vector<string> ret;
    ret.clear();
    
    string buf = "";
    for (int i=0; i<fomula.size(); i++) {
        if (fomula[i] == '+' || fomula[i] == '-'  || fomula[i] == 'x'  || fomula[i] == '=') {
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
        } else if (fomula.at(i) == "*") {
            rt *= TodoUtil::stoi(fomula.at(i+1));
            i++;
            continue;
        } else if (fomula.at(i) == "x") {
            rt *= TodoUtil::stoi(fomula.at(i+1));
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

string ProblemBank::getJoinString(vector<string> strings, string separator) {
    string rt = "";
    for (auto it : strings) {
        if (rt != "") rt += separator;
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

bool ProblemBank::hasSameAnswer(string templateName, string questionOption1, string answer) {
    for (auto it : _problems) {
        if (templateName == it.templateName && questionOption1 == it.questionOption1 && it.answer == answer) return true;
    }
    return false;
}

bool ProblemBank::checkReviewFail(EggQuizTemplateStruct type) {
    _reviewCount++;
    if (_reviewCount == 10000) {
        NativeAlert::show("problem review failed", TodoUtil::itos(type.type) + "," + type.templateName + "," + type.questionOption1 + "," + type.questionOption2 + "," + type.questionOption3 + "," + type.answer + "," + type.answerOption1, "OK");
        _reviewCount = 0;
        return true;
    }
    return false;
}

END_NS_EGGQUIZ;

