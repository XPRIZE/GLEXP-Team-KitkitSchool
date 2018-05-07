/////////////////////////////////////////////////////////////////////////////////////////
/****************************************************************************************
 Created        :    2018.02.21
 
 Author        :    Mikey.p
 
 Purpose        :    FeedingTime Node
 
 ****************************************************************************************/
/////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "cocos2d.h"

USING_NS_CC;

struct FeedingTimeCellData;
namespace cocostudio
{
	class Armature;
}

class FeedingTimeNode : public Node
{
public:
    virtual void Success() = 0;
    virtual void Fail() = 0;
    virtual void StartGame() = 0;
    virtual void NextGame() = 0;
    virtual void EndGame() = 0;
};

// 물음표와 동물
class FTCenterNode: public FeedingTimeNode
{
public:
    CREATE_FUNC(FTCenterNode);

	~FTCenterNode();

	virtual bool init() override;;
	virtual void Success() override;;
	virtual void Fail() override;;
	virtual void StartGame() override;;
	virtual void NextGame() override;;
	virtual void EndGame() override;;

	void SetData(int index, const std::string& armatureFile);
	void SetNextArmatureFile(const std::string armatureFile) { _nextArmatureFile = armatureFile; }

private:
	cocostudio::Armature* LoadArmature(std::string file);

	int _index = 0;
	std::string _armatureFile;
	std::string _nextArmatureFile;
	bool _changeArmature = true;
	std::set<std::string> _armatureFiles;
};

// 과일 혹은 숫자계산
class FTCellNode: public FeedingTimeNode
{
public:
    enum class Side
    {
        None = -1,
        Left,
        Right
    };

    CREATE_FUNC(FTCellNode);
	virtual bool init() override;;
	virtual void Success() override;;
	virtual void Fail() override;;
	virtual void StartGame() override;;
	virtual void NextGame() override;;
	virtual void EndGame() override;;
    
	void EatAll();
	void EatIt();
	void FallDown();

	void SetData(const Side& type, const FeedingTimeCellData& data);
private:
	std::map<int, std::vector<Vec2>> _positionMap;
	std::map<std::string, Rect> _stringSpriteRect;
    Side _side = Side::None;
    int _count = 0;
	int _count2 = 0;
	std::string _relation;
	std::string _type;
};

// 부등호와 등호
class FTRelationNode: public FeedingTimeNode
{
public:
    enum class Type
    {
        None = -1,
		Lager,
        Equal,
		Smaller
	};

	enum class Nodes
	{
		None = -1,
		BG,
		main,
		correct,
		lager,
		lagerAnswer,
		lagerDisabled,
		smaller,
		smallerAnswer,
		smallerDisabled,
		equal,
		equalAnswer,
		equalDisabled,
		disabled,
		normalShade,
		moveShade
	};

    CREATE_FUNC(FTRelationNode);
	virtual bool init() override;;
	virtual void Success() override;;
	virtual void Fail() override;;
	virtual void StartGame() override;;
	virtual void NextGame() override;;
	virtual void EndGame() override;;
    
	void GoCenter(bool success, std::function<void()> callback);

	void SetData(const Type& type, const Type& resultType, const int& count);;
	void SetClicked(bool clicked) { _clicked = clicked; }
	void SetCallBack(std::function<void()> callback) { _callback = callback; };

private:
    Type _type = Type::None;
	bool _clicked = false;
	std::function<void()> _callback = nullptr;
	std::map<Nodes, Node*> _nodes;

	Type _resultType = Type::None;
	int _count = 0;
};
