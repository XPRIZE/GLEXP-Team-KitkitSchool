#ifndef __MAIN_SCENE_H__
#define __MAIN_SCENE_H__

#include "cocos2d.h"
#include <string>


class MainScene : public cocos2d::Layer
{
public:
    CREATE_FUNC(MainScene);
    
    static cocos2d::Scene* createScene();
    static cocos2d::Scene* createBookScene(std::string bookfolder);
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WINRT)
	static std::string __launchString;
#endif

    static std::string getLaunchString();
    static void clearLaunchString();
    static std::string currentBook;
    
    
    virtual bool init();
    

    void menuCloseCallback(cocos2d::Ref* pSender);


    void showLoading();
    void showChooser();
    bool showBook(std::string bookfolder);

    
};

#endif // __MAIN_SCENE_H__
