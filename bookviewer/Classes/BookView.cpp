//
//  BookView.cpp
//  booktest
//
//  Created by Gunho Lee on 6/29/16.
//
//

#include "BookView.hpp"


#include "Utils/TodoUtil.h"
#include "Common/Controls/CompletePopup.hpp"
#include "Common/Controls/ProgressIndicator.hpp"
#include "Managers/GameSoundManager.h"
#include "CCAppController.hpp"
#include "Managers/LogManager.hpp"

using namespace ui;


namespace BookViewSpace {
    const Size defaultSize = Size(2560, 1800);
    
    const int upperZ = 100;
    const int lowerZ = 50;
    
    const float turnDuration = 0.25;
    
    
}

using namespace BookViewSpace;

bool BookView::_isReadAll = false;
bool BookView::_libraryMode = false;
static std::string _currentBook;
std::string BookView::_languageCode = "en";


BookView* BookView::create(const cocos2d::Size &size, std::string &bookPath)
{
    return BookView::create(size, bookPath, true);
}

BookView* BookView::create(const cocos2d::Size &size, std::string &bookPath, bool checkCompleteCondition)
{
    BookView *pRet = new(std::nothrow) BookView();
    if (pRet && pRet->init(size, bookPath, checkCompleteCondition)) {
        pRet->autorelease();
        return pRet;
    } else {
        delete pRet;
        pRet= nullptr;
        return nullptr;
    }
}

bool BookView::init(const Size &size, std::string &bookPath, bool checkCompleteCondition)
{
    if (!Node::init()) return false;

    GameSoundManager::getInstance()->stopAllEffects();
    GameSoundManager::getInstance()->unloadAllEffect();
    
    _checkCompleteCondition = checkCompleteCondition;
    
    _buttonEnabled = false;
    _soundSetting = true;

    // for memory release
    Director::getInstance()->getTextureCache()->removeUnusedTextures();

    TodoBook *book = new TodoBook;
    if (!book->readFile(bookPath)) return false;
    
    setBook(book);
    
    
    setColor(Color3B::WHITE);
    
    setContentSize(size);
    
    _isReadAll = false;
    _finishReading = false;
    
    _bgView = Sprite::create("Books/book_vertical_bg.jpg");
    
    auto bgSize = _bgView->getContentSize();
    
    auto bgScale = MAX(size.width/bgSize.width, size.height/bgSize.height);
    _bgView->setScale(bgScale);
    _bgView->setPosition(size/2);
    addChild(_bgView);

    /*  moved to below: apply to all book layout
    if (_libraryMode && _book->bookType == TDBookType::WithAudio && (_bookLayout == TDBookLayout::Portrait || _bookLayout == TDBookLayout::Portrait_Traditional)) {
        
        _soundBtn = ui::Button::create("Common/Controls/library_book_button_sound-on.png");
        _soundBtn->setAnchorPoint(Vec2::ANCHOR_TOP_RIGHT);
        _soundBtn->setPosition(Vec2(size.width-25, size.height-25));
        addChild(_soundBtn);
        _soundBtn->addClickEventListener([this](Ref*) {
            this->setSoundSetting(!_soundSetting);
        });
        _soundSetting = getSoundSetting();
        setSoundSetting(_soundSetting);
        
        
        _soundView = Sprite::create("Common/Controls/book_readaloud_base.png");

        Size soundSize = _soundView->getContentSize();

        bool isEnglish = _languageCode == "en";
        
        std::string textAloud = (isEnglish) ? "Read Aloud?" : "Sauti";
        auto labelAloud = Label::createWithTTF(textAloud, "fonts/TodoSchoolV2.ttf", 90);
        labelAloud->setTextColor(Color4B(255, 248, 219, 255));
        labelAloud->setPosition(soundSize.width / 2, soundSize.height / 2 + 255);
        _soundView->addChild(labelAloud);


        _soundOnButton = ImageView::create("Common/Controls/book_readaloud_button_normal.png");
        _soundOnButton->setTouchEnabled(true);
        _soundOnButton->addTouchEventListener([this, isEnglish](Ref*, Widget::TouchEventType type) {
            if (type == Widget::TouchEventType::BEGAN) {
                if (!_soundSetting) {
                    if (isEnglish) {
                        GameSoundManager::getInstance()->playEffectSound("Common/Sounds/Effect/soundon.m4a");
                    } else {
                        GameSoundManager::getInstance()->playEffectSound("Common/Sounds/Effect/washasauti.m4a");
                    }
                }
                setSoundButton(true, _soundOnButton, _soundOnLabel);
                setSoundButton(false, _soundOffButton, _soundOffLabel);
                _soundSetting = true;
                BookView::setSoundSetting(_soundSetting);
            }
        });

        std::string textOn = (isEnglish) ? "on" : "Washa";
        float textSizeOnOff = (isEnglish) ? 120 : 95;
        _soundOnLabel = Label::createWithTTF(textOn, "fonts/TodoSchoolV2.ttf", textSizeOnOff);
        _soundOnLabel->setTextColor(Color4B(255, 255, 255, 255));
        _soundOnLabel->setPosition(Vec2(_soundOnButton->getContentSize().width / 2, 156));
        _soundOnButton->addChild(_soundOnLabel);

        _soundOnButton->setAnchorPoint(Vec2(0.5f, 0.0f));
        _soundOnButton->setPosition(Vec2(soundSize.width / 2, soundSize.height / 2 - 90));
        _soundView->addChild(_soundOnButton);

        _soundOffButton = ImageView::create("Common/Controls/book_readaloud_button_normal.png");
        _soundOffButton->setTouchEnabled(true);
        _soundOffButton->addTouchEventListener([this, isEnglish](Ref*, Widget::TouchEventType type) {
            if (type == Widget::TouchEventType::BEGAN) {
                bool beforeSoundSetting = _soundSetting;

                setSoundButton(false, _soundOnButton, _soundOnLabel);
                setSoundButton(true, _soundOffButton, _soundOffLabel);
                _soundSetting = false;
                BookView::setSoundSetting(_soundSetting);
                if (_currentPageView != nullptr) {
                    _currentPageView->stopReading();
                }
                
                if (beforeSoundSetting) {
                    if (isEnglish) {
                        GameSoundManager::getInstance()->playEffectSound("Common/Sounds/Effect/soundoff.m4a");
                    } else {
                        GameSoundManager::getInstance()->playEffectSound("Common/Sounds/Effect/zimasauti.m4a");
                    }
                }
            }
        });

        std::string textOff = (isEnglish) ? "off" : "Zima";
        _soundOffLabel = Label::createWithTTF(textOff, "fonts/TodoSchoolV2.ttf", textSizeOnOff);
        _soundOffLabel->setTextColor(Color4B(255, 255, 255, 255));
        _soundOffLabel->setPosition(Vec2(_soundOffButton->getContentSize().width / 2, 156));
        _soundOffButton->addChild(_soundOffLabel);

        _soundOffButton->setAnchorPoint(Vec2(0.5f, 0.0f));
        _soundOffButton->setPosition(Vec2(soundSize.width / 2, soundSize.height / 2 - 340));
        _soundView->addChild(_soundOffButton);

        _soundView->setPosition(Vec2(size.width / 4, size.height / 2));
        addChild(_soundView);

        _soundSetting = BookView::getSoundSetting();
        setSoundButton(_soundSetting, _soundOnButton, _soundOnLabel);
        setSoundButton(!_soundSetting, _soundOffButton, _soundOffLabel);
         
     
        
    }
 */
    
    _contentsView = Node::create();
    _contentsView->setContentSize(size);
    addChild(_contentsView);

    

    
    _pageScale = MIN(size.width/defaultSize.width, size.height/defaultSize.height);

    
    
    //auto imageScale = MIN(size.width/2560, size.height/1800.0);
    //auto imageScale = 1.f;
    //Size imageSize = Size(imageScale*2280, imageScale*1410);
    //Size textSize = Size(imageScale*2280, imageScale*200);
    //float topMargin = (size.height-imageSize.height)/2-100*imageScale;
    
    //auto backButton = TodoSchoolBackButton::create();
    //backButton->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
    //backButton->setPosition(Vec2(25, size.height-25));
    //addChild(backButton);
    
    {
        auto backBtn = ui::Button::create();
        backBtn->loadTextures("Common/Controls/back_arrow_inactive.png", "Common/Controls/back_arrow_active.png");
        
        auto keyListener = EventListenerKeyboard::create();
        keyListener->onKeyReleased = [this](EventKeyboard::KeyCode keyCode, Event *event) {
            if (keyCode == EventKeyboard::KeyCode::KEY_ESCAPE) {
                LogManager::getInstance()->logEvent(_book->bookTitle, "esc_pressed", "", _currentPage);
                this->popBookScene();
            }
        };
        backBtn->getEventDispatcher()->addEventListenerWithSceneGraphPriority(keyListener, backBtn);
        backBtn->addClickEventListener([this](Ref*){
            LogManager::getInstance()->logEvent(_book->bookTitle, "back_pressed", "", _currentPage);
            GameSoundManager::getInstance()->playEffectSound("Common/Sounds/Effect/SFX_GUIBack.m4a");
            this->popBookScene();
        });
        
        backBtn->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
        backBtn->setPosition(Vec2(25, size.height-25));
        addChild(backBtn);
    }
    
    
    {
        if (_libraryMode && _book->bookType == TDBookType::WithAudio) {
            // change - audio button for all book layout
            
            _soundBtn = ui::Button::create("Common/Controls/library_book_button_sound-on.png");
            _soundBtn->setAnchorPoint(Vec2::ANCHOR_TOP_RIGHT);
            _soundBtn->setPosition(Vec2(size.width-25, size.height-25));
            addChild(_soundBtn);
            _soundBtn->addClickEventListener([this](Ref*) {
                this->setSoundSetting(!_soundSetting);
                if (_soundSetting) {
                    _currentPageView->startReading();
                } else {
                    _currentPageView->stopReading();
                }
            });
            _soundSetting = getSoundSetting();
            setSoundSetting(_soundSetting);
        }
    }
    _progressBar = ProgressIndicator::create();
    _progressBar->setPosition(Vec2(size.width/2, size.height - _progressBar->getContentSize().height));
    addChild(_progressBar);
    _progressBar->setMax(_book->pages.size());
    
    
    if (true) {
        _prevButton = Button::create("Books/book_arrow_left_normal.png", "Books/book_arrow_left_active.png");
        _nextButton = Button::create("Books/book_arrow_right_normal.png", "Books/book_arrow_right_active.png");
        
        Director::getInstance()->getTextureCache()->addImage("Books/book_vertical_pagespread_texture.png");

        
        
    } else {
        _prevButton = Button::create("Books/page_left.png");
        _nextButton = Button::create("Books/page_right.png");
    }

    _prevButtonPos = Vec2(_prevButton->getContentSize().width*0.5, size.height/2);
    _prevButton->setPosition(_prevButtonPos);
    _prevButton->addClickEventListener([this](Ref*){
        previousPage();
    });
    addChild(_prevButton);
    
    
    _nextButtonPos = Vec2(size.width-_nextButton->getContentSize().width*0.5, size.height/2);
    _nextButton->setPosition(_nextButtonPos);
    _nextButton->addClickEventListener([this](Ref*){
        nextPage();
    });
    addChild(_nextButton);

    Director::getInstance()->getTextureCache()->addImage("Common/Controls/CompletePopup/game_effect_glow.png");
    Director::getInstance()->getTextureCache()->addImage("Common/Controls/CompletePopup/game_effect_rotatingleft.png");
    Director::getInstance()->getTextureCache()->addImage("Common/Controls/CompletePopup/game_effect_rotatingright.png");
    Director::getInstance()->getTextureCache()->addImage("Common/Controls/CompletePopup/game_effect_sparkle_1.png");
    Director::getInstance()->getTextureCache()->addImage("Common/Controls/CompletePopup/game_effect_starmedal.png");
    Director::getInstance()->getTextureCache()->addImage("Common/Controls/CompletePopup/game_effect_done_touch.png");
    Director::getInstance()->getTextureCache()->addImage("Common/Controls/CompletePopup/game_effect_done_normal.png");
    
    _currentPage = -1;
    _currentPageView = nullptr;
    
    showPageButton();
    
    return true;
    
}

void BookView::onEnter()
{
    Node::onEnter();
    LogManager::getInstance()->tagScreen(_book->bookTitle);

}

void BookView::onExit()
{
    Node::onExit();
    GameSoundManager::getInstance()->stopAllEffects();
    GameSoundManager::getInstance()->unloadAllEffect();
    

    
}

void BookView::setBook(TodoBook *book)
{
    _book = book;
    _bookLayout = book->bookLayout;
    //_isPortrait = (_bookLayout==TDBookLayout::Portrait || _bookLayout==TDBookLayout::Portrait_Traditional);
    
    
    
}
void BookView::viewTitle(float delay)
{
    _currentPage = -1;
    
    unscheduleUpdate();
    
    _progressBar->setVisible(false);
    
    

    if (_currentPageView) {
        _currentPageView->removeFromParent();
        _currentPageView = nullptr;
    }
    
    _currentPageView = BookPage::create();
    //_currentPageView->setTitle(_book->bookTitle, _book->imagePrefix+_book->titleImageFilename, _soundSetting ? _book->imagePrefix+_book->titleAudioFilename : "", _bookLayout, delay);
    _currentPageView->setTitle(_book->bookTitle, _book->imagePrefix+_book->titleImageFilename, _book->imagePrefix+_book->titleAudioFilename, _bookLayout, delay);
    
    _currentPageView->setScale(_pageScale);
    _currentPageView->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    _currentPageView->setPosition(getContentSize()/2);
    _contentsView->addChild(_currentPageView);

    //showSoundButton();
}


void BookView::viewPage(int page)
{
    if (page<0) return;
    if (page>=_book->pages.size()) return;
    if (page==_currentPage) return;
    

    if (page == _book->pages.size()-1) {
        _finishReading = true;
    }
    
    _currentPage = page;
  
    
    _progressBar->setVisible(true);
    _progressBar->setCurrent(page+1);
    
    



    
    if (_currentPageView) {
        _currentPageView->removeFromParent();
        _currentPageView = nullptr;
    }
    
    _currentPageView = BookPage::create();
    //_currentPageView->setPage(_book, &(_book->pages[_currentPage]), _bookLayout, _book->bookType == TDBookType::WithAudio && _soundSetting);
    _currentPageView->setPage(_book, &(_book->pages[_currentPage]), _bookLayout, _book->bookType == TDBookType::WithAudio);
    
    _currentPageView->setScale(_pageScale);
    _currentPageView->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    _currentPageView->setPosition(getContentSize()/2);
    
    _contentsView->addChild(_currentPageView);

}

void BookView::nextPage()
{
    
    if (!_buttonEnabled) return;

    hidePageButton();
    
    if (true) {
    
        auto oldPage = _currentPageView;
        oldPage->stopReading();
        _currentPageView = nullptr;
        


        viewPage(_currentPage+1);
        
        _contentsView->reorderChild(oldPage, upperZ);
        _contentsView->reorderChild(_currentPageView, lowerZ);

        _currentPageView->hideLeftHalf(false);
        
        float nextDuration = (_currentPage + 1 >= _book->pages.size()) ? turnDuration * 2 : turnDuration;
        oldPage->runAction(Sequence::create(CallFunc::create([oldPage](){ oldPage->hideRightHalf(true); }),
                                            DelayTime::create(turnDuration),
                                            CallFunc::create([this, oldPage](){
                                                _contentsView->reorderChild(oldPage, lowerZ);
                                                _contentsView->reorderChild(_currentPageView, upperZ);
                                                _currentPageView->showLeftHalf(true);
  
                                            }),
                                            DelayTime::create(nextDuration),
                                            CallFunc::create([this](){
                                                if (_currentPage + 1 >= _book->pages.size()) {
                                                    LogManager::getInstance()->logEvent(
                                                            _book->bookTitle, "finish_read", "",
                                                            _currentPage);
                                                    CompletePopup::create()->show(0.0, [this]() {
                                                        _isReadAll = true;
                                                        
                                                        if (_checkCompleteCondition)
                                                        {
                                                            CCAppController::sharedAppController()->handleGameComplete(1);
                                                        }
                                                        else
                                                        {
                                                            CCAppController::sharedAppController()->handleGameQuit();
                                                        }
                                                        
                                                        //TodoSchoolBackButton::popGameScene();
                                                    });
                                                } else {
                                                    showPageButton();
                                                    if (getSoundSetting())  _currentPageView->startReading();
                                                }

                                                //hideSoundButton();
                                            }),
                                            DelayTime::create(turnDuration),
                                            CallFunc::create([oldPage](){
                                                oldPage->removeFromParent();
                                            }),
                                            nullptr));
        
        
        

    } else {


        
        float w = getContentSize().width;
        
        _contentsView->runAction(Sequence::create(
                                                  MoveBy::create(0.15, Vec2(-w, 0)),
                                                  CallFunc::create([this, w](){
            _contentsView->setPositionX(w);
            viewPage(_currentPage+1);
            showPageButton();
            if (getSoundSetting()) _currentPageView->startReading();
        }),
                                                  
                                                  MoveBy::create(0.15, Vec2(-w, 0)),
                                                  nullptr));
    }
    
}

void BookView::previousPage()
{
    

    if (!_buttonEnabled) return;
    
    hidePageButton();
    
    if (true) {
        
        auto oldPage = _currentPageView;
        oldPage->stopReading();
        _currentPageView = nullptr;
        
        if (_currentPage==0) {
            viewTitle(turnDuration*2);
        } else if (_currentPage>0) {
            viewPage(_currentPage-1);

        }
        
        _contentsView->reorderChild(oldPage, upperZ);
        _contentsView->reorderChild(_currentPageView, lowerZ);
        
        _currentPageView->hideRightHalf(false);


        
        oldPage->runAction(Sequence::create(CallFunc::create([oldPage](){ oldPage->hideLeftHalf(true); }),
                                            DelayTime::create(turnDuration),
                                            CallFunc::create([this, oldPage](){
                                                _contentsView->reorderChild(oldPage, lowerZ);
                                                _contentsView->reorderChild(_currentPageView, upperZ);
                                                _currentPageView->showRightHalf(true);
                                            }),
                                            DelayTime::create(turnDuration),
                                            CallFunc::create([this](){
                                                showPageButton();
                                                if (getSoundSetting())  _currentPageView->startReading();
                                            }),
                                            DelayTime::create(turnDuration),
                                            CallFunc::create([oldPage](){
                                                oldPage->removeFromParent();
                                            }),

                                            nullptr));
        
        
        
        
        
        
    } else {
        
        float w = getContentSize().width;
        
        

        
        
        
        _contentsView->runAction(Sequence::create(
                                                  MoveBy::create(0.15, Vec2(w, 0)),
                                                  CallFunc::create([this, w](){
            _contentsView->setPositionX(-w);
            
            if (_currentPage==0) {
                viewTitle();
            } else if (_currentPage>0) {
                viewPage(_currentPage-1);
            }
            showPageButton();
            if (getSoundSetting()) _currentPageView->startReading();
        }),
                                                  MoveBy::create(0.15, Vec2(w, 0)),
                                                  nullptr));
    }
    
}

void BookView::showPageButton()
{
    
    _buttonEnabled = true;
    
    _prevButton->setVisible(_currentPage>=0);
    
    _prevButton->setPositionX(-_prevButton->getContentSize().width/2);
    _prevButton->runAction(Sequence::create(DelayTime::create(1.0),
                                            EaseBounceOut::create(MoveTo::create(0.2, _prevButtonPos)), nullptr));
    
    
    _nextButton->setVisible(true);
    _nextButton->setPositionX(getContentSize().width+_nextButton->getContentSize().width/2);
    _nextButton->runAction(Sequence::create(DelayTime::create(1.0),
                                            EaseBounceOut::create(MoveTo::create(0.2, _nextButtonPos)), nullptr));
}

void BookView::hidePageButton()
{
    _buttonEnabled = false;
    
    _prevButton->setPosition(_prevButtonPos);
    _prevButton->runAction(MoveBy::create(0.1, Vec2(-_prevButton->getContentSize().width, 0)));
    
    _nextButton->setPosition(_nextButtonPos);
    _nextButton->runAction(MoveBy::create(0.1, Vec2(_nextButton->getContentSize().width, 0)));
}

void BookView::popBookScene()
{
    if (_libraryMode) {
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
        if (_finishReading) {
            LogManager::getInstance()->logEvent(_book->bookTitle, "finish_read", "", _currentPage);
            CCAppController::sharedAppController()->handleGameComplete(1);

        } else {
            JniMethodInfo t;

            bool result = JniHelper::getStaticMethodInfo(t, "org/cocos2dx/cpp/AppActivity", "sendToBack", "()V");
            if (result)
            {
                t.env->CallStaticVoidMethod(t.classID, t.methodID);
                t.env->DeleteLocalRef(t.classID);
            }
        }
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_WINRT)
	{
		auto uri = ref new Windows::Foundation::Uri("kitkitlibrary:");
		concurrency::create_task(Windows::System::Launcher::LaunchUriAsync(uri)).then([](bool launchResult)
		{

		});
	}


#else
        Director::getInstance()->end();
#endif
        
    } else {
        if (_finishReading) {
            LogManager::getInstance()->logEvent(_book->bookTitle, "finish_read", "", _currentPage);
            CCAppController::sharedAppController()->handleGameComplete(1);

        } else {
            //(Director::getInstance())->popScene();
            CCAppController::sharedAppController()->handleGameQuit();
        }
    }

}

//void BookView::setSoundButton(bool isSelect, ui::ImageView *imageButton, Label *textLabel) {
//    Size size = imageButton->getContentSize();
//
//    if (isSelect) {
//        imageButton->loadTexture("Common/Controls/book_readaloud_button_active.png");
//        textLabel->setTextColor(Color4B(255, 255, 255, 255));
//        textLabel->setPosition(size.width / 2, 156 - 15);
//
//    } else {
//        imageButton->loadTexture("Common/Controls/book_readaloud_button_normal.png");
//        textLabel->setTextColor(Color4B(87, 44, 0, 255));
//        textLabel->setPosition(size.width / 2, 156);
//
//    }
//}

//void BookView::showSoundButton() {
//    if (_soundView != nullptr) {
//        _soundView->setVisible(true);
//    }
//}
//
//void BookView::hideSoundButton() {
//    if (_soundView != nullptr) {
//        _soundView->setVisible(false);
//    }
//}

string BookView::getCurrentBook() {
    return _currentBook;
}

void BookView::setCurrentBook(string book) {
    _currentBook = book;
}

bool BookView::getSoundSetting() {
    
    bool result = UserDefault::getInstance()->getBoolForKey("SOUND_ENABLE_BOOK_GLOBAL", true);
    return result;
    
    /*
    bool result = true;

    string soundOffBook = UserDefault::getInstance()->getStringForKey("SOUND_OFF_BOOK", "");
    vector<std::string> splitSoundOffBook = TodoUtil::split(soundOffBook, ',');
    if (splitSoundOffBook.size() == 0) {
        return true;
    }

    for (string soundOffBook : splitSoundOffBook) {
        if (soundOffBook == getCurrentBook()) {
            return false;
        }
    }

    return result;
     */
    
}

void BookView::setSoundSetting(bool enable) {
    _soundSetting = enable;
    if (enable) {
        _soundBtn->loadTextureNormal("Common/Controls/library_book_button_sound-on.png");
    } else {
        _soundBtn->loadTextureNormal("Common/Controls/library_book_button_sound-off.png");
    }
    
    UserDefault::getInstance()->setBoolForKey("SOUND_ENABLE_BOOK_GLOBAL", enable);
    
    /*
    string soundOffBook = UserDefault::getInstance()->getStringForKey("SOUND_OFF_BOOK", "");
    vector<std::string> splitSoundOffBook = TodoUtil::split(soundOffBook, ',');
    splitSoundOffBook.erase(std::remove(splitSoundOffBook.begin(), splitSoundOffBook.end(), getCurrentBook()), splitSoundOffBook.end());

    if (enable == false) {
        splitSoundOffBook.push_back(getCurrentBook());
    }

    string strSave = "";
    for (int i = 0; i < splitSoundOffBook.size(); ++i) {
        strSave += splitSoundOffBook[i];
        if (i < splitSoundOffBook.size() - 1) {
            strSave += ",";
        }
    }

    UserDefault::getInstance()->setStringForKey("SOUND_OFF_BOOK", strSave);
     */
    
    
    
    
}
