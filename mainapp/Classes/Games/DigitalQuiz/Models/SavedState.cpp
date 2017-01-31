//
//  SavedState.cpp -- Save and load the test progress
//  TodoSchool on Oct 13, 2016
//
//  Copyright (c) 2016 Enuma, Inc. All rights reserved.
//  See LICENSE.md for more details.
//


#include "SavedState.h"


BEGIN_NS_DIGITALQUIZ

SavedState::SavedState()
: Valid(false)
{
}

SavedState::SavedState(const string& LanguageTag,
                       int LevelID,
                       int WorksheetID,
                       const string& Classroom,
                       int StudentNumber,
                       const string& CourseKind)
: Valid(true)
, LanguageTag(LanguageTag)
, LevelID(LevelID)
, WorksheetID(WorksheetID)
, Classroom(Classroom)
, StudentNumber(StudentNumber)
, CourseKind(CourseKind)
, ProblemID(-1)
, TestClock(0)
{
}

bool SavedState::empty() {
    pull();

    if (ProblemID == -1) { return true; }
    return false;
}

void SavedState::clear() {
    ProblemID = -1;
    TestClock = 0.f;
    deleteInstanceFromTheDatabase();
}

int SavedState::savedProblemID() {
    pull();

    return ProblemID;
}

float SavedState::savedTestClock() {
    pull();
    
    return TestClock;
}

void SavedState::updateProblemID(int ProblemID) {
    this->ProblemID = ProblemID;
    push();
}

void SavedState::updateTestClock(float TestClock) {
    this->TestClock = TestClock;
    push();
}

string SavedState::keyFromContext() {
    if (!Valid) {
        CCLOGERROR("Using not valid SavedState(%p) in %s", this, __PRETTY_FUNCTION__);
        assert(Valid);
    }

    stringstream SS;
    
    SS << "DigitalQuiz_";
    SS << "SavedState_";
    SS << LanguageTag << "_";
    SS << LevelID << "_";
    SS << WorksheetID << "_";
    SS << Classroom << "_";
    SS << StudentNumber << "_";

    SS << CourseKind;

    return SS.str();
}

void SavedState::pull() {
    auto Dump = UserDefault::getInstance()->getStringForKey(keyFromContext().c_str());

    Json::Value JV(Json::nullValue);
    Json::Reader().parse(Dump, JV);

    if (JV.type() != Json::nullValue && JV.type() != Json::objectValue) {
        // XXX: Type mismatch -> cleanup.
        deleteInstanceFromTheDatabase();
        JV = Json::Value(Json::nullValue);
    }

    ProblemID = JV.get("ProblemID", -1).asInt();
    TestClock = JV.get("TestClock", 0.f).asFloat();
}

void SavedState::push() {
    Json::Value JV;

    JV["ProblemID"] = ProblemID;
    JV["TestClock"] = TestClock;
    
    auto Dump = Json::FastWriter().write(JV);
    UserDefault::getInstance()->setStringForKey(keyFromContext().c_str(),
                                                Dump);
}

void SavedState::deleteInstanceFromTheDatabase() {
    UserDefault::getInstance()->deleteValueForKey(keyFromContext().c_str());
}

END_NS_DIGITALQUIZ
