//
//  Namespaces.h on Apr 26, 2016
//  TodoSchool
//
//  Copyright (c) 2016 Enuma, Inc. All rights reserved.
//  See LICENSE.md for more details.
//

#ifndef __TODOSCHOOL__NAMESPACE_H__
#define __TODOSCHOOL__NAMESPACE_H__

#define NS_TODOSCHOOL_BEGIN namespace todoschool {
#define NS_TODOSCHOOL_END }
#define USING_NS_TODOSCHOOL using namespace todoschool

#define USING_WHATEVER \
    USING_NS_TODOSCHOOL; \
    USING_NS_CC; \
    using namespace std

#endif //!defined(__TODOSCHOOL__NAMESPACE_H__)
