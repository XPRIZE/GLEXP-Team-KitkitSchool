//
//  FishObject.hpp
//  KitKit_FB
//
//  Created by jesun kim on 05/09/2018.
//

#ifndef FishObject_hpp
#define FishObject_hpp

#include "cocos2d.h"
#include "cocostudio/CocoStudio.h"

#include "FishData.hpp"
#include "ui/CocosGUI.h"
#include "SimpleAudioEngine.h"

USING_NS_CC;
using namespace CocosDenshion;
using namespace cocostudio;
using namespace cocostudio::timeline;
using namespace ui;

namespace Status {
    enum Enum{
        IDLE,
        MOVING,
        EASING
    };
}

namespace Direction {
    enum Enum{
        LEFT = -1,
        RIGHT = 1
    };
}

#define MAX_ANGLE   20
#define ROTATION_INTERVAL 5
#define IDLE_SPEED_RATE 5.0f
#define BORDER_LEFT -300
#define BORDER_RIGHT 300
#define BORDER_UP 300
#define INIT_TARGETROTATION 9999

class FishObject : public Ref{
    
public:
    FishObject();
    ~FishObject();
    
    void setDirection(int direction);
    void setRotation(float angle);
    void setPosition(Vec2 position);
    void moveFish(float fDelta, Map<std::string, Node *> mapZOrders, int fishCount);
    void setFishMove(bool isTouch);
    void setBottomFishMove();
    void setFishIdle();
    void loadFishAction();
    void setButtonEvent();
    void setScaleByZ();
    void changeZOrder(Map<std::string, Node *> mapZOrders, int fishCount);
    void selectFish(Ref *pSender, Widget::TouchEventType type);
    int getRandomZ(int fishCount);
    
    
public:
    // db data
    int _id;
    std::string _userName;
    std::string _fishID;
    int _skinNo;
    std::string _name;
    std::string _createTime;
    std::string _position;
    
    // game data
    Node *node;
    float nowIdleTime;
    float targetRotation;
    bool isRotating;
    float rotationInterval;
    int targetDistance;
    int remainDistance;
    float targetSpeed;
    float animationRate;
    ActionTimeline *action;
    bool isEscaping;
    int zOrder;
    bool willBlack;
    bool willVanish;
    time_t time;
    
    // particle
    Node *prevParent;
    Vec2 prevPosition;
    float prevScaleX;
    float prevScaleY;
    
    int bottomTargetY = 0;
    FishData *fishData;
    Status::Enum status;
    Direction::Enum direction;
    const int BOTDER_BOTTOM[4] = {500, 800, 800, 900};

};
#endif /* FishObject_hpp */
