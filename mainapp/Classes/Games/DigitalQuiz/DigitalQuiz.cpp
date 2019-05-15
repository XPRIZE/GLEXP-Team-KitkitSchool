//
//  DigitalQuiz.cpp -- Pre/post testing
//  TodoSchool on Oct 31, 2016
//
//  Copyright (c) 2016 Enuma, Inc. All rights reserved.
//  See LICENSE.md for more details.
//


#include "DigitalQuiz.h"
#include "Core/DigitalQuizScene.h"
#include "Models/DigitalQuizLevelData.h"
#include "Models/SavedState.h"
#include <Managers/LanguageManager.hpp>
#include <numeric>

using todoschool::digitalquiz::LevelData;
using todoschool::digitalquiz::SavedState;


DigitalQuiz::DigitalQuiz()
{
}

cocos2d::Scene* DigitalQuiz::createScene() {
    using namespace todoschool::digitalquiz;

    // NB(xenosoz, 2016): DigitalQuiz is in Swahili only.
    auto Lang = string("sw-TZ");
    auto StudentName = format("%s%d", TheClassroom.c_str(), TheStudentNumber);

    // NB(xenosoz, 2016): We use the same level for pre/post testings.
    auto LevelID = 1;
    auto WorksheetID = 1;
    auto CourseKindString = (TheCourseKind == CourseKind::PreTest ? "PreTest" :
                             (TheCourseKind == CourseKind::PostTest ? "PostTest" :
                              "Unknown"));

    auto Sheet = LevelData::defaultData().randomSheetFor(Lang, LevelID);
    SavedState TheSavedState(Lang,
                             LevelID,
                             WorksheetID,
                             TheClassroom,
                             TheStudentNumber,
                             CourseKindString);

    auto It = DigitalQuizScene::create();

    It->TheWorksheet.update(Sheet);
    It->TheStudentName.update(StudentName);
    It->TheSavedState.update(TheSavedState);

    return It;
}

void DigitalQuiz::setClassroom(const std::string& classroom) {
    TheClassroom = classroom;
}

void DigitalQuiz::setStudentNumber(int studentNumber) {
    TheStudentNumber = studentNumber;
}

void DigitalQuiz::setCourseKind(CourseKind courseKind) {
    TheCourseKind = courseKind;
}

