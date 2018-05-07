//
//  KitkitVideoPlayer.cpp
//  KitkitSchool
//
//  Created by Gunho Lee on 12/10/17.
//

#include "KitkitVideoPlayer.hpp"


#if (defined(VIDEO_PLAYER_NOT_AVAILABLE) && (CC_TARGET_PLATFORM == CC_PLATFORM_WINRT))


struct winVideoView {
	Windows::UI::Xaml::Controls::Canvas ^canvas;
	Windows::UI::Xaml::Controls::MediaElement ^media;
	//std::function<void(Platform::Object ^sender, Windows::UI::Xaml::RoutedEventArgs^ e)> &endHandler;
	//Windows::UI::Xaml::RoutedEventHandler ^endHandler;
	Windows::Foundation::EventRegistrationToken endHandlerToken, failedHandlerToken;

};

KitkitVideoPlayer::KitkitVideoPlayer()
	: _isPlaying(false)
	, _fullScreenDirty(false)
	, _fullScreenEnabled(false)
	, _keepAspectRatioEnabled(false)
	, _videoPlayerIndex(-1)
	, _eventCallback(nullptr)
{
	
	auto view = new winVideoView();
	_videoView = view;

	auto dispatcher = cocos2d::GLViewImpl::sharedOpenGLView()->getDispatcher();
	auto panel = cocos2d::GLViewImpl::sharedOpenGLView()->getPanel();

	if (dispatcher->GetType() && panel->GetType()) {

		dispatcher->RunAsync(Windows::UI::Core::CoreDispatcherPriority::Normal, ref new Windows::UI::Core::DispatchedHandler([this]() {

			auto panel = cocos2d::GLViewImpl::sharedOpenGLView()->getPanel();
			double height = panel->ActualHeight;
			double width = panel->ActualWidth;

			auto view = (winVideoView*)_videoView;

			auto canvas = ref new Windows::UI::Xaml::Controls::Canvas();
			view->canvas = canvas;
			canvas->Height = height;
			canvas->Width = width;
			panel->Children->Append(canvas);

			

			auto mediaElement = ref new Windows::UI::Xaml::Controls::MediaElement();
			view->media = mediaElement;

			//mediaElement->Width = width / 2;
			//mediaElement->Height = height / 2;
			//mediaElement->SetValue(Windows::UI::Xaml::Controls::Canvas::LeftProperty, 200);
			//mediaElement->SetValue(Windows::UI::Xaml::Controls::Canvas::TopProperty, 200);


			mediaElement->AutoPlay = true;
			mediaElement->IsFullWindow = false;
			mediaElement->Name = "Intro";
			mediaElement->IsMuted = false;

			auto endHandler = [this](Platform::Object ^sender, Windows::UI::Xaml::RoutedEventArgs^ e) {

				this->onPlayEvent((int)VideoPlayer::EventType::COMPLETED);

			};
			
			auto failedHandler = [](Platform::Object ^sender, Windows::UI::Xaml::RoutedEventArgs^ e) {

				MessageBox("media play failed", "Error");

			};

			view->endHandlerToken = mediaElement->MediaEnded += ref new Windows::UI::Xaml::RoutedEventHandler(endHandler);
			view->failedHandlerToken = mediaElement->MediaFailed += ref new Windows::UI::Xaml::ExceptionRoutedEventHandler(failedHandler);


			canvas->Children->Append(mediaElement);
		}));
	}

#if CC_VIDEOPLAYER_DEBUG_DRAW
	_debugDrawNode = DrawNode::create();
	addChild(_debugDrawNode);
#endif
}

KitkitVideoPlayer::~KitkitVideoPlayer()
{
	if (_videoView) {
		auto view = (winVideoView*)_videoView;
		auto dispatcher = cocos2d::GLViewImpl::sharedOpenGLView()->getDispatcher();


		if (dispatcher->GetType()) {


			dispatcher->RunAsync(Windows::UI::Core::CoreDispatcherPriority::Normal, ref new Windows::UI::Core::DispatchedHandler([view]() {

				if (view->media) {

					view->media->Pause();
					view->media->Stop();
					view->media->MediaEnded -= view->endHandlerToken;
					view->media->MediaFailed -= view->failedHandlerToken;
					delete view->media;

				}
				if (view->canvas) {
					delete view->canvas;
				}


				delete view;
			}));
		}
	}
}

void KitkitVideoPlayer::setFileName(const std::string& fileName)
{
	_videoURL = FileUtils::getInstance()->fullPathForFilename(fileName);
	_videoSource = KitkitVideoPlayer::Source::FILENAME;
	
}

void KitkitVideoPlayer::setURL(const std::string& videoUrl)
{
	_videoURL = videoUrl;
	_videoSource = KitkitVideoPlayer::Source::URL;
	
}

void KitkitVideoPlayer::draw(Renderer* renderer, const Mat4 &transform, uint32_t flags)
{
	cocos2d::ui::Widget::draw(renderer, transform, flags);

	if (flags & FLAGS_TRANSFORM_DIRTY)
	{



		if (_videoView) {
			auto view = (winVideoView*)_videoView;
			auto mediaSize = getContentSize();
			auto leftBottom = convertToWorldSpace(Vec2::ZERO);
			auto rightTop = convertToWorldSpace(Vec2(_contentSize.width, _contentSize.height));

			auto dispatcher = cocos2d::GLViewImpl::sharedOpenGLView()->getDispatcher();
			if (dispatcher->GetType()) {

				dispatcher->RunAsync(Windows::UI::Core::CoreDispatcherPriority::Normal, ref new Windows::UI::Core::DispatchedHandler([view, mediaSize, leftBottom, rightTop]() {
					


					auto winSize = Director::getInstance()->getWinSize();
					

					auto canvasSize = Size(view->canvas->Width, view->canvas->Height);

					auto uiWidth = mediaSize.width * (canvasSize.width / winSize.width);
					auto uiHeight = mediaSize.height * (canvasSize.height / winSize.height);

					auto uiLeft = leftBottom.x  * (canvasSize.width / winSize.width);
					auto uiTop = (winSize.height - rightTop.y)  * (canvasSize.width / winSize.width);

					view->media->Width = uiWidth;
					view->media->Height = uiHeight;

					view->media->SetValue(Windows::UI::Xaml::Controls::Canvas::LeftProperty, uiLeft);
					view->media->SetValue(Windows::UI::Xaml::Controls::Canvas::TopProperty, uiTop);

				}));		
			}
		}

	}

#if CC_VIDEOPLAYER_DEBUG_DRAW
	_debugDrawNode->clear();
	auto size = getContentSize();
	Point vertices[4] =
	{
		Point::ZERO,
		Point(size.width, 0),
		Point(size.width, size.height),
		Point(0, size.height)
	};
	_debugDrawNode->drawPoly(vertices, 4, true, Color4F(1.0, 1.0, 1.0, 1.0));
#endif
}

bool KitkitVideoPlayer::isFullScreenEnabled()const
{
	return false;
}

void KitkitVideoPlayer::setFullScreenEnabled(bool enabled)
{
	
}

void KitkitVideoPlayer::setKeepAspectRatioEnabled(bool enable)
{
	if (_keepAspectRatioEnabled != enable)
	{
		_keepAspectRatioEnabled = enable;
	//	[((UIVideoViewWrapperMac*)_videoView) setKeepRatioEnabled:enable];
	}
}


void KitkitVideoPlayer::play()
{

	


	if (!_videoURL.empty())
	{

		auto dispatcher = cocos2d::GLViewImpl::sharedOpenGLView()->getDispatcher();
		auto m_panel = cocos2d::GLViewImpl::sharedOpenGLView()->getPanel();
		if (dispatcher->GetType() && m_panel->GetType()) {

			dispatcher->RunAsync(Windows::UI::Core::CoreDispatcherPriority::Normal, ref new Windows::UI::Core::DispatchedHandler([this]() {

				auto conv = [](const std::string& s) {
					if (s.empty())
						return ref new Platform::String();

					int len = MultiByteToWideChar(CP_UTF8, 0, s.c_str(), s.length(), NULL, 0);
					auto ps = ref new Platform::String(L"", len);
					wchar_t* pPSBuf = const_cast<wchar_t*>(ps->Data());
					MultiByteToWideChar(CP_UTF8, 0, s.c_str(), -1, pPSBuf, len);
					return ps;
				};

				auto mediaElement = ((winVideoView*)_videoView)->media;
				auto urlString = conv(_videoURL);
				Windows::Foundation::Uri^ uri = ref new Windows::Foundation::Uri(urlString);
				mediaElement->Source = uri;

			}));
		}

	}
}

void KitkitVideoPlayer::pause()
{
	if (!_videoURL.empty())
	{
	//	[((UIVideoViewWrapperMac*)_videoView) pause];
	}
}

void KitkitVideoPlayer::resume()
{
	if (!_videoURL.empty())
	{
		//[((UIVideoViewWrapperMac*)_videoView) resume];
	}
}

void KitkitVideoPlayer::stop()
{
	if (!_videoURL.empty())
	{
	//	[((UIVideoViewWrapperMac*)_videoView) stop];
	}
}

void KitkitVideoPlayer::seekTo(float sec)
{
	if (!_videoURL.empty())
	{
		//[((UIVideoViewWrapperMac*)_videoView) seekTo:sec];
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
	//	[((UIVideoViewWrapperMac*)_videoView) setVisible:NO];
	}
	else if (isRunning())
	{
		//[((UIVideoViewWrapperMac*)_videoView) setVisible:YES];
	}
}

void KitkitVideoPlayer::onEnter()
{
	Widget::onEnter();
	if (isVisible())
	{
	//	[((UIVideoViewWrapperMac*)_videoView) setVisible:YES];
	}
}

void KitkitVideoPlayer::onExit()
{
	Widget::onExit();

	auto dispatcher = cocos2d::GLViewImpl::sharedOpenGLView()->getDispatcher();
	auto m_panel = cocos2d::GLViewImpl::sharedOpenGLView()->getPanel();
	if (dispatcher->GetType() && m_panel->GetType() && _videoView) {

		
		auto action = dispatcher->RunAsync(Windows::UI::Core::CoreDispatcherPriority::Normal, ref new Windows::UI::Core::DispatchedHandler([this]() {
			auto view = (winVideoView*)_videoView;
			view->canvas->Visibility = Windows::UI::Xaml::Visibility::Collapsed;


		}));

		auto task = concurrency::create_task(action);
		task.get();



		

	}

//	[((UIVideoViewWrapperMac*)_videoView) setVisible:NO];
}

void KitkitVideoPlayer::addEventListener(const KitkitVideoPlayer::ccVideoPlayerCallback& callback)
{
	_eventCallback = callback;
}

void KitkitVideoPlayer::onPlayEvent(int event)
{
	if (event == (int)VideoPlayer::EventType::PLAYING) {
		_isPlaying = true;
	}
	else {
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

