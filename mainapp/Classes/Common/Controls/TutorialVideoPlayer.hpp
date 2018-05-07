//
//  TutorialVideoPlayer.hpp
//  KitkitSchool
//
//  Created by Gunho Lee on 12/20/16.
//
//

#ifndef TutorialVideoPlayer_hpp
#define TutorialVideoPlayer_hpp

#include <stdio.h>
#include <string>
#include <functional>

#include "cocos2d.h"
#include <cocos/ui/CocosGUI.h>

#include "KitkitVideoPlayer.hpp"

USING_NS_CC;
using namespace std;


class TutorialVideoPlayer : public Layer
{
public:

    CREATE_FUNC(TutorialVideoPlayer);

    virtual bool init() override;
    virtual void onExitTransitionDidStart() override;
    
    void playVideo(string filename);
    
    std::function<void()> onCompleted;
    
protected:
    KitkitVideoPlayer *_player;
    
    void handleVideoComplete();
    
    
};


#endif /* TutorialVideoPlayer_hpp */
