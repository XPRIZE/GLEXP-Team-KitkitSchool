//
//  BookChoiceScene.cpp
//  KitkitSchool
//
//  Created by Gunho Lee on 11/3/16.
//
//

#include "BookChoiceScene.hpp"

#include "Managers/LanguageManager.hpp"
#include "Common/Controls/TodoSchoolBackButton.hpp"
#include <functional>

#include "Games/Books/TodoBook.hpp"
#include "CCAppController.hpp"
#include "Utils/TodoUtil.h"
#include "Common/Controls/PopupBase.hpp"


Scene* BookChoiceScene::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    Size visibleSize = Director::getInstance()->getVisibleSize();
    scene->setContentSize(visibleSize);
    
    // 'layer' is an autorelease object
    auto layer = BookChoiceScene::create();
    
    // add layer as a child to scene
    scene->addChild(layer);
    
    // return the scene
    return scene;
}

bool BookChoiceScene::init()
{
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }
    
    Size winSize = Director::getInstance()->getWinSize();
    
    auto chooser = PopupBase::create(this, winSize);
    
    auto innerView = Node::create();

    
    
    float y = -80;
    
    auto list = FileUtils::getInstance()->listFiles("Books/BookData");
    
    for (auto folder : list) {
        string bookPath = folder + "bookinfo.csv";
        if (FileUtils::getInstance()->isFileExist(bookPath)) {
            auto button = ui::Button::create();
            button->setTitleFontSize(50);
            button->setTitleColor(Color3B::WHITE);
            auto book = TodoUtil::split(folder, '/').back();
            button->setTitleText(book);
            button->setPosition(Vec2(winSize.width/2, y-=80));
            innerView->addChild(button);
            
            button->addClickEventListener([this, book, button, chooser](Ref*) {
                chooser->dismiss(true);
                CCAppController::sharedAppController()->startBookScene(book+"/", true);
                
            });
            
        }
        
    }
    
    
    auto height = -y + 100;
    auto scroll = ui::ScrollView::create();
    scroll->setContentSize(winSize);
    scroll->setInnerContainerSize(Size(winSize.width, height));
    innerView->setPosition(Vec2(0, height));
    scroll->addChild(innerView);
    chooser->addChild(scroll);
    
    
    chooser->show(this, true);
    
    auto backButton = TodoSchoolBackButton::create();
    backButton->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
    backButton->setPosition(Vec2(25, winSize.height-25));
    addChild(backButton);
    
    return true;
}

/*
bool BookChoiceScene::init()
{
    
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }
    
    auto lang = LanguageManager::getInstance()->getCurrentLanguage();
    auto books = BookManager::getInstance()->getBookVector(lang);
    
    const Size bookButtonSize = Size(228*2, 141*2);
    auto numBooks = books.size();
    Size visibleSize = Director::getInstance()->getVisibleSize();
    float x = bookButtonSize.width;
    float y = bookButtonSize.height;
 
    function<void(BookInfo*)> addBookButton = [&](BookInfo *info) {
        TodoBook book;
        book.readFile(info->bookPath);
        
        auto image = Sprite::create(info->bookImagePath+book.titleImageFilename);
        auto imageSize = image->getContentSize();
        auto viewSize = bookButtonSize;
        auto scale = MIN(viewSize.width/imageSize.width, viewSize.height/imageSize.height);
        image->setScale(scale);
        image->setPosition(viewSize/2);
        
   
        auto listener = EventListenerTouchOneByOne::create();
        listener->setSwallowTouches(true);
        listener->onTouchBegan = [](Touch* T, Event* E) { return true; };
        listener->onTouchEnded = [info, book](Touch* T, Event* E) {
            CCAppController::sharedAppController()->startBookScene(info->bookTitle, book.bookOrientation, book.bookType, true);
        };
        
        image->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, image);
 
//        auto btn = ui::Widget::create();
//        btn->setContentSize(viewSize);
//        btn->addClickEventListener([info](Ref *pSender) {
//                    });
//        
//        image->addChild(btn);
        
        image->setPosition(x, y);
        
        x+= bookButtonSize.width*1.5;
        if (x+bookButtonSize.width*1.4>visibleSize.width) y += bookButtonSize.height*1.5;
        
        addChild(image);
    };
 
    for (int i=0; i<numBooks; i++) {
        auto info = books.at(i);
        addBookButton(info);
    }

    return true;
}
*/
