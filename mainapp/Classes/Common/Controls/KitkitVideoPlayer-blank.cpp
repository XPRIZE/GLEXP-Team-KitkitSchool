//
//  KitkitVideoPlayer.cpp
//  KitkitSchool
//
//  Created by Gunho Lee on 12/10/17.
//

#include "KitkitVideoPlayer.hpp"

#ifdef VIDEO_PLAYER_NOT_AVAILABLE

void KitkitVideoPlayer::setFileName(const std::string& videoPath) { }
const std::string& KitkitVideoPlayer::getFileName() const { return _videoURL;}
void KitkitVideoPlayer::setURL(const std::string& _videoURL) {};
const std::string& KitkitVideoPlayer::getURL() const { return _videoURL;}
void KitkitVideoPlayer::play() {
    
    if (_eventCallback) {
        scheduleOnce([this](float) {
            _eventCallback(this, VideoPlayer::EventType::COMPLETED);
        }, 0.1, "VideoPlayerNotAvailable");
    }
    
    
}
void KitkitVideoPlayer::stop()  {  }
void KitkitVideoPlayer::seekTo(float sec) {}
bool KitkitVideoPlayer::isPlaying()const { return false; }
void KitkitVideoPlayer::setKeepAspectRatioEnabled(bool enable) {}
bool KitkitVideoPlayer::isKeepAspectRatioEnabled()const { return true;}
void KitkitVideoPlayer::setFullScreenEnabled(bool fullscreen) {}
bool KitkitVideoPlayer::isFullScreenEnabled()const { return false; }
void KitkitVideoPlayer::addEventListener(const KitkitVideoPlayer::ccVideoPlayerCallback& callback) { _eventCallback = callback; }
void KitkitVideoPlayer::onPlayEvent(int event) {}

#endif
