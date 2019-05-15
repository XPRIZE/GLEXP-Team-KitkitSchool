//
//  LRGuideView.hpp
//  KitkitSchool
//
//  Created by HyeonGyu Yu on 08/08/2018.
//

#ifndef LRGuideView_hpp
#define LRGuideView_hpp

#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include <stdio.h>

#include "LRSoundButton.hpp"

USING_NS_CC;
using namespace ui;
using namespace std;

class LRGuideView : public Node
{
public:
    bool init(bool isSoundType, string soundPath, string textContent);
    
    LRGuideView();
    static LRGuideView* createWithSound(string soundPath);
    static LRGuideView* createWithText(string textContent);
    
//
private:
    LRSoundButton* _speakerButton;
    Label* _textLabel;
    bool _bSoundType;
    bool _bActive;
    string _soundPath;
    string _textContent;
    
public:
    function<void()> onClickLeftButton = nullptr;
    function<void()> onClickRightButton = nullptr;
    function<void()> onClickSpeakerButton = nullptr;
    
    Button* leftButton;
    Button* rightButton;
    
    void setSoundPath(string soundPath) { _soundPath = soundPath; }
    void setTextContent(string textContent) { _textContent = textContent; }
    void setSoundType(bool isSoundType) { _bSoundType = isSoundType; }
    
    void showPanel(bool isShow);
};

#endif /* LRGuideView_hpp */
