//
//  KitkitVideoPlayer.hpp
//  KitkitSchool
//
//  Created by Gunho Lee on 12/10/17.
//

#pragma once

#include "cocos2d.h"
#include <cocos/ui/CocosGUI.h>
#include "ui/UIWidget.h"

USING_NS_CC;

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID || CC_TARGET_PLATFORM == CC_PLATFORM_IOS || CC_TARGET_PLATFORM == CC_PLATFORM_TIZEN) && !defined(CC_PLATFORM_OS_TVOS)


using namespace cocos2d::experimental::ui;

class KitkitVideoPlayer : public VideoPlayer
{
public:
    CREATE_FUNC(KitkitVideoPlayer);
};


#else

#define VIDEO_PLAYER_NOT_AVAILABLE
#define EXTENDED_KITKIT_VIDEO_PLAYER

#undef CC_VIDEOPLAYER_DEBUG_DRAW
#define CC_VIDEOPLAYER_DEBUG_DRAW 0

namespace VideoPlayer {
    enum class EventType
    {
        PLAYING = 0,
        PAUSED,
        STOPPED,
        COMPLETED
    };
};


class KitkitVideoPlayer : public cocos2d::ui::Widget
{
public:
    KitkitVideoPlayer();
    virtual ~KitkitVideoPlayer();
    
    /**
     * A callback which will be called after specific VideoPlayer event happens.
     */
    typedef std::function<void(Ref*,VideoPlayer::EventType)> ccVideoPlayerCallback;
    
    /**
     *Static create method for instancing a VideoPlayer.
     */
    CREATE_FUNC(KitkitVideoPlayer);
    
    /**
     * Sets a file path as a video source for VideoPlayer.
     */
    virtual void setFileName(const std::string& videoPath);
    
    /**
     * @brief Get the local video file name.
     *
     * @return The video file name.
     */
    virtual const std::string& getFileName() const { return _videoURL;}
    
    /**
     * Sets a URL as a video source for VideoPlayer.
     */
    virtual void setURL(const std::string& _videoURL);
    
    
    /**
     * @brief Get the URL of remoting video source.
     *
     * @return A remoting URL address.
     */
    virtual const std::string& getURL() const { return _videoURL;}
    
    /**
     * Starts playback.
     */
    virtual void play();
    
    /**
     * Pauses playback.
     */
    virtual void pause()override;
    
    /**
     * Resumes playback.
     */
    virtual void resume()override;
    
    /**
     * Stops playback.
     */
    virtual void stop();
    
    /**
     * Seeks to specified time position.
     *
     * @param sec   The offset in seconds from the start to seek to.
     */
    virtual void seekTo(float sec);
    
    /**
     * Checks whether the VideoPlayer is playing.
     *
     * @return True if currently playing, false otherwise.
     */
    virtual bool isPlaying() const;
    
    /**
     * Causes the video player to keep aspect ratio or no when displaying the video.
     *
     * @param enable    Specify true to keep aspect ratio or false to scale the video until
     * both dimensions fit the visible bounds of the view exactly.
     */
    virtual void setKeepAspectRatioEnabled(bool enable);
    
    /**
     * Indicates whether the video player keep aspect ratio when displaying the video.
     */
    virtual bool isKeepAspectRatioEnabled()const { return _keepAspectRatioEnabled;}
    
    /**
     * Causes the video player to enter or exit full-screen mode.
     *
     * @param fullscreen    Specify true to enter full-screen mode or false to exit full-screen mode.
     */
    virtual void setFullScreenEnabled(bool fullscreen);
    
    /**
     * Indicates whether the video player is in full-screen mode.
     *
     * @return True if the video player is in full-screen mode, false otherwise.
     */
    virtual bool isFullScreenEnabled()const;
    
    /**
     * Register a callback to be invoked when the video state is updated.
     *
     * @param callback  The callback that will be run.
     */
    virtual void addEventListener(const KitkitVideoPlayer::ccVideoPlayerCallback& callback);
    
    /**
     * @brief A function which will be called when video is playing.
     *
     * @param event @see VideoPlayer::EventType.
     
     */
    virtual void onPlayEvent(int event);

    /**
     * @brief A function which will be called when video is playing.
     *
     * @param event @see VideoPlayer::EventType.
     
     */

    virtual void setVisible(bool visible) override;
    virtual void draw(Renderer *renderer, const Mat4& transform, uint32_t flags) override;
    virtual void onEnter() override;
    virtual void onExit() override;
    
protected:
    virtual cocos2d::ui::Widget* createCloneInstance() override;
    virtual void copySpecialProperties(Widget* model) override;



    
protected:
#if CC_VIDEOPLAYER_DEBUG_DRAW
    DrawNode *_debugDrawNode;
#endif
    enum class Source
    {
        FILENAME = 0,
        URL
    };
    
    bool _isPlaying;
    bool _fullScreenDirty;
    bool _fullScreenEnabled;
    bool _keepAspectRatioEnabled;
    
    std::string _videoURL;
    Source _videoSource;
    
    int _videoPlayerIndex;
    ccVideoPlayerCallback _eventCallback;
    
    void* _videoView;
    
    
    
};



#endif



