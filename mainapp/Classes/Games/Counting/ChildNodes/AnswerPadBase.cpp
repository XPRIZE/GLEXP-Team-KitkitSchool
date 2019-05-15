//
//  AnswerPadBase.cpp
//  KitkitSchool
//
//  Created by Gunho Lee on 6/27/16.
//
//

#include "AnswerPadBase.hpp"



bool AnswerPadBase::init()
{
    if (!Node::init())
    {
        return false;
    }

    return true;
}
