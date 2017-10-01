//
//  LetterTracingCardUtility.cpp
//  enumaXprize
//
//  Created by HyeonGyu Yu on 9/15/17.
//
//

#include "LetterTracingCardUtility.hpp"
#include <vector>


int LetterTracingCardUtility::getLevelByLetter(std::string letter, bool isEnglish)
{
    std::vector<std::string> levelDataEnArray = {"a","b","c","d","e","f","g","h","i","j","k","l","m","o","p","q","r","s","t","u","v","w","z","y","z"};
    std::vector<std::string> levelDataSwArray = {"a","b","ch","d","e","f","g","h","i","j","k","l","m","n","o","p","r","s","t","u","v","w","y","z","sh","ng'","ng","th","gh", "dh"};
    
    auto targetArray = isEnglish ? levelDataEnArray : levelDataSwArray;
    for (int i = 0; i < targetArray.size(); i++)
    {
        if (letter == targetArray[i])
            return i + 1;
    }
    
    return -1;
}
