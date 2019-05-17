/////////////////////////////////////////////////////////////////////////////////////////
/****************************************************************************************
Created		:	2018.02.08

Author		:	Mikey.p

Purpose		:	Util

****************************************************************************************/
/////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "cocos2d.h"

USING_NS_CC;

class MMUtil
{
public:
	static void GetStringVector(std::vector<std::string>& result, const std::string& szData, const char& cSlicer);
	static void ParseCSV(std::vector<std::vector<std::string>>& result, const std::string& szData);
	static void ParseTSV(std::vector<std::vector<std::string>>& result, const std::string& szData);
	static std::string ReplaceReturn(std::string source);
	static std::string ReplaceString(std::string source, const std::string from, const std::string to);
private:
};

class AsyncLoader : public Node
{
public:
	CREATE_FUNC(AsyncLoader);

	virtual bool init() override;

	void Reset();

	// 로딩할 파일 등록
	void AddAsyncLoadFile(const std::string& fileName);
	// 로딩 완료 콜백. 로딩할 파일을 모두 등록 후 세팅해야 함.
	void SetCompleteCallBack(std::function<void()> cbf);
	// 로딩중인지 확인
	bool IsLoading() { return loadingFiles.size() > 0; }

private:
	void RunCompleteCallBack();

	std::set<std::string> loadingFiles;
	std::function<void()> completeCallBackFunction = nullptr;

};
