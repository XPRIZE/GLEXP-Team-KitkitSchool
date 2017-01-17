//
//  SavedState.h -- Save and load the test progress
//  TodoSchool on Oct 13, 2016
//
//  Copyright (c) 2016 Enuma, Inc. All rights reserved.
//  See LICENSE.md for more details.
//


#pragma once

#include "../Utils/MainNS.h"


BEGIN_NS_DIGITALQUIZ

class SavedState {
public:
    bool Valid;

    // NB(xenosoz, 2016): The context.
    string LanguageTag;
    int LevelID;
    int WorksheetID;
    string Classroom;
    int StudentNumber;
    string CourseKind;
    
    // NB(xenosoz, 2016): The saved state.
    // NB(xenosoz, 2016): We use 'ProblemID == -1' as the magical empty state.
    int ProblemID;
    float TestClock;

public:
    SavedState();
    SavedState(const string& LanguageTag,
               int LevelID,
               int WorksheetID,
               const string& Classroom,
               int StudentNumber,
               const string& CourseKind);

    bool empty();
    void clear();

    int savedProblemID();
    float savedTestClock();

    void updateProblemID(int ProblemID);
    void updateTestClock(float TestClock);

private:
    string keyFromContext();

    void pull();
    void push();
    void deleteInstanceFromTheDatabase();
};

END_NS_DIGITALQUIZ
