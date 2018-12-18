//
//  LRSoundButton.hpp
//  KitkitSchool
//
//  Created by HyeonGyu Yu on 21/09/2018.
//

#ifndef LRSoundButton_hpp
#define LRSoundButton_hpp

#include "cocos2d.h"
#include "ui/CocosGUI.h"

#include <stdio.h>

USING_NS_CC;
using namespace ui;
using namespace std;


class LRSoundButton : public Node
{
public:
    bool init(bool isLarge, string soundName);
    static LRSoundButton* createLarge(string soundName);
    static LRSoundButton* createSmall(string soundName);
    virtual void onEnter() override;

    static vector<LRSoundButton*> soundButtons;
    bool isPlaying();
    void setPressed(bool isPressed);
    
private:
    map<string,float> _durationMap;
    string _soundName;
    float _duration = 0.f;
    bool _isPlaying = false;
    Sprite* _defaultImage;
    Sprite* _playingImage;
    
    void initData();
    void playSound();
};

#endif /* LRSoundButton_hpp */
