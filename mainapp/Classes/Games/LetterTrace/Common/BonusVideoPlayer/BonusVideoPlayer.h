//
//  BonusVideoPlayer.h on May 25, 2016
//  TodoSchool
//
//  Copyright (c) 2016 Enuma, Inc. All rights reserved.
//  See LICENSE.md for more details.
//


#pragma once

#include <Games/NumberTrace/Common/ADT/Chain.h>
#include <Games/NumberTrace/Common/Basic/TodoSchoolNode.h>
#include <cocos/cocos2d.h>
#include <cocos/ui/CocosGUI.h>
#include <string>

namespace todoschool {

class BonusVideoPlayer : public cocos2d::Node {
    typedef cocos2d::Node Super;
    cocos2d::experimental::ui::VideoPlayer* TheVideoPlayer;

public:
    // NB(xenosoz, 2016): Here, "Path = (Path) + Filename".
    Chain<std::string> VideoFilePath;

    std::function<void(BonusVideoPlayer*)> OnPlayDidBegin;
    std::function<void(BonusVideoPlayer*)> OnPlayDidPause;
    std::function<void(BonusVideoPlayer*)> OnPlayDidStop;
    std::function<void(BonusVideoPlayer*)> OnPlayDidEnd;

public:
    CREATE_FUNC(BonusVideoPlayer);
    BonusVideoPlayer();
    bool init() override;
    void setContentSize(const cocos2d::Size& ContentSize) override;
    void setVisible(bool Visible) override;

    void clear();
    void play();
    void stop();

private:
    void clearInternals();
    void handleVideoPlayerStateUpdated(cocos2d::Ref*, cocos2d::experimental::ui::VideoPlayer::EventType);
};

} // namespace todoschool
