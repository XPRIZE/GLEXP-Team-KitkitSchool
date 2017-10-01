//
//  CommentStream.cpp -- (Input) stream which ignores '#' comment strings
//  TodoSchool on Sep 30, 2016
//
//  Copyright (c) 2016 Enuma, Inc. All rights reserved.
//  See LICENSE.md for more details.
//

#include "CommentStream.hpp"
#include <regex>
#include <iostream>



using namespace std;

namespace {
    // NB(xenosoz, 2016): Remove comments line by line. We don't support to escape "\#" now.
    string sanitizedString(const string& S) {
        stringstream OutSS;
        regex CommentRegex("#(.|\\s)*$");
        
        stringstream InSS(S);
        string Line;
        string LineFeed;
        
        while (getline(InSS, Line)) {
            Line = regex_replace(Line, CommentRegex, "");
            
            OutSS << Line << LineFeed;
            LineFeed = "\n";
        }
        
        return OutSS.str();
    }
}  // unnamed namespace


CommentStream::CommentStream() {
}

CommentStream::CommentStream(const std::string& S) {
    ISS.str(sanitizedString(S));
}

CommentStream::operator bool() const {
    return bool(ISS);
}

locale CommentStream::getloc() const {
    return ISS.getloc();
}

locale CommentStream::imbue(const std::locale& loc) {
    return ISS.imbue(loc);
}


