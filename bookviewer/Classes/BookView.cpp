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

bool BookView::_libraryMode = false;

BookView* BookView::create(const cocos2d::Size &size, std::string &bookPath)
{
    BookView *pRet = new(std::nothrow) BookView();
    if (pRet && pRet->init(size, bookPath)) {
        pRet->autorelease();
        return pRet;
    } else {
        delete pRet;
        pRet= nullptr;
        return nullptr;
    }
}


bool BookView::init(const Size &size, std::string &bookPath)
{
    if (!Node::init()) return false;
    
    
    GameSoundManager::getInstance()->stopAllEffects();
    GameSoundManager::getInstance()->unloadAllEffect();
    
    _buttonEnabled = false;
    
    // for memory release
    Director::getInstance()->getTextureCache()->removeUnusedTextures();

    TodoBook *book = new TodoBook;
    if (!book->readFile(bookPath)) return false;
    
    setBook(book);
    
    
    setColor(Color3B::WHITE);
    
    setContentSize(size);
    
    
    _finishReading = false;
    
    _bgView = Sprite::create("Books/book_vertical_bg.jpg");
    
    auto bgSize = _bgView->getContentSize();
    
    auto bgScale = MAX(size.width/bgSize.width, size.height/bgSize.height);
    _bgView->setScale(bgScale);
    _bgView->setPosition(size/2);
    addChild(_bgView);
    

   
    
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
    _currentPageView->setTitle(_book->bookTitle, _book->imagePrefix+_book->titleImageFilename, _book->imagePrefix+_book->titleAudioFilename, _bookLayout, delay);
    _currentPageView->setScale(_pageScale);
    _currentPageView->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    _currentPageView->setPosition(getContentSize()/2);
    _contentsView->addChild(_currentPageView);
    
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
    _currentPageView->setPage(&(_book->pages[_currentPage]), _book->filePrefix, _bookLayout, _book->bookType == TDBookType::WithAudio);
    
    _currentPageView->setScale(_pageScale);
    _currentPageView->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    _currentPageView->setPosition(getContentSize()/2);
    _contentsView->addChild(_currentPageView);
    
    
    
}

void BookView::nextPage()
{
    
    if (!_buttonEnabled) return;
    
    if (_currentPage+1>=_book->pages.size()) {
        LogManager::getInstance()->logEvent(_book->bookTitle, "finish_read", "", _currentPage);
        CompletePopup::create()->show(0.0, [](){
            CCAppController::sharedAppController()->handleGameComplete(1);
            //TodoSchoolBackButton::popGameScene();
        });
        
        return;
        
    }
    
    hidePageButton();
    
    if (true) {
    
        auto oldPage = _currentPageView;
        oldPage->stopReading();
        _currentPageView = nullptr;
        


        viewPage(_currentPage+1);
        
        _contentsView->reorderChild(oldPage, upperZ);
        _contentsView->reorderChild(_currentPageView, lowerZ);

        _currentPageView->hideLeftHalf(false);
        
        
        
        oldPage->runAction(Sequence::create(CallFunc::create([oldPage](){ oldPage->hideRightHalf(true); }),
                                            DelayTime::create(turnDuration),
                                            CallFunc::create([this, oldPage](){
                                                _contentsView->reorderChild(oldPage, lowerZ);
                                                _contentsView->reorderChild(_currentPageView, upperZ);
                                                _currentPageView->showLeftHalf(true);
  
                                            }),
                                            DelayTime::create(turnDuration),
                                            CallFunc::create([this](){
                                                showPageButton();
                                                _currentPageView->startReading();
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
            _currentPageView->startReading();
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
                                                _currentPageView->startReading();
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
            _currentPageView->startReading();
            
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
        JniMethodInfo t;
        
        bool result = JniHelper::getStaticMethodInfo(t, "org/cocos2dx/cpp/AppActivity", "sendToBack", "()V");
        if (result)
        {
            t.env->CallStaticVoidMethod(t.classID, t.methodID);
            t.env->DeleteLocalRef(t.classID);
        }
        
#else
        Director::getInstance()->end();
#endif
        
    } else {
        if (_finishReading) {
            LogManager::getInstance()->logEvent(_book->bookTitle, "finish_read", "", _currentPage);
            CompletePopup::create()->show(0.0, [](){
                CCAppController::sharedAppController()->handleGameComplete(1);
                //TodoSchoolBackButton::popGameScene();
            });
        } else {
            (Director::getInstance())->popScene();
        }
    }

}
