//
//  LetterTracingCardUtility.hpp
//  KitkitSchool
//
//  Created by HyeonGyu Yu on 9/15/17.
//
//

#ifndef LetterTracingCardUtility_hpp
#define LetterTracingCardUtility_hpp

#include <stdio.h>
#include <string>

class LetterTracingCardUtility
{
public:
    static int getLevelByLetter(std::string letter, bool isEnglish);
};

#endif /* LetterTracingCardUtility_hpp */
