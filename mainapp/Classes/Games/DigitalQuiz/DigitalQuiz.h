//
//  DigitalQuiz.h -- Pre/post testing
//  TodoSchool on Oct 31, 2016
//
//  Copyright (c) 2016 Enuma, Inc. All rights reserved.
//  See LICENSE.md for more details.
//


#pragma once

#include <cocos/2d/CCScene.h>
#include "Models/LevelData.h"


class DigitalQuiz {
public:
    enum class CourseKind {
        PreTest,
        PostTest,
    };

public:
    std::string TheClassroom;
    int TheStudentNumber;
    CourseKind TheCourseKind;

public:
    DigitalQuiz();

    // NB(xenosoz, 2016): Adaptors for coding convention of upstream.
    void setClassroom(const std::string& classroom);
    void setStudentNumber(int studentNumber);
    void setCourseKind(CourseKind courseKind);

    cocos2d::Scene* createScene();
};
