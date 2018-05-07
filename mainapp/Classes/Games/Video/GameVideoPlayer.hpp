//
//  GameVideoPlayer.hpp
//  KitkitSchool
//
//  Created by Sungwoo Kang on 9/6/17.
//
//

#ifndef GameVideoPlayer_hpp
#define GameVideoPlayer_hpp

#include <stdio.h>
#include <string>
#include <functional>

#include "cocos2d.h"
#include <cocos/ui/CocosGUI.h>

#include "Common/Controls/KitkitVideoPlayer.hpp"

USING_NS_CC;
using namespace std;


class GameVideoPlayer : public Layer
{
public:
    
    CREATE_FUNC(GameVideoPlayer);
    
    virtual bool init() override;
    virtual void onExitTransitionDidStart() override;
    
    void playVideo(string filename);
    
    std::function<void()> onCompleted;
    
protected:
    KitkitVideoPlayer *_player;
    
    void handleVideoComplete();
    
    
    string getResourceUri(string filename);


};
#endif /* GameVideoPlayer_hpp */
