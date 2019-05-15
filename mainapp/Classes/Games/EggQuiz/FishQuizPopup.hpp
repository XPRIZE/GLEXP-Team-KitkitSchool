//
//  EggQuizPopup.hpp
//  KitkitSchool
//
//  Created by YongSoo Hong on 9/3/17.
//
//

#ifndef FishQuizPopup_hpp
#define FishQuizPopup_hpp

#include <stdio.h>
#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "Common/Controls/PopupBase.hpp"
#include "Managers/CurriculumManager.hpp"
#include "Menu/Fish.hpp"
#include "Utils/TodoUtil.h"

USING_NS_CC;
using namespace std;

typedef struct FishData {
    int _id;
    string _userName;
    string _fishID;
    string _skinNo;
    string _name;
    string _createTime;
    long _time;
    string _position;
    
    string toString() {
        string result = "FishData : ";
        result += "{";
        result += "\n_id : " + TodoUtil::itos(_id);
        result += "\n_userName : " + _userName;
        result += "\n_fishID : " + _fishID;
        result += "\n_skinNo : " + _skinNo;
        result += "\n_name : " + _name;
        result += "\n_createTime : " + _createTime;
        result += "\n_position : " + _position;
        result += "\n}";
        return result;
    };
} FISH_DATA;

class FishQuizPopup : public PopupBase
{
public:
    static FishQuizPopup* create(Node* parent);
    static FishQuizPopup* create(Node* parent, Size size);
    
    void dismiss(bool animate);
    
protected:
    LevelCurriculum *_cur;
    
public:
    void setFishCompleted(char category, int progressLevelIndex, int progressIndex, int score);

private:
    Node* createGlow();
    ui::Button* createButton(string prefix, string text);
    
    void showSuccess(char category, int progressLevelIndex, int progressIndex, int score);
    void showFail(char category, int progressLevelIndex, int progressIndex, int score);

    void loadFishDataFromDB();
    void addFishToDB(Fish* fish);
    
    int generateFishSkin(string fishID);
    string generateFishPosition(string fishID);
    FISH_DATA* findDeleteFish(string fishID, string position = "");
    
    std::vector<FISH_DATA*> _fishData;
};

#endif /* FishQuizPopup_hpp */
