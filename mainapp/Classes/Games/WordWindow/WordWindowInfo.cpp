#include "WordWindowInfo.h"
#include "Utils/TodoUtil.h"

std::vector<std::string> WordWindowInfo::enumerateLevelIDs()
{
    vector<std::string> ddLevels;

	string P = "Games/wordwindow/wordwindow_level.tsv";
	string S = FileUtils::getInstance()->getStringFromFile(P);
	auto data = TodoUtil::readTSV(S);

	std::vector<WordWindowLevelStruct> vecDatas;
	std::set<int> setWorkSheets;

	int rowIndex = 0;
	int maxLevel = 0;
	while (rowIndex < (int)data.size())
	{
		auto row = data[rowIndex++];

		int level = TodoUtil::stoi(row[1]);
		if (level > maxLevel)
		{
			maxLevel = level;
		}
	}

	for (int i = 1; i <= maxLevel; i++)
	{
		ddLevels.push_back(TodoUtil::itos(i));
	}

	return ddLevels;
}