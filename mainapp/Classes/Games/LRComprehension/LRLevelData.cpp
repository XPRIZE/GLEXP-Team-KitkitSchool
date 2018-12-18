 //
//  LRLevelData.cpp
//  KitkitSchool
//
//  Created by HyeonGyu Yu on 8/24/18.
//

#include "LRLevelData.hpp"
#include "Utils/TodoUtil.h"

vector<LRProblem*> LRLevelData::getRandomProblemsByLevel(int level)
{
    map<int, vector<LRProblem*>> problemsByWorksheet;
    
    for (const auto problem : problems)
    {
        if (level != problem->level)
            continue;

        problemsByWorksheet[problem->worksheet].push_back(problem);
    }
    
    int index = random(1, (int)problemsByWorksheet.size());
    return problemsByWorksheet[index];
}

LRLevelData* LRLevelData::parse()
{
    auto result = new LRLevelData();
    result->problems.clear();
    
    auto rawString = FileUtils::getInstance()->getStringFromFile("lrcomprehension/lrcomprehension_levels.tsv");
    auto data = TodoUtil::readTSV(rawString);
    
    for (auto row : data)
    {
        if (row.size() < 2) continue;
        if (row[0][0] == '#') continue;
        
        LRProblem* pb = new LRProblem();

        CCLOG("%s", row[4].c_str());
        
        if (TodoUtil::trim(row[(int)Col::QuestionType]).empty())
            continue;
        
        if (LanguageManager::getInstance()->getCurrentLanguageTag() != row[(int)Col::LanguageTag])
            continue;
        
        pb->languageTag = row[(int)Col::LanguageTag];
            
        pb->level = TodoUtil::stoi(row[(int)Col::Level]);
        pb->worksheet = TodoUtil::stoi(row[(int)Col::WorkSheet]);
        pb->problem = TodoUtil::stoi(row[(int)Col::Problem]);
        pb->questionType = getProblemType(row[(int)Col::QuestionType]);
        pb->script = row[(int)Col::Script];
        pb->question = row[(int)Col::Question];
        pb->audio = row[(int)Col::Audio];
        pb->answerType = row[(int)Col::AnswerType];
        pb->option1 = row[(int)Col::Option1];
        pb->option2 = row[(int)Col::Option2];
        pb->option3 = row[(int)Col::Option3];
        pb->option4 = row[(int)Col::Option4];
        pb->answer = row[(int)Col::Answer];
        
        result->problems.push_back(pb);
    }
    return result;
}

LRComprehensionType LRLevelData::getProblemType(string rowValue)
{
    LRComprehensionType returnType;
    
    if (rowValue == "lcallinone")
    {
        returnType = LRComprehensionType::LCAllInOne;
    }
    else if (rowValue == "rcallinone")
    {
        returnType = LRComprehensionType::RCAllInOne;
    }
    else if (rowValue == "title")
    {
        returnType = LRComprehensionType::TitlePage;
    }
    else if (rowValue == "listeningonly")
    {
        returnType = LRComprehensionType::ListeningOnly;
    }
    else if (rowValue == "readingonly")
    {
        returnType = LRComprehensionType::ReadingOnly;
    }
    else if (rowValue == "multiplechoices")
    {
        returnType = LRComprehensionType::MultipleChoices;
    }
    else if (rowValue == "fillintheblank")
    {
        returnType = LRComprehensionType::FillTheBlank;
    }
    else if (rowValue == "ordering")
    {
        returnType = LRComprehensionType::Ordering;
    }
    else
    {
        returnType = LRComprehensionType::None;
    }
    
    return returnType;
}

