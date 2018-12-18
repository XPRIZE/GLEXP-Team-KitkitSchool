//
//  Problem.cpp on Sep 5, 2016
//  TodoSchool
//
//  Copyright (c) 2016 Enuma, Inc. All rights reserved.
//  See LICENSE.md for more details.
//


#include "FindTheMatchProblem.h"
#include <Common/Basic/CommentStream.h>
#include <Utils/Random.h>


BEGIN_NS_FINDTHEMATCH

namespace {
}  // unnamed namespace


Problem::Problem()
: NumberOfChoices(0)
, AnswerID() {}

Problem::Problem(size_t NumberOfChoices,
                 const string& AnswerStr,
                 const string& Choice0,
                 const string& Choice1,
                 const string& Choice2,
                 const string& Choice3)
: NumberOfChoices(NumberOfChoices)
{
    if (AnswerStr == "RANDOM")
        AnswerID = random((size_t)1, NumberOfChoices) - 1;
    else
        AnswerID = findthematch::stoi(AnswerStr);

    if (Choice0 != "-") {
        Choices.resize(0);
        Choices.push_back(Choice0);
    }
    
    if (Choice1 != "-") {
        Choices.resize(1);
        Choices.push_back(Choice1);
    }
    
    if (Choice2 != "-") {
        Choices.resize(2);
        Choices.push_back(Choice2);
    }
    
    if (Choice3 != "-") {
        Choices.resize(3);
        Choices.push_back(Choice3);
    }
}

string Problem::assetForProblemCard() const {
    if (NumberOfChoices == 0 || Choices.size() <= AnswerID) { return ""; }
    return Choices[AnswerID];
}

string Problem::assetForChoiceCard(size_t CardID) const {
    if (NumberOfChoices == 0 || Choices.size() <= CardID) { return ""; }
    return Choices[CardID];
}

Problem Problem::randomWork() {
    vector<string> Choices = {
        "Ant",
        "Bee",
        "Beetle",
        "BlueButterfly",
        "Ladybug",
        "Moth",
        "Spider",
        "StagBeetle",
        "YellowButterfly",
    };
    
    size_t MinCount = 2;
    size_t MaxCount = 4;
    size_t ChoiceCount = random(MinCount, MaxCount);
    size_t Answer = random((size_t)1, ChoiceCount) - 1;

    auto C = sample(Choices, ChoiceCount);
    for (size_t I = ChoiceCount; I < MaxCount; ++I)
        C.push_back("-");
    
    return Problem(ChoiceCount, itos((int)Answer), C[0], C[1], C[2], C[3]);
}

Problem Problem::fromInputStream(istream& IS) {
    size_t ChoiceCount;
    string Answer;
    string C0, C1, C2, C3;
    
    if (IS >> ChoiceCount >> Answer >> C0 >> C1 >> C2 >> C3)
        return Problem(ChoiceCount, Answer, C0, C1, C2, C3);
    
    // NB(xenosoz, 2016): Fallback to a random work
    CCLOGERROR("Something bad happen in %s", __PRETTY_FUNCTION__);
    return Problem::randomWork();
}

istream& operator>>(istream& IS, Problem& Work) {
    Work = Problem::fromInputStream(IS);
    return IS;
}

END_NS_FINDTHEMATCH
