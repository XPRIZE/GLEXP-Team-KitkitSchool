//
//  BookPage.cpp
//  booktest
//
//  Created by Gunho Lee on 12/27/16.
//
//

#include "BookPage.hpp"

#include "Managers/GameSoundManager.h"
#include "Utils/TodoUtil.h"



using namespace ui;


namespace BookPageSpace {
    const auto textColor = Color3B(79, 61, 24);
    const Size defaultSize = Size(2560, 1800);
    const Size halfSize = Size(1280, 1800);

    const Size bigSize = Size(2480, 1800);
    const Size smallSize = Size(80, 1800);
    
    const Size squareSize = Size(1520, 1520);
    const Size squareImageSize = Size(909, 909);
    

    const float squareMiddle = defaultSize.width/2 - squareSize.width/2;
    
    const float skewY = 10;
    const float turnDuration = 0.25;
    const float turnEase = 1.4;
    
    const string titleFont = "fonts/Seshat.otf";
    const string bodyFont = "fonts/Andika-R.ttf";
    
    
    
    
}

using namespace BookPageSpace;


BookPage* BookPage::create()
{
    BookPage *pRet = new(std::nothrow) BookPage();
    if (pRet && pRet->init()) {
        pRet->autorelease();
        return pRet;
    } else {
        delete pRet;
        pRet= nullptr;
        return nullptr;
    }
}


bool BookPage::init()
{
    if (!Node::init()) return false;
    
    setColor(Color3B::WHITE);
    
    setContentSize(defaultSize);

    _contentsView = Node::create();
    _contentsView->setContentSize(defaultSize);
    addChild(_contentsView);
    
    
    return true;
    
}

void BookPage::onExit()
{
    Node::onExit();

    
}




void BookPage::update(float delta)
{
    if (_pauseReading) {
        
        _pauseLength -= delta;
        if (_pauseLength<0) {
            _pauseLength = 0;
            _pauseReading = false;
            
            //GameSoundManager::getInstance()->resumeEffect(_readingAudioID);
            GameSoundManager::getInstance()->resumeBGM();
            
        } else {
            return;
        }
    } else {
        _timePage+=delta;
        _timeSentence+=delta;
        
        auto page = _page;
        
        int newReadingSentenceIndex = _readingSentenceIndex;
        TodoSentence newReadingSentence;
        
        int i=-1;
        
        for (auto p : page->paragraphs) {
            for (auto s : p.sentences) {
                i++;
                if (s.startTimingInPage <= _timePage) {
                    newReadingSentenceIndex = i;
                    newReadingSentence = s;
                }
            }
        }
        
        /*
        for (int i=_readingSentence+1; i<page->paragraphs[0].sentences.size(); i++) {
            auto sentence= page->paragraphs[0].sentences[i];
            if (sentence.startTimingInPage<=_timePage) {
                
                newReadingSentence = i;
            }
        }
         */
        
        
        if (newReadingSentenceIndex!=_readingSentenceIndex) {
            _readingSentenceIndex = newReadingSentenceIndex;
            //auto sentence= page->paragraphs[0].sentences[_readingSentenceIndex];
            
            GameSoundManager::getInstance()->stopAllEffects();
            
            //auto audioPath = _book->filePrefix+_book->pagePrefix+sentence.sentenceAudioFilename;
            auto audioPath = _book->filePrefix + "page/"+newReadingSentence.sentenceAudioFilename;
            //_readingAudioID = GameSoundManager::getInstance()->playEffectSound(audioPath);
            GameSoundManager::getInstance()->playBGM(audioPath);
            
            _timeSentence = 0.0;
            
        }
        
    }
    
    
    for (auto b : _wordButtons) {
        auto wordObj = _words[b->getTag()];
        if (wordObj.startTimingInPage<=_timePage && _timePage<=wordObj.endTimingInPage) {
            b->resetNormalRender();
            b->loadTextureNormal("Common/lightblue.png");
            b->setTitleColor(Color3B::BLACK);
        } else {
            b->resetNormalRender();
            b->loadTextureNormal("Common/transparent.png");
            b->setTitleColor(textColor);
        }
    }
}


void BookPage::startReading()
{
    if (_withAudio) scheduleUpdate();
}

void BookPage::stopReading()
{
    if (_withAudio) unscheduleUpdate();
    GameSoundManager::getInstance()->stopAllEffects();
    GameSoundManager::getInstance()->stopBGM();
    
}

void BookPage::setTitle(string title, string titleImagePath, string audioPath, TDBookLayout layout, float delay)
{
    Size size = getContentSize();
    
    _isTitle = true;
    
    unscheduleUpdate();
    
    
    //_isPortrait = layout==TDBookLayout::Portrait || layout==TDBookLayout::Portrait_Traditional;
    _hasBackCover = layout==TDBookLayout::Portrait || layout==TDBookLayout::Portrait_Traditional;
    
    if (layout==TDBookLayout::Portrait || layout==TDBookLayout::Portrait_Traditional) {
        
        
        
        _leftView = Node::create();
        _leftView->setContentSize(halfSize);
        _contentsView->addChild(_leftView);
        
        
        
        _leftCover = Node::create();
        _leftCover->setContentSize(halfSize);
        
        Sprite *sp;
        if (layout==TDBookLayout::Portrait_Traditional) {
            sp = Sprite::create("Books/portrait_traditional/koreanbook_cover.jpg", Rect(0, 0, 20, 1800));
        } else {
            sp = Sprite::create("Books/portrait/xprize_vertical_layout_cover.jpg", Rect(0, 0, 20, 1800));
        }
        sp->setAnchorPoint(Vec2::ANCHOR_BOTTOM_RIGHT);
        sp->setPosition(Vec2(halfSize.width, 0));
        _leftCover->addChild(sp);
        _leftView->addChild(_leftCover);
        
        
        _rightView = Node::create();
        _rightView->setContentSize(halfSize);
        _rightView->setPosition(Vec2(halfSize.width, 0));
        _contentsView->addChild(_rightView);
        
        if (layout==TDBookLayout::Portrait_Traditional) {
            _rightCover = Sprite::create("Books/portrait_traditional/koreanbook_cover.jpg", Rect(20, 0, 1280, 1800));
        } else {
            _rightCover = Sprite::create("Books/portrait/xprize_vertical_layout_cover.jpg", Rect(20, 0, 1280, 1800));
        }
        _rightCover->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
        _rightCover->setPosition(Vec2(0, 0));
        _rightView->addChild(_rightCover);
        
    
        Vec2 imagePos = Vec2(halfSize.width-610, 720);
        
        Sprite *cover;
        if (layout==TDBookLayout::Portrait_Traditional) {
            cover = Sprite::create("Books/portrait_traditional/koreanbook_cover_illustration_blank.png");
        } else {
            cover = Sprite::create("Books/portrait/book_vertical_cover_illustration.png");
        }
        
        cover->setPosition(imagePos);
        _rightView->addChild(cover);
        
        _imageView = Node::create();
        _imageView->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        _imageView->setContentSize(Size(1020, 1170));
        _imageView->setPosition(imagePos);
        _rightView->addChild(_imageView);
        
        if (layout==TDBookLayout::Portrait) {
            auto texture = Sprite::create("Books/portrait/book_vertical_cover_illustration_texture.png");
            texture->setScale(2.0);
            texture->setPosition(imagePos);
            _rightView->addChild(texture);
        }
        
        Sprite *frame;
        if (layout==TDBookLayout::Portrait_Traditional) {
            frame = Sprite::create("Books/portrait_traditional/koreanbook_cover_illustration_frame.png");
        } else {
            frame = Sprite::create("Books/portrait/xprize_vertical_cover_illustration_frame.png");
        }

        frame->setPosition(imagePos);
        _rightView->addChild(frame);
        
        
        auto textViewSize = Size(1038, 360);
        
        _textView = Node::create();
        _textView->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        _textView->setContentSize(textViewSize);
        _textView->setPosition(Vec2(halfSize.width-610, 1800-250));
        _rightView->addChild(_textView);
        
        float fontSize = 150;
        
        auto labelFunc = [&](Color4B color, string fontName, Size offset){
            
            auto l = Label::createWithTTF(title, fontName, fontSize, Size(textViewSize.width, 0), TextHAlignment::CENTER);
            auto lSize = l->getContentSize();
            if (lSize.height > textViewSize.height) {
                l->setScale(textViewSize.height / lSize.height);
            }
            l->setPosition(textViewSize/2 + offset);
            l->setTextColor(color);
            return l;
        };
        
        
        if (layout==TDBookLayout::Portrait_Traditional) {
            _textView->addChild(labelFunc(Color4B(77, 35, 20, 255),bodyFont, Size(0, 0)));
        } else {
            _textView->addChild(labelFunc(Color4B(148, 94, 48, 255), titleFont, Size(5, -5)));
            _textView->addChild(labelFunc(Color4B(255, 250, 227, 255), titleFont, Size(0, 0)));
        }
        
        
    } else if (layout==TDBookLayout::Landscape) {
        
        _leftView = Node::create();
        _leftView->setContentSize(smallSize);
        _contentsView->addChild(_leftView);
        
        _leftCover = Node::create();
        _leftCover->setContentSize(smallSize);
        _leftView->addChild(_leftCover);
        
        
        auto sp = Sprite::create("Books/book_horizontal_bg.jpg", Rect(0, 0, 80/2, 1800/2));
        sp->setScale(2.0);
        sp->setAnchorPoint(Vec2::ANCHOR_BOTTOM_RIGHT);
        sp->setPosition(Vec2(smallSize.width, 0));
        _leftCover->addChild(sp);

        
        auto bindingLeft = Sprite::create("Books/book_horizontal_cover_binding.png", Rect(0, 0, 85, 1800));
        bindingLeft->setAnchorPoint(Vec2::ANCHOR_BOTTOM_RIGHT);
        bindingLeft->setPosition(Vec2(smallSize.width, 0));
        _leftCover->addChild(bindingLeft);
        
        
        
        
        _rightView = Node::create();
        _rightView->setContentSize(bigSize);
        _rightView->setPosition(Vec2(smallSize.width, 0));
        _contentsView->addChild(_rightView);
        
        _rightCover = Sprite::create("Books/book_horizontal_bg.jpg", Rect(80/2, 0, (2560-80)/2, 1800/2));
        _rightCover->setScale(2);
        _rightCover->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
        _rightCover->setPosition(Vec2(0, 0));
        _rightView->addChild(_rightCover);
        
        auto bindingRight = Sprite::create("Books/book_horizontal_cover_binding.png", Rect(85, 0, 150-85, 1800));
        bindingRight->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
        bindingRight->setPosition(Vec2(0, 0));
        _rightCover->addChild(bindingRight);
        
        
        Vec2 imagePos = Vec2(bigSize.width/2, 750);
        
        auto cover = Sprite::create("Books/xprize_horizontal_cover_illustration.png");
        cover->setPosition(imagePos);
        _rightView->addChild(cover);
        
        _imageView = Node::create();
        _imageView->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        _imageView->setContentSize(Size(1560, 1090));
        _imageView->setPosition(imagePos);
        _rightView->addChild(_imageView);
        
        
        
        auto texture = Sprite::create("Books/xprize_horizontal_cover_illustration_texture.png");
        texture->setScale(2.0);
        texture->setPosition(imagePos);
        _rightView->addChild(texture);
        
        auto frame = Sprite::create("Books/xprize_horizontal_cover_frame.png");
        frame->setPosition(imagePos);
        _rightView->addChild(frame);
        
        
        auto textViewSize = Size(1560, 360);
        
        _textView = Node::create();
        _textView->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
        _textView->setContentSize(textViewSize);
        _textView->setPosition(Vec2(bigSize.width/2, 1800-380-100));
        _rightView->addChild(_textView);
        
        float fontSize = 150;
        
        
        
        auto labelFunc = [&](Color4B color, Size offset){
            
            auto l = Label::createWithTTF(title, titleFont, fontSize, Size(textViewSize.width, 0), TextHAlignment::CENTER, TextVAlignment::CENTER);
            auto lSize = l->getContentSize();
            if (lSize.height > textViewSize.height) {
                l->setScale(textViewSize.height / lSize.height);
            }
            l->setPosition(Size(textViewSize.width/2, lSize.height/2) + offset);
            l->setTextColor(color);
            return l;
        };
        
        
        _textView->addChild(labelFunc(Color4B(84, 37, 11, 255), Size(5, -5)));
        _textView->addChild(labelFunc(Color4B(255, 248, 227, 255), Size(0, 0)));
        
        
        
        
        
        
        
    }  else if (layout==TDBookLayout::Square) {
        
        
        _leftView = Node::create();
        _leftView->setContentSize(squareSize);
        _contentsView->addChild(_leftView);
        
        
        
        _leftCover = Node::create();
        _leftCover->setContentSize(squareSize);
        _leftView->addChild(_leftCover);
        
        
        _rightView = Node::create();
        _rightView->setContentSize(squareSize);
        _rightView->setPosition(Vec2(squareMiddle, defaultSize.height/2 - squareSize.height/2));
        _contentsView->addChild(_rightView);
        
        _rightCover = Node::create();
        _rightCover->setContentSize(squareSize);
        
        auto coverBG = Sprite::create("Books/square/_alphabet_book_border.jpg");
        coverBG->setPosition(squareSize/2);
        _rightCover->addChild(coverBG);
        _rightCover->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
        _rightCover->setPosition(Vec2(0, 0));
        _rightView->addChild(_rightCover);
        
        
        Vec2 imagePos = Vec2(squareSize.width/2, squareSize.height/2);
        
        
        _imageView = Node::create();
        _imageView->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        _imageView->setContentSize(squareImageSize);
        _imageView->setPosition(imagePos);
        _rightView->addChild(_imageView);
        
        
        auto rightBinding = Sprite::create("Books/square/book_abc_insidespread_binding.png", Rect(171/2, 0, 171/2, 1520));
        rightBinding->setPosition(Vec2(0, 0));
        rightBinding->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
        _rightView->addChild(rightBinding);
        
//        Sprite *frame;
//        if (layout==TDBookLayout::Portrait_Traditional) {
//            frame = Sprite::create("Books/portrait_traditional/koreanbook_cover_illustration_frame.png");
//        } else {
//            frame = Sprite::create("Books/portrait/xprize_vertical_cover_illustration_frame.png");
//        }
//        
//        frame->setPosition(imagePos);
//        _rightView->addChild(frame);
        
        
        
        
    }
    
    
    
    
    
    
    
    auto image = Sprite::create(titleImagePath);
    if (!image) image = Sprite::create(titleImagePath+".jpg");
    if (!image) image = Sprite::create(titleImagePath+".png");
    if (image) {
        auto imageSize = image->getContentSize();
        auto viewSize = _imageView->getContentSize();
        auto scale = MIN(viewSize.width/imageSize.width, viewSize.height/imageSize.height);
        image->setScale(scale);
        image->setPosition(viewSize/2);
        _imageView->addChild(image);
    }
    
    
    
    
    GameSoundManager::getInstance()->stopAllEffects();
    if (audioPath.length() > 0) {
        auto titleAudioPath = audioPath;

        scheduleOnce([titleAudioPath](float){  // Sound delay
            GameSoundManager::getInstance()->playEffectSound(titleAudioPath);
        }, delay, "titleAudio");

    }
}


void BookPage::setPage(TodoBook *book, TodoPage *page, TDBookLayout layout,  bool withAudio)
{
    _isTitle = false;
    
    _page = page;
    _book = book;
    auto folder = _book->filePrefix;
    _bookLayout = layout;
    _withAudio = withAudio;
    //_isPortrait = layout==TDBookLayout::Portrait || layout==TDBookLayout::Portrait_Traditional;
    _hasBackCover = layout==TDBookLayout::Portrait || layout==TDBookLayout::Portrait_Traditional;
    int pageNum = _page->pageNum;
    if (page->creditPage) {
        setCreditPage();
        return;
        
    }
    Size size = getContentSize();



    
    if (layout==TDBookLayout::Portrait || layout==TDBookLayout::Portrait_Traditional) {
        

        
        _leftView = Node::create();
        _leftView->setContentSize(halfSize);
        _leftView->setAnchorPoint(Vec2::ANCHOR_BOTTOM_RIGHT);
        _leftView->setPosition(Vec2(halfSize.width, 0));
        _contentsView->addChild(_leftView);
        
        if (layout==TDBookLayout::Portrait_Traditional) {
            _leftCover = Sprite::create("Books/portrait_traditional/koreanbook_coverplain.jpg", Rect(0, 0, 1280, 1800));
        } else {
            _leftCover = Sprite::create("Books/portrait/book_vertical_yellowcover_bg.jpg", Rect(0, 0, 1280, 1800));
        }
        _leftCover->setPosition(halfSize/2);
        _leftView->addChild(_leftCover);
        
        auto leftBinding = Sprite::create("Books/book_vertical_yellowcover_bg_binding.png", Rect(0, 0, 472/2, 1800));
        leftBinding->setPosition(Vec2(halfSize.width, 0));
        leftBinding->setAnchorPoint(Vec2::ANCHOR_BOTTOM_RIGHT);
        _leftCover->addChild(leftBinding);
        
        
        auto wp1 = Scale9Sprite::create("Books/book_vertical_pagespread_1.png");
        wp1->setPreferredSize(Size(1280, 1718));
        wp1->setAnchorPoint(Vec2::ANCHOR_BOTTOM_RIGHT);
        wp1->setPosition(Vec2(halfSize.width, 30));
        _leftView->addChild(wp1);
        
        auto sd1 = Scale9Sprite::create("Books/book_vertical_middleshadow_left_1.png");
        sd1->setPreferredSize(Size(85, 1698));
        sd1->setAnchorPoint(Vec2::ANCHOR_BOTTOM_RIGHT);
        sd1->setPosition(Vec2(halfSize.width, 50));
        _leftView->addChild(sd1);
        
        
        
        _rightView = Node::create();
        _rightView->setContentSize(halfSize);
        _rightView->setPosition(Vec2(halfSize.width, 0));
        _contentsView->addChild(_rightView);
        
      
        if (layout==TDBookLayout::Portrait_Traditional) {
            _rightCover = Sprite::create("Books/portrait_traditional/koreanbook_coverplain.jpg", Rect(0, 0, 1280, 1800));
        } else {
            _rightCover = Sprite::create("Books/portrait/book_vertical_yellowcover_bg.jpg", Rect(1280, 0, 1280, 1800));
        }
        
        _rightCover->setPosition(halfSize/2);
        _rightView->addChild(_rightCover);
     
        auto rightBinding = Sprite::create("Books/book_vertical_yellowcover_bg_binding.png", Rect(472/2, 0, 472/2, 1800));
        rightBinding->setPosition(Vec2(0, 0));
        rightBinding->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
        _rightCover->addChild(rightBinding);
        
        
        auto wp2 = Scale9Sprite::create("Books/book_vertical_pagespread_1.png");
        wp2->setPreferredSize(Size(1280, 1718));
        wp2->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
        wp2->setPosition(Vec2(0, 30));
        _rightView->addChild(wp2);
        
        
        if (layout==TDBookLayout::Portrait) {
            auto wp3 = Sprite::create("Books/book_vertical_toppage.png");
            wp3->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
            wp3->setPosition(Vec2(0, 30));
            _rightView->addChild(wp3);
        }
        
        auto sd2 = Scale9Sprite::create("Books/book_vertical_middleshadow_right_1.png");
        sd2->setPreferredSize(Size(85, 1698));
        sd2->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
        sd2->setPosition(Vec2(0, 50));
        _rightView->addChild(sd2);
        
        
        _imageView = Node::create();
        _imageView->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        _imageView->setContentSize(Size(1110, 1500));
        _imageView->setPosition(halfSize/2);
        _leftView->addChild(_imageView);
        
        auto leftTexture = Sprite::create("Books/book_vertical_pagespread_texture.png", Rect(0, 0, 1280/2, 1718/2));
        leftTexture->setScale(2);
        leftTexture->setAnchorPoint(Vec2::ANCHOR_BOTTOM_RIGHT);
        leftTexture->setPosition(wp1->getPosition());
        _leftView->addChild(leftTexture);
        
        setPageNum((pageNum * 2 - 1), _leftView, Vec2(_leftView->getPositionX()/2, halfSize.height - _imageView->getContentSize().height - 150));
        
        _textView = Node::create();
        _textView->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        _textView->setContentSize(Size(1000, 1500));
        _textView->setPosition(halfSize/2);
        _rightView->addChild(_textView);
        
        auto rightTexture = Sprite::create("Books/book_vertical_pagespread_texture.png", Rect(1280/2, 0, 1280/2, 1718/2));
        rightTexture->setScale(2);
        rightTexture->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
        rightTexture->setPosition(wp2->getPosition());
        _rightView->addChild(rightTexture);
        
        setPageNum(( pageNum * 2), _rightView, Vec2(_rightView->getPositionX()/2, halfSize.height - _textView->getContentSize().height - 150));
        
    } else if (layout==TDBookLayout::Landscape) {

        _leftView = Node::create();
        _leftView->setContentSize(bigSize);
        _leftView->setAnchorPoint(Vec2::ANCHOR_BOTTOM_RIGHT);
        _leftView->setPosition(Vec2(smallSize.width, 0));
        _contentsView->addChild(_leftView);
        
        _leftCover = Node::create();
        _leftCover->setContentSize(bigSize);
        _leftView->addChild(_leftCover);
        
        
        auto sp = Sprite::create("Books/book_horizontal_bg.jpg", Rect(80/2, 0, (2560-80)/2, 1800/2));
        sp->setScale(2);
        sp->setAnchorPoint(Vec2::ANCHOR_BOTTOM_RIGHT);
        sp->setPosition(Vec2(bigSize.width, 0));
        _leftCover->addChild(sp);
        

        
        auto leftBinding = Sprite::create("Books/book_horizontal_inside_bookbinding.png", Rect(0, 0, 476/2, 1800));
        
        leftBinding->setPosition(Vec2(bigSize.width, 0));
        leftBinding->setAnchorPoint(Vec2::ANCHOR_BOTTOM_RIGHT);
        _leftCover->addChild(leftBinding);
        
        
        auto wp1 = Scale9Sprite::create("Books/book_horizontal_inside_pagespread_1.png");
        wp1->setPreferredSize(Size(bigSize.width, 1712));
        wp1->setAnchorPoint(Vec2::ANCHOR_BOTTOM_RIGHT);
        wp1->setPosition(Vec2(bigSize.width, 30));
        _leftView->addChild(wp1);
        
        auto sd1 = Scale9Sprite::create("Books/book_vertical_middleshadow_left_1.png");
        sd1->setPreferredSize(Size(85, 1692));
        sd1->setAnchorPoint(Vec2::ANCHOR_BOTTOM_RIGHT);
        sd1->setPosition(Vec2(bigSize.width, 50));
        _leftView->addChild(sd1);
        
        
        
        
        _rightView = Node::create();
        _rightView->setContentSize(bigSize);
        _rightView->setPosition(Vec2(smallSize.width, 0));
        _contentsView->addChild(_rightView);
        
        
        _rightCover = Node::create();
        _rightCover->setContentSize(bigSize);
        _rightView->addChild(_rightCover);
        
        
        auto rsp = Sprite::create("Books/book_horizontal_bg.jpg", Rect(80/2, 0, (2560-80)/2, 1800/2));
        rsp->setScale(2);
        rsp->setPosition(bigSize/2);
        _rightCover->addChild(rsp);
        

        auto rightBinding = Sprite::create("Books/book_horizontal_inside_bookbinding.png", Rect(476/2, 0, 476/2, 1800));
        rightBinding->setPosition(Vec2(0, 0));
        rightBinding->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
        _rightCover->addChild(rightBinding);

        
        
        auto wp2 = Scale9Sprite::create("Books/book_horizontal_inside_pagespread_1.png");
        wp2->setPreferredSize(Size(bigSize.width, 1712));
        wp2->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
        wp2->setPosition(Vec2(0, 30));
        _rightView->addChild(wp2);
        
        
        auto sd2 = Scale9Sprite::create("Books/book_vertical_middleshadow_right_1.png");
        sd2->setPreferredSize(Size(85, 1692));
        sd2->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
        sd2->setPosition(Vec2(0, 50));
        _rightView->addChild(sd2);
        
        
        _imageView = Node::create();
        _imageView->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
        _imageView->setContentSize(Size(1820, 1274));
        _imageView->setPosition(Vec2(bigSize.width/2, 390));
        _rightView->addChild(_imageView);
        
        
        auto leftTexture = Sprite::create("Books/book_horizontal_inside_pagespread_texture.png");
        leftTexture->setScale(2);
        leftTexture->setAnchorPoint(Vec2::ANCHOR_BOTTOM_RIGHT);
        leftTexture->setPosition(wp1->getPosition());
        _leftView->addChild(leftTexture);
        
        
        
        _textView = Node::create();
        _textView->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        _textView->setContentSize(Size(2000, 300));
        _textView->setPosition(Vec2(bigSize.width/2, 280));
        _rightView->addChild(_textView);
        
        
        auto rightTexture = Sprite::create("Books/book_horizontal_inside_pagespread_texture.png");
        rightTexture->setScale(2);
        rightTexture->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
        rightTexture->setPosition(wp2->getPosition());
        _rightView->addChild(rightTexture);
        
        setPageNum(pageNum, _rightView, Vec2(bigSize.width/2, 130));
         
        
    } else if (layout==TDBookLayout::Square) {
        
        
        
        _leftView = Node::create();
        _leftView->setContentSize(squareSize);
        _leftView->setAnchorPoint(Vec2::ANCHOR_BOTTOM_RIGHT);
        _leftView->setPosition(Vec2(squareMiddle, defaultSize.height/2 - squareSize.height/2));
        _contentsView->addChild(_leftView);
        
        _leftCover = Node::create();
        _leftView->addChild(_leftCover);
        
        auto cl = LayerColor::create(Color4B::WHITE, squareSize.width, squareSize.height);
        _leftView->addChild(cl);
        
        auto leftBinding = Sprite::create("Books/square/book_abc_insidespread_binding.png", Rect(0, 0, 171/2, 1520));
        leftBinding->setPosition(Vec2(squareSize.width, 0));
        leftBinding->setAnchorPoint(Vec2::ANCHOR_BOTTOM_RIGHT);
        _leftView->addChild(leftBinding);
        
        
        
        
        _rightView = Node::create();
        _rightView->setContentSize(squareSize);
        _rightView->setPosition(Vec2(squareMiddle, defaultSize.height/2 - squareSize.height/2));
        _contentsView->addChild(_rightView);
        
        _rightCover = Node::create();
        _rightView->addChild(_rightCover);
        

        
        auto cr = LayerColor::create(Color4B::WHITE, squareSize.width, squareSize.height);
        _rightView->addChild(cr);
        
        auto rightBinding = Sprite::create("Books/square/book_abc_insidespread_binding.png", Rect(171/2, 0, 171/2, 1520));
        rightBinding->setPosition(Vec2(0, 0));
        rightBinding->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
        _rightView->addChild(rightBinding);
        
        
        _imageView = Node::create();
        _imageView->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        _imageView->setContentSize(Size(1200, 1000));
        _imageView->setPosition(Vec2(squareSize.width/2, squareSize.height/2 + 120));
        _rightView->addChild(_imageView);
        
        
        _textView = Node::create();
        _textView->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        _textView->setContentSize(Size(1200, 300));
        _textView->setPosition(Vec2(squareSize.width/2, 240));
        _rightView->addChild(_textView);
        
        setPageNum(pageNum, _rightView, Vec2(squareSize.width/2, 120));
        

        
    }
    
    
    _timePage = -0.5;
    _timeSentence = -0.5;

    
    //auto image = Sprite::create(_book->imagePrefix+pageObj.pageImageFilename);
    auto image = Sprite::create(folder+"page/"+page->pageImageFilename);
    if (!image) image = Sprite::create(folder+"page/"+page->pageImageFilename+".jpg");
    if (!image) image = Sprite::create(folder+"page/"+page->pageImageFilename+".png");
    if (image) {
        auto imageSize = image->getContentSize();
        auto viewSize = _imageView->getContentSize();
        auto scale = MIN(viewSize.width/imageSize.width, viewSize.height/imageSize.height);
        image->setScale(scale);
        image->setPosition(viewSize/2);
        _imageView->addChild(image);
    }
    
    GameSoundManager::getInstance()->stopAllEffects();
    
    _readingSentenceIndex = -1;
    
    _wordButtons.clear();
    
    auto textViewSize = _textView->getContentSize();
    
    
    _words.clear();
    for (auto p : page->paragraphs) {
        for (auto s : p.sentences) {
            for (auto w : s.words) {
                _words.push_back(w);
            }
        }
    }
    
    /*
    for (int i=0; i<page->paragraphs[0].sentences.size(); i++) {
        TodoSentence s = page->paragraphs[0].sentences[i];
        
        for (int j=0; j<s.words.size(); j++) {
            
            auto word = s.words[j];
            _words.push_back(word);
        }
    }
     */
    
    if (true) {
        auto text = createTextViewMultiLine(textViewSize, page->fontSize);
        _textView->addChild(text);
    } else {
        auto text = createTextViewOneLine(textViewSize, page->fontSize);
        _textView->addChild(text);
    }
    
    




    
}

void BookPage::setCreditPage()
{
    
    auto getSentence = [this](int index){
        string str;
        
        if (_page->paragraphs.size()>index) {
            auto p = _page->paragraphs.at(index);
            
            for (auto s : p.sentences) {
                if (s.words.size()==0) continue;
                auto w = s.words[0];
                str += w.word + "\n";
            }
        }
        return str;
    };
    

    if (_bookLayout==TDBookLayout::Portrait || _bookLayout==TDBookLayout::Portrait_Traditional) {
        _leftView = Node::create();
        _leftView->setContentSize(halfSize);
        _leftView->setAnchorPoint(Vec2::ANCHOR_BOTTOM_RIGHT);
        _leftView->setPosition(Vec2(halfSize.width, 0));
        _contentsView->addChild(_leftView);
        
        if (_bookLayout==TDBookLayout::Portrait_Traditional) {
            _leftCover = Node::create();
            _leftCover->setContentSize(halfSize);
            _leftCover->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
            
            auto cv = Sprite::create("Books/portrait_traditional/koreanbook_cover.jpg", Rect(20, 00, 1280, 1800));
            cv->setScale(-1, 1);
            cv->setPosition(halfSize/2);
            _leftCover->addChild(cv);
            
        } else {
            _leftCover = Sprite::create("Books/portrait/xprize_vertical_layout_cover_back.jpg", Rect(0, 0, 1280, 1800));
        }
        _leftCover->setPosition(halfSize/2);
        _leftView->addChild(_leftCover);
        
        
        
        _rightView = Node::create();
        _rightView->setContentSize(halfSize);
        _rightView->setPosition(Vec2(halfSize.width, 0));
        _contentsView->addChild(_rightView);
        
        if (_bookLayout==TDBookLayout::Portrait_Traditional) {
            _rightCover = Node::create();
            _rightCover->setContentSize(Size(20, 1800));
            _rightCover->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
            
            auto cv = Sprite::create("Books/portrait_traditional/koreanbook_cover.jpg", Rect(0, 0, 20, 1800));
            cv->setScale(-1, 1);
            cv->setPosition(_rightCover->getContentSize()/2);
            _rightCover->addChild(cv);
            
        } else {
            _rightCover = Sprite::create("Books/portrait/xprize_vertical_layout_cover_back.jpg", Rect(1280, 0, 20, 1800));
        }
        
        _rightCover->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
        _rightView->addChild(_rightCover);
        

        

        
        
        _imageView = Node::create();
        _leftView->addChild(_imageView);
        
        
        auto centerOffset = -30;
        
        auto textViewSize = Size(1110, 1500);
        _textView = Node::create();
        _textView->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        _textView->setContentSize(textViewSize);
        _textView->setPosition(halfSize/2 + Size(centerOffset, 100));
        _leftView->addChild(_textView);
        
        
        
        
        {
            string titleTxt = getSentence(0);
            Label* label = Label::createWithTTF(titleTxt, titleFont, 75, Size::ZERO, TextHAlignment::CENTER, TextVAlignment::CENTER);
            if (_bookLayout==TDBookLayout::Portrait_Traditional) {
                label->setTextColor(Color4B(77, 35, 20, 255));
            } else {
                label->setTextColor(Color4B(255, 249, 225, 255));
            }
            label->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
            label->setPosition(textViewSize/2 + Size(0, 50));
            _textView->addChild(label);
        }
        
        
        {
            string creditTxt = getSentence(1);
            
            Label* label = Label::createWithTTF(creditTxt, titleFont, 50, textViewSize, TextHAlignment::CENTER, TextVAlignment::TOP);
            label->setTextColor(Color4B(98, 61, 29, 255));
            label->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
            label->setPosition(textViewSize/2);
            _textView->addChild(label);
        }
        
        {
            string licenseTxt = getSentence(2);
            
            auto bottom = Label::createWithTTF(licenseTxt, titleFont, 25, Size(0, 0), TextHAlignment::CENTER, TextVAlignment::CENTER);
            bottom->setTextColor(Color4B(98, 61, 29, 255));
            bottom->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
            bottom->setPosition(Size(halfSize.width/2+centerOffset, 50));
            _leftView->addChild(bottom);
            
            auto cc = Sprite::create("Books/cc_logo.png");
            cc->setPosition(Vec2(halfSize.width/2+centerOffset, bottom->getContentSize().height + 120));
            _leftView->addChild(cc);
            
            auto enuma = Label::createWithTTF("(c) 2015 Enuma, Inc.", titleFont, 30);
            enuma->setTextColor(Color4B(98, 61, 29, 255));
            enuma->setPosition(cc->getPosition() + Vec2(0, cc->getContentSize().height/2 + 40));
            _leftView->addChild(enuma);
            
        }

        
    } else if (_bookLayout==TDBookLayout::Landscape) {
        
        
        _leftView = Node::create();
        _leftView->setContentSize(bigSize);
        _leftView->setAnchorPoint(Vec2::ANCHOR_BOTTOM_RIGHT);
        _leftView->setPosition(Vec2(smallSize.width, 0));
        _contentsView->addChild(_leftView);
        
        _leftCover = Node::create();
        _leftCover->setContentSize(bigSize);
        _leftView->addChild(_leftCover);
        
        
        auto sp = Sprite::create("Books/book_horizontal_bg.jpg" , Rect(80/2, 0, (2560-80)/2, 1800/2));
        sp->setScale(2);
        sp->setAnchorPoint(Vec2::ANCHOR_BOTTOM_RIGHT);
        sp->setPosition(Vec2(bigSize.width, 0));
        _leftCover->addChild(sp);
        
        
        
        auto leftBinding = Sprite::create("Books/book_horizontal_inside_bookbinding.png", Rect(0, 0, 476/2, 1800));
        leftBinding->setPosition(Vec2(bigSize.width, 0));
        leftBinding->setAnchorPoint(Vec2::ANCHOR_BOTTOM_RIGHT);
        _leftCover->addChild(leftBinding);
        
        
        auto wp1 = Scale9Sprite::create("Books/book_horizontal_inside_pagespread_1.png");
        wp1->setPreferredSize(Size(bigSize.width, 1712));
        wp1->setAnchorPoint(Vec2::ANCHOR_BOTTOM_RIGHT);
        wp1->setPosition(Vec2(bigSize.width, 30));
        _leftView->addChild(wp1);
        
        auto sd1 = Scale9Sprite::create("Books/book_vertical_middleshadow_left_1.png");
        sd1->setPreferredSize(Size(85, 1692));
        sd1->setAnchorPoint(Vec2::ANCHOR_BOTTOM_RIGHT);
        sd1->setPosition(Vec2(bigSize.width, 50));
        _leftView->addChild(sd1);
        
        
        
        _rightView = Node::create();
        _rightView->setContentSize(bigSize);
        _rightView->setPosition(Vec2(smallSize.width, 0));
        _contentsView->addChild(_rightView);
        
        _rightCover = Node::create();
        _rightView->addChild(_rightCover);
        
        auto rsp = Sprite::create("Books/book_horizontal_bg.jpg", Rect(80/2, 0, (2560-80)/2, 1800/2));
        rsp->setScale(2);
        rsp->setPosition(bigSize/2);
        _rightCover->addChild(rsp);
        
        
        auto rightBinding = Sprite::create("Books/book_horizontal_inside_bookbinding.png", Rect(476/2, 0, 476/2, 1800));
        rightBinding->setPosition(Vec2(0, 0));
        rightBinding->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
        _rightCover->addChild(rightBinding);
        
        auto textViewSize = Size(2000, 1200);
        
        
        _textView = Node::create();
        _textView->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        _textView->setContentSize(textViewSize);
        _textView->setPosition(defaultSize/2 + Size(0, 400));
        _rightView->addChild(_textView);
        
        

        
        
        {
            string titleTxt = getSentence(0);
            Label* label = Label::createWithTTF(titleTxt, titleFont, 75, Size::ZERO, TextHAlignment::CENTER, TextVAlignment::CENTER);
            label->setTextColor(Color4B(255, 249, 225, 255));
            label->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
            label->setPosition(Vec2(textViewSize.width/2, textViewSize.height - 450));
            _textView->addChild(label);
        }
        
        
        {
            string creditTxt = getSentence(1);
            
            Label* label = Label::createWithTTF(creditTxt, titleFont, 50, textViewSize, TextHAlignment::CENTER, TextVAlignment::TOP);
            label->setTextColor(Color4B(54, 28, 14, 255));
            label->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
            label->setPosition(Vec2(textViewSize.width/2, textViewSize.height - 600));
            _textView->addChild(label);
        }
        
        {
            string licenseTxt = getSentence(2);
            
            auto bottom = Label::createWithTTF(licenseTxt, titleFont, 25, Size(0, 0), TextHAlignment::CENTER, TextVAlignment::CENTER);
            bottom->setTextColor(Color4B(255, 249, 225, 255));
            bottom->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
            bottom->setPosition(Size(defaultSize.width/2, 50));
            _rightView->addChild(bottom);
            
            auto cc = Sprite::create("Books/cc_logo.png");
            cc->setPosition(Vec2(defaultSize.width/2, bottom->getContentSize().height + 120));
            _rightView->addChild(cc);
            
            auto enuma = Label::createWithTTF("(c) 2015 Enuma, Inc.", titleFont, 30);
            enuma->setTextColor(Color4B(255, 249, 225, 255));
            enuma->setPosition(cc->getPosition() + Vec2(0, cc->getContentSize().height/2 + 40));
            _rightView->addChild(enuma);
            
        }
        


    }  else if (_bookLayout==TDBookLayout::Square) {
        
        
        _leftView = Node::create();
        _leftView->setContentSize(squareSize);
        _leftView->setAnchorPoint(Vec2::ANCHOR_BOTTOM_RIGHT);
        _leftView->setPosition(Vec2(squareMiddle, defaultSize.height/2 - squareSize.height/2));
        _contentsView->addChild(_leftView);
        
        _leftCover = Node::create();
        _leftView->addChild(_leftCover);
        
        auto cl = LayerColor::create(Color4B::WHITE, squareSize.width, squareSize.height);
        _leftView->addChild(cl);
        
        auto leftBinding = Sprite::create("Books/square/book_abc_insidespread_binding.png", Rect(0, 0, 171/2, 1520));
        leftBinding->setPosition(Vec2(squareSize.width, 0));
        leftBinding->setAnchorPoint(Vec2::ANCHOR_BOTTOM_RIGHT);
        _leftView->addChild(leftBinding);


        
        
        _rightView = Node::create();
        _rightView->setContentSize(squareSize);
        _rightView->setPosition(Vec2(squareMiddle, defaultSize.height/2 - squareSize.height/2));
        _contentsView->addChild(_rightView);
        
        _rightCover = Node::create();
        _rightView->addChild(_rightCover);
        
        
        
        auto cr = LayerColor::create(Color4B::WHITE, squareSize.width, squareSize.height);
        _rightView->addChild(cr);
        
        auto rightBinding = Sprite::create("Books/square/book_abc_insidespread_binding.png", Rect(171/2, 0, 171/2, 1520));
        rightBinding->setPosition(Vec2(0, 0));
        rightBinding->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
        _rightView->addChild(rightBinding);
        

        
        auto textViewSize = Size(1200, 1200);
        
        
        _textView = Node::create();
        _textView->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        _textView->setContentSize(textViewSize);
        _textView->setPosition(squareSize/2);
        _rightView->addChild(_textView);
        
        
        
        
        
        {
            string titleTxt = getSentence(0);
            Label* label = Label::createWithTTF(titleTxt, titleFont, 75, Size::ZERO, TextHAlignment::CENTER, TextVAlignment::CENTER);
            label->setTextColor(Color4B(255, 249, 225, 255));
            label->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
            label->setPosition(Vec2(textViewSize.width/2, textViewSize.height - 450));
            _textView->addChild(label);
        }
        
        
        {
            string creditTxt = getSentence(1);
            
            Label* label = Label::createWithTTF(creditTxt, titleFont, 50, textViewSize, TextHAlignment::CENTER, TextVAlignment::TOP);
            label->setTextColor(Color4B(54, 28, 14, 255));
            label->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
            label->setPosition(Vec2(textViewSize.width/2, textViewSize.height - 600));
            _textView->addChild(label);
        }
        
        {
            string licenseTxt = getSentence(2);
            
            auto bottom = Label::createWithTTF(licenseTxt, titleFont, 25, Size(0, 0), TextHAlignment::CENTER, TextVAlignment::CENTER);
            bottom->setTextColor(Color4B(54, 28, 14, 255));
            bottom->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
            bottom->setPosition(Size(squareSize.width/2, 50));
            _rightView->addChild(bottom);
            
            auto cc = Sprite::create("Books/cc_logo.png");
            cc->setPosition(Vec2(squareSize.width/2, bottom->getContentSize().height + 120));
            _rightView->addChild(cc);
            
            auto enuma = Label::createWithTTF("(c) 2015 Enuma, Inc.", titleFont, 30);
            enuma->setTextColor(Color4B(54, 28, 14, 255));
            enuma->setPosition(cc->getPosition() + Vec2(0, cc->getContentSize().height/2 + 40));
            _rightView->addChild(enuma);
            
        }
        
        
        
    }
    

    
    
    
}

void BookPage::showRightHalf(bool animate)
{
    if (!animate) {
        _rightView->setVisible(true);
        _rightCover->setVisible(true);
    } else {
        _rightView->setVisible(true);
        _rightView->setScale(0, 1);
        _rightView->setSkewY(skewY);
        if (!isTitlePage() && !isLastPage()) _rightCover->setVisible(false);
        
        
        if (!isTitlePage() && !_hasBackCover && isLastPage()) _rightCover->setVisible(false);
        auto ani = Spawn::create(EaseIn::create(ScaleTo::create(turnDuration, 1, 1), turnEase),
                                 EaseIn::create(SkewTo::create(turnDuration, 0, 0), turnEase),
                                 nullptr);
        _rightView->runAction(Sequence::create(ani,
                                               CallFunc::create([this](){ _rightCover->setVisible(true); }), nullptr));
    }
}

void BookPage::showLeftHalf(bool animate)
{
    if (!animate) {
        _leftView->setVisible(true);
        _leftCover->setVisible(true);
    } else {
        _leftView->setVisible(true);
        _leftView->setScale(0, 1);
        _leftView->setSkewY(-skewY);
        
        if (!isFirstPage() && !isCreditPage()) _leftCover->setVisible(false);
        if (!_hasBackCover && isCreditPage()) _leftCover->setVisible(false);
        auto ani = Spawn::create(EaseIn::create(ScaleTo::create(turnDuration, 1, 1), turnEase),
                                 EaseIn::create(SkewTo::create(turnDuration, 0, 0), turnEase),
                                 nullptr);
        _leftView->runAction(Sequence::create(ani,
                                              CallFunc::create([this](){ _leftCover->setVisible(true); }), nullptr));
        
    }
    
}

void BookPage::hideRightHalf(bool animate)
{
    if (!animate) {
        _rightView->setVisible(false);
    } else {
        _rightView->setScale(1, 1);
        if (!isTitlePage() && !isLastPage()) _rightCover->setVisible(false);
        if (!isTitlePage() && !_hasBackCover && isLastPage()) _rightCover->setVisible(false);
        auto ani = Spawn::create(EaseOut::create(ScaleTo::create(turnDuration, 0, 1), turnEase),
                                 EaseOut::create(SkewTo::create(turnDuration, 0, skewY), turnEase),
                                 nullptr);
        _rightView->runAction(Sequence::create(ani,
                                              CallFunc::create([this](){_rightView->setVisible(false);}),
                                              nullptr));
    }
}

void BookPage::hideLeftHalf(bool animate)
{
    if (!animate) {
        _leftView->setVisible(false);
    } else {
        _leftView->setScale(1, 1);
        if (!isFirstPage() && !isCreditPage()) _leftCover->setVisible(false);
        if (!_hasBackCover && isCreditPage()) _leftCover->setVisible(false);
        auto ani = Spawn::create(EaseOut::create(ScaleTo::create(turnDuration, 0, 1), turnEase),
                                 EaseOut::create(SkewTo::create(turnDuration, 0, -skewY), turnEase),
                                 nullptr);
        _leftView->runAction(Sequence::create(ani,
                                              CallFunc::create([this](){_leftView->setVisible(false);}),
                                              nullptr));
    }
}

Node* BookPage::createTextViewOneLine(Size size, float fontSize)
{
    
    auto tempTextView = Node::create();
    tempTextView->setContentSize(size);
    
    float pageFontSize = (fontSize>0) ? fontSize : size.height-5;
    float currentX = 0.0;
    
    for (int i=0; i<_words.size(); i++) {
        auto word = _words[i];
        auto wordButton = Button::create();
        
        wordButton->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        wordButton->setTag(i);
        wordButton->setTitleFontSize(pageFontSize);
        wordButton->setTitleFontName(bodyFont);
        wordButton->setTitleText(word.word);
        wordButton->setTitleColor(textColor);
        
        
        auto buttonSize = wordButton->getContentSize();
        
        wordButton->setPosition(Vec2(currentX+buttonSize.width/2, size.height/2));
        currentX += buttonSize.width + pageFontSize/2;
        
        if (_withAudio) {
            auto wordAudioPath = _book->getWordAudioPath(word.wordAudioFilename);
            GameSoundManager::getInstance()->preloadEffect(wordAudioPath);
            wordButton->addClickEventListener([this, word, wordAudioPath, wordButton](Ref*){
                //GameSoundManager::getInstance()->pauseEffect(_readingAudioID);
                GameSoundManager::getInstance()->pauseBGM();
                
                wordButton->resetNormalRender();
                wordButton->loadTextureNormal("Common/lightblue.png");
                GameSoundManager::getInstance()->playEffectSound(wordAudioPath);
                _pauseReading = true;
                _pauseLength = word.wordAudioLength;
                
            });
        }
        
        
        tempTextView->addChild(wordButton);
        _wordButtons.push_back(wordButton);
    }
    
    
    Size tempTextViewSize = Size(currentX-pageFontSize, size.height);
    tempTextView->setContentSize(tempTextViewSize);
    tempTextView->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    tempTextView->setPosition(size/2);
    
    float textScale = MIN(1.0, size.width/tempTextViewSize.width);
    tempTextView->setScale(textScale);
    
    return tempTextView;
}


Node* BookPage::createTextViewMultiLine(Size size, float fontSize)
{


    auto innerTextView = Node::create();
    
    auto pageFontSize = (fontSize>0) ? fontSize : 80;
    
    // (s) one-pass
    Size viewSize = size;
    
    float maxWidth = 0.f;
    float minBottom = 0.f;
    
    float currentLeft = 0.f;
    float currentTop = 0.f;
    
    int wordIndex = 0;
    
    float paragraphSpace = pageFontSize;
    float lineSpace = pageFontSize*1.5;
    float indentSpace = pageFontSize*0.5;
    float wordSpace = pageFontSize*0.5;
    
    
    bool wordWrap = _page->wordwrap;
    
    
    
    bool indentation = wordWrap || (_page->paragraphs.size()>1);
    bool finalPass = !wordWrap;
    
    bool firstInLine;
    
    
    
    auto createWordButton = [&](string word) {
        
        auto wordButton = Button::create();
        wordButton->setTag(wordIndex++);
        wordButton->setTouchEnabled(_withAudio);
        wordButton->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
        wordButton->setTitleFontSize(pageFontSize);
        wordButton->setTitleFontName(bodyFont);
        wordButton->setTitleText(word);
        wordButton->setTitleColor(textColor);
        return wordButton;
         
    };
    
    auto addAudioHandler = [&](Button* button, string path, float length) {
        GameSoundManager::getInstance()->preloadEffect(path);
        button->addClickEventListener([this, path, button, length](Ref*){
            
            //GameSoundManager::getInstance()->pauseEffect(_readingAudioID);
            GameSoundManager::getInstance()->pauseBGM();
            button->resetNormalRender();
            button->loadTextureNormal("Common/lightblue.png");
            
            GameSoundManager::getInstance()->playEffectSound(path);
            _pauseReading = true;
            _pauseLength = length;
            
        });
    };
    
    
    
    for (int i = 0; i < _page->paragraphs.size(); i++)
    {
        auto paragraph = _page->paragraphs[i];
        
        currentLeft = indentation ? indentSpace : 0; // auto indentation
        if (i > 0) currentTop -= paragraphSpace;  // paragraph spacing
        
        firstInLine = true;
        

        for (int j = 0; j < paragraph.sentences.size(); j++)
        {
            auto sentence = paragraph.sentences[j];
            
            for (int k = 0; k < sentence.words.size(); k++)
            {
                auto word = sentence.words[k];
                auto wordButton = createWordButton(word.word);
                auto buttonSize = wordButton->getContentSize();

                if (wordWrap && (currentLeft + wordSpace + buttonSize.width > viewSize.width)) { // do wordwrapping
                    currentLeft = 0;
                    currentTop -= lineSpace;
                    firstInLine = true;
                } else {
                    currentLeft += firstInLine ? 0 : wordSpace;
                }
                
                if (finalPass) {
                    wordButton->setPosition(Vec2(currentLeft, currentTop));
                    innerTextView->addChild(wordButton);
                    
                    if (_withAudio) {
                        auto wordAudioPath = _book->getWordAudioPath(word.wordAudioFilename);
                        addAudioHandler(wordButton, wordAudioPath, word.wordAudioLength);
                        _wordButtons.push_back(wordButton);
                    }
                }
                
                firstInLine = false;
                currentLeft += buttonSize.width;
                minBottom = MIN(minBottom, (currentTop-buttonSize.height));
                maxWidth = MAX(maxWidth, currentLeft);
                
            }
            
            if (!wordWrap) { // force next line if no wordwrap
                currentLeft = 0;
                currentTop -= lineSpace;
                firstInLine = true;
            }
        }
        
        if (wordWrap) {
            currentLeft = 0;
            currentTop -= lineSpace;
            firstInLine = true;
        }
    }
    // (e) one-pass
    
    
    if (finalPass) {
        auto textView = Node::create();
        textView->setContentSize(Size(maxWidth, -minBottom));
        innerTextView->setPosition(Vec2(0, -minBottom));
        textView->addChild(innerTextView);
        textView->setScale(MIN(1.f, size.width / maxWidth));
        textView->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        textView->setPosition(size/2);
        return textView;
    }
    
    
    
    // (s) two-pass
    
    auto sampleHeight = -minBottom;
    
    auto pageScale = MAX(1.f, sqrt(sampleHeight/size.height));

    viewSize = Size(size.width*pageScale, sampleHeight);
    finalPass = true;
    
    currentLeft = 0.f;
    currentTop = viewSize.height;
    
    maxWidth = 0.f;
    minBottom = currentTop;
    

    
    wordIndex = 0;

    
    
    for (int i = 0; i < _page->paragraphs.size(); i++)
    {
        auto paragraph = _page->paragraphs[i];
        
        currentLeft = indentation ? indentSpace : 0; // auto indentation
        if (i > 0) currentTop -= paragraphSpace;  // paragraph spacing
        
        firstInLine = true;
        
        
        for (int j = 0; j < paragraph.sentences.size(); j++)
        {
            auto sentence = paragraph.sentences[j];
            
            for (int k = 0; k < sentence.words.size(); k++)
            {
                
                auto word = sentence.words[k];
                
                auto wordButton = createWordButton(word.word);
                
                
                auto buttonSize = wordButton->getContentSize();
                if (wordWrap && (currentLeft + wordSpace + buttonSize.width > viewSize.width)) { // do wordwrapping
                    
                    currentLeft = 0;
                    currentTop -= lineSpace;
                    firstInLine = true;
                    
                    
                } else {
                    currentLeft += firstInLine ? 0 : wordSpace;
                }
                
                if (finalPass) {
                    wordButton->setPosition(Vec2(currentLeft, currentTop));
                    innerTextView->addChild(wordButton);
                    
                    if (_withAudio) {
                        auto wordAudioPath = _book->getWordAudioPath(word.wordAudioFilename);
                        GameSoundManager::getInstance()->preloadEffect(wordAudioPath);
                        addAudioHandler(wordButton, wordAudioPath, word.wordAudioLength);
                        
                        _wordButtons.push_back(wordButton);

                        
                    }
                    
                }
                
                firstInLine = false;
                currentLeft += buttonSize.width;
                minBottom = MIN(minBottom, (currentTop-buttonSize.height));
                maxWidth = MAX(maxWidth, currentLeft);
                
            }
            
            if (!wordWrap) { // force next line if no wordwrap
                currentLeft = 0;
                currentTop -= lineSpace;
                firstInLine = true;
            }
        }
        
        if (wordWrap) {
            currentLeft = 0;
            currentTop -= lineSpace;
            firstInLine = true;
        }
    }
    
    {
        auto textView = Node::create();
        viewSize = Size(maxWidth, viewSize.height-minBottom);
        textView->setContentSize(viewSize);
        innerTextView->setPosition(Vec2(0, -minBottom));
        textView->addChild(innerTextView);
        textView->setScale(MIN(1.f, MIN(size.width / viewSize.width, size.height / viewSize.height)));
        textView->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        textView->setPosition(size/2);
        return textView;
    }
    
}

void BookPage::setPageNum(int page, Node *_pageView, Vec2 pos)
{
    Label* pagelabel = Label::create();
    pagelabel->setSystemFontSize(40);
    pagelabel->setTextColor(Color4B::BLACK);
    pagelabel->setString(TodoUtil::itos(page));
    pagelabel->setPosition(pos);
    
    _pageView->addChild(pagelabel);
}

