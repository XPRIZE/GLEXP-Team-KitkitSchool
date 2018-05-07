//
//  LevelOpenPopup.hpp
//  KitkitSchool
//
//  Created by Gunho Lee on 12/25/16.
//
//

#ifndef LevelOpenPopup_hpp
#define LevelOpenPopup_hpp

#include "Common/Controls/PopupBase.hpp"
#include <cocos/ui/CocosGUI.h>
#include <functional>

USING_NS_CC;

class LevelOpenPopup : public PopupBase
{
public:
    
    LevelOpenPopup() : onOpenLevel(nullptr) {}
    
    static LevelOpenPopup* create(Node* parentView);
    bool init(Node* parentView);
    void setup(std::string levelID);
    void playGuideVoice(float dt);
    
    std::function<void()> onOpenLevel;
    

private:

};

#endif /* LevelOpenPopup_hpp */
