/////////////////////////////////////////////////////////////////////////////////////////
/****************************************************************************************
Created		:	2018.02.08

Author		:	Mikey.p

Purpose		:	Util

****************************************************************************************/
/////////////////////////////////////////////////////////////////////////////////////////

#include "MMUtil.h"

/////////////////////////////////////////////////////////////////////////////////////////
void MMUtil::GetStringVector(std::vector<std::string>& result, const std::string & szData, const char & cSlicer)
{
	auto size = szData.size();
    std::string source = szData;
	result.clear();

    std::string r = "\xef";
    if (source.find(r) != std::string::npos)
    {
        source = source.substr(3);
    }
    
	if (cSlicer == '{' || cSlicer == '[' || cSlicer == '(' || cSlicer == '<')
	{
		char cCloser = 0;
		if (cSlicer == '{')
			cCloser = '}';
		else if (cSlicer == '[')
			cCloser = ']';
		else if (cSlicer == '(')
			cCloser = ')';
		else if (cSlicer == '<')
			cCloser = '>';

		int nCurIndex = 0;
		do
		{
			int nNextIndex = source.find(cSlicer, nCurIndex);
			if (nNextIndex == std::string::npos)
				break;
			int nEndIndex = source.find(cCloser, nNextIndex);
			result.push_back(source.substr(nNextIndex + 1, nEndIndex - nNextIndex - 1));
			nCurIndex = nEndIndex;
		} while (true);
	}
	else
	{
        if (cSlicer == '\n')
        {
            size_t start_pos = 0;
            while((start_pos = source.find('\r', start_pos)) != std::string::npos)
            {
                source.replace(start_pos, 1, "");
            }
        }
        
		int nCurCommaPos = static_cast<int>(source.find(cSlicer));
		int nLastCommaPos = -1;

		while (nCurCommaPos != std::string::npos)
		{
			while (nLastCommaPos + 1 < size && source[nLastCommaPos + 1] == ' ')
				nLastCommaPos++;
			result.push_back(source.substr(nLastCommaPos + 1, nCurCommaPos - nLastCommaPos - 1));
			nLastCommaPos = nCurCommaPos;
			nCurCommaPos = static_cast<int>(source.find(cSlicer, nCurCommaPos + 1));
		}

		if (source != "")
		{
			while (nLastCommaPos + 1 < size && source[nLastCommaPos + 1] == ' ')
				nLastCommaPos++;
			auto last = source.substr(nLastCommaPos + 1);
			result.push_back(last);
		}
	}
}

void MMUtil::ParseCSV(std::vector<std::vector<std::string>>& result, const std::string & szData)
{
	result.clear();

	std::vector<std::string> lineBreak;
	GetStringVector(lineBreak, szData, '\n');

	for (auto it = lineBreak.begin(); it != lineBreak.end(); it++)
	{
		std::vector<std::string> cells;
		GetStringVector(cells, *it, ',');
		result.push_back(cells);
	}
}

void MMUtil::ParseTSV(std::vector<std::vector<std::string>>& result, const std::string & szData)
{
	result.clear();

	std::vector<std::string> lineBreak;
	GetStringVector(lineBreak, szData, '\n');

	for (auto it = lineBreak.begin(); it != lineBreak.end(); it++)
	{
		std::vector<std::string> cells;
		GetStringVector(cells, *it, '\t');
		result.push_back(cells);
	}
}

std::string MMUtil::ReplaceReturn(std::string source)
{
	return ReplaceString(source, "\\n", "\n");;
}

std::string MMUtil::ReplaceString(std::string source, const std::string from, const std::string to)
{
	size_t start_pos = 0;
	auto size = from.size();
	while ((start_pos = source.find(from, start_pos)) != std::string::npos)
	{
		source.replace(start_pos, size, to);
	}

	return source;
}

bool AsyncLoader::init()
{
	if (Node::init())
	{
		Reset();
		return true;
	}

	return false;
}

void AsyncLoader::Reset()
{
	loadingFiles.clear();
	completeCallBackFunction = nullptr;
}

void AsyncLoader::AddAsyncLoadFile(const std::string & fileName)
{
	int extPos = fileName.find('.');

	if (extPos == std::string::npos)
		return;

	if (loadingFiles.find(fileName) != loadingFiles.end())
		return;

	auto ext = fileName.substr(extPos);
	if (ext == ".jpg" || ext == ".png")
	{
		loadingFiles.insert(fileName);
		Director::getInstance()->getTextureCache()->addImageAsync(fileName, [this, fileName](Texture2D* pTexture) 
		{ 
			loadingFiles.erase(fileName);
			if (loadingFiles.empty())
				RunCompleteCallBack();
		});
	}
	else if (ext == ".csb" || ext == ".json" || ext == ".ExportJson")
	{
		//ArmatureDataManager::getInstance()->addArmatureFileInfoAsync(fileName, this, CC_SCHEDULE_SELECTOR(AsyncLoader::ArmatureLoadCallBack));
	}

}

void AsyncLoader::SetCompleteCallBack(std::function<void()> cbf)
{ 
	if (loadingFiles.empty())
		cbf();
	else
		completeCallBackFunction = cbf; 
}

void AsyncLoader::RunCompleteCallBack()
{
	if (completeCallBackFunction)
		completeCallBackFunction();

	completeCallBackFunction = nullptr;
}
