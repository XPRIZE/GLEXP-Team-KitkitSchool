//
//  BookChoiceScene.cpp
//  enumaXprize
//
//  Created by Gunho Lee on 11/3/16.
//
//

#include "BookChoiceScene.hpp"

#include "Managers/BookManager.hpp"
#include "Common/Controls/TodoSchoolBackButton.hpp"
#include <functional>

#include "Games/Books/TodoBook.hpp"
#include "CCAppController.hpp"


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
    
    auto lang = LanguageManager::getInstance()->getCurrentLanguage();
    auto books = BookManager::getInstance()->getBookVector(lang);
    
    const Size bookButtonSize = Size(228 * 2, 141 * 2);
    auto numBooks = books.size();
    Size visibleSize = Director::getInstance()->getVisibleSize();
    float x = bookButtonSize.width;
    float y = bookButtonSize.height;

    
    function<void(int)> addBookButton = [&](int index) {
        auto info = books.at(index);
        TodoBook book;
        book.readFile("Books/BookData/"+info->bookPath);
        
        auto image = Sprite::create("Books/BookData/"+info->bookImagePath + book.titleImageFilename);
        auto imageSize = image->getContentSize();
        auto viewSize = bookButtonSize;
        auto scale = MIN(viewSize.width / imageSize.width, viewSize.height / imageSize.height);
        image->setScale(scale);
        image->setPosition(viewSize / 2);
        image->setTag(index);
        auto listener = EventListenerTouchOneByOne::create();
        listener->setSwallowTouches(true);
        
        listener->onTouchBegan = [image](Touch* T, Event* E) {
            return image->getBoundingBox().containsPoint(T->getLocation()) ? true : false;
        };
        
        listener->onTouchEnded = [books](Touch* T, Event* E) {
            auto info = books.at(E->getCurrentTarget()->getTag()); // tag value = index value
            TodoBook book;
            book.readFile(info->bookPath);
            CCAppController::sharedAppController()->startBookScene(info->bookPath, true);
        };
        
        image->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, image);
        image->setPosition(x, y);
        x += bookButtonSize.width * 1.5f;
        if (x + bookButtonSize.width * 1.4f > visibleSize.width) y += bookButtonSize.height * 1.5f;
        
        addChild(image);
    };
    
    for (int i = 0; i < numBooks; i++) {
        addBookButton(i);
    }
    
    auto backButton = TodoSchoolBackButton::create();
    backButton->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
    backButton->setPosition(Vec2(25, visibleSize.height-25));
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
