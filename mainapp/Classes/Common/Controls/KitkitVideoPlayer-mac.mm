
#include "KitkitVideoPlayer.hpp"

#if (CC_TARGET_PLATFORM == CC_PLATFORM_MAC)
//-------------------------------------------------------------------------------------

#include "platform/desktop/CCGLViewImpl-desktop.h"
#include "base/CCDirector.h"
#include "platform/CCFileUtils.h"

#import <AVKit/AVPlayerView.h>

USING_NS_CC;



@interface UIVideoViewWrapperMac : NSObject

@property (strong,nonatomic) AVPlayerView *movieView;


- (void) setFrame:(int) left :(int) top :(int) width :(int) height;
- (void) setURL:(int) videoSource :(std::string&) videoUrl;
- (void) play;
- (void) pause;
- (void) resume;
- (void) stop;
- (void) seekTo:(float) sec;
- (void) setVisible:(BOOL) visible;
- (void) setKeepRatioEnabled:(BOOL) enabled;
- (void) setFullScreenEnabled:(BOOL) enabled;
- (BOOL) isFullScreenEnabled;

-(id) init:(void*) videoPlayer;

-(void) videoFinished:(NSNotification*) notification;
-(void) playStateChange;


@end

@implementation UIVideoViewWrapperMac
{
    int _left;
    int _top;
    int _width;
    int _height;
    bool _keepRatioEnabled;

    KitkitVideoPlayer* _kitkitVideoPlayer;
}

-(id)init:(void*)kitkitVideoPlayer
{
    if (self = [super init]) {
        self.movieView = nullptr;
        _kitkitVideoPlayer = (KitkitVideoPlayer*)kitkitVideoPlayer;
        _keepRatioEnabled = false;
    }

    return self;
}

-(void) dealloc
{
  
    if (self.movieView != nullptr) {
        
        if (self.movieView.player.currentItem != nullptr) {
            [[NSNotificationCenter defaultCenter] removeObserver:self name:AVPlayerItemDidPlayToEndTimeNotification object:self.movieView.player.currentItem];
        }
        
        [self.movieView.player pause];
        [self.movieView removeFromSuperview];
        self.movieView = nullptr;
        
    }
    _kitkitVideoPlayer = nullptr;
    [super dealloc];
}

-(void) setFrame:(int)left :(int)top :(int)width :(int)height
{
    _left = left;
    _width = width;
    _top = top;
    _height = height;

    if (self.movieView != nullptr) {
        [self.movieView setFrame:CGRectMake(left, top, width, height)];
    }
}

-(void) setFullScreenEnabled:(BOOL) enabled
{
//    if (self.movieView != nullptr) {
//        [self.movieView.player fullsc setFullscreen:enabled animated:(true)];
//    }
}

-(BOOL) isFullScreenEnabled
{
//    if (self.movieView != nullptr) {
//        return [self.movieView.player isFullscreen];
//    }

    return false;
}

-(void) setURL:(int)videoSource :(std::string &)videoUrl
{
    if (self.movieView != nullptr) {
        
        if (self.movieView.player.currentItem != nullptr) {
            [[NSNotificationCenter defaultCenter] removeObserver:self name:AVPlayerItemDidPlayToEndTimeNotification object:self.movieView.player.currentItem];
        }
        
        [self.movieView.player pause];
        [self.movieView removeFromSuperview];
        self.movieView = nullptr;
    }
    
    self.movieView = [[AVPlayerView alloc] init];
    NSString *videoString = @(videoUrl.c_str());
    
    //NSString *videoPath = [videoString stringByDeletingPathExtension];
    //NSString *videoExt = [videoString pathExtension];
    
    //NSURL* videoURL = [NSURL URLWithString:  [NSString stringWithFormat:@"file://%@", videoString]];
    //NSURL* videoURL = [[NSBundle mainBundle] URLForResource:videoPath withExtension:videoExt];
    //NSURL* videoURL2 = [NSURL URLWithString:@(videoUrl.c_str())];
    NSURL* videoURL = [NSURL fileURLWithPath:videoString];
    self.movieView.player = [AVPlayer playerWithURL:videoURL];
    
    
//    if (videoSource == 1) {
        
        
//
//        self.moviePlayer = [[[MPMoviePlayerController alloc] init] autorelease];
//        self.moviePlayer.movieSourceType = MPMovieSourceTypeStreaming;
//        [self.moviePlayer setContentURL:[NSURL URLWithString:@(videoUrl.c_str())]];
//    } else {
//        self.moviePlayer = [[[MPMoviePlayerController alloc] initWithContentURL:[NSURL fileURLWithPath:@(videoUrl.c_str())]] autorelease];
//        self.moviePlayer.movieSourceType = MPMovieSourceTypeFile;
//    }
    self.movieView.controlsStyle = AVPlayerViewControlsStyleNone;
    
//    self.moviePlayer.allowsAirPlay = false;
//    self.moviePlayer.controlStyle = MPMovieControlStyleEmbedded;
//    self.moviePlayer.view.userInteractionEnabled = true;

    

    
    
    if (_keepRatioEnabled) {
        self.movieView.videoGravity = AVLayerVideoGravityResizeAspect;
    } else {
        self.movieView.videoGravity = AVLayerVideoGravityResize;
    }

    auto view = cocos2d::Director::getInstance()->getOpenGLView();

    NSWindow *cocoaWindow = view->getCocoaWindow();
    [cocoaWindow.contentView addSubview:self.movieView];
    
    [[NSNotificationCenter defaultCenter] addObserver:self selector:@selector(videoFinished:) name:AVPlayerItemDidPlayToEndTimeNotification object:self.movieView.player.currentItem];

}

-(void) videoFinished:(NSNotification *)notification
{
    if(_kitkitVideoPlayer != nullptr)
    {
        //if([self.movieView playbackState] != MPMoviePlaybackStateStopped)
        {
            _kitkitVideoPlayer->onPlayEvent((int)VideoPlayer::EventType::COMPLETED);
        }
    }
}

-(void) playStateChange
{
//    MPMoviePlaybackState state = [self.moviePlayer playbackState];
//    switch (state) {
//        case MPMoviePlaybackStatePaused:
//            _videoPlayer->onPlayEvent((int)VideoPlayer::EventType::PAUSED);
//            break;
//        case MPMoviePlaybackStateStopped:
//            _videoPlayer->onPlayEvent((int)VideoPlayer::EventType::STOPPED);
//            break;
//        case MPMoviePlaybackStatePlaying:
//            _videoPlayer->onPlayEvent((int)VideoPlayer::EventType::PLAYING);
//            break;
//        case MPMoviePlaybackStateInterrupted:
//            break;
//        case MPMoviePlaybackStateSeekingBackward:
//            break;
//        case MPMoviePlaybackStateSeekingForward:
//            break;
//        default:
//            break;
//    }
}

-(void) seekTo:(float)sec
{
    if (self.movieView != NULL) {
        [self.movieView.player seekToTime:CMTimeMakeWithSeconds(sec, 60000)];
    }
}

-(void) setVisible:(BOOL)visible
{
    if (self.movieView != NULL) {
        [self.movieView setHidden:!visible];
    }
}

-(void) setKeepRatioEnabled:(BOOL)enabled
{
    _keepRatioEnabled = enabled;

    if (self.movieView != NULL) {
        if (enabled) {
            self.movieView.videoGravity = AVLayerVideoGravityResizeAspect;
            
        } else {
            self.movieView.videoGravity = AVLayerVideoGravityResize;
        }
    }
}

-(void) play
{
    if (self.movieView != NULL) {
        [self.movieView.player play];
    }
}

-(void) pause
{
    if (self.movieView != NULL) {
        [self.movieView.player pause];
    }
}

-(void) resume
{
//    if (self.movieView != NULL) {
//        self.movieView.player.status
//        if([self.moviePlayer playbackState] == MPMoviePlaybackStatePaused)
//        {
//            [self.moviePlayer play];
//        }
//    }
}

-(void) stop
{
    if (self.movieView != NULL) {
        [self.movieView.player pause];
    }
}

@end
//------------------------------------------------------------------------------------------------------------

KitkitVideoPlayer::KitkitVideoPlayer()
: _isPlaying(false)
, _fullScreenDirty(false)
, _fullScreenEnabled(false)
, _keepAspectRatioEnabled(false)
, _videoPlayerIndex(-1)
, _eventCallback(nullptr)
{
    _videoView = [[UIVideoViewWrapperMac alloc] init:this];

#if CC_VIDEOPLAYER_DEBUG_DRAW
    _debugDrawNode = DrawNode::create();
    addChild(_debugDrawNode);
#endif
}

KitkitVideoPlayer::~KitkitVideoPlayer()
{
    if(_videoView)
    {
        [((UIVideoViewWrapperMac*)_videoView) dealloc];
    }
}

void KitkitVideoPlayer::setFileName(const std::string& fileName)
{
    _videoURL = FileUtils::getInstance()->fullPathForFilename(fileName);
    _videoSource = KitkitVideoPlayer::Source::FILENAME;
    [((UIVideoViewWrapperMac*)_videoView) setURL:(int)_videoSource :_videoURL];
}

void KitkitVideoPlayer::setURL(const std::string& videoUrl)
{
    _videoURL = videoUrl;
    _videoSource = KitkitVideoPlayer::Source::URL;
    [((UIVideoViewWrapperMac*)_videoView) setURL:(int)_videoSource :_videoURL];
}

void KitkitVideoPlayer::draw(Renderer* renderer, const Mat4 &transform, uint32_t flags)
{
    cocos2d::ui::Widget::draw(renderer,transform,flags);

    if (flags & FLAGS_TRANSFORM_DIRTY)
    {
        auto directorInstance = Director::getInstance();
        auto glView = directorInstance->getOpenGLView();
        auto frameSize = glView->getFrameSize();

        
        
        auto scaleFactor = glView->getContentScaleFactor();//[static_cast<CCEAGLView *>(glView->getEAGLView()) contentScaleFactor];
        

        auto winSize = directorInstance->getWinSize();

        auto leftBottom = convertToWorldSpace(Vec2::ZERO);
        auto rightTop = convertToWorldSpace(Vec2(_contentSize.width,_contentSize.height));
        auto playerHeight = rightTop.y - leftBottom.y;

        auto uiLeft = (frameSize.width / 2 + (leftBottom.x - winSize.width / 2 ) * glView->getScaleX()) / scaleFactor;
        
        // weird coordination scale for mac.... (gunho)
        auto uiTop = (frameSize.height /2 + (rightTop.y - winSize.height / 2 - playerHeight ) * glView->getScaleY()) / scaleFactor;

        [((UIVideoViewWrapperMac*)_videoView) setFrame :uiLeft :uiTop
                                                          :(rightTop.x - leftBottom.x) * glView->getScaleX() / scaleFactor
                                                          :( (rightTop.y - leftBottom.y) * glView->getScaleY()/scaleFactor)];
    }

#if CC_VIDEOPLAYER_DEBUG_DRAW
    _debugDrawNode->clear();
    auto size = getContentSize();
    cocos2d::Point vertices[4]=
    {
        cocos2d::Point::ZERO,
        cocos2d::Point(size.width, 0),
        cocos2d::Point(size.width, size.height),
        cocos2d::Point(0, size.height)
    };
    _debugDrawNode->drawPoly(vertices, 4, true, Color4F(1.0, 1.0, 1.0, 1.0));
#endif
}

bool KitkitVideoPlayer::isFullScreenEnabled()const
{
    return [((UIVideoViewWrapperMac*)_videoView) isFullScreenEnabled];
}

void KitkitVideoPlayer::setFullScreenEnabled(bool enabled)
{
    [((UIVideoViewWrapperMac*)_videoView) setFullScreenEnabled:enabled];
}

void KitkitVideoPlayer::setKeepAspectRatioEnabled(bool enable)
{
    if (_keepAspectRatioEnabled != enable)
    {
        _keepAspectRatioEnabled = enable;
        [((UIVideoViewWrapperMac*)_videoView) setKeepRatioEnabled:enable];
    }
}

void KitkitVideoPlayer::play()
{
    if (! _videoURL.empty())
    {
        [((UIVideoViewWrapperMac*)_videoView) play];
    }
}

void KitkitVideoPlayer::pause()
{
    if (! _videoURL.empty())
    {
        [((UIVideoViewWrapperMac*)_videoView) pause];
    }
}

void KitkitVideoPlayer::resume()
{
    if (! _videoURL.empty())
    {
        [((UIVideoViewWrapperMac*)_videoView) resume];
    }
}

void KitkitVideoPlayer::stop()
{
    if (! _videoURL.empty())
    {
        [((UIVideoViewWrapperMac*)_videoView) stop];
    }
}

void KitkitVideoPlayer::seekTo(float sec)
{
    if (! _videoURL.empty())
    {
        [((UIVideoViewWrapperMac*)_videoView) seekTo:sec];
    }
}

bool KitkitVideoPlayer::isPlaying() const
{
    return _isPlaying;
}

void KitkitVideoPlayer::setVisible(bool visible)
{
    cocos2d::ui::Widget::setVisible(visible);

    if (!visible)
    {
        [((UIVideoViewWrapperMac*)_videoView) setVisible:NO];
    }
    else if(isRunning())
    {
        [((UIVideoViewWrapperMac*)_videoView) setVisible:YES];
    }
}

void KitkitVideoPlayer::onEnter()
{
    Widget::onEnter();
    if (isVisible())
    {
        [((UIVideoViewWrapperMac*)_videoView) setVisible: YES];
    }
}

void KitkitVideoPlayer::onExit()
{
    Widget::onExit();
    [((UIVideoViewWrapperMac*)_videoView) setVisible: NO];
}

void KitkitVideoPlayer::addEventListener(const KitkitVideoPlayer::ccVideoPlayerCallback& callback)
{
    _eventCallback = callback;
}

void KitkitVideoPlayer::onPlayEvent(int event)
{
    if (event == (int)VideoPlayer::EventType::PLAYING) {
        _isPlaying = true;
    } else {
        _isPlaying = false;
    }

    if (_eventCallback)
    {
        _eventCallback(this, (VideoPlayer::EventType)event);
    }
}

cocos2d::ui::Widget* KitkitVideoPlayer::createCloneInstance()
{
    return KitkitVideoPlayer::create();
}

void KitkitVideoPlayer::copySpecialProperties(Widget *widget)
{
    KitkitVideoPlayer* videoPlayer = dynamic_cast<KitkitVideoPlayer*>(widget);
    if (videoPlayer)
    {
        _isPlaying = videoPlayer->_isPlaying;
        _fullScreenEnabled = videoPlayer->_fullScreenEnabled;
        _fullScreenDirty = videoPlayer->_fullScreenDirty;
        _videoURL = videoPlayer->_videoURL;
        _keepAspectRatioEnabled = videoPlayer->_keepAspectRatioEnabled;
        _videoSource = videoPlayer->_videoSource;
        _videoPlayerIndex = videoPlayer->_videoPlayerIndex;
        _eventCallback = videoPlayer->_eventCallback;
        _videoView = videoPlayer->_videoView;
    }
}

#endif

