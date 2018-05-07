/////////////////////////////////////////////////////////////////////////////////////////
/****************************************************************************************
 Created        :    2018.02.21
 
 Author        :    Mikey.p
 
 Purpose        :    FeedingTime Node
 
 ****************************************************************************************/
/////////////////////////////////////////////////////////////////////////////////////////

#include "FeedingTimeNode.h"

#include "ui/CocosGUI.h"
#include "cocostudio/CocoStudio.h"
#include "FeedingTimeScene.h"
#include "Managers/GameSoundManager.h"

using namespace cocostudio;

#define APPLE_WIDE 124.f
#define APPLE_HEIGHT 120.f
#define APPLE_BASE_POS Vec2(0.f, 0.f)
#define NUMBER_BASE_POS Vec2(0.f, 50.f)

#define APPLE_SPAWN_TIME 0.3f
#define APPLE_TOTAL_TIME 0.7f
#define APPLE_EAT_TIME 0.4f

#define RELATION_MOVE_TIME 0.15f
#define SUCCESS_WAIT_TIME 2.f

/////////////////////////////////////////////////////////////////////////////////////////
FTCenterNode::~FTCenterNode()
{
	for (auto& file : _armatureFiles)
		ArmatureDataManager::getInstance()->removeArmatureFileInfo(file);
}

bool FTCenterNode::init() 
{ 
	// 타겟 물음표
	if (auto sprite = Sprite::create("FeedingTime/whichisbigger_rsc_balloon.png"))
	{
		sprite->setPosition({ 0, 438 });
		sprite->setOpacity(0);
		addChild(sprite, 0, "balloon");
	}

	return true; 
}

void FTCenterNode::Success()
{
	if (auto pNode = getChildByName("balloon"))
	{
		Vector<FiniteTimeAction*> actions;
		actions.pushBack(DelayTime::create(APPLE_TOTAL_TIME));
		actions.pushBack(CallFunc::create([this] 
		{
			if (auto armature = dynamic_cast<Armature*>(getChildByName("armature")))
			{
				std::string anim;

				if (_index == 0)
					anim = "left";
				else if (_index == 1)
					anim = "equal";
				else if (_index == 2)
					anim = "right";
				if (auto animation = dynamic_cast<ArmatureAnimation*>(armature->getAnimation()))
					animation->play(anim);

				GameSoundManager::getInstance()->playEffectSound("FeedingTime/sfx/SFX_feedingTime_comeup.m4a");
			}
		}));

		actions.pushBack(Spawn::create(ScaleTo::create(0.f, 1.2f), FadeOut::create(0.f), nullptr));

		pNode->runAction(Sequence::create(actions));
	}
}

void FTCenterNode::Fail()
{
	runAction(Sequence::create(
		DelayTime::create(APPLE_SPAWN_TIME),
		CallFunc::create([this] 
		{
			if (auto armature = dynamic_cast<Armature*>(getChildByName("armature")))
			{
				if (auto animation = dynamic_cast<ArmatureAnimation*>(armature->getAnimation()))
					animation->play("wrong");
			}

		}),
		nullptr));
}

void FTCenterNode::StartGame()
{
	if (auto pNode = getChildByName("balloon"))
	{
		pNode->setScale(1);

		Vector<FiniteTimeAction*> actions;
		actions.pushBack(FadeIn::create(APPLE_SPAWN_TIME));

		pNode->runAction(Sequence::create(actions));
	}

	if (_changeArmature)
	{
		_changeArmature = false;
		if (_armatureFile.empty())
			return;

		auto pArmature = LoadArmature(_armatureFile);
		if (pArmature)
		{
			auto pAnimation = pArmature->getAnimation();
			pAnimation->setMovementEventCallFunc([pAnimation](Armature* armature, MovementEventType type, const std::string& id)
			{
				switch (type)
				{
				case cocostudio::START:
					break;
				case cocostudio::COMPLETE:
					if (id == "out")
						armature->removeFromParent();
					else if (id != "idle")
						pAnimation->play("idle");
					break;
				case cocostudio::LOOP_COMPLETE:
					break;
				default:
					break;
				}
			});

			pAnimation->play("in");
		}

		pArmature->setPosition({ 0, -128 });

		addChild(pArmature, 0, "armature");
	}
}

void FTCenterNode::NextGame() 
{
	if (_armatureFile != _nextArmatureFile)
	{
		if (auto armature = dynamic_cast<Armature*>(getChildByName("armature")))
		{
			auto pAnimation = armature->getAnimation();
			pAnimation->play("out");
		}
		_changeArmature = true;
	}
}

void FTCenterNode::EndGame() {}

void FTCenterNode::SetData(int index, const std::string& armatureFile)
{
	_index = index;
	_armatureFile = armatureFile;
}

Armature* FTCenterNode::LoadArmature(std::string file)
{
	std::string armatureName = file;
	auto iNum = armatureName.find_last_of("/") + 1;
	armatureName = armatureName.erase(0, iNum);
	iNum = armatureName.find_last_of(".");
	armatureName = armatureName.erase(iNum, armatureName.size());

	if (_armatureFiles.find(file) != _armatureFiles.end())
		return Armature::create(armatureName);;

	_armatureFiles.insert(file);

	ArmatureDataManager::getInstance()->addArmatureFileInfo(file);

	while (true)
	{
		// 기기에 따라 로딩이 안끝날 수도 있음.
		auto armatureDataManager = ArmatureDataManager::getInstance();
		if (armatureDataManager->getArmatureData(armatureName) && armatureDataManager->getAnimationData(armatureName))
		{
			break;
		}
		CCLOG("loading armature...");
	}

	return Armature::create(armatureName);
}

/////////////////////////////////////////////////////////////////////////////////////////
bool FTCellNode::init()
{
	_positionMap.clear();

	_positionMap[1] = 
	{
		{0, 0} 
	};

	_positionMap[2] =
	{
		{ - APPLE_WIDE * 0.5f, 0 },
		{ APPLE_WIDE * 0.5f, 0 }
	};

	_positionMap[3] =
	{
		{ -APPLE_WIDE * 0.5f, 0 },
		{ APPLE_WIDE * 0.5f, 0 },
		{ 0, APPLE_HEIGHT },
	};

	_positionMap[4] =
	{
		{ -APPLE_WIDE, 0 },
		{ 0, 0 },
		{ APPLE_WIDE, 0 },
		{ -APPLE_WIDE * 0.5f, APPLE_HEIGHT },
	};

	_positionMap[5] =
	{
		{ -APPLE_WIDE, 0 },
		{ 0, 0 },
		{ APPLE_WIDE, 0 },
		{ -APPLE_WIDE * 0.5f, APPLE_HEIGHT },
		{ APPLE_WIDE * 0.5f, APPLE_HEIGHT }
	};

	_positionMap[6] =
	{
		{ -APPLE_WIDE, 0 },
		{ 0, 0 },
		{ APPLE_WIDE, 0 },
		{ -APPLE_WIDE * 0.5f, APPLE_HEIGHT },
		{ APPLE_WIDE * 0.5f, APPLE_HEIGHT },
		{ 0, APPLE_HEIGHT * 2 }
	};

	_positionMap[7] =
	{
		{ -APPLE_WIDE * 1.5f, 0 },
		{ -APPLE_WIDE * 0.5f, 0 },
		{ APPLE_WIDE * 0.5f, 0 },
		{ APPLE_WIDE * 1.5f, 0 },
		{ -APPLE_WIDE, APPLE_HEIGHT },
		{ 0, APPLE_HEIGHT },
		{ APPLE_WIDE, APPLE_HEIGHT },
	};

	_positionMap[8] =
	{
		{ -APPLE_WIDE * 1.5f, 0 },
		{ -APPLE_WIDE * 0.5f, 0 },
		{ APPLE_WIDE * 0.5f, 0 },
		{ APPLE_WIDE * 1.5f, 0 },
		{ -APPLE_WIDE, APPLE_HEIGHT },
		{ 0, APPLE_HEIGHT },
		{ APPLE_WIDE, APPLE_HEIGHT },
		{ -APPLE_WIDE * 0.5f, APPLE_HEIGHT * 2 }
	};

	_positionMap[9] =
	{
		{ -APPLE_WIDE * 1.5f, 0 },
		{ -APPLE_WIDE * 0.5f, 0 },
		{ APPLE_WIDE * 0.5f, 0 },
		{ APPLE_WIDE * 1.5f, 0 },
		{ -APPLE_WIDE, APPLE_HEIGHT },
		{ 0, APPLE_HEIGHT },
		{ APPLE_WIDE, APPLE_HEIGHT },
		{ -APPLE_WIDE * 0.5f, APPLE_HEIGHT * 2 },
		{ APPLE_WIDE * 0.5f, APPLE_HEIGHT * 2 }
	};

	_positionMap[10] =
	{
		{ -APPLE_WIDE * 1.5f, 0 },
		{ -APPLE_WIDE * 0.5f, 0 },
		{ APPLE_WIDE * 0.5f, 0 },
		{ APPLE_WIDE * 1.5f, 0 },
		{ -APPLE_WIDE, APPLE_HEIGHT },
		{ 0, APPLE_HEIGHT },
		{ APPLE_WIDE, APPLE_HEIGHT },
		{ -APPLE_WIDE * 0.5f, APPLE_HEIGHT * 2 },
		{ APPLE_WIDE * 0.5f, APPLE_HEIGHT * 2 },
		{ 0, APPLE_HEIGHT * 3 }
	};

	float numberWidth = 196;
	float numberHeight = 288;
	_stringSpriteRect["*"] = { numberWidth * 0, 0, numberWidth, numberHeight };
	_stringSpriteRect["+"] = { numberWidth * 1, 0, numberWidth, numberHeight };
	_stringSpriteRect["/"] = { numberWidth * 2, 0, numberWidth, numberHeight };
	_stringSpriteRect["-"] = { numberWidth * 3, 0, numberWidth, numberHeight };

	_stringSpriteRect["0"] = { numberWidth * 6, 0, numberWidth, numberHeight };
	_stringSpriteRect["1"] = { numberWidth * 7, 0, numberWidth, numberHeight };
	_stringSpriteRect["2"] = { numberWidth * 8, 0, numberWidth, numberHeight };
	_stringSpriteRect["3"] = { numberWidth * 9, 0, numberWidth, numberHeight };
	_stringSpriteRect["4"] = { numberWidth * 10, 0, numberWidth, numberHeight };
	_stringSpriteRect["5"] = { numberWidth * 11, 0, numberWidth, numberHeight };
	_stringSpriteRect["6"] = { numberWidth * 12, 0, numberWidth, numberHeight };
	_stringSpriteRect["7"] = { numberWidth * 13, 0, numberWidth, numberHeight };
	_stringSpriteRect["8"] = { numberWidth * 14, 0, numberWidth, numberHeight };
	_stringSpriteRect["9"] = { numberWidth * 15, 0, numberWidth, numberHeight };

	if (!getChildByName("AppleNode"))
	{
		/* auto draw = DrawNode::create();
		draw->drawSolidRect({ -1000, -148 }, { 2000, 1000 }, Color4F::BLACK);
		auto mask = ClippingNode::create(draw);
		mask->setAlphaThreshold(0.5f);
		addChild(mask, 0, "AppleNode");*/
		addChild(Node::create(), 0, "AppleNode");
	}
		

	return true;
}

void FTCellNode::Success() 
{

}

void FTCellNode::Fail() {}

void FTCellNode::StartGame()
{
}

void FTCellNode::NextGame()
{

}

void FTCellNode::EndGame()
{
}

void FTCellNode::EatAll()
{
	GameSoundManager::getInstance()->playEffectSound("FeedingTime/sfx/feedingtime_ani_equal.m4a");

	if (auto node = getChildByName("AppleNode"))
	{
		auto visibleSize = Director::getInstance()->getVisibleSize();
		Vec2 origin = Director::getInstance()->getVisibleOrigin();
		Vec2 center = { visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y };

		auto children = node->getChildren();
		float timer = 0.f;
		auto targetPos = center - getPosition() + Vec2(0, 245);

		std::sort(children.begin(), children.end(), [targetPos](Node* lptr, Node* rptr)
		{
			float ldistance = (targetPos - lptr->getPosition()).length();
			float rdistance = (targetPos - rptr->getPosition()).length();

			return ldistance < rdistance;
		});

		for (auto child : children)
		{
			child->runAction(Sequence::create(
				DelayTime::create(timer),
				CallFunc::create([this, child]
			{
				if (auto sprite = Sprite::create("FeedingTime/whichisbigger_rsc_fx_dust.png"))
				{
					addChild(sprite);
					sprite->setPosition(child->getPosition());
					sprite->runAction(Sequence::create(
						Spawn::create(
							ScaleTo::create(APPLE_SPAWN_TIME, 1.2f),
							FadeOut::create(APPLE_SPAWN_TIME),
							nullptr),
						RemoveSelf::create(),
						nullptr));
				}
			}),
				Spawn::create(
					EaseInOut::create(MoveTo::create(APPLE_EAT_TIME, targetPos), 2.f),
					RotateBy::create(APPLE_EAT_TIME, 540),
					ScaleTo::create(APPLE_EAT_TIME, 0.5f),
					FadeOut::create(APPLE_EAT_TIME),
					nullptr),
				nullptr));
			timer += 0.05f;
		}

		schedule([this, targetPos](float)
		{
			if (auto sprite = Sprite::create("FeedingTime/whichisbigger_rsc_fx_speed.png"))
			{
				auto parent = getParent();
				parent->addChild(sprite);
				auto thisPos = getPosition();
				float rndAngle = random(0.f, (float)M_PI * 2.f);
				float rndDistance = random(0.f, APPLE_HEIGHT * 2);
				Vec2 pos = Vec2(cosf(rndAngle) * rndDistance, sinf(rndAngle) * rndDistance + targetPos.y);
				sprite->setPosition(pos + thisPos);
				sprite->setRotation(-(targetPos - pos).getAngle() / M_PI * 180);
				sprite->runAction(Sequence::create(
					Spawn::create(
						EaseInOut::create(MoveTo::create(0.5f, targetPos + thisPos), 2.f),
						FadeOut::create(0.5f),
						nullptr),
					RemoveSelf::create(),
					nullptr));
			}
		}, 0.05f, 7, 0.f, "speedLine");
	}
}

void FTCellNode::EatIt()
{
	if (auto node = getChildByName("AppleNode"))
	{
		node->runAction(Sequence::create(
			DelayTime::create(0.3f),
			CallFuncN::create([](Node* target) {GameSoundManager::getInstance()->playEffectSound("FeedingTime/sfx/feedingtime_ani.m4a"); }),
			DelayTime::create(0.2f),
			CallFuncN::create([](Node* target) 
		{
			auto children = target->getChildren();
			for (auto child : children)
			{
				child->removeFromParent();
			}
		}),
			nullptr
		));
	}
}

void FTCellNode::FallDown()
{
	if (auto node = getChildByName("AppleNode"))
	{
		for (auto child : node->getChildren())
		{
			child->runAction(MoveBy::create(APPLE_SPAWN_TIME, Vec2(0.f, -APPLE_HEIGHT * 6.f)));
		}
	}
}

void FTCellNode::SetData(const Side & side, const FeedingTimeCellData & data) 
{
	_side = side;
	_count = data.count;
	_count2 = data.count2;
	_type = data.type;
	_relation = data.operatorType;

	if (auto node = getChildByName("AppleNode"))
	{
		node->removeAllChildren();
		
		if (_type == "image")
		{
			std::string imageFile1, imageFile2;
			int index = random(0, 9);

			imageFile1 = StringUtils::format("FeedingTime/Objects/fruit_%d_0.png", index);
			imageFile2 = StringUtils::format("FeedingTime/Objects/fruit_%d_1.png", index);

			for (int i = 0; i < _count; i++)
			{
				std::string imageFile = imageFile1;
				if (_count > 2 && i % 2 == 1)
					imageFile = imageFile2;
				auto sprite = Sprite::create(imageFile);
				sprite->setPosition(_positionMap[_count][i] + APPLE_BASE_POS);
				sprite->setOpacity(0);
				sprite->runAction(Sequence::create(
					DelayTime::create(APPLE_TOTAL_TIME * i / _count),
					FadeIn::create(APPLE_SPAWN_TIME),
					nullptr));
				node->addChild(sprite, _count - i);
			}
		}
		else
		{
			float scale = 1.f;

			if (_stringSpriteRect.find(_relation) != _stringSpriteRect.end())
				scale = 0.65f;
			else if (_count > 999)
				scale = 0.65f;

			float totalWidth = 0.f;
			std::vector<Node*> nodes;

			auto SetNumber = [this, scale, &nodes, &totalWidth](int a)
			{
				std::vector<int> numbers;
				int number = a;

				do
				{
					numbers.push_back(number % 10);
					number /= 10;
				} while (number > 0);

				for (int i = numbers.size() - 1; i >= 0; i--)
				{
					if (auto sprite = Sprite::create("FeedingTime/Objects/obj_number.png", _stringSpriteRect[StringUtils::toString(numbers[i])]))
					{
						sprite->setScale(scale);
						sprite->setPositionX(totalWidth);
						nodes.push_back(sprite);
						totalWidth += sprite->getContentSize().width * scale;
					}
				}
			};

			SetNumber(_count);

			if (_stringSpriteRect.find(_relation) != _stringSpriteRect.end())
			{
				if (auto sprite = Sprite::create("FeedingTime/Objects/obj_number.png", _stringSpriteRect[_relation]))
				{
					sprite->setScale(scale);
					sprite->setPositionX(totalWidth);
					nodes.push_back(sprite);
					totalWidth += sprite->getContentSize().width * scale;
				}

				SetNumber(_count2);
			}

			totalWidth -= nodes[0]->getContentSize().width * scale;
			for (unsigned int i = 0; i < nodes.size(); i++)
			{
				nodes[i]->setPosition(_positionMap[1][0] + NUMBER_BASE_POS + Vec2(nodes[i]->getPositionX() - totalWidth / 2.f, 0));
				node->addChild(nodes[i], nodes.size() - i);
			}
		}
	}
}

/////////////////////////////////////////////////////////////////////////////////////////
bool FTRelationNode::init()
{
	auto touchSize = Size();

	auto AddNode = [this](Nodes nodeType, std::string name, std::string file, Node* parent = nullptr)
	{
		if (auto sprite = Sprite::create(file))
		{
			_nodes[nodeType] = sprite;
			if (name.find("smaller") != std::string::npos)
				sprite->setScaleX(-1);
			if (parent)
			{
				sprite->setPosition(parent->getContentSize() / 2.f);
				parent->addChild(sprite, 0, name);
			}
			else
				addChild(sprite, 0, name);
		}
	};

	AddNode(Nodes::disabled, "disabled", "FeedingTime/UI/whichisbigger_rsc_slot_disabled.png");
	AddNode(Nodes::lagerDisabled, "lagerDisabled", "FeedingTime/UI/whichisbigger_rsc_inequality_0_disabled.png", _nodes[Nodes::disabled]);
	AddNode(Nodes::smallerDisabled, "smallerDisabled", "FeedingTime/UI/whichisbigger_rsc_inequality_0_disabled.png", _nodes[Nodes::disabled]);
	AddNode(Nodes::equalDisabled, "equalDisabled", "FeedingTime/UI/whichisbigger_rsc_inequality_1_disabled.png", _nodes[Nodes::disabled]);

	if (auto sprite = Sprite::create("FeedingTime/UI/whichisbigger_rsc_slot_normal.png"))
	{
		_nodes[Nodes::main] = Node::create();
		_nodes[Nodes::BG] = sprite;
		_nodes[Nodes::main]->addChild(sprite, 0, "BG");
		touchSize = sprite->getContentSize();
		addChild(_nodes[Nodes::main], 0, "BGNode");

		if (auto shade = Sprite::create("FeedingTime/UI/whichisbigger_rsc_slot_normal_shade.png"))
		{
			_nodes[Nodes::normalShade] = shade;
			shade->setPositionY(-touchSize.height / 2.f);
			_nodes[Nodes::main]->addChild(shade, -1, "normalShadow");
		}

		if (auto shade = Sprite::create("FeedingTime/UI/whichisbigger_rsc_slot_selected_shade.png"))
		{
			_nodes[Nodes::moveShade] = shade;
			_nodes[Nodes::main]->addChild(shade, -1, "moveShadow");
		}

		if (auto correct = Sprite::create("FeedingTime/UI/whichisbigger_rsc_slot_correct.png"))
		{
			_nodes[Nodes::correct] = correct;
			_nodes[Nodes::main]->addChild(correct, 0, "correct");
		}
	}

	AddNode(Nodes::lager, "lager", "FeedingTime/UI/whichisbigger_rsc_inequality_0.png", _nodes[Nodes::main]);
	AddNode(Nodes::smaller, "smaller", "FeedingTime/UI/whichisbigger_rsc_inequality_0.png", _nodes[Nodes::main]);
	AddNode(Nodes::equal, "equal", "FeedingTime/UI/whichisbigger_rsc_inequality_1.png", _nodes[Nodes::main]);

	AddNode(Nodes::smallerAnswer, "smallerAnswer", "FeedingTime/UI/whichisbigger_rsc_inequality_0_answer.png");
	AddNode(Nodes::equalAnswer, "equalAnswer", "FeedingTime/UI/whichisbigger_rsc_inequality_1_answer.png");
	AddNode(Nodes::lagerAnswer, "lagerAnswer", "FeedingTime/UI/whichisbigger_rsc_inequality_0_answer.png");

	if (auto label = ui::Text::create("", "fonts/TodoSchoolV2.ttf", 440))
	{
		label->setTextColor(Color4B(255, 247, 237, 255));
		addChild(label, 0, "String");
	}

	if (!getChildByName("touchNode"))
	{
		auto layout = ui::Layout::create();
		layout->setContentSize(touchSize);
		layout->setTouchEnabled(true);
		layout->setPosition(touchSize / -2.f);
		layout->addClickEventListener([this](Ref*)
		{
			if (_callback)
				_callback();
		});

		addChild(layout, 0, "touchNode");
	}

	std::function<void(Node*)> SetCascade = [&SetCascade](Node* node)
	{
		node->setCascadeColorEnabled(true);
		node->setCascadeOpacityEnabled(true);

		for (auto& child : node->getChildren())
			SetCascade(child);
	};

	SetCascade(this);

	return true;
}

void FTRelationNode::Success()
{
	if (_clicked)
	{
		_clicked = false;

		GoCenter(true, nullptr);
	}

	Vector<FiniteTimeAction*> actions;

	actions.pushBack(DelayTime::create(APPLE_TOTAL_TIME));
	actions.pushBack(CallFunc::create([this] 
	{
		if (auto node = _nodes[Nodes::main])
			node->runAction(Spawn::create(ScaleTo::create(0.f, 1.2f), FadeOut::create(0.f), nullptr));
		if (auto node = _nodes[Nodes::disabled])
			node->runAction(Spawn::create(ScaleTo::create(0.f, 1.2f), FadeOut::create(0.f), nullptr));

		Node* targetNode = nullptr;

		switch (_type)
		{
		case FTRelationNode::Type::Lager:
		case FTRelationNode::Type::Smaller:
			if (auto text = dynamic_cast<ui::Text*>(getChildByName("String")))
			{
				text->setString(StringUtils::toString(_count));
				targetNode = text;
			}
			break;
		case FTRelationNode::Type::Equal:
			switch (_resultType)
			{
			case FTRelationNode::Type::Lager:
				targetNode = _nodes[Nodes::lagerAnswer];
				break;
			case FTRelationNode::Type::Equal:
				targetNode = _nodes[Nodes::equalAnswer];
				break;
			case FTRelationNode::Type::Smaller:
				targetNode = _nodes[Nodes::smallerAnswer];
				break;
			default:
				break;
			}
			break;
		default:
			break;
		}

		if (targetNode)
		{
			targetNode->setVisible(true);
			targetNode->setOpacity(0);
			targetNode->runAction(FadeIn::create(APPLE_SPAWN_TIME));
		}
	}));


	runAction(Sequence::create(actions));
}

void FTRelationNode::Fail()
{
	if (_clicked)
	{
		_clicked = false;

		GoCenter(false, nullptr);
	}
}

void FTRelationNode::StartGame()
{
	setVisible(true);
}

void FTRelationNode::NextGame() {}

void FTRelationNode::EndGame() {}

void FTRelationNode::GoCenter(bool success, std::function<void()> callback)
{
	auto visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();
	Vec2 center = { visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y };
	Vec2 target = center + Vec2(0, 469) - getPosition();

	Vector<FiniteTimeAction*> actions;
	actions.pushBack(EaseInOut::create(MoveTo::create(0.2f, target), 1.5f));
	_nodes[Nodes::normalShade]->setVisible(false);
	_nodes[Nodes::moveShade]->setVisible(true);

    actions.pushBack(CallFunc::create([this, success]
    {
		_nodes[Nodes::moveShade]->setVisible(false);

		if (success)
		{
			GameSoundManager::getInstance()->playEffectSound("FeedingTime/sfx/SFX_feedingTime_correct.m4a");
			_nodes[Nodes::correct]->setVisible(true);
			_nodes[Nodes::BG]->setVisible(false);
		}
        else
            GameSoundManager::getInstance()->playEffectSound("FeedingTime/sfx/SFX_feedingTime_incorrect.m4a");
    }));
    
	actions.pushBack(DelayTime::create(APPLE_TOTAL_TIME));
	if (success)
	{
		actions.pushBack(FadeOut::create(APPLE_SPAWN_TIME));
	}
	else
	{
		actions.pushBack(CallFunc::create([this] { _nodes[Nodes::moveShade]->setVisible(true); }));
		actions.pushBack(EaseInOut::create(MoveTo::create(0.2f, Vec2::ZERO), 1.5f));
		actions.pushBack(CallFunc::create([this] { _nodes[Nodes::moveShade]->setVisible(false); _nodes[Nodes::normalShade]->setVisible(true); }));
	}

	if (callback)
		actions.pushBack(CallFunc::create(callback));

	_nodes[Nodes::main]->runAction(Sequence::create(actions));
}

void FTRelationNode::SetData(const Type & type, const Type& resultType, const int& count)
{
	_type = type;
	_resultType = resultType;
	_count = count;

	if (auto text = dynamic_cast<ui::Text*>(getChildByName("String")))
		text->setVisible(false);

	for (auto child : _nodes)
	{
		child.second->setVisible(false);
		child.second->setOpacity(255);
	}

	_nodes[Nodes::lager]->setVisible(_type == Type::Lager);
	_nodes[Nodes::smaller]->setVisible(_type == Type::Smaller);
	_nodes[Nodes::equal]->setVisible(_type == Type::Equal);
	_nodes[Nodes::lagerDisabled]->setVisible(_type == Type::Lager);
	_nodes[Nodes::smallerDisabled]->setVisible(_type == Type::Smaller);
	_nodes[Nodes::equalDisabled]->setVisible(_type == Type::Equal);

	_nodes[Nodes::correct]->setVisible(true);
	_nodes[Nodes::disabled]->setVisible(true);
	_nodes[Nodes::disabled]->setScale(1);
	_nodes[Nodes::BG]->setVisible(true);
	_nodes[Nodes::main]->setVisible(true);
	_nodes[Nodes::main]->setPosition(Vec2::ZERO);
	_nodes[Nodes::main]->setScale(1);
	_nodes[Nodes::normalShade]->setVisible(true);
}
