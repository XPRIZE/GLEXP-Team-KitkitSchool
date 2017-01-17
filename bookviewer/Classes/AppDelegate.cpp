#include "AppDelegate.h"
#include "MainScene.h"
#include <string>
#include "SimpleAudioEngine.h"

USING_NS_CC;


static cocos2d::Size macResolutionSize = cocos2d::Size(1024, 720);
static cocos2d::Size designResolutionSize = cocos2d::Size(2560, 1800);


AppDelegate::AppDelegate() {

}

AppDelegate::~AppDelegate() 
{
}

//if you want a different context,just modify the value of glContextAttrs
//it will takes effect on all platforms
void AppDelegate::initGLContextAttrs()
{
    //set OpenGL context attributions,now can only set six attributions:
    //red,green,blue,alpha,depth,stencil
    GLContextAttrs glContextAttrs = {8, 8, 8, 8, 24, 8};

    GLView::setGLContextAttrs(glContextAttrs);
}

// If you want to use packages manager to install more packages, 
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
        glview = GLViewImpl::createWithRect("booktest", Rect(0, 0, macResolutionSize.width, macResolutionSize.height));
#else
        glview = GLViewImpl::create("booktest");
#endif
        director->setOpenGLView(glview);
    }

    // turn on display FPS
    director->setDisplayStats(false);

    // set FPS. the default value is 1.0/60 if you don't call this
    director->setAnimationInterval(1.0 / 60);

    // Set the design resolution
    glview->setDesignResolutionSize(designResolutionSize.width, designResolutionSize.height, ResolutionPolicy::FIXED_HEIGHT);
    
    register_all_packages();

    Scene* scene;
    auto book= MainScene::getLaunchString();
    //if (book.length()>0) {
    //    scene = HelloWorld::createBookScene(book);
    //} else
    
    {
        scene = MainScene::createScene();
    }

    // run
    director->runWithScene(scene);

    return true;
}

// This function will be called when the app is inactive. When comes a phone call,it's be invoked too
void AppDelegate::applicationDidEnterBackground() {
    Director::getInstance()->stopAnimation();
    //Director::getInstance()->pause();
    // if you use SimpleAudioEngine, it must be pause
    //CocosDenshion::SimpleAudioEngine::getInstance()->pauseBackgroundMusic();
    

}

// this function will be called when the app is active again
void AppDelegate::applicationWillEnterForeground() {
    
    //Director::getInstance()->resume();
    Director::getInstance()->startAnimation();

    // if you use SimpleAudioEngine, it must resume here
    //CocosDenshion::SimpleAudioEngine::getInstance()->resumeBackgroundMusic();
    
    auto book= MainScene::getLaunchString();
    if (book.length()>0 && book!=MainScene::currentBook) {
        MainScene::clearLaunchString();
        MainScene::currentBook = book;
        auto scene = MainScene::createBookScene(book);
        Director::getInstance()->replaceScene(scene);
    }
}
