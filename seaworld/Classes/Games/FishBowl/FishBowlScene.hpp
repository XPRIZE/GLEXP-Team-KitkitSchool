//
//  FishBowlScene.hpp
//  KitKit_FB
//
//  Created by jesun kim on 27/08/2018.
//

#ifndef FishBowlScene_hpp
#define FishBowlScene_hpp

#include <stdio.h>
#include "cocos2d.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
#include "SimpleAudioEngine.h"

#include "FishData.hpp"
#include "FishObject.hpp"

USING_NS_CC;
using namespace CocosDenshion;
using namespace cocostudio;
using namespace cocostudio::timeline;
using namespace ui;
using namespace std;

#define MAX_FISH 30
//#define TEST_MODE

class FishBowlScene : public Layer {
    
    
public:
    // there's no 'id' in cpp, so we recommend returning the class instance pointer
    static Scene* createScene(int _testCode);
    
    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();
    virtual void onEnterTransitionDidFinish();
    
    // implement the "static create()" method manually
    CREATE_FUNC(FishBowlScene);
    
private:
    ~FishBowlScene();
    
    void loadData();
    void drawBG();
    void arrangeChildren(Node *node);
    void makeFishes();
    void moveFishes(float fDelta);
    void checkNewFish(FishObject *fishObject);
    void arrangeNewFish();
    void drawNewFish();
    void createShiningParticle(Vec2 position);
    void deleteFish(int id);
    void getFishData(Ref *);
    void toGray(Sprite *sprite);
    void makeGetSoundsList();
    void playGetSound();
    void takeScreenshot(float fDelta);
//    void screenshotThread();

private:
    std::thread* _loadingThread;
    Button *buttonBack;
    int fishCount;
    bool checkDays;
    Size visibleSize;
    Vector<FishObject *> vecFishes;
    Map<std::string, Node *> mapZOrders;
    Map<std::string, Node *> mapDecoPositions;
    Map<std::string, FishData *> mapFishData;
    Vector<FishObject *> vecNewFishes;
    Vector<Node *> vecParticles;
    LayerColor *layerColor;
    std::vector<std::string> vecGetSounds;
};

#endif /* FishBowlScene_hpp */
