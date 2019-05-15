//
//  CacheManager.cpp
//  KitkitSchool-mobile
//
//  Created by JungJaehun on 16/05/2018.
//

#include "CacheManager.hpp"
#include "CurriculumManager.hpp"
#include "Managers/UserManager.hpp"
#include "Menu/Bird.hpp"
#include "Menu/MainScene.hpp"
#include "Menu/CoopScene.hpp"

#include "cocos2d.h"
#include "Utils/TodoUtil.h"

USING_NS_CC;
using namespace std;

CacheManager* CacheManager::_instance = 0;

CacheManager* CacheManager::getInstance() {
    if(!_instance) {
        _instance = new CacheManager();
        _instance->init();
    }
    return _instance;
}


void CacheManager::init(){ }

void CacheManager::loadBirdCache(std::function<void()> callback) {
    
    vector<string> fileListMainScene = {
        "MainScene/main_bg_sky.png",
        "MainScene/cloud_day_1.png",
        "MainScene/cloud_day_2.png",
        "MainScene/cloud_day_3.png",
        "MainScene/right_mountain.png",
        "MainScene/main_coop_math.png",
        "MainScene/main_coop_literacy.png",
        "MainScene/day_grass_ground.png",
        "MainScene/main_leaves_left.png",
        "MainScene/main_leaves_right.png",
        "MainScene/coop_highlight_02.png",
        "MainScene/mainscreen_exitbutton_normal.png",
        "MainScene/mainscreen_exitbutton_active.png"
    };
    
    for (auto it : fileListMainScene) {
        if (!FileUtils::getInstance()->isFileExist(it)) continue;
        Director::getInstance()->getTextureCache()->addImageAsync(it,[this, it](Texture2D *){
            CCLOG("file cached: %s", it.c_str());
        });
    }

    bool firstFlag = true;
    for (auto it : CurriculumManager::getInstance()->levels) {
        LevelCurriculum cur = it.second;
        if (!firstFlag && !UserManager::getInstance()->isLevelOpen(cur.levelID)) continue;
        firstFlag = false;
        string birdID = TodoUtil::itos(Bird::getBirdID(cur.category, cur.categoryLevel));
        bool isCleard = UserManager::getInstance()->isLevelCleared(cur.levelID);
        for (int i=0; i<3; i++) {
            string filePath = "";
            if (cur.categoryLevel == CoopScene::LEVEL_FISH_PRESENT) {
                filePath = "BirdAnimation/Bird" + birdID + "/fishpresent.png";
            } else {
                filePath = "BirdAnimation/Bird" + birdID + "/Bird" + birdID + (isCleard?"-crown-":"-")+ TodoUtil::itos(i) + ".png";
            }
            
            //CCLOG("filePath:%s", filePath.c_str());
            if (!FileUtils::getInstance()->isFileExist(filePath)) break;
            _lastFilePath = filePath;
            Director::getInstance()->getTextureCache()->addImageAsync(filePath,[this, filePath, callback](Texture2D *){
                CCLOG("file cached: %s", filePath.c_str());
                if (filePath == _lastFilePath) {
                    if (callback) {
                        callback();
                    }
                    //CCLOG("bird finished!!!");
//                    Director::getInstance()->replaceScene(TouchEventLogger::wrapScene(MainScene::createScene()));
                }
            });
        }
    }

}

void CacheManager::loadCoopCache() {
    
    vector<string> fileListCoopScene = {
        "CoopScene/coop-bg.png",
        "CoopScene/coop-roof-literacy.png",
        "CoopScene/coop_woodpanel_title_literacy.png",
        "CoopScene/coop-woodpanel-level-literacy.png",
        "CoopScene/coop-slot-bottom-literacy.png",
        "CoopScene/coop_bird_shadow.png",
        "CoopScene/coop-light.png",
        "CoopScene/coop-shade.png",
        "CoopScene/coop-slot-bottom-literacy-special.png",
        "CoopScene/effect_egg_glow_1.png",
        "CoopScene/effect_egg_glow_2.png",
        "CoopScene/effect_egg_glow_3.png"
    };
    
    for (auto it : fileListCoopScene) {
        if (!FileUtils::getInstance()->isFileExist(it)) continue;
        Director::getInstance()->getTextureCache()->addImageAsync(it,[this, it](Texture2D *){
            CCLOG("file cached: %s", it.c_str());
        });
    }
}

void CacheManager::loadDailyCache() {
    vector<string> fileListDailyScene = {
        "MainScene/DailyScene/daily_bg_large.png",
        "MainScene/DailyScene/daily_beam_large.png",
        "MainScene/DailyScene/daily_treetop_right.png",
        "MainScene/DailyScene/daily_panel_prek.png",
        "MainScene/DailyScene/freechoice_popup_bg.png",
        "MainScene/DailyScene/freechoice_popup_button_open.png",
        "MainScene/DailyScene/freechoice_icon_normal_bg.png",
        "MainScene/DailyScene/freechoice_icon_normal.png"
    };
    
    for (auto it : fileListDailyScene) {
        if (!FileUtils::getInstance()->isFileExist(it)) continue;
        Director::getInstance()->getTextureCache()->addImageAsync(it,[this, it](Texture2D *){
            CCLOG("file cached: %s", it.c_str());
        });
    }

}
