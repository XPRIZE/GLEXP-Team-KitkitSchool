//
//  EquationMakerScene.cpp
//  todomath
//
//  Created by Hye Kyoung Hong on 7/23/15.
//
//

#include "EquationMakerScene.h"
#include "EquationMakerObject.h"
#include "Utils/TodoUtil.h"
#include "Common/Controls/TodoSchoolBackButton.hpp"
#include "Managers/GameSoundManager.h"
#include "Common/Controls/CompletePopup.hpp"
#include "CCAppController.hpp"

USING_NS_CC;


Size winSize = Size(2048, 1440);
EquationMakerScene::EquationMakerScene():m_GameState(K_TAG_GAME_WAIT){
    m_CurrentLevelType = 0;
}

EquationMakerScene::~EquationMakerScene(){
 
}

bool EquationMakerScene::init(){
    if ( !Layer::init() )
    {
        return false;
    }

    this->setContentSize(winSize);
    
    initUI();
    
    return true;
}

void EquationMakerScene::initUI(){
    auto dirSize = Director::getInstance()->getWinSize();
    
    auto bg = Sprite::create("EquationMaker/equationmaker_bg_sky.jpg");
    auto bgSize = bg->getContentSize();
    float bgScale = MAX(dirSize.width/bgSize.width, dirSize.height/bgSize.height);
    bg->setScale(bgScale);
    bg->setPosition(dirSize/2);
    addChild(bg);
    

    
    m_gameView = Node::create();
    m_gameView->setContentSize(winSize);
    m_gameView->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
    m_gameView->setPosition(Vec2(dirSize.width/2, 0));
    this->addChild(m_gameView);
    
//    auto colorLayer = LayerColor::create(Color4B(126, 206, 166, 128), winSize.width, winSize.height);
//    m_gameView->addChild(colorLayer);
    
    auto backButton = TodoSchoolBackButton::create();
    backButton->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
    backButton->setPosition(Vec2(25, dirSize.height-25));
    addChild(backButton);
    
    _progressBar = ProgressIndicator::create();
    _progressBar->setPosition(Vec2(dirSize.width/2, dirSize.height - _progressBar->getContentSize().height));
    addChild(_progressBar);
    
    
}

void EquationMakerScene::onEnter(){
    Layer::onEnter();
    
    auto listener = EventListenerTouchAllAtOnce::create();
    listener->onTouchesBegan = CC_CALLBACK_2(EquationMakerScene::onTouchesBegan, this);
    listener->onTouchesMoved = CC_CALLBACK_2(EquationMakerScene::onTouchesMoved, this);
    listener->onTouchesEnded = CC_CALLBACK_2(EquationMakerScene::onTouchesEnded, this);
    
    this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);
}

void EquationMakerScene::clear(Ref *ref, Control::EventType event) {
	RenderTexture * s = (RenderTexture * )getChildByTag(1);
	s->clear(0, 0, 0, 0);
}

void EquationMakerScene::prepareNewGameWithLevel(Json::Value problemParameter)
{
    
    requiredChildSound = true;
    
//    if ((LanguageManager::getInstance()->getCurrentLanguageType() == LanguageType::KOREAN) || ((LanguageManager::getInstance()->getCurrentLanguageType() == LanguageType::ENGLISH))) {
//        requiredChildSound = false;
//    }
    
    answers = problemParameter["answers"];

    m_CurrentLevelType = problemParameter["levelType"].asInt();
    m_CurrentLevel = problemParameter["level"].asInt();
    m_CurrentStage = 0;
    
    _progressBar->setMax((int)answers.size());
    
    this->scheduleOnce(schedule_selector(EquationMakerScene::setProblemInit), 1.f);
}

void EquationMakerScene::setProblemInit(float){
    

    GameSoundManager::getInstance()->preloadAdultVoice();
//    std::string name = "EquationMaker/bg_wood.png";
//    
//    if(m_CurrentLevelType == 5) name = "EquationMaker/bg_wood_5level.jpg";
    
    std::string name = "EquationMaker/bg_wood_5level.jpg";
    auto panelView = Sprite::create(name);
    panelView->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
    //m_PanelHeight = panelView->getContentSize().height;
    m_PanelHeight = (m_CurrentLevelType==5) ? 693 : 486;
    
    auto panelScale = Director::getInstance()->getWinSize().width / panelView->getContentSize().width;
    panelView->setScale(panelScale);

    panelView->setPosition(winSize.width/2.f, m_PanelHeight);
    m_gameView->addChild(panelView);
    
    m_CurrentStage = 0;
    m_StageClearCount = 0;
    
    m_StageSuccCount = 0;
    m_ProblemCount = answers.size();
    
    m_GameState = K_TAG_GAME_WAIT;
    
    for(int i = K_SOUND_TOUCH; i < K_SOUND_MAX; ++i){
        std::string name = getSoundName(i);
        GameSoundManager::getInstance()->preloadEffect(name);
    }
    
    setProblem(0.f);
}

void EquationMakerScene::setProblem(float){
    eraseHint();
    
    _progressBar->setCurrent(m_CurrentStage+1);
    Json::Value& val = answers[m_CurrentStage];
    Json::Value& objectList = val["objectList"];
    
    std::string fileName = val.get("file", "").asString();
    std::string fileOtherName = val.get("fileOther", "").asString();
    
    int size = objectList.size();
    int levelType = m_CurrentLevelType;
    int prevRowType = K_OBJECT_TYPE_NONE;
    int rowCount[2] = {0, 0};
    int rowIndex = -1;
    
    m_ObjectSize = Size(getObjectWidth(levelType),
                        getObjectHeight(levelType));
    
    std::vector<int> vecNonSlotObjectList;
    for(int i = 0; i < size; ++i){
        Json::Value& jObject = objectList[i];
        
        sSlot slotData;
        slotData.rect.size = m_ObjectSize;
        slotData.type = jObject.get("type", K_TYPE_NUM).asInt();;
        slotData.isSlotIn = jObject.get("isSlotIn", false).asBool();
        slotData.isSlotLineHide = jObject.get("isSlotLineHide", false).asBool();
        slotData.rowType = jObject.get("rowType", K_OBJECT_TYPE_NONE).asInt();
        
        if(slotData.rowType == K_OBJECT_TYPE_OBJECT_NON_SLOT){
            vecNonSlotObjectList.push_back(slotData.type);
        }
        else{
            m_vecSlot.push_back(slotData);
            
            if(prevRowType != slotData.rowType){
                prevRowType = slotData.rowType;
                rowIndex += 1;
            }
            
            ++rowCount[rowIndex];
        }
    }
    
    size = MAX(rowCount[0], rowCount[1]);
    
    float yCount = objectList.size()/size;
    if(vecNonSlotObjectList.size() > 0){
        yCount = 2;
        
        m_ObjectSize.height += m_ObjectSize.height*0.2f;
        m_ObjectSize.width += m_ObjectSize.width*0.3f;
    }
    
    float startX = getStartPoint(size, winSize.width/2.f, m_ObjectSize.width);
    float startY = getStartPoint(yCount, (winSize.height/2.f)+(m_PanelHeight/2.f),
                                 m_ObjectSize.height, true);
    
    startY = createNoneSlotObject(vecNonSlotObjectList, fileName,
                                  fileOtherName, startY, m_ObjectSize.height);
    
    std::vector<float> vecXWidth;
    if(levelType == 5){
        int countSize = 0;
        for(unsigned int i = 0; i < m_vecSlot.size(); ++i){
            int type = m_vecSlot[i].type;
            float addX = m_ObjectSize.width*0.35f;
            
            if(type < K_TYPE_SIGN_PLUS){
                ++countSize;
                addX = m_ObjectSize.width;
            }
            
            m_vecSlot[i].rect.size.width = addX;
            vecXWidth.push_back(addX);
        }
        
        startX = getStartPoint(countSize, winSize.width/2.f, m_ObjectSize.width*1.4f);
    }
    
    m_AnimationStartIndex = ((rowCount[1] > 0) ? rowCount[0]: 0);
    prevRowType = m_vecSlot[0].rowType;
    for(unsigned int i = 0; i < m_vecSlot.size(); ++i){
        int rowType = m_vecSlot[i].rowType;
        if(prevRowType != rowType){
            prevRowType = rowType;
            startX = getStartPoint(size, winSize.width/2.f, m_ObjectSize.width);
            startY -= m_ObjectSize.height;
        }
        
        createObject(i, levelType, fileName, Point(startX, startY), i-m_AnimationStartIndex);
        
        if(vecXWidth.empty()) startX += m_ObjectSize.width;
        else startX += vecXWidth[i];
    }
    
    m_StageClearCount = (int)m_vecObjectList.size();
    
    createPanelObject(val);
    m_isFirstMiss = false;

    this->scheduleOnce(schedule_selector(EquationMakerScene::setGameStart), 0.1f);
}

void EquationMakerScene::setGameStart(float){
    m_StageSuccCount = 0;
    m_SelectIndex = (int)m_vecObjectList.size();
    
    m_GameState = K_TAG_GAME_ING;
    //GameController::getInstance()->onReady();
    
    setAlignPanelObject();
}

void EquationMakerScene::setAlignPanelObject(){
    float startX = getStartPoint((int)m_vecObjectList.size(), winSize.width/2.f, m_ObjectSize.width);
    
    std::random_shuffle(m_vecObjectList.begin(), m_vecObjectList.end());
    
    for(unsigned int i = 0; i < m_vecObjectList.size(); ++i){
        Point pos = Point(startX, m_PanelHeight/2.f);
        m_vecObjectList[i]->setOriginPos(pos);
        m_vecObjectList[i]->setPosition(Point(-winSize.height, pos.y));
        
        auto delay = DelayTime::create(0.1f*i);
        auto effect = CallFunc::create([this](){GameSoundManager::getInstance()->playEffectSound(getSoundName(K_SOUND_MISS));});
        
        auto move = MoveTo::create(0.15, pos);
        auto seq = Sequence::create(delay, effect, move, NULL);
        
        m_vecObjectList[i]->runAction(seq);
        
        startX += m_ObjectSize.width;
    }
}

void EquationMakerScene::createPanelObject(Json::Value& val){
    int panelCount = val.get("panelCount", 0).asInt();
    int problemCount = panelCount - (int)m_vecObjectList.size();
    
    
    std::vector<int> vecList;
    std::string name = val.get("fileOther", "").asString();
    if(m_CurrentLevelType == 3) name = val.get("file", "").asString();
    
    if(m_CurrentLevelType != 4) m_ObjectSize.width *= 1.35f;
    
    Json::Value& dummy = val["dummyList"];
    int size = dummy.size();
    for(int i = 0; i < size; ++i){
        int num = dummy[i].asInt();
        vecList.push_back(num);
    }
    
    std::vector<int> vecRowList;
    int prevRowType = K_OBJECT_TYPE_NONE;
    for(unsigned int i = 0; i < m_vecObjectList.size(); ++i){
        int rowType = m_vecObjectList[i]->getRowType();
        
        if(prevRowType != rowType){
            prevRowType = rowType;
            vecRowList.push_back(rowType);
        }
    }

    int rowIdx = 0;
    for(int i = 0; i < problemCount; ++i){
        if(i >= (int)vecList.size())
            break;
        
        EquationMakerObject* pObject = EquationMakerObject::create();
        
        pObject->setTopLayer(this);
        pObject->createObject(vecList[i], vecRowList[rowIdx], m_CurrentLevelType, name, m_ObjectSize, false, false);

        pObject->setPositionX(-winSize.width);
        m_gameView->addChild(pObject);
        m_vecObjectList.push_back(pObject);
        
        ++rowIdx;
        if(rowIdx >= vecRowList.size()) rowIdx = 0;
    }
}

float EquationMakerScene::createNoneSlotObject(std::vector<int> vecList, std::string file, std::string fileOther, float _retStartY, float _marginY){
    if((int)vecList.size() > 2){
        int first = vecList[0];
        int sign = vecList[1];
        int second = vecList[2];

        if(sign == K_TYPE_SIGN_MINUS){
            createNoneSlotMinus(first, second, file, fileOther, _retStartY, _marginY);
        }
        else{
            createNoneSlotPlus(first, second, file, fileOther, _retStartY, _marginY);
        }
        
        _retStartY -= _marginY;
    }
    
    return _retStartY;
}

void EquationMakerScene::createNoneSlotMinus(int first, int second,std::string file, std::string fileOther, float _retStartY, float _marginY){
    int right = first - second;
    int half = first/2;
    float winHalf = winSize.width/2.f;
    
    std::vector<int> vecRows;
    std::string fileName = "EquationMaker/counting_"+file+".png";
    std::string emptyFilename;
    if (file.find("cup")==0) {
        emptyFilename = "EquationMaker/counting_cup_minus.png";
    } else { //icecream
        emptyFilename = "EquationMaker/counting_icecream_minus.png";
    }

    
    auto sprite = Sprite::create(fileName.c_str());
    Size spriteSize = sprite->getContentSize();
    spriteSize = Size(spriteSize.width+(spriteSize.width*0.7f),
                      spriteSize.height+(spriteSize.height*0.2f));
    
    vecRows.push_back(first-half);
    vecRows.push_back(half);
    
    int createCount = 0;
    
    float startY = _retStartY;
    
    if(first > 10){
        int addFlag = -1;
        
        first -= 10;
        half = first/2;
        
        winHalf += (spriteSize.width*5.f)/2.f;
        
        vecRows[0] = first-half;
        vecRows[1] = half;
        
        float sy = getStartPoint(2, _retStartY, spriteSize.height);
        float sx = winHalf+(spriteSize.width*7.5f*addFlag);
        for(int i = 0; i < 10; ++i){
            auto object = Sprite::create(fileName);
            object->setPosition(sx, sy);
            m_gameView->addChild(object);
            m_vecEraseObjectList.push_back(object);
            
            sx += spriteSize.width;
            
            ++createCount;
            
            if(createCount >= right)
                fileName =  emptyFilename; // "EquationMaker/counting_"+file+"_minus.png";
            
            if(i == 4){
                sy += spriteSize.height;
                sx = winHalf+(spriteSize.width*7.5f*addFlag);
            }
        }
    }
    
    
    startY = getStartPoint((int)vecRows.size(), startY, spriteSize.height);
    for(unsigned int i = 0; i < vecRows.size(); ++i){
        int size = vecRows[i];
        float startX = getStartPoint(size, winHalf, spriteSize.width);
        
        for(int j = 0; j < size; ++j){
            auto object = Sprite::create(fileName.c_str());
            object->setPosition(startX, startY);
            m_gameView->addChild(object);
            m_vecEraseObjectList.push_back(object);
            
            ++createCount;
            
            if(createCount >= right)
                fileName = emptyFilename; //"EquationMaker/counting_"+file+"_minus.png";
            
            startX += spriteSize.width;
        }
        
        startY += spriteSize.height;
    }
}

void EquationMakerScene::createNoneSlotPlus(int first, int second,std::string file, std::string fileOther, float _retStartY, float _marginY){
    int firstHalf = first/2;
    int secondHalf = second/2;
    int rowArray[2][2] = {
        {first-firstHalf, firstHalf},
        {second-secondHalf, secondHalf}};
    
    std::string fileName = "EquationMaker/counting_"+file+".png";
    float winHalf = winSize.width/2.f;
    
    float startY = _retStartY;
    float add = 5.5f;
    
    std::string carryName = "";
    Size marginSize;
    int carryNum[2] = {0, 0};
    
    const float marginScale = 1.5;
    
    if(second > 10){
        carryName = "EquationMaker/counting_"+fileOther+".png";
        auto marginSprite = Sprite::create(carryName);
        marginSprite->setScale(marginScale);
        marginSize = marginSprite->getContentSize()*marginScale;
        
        marginSize.width *= 1.1f;
        marginSize.height *= 1.1f;
        
        second -= 10;
        secondHalf = second/2;
        rowArray[1][0] = second-secondHalf;
        rowArray[1][1] = secondHalf;
        carryNum[1] = 10;
        
        winHalf -= (marginSize.width*(second-secondHalf));
        add = 1.f;
    }
    else if(first > 10){
        carryName = "EquationMaker/counting_"+file+".png";
        auto marginSprite = Sprite::create(carryName);
        marginSprite->setScale(marginScale);
        marginSize = marginSprite->getContentSize()*marginScale;
        
        marginSize.width *= 1.1f;
        marginSize.height *= 1.1f;
        
        first -= 10;
        firstHalf = first/2;
        rowArray[0][0] = first-firstHalf;
        rowArray[0][1] = firstHalf;
        carryNum[0] = 10;
        
        winHalf += (marginSize.width*(first-firstHalf));
        
        if(first % 2 == 0) add += firstHalf;
        else add += MAX(firstHalf+1, 0);
    }
    
    int addFlag = -1;
    for(int i = 0; i < 2; ++i){
        int size = carryNum[i];
        
        float originX = winHalf+(marginSize.width*add*addFlag);
        float sx = originX;
        float sy = getStartPoint(2, _retStartY, marginSize.height);
        
        for(int j = 0; j < size; ++j){
            auto object = Sprite::create(carryName);
            object->setScale(marginScale);
            object->setPosition(sx, sy);
            m_gameView->addChild(object);
            m_vecEraseObjectList.push_back(object);
            
            sx += marginSize.width;
            
            if(j == 4){
                sy += marginSize.height;
                sx = originX;
            }
        }
        
        addFlag = 1;
    }
    
    addFlag = -1;
    for(int i = 0; i < 2; ++i){
        int rowSize = (rowArray[i][1] > 0) ? 2:1;
        
        auto sprite = Sprite::create(fileName.c_str());
        sprite->setScale(marginScale);
        Size spriteSize = sprite->getContentSize()*marginScale;
        
        spriteSize.width *= 1.1f;
        spriteSize.height *= 1.1f;
        
        float subHalf = winHalf+(spriteSize.width * addFlag);
        startY = getStartPoint(rowSize, _retStartY, spriteSize.height);
        
        if(i == 1 && carryNum[i] > 0) subHalf+=(spriteSize.width*((carryNum[i]/2)+0.5f));
        
        for(int j = 0; j < 2; ++j){
            int size = rowArray[i][j];
            
            float startX = subHalf;
            if(j == 1){
                startX = subHalf;
                int check = rowArray[i][0] + rowArray[i][1];
                if((check%2) > 0) startX += ((spriteSize.width/2.f)*addFlag);
            }
            
            for(int k = 0; k < size; ++k){
                auto object = Sprite::create(fileName.c_str());
                object->setScale(marginScale);
                object->setPosition(Point(startX, startY));
                m_gameView->addChild(object);
                
                m_vecEraseObjectList.push_back(object);
                
                startX += (spriteSize.width*addFlag);
            }
            
            startY += spriteSize.height;
        }
        
        addFlag = 1;
        fileName = "EquationMaker/counting_"+fileOther+".png";
    }
}

void EquationMakerScene::createObject(int idx, int levelType, std::string fileName, Point pos, int animationTagIndex){
    sSlot slotData = m_vecSlot[idx];
    
    int type = slotData.type;
    int rowType = slotData.rowType;
    bool isSlotIn = slotData.isSlotIn;
    bool lineHide = slotData.isSlotLineHide;
    
    Size objectSize = m_vecSlot[idx].rect.size;
    
    EquationMakerObject* pObject = EquationMakerObject::create();
    m_vecSlot[idx].rect.origin = pos;
    
    if(lineHide == false){
        std::string name = "block_drop_bg";
        if(levelType == 5) name += "_5level";
        
        m_vecSlot[idx].m_pEmptySlot = Sprite::create("EquationMaker/"+name+".png");
        m_vecSlot[idx].m_pEmptySlot->setPosition(m_vecSlot[idx].rect.origin);
        m_gameView->addChild(m_vecSlot[idx].m_pEmptySlot);
    }
    
    pObject->setTopLayer(this);
    pObject->createObject(type, rowType, levelType, fileName, objectSize, isSlotIn, lineHide);
    pObject->setPosition(m_vecSlot[idx].rect.origin);
    m_gameView->addChild(pObject);
    
    EquationMakerObject* pHintObject = EquationMakerObject::create();
    pHintObject->setTopLayer(this);
    pHintObject->setHintObject();
    pHintObject->createObject(type, rowType, levelType, fileName, objectSize, isSlotIn, lineHide);
    pHintObject->setPosition(pos);
    m_gameView->addChild(pHintObject, K_TAG_OBJECT_HINT);

    pHintObject->setVisible(false);
    m_vecHintList.push_back(pHintObject);
    
    if(isSlotIn == false && lineHide == false){
        pObject->setPositionX(-winSize.width);
        m_vecObjectList.push_back(pObject);
    }
    else{
        m_vecEraseObjectList.push_back(pObject);
    }
    
    if(animationTagIndex >= 0){
        int tag = K_TAG_OBJECT_TAG+animationTagIndex;
        pObject->setTag(tag);
        m_vecSlot[idx].correctIndex = tag;
    }
}

float EquationMakerScene::getStartPoint(int size, float mid, float margin, bool isYPos){
    float surplus = (size%2 == 0) ? 2.f: 1.f;
    float addValue = 0;
    int halfValue = size/2;
    if(surplus == 2.f && halfValue > 1) addValue = (halfValue-1)*(margin/surplus);
    
    if(isYPos == false)
        mid -= (halfValue*(margin/surplus)+addValue);
    else mid += (halfValue*(margin/surplus)+addValue);
    
    return mid;
}

float EquationMakerScene::getObjectWidth(int lvType){
    if(lvType == 3) return 297.f;
    else if(lvType == 4) return 297.f;
    else if(lvType == 5) return 297.f;
    
    return 180.f;
}

float EquationMakerScene::getObjectHeight(int lvType){
    if(lvType == 3) return 383.f;
    else if(lvType == 4) return 383.f;
    else if(lvType == 5) return 383.f;
    
    return 90.f;
}

void EquationMakerScene::handleStart(){
    //GameController::getInstance()->onBeginWithTotalProblems(answers.size());
}

void EquationMakerScene::handlePause(){
    
}

void EquationMakerScene::handleQuit(){
    removeAllObject();
    
    this->stopAllActions();
    this->unscheduleAllCallbacks();
    
}

void EquationMakerScene::handleResume(){
    
}

void EquationMakerScene::handleAnswerEntered(std::string &answer){
   // GameController::getInstance()->onGameComplete();
}



void EquationMakerScene::onTouchesBegan(const std::vector<Touch*>& touches,
                                       Event *unused_event){
    if(isCheckTouch(touches)){
        m_isMiss = false;
        m_SelectIndex = (int)m_vecObjectList.size();
        
        Point touchPos = m_gameView->convertToNodeSpace(touches[0]->getLocation());
        for(unsigned j = 0; j < m_vecObjectList.size(); ++j){
            EquationMakerObject* pObject = m_vecObjectList[j];
            
            if(pObject->isTouchedIn(touchPos)){
                m_SelectIndex = j;
                
                auto scale = ScaleTo::create(0.15f, 1.2f);
                
                pObject->stopAllActions();
                pObject->runAction(scale);
                
                m_gameView->reorderChild(pObject, K_TAG_SELECT_OBJECT);
                
                std::string name = getSoundName(K_SOUND_TOUCH);
                GameSoundManager::getInstance()->playEffectSound(name);
                
                break;
            }
        }
    }
}

void EquationMakerScene::onTouchesMoved(const std::vector<Touch*>& touches,
                                       Event *unused_event){
    int size = (int)m_vecObjectList.size();
    
    if(m_SelectIndex < size && isCheckTouch(touches)){
        Point touchPos = m_gameView->convertToNodeSpace(touches[0]->getLocation());
        EquationMakerObject* pObject = m_vecObjectList[m_SelectIndex];
        
        if(pObject->isSlotIn() == false){
            for(unsigned int i = 0; i < m_vecSlot.size(); ++i){
                bool isMatch = false;
                
                if(m_vecSlot[i].isSlotIn == false){
                    isMatch = isSlotIn(i, pObject->getType(), pObject->getRowType(), touchPos);
                    
                    if(isMatch){
                        m_isMiss = false;
                        
                        m_GameState = K_TAG_GAME_WAIT;
                        
                        pObject->setSlotIn(true);
                        
                        Point dest = m_vecSlot[i].rect.origin;
                        auto move = MoveTo::create(0.1f, dest);
                        auto seq = Sequence::create(move, CallFunc::create(CC_CALLBACK_0(EquationMakerScene::setObjectMatchMoveEnd, this)), NULL);
                        
                        pObject->stopAllActions();
                        pObject->runAction(seq);
                        pObject->setShadowVisible(false);
                        
                        auto scale = ScaleTo::create(0.1f, 1.0f);
                        pObject->runAction(scale);
                        
                        m_vecSlot[i].correctIndex = pObject->getTag();
                        m_vecSlot[i].isSlotIn = true;
                        
                        m_gameView->reorderChild(pObject, K_TAG_OBJECT);
                        
                        std::string name = getSoundName(K_SOUND_SLOT_IN);
                        GameSoundManager::getInstance()->playEffectSound(name);
                    
                        ++m_StageSuccCount;
                        
                        break;
                    }
                }
            }
            
            pObject->setPosition(touchPos);
        }
    }

}

void EquationMakerScene::onTouchesEnded(const std::vector<Touch*>& touches,
                                       Event *unused_event){
    int size = (int)m_vecObjectList.size();
    
    if(m_SelectIndex < size && isCheckTouch(touches)){
        EquationMakerObject* pObject = m_vecObjectList[m_SelectIndex];
        
        if(pObject->isSlotIn() == false){
            Point origin = pObject->getOriginPos();
            
            auto move = MoveTo::create(0.15f, origin);
            pObject->stopAllActions();
            pObject->runAction(move);
            
            auto scale = ScaleTo::create(0.15f, 1.f);
            pObject->runAction(scale);
            
            m_gameView->reorderChild(pObject, K_TAG_OBJECT);
            
            m_SelectIndex = (int)m_vecObjectList.size();
        }
    }
    
    if(m_isMiss){
        std::string name = getSoundName(K_SOUND_MISS);
        GameSoundManager::getInstance()->playEffectSound(name);
        
        m_isMiss = false;
        m_isFirstMiss = true;
        //GameController::getInstance()->onMiss();
    }

}

void EquationMakerScene::setObjectMatchMoveEnd(){
    if(m_StageSuccCount >= m_StageClearCount){
        ++m_CurrentStage;
        _progressBar->setCurrent(m_CurrentStage, true);
        this->scheduleOnce(schedule_selector(EquationMakerScene::setClearAnimationFlow), 0.5f);
    }
    else{
        m_GameState = K_TAG_GAME_ING;
    }
}


Node* EquationMakerScene::createCrabResult(int lhs, int rhs)
{
    auto result = Node::create();
    result->setContentSize(winSize);
    result->setTag(K_TAG_OBJECT_RESULT);
    m_gameView->addChild(result);
    
    
    
    Point crabPos = Point(winSize.width/2.f, m_PanelHeight/2.f);
    
    auto crabSprite = Sprite::create("EquationMaker/eq_bottom_crab.png");
    crabSprite->setPosition(crabPos);
    result->addChild(crabSprite);
    
    Size spriteSize = crabSprite->getContentSize();
    
    crabSprite->setPositionY(-m_PanelHeight);
    
    auto seaSprite = Sprite::create("EquationMaker/eq_bottom_sea.png");
    seaSprite->setPosition(winSize.width/2.f, seaSprite->getContentSize().height/2.f);
    result->addChild(seaSprite);
    seaSprite->setScale(0.f);
    
    seaSprite->runAction(ScaleTo::create(0.3f, 1.f));
    
    auto scaleDelay = DelayTime::create(0.3f);
    auto move = MoveTo::create(0.3f, crabPos);
    auto seq = Sequence::create(scaleDelay, move, NULL);
    
    crabSprite->runAction(seq);
    
    char buf[64];
    sprintf(buf, "%d X %d = %d", lhs, rhs, lhs*rhs);
    
    
    auto label = TodoUtil::createLabel(buf, 140, Size(800, 150), defaultFont, Color4B(165, 96, 83,255));
    
    //TodoLabel::createLabelToFitWidth(buf, 140, Size(800, 150), TodoFontNameForNumber, Color4B(165, 96, 83,255));
    label->setAlignment(TextHAlignment::CENTER, TextVAlignment::CENTER);
    //        label->setPosition(spriteSize.width/2.f, spriteSize.height*0.95f);
    label->setPosition(spriteSize.width/2.f, spriteSize.height*0.80f);
    label->setColor(Color3B(165, 96, 83));
    crabSprite->addChild(label);
    
    return result;
}

Node* EquationMakerScene::createBaloonResult(int lhs, int rhs)
{
    auto result = Node::create();
    result->setContentSize(winSize);
    result->setTag(K_TAG_OBJECT_RESULT);
    m_gameView->addChild(result);
    
    
    
    auto func = [this, result](string s, bool isNum, int index) {
        
        const float step = 250;
        
        auto filename = isNum ? "EquationMaker/balloon_1.png" : "EquationMaker/balloon_2.png";
        
        Sprite *b = Sprite::create(filename);
        b->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
        
        float x = winSize.width/2 - step*2 + step*index + random(-5, 5);
        b->setPosition(Vec2(x, 0));

        auto l = TodoUtil::createLabel(s, 140, Size::ZERO, defaultFont, Color4B(222, 222, 222, 255));
        l->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        l->setPosition(Vec2(150, isNum ? 920 : 900));
        b->addChild(l);
        
        result->addChild(b);
        
        auto t = random(0.45, 0.55);
        auto p = random(5, 8);
        
        auto shake = Repeat::create(Sequence::create(EaseIn::create(MoveBy::create(t, Vec2(0, p)), 2),
                                               EaseOut::create(MoveBy::create(2*t, Vec2(0, -2*p)), 2),
                                               EaseIn::create(MoveBy::create(t, Vec2(0, p)), 2),
                                               NULL), 10);
        
        b->runAction(Sequence::create(DelayTime::create(random(0.25, 0.35)),
                                      EaseOut::create(MoveBy::create(random(0.25, 0.35), Vec2(0, random(590, 610))), 2),
                                      shake, NULL));
        
    };
    
    
    func(TodoUtil::itos(lhs), true, 0);
    func("X", false, 1);
    func(TodoUtil::itos(rhs), true, 2);
    func("=", false, 3);
    func(TodoUtil::itos(lhs*rhs), true, 4);
    
    
    return result;
}

// Reading Equation animation
void EquationMakerScene::setClearAnimationFlow(float)
{
    
    m_ClearEffectType.clear();
    
    m_ClearSoundIndex = 0;
    
    Vector<cocos2d::FiniteTimeAction *> vecAnimation;
    if(m_CurrentLevelType == 5){
        
        
        
        int count = 0;
        int type = 0;
        for(int i = m_AnimationStartIndex; i < m_vecSlot.size(); ++i){
            int index = m_vecSlot[i].correctIndex;
            
            EquationMakerObject* pObject = dynamic_cast<EquationMakerObject*>(m_gameView->getChildByTag(index));
            
            if(pObject){
                if(type == 0) type = pObject->getType();

                if(type == pObject->getType()) ++count;
            }
        }
        
        createBaloonResult(type, count);
        
        
        if(count > 0 && type > 0){
            
            m_ClearEffectType.push_back(type);
            m_ClearEffectType.push_back(K_TYPE_SIGN_TIMES);
            m_ClearEffectType.push_back(count);
            {
                m_ClearEffectType.push_back(K_TYPE_SIGN_EQ);
            }
            m_ClearEffectType.push_back(type*count);
            
            vecAnimation.pushBack(DelayTime::create(0.2f));
        }
        
        bool isPrevLongDelay = false;
        for(unsigned int i = 0; i < m_ClearEffectType.size(); ++i){
            float delay = 0.6f;
            if(isPrevLongDelay) delay = 0.9f;
            vecAnimation.pushBack(DelayTime::create(delay));
            vecAnimation.pushBack(CallFunc::create(CC_CALLBACK_0(EquationMakerScene::playClearAnimationObjectSound, this)));
            
            isPrevLongDelay = ((m_ClearEffectType[i]%10 == 7) || (m_ClearEffectType[i]%10 == 6));
        }
    
    } else{ // Level 1 ~ Level 7
        
        int count = 0;
        bool isPrevLongDelay = false;
        
        for(int i = m_AnimationStartIndex; i < m_vecSlot.size(); ++i){
            int index = m_vecSlot[i].correctIndex;
            
            EquationMakerObject* pObject = dynamic_cast<EquationMakerObject*>(m_gameView->getChildByTag(index));
            
            if(pObject){
                ++count;
                
                if (pObject->getType() == K_TYPE_SIGN_EQ) {
//                    if (LanguageManager::getInstance()->getCurrentLanguageType() == LanguageType::KOREAN) {
//                        int prevIndex = m_vecSlot[i-1].correctIndex;
//                        EquationMakerObject* prevObject = dynamic_cast<EquationMakerObject*>(this->getChildByTag(prevIndex));
//
//                        switch (prevObject->getType()%10) {
//                            case 1:
//                            case 3:
//                            case 6:
//                            case 7:
//                            case 8:
//                            case 0:
//                                m_ClearEffectType.push_back(K_TYPE_SIGN_EQ_KOREAN_E);
//                                break;
//                            default:
//                                m_ClearEffectType.push_back(K_TYPE_SIGN_EQ_KOREAN_N);
//                                break;
//                        }
//                    } else
                    {
                        m_ClearEffectType.push_back(K_TYPE_SIGN_EQ);
                    }
                }
                else {
                    m_ClearEffectType.push_back(pObject->getType());
                }

                
                auto delay = DelayTime::create(count*0.6f);
                auto scale = ScaleTo::create(0.2f, 1.2f);
                auto scaleBack = ScaleTo::create(0.2f, 1.f);
                auto seq = Sequence::create(delay, scale, scaleBack, NULL);
                
                pObject->stopAllActions();
                pObject->runAction(seq);
                
                
                float delayf = 0.6f;
                if(isPrevLongDelay) delayf = 0.9f;
                vecAnimation.pushBack(DelayTime::create(delayf));
                vecAnimation.pushBack(CallFunc::create(CC_CALLBACK_0(EquationMakerScene::playClearAnimationObjectSound, this)));
                
                isPrevLongDelay = ((m_ClearEffectType[i]%10 == 7) || (m_ClearEffectType[i]%10 == 6));
            }
        }
    }
    
    vecAnimation.pushBack(DelayTime::create(1.2f));
    vecAnimation.pushBack(CallFunc::create(CC_CALLBACK_0(EquationMakerScene::setClearAnimationFlowEnd, this)));
    
    auto seq = Sequence::create(vecAnimation);
    this->runAction(seq);
}

void EquationMakerScene::playClearAnimationObjectSound(){
    
    if(m_ClearEffectType.size() > m_ClearSoundIndex){
        
        int num = m_ClearEffectType[m_ClearSoundIndex];
        {
            
            GameSoundManager::eAdultVoice soundType = GameSoundManager::K_ADULT_VOIDE_NONE;
            
            if(num >= K_TYPE_SIGN_PLUS){
                if(num == K_TYPE_SIGN_PLUS)
                    soundType = GameSoundManager::K_ADULT_VOICE_PLUS;
                else if(num == K_TYPE_SIGN_MINUS)
                    soundType = GameSoundManager::K_ADULT_VOICE_MINUS;
                else if(num == K_TYPE_SIGN_EQ)
                    soundType = GameSoundManager::K_ADULT_VOICE_EQ;
                else if(num == K_TYPE_SIGN_EQ_KOREAN_E)
                    soundType = GameSoundManager::K_ADULT_VOICE_EQ_KOREAN_E;
                else if(num == K_TYPE_SIGN_EQ_KOREAN_N)
                    soundType = GameSoundManager::K_ADULT_VOICE_EQ_KOREAN_N;
                else if(num == K_TYPE_SIGN_TIMES)
                    soundType = GameSoundManager::K_ADULT_VOICE_TIMES;
                
                num = 0;
            }
            
             GameSoundManager::getInstance()->playAdultVoice(num, soundType);
            
        }
        
        ++m_ClearSoundIndex;
    }
}

void EquationMakerScene::setClearAnimationFlowEnd(){
   // GameController::getInstance()->onSolve();
    
    this->scheduleOnce(schedule_selector(EquationMakerScene::setClearAnimationFlowEndDelay), 1.2f);
}

void EquationMakerScene::setClearAnimationFlowEndDelay(float){
    
    if(m_CurrentStage >= m_ProblemCount){
        m_GameState = K_TAG_GAME_END;
        //GameController::getInstance()->onGameComplete();
        
        CompletePopup::create()->show(1.0, [](){
            CCAppController::sharedAppController()->handleGameComplete(1);
        });
        
    }
    else{
        removeAllObject();
        
        setProblem(0.f);
    }
}

void EquationMakerScene::removeAllObject(){
    for(unsigned int i = 0; i < m_vecSlot.size(); ++i){
        if(m_vecSlot[i].m_pEmptySlot){
            m_gameView->removeChild(m_vecSlot[i].m_pEmptySlot, true);
        }
    }
    
    for(unsigned int i = 0; i < m_vecObjectList.size(); ++i){
        m_gameView->removeChild(m_vecObjectList[i], true);
    }
    
    for(unsigned int i = 0; i < m_vecEraseObjectList.size(); ++i){
        m_gameView->removeChild(m_vecEraseObjectList[i], true);
    }
    
    auto resultNode = m_gameView->getChildByTag(K_TAG_OBJECT_RESULT);
    if (resultNode) m_gameView->removeChild(resultNode, true);
    
//    auto crabNode = m_gameView->getChildByTag(K_TAG_OBJECT_CRAB);
//    if(crabNode) m_gameView->removeChild(crabNode, true);
//    
//    auto seaNode = m_gameView->getChildByTag(K_TAG_OBJECT_SEA);
//    if(seaNode) m_gameView->removeChild(seaNode, true);
    
    m_vecSlot.clear();
    m_vecObjectList.clear();
    m_vecEraseObjectList.clear();
}

bool EquationMakerScene::isCheckTouch(const std::vector<Touch*>& touches){
    bool ret = false;
    
    if(m_GameState == K_TAG_GAME_ING){
        int size = (int)touches.size();
        for(int i = 0; i < size; ++i){
            int touchID = touches[i]->getID();
            if(touchID == 0)
                ret = true;
            
            break;
        }
    }
    
    return ret;
}

bool EquationMakerScene::isSlotIn(int idx, int type, int rowType, Point pos){
    bool ret = false;
    
    Rect spriteRect = m_vecSlot[idx].rect;
    spriteRect.origin.x -= (spriteRect.size.width/ 2.0f);
    spriteRect.origin.y -= (spriteRect.size.height/ 2.0f);
    ret = spriteRect.containsPoint(pos);
    
    if(ret){
        if(m_vecSlot[idx].type == type &&
           m_vecSlot[idx].rowType == rowType){
            ret = true;
        }
        else ret = false;
        
        m_isMiss = (!ret);
    }
    
    return ret;
}

std::string EquationMakerScene::getSoundName(int type){
    std::string ret = "";
    
    if(type == K_SOUND_SLOT_IN) ret = "Common/Sounds/Effect/BlockSlotIn.m4a";
    else if(type == K_SOUND_TOUCH) ret = "Common/Sounds/Effect/BlockTouch.m4a";
    else if(type == K_SOUND_MISS) ret = "Common/Sounds/Effect/BlockMiss.m4a";
    
    return ret;
}


void EquationMakerScene::eraseHint(){
    for(unsigned int i = 0; i < m_vecHintList.size(); ++i){
        m_gameView->removeChild(m_vecHintList[i], true);
    }
    
    m_vecHintList.clear();
}

void EquationMakerScene::onExit(){
    Layer::onExit();
    
    removeAllObject();
    
   // GameSoundManager::shared()->unloadAllEffect();
    
    this->removeAllChildren();
    this->unscheduleAllCallbacks();
    Director::getInstance()->getEventDispatcher()->removeEventListenersForTarget(this);
    
//    SpriteFrameCache::getInstance()->removeUnusedSpriteFrames();
    Director::getInstance()->getTextureCache()->removeUnusedTextures();
}



