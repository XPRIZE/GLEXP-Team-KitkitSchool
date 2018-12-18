//
//  FishBowlScene.cpp
//  KitKit_FB
//
//  Created by jesun kim on 27/08/2018.
//

#include "FishBowlScene.hpp"

#include "MainScene.hpp"

#include <time.h>

int testCode;

Scene* FishBowlScene::createScene(int _testCode)
{
    testCode = _testCode;
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = FishBowlScene::create();
    
    // add layer as a child to scene
    scene->addChild(layer);
    
    // return the scene
    return scene;
}

void FishBowlScene::onEnterTransitionDidFinish()
{
    this->checkDays = true;
}

FishBowlScene::~FishBowlScene()
{
    __NotificationCenter::getInstance()->removeObserver(this, "GET_FISH_DATA");
    SimpleAudioEngine::getInstance()->stopBackgroundMusic();
    
    this->vecGetSounds.clear();
    this->vecParticles.clear();
    this->vecFishes.clear();
    this->vecNewFishes.clear();
    this->mapZOrders.clear();
    this->mapDecoPositions.clear();
    this->mapFishData.clear();
}

bool FishBowlScene::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }
    
    this->_loadingThread = nullptr;
    this->checkDays = false;
    __NotificationCenter::getInstance()->addObserver(this, CC_CALLFUNCO_SELECTOR(FishBowlScene::getFishData), "GET_FISH_DATA", nullptr);
     
    this->visibleSize = Director::getInstance()->getVisibleSize();
    this->loadData();
    this->drawBG();
    this->getFishData(nullptr);
    
    this->schedule(CC_SCHEDULE_SELECTOR(FishBowlScene::moveFishes));

    // back button 생성
    this->buttonBack = Button::create("images/back_arrow_inactive.png", "images/back_arrow_active.png");
    this->buttonBack->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
    this->buttonBack->setPosition(Vec2(25, this->visibleSize.height-25));
    this->buttonBack->addClickEventListener([this](Ref*){
        
#ifdef TEST_MODE
        Director::getInstance()->replaceScene(MainScene::createScene());
#else
        Director::getInstance()->end();
#endif
    });
    this->addChild(this->buttonBack);

    EventListenerKeyboard *touchListener = EventListenerKeyboard::create();
    touchListener->onKeyReleased = [](EventKeyboard::KeyCode keyCode, cocos2d::Event *event)
    {
        if (keyCode == EventKeyboard::KeyCode::KEY_BACK)
        {
#ifdef TEST_MODE
            Director::getInstance()->replaceScene(MainScene::createScene());
#else
            Director::getInstance()->end();
#endif
        }
    };
    _eventDispatcher->addEventListenerWithSceneGraphPriority(touchListener, this);
    
    SimpleAudioEngine::getInstance()->playBackgroundMusic("sounds/bgm.m4a", true);
    
    return true;
}

/*
 * 새로 획득한 물고기일 경우 vecNewFishes 넣는다
 */
void FishBowlScene::checkNewFish(FishObject *fishObject)
{
    if(UserDefault::getInstance()->getIntegerForKey(StringUtils::format("%d", fishObject->_id).c_str(), -1) == -1){
        // new fish
        this->vecNewFishes.pushBack(fishObject);
        UserDefault::getInstance()->setIntegerForKey(StringUtils::format("%d", fishObject->_id).c_str(), 1);
    }
}

/*
 * 새로 획득한 물고기가 5마리가 넘을 경우 정리
 */
void FishBowlScene::arrangeNewFish()
{
    if(this->vecNewFishes.size() > 5){
        int removeCount = this->vecNewFishes.size()-5;
        for(int i=0 ; i<removeCount ; i++){
            this->vecNewFishes.erase(0);
        }
    }
    
    for(int i=0 ; i<this->vecNewFishes.size() ; i++){
        CCLOG("New Fish... ID = %d\tFishID = %s\tFishName = %s", this->vecNewFishes.at(i)->_id, this->vecNewFishes.at(i)->_fishID.c_str(), this->vecNewFishes.at(i)->_name.c_str());
    }
}

/*
 * 획득 사운드 리스트 정리
 */
void FishBowlScene::makeGetSoundsList()
{
    this->vecGetSounds.clear();
    for(int i=0 ; i<6 ; i++){
        this->vecGetSounds.push_back(StringUtils::format("sounds/card_hit_%d.m4a", i));
    }
}

/*
 * 획득 사운드 랜덤 재생
 */
void FishBowlScene::playGetSound()
{
    int index = RandomHelper::random_int(0, (int)this->vecGetSounds.size()-1);
    std::string str = this->vecGetSounds.at(index);
    this->vecGetSounds.erase(this->vecGetSounds.begin()+index);
    SimpleAudioEngine::getInstance()->playEffect(str.c_str());
}

/*
 * 신규 획득 연출 그리기
 */
void FishBowlScene::drawNewFish()
{
    if(this->vecNewFishes.size() == 0){
        return;
    }
    
    this->makeGetSoundsList();
    
    // 반투명 배경
    this->layerColor = LayerColor::create(Color4B(0, 0, 0, 178), this->visibleSize.width, this->visibleSize.height);
    this->addChild(layerColor);
    
    // 화면에 그려질 particle 미리 생성
    for(int i=0 ; i<this->vecNewFishes.size() ; i++){
        this->createShiningParticle(this->visibleSize/2);
    }
    
    // 신규 획득이 1개일 경우...(2개 이상일 경우와 sequence가 약간 다르므로 별도로 만들었음)
    if(this->vecNewFishes.size() == 1){
        // 가운데로 이동
        CallFunc *moveToCenter = CallFunc::create([&](){
            FishObject *fishObject = this->vecNewFishes.at(0);
            fishObject->prevPosition = fishObject->node->getPosition();
            fishObject->prevScaleX = fishObject->node->getScaleX();
            fishObject->prevScaleY = fishObject->node->getScaleY();
            fishObject->prevParent = fishObject->node->getParent();
            if(!fishObject->fishData->isFish){
                fishObject->node->setPosition(fishObject->node->convertToWorldSpace(fishObject->node->getPosition()));
            }
            CC_SAFE_RETAIN(fishObject->node);
            fishObject->node->removeFromParentAndCleanup(false);
            this->addChild(fishObject->node);
            CC_SAFE_RELEASE(fishObject->node);
            fishObject->node->runAction(MoveTo::create(0.3f, Vec2(this->visibleSize.width/2, this->visibleSize.height/2 - (fishObject->fishData->isFish ? 0 : 200))));
            fishObject->node->runAction(ScaleTo::create(0.3f, fishObject->node->getScaleX() > 0 ? 2.f : -2.f, 2.f));
        });
        
        // 파티클, 사운드 재생
        CallFunc *drawParticle = CallFunc::create([&](){
            this->playGetSound();
            Node *nodeParticle = this->vecParticles.at(0);
            for(int j=0 ; j<nodeParticle->getChildrenCount() ; j++){
                ParticleSystemQuad *particle = static_cast<ParticleSystemQuad *>(nodeParticle->getChildren().at(j));
                particle->start();
            }
        });
        
        // 원래 위치로 돌아가기
        CallFunc *moveToOrigin = CallFunc::create([&](){
            FishObject *fishObject = this->vecNewFishes.at(0);
            CC_SAFE_RETAIN(fishObject->node);
            fishObject->node->removeFromParentAndCleanup(false);
            fishObject->prevParent->addChild(fishObject->node);
            CC_SAFE_RELEASE(fishObject->node);
            if(!fishObject->fishData->isFish){
                fishObject->node->setPosition(fishObject->node->getParent()->convertToNodeSpace(fishObject->node->getPosition()));
            }
            
            this->layerColor->removeFromParentAndCleanup(true);
            this->vecParticles.at(0)->removeFromParentAndCleanup(true);
            fishObject->node->runAction(MoveTo::create(0.3f, fishObject->prevPosition));
            fishObject->node->runAction(ScaleTo::create(0.3f, fishObject->prevScaleX, fishObject->prevScaleY));
        });
        
        // 마무리
        CallFunc *finalize = CallFunc::create([&](){
            this->scheduleOnce(CC_SCHEDULE_SELECTOR(FishBowlScene::takeScreenshot), 0.5f);
            this->vecParticles.clear();
            this->vecNewFishes.clear();
        });
        
        this->runAction(Sequence::create(moveToCenter, DelayTime::create(0.3f), drawParticle, DelayTime::create(1.f), moveToOrigin, DelayTime::create(0.3f), finalize, NULL));
        
    }else{
        // 신규 획득 2개 이상일 경우....
        for(int i=0 ; i<this->vecNewFishes.size() ; i++){
            // 가운데로 이동
            CallFunc *moveToCenter = CallFunc::create([&, i](){
                FishObject *fishObject = this->vecNewFishes.at(i);
                fishObject->prevPosition = fishObject->node->getPosition();
                fishObject->prevScaleX = fishObject->node->getScaleX();
                fishObject->prevScaleY = fishObject->node->getScaleY();
                fishObject->prevParent = fishObject->node->getParent();
                if(!fishObject->fishData->isFish){
                    fishObject->node->setPosition(fishObject->node->convertToWorldSpace(fishObject->node->getPosition()));
                }
                CC_SAFE_RETAIN(fishObject->node);
                fishObject->node->removeFromParentAndCleanup(false);
                this->addChild(fishObject->node);
                CC_SAFE_RELEASE(fishObject->node);
                fishObject->node->runAction(MoveTo::create(0.3f, Vec2(this->visibleSize.width/2, this->visibleSize.height/2 - (fishObject->fishData->isFish ? 0 : 200))));
                fishObject->node->runAction(ScaleTo::create(0.3f, fishObject->node->getScaleX() > 0 ? 2.f : -2.f, 2.f));
            });
            
            // 파티클, 사운드 재생
            CallFunc *drawParticle = CallFunc::create([&, i](){
                this->playGetSound();
                Node *nodeParticle = this->vecParticles.at(i);
                for(int j=0 ; j<nodeParticle->getChildrenCount() ; j++){
                    ParticleSystemQuad *particle = static_cast<ParticleSystemQuad *>(nodeParticle->getChildren().at(j));
                    particle->start();
                }
            });
            
            // 하단으로 이동
            CallFunc *moveToBottom = CallFunc::create([&, i](){
                FishObject *fishObject = this->vecNewFishes.at(i);
                Node *nodeParticle = this->vecParticles.at(i);
                int gap = this->visibleSize.width/(this->vecNewFishes.size()+1);
                MoveTo *moveFish = MoveTo::create(0.3f, Vec2(gap+(i*gap), 300 - (fishObject->fishData->isFish ? 0 : 100)));
                MoveTo *moveParticle = MoveTo::create(0.3f, Vec2(gap+(i*gap), 300));
//                CCLOG("x = %d", (gap+(i*gap)));
                ScaleTo *scaleFish = ScaleTo::create(0.3f, fishObject->node->getScaleX() > 0 ? 1.f : -1.f, 1.f);
                ScaleTo *scaleParticle = ScaleTo::create(0.3f, 1.f);
                fishObject->node->runAction(moveFish);
                fishObject->node->runAction(scaleFish);
                nodeParticle->runAction(moveParticle);
                nodeParticle->runAction(scaleParticle);
            });
            
            this->runAction(Sequence::create(DelayTime::create(i*1.9f), moveToCenter, DelayTime::create(0.3f), drawParticle, DelayTime::create(1.f), moveToBottom, NULL));
        }
        
        // 신규 획득 물고기들이 하단에 다 모인 다음 원래 위치로 이동
        for(int i=0 ; i<this->vecNewFishes.size() ; i++){
            CallFunc *moveToOrigin = CallFunc::create([&, i](){
                FishObject *fishObject = this->vecNewFishes.at(i);
                CC_SAFE_RETAIN(fishObject->node);
                fishObject->node->removeFromParentAndCleanup(false);
                fishObject->prevParent->addChild(fishObject->node);
                CC_SAFE_RELEASE(fishObject->node);
                if(!fishObject->fishData->isFish){
                    fishObject->node->setPosition(fishObject->node->getParent()->convertToNodeSpace(fishObject->node->getPosition()));
                }
                if(this->vecNewFishes.size()-1 == i){
                    this->layerColor->removeFromParentAndCleanup(true);
                }
                this->vecParticles.at(i)->removeFromParentAndCleanup(true);
                fishObject->node->runAction(MoveTo::create(0.5f, fishObject->prevPosition));
                fishObject->node->runAction(ScaleTo::create(0.5f, fishObject->prevScaleX, fishObject->prevScaleY));
            });

            this->runAction(Sequence::create(DelayTime::create((1.9f*this->vecNewFishes.size()-1) + 1.0f), moveToOrigin, NULL));
        }
        
        // 마무리
        CallFunc *finalize = CallFunc::create([&](){
            this->scheduleOnce(CC_SCHEDULE_SELECTOR(FishBowlScene::takeScreenshot), 1.0f);
            this->vecParticles.clear();
            this->vecNewFishes.clear();
        });
        this->runAction(Sequence::create(DelayTime::create((1.9f*this->vecNewFishes.size()-1) + 1.0f), finalize, NULL));
    }
}

/*
 * 신규 획득에 나오는 파티클 생성
 */
void FishBowlScene::createShiningParticle(Vec2 position)
{
    Node *_shiningParitcleNode = Node::create();
    ParticleSystemQuad *particleEffect = nullptr;
    
    auto createParticleEffect = [&](std::string name, std::string plist){
        particleEffect = ParticleSystemQuad::create(plist);
        particleEffect->setName(name);
        particleEffect->setScale(1.f);
        particleEffect->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        particleEffect->setPosition(Vec2::ZERO);
        particleEffect->stopSystem();
        _shiningParitcleNode->addChild(particleEffect);
    };
    
    createParticleEffect("particle1", "particles/shining_particle_blur.plist");
    createParticleEffect("particle1", "particles/shining_particle_circle.plist");
    createParticleEffect("particle1", "particles/shining_particle_star.plist");
    
    _shiningParitcleNode->setPosition(position);
    _shiningParitcleNode->setScale(2.f);
    this->layerColor->addChild(_shiningParitcleNode);
    this->vecParticles.pushBack(_shiningParitcleNode);
}

/*
 * 물고기, 장식물 데이터 불러오기
 */
void FishBowlScene::loadData()
{
    rapidjson::Document d;
    std::string file = "data/data.json";
    std::string content = FileUtils::getInstance()->getStringFromFile(file.c_str());
    d.Parse<0>(content.c_str());
    for(int i=0 ; i<d.Size() ; i++){
        FishData *fishData = new FishData();
        fishData->strFishID = d[i]["Fish_ID"].GetString();
        fishData->strName = d[i]["Name"].GetString();
        fishData->idleTime = d[i]["IdleTime"].GetFloat();
        fishData->idleTime *= 5;
        fishData->speed = d[i]["Speed"].GetInt();
        fishData->speed *= 5;
        fishData->moveRange.push_back(d[i]["MoveRange"].GetArray()[0].GetInt());
        fishData->moveRange.push_back(d[i]["MoveRange"].GetArray()[1].GetInt());
        fishData->isRare = d[i]["IsRare"].GetBool();
        fishData->isBottom = d[i]["IsBottom"].GetBool();
        fishData->isHorizontal = d[i]["Horizontal"].GetBool();
        fishData->isLookTop = d[i]["IsLookTop"].GetBool();
        // 바닥으로만 다니는 수중 생물 4개만 "SoundTiming"이 존재한다.
        // value는 애니메이션 프레임
        if(d[i].HasMember("SoundTiming")){
            for (auto& v : d[i]["SoundTiming"].GetArray()){
                fishData->vecSoundFrame.push_back(v.GetInt());
            }
        }
        if(fishData->strFishID[0] == 'f'){
            fishData->isFish = true;
            fishData->strDirectory = "fishes";
        }else{
            fishData->isFish = false;
            fishData->strDirectory = "objects";
        }
        this->mapFishData.insert(fishData->strFishID.c_str(), fishData);
        CC_SAFE_RELEASE(fishData);
    }
}

/*
 * 물고기/장식물 그리기
 */
void FishBowlScene::makeFishes()
{
    FishObject *fishObject;
    std::string strFileNameWithSkin;
    this->fishCount = 0;
    for(int i=0 ; i<this->vecFishes.size() ; i++){
        fishObject = this->vecFishes.at(i);
        // background -> foreground일 경우 새로 생성된 객체만 그리기
        if(fishObject->node != NULL){
            continue;
        }
        fishObject->fishData = this->mapFishData.at(fishObject->_fishID.c_str());
        strFileNameWithSkin = StringUtils::format("%s_%d", fishObject->fishData->strName.c_str(), fishObject->_skinNo);
        fishObject->fishData->strFileName = StringUtils::format("%s/%s/%s.csb", fishObject->fishData->strDirectory.c_str(), fishObject->fishData->strName.c_str(), strFileNameWithSkin.c_str());
        fishObject->node = static_cast<Node *>(CSLoader::createNode(fishObject->fishData->strFileName));
    
        fishObject->loadFishAction();
        fishObject->setFishIdle();
        fishObject->setButtonEvent();
        
        // 물고기일 경우...
        if(fishObject->fishData->isFish){
            // 수평으로만 이동하지 않고, 바닥으로만 다니는 수중생물이 아닌 경우 각도 각도 랜덤 생성
            if(!fishObject->fishData->isHorizontal && !fishObject->fishData->isBottom){
                fishObject->setRotation(RandomHelper::random_int(-MAX_ANGLE, MAX_ANGLE));
            }
            // 수중생물일 경우 무조건 z1에 위치
            if(fishObject->fishData->isBottom){
                fishObject->zOrder = 1;
            }else{
                this->fishCount++;
                // 수중생물이 아닐 경우 랜덤 z에 위치
                fishObject->zOrder = fishObject->getRandomZ(this->fishCount);
                // 새로운 물고기의 경우 자연스러운 연출을 위하여 무조건 z1에 위치
                if(this->vecNewFishes.contains(fishObject)){
                    fishObject->zOrder = 1;
                }
            }
            // 물고기에 할당된 z node에 그리기
            this->mapZOrders.at(StringUtils::format("z%d", fishObject->zOrder))->addChild(fishObject->node);
            int x, y;
            x = RandomHelper::random_int(200, (int)(this->visibleSize.width-200));
            if(fishObject->fishData->isBottom){
                y = RandomHelper::random_int(40, 150);
                fishObject->node->setLocalZOrder(10000-y);
            }else{
                y = RandomHelper::random_int(fishObject->BOTDER_BOTTOM[fishObject->zOrder-1], (int)(this->visibleSize.height-500));
                fishObject->node->setLocalZOrder(10000);
            }
            fishObject->setPosition(Vec2(x, y));
        }else{
            // 장식물의 경우 정해진 위치의 노드에 넣기
            fishObject->zOrder = 0;
            this->mapDecoPositions.at(fishObject->_position)->addChild(fishObject->node);
        }
        
        // 랜덤 방향
        fishObject->setDirection(RandomHelper::random_int(0, 1)*2-1);
        // z에 따른 크기 조정
        fishObject->setScaleByZ();
    }
    
    
    // 물고기 z 변경
    for(int i=0 ; i<this->vecFishes.size() ; i++){
        if(this->vecFishes.at(i)->fishData->isFish && !this->vecFishes.at(i)->fishData->isBottom){
            fishObject = this->vecFishes.at(i);
            if(this->vecNewFishes.contains(fishObject)){
                continue;
            }
            fishObject->zOrder = fishObject->getRandomZ(this->fishCount);
            fishObject->changeZOrder(this->mapZOrders, this->fishCount);
            fishObject->setScaleByZ();
        }
    }
    
    // 물고기가 MAX_FISH보다 많을 경우 일반 물고기 삭제
    if(this->fishCount > MAX_FISH){
        for(int i=0 ; i<this->vecFishes.size() ; i++){
            FishObject *fishObject = this->vecFishes.at(i);
            if(fishObject->fishData->isFish && !fishObject->fishData->isRare){
                CCLOG("Exceeded... ID = %d\tFishID = %s\tFishName = %s", fishObject->_id, fishObject->_fishID.c_str(), fishObject->_name.c_str());
                this->deleteFish(fishObject->_id);
                fishObject->node->removeFromParentAndCleanup(true);
                this->vecFishes.eraseObject(fishObject);
                this->vecNewFishes.eraseObject(fishObject);
                i--;
                this->fishCount--;
                if(this->fishCount == MAX_FISH){
                    break;
                }
            }
        }
    }
    // 물고기가 MAX_FISH보다 많을 경우 레어 물고기 삭제
    if(this->fishCount > MAX_FISH){
        for(int i=0 ; i<this->vecFishes.size() ; i++){
            FishObject *fishObject = this->vecFishes.at(i);
            if(fishObject->fishData->isFish){
                CCLOG("Exceeded... ID = %d\tFishID = %s\tFishName = %s", fishObject->_id, fishObject->_fishID.c_str(), fishObject->_name.c_str());
                this->deleteFish(fishObject->_id);
                fishObject->node->removeFromParentAndCleanup(true);
                this->vecFishes.eraseObject(fishObject);
                this->vecNewFishes.eraseObject(fishObject);
                i--;
                this->fishCount--;
                if(this->fishCount == MAX_FISH){
                    break;
                }
            }
        }
    }
}

/*
 * 배경 그리기
 */
void FishBowlScene::drawBG()
{
    Node *node = static_cast<Node *>(CSLoader::createNode("images/node.csb"));
    node->setPosition(this->visibleSize/2);
    this->addChild(node);
    
    this->arrangeChildren(node);
}

/*
 * 배경 노드에 있는 children 정리
 */

void FishBowlScene::arrangeChildren(Node *node)
{
    std::string strName = node->getName();
    if(strcmp(strName.c_str(), "ignore") != 0){
        // z node의 경우 물고기의 좌표 때문에 worldSpace로 변경
        if(strName[0] == 'z'){
            Vec2 worldPosition = node->convertToWorldSpace(node->getPosition());
            node->setPosition(node->getPosition() - worldPosition);
            for(int i=0 ; i<node->getChildrenCount() ; i++){
                node->getChildren().at(i)->setPosition(node->getChildren().at(i)->getPosition() + worldPosition);
            }
            this->mapZOrders.insert(strName, node);
        }else {
            this->mapDecoPositions.insert(strName, node);
            // Cx 노드는 Y좌표에 따른 zorder 서렂ㅇ
            if(strName[0] == 'c'){
                // 바닥인 경우 Zorder 설정
                node->setLocalZOrder(10000-node->getPositionY());
            }
        }
    }
    
    // recursive....
    for(int i=0 ; i<node->getChildrenCount() ; i++){
        this->arrangeChildren(node->getChildren().at(i));
    }
}

/*
 * 매 프레임 물고기 움직임
 */
void FishBowlScene::moveFishes(float fDelta)
{
    if(this->vecNewFishes.size() != 0){
        return;
    }
    time_t now;
    time(&now);
    struct tm *tm_now = localtime(&now);
    now = mktime(tm_now);
    FadeOut *fade = FadeOut::create(1.0f);
    FishObject *fishObject;
    int count = 0;
    for(int i=0 ; i<this->vecFishes.size() ; i++){
        fishObject = this->vecFishes.at(i);
        
        if(this->checkDays){
            if(!fishObject->willVanish){
                double diff = difftime(now, fishObject->time);
                // 6일 지남 체크
                if(diff > 604800){
                    // over 7 days
                    fishObject->willVanish = true;
                    CallFunc *endOfAction = CallFunc::create([&, fishObject](){
                        this->deleteFish(fishObject->_id);
                        fishObject->node->removeFromParentAndCleanup(true);
                        this->vecFishes.eraseObject(fishObject);
                    });
                    fishObject->node->runAction(Sequence::create(DelayTime::create(count*0.5f), fade->clone(), endOfAction, NULL));
                    CCLOG("7Days... ID = %d\tFishID = %s\tFishName = %s", fishObject->_id, fishObject->_fishID.c_str(), fishObject->_name.c_str());
                    count++;
                }else if(!fishObject->willBlack){
                    // 7일 지남 체크
                    if(diff > 518400){
                        // over 6 days
                        fishObject->willBlack = true;
                        CCLOG("6Days... ID = %d\tFishID = %s\tFishName = %s", fishObject->_id, fishObject->_fishID.c_str(), fishObject->_name.c_str());
                        
                        CallFunc *toGray = CallFunc::create([&, fishObject](){
                            for(int i=0 ; i<fishObject->node->getChildrenCount() ; i++){
                                Sprite *sprite = static_cast<Sprite *>(fishObject->node->getChildren().at(i));
                                this->toGray(sprite);
                            }
                        });
                        fishObject->node->runAction(Sequence::createWithTwoActions(DelayTime::create(count*0.5f), toGray));
                        count++;
                    }
                }   
            }
        }
        
        // 물고기일 경우 이동 및 idleTime에 따른 애니메이션 재생
        if(fishObject->fishData->isFish){
            fishObject->moveFish(fDelta, this->mapZOrders, this->fishCount);
            
            if(fishObject->status == Status::IDLE){
                fishObject->nowIdleTime += fDelta;
                if(fishObject->nowIdleTime >= fishObject->fishData->idleTime){
                    fishObject->setFishMove(false);
                }
            }
        }
    }
}

/*
 * 6일 지난 물고기 gray 적용
 */
void FishBowlScene::toGray(Sprite *sprite)
{
    if(strcmp(sprite->getName().c_str(), "ignore") != 0){
        sprite->setGLProgram(GLProgramCache::getInstance()->getGLProgram(GLProgram::SHADER_NAME_POSITION_GRAYSCALE));
    }
    // recursive...
    for(int i=0 ; i<sprite->getChildrenCount() ; i++){
        this->toGray(static_cast<Sprite *>(sprite->getChildren().at(i)));
    }
}

/*
 * 물고기 삭제
 */
void FishBowlScene::deleteFish(int id)
{
    UserDefault::getInstance()->deleteValueForKey(StringUtils::format("%d", id).c_str());
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    JniHelper::callStaticBooleanMethod("org/cocos2dx/cpp/AppActivity", "deleteFish", id);
#endif
}

/*
 * 물고기 데이터 가져오기
 */
void FishBowlScene::getFishData(Ref *obj)
{
    std::string strFishes;
    // normal 모드
    if(testCode == 0){
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
        // android의 경우 DB에서 가져오기
        CCLOG("일반모드 / ANDROID");
        strFishes = JniHelper::callStaticStringMethod("org/cocos2dx/cpp/AppActivity", "getFishes");
        CCLOG("strFishes = %s", strFishes.c_str());
#else
        // android가 아닌 경우 test.json을 데이터로 사용
        CCLOG("일반모드 / non ANDROID");
        strFishes = FileUtils::getInstance()->getStringFromFile("data/test.json");
        CCLOG("strFishes = %s", strFishes.c_str());
#endif
    }else {
        // 테스트 모드인 경우 무조건 test.json을 사용
        CCLOG("테스트모드");
        strFishes = FileUtils::getInstance()->getStringFromFile("data/test.json");
        CCLOG("strFishes = %s", strFishes.c_str());
    }

    bool isNew;
    int id;
    rapidjson::Document d;
    d.Parse<0>(strFishes.c_str());
    // 실행후 새로 추가된 물고기인지 체크
    for(int i=0 ; i<d.Size() ; i++){
        CCLOG("ID = %d\tFishID = %s\tFishSkin = %d\tCreateTime = %s\tFishName = %s", d[i]["_id"].GetInt(), d[i]["_fishID"].GetString(), d[i]["_skinNo"].GetInt(), d[i]["_createTime"].GetString(), d[i]["_name"].GetString());
        id = d[i]["_id"].GetInt();
        isNew = true;
        for(int j=0 ; j<this->vecFishes.size() ; j++){
            if(id == this->vecFishes.at(j)->_id){
                isNew = false;
                break;
            }
        }
        if(isNew){
            // 새로 추가된 물고기 객체 생성
            FishObject *fishObject = new FishObject();
            std::string createTime = d[i]["_createTime"].GetString();
            fishObject->_id = id;
            fishObject->_userName = d[i]["_userName"].GetString();
            fishObject->_fishID = d[i]["_fishID"].GetString();
            fishObject->_skinNo = d[i]["_skinNo"].GetInt();
            fishObject->_name = d[i]["_name"].GetString();
            fishObject->_createTime = d[i]["_createTime"].GetString();
            fishObject->_position = d[i]["_position"].GetString();
            struct tm time;
            strptime(fishObject->_createTime.c_str(), "%Y%m%d%H%M%S", &time);
            fishObject->time = mktime(&time);
            this->vecFishes.pushBack(fishObject);
            this->checkNewFish(fishObject);
            CC_SAFE_RELEASE(fishObject);
        }
    }

    // 6일 지남 테스트 경우 createTime 설정
    if(testCode == 6){
        time_t now;
        time(&now);
        struct tm *tm_now = localtime(&now);
        now -= (518400);
        tm_now = localtime(&now);
        for(int i=0 ; i<this->vecFishes.size() ; i++){
            this->vecFishes.at(i)->time = mktime(tm_now);
        }
    }
    
    // 7일 지남 테스트 경우 createTime 설정
    if(testCode == 7){
        time_t now;
        time(&now);
        struct tm *tm_now = localtime(&now);
        now -= (604800);
        tm_now = localtime(&now);
        for(int i=0 ; i<this->vecFishes.size() ; i++){
            this->vecFishes.at(i)->time = mktime(tm_now);
        }
    }
    
    this->arrangeNewFish();
    this->makeFishes();
    this->drawNewFish();
}

/*
 * 스크린샷...
 */
void FishBowlScene::takeScreenshot(float fDelta)
{
    char buff[20];
    time_t now = time(NULL);
    strftime(buff, 20, "%Y%m%d%H%M%S", localtime(&now));
    
    float height = this->visibleSize.height;
    std::string strPath;
    std::string strFullPath;
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    strPath = JniHelper::callStaticStringMethod("org/cocos2dx/cpp/AppActivity", "getSavePath");
    strFullPath = StringUtils::format("%s/%s.jpg", strPath.c_str(), std::string(buff).c_str());
#else
    strFullPath = StringUtils::format("%s%s/%s.jpg", FileUtils::getInstance()->getWritablePath().c_str(), strPath.c_str(), std::string(buff).c_str());
#endif
    
    // 세로 400 기준으로 스케일링
    float ratio = height / 400.f;
    Image *img = cocos2d::utils::captureNode(this, 1.f/ratio);
    img->saveToFile(strFullPath, true);

    
    
//    this->screenshotThread();
    
    
//    if(this->_loadingThread == nullptr){
//        this->_loadingThread = new (std::nothrow) std::thread(&FishBowlScene::screenshotThread, this);
//    }
    
    
    
    
//    this->buttonBack->setVisible(false);
//    char buff[20];
//    time_t now = time(NULL);
//    strftime(buff, 20, "%Y%m%d%H%M%S", localtime(&now));
//
//    float height = this->visibleSize.height;
//    std::string strPath;
//    std::string strFullPath;
//#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
//    strPath = JniHelper::callStaticStringMethod("org/cocos2dx/cpp/AppActivity", "getSavePath");
//    strFullPath = StringUtils::format("%s/%s.jpg", strPath.c_str(), std::string(buff).c_str());
//#else
//    strFullPath = StringUtils::format("%s%s/%s.jpg", FileUtils::getInstance()->getWritablePath().c_str(), strPath.c_str(), std::string(buff).c_str());
//#endif
//
//
//    static bool succeedSaveToFile = false;
//    std::function<void(void*)> mainThread = [](void* /*param*/)
//    {
//        CCLOG("end of capture");
//    };
//
//    AsyncTaskPool::getInstance()->enqueue(AsyncTaskPool::TaskType::TASK_IO, std::move(mainThread), nullptr, [height, strFullPath, this]()
//    {
//        // 세로 400 기준으로 스케일링
//        float ratio = height / 400.f;
////        Image *img = cocos2d::utils::captureNode(this, 1.f/ratio);
////        succeedSaveToFile = img->saveToFile(strFullPath, true);
//        Image *img = cocos2d::utils::captureNode(Director::getInstance()->getRunningScene(), 1.f/ratio);
//  });
}

//void FishBowlScene::screenshotThread()
//{
//    char buff[20];
//    time_t now = time(NULL);
//    strftime(buff, 20, "%Y%m%d%H%M%S", localtime(&now));
//
//    float height = this->visibleSize.height;
//    std::string strPath;
//    std::string strFullPath;
//#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
//    strPath = JniHelper::callStaticStringMethod("org/cocos2dx/cpp/AppActivity", "getSavePath");
//    strFullPath = StringUtils::format("%s/%s.jpg", strPath.c_str(), std::string(buff).c_str());
//#else
//    strFullPath = StringUtils::format("%s%s/%s.jpg", FileUtils::getInstance()->getWritablePath().c_str(), strPath.c_str(), std::string(buff).c_str());
//#endif
//
//    // 세로 400 기준으로 스케일링
//    float ratio = height / 400.f;
//    Image *img = cocos2d::utils::captureNode(this, 1.f/ratio);
//    img->saveToFile(strFullPath, true);
//}
