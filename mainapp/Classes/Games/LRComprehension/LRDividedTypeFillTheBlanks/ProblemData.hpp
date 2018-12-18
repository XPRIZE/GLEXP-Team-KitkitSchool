//
//  ProblemData.hpp
//  KitkitSchool
//
//  Created by timewalker on 13/12/2016.
//
//

#include "LRDividedTypeFillTheBlanksHeader.hpp"

BEGIN_NS_LR_DIVIDED_TYPE_FILL_THE_BLANKS;

class Word
{
public:
    std::string value;
    bool bBlank;
    
};

class ProblemData
{
public:
    std::string textOrSoundPath;
    std::vector<Word*> wordSet;
    std::vector<std::string> wrongWordSet;
    std::string soundPath;
    
    static ProblemData* parse(std::vector<std::string> rawData);
};

END_NS_LR_DIVIDED_TYPE_FILL_THE_BLANKS;
