//
//  FishObject.cpp
//  KitKit_FB
//
//  Created by jesun kim on 05/09/2018.
//

#include "FishObject.hpp"

FishObject::FishObject()
{
    this->node = NULL;
    this->willVanish = false;
    this->willBlack = false;
    this->status = Status::IDLE;
    this->targetRotation = INIT_TARGETROTATION;
}

FishObject::~FishObject()
{
    CC_SAFE_RELEASE(this->action);
}

/*
 * 물고기 방향과 z에 따른 scaleX 설정
 */
void FishObject::setDirection(int direction)
{
    this->direction = (Direction::Enum)direction;
    if(this->direction == Direction::LEFT){
        this->node->setScaleX(1.f - ((this->zOrder-1.f)*0.2f));
    }else if(this->direction == Direction::RIGHT){
        this->node->setScaleX(-1.f + ((this->zOrder-1.f)*0.2f));
    }
}

/*
 * 물고기 각도 설정
 */
void FishObject::setRotation(float angle)
{
    this->node->setRotation(angle);
}

/*
 * 물고기 위치 설정
 */
void FishObject::setPosition(Vec2 position)
{
    this->node->setPosition(position);
}

/*
 * 물고기 scaleY 설정
 */
void FishObject::setScaleByZ()
{
    this->node->setScaleY(fabs(this->node->getScaleX()));
}

/*
 * 물고기 z 변경
 */
void FishObject::changeZOrder(Map<std::string, Node *> mapZOrders, int fishCount)
{
    // 수중생물의 경우 패스...
    if(this->fishData->isBottom){
        return;
    }
    
    int z = this->getRandomZ(fishCount);
    if(this->BOTDER_BOTTOM[z-1] > this->node->getPositionY()){
        return;
    }
    this->zOrder = z;
    
    // 새로 바뀐 z로 parent를 바꾼다. setParent로 할 경우 문제가 있어서 remove-add로 했음.
    CC_SAFE_RETAIN(this->node);
    this->node->removeFromParentAndCleanup(false);
    mapZOrders.at(StringUtils::format("z%d", this->zOrder))->addChild(this->node);
    CC_SAFE_RELEASE(this->node);
}

/*
 * 물고기 수에 따른 z 설정
 */
 
int FishObject::getRandomZ(int fishCount)
{
    if(fishCount <= 7){
//        CCLOG("7마리 이하라 z1에만 생성");
        return 1;
    }else if(fishCount <= 14){
//        CCLOG("14마리 이하라 z1, z2에만 생성");
        return RandomHelper::random_int(1, 2);
    }else if(fishCount <= 21){
//        CCLOG("21마리 이하라 z1, z3, z3에만 생성");
        return RandomHelper::random_int(1, 3);
    }else{
//        CCLOG("22마리 이상이라 z1, z2, z3, z4에 생성");
        return RandomHelper::random_int(1, 4);
    }
}

/*
 * 물고기 이동
 */
void FishObject::moveFish(float fDelta, Map<std::string, Node *> mapZOrders, int fishCount)
{
    // 수중생물이고 move 애니메이션 재생중에는 패스...(수중생물의 move 애니메이션은 제자리에 멈춰서 애니메이션을 재생하므로 이동하면 안됨)
    if(this->fishData->isBottom && this->status == Status::MOVING){
        return;
    }
    float rotation = this->node->getRotation();
    
    // 물고기 회전 설정
    if(this->isRotating){
        if(this->targetRotation != INIT_TARGETROTATION){
            if(rotation < this->targetRotation){
                rotation += (0.05f*this->action->getTimeSpeed());
            }else{
                rotation -= (0.05f*this->action->getTimeSpeed());
            }
            
            this->setRotation(rotation);
            if(fabs(fabs(rotation) - fabs(this->targetRotation)) < 0.01f){
                this->targetRotation = INIT_TARGETROTATION;
                this->isRotating = false;
                this->rotationInterval = 0;
            }
        }else{
            this->isRotating = false;
        }
    }else{
        this->rotationInterval += fDelta;
    }
    
    // 각도에 따른 이동할 y
    float yMove = fabs(rotation / 100.f);
    // y에 따른 x 설정
    float xMove = 1.f - yMove;
    if(this->status == Status::IDLE){
        // IDLE 상태일때 속도에 따른 x,y 설정
        yMove *= (this->fishData->speed/IDLE_SPEED_RATE);
        xMove *= (this->fishData->speed/IDLE_SPEED_RATE);
    }else if(this->status == Status::MOVING){
        // MOVE 상태일때 속도에 따른 x, y 설정
        yMove *= this->fishData->speed;
        xMove *= this->fishData->speed;
        // 남은 거리 계산...
        this->remainDistance -= (yMove + xMove);
        // 이동을 끝나면 감속 status 설정
        if(this->remainDistance < 0){
            this->targetSpeed -= 0.02f;
            this->status = Status::EASING;
        }
    }else if(this->status == Status::EASING){
        yMove *= this->targetSpeed;
        xMove *= this->targetSpeed;
        this->targetSpeed -= 0.02f;
        this->action->setTimeSpeed(this->targetSpeed * this->animationRate);
        if(this->targetSpeed < this->fishData->speed/IDLE_SPEED_RATE){
            this->action->setTimeSpeed(1);
            this->status = Status::IDLE;
        }
    }
    
    float x = this->node->getPositionX();
    float y = this->node->getPositionY();
    
    // 수중생물의 경우 랜덤하게 y이동
    if(this->fishData->isBottom){
        if(this->bottomTargetY == 0){
            do {
                this->bottomTargetY = RandomHelper::random_int(40, 150);
            } while (this->bottomTargetY == y);
        }else{
            if(this->bottomTargetY > y){
                yMove = 1;
            }else{
                yMove = -1;
            }
            if(this->direction == Direction::LEFT){
                yMove *= -1;
            }
            if(fabs(fabs(y) - abs(this->bottomTargetY)) < 0.01f){
                this->bottomTargetY = 0;
            }
            // y좌표에 따른 zorder 설정
            this->node->setLocalZOrder(10000-y);
        }
    }
    
    // 방향과 각도에 따른 +,- 설정
    if(this->direction == Direction::LEFT){
        x -= xMove;
        if(rotation > 0){
            y += yMove;
        }else{
            y -= yMove;
        }
    }else{
        x += xMove;
        if(rotation > 0){
            y -= yMove;
        }else{
            y += yMove;
        }
    }
    this->setPosition(Vec2(x, y));
    
    // 왼쪽으로 나갔을 경우 z 변경
    if(x < BORDER_LEFT){
        this->changeZOrder(mapZOrders, fishCount);
        this->setDirection(Direction::RIGHT);
        this->setScaleByZ();
        this->node->setRotation(this->node->getRotation()*-1);
        if(this->isRotating){
            if(!this->isEscaping){
                this->targetRotation *= -1;
            }
            
        }
    }
    // 오른쪽으로 나갔을 경우 z 변경
    else if(x > Director::getInstance()->getVisibleSize().width + BORDER_RIGHT){
        this->changeZOrder(mapZOrders, fishCount);
        this->setDirection(Direction::LEFT);
        this->setScaleByZ();
        this->node->setRotation(this->node->getRotation()*-1);
        if(this->isRotating){
            if(!this->isEscaping){
                this->targetRotation *= -1;
            }
        }
    }
    // 상단 한계 지점까지 오면 강제로 밑으로 내려가게 회전 설정
    else if(y > Director::getInstance()->getVisibleSize().height - BORDER_UP){
        if(this->direction == Direction::LEFT){
            this->targetRotation = -MAX_ANGLE;
        }else if(this->direction == Direction::RIGHT){
            this->targetRotation = MAX_ANGLE;
        }
        this->isRotating = true;
        return;
    }
    // 수중생물이 아닐경우 현재 물고기가 속한 z의 최 하단에 오면 강제로 올라가게 회전 설정
    if(!this->fishData->isBottom && this->node->getPositionY() < this->BOTDER_BOTTOM[this->zOrder-1]){
        if(this->direction == Direction::LEFT){
            this->targetRotation = MAX_ANGLE;
        }else if(this->direction == Direction::RIGHT){
            this->targetRotation = -MAX_ANGLE;
        }
        this->isRotating = true;
        return;
    }
    
    // 일반적인 경우 랜덤한 회전 설정
    if(!this->isRotating && !this->fishData->isHorizontal && !this->fishData->isBottom){
        if(this->rotationInterval > ROTATION_INTERVAL){
            this->targetRotation = RandomHelper::random_int(-MAX_ANGLE, MAX_ANGLE);
            if(this->targetRotation != 0){
                this->isRotating = true;
            }
        }
    }
}

/*
 * touch 또는 idleTime에 의한 move
 */
void FishObject::setFishMove(bool isTouch)
{
    // MOVE중이 아닐때만....
    if(this->status != Status::MOVING){
        this->nowIdleTime = 0;
        this->status = Status::MOVING;
        if(this->fishData->isBottom){
            // touch 했을때만 사운드 재생, 수중생물의 경우 애니메이션에 맞춰 사운드를 x회 재생
            if(isTouch){
                for(int i=0 ; i<this->fishData->vecSoundFrame.size() ; i++){
                    CCLOG("%s play sound %d", this->fishData->strName.c_str(), i);
                    CallFunc *playSound = CallFunc::create([&](){
                        SimpleAudioEngine::getInstance()->playEffect("sounds/sandpounding.m4a", false);
                    });
                    this->node->runAction(Sequence::createWithTwoActions(DelayTime::create(this->fishData->vecSoundFrame.at(i)/60.f), playSound));
                }
            }
            this->setBottomFishMove();
        }else{
            if(this->fishData->isFish){
                // touch 했을때만 사운드 재생
                if(isTouch){
                    SimpleAudioEngine::getInstance()->playEffect("sounds/waterflow.m4a", false);
                }
                // 일반적인 물고기의 경우 속도, 이동할 거리 설정
                this->targetSpeed = this->fishData->speed;
                this->remainDistance = this->targetDistance = RandomHelper::random_int(this->fishData->moveRange.at(0), this->fishData->moveRange.at(1));
                this->action->setTimeSpeed(IDLE_SPEED_RATE);
            }else{
                // touch 했을때만 사운드 재생
                if(isTouch){
                    SimpleAudioEngine::getInstance()->playEffect("sounds/waterdrop.m4a", false);
                }
                // move 애니메이션 재생
                this->setBottomFishMove();
            }
        }
    }
}

/*
 * 수중생물, 장식물의 경우 move -> idle 애니메이션 재생
 */
void FishObject::setBottomFishMove()
{
    this->status = Status::MOVING;
    this->node->stopAction(this->action);
    this->action->play("move", false);
    this->node->runAction(this->action);
    this->action->setLastFrameCallFunc([=](){
        if(this->status != Status::IDLE){
            this->status = Status::IDLE;
            // 낮은 버전의 os에서 crash 때문에 clearLastFrameCallFunc() 삭제
            //        this->action->clearLastFrameCallFunc();
            this->node->stopAction(this->action);
            AnimationInfo info = this->action->getAnimationInfo("idle");
            this->action->gotoFrameAndPlay(info.startIndex, info.endIndex, info.startIndex, true);
            this->node->runAction(this->action);
        }
    });
}

/*
 * 물고기 idle 애니메이션 재생
 */
void FishObject::setFishIdle()
{
    this->animationRate = (IDLE_SPEED_RATE/2.f) / this->fishData->speed;
    this->status = Status::IDLE;
    this->action->play("idle", true);
    this->node->runAction(this->action);
}

/*
 * action...
 */
void FishObject::loadFishAction()
{
    this->action = CSLoader::createTimeline(this->fishData->strFileName);
    CC_SAFE_RETAIN(this->action);
}

/*
 * 터치 입력을 위한 button 이벤트 설정
 */
void FishObject::setButtonEvent()
{
    Button *buttonFish = static_cast<Button *>(this->node->getChildByName("button"));
    buttonFish->addTouchEventListener(CC_CALLBACK_2(FishObject::selectFish, this));
    
}

void FishObject::selectFish(Ref *pSender, Widget::TouchEventType type)
{
    switch (type) {
        case Widget::TouchEventType::CANCELED:
        case Widget::TouchEventType::BEGAN:
        case Widget::TouchEventType::MOVED:
            break;
        case Widget::TouchEventType::ENDED:
            this->setFishMove(true);
            break;
    }
}

