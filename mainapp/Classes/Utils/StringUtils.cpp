//
//  StringUtils.cpp on May 25, 2016
//  TodoSchool
//
//  Copyright (c) 2016 Enuma, Inc. All rights reserved.
//  See LICENSE.md for more details.
//

#include "StringUtils.h"
#include <algorithm>
#include <cocos/cocos2d.h>

using namespace cocos2d;
using namespace std;

namespace todoschool {

bool isDigit(char C) {
	return isdigit(C);
}

bool isUpper(char C) {
    return isupper(C);
}

bool isLower(char C) {
    return islower(C);
}

bool isPunct(char C) {
    return ispunct(C);
}

char toUpper(char C) {
    return (char)toupper(C);
}

char toLower(char C) {
    return (char)tolower(C);
}


bool isDigit(string S) {
    bool (*Worker)(char) = isDigit;
    return all_of(S.begin(), S.end(), Worker);
}

bool isUpper(string S) {
    bool (*Worker)(char) = isUpper;
    return all_of(S.begin(), S.end(), Worker);
}

bool isLower(string S) {
    bool (*Worker)(char) = isLower;
    return all_of(S.begin(), S.end(), Worker);
}

bool isPunct(string S) {
    bool (*Worker)(char) = isPunct;
    return all_of(S.begin(), S.end(), Worker);
}

string toUpper(string S) {
    char (*Worker)(char) = toUpper;
    transform(S.begin(), S.end(), S.begin(), Worker);
    return S;
}

string toLower(string S) {
    char (*Worker)(char) = toLower;
    transform(S.begin(), S.end(), S.begin(), Worker);
    return S;
}

string toURLEncoded(string S) {
    string Ret;
    for (char C : S)
        Ret += "%" + StringUtils::format("%02x", (int)C);
    
    return Ret;
}

Optional<int> parseInt(string S, int Base) {
    Optional<int> Ret;

    char* Pos = const_cast<char*>(S.c_str());
    int It = (int)std::strtol(S.c_str(), &Pos, Base);

    if (static_cast<size_t>(Pos - S.c_str()) != S.size()) {
        // NB(xenosoz, 2016): S have invalid literal for base `Base'.
        return Ret;
    }

    Ret.reset(It);
    return Ret;
}

Optional<int> parseIntWithSuffix(string S, string Suffix, int Base) {
    auto SuffixBegin = find_end(S.begin(), S.end(), Suffix.begin(), Suffix.end());

    return parseInt(string(S.begin(), SuffixBegin), Base);
}

}  // namespace todoschool