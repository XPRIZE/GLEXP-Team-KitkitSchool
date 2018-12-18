#include "WordMatrixInfo.h"
#include "Utils/TodoUtil.h"

std::vector<std::string> WordMatrixInfo::enumerateLevelIDs()
{
    int levelCnt = getMaxLevel();
    vector<std::string> ddLevels;
    for (int i=1; i<=levelCnt; i++) ddLevels.push_back(TodoUtil::itos(i));
    return ddLevels;
}

int WordMatrixInfo::getMaxLevel()
{
    int maxLevel = 0;
    string S = FileUtils::getInstance()->getStringFromFile(WM_TSV_PATH);
    auto data = TodoUtil::readTSV(S);
    
    int i = 0;
    map<int, vector<WORD_MATRIX_DATA>> questionInfoMap;
    while (i < data.size())
    {
        auto row = data[i++];
        int level = TodoUtil::stoi(row[1]);
        
        if(level > maxLevel)
        {
            maxLevel = level;
        }
    }
    
    return maxLevel;
}
