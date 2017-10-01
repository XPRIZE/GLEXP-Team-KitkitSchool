//
//  PatternTrainProblemBank.cpp
//  enumaXprize
//
//  Created by JungJaehun on 08/09/2017.
//
//

#include "PatternTrainProblemBank.hpp"

BEGIN_NS_PATTERNTRAIN;

vector<string> patternSequence1 = {"A-A", "A-B"};
vector<string> patternSequence2 = {"A-A-B", "A-B-B", "A-B-C"};
vector<string> patternSequence3 = {"A-A-B", "A-B-B", "A-B-C"};
vector<string> patternSequence4 = {"A-A-B", "A-B-B", "A-B-C", "A-B"};
vector<string> patternSequence5 = {"A-A-B", "A-B-B", "A-B-C", "A-B"};

vector<string> patternFigure = {
    "train_pattern_block_shape_1",
    "train_pattern_block_shape_2",
    "train_pattern_block_shape_3",
    "train_pattern_block_shape_4",
    "train_pattern_block_shape_5",
    "train_pattern_block_shape_6"
};

vector<string> patternNumber1 = {
    "train_pattern_block_1_1",
    "train_pattern_block_1_2"
};

vector<string> patternNumber2 = {
    "train_pattern_block_2_1",
    "train_pattern_block_2_2"
};

vector<string> patternNumber3 = {
    "train_pattern_block_3_1",
    "train_pattern_block_3_2"
};

vector<string> patternAlphabetA = {
    "train_pattern_block_a_1",
    "train_pattern_block_a_2"
};

vector<string> patternAlphabetB = {
    "train_pattern_block_b_1",
    "train_pattern_block_b_2"
};
vector<string> patternAlphabetC = {
    "train_pattern_block_c_1",
    "train_pattern_block_c_2"
};


vector<string> patternSize = {
    "train_pattern_block_SL_shape1_large",
    "train_pattern_block_SL_shape2_large",
    "train_pattern_block_SL_shape3_large",
    "train_pattern_block_SL_shape4_large",
    "train_pattern_block_SL_shape5_large",
    "train_pattern_block_SL_shape6_large"
};

PatternTrainProblemBank::PatternTrainProblemBank() {
    
}

string getRandomItemOne(vector<string> stringSet) {
    random_shuffle(stringSet.begin(), stringSet.end(), [](int n) { return rand() % n; });
    return stringSet[0];
}

vector<string> PatternTrainProblemBank::getShapeVec(shape shapeKey) {
    vector<string> ret;
    int typeChoice = rand()%2;

    switch(shapeKey) {
        case shape::FIGURE:
            random_shuffle(patternFigure.begin(), patternFigure.end(), [](int n) { return rand() % n; });
            ret = patternFigure;
            break;
        case shape::NUMBERALPHABET:
            if (rand()%2 == 0) { // number
                ret.insert(ret.end(), patternNumber1[typeChoice]);
                ret.insert(ret.end(), patternNumber2[typeChoice]);
                ret.insert(ret.end(), patternNumber3[typeChoice]);
            } else { // alphabet
                ret.insert(ret.end(), patternAlphabetA[typeChoice]);
                ret.insert(ret.end(), patternAlphabetB[typeChoice]);
                ret.insert(ret.end(), patternAlphabetC[typeChoice]);
            }
            random_shuffle(ret.begin(), ret.end(), [](int n) { return rand() % n; });
            break;
        case shape::SIZE:
            random_shuffle(patternSize.begin(), patternSize.end(), [](int n) { return rand() % n; });
            
            ret.push_back(patternSize[0]);
            ret.push_back(patternSize[0] + "_downscale1");
            ret.push_back(patternSize[0] + "_downscale2");
            random_shuffle(ret.begin(), ret.end(), [](int n) { return rand() % n; });
            break;
    }
    return ret;
}

Json::Value PatternTrainProblemBank::generateProblems(int level){
    

    Json::Value problems;
    Json::Value jPattern(Json::arrayValue);
    Json::Value jShape;
    Json::Value jBlank;
    Json::Value jChoice;

    srand((unsigned)time(0));
    
    switch(level) {
        case 1:
            jPattern.append("A-A");
            jPattern.append("A-B");
            for (int i=2; i<5; i++) { jPattern.append(patternSequence1[rand()%patternSequence1.size()]); }
            jShape = shape::FIGURE;
            jBlank = blank::LAST;
            jChoice = choice::AB;
            break;
        case 2:
            for (int i=0; i<5; i++) { jPattern.append(patternSequence2[rand()%patternSequence2.size()]); }
            jShape = shape::FIGURE;
            jBlank = blank::LAST;
            jChoice = choice::ABC;
            break;
        case 3:
            for (int i=0; i<5; i++) { jPattern.append(patternSequence3[rand()%patternSequence3.size()]); }
            jShape = shape::FIGURE;
            jBlank = blank::RANDOM;
            jChoice = choice::ABC;
            break;
        case 4:
            for (int i=0; i<5; i++) { jPattern.append(patternSequence4[rand()%patternSequence4.size()]); }
            jShape = shape::SIZE;
            jBlank = blank::ALL;
            jChoice = choice::UNLIMITEDABC;
            break;
        case 5:
            for (int i=0; i<5; i++) { jPattern.append(patternSequence5[rand()%patternSequence5.size()]); }
            jShape = shape::NUMBERALPHABET;
            jBlank = blank::ALL;
            jChoice = choice::UNLIMITEDABC;
            break;
    }
    

    problems["pattern"] = jPattern;
    problems["blank"] = jBlank;
    problems["choice"] = jChoice;
    problems["shape"] = jShape;

    Json::StyledWriter writer;
    CCLOG("%s", writer.write(problems).c_str());

    return problems;
}

END_NS_PATTERNTRAIN;
