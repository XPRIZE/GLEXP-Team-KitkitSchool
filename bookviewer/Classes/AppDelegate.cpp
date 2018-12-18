#include "AppDelegate.h"
#include "MainScene.h"
#include <string>
#include "SimpleAudioEngine.h"
#include "Common/Controls/TodoLoadingScene.hpp"
#include "Managers/LanguageManager.hpp"
#include "Utils/TodoUtil.h"
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
    CCLOG("applicationDidFinishLaunching()");

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
    
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    std::string devicePath = "/sdcard/KitkitSchool/";
    if (FileUtils::getInstance()->isFileExist(devicePath + "cache.txt")) {
        FileUtils::getInstance()->setDefaultResourceRootPath(devicePath);
    }
    
#endif

    Scene* scene;
    /*
    auto book= MainScene::getLaunchString();
    //if (book.length()>0) {
    //    scene = HelloWorld::createBookScene(book);
    //} else
    
    {
        scene = MainScene::createScene();
    }

    // run
    director->runWithScene(scene);
    */

	if (!launchBook()) {
		scene = MainScene::createScene();
		director->runWithScene(scene);
	}

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
    CCLOG("applicationWillEnterForeground()");

//    Director::getInstance()->resume();
    Director::getInstance()->startAnimation();

    // if you use SimpleAudioEngine, it must resume here
    //CocosDenshion::SimpleAudioEngine::getInstance()->resumeBackgroundMusic();
    
    
	launchBook();
}

bool AppDelegate::launchBook()
{
	auto book = MainScene::getLaunchString();
	CCLOG("argument = [%s]", book.c_str());
	if (book.length() > 0 && book != MainScene::currentBook) {
		MainScene::clearLaunchString();
		MainScene::currentBook = book;


		auto folderName = TodoUtil::split(book, '/').back();
		if (folderName.find("en") == 0) {
			LanguageManager::getInstance()->setCurrentLocale(LanguageManager::LocaleType::en_US);
		}
		else if (folderName.find("sw") == 0) {
			LanguageManager::getInstance()->setCurrentLocale(LanguageManager::LocaleType::sw_TZ);
		}


		std::function<Scene*(void)> creator = [book]() {
			auto scene = MainScene::createBookScene(book);
			return scene;
		};


		auto running = Director::getInstance()->getRunningScene();
		if (running) {
			Director::getInstance()->replaceScene(TodoLoadingScene::createScene(creator));
		}
		else {
			Director::getInstance()->runWithScene(TodoLoadingScene::createScene(creator));
		}

		return true;
	}




	return false;
}
