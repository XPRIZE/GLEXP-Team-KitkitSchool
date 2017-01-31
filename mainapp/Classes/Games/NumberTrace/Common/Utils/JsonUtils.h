//
//  JsonUtils.h on Jun 17, 2016
//  TodoSchool
//
//  Copyright (c) 2016 Enuma, Inc. All rights reserved.
//  See LICENSE.md for more details.
//

#ifndef TODOSCHOOL_UTILS_JSONUTILS_H
#define TODOSCHOOL_UTILS_JSONUTILS_H

#include <Games/NumberTrace/Common/Basic/JsonFwd.h>
#include <Games/NumberTrace/Common/ADT/Optional.h>
#include <string>

namespace todoschool {

Optional<Json::Value> jsonFromFile(const std::string& Filename);

}  // namespace todoschool

#endif // !defined(TODOSCHOOL_UTILS_JSONUTILS_H)
