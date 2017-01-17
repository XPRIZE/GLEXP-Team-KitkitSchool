//
//  StringUtils.h on May 25, 2016
//  TodoSchool
//
//  Copyright (c) 2016 Enuma, Inc. All rights reserved.
//  See LICENSE.md for more details.
//

#ifndef TODOSCHOOL_UTILS_STRINGUTILS_H
#define TODOSCHOOL_UTILS_STRINGUTILS_H

#include <Games/NumberTrace/Common/ADT/Optional.h>
#include <string>

namespace todoschool {

bool isDigit(char C);
bool isUpper(char C);
bool isLower(char C);
char toUpper(char C);
char toLower(char C);

bool isDigit(std::string S);
bool isUpper(std::string S);
bool isLower(std::string S);
bool isPunct(std::string S);

std::string toUpper(std::string S);
std::string toLower(std::string S);
std::string toURLEncoded(std::string S);

Optional<int> parseInt(std::string S, int Base = 10);
Optional<int> parseIntWithSuffix(std::string S, std::string Suffix, int Base = 10);

}

#endif //!defined(TODOSCHOOL_UTILS_STRINGUTILS_H)
