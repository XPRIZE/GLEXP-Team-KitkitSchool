//
//  SignLanguageVideoPlayer.hpp
//  KitkitSchool
//
//  Created by HyeonGyu Yu on 5/11/18.
//

#ifndef SignLanguageVideoPlayer_hpp
#define SignLanguageVideoPlayer_hpp


#define SHOW_SL_VIDEO_IF_ENABLED(VIDEO_PATH)\
SignLanguageVideoPlayer::playVideoIfSignLanguageMode(this, VIDEO_PATH);

#define SHOW_SL_VIDEO_WITH_CALLBACK_IF_ENABLED(VIDEO_PATH, CALLBACK_CONTENT)\
SignLanguageVideoPlayer::playVideoIfSignLanguageMode(this, VIDEO_PATH, [this](){\
DELAYED_CALLFUNC(0.01f, {\
CALLBACK_CONTENT;\
});\
});


#include <stdio.h>
#include <string>
#include <functional>

#include "cocos2d.h"
#include <cocos/ui/CocosGUI.h>

#include "KitkitVideoPlayer.hpp"

USING_NS_CC;
using namespace std;

class SignLanguageVideoPlayer : public Layer
{
public:
    CREATE_FUNC(SignLanguageVideoPlayer);
    virtual bool init() override;
    virtual void onExit() override;
    
    static SignLanguageVideoPlayer *currentPlayer;
    static void playVideoIfSignLanguageMode(Node* parent, string videoPath, function<void()> callback = nullptr);
    
    void playVideo(string filename);
    std::function<void()> onCompleted;
    
protected:
    KitkitVideoPlayer *_player;
    void handleVideoComplete();
    string getResourceUri(string filename);
    
};

#endif /* SignLanguageVideoPlayer_hpp */
