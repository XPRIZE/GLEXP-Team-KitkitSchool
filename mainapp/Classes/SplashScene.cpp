//
//  SplashScene.cpp
//  KitkitSchool
//
//  Created by Gunho Lee on 6/28/16.
//
//

#include "SplashScene.hpp"
#include "Menu/MainScene.hpp"
#include "Menu/WelcomeVideoScene.hpp"
#include "CCAppController.hpp"
#include "Managers/VoiceMoldManager.h"
#include "Common/Controls/TouchEventLogger.h"
#include "Common/Controls/TodoLoadingScene.hpp"
#include "Managers/GameSoundManager.h"

#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
#include "platform/android/jni/JniHelper.h"
#endif

using namespace cocos2d;

cocos2d::Scene* SplashScene::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = SplashScene::create();
    
    // add layer as a child to scene
    scene->addChild(layer);
    
    // return the scene
    return scene;
}



// on "init" you need to initialize your instance
bool SplashScene::init()
{
    if ( !Layer::init() )
    {
        return false;
    }
    
    auto visibleSize = Director::getInstance()->getVisibleSize();
    
    auto sprite = Sprite::create("System/enuma.png");
    auto spriteSize = sprite->getContentSize();
    auto scale = MAX(visibleSize.width/spriteSize.width, visibleSize.height/spriteSize.height);
    sprite->setScale(scale);
    
    // position the sprite on the center of the screen
    sprite->setPosition(Vec2(visibleSize.width/2 , visibleSize.height/2 ));
    
    // add the sprite as a child to this layer
    this->addChild(sprite, 0);
    
    return true;
}

void SplashScene::onEnter() {
    Layer::onEnter();
    
    std::string test = SplashScene::getLaunchString();
    if (test.length()>0) {
        SplashScene::clearLaunchString();
        CCAppController::sharedAppController()->startQuiz("", 0, test);
        return;
    }


    // Wait for 0.5 seconds to load main scene
    this->scheduleOnce(schedule_selector(SplashScene::finishSplash), 0.2f);
}

void SplashScene::finishSplash(float dt) {
    // ... do whatever other initializations here
    // Show the actual main scene
    
    CCLOG("finishSplash");
    
    manageCache();

    VoiceMoldManager::shared()->secureVoiceData();
    VoiceMoldManager::shared()->warmup();
    
    if (UserDefault::getInstance()->getBoolForKey("WelcomeVideoPlayed", false)) {
        std::function<Scene*(void)> creator = []() {
            auto scene = MainScene::createScene();
            scene->setName("MainScene");
            return scene;
        };
        //Director::getInstance()->replaceScene(TouchEventLogger::wrapScene(TodoLoadingScene::createScene(creator)));
        Director::getInstance()->replaceScene(TransitionFade::create(1.f, TodoLoadingScene::createScene(creator)));

    } else {
        this->scheduleOnce(schedule_selector(SplashScene::checkOpeningVideo), 2.f);
    }
}

void SplashScene::checkOpeningVideo(float dt) {
    auto scene = WelcomeVideoScene::createScene();
    Director::getInstance()->replaceScene(TouchEventLogger::wrapScene(scene));
}

void SplashScene::manageCache() {
    
    vector<string> fileList = {
        /*"MainScene/character_shadow.png",
        "MainScene/cloud_day_1.png",
        "MainScene/cloud_day_2.png",
        "MainScene/cloud_day_3.png",
        "MainScene/day_grass_ground.png",
        "MainScene/main_bg_sky.png",
        "MainScene/main_coop_literacy.png",
        "MainScene/main_coop_math.png",*/
        "MainScene/main_leaves_left.png",
        "MainScene/main_leaves_right.png",
        "MainScene/mainscreen_exitbutton_active.png",
        "MainScene/mainscreen_exitbutton_normal.png",
        "MainScene/panel_day.png",
        "MainScene/panel_english.png",
        "MainScene/panel_math.png",
        "MainScene/panel_prek.png",
        "MainScene/right_mountain.png",
        "CoopScene/coop_bg.jpg",
        "CoopScene/coop_bird_shadow.png",
        "CoopScene/coop_cell_light.png",
        "CoopScene/coop_cell_shade.png",
        "CoopScene/coop_egg_shadow.png",
        "CoopScene/coop_roof_literacy.png",
        "CoopScene/coop_roof_math.png",
        "CoopScene/coop_slot_bottom_literacy.png",
        "CoopScene/coop_slot_bottom_math.png"
        "CoopScene/coop_woodpanel_english.png",
        "CoopScene/coop_woodpanel_level_literacy.png",
        "CoopScene/coop_woodpanel_level_math.png",
        "CoopScene/coop_woodpanel_math.png",
        "CoopScene/coop_woodpanel_prek.png",
        "CoopScene/coop_woodpanel_title_literacy.png",
        "CoopScene/coop_woodpanel_title_math.png",
        "CoopScene/effect_egg_glow_1.png",
        "CoopScene/effect_egg_glow_2.png",
        "CoopScene/effect_egg_glow_3.png",
        "CoopScene/effect_egg_nest_enlarged.png",
        "CoopScene/main_effect_sparkle_1.png",
        "CoopScene/main_effect_sparkle_2.png",
        "CoopScene/main_effect_sparkle_3.png",
        "CoopScene/new_creature_effect.png",
        "MainScene/DailyScene/daily_arrow_left_active.png",
        "MainScene/DailyScene/daily_arrow_left_normal.png",
        "MainScene/DailyScene/daily_arrow_right_active.png",
        "MainScene/DailyScene/daily_arrow_right_normal.png",
        "MainScene/DailyScene/daily_beam_large.png"
    };
    
    for (auto it : fileList) {
        if (!FileUtils::getInstance()->isFileExist(it)) continue;
        Director::getInstance()->getTextureCache()->addImage(it);
        CCLOG("add cache: %s", it.c_str());
    }
    
}




string SplashScene::getLaunchString() {
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
    
    JniMethodInfo t;
    std:: string a("");
    if (JniHelper::getStaticMethodInfo(t, "org/cocos2dx/cpp/AppActivity", "getLaunchString", "()Ljava/lang/String;"))
    {
        jstring jstr = (jstring)t.env->CallStaticObjectMethod(t.classID, t.methodID);
        a = JniHelper::jstring2string(jstr);
        t.env->DeleteLocalRef(jstr);
        return a;
    }
    return "";
    
#else
    return "";
    
#endif
}

void SplashScene::clearLaunchString() {
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
    // get Documents directory path
    JniMethodInfo t;
    if (JniHelper::getStaticMethodInfo(t, "org/cocos2dx/cpp/AppActivity", "clearLaunchString", "()V"))
    {
        t.env->CallStaticVoidMethod(t.classID, t.methodID);
        t.env->DeleteLocalRef(t.classID);
    }
    
    return;
#endif
    
}
