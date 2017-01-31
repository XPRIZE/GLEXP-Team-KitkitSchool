//
//  OneBasedVector.h -- 1-base vector
//  TodoSchool on Oct 21, 2016
//
//  Copyright (c) 2016 Enuma, Inc. All rights reserved.
//  See LICENSE.md for more details.
//

#pragma once

#include "SkewedVector.h"
#include "TodoSchoolNS.h"


BEGIN_NS_TODOSCHOOL

template <typename T>
using OneBasedVector = SkewedVector<T, 1>;

END_NS_TODOSCHOOL