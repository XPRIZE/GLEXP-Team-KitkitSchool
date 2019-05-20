
#include "AppDelegate.h"
#include "SplashScene.hpp"
#include "Managers/LogManager.hpp"
#include "Managers/UserManager.hpp"
#include "Managers/CacheManager.hpp"
#include "Common/Controls/TouchEventLogger.h"
#include "Games/LRComprehension/LRComprehensionScene.hpp"
#include "Menu/MainScene.hpp"
#include "Menu/WelcomeVideoScene.hpp"
#include "CCAppController.hpp"

// #define USE_AUDIO_ENGINE 1
#define USE_SIMPLE_AUDIO_ENGINE 1

#if USE_AUDIO_ENGINE && USE_SIMPLE_AUDIO_ENGINE
#error "Don't use AudioEngine and SimpleAudioEngine at the same time. Please just select one in your game!"
#endif

#if USE_AUDIO_ENGINE
#include "audio/include/AudioEngine.h"
using namespace cocos2d::experimental;
#elif USE_SIMPLE_AUDIO_ENGINE
#include "audio/include/SimpleAudioEngine.h"
using namespace CocosDenshion;
#endif

USING_NS_CC;

static cocos2d::Size designResolutionSize = cocos2d::Size(2560, 1800);
//static cocos2d::Size desktopSize = cocos2d::Size(640, 450);
static cocos2d::Size desktopSize = cocos2d::Size(2560, 1800)/2; // pixel-c
//static cocos2d::Size desktopSize = cocos2d::Size(1920, 1080)/2; // 16:9

Scene* findMainScene();

AppDelegate::AppDelegate()
{
}

AppDelegate::~AppDelegate() 
{
#if USE_AUDIO_ENGINE
    AudioEngine::end();
#elif USE_SIMPLE_AUDIO_ENGINE
    SimpleAudioEngine::end();
#endif
}

// if you want a different context, modify the value of glContextAttrs
// it will affect all platforms
void AppDelegate::initGLContextAttrs()
{
    // set OpenGL context attributes: red,green,blue,alpha,depth,stencil
    GLContextAttrs glContextAttrs = {8, 8, 8, 8, 24, 8};

    GLView::setGLContextAttrs(glContextAttrs);
}

// if you want to use the package manager to install more packages,  
// don't modify or remove this function
static int register_all_packages()
{
    return 0; //flag for packages manager
}

bool AppDelegate::applicationDidFinishLaunching() {
    // initialize director
    auto director = Director::getInstance();
    auto glview = director->getOpenGLView();
    if(!glview) {
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32) || (CC_TARGET_PLATFORM == CC_PLATFORM_MAC) || (CC_TARGET_PLATFORM == CC_PLATFORM_LINUX)
        glview = GLViewImpl::createWithRect("KitkitSchool", cocos2d::Rect(0, 0, desktopSize.width, desktopSize.height));
#else
        glview = GLViewImpl::create("KitkitSchool");
#endif
        director->setOpenGLView(glview);
    }

    // turn on display FPS
    director->setDisplayStats(false);

    // set FPS. the default value is 1.0/60 if you don't call this
    director->setAnimationInterval(1.0f / 60);

    // Set the design resolution
    glview->setDesignResolutionSize(designResolutionSize.width, designResolutionSize.height, ResolutionPolicy::FIXED_HEIGHT);


    register_all_packages();
    
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32) || (CC_TARGET_PLATFORM == CC_PLATFORM_MAC) || (CC_TARGET_PLATFORM == CC_PLATFORM_LINUX)
    
    string path = FileUtils::getInstance()->getWritablePath() + "KitkitSchool";
    
    if (FileUtils::getInstance()->isFileExist(path+"/location.txt")) {
        path = FileUtils::getInstance()->getStringFromFile(path+"/location.txt");
        path.erase(std::remove(path.begin(),path.end(),'\n'),path.end());
    }

    FileUtils::getInstance()->setDefaultResourceRootPath(path);
//
//    FileUtils::getInstance()->addSearchPath(path);
//    FileUtils::getInstance()->addSearchPath(path+"/Games");
//
    
#endif
    
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
//    string devicePath = "/storage/emulated/0/KitkitSchool/";
    string devicePath = "/storage/emulated/0/Android/data/com.maq.xprize.kitkitschool.hindi/files/"; // can be changed to any other resource location
    /*if (FileUtils::getInstance()->isFileExist(devicePath + "cache.txt")) {
        FileUtils::getInstance()->setDefaultResourceRootPath(devicePath);
    }*/
    FileUtils::getInstance()->setDefaultResourceRootPath(devicePath);

#endif
    
    // search path will be set in LanguageManager
    
    //FileUtils::getInstance()->addSearchPath("res");
    //FileUtils::getInstance()->addSearchPath("Games");
    
    CCAppController::sharedAppController();
    
    // shortcut
    //{ CCAppController::sharedAppController()->startGame("EggQuizLiteracy", 0, "PostTest_N"); return true; }
    
    auto scene = SplashScene::createScene();
    scene->setName("SplashScene");
    director->runWithScene(TouchEventLogger::wrapScene(scene));
    
    /* log test
     LogManager::getInstance()->logEvent("key", "v1");
     LogManager::getInstance()->logEvent("key", "v2");
     LogManager::getInstance()->logEvent("key", "v3");
     LogManager::getInstance()->logEvent("key", "v4");
     LogManager::getInstance()->logEvent("key", "v5");
     
     Json::Value vl;
     vl["test"] = "test";
     vl["num"] = 1;
     LogManager::getInstance()->logEventJson("json", vl);
     
     for(int i=0; i<100; i++){
     LogManager::getInstance()->logEvent("volumeTest", "abcdefghijklmnopqrstuvwxyz");
     }
     */
    
    
    
    CacheManager::getInstance()->loadBirdCache([this](){
        Director::getInstance()->replaceScene(TouchEventLogger::wrapScene(MainScene::createScene()));
    });
    return true;
}

// This function will be called when the app is inactive. Note, when receiving a phone call it is invoked.
void AppDelegate::applicationDidEnterBackground() {
    Director::getInstance()->stopAnimation();

#if USE_AUDIO_ENGINE
    AudioEngine::pauseAll();
#elif USE_SIMPLE_AUDIO_ENGINE
    SimpleAudioEngine::getInstance()->pauseBackgroundMusic();
    SimpleAudioEngine::getInstance()->pauseAllEffects();
#endif
}

// this function will be called when the app is active again
void AppDelegate::applicationWillEnterForeground() {
    Director::getInstance()->startAnimation();

#if USE_AUDIO_ENGINE
    AudioEngine::resumeAll();
#elif USE_SIMPLE_AUDIO_ENGINE
    SimpleAudioEngine::getInstance()->resumeBackgroundMusic();
    SimpleAudioEngine::getInstance()->resumeAllEffects();
#endif
    
    UserManager::getInstance()->refreshUsername();
    //Director::getInstance()->getOpenGLView()->setDesignResolutionSize(designResolutionSize.width, designResolutionSize.height, ResolutionPolicy::FIXED_HEIGHT);
    
    // if you use SimpleAudioEngine, it must resume here
    //CocosDenshion::SimpleAudioEngine::getInstance()->resumeBackgroundMusic();
    CCLOG("applicationWillEnterForeground");
    auto test = SplashScene::getLaunchString();
    CCLOG("applicationWillEnterForeground : %s",test.c_str());
    if (test.length()>0) {
        auto scene = SplashScene::createScene();
        
        Director::getInstance()->replaceScene(scene);
        
    }
    
    Scene * mainScene = findMainScene();
    if (mainScene != nullptr) {
        mainScene->resume();
    }
}

Scene* findMainScene()
{
    Scene* result = nullptr;
    const int MAIN_SCENE_ID = 250;
    auto director = Director::getInstance();
    Scene* scene = nullptr;
    if (director != nullptr)
    {
        scene = director->getRunningScene();
        if (scene != nullptr)
        {
            result = (Scene*)(scene->getChildByTag(MAIN_SCENE_ID));
            if (result == nullptr)
            {
                Vector<Node *> children = scene->getChildren();
                for (Node * child : children)
                {
                    result = (Scene*)(child->getChildByTag(MAIN_SCENE_ID));
                    if (result != nullptr)
                    {
                        break;
                    }
                }
            }
        }
    }
    
    return result;
}
