#include "MainScene.h"
#include "stdio.h"
#include <string>
#include <vector>
#include "Utils/TodoUtil.h"
#include "TodoBook.hpp"
#include "BookView.hpp"
#include "ui/cocosGUI.h"


#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
#include "platform/android/jni/JniHelper.h"
#endif

using namespace std;
using namespace ui;

USING_NS_CC;

std::string MainScene::currentBook = "";




Scene* MainScene::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = MainScene::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}



string getDocumentsPath()
{

#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
    
    return "BookData/";
    
    string dir("");
    string tmp = JniHelper::callStaticStringMethod("org/cocos2dx/cpp/AppActivity", "getDocumentsPath");
    
    if (tmp.length() > 0)
    {
        dir.append(tmp).append("/");
        
        return dir + "Books/";
    }
    else
    {
        return "";
    }
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_MAC)

    
    string path = FileUtils::getInstance()->getWritablePath();
    
    //return path + "../work/xprize_books/game_books/";
    return path + "Books/";
#else
    return "BookData/";
#endif
    
}

string MainScene::getLaunchString() {
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
    string dir("");
    string tmp = JniHelper::callStaticStringMethod("org/cocos2dx/cpp/AppActivity", "getLaunchString");
    
    return tmp;
    
#else
    //return "book_portrait";
    return "";
    
#endif
}

void MainScene::clearLaunchString() {
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
    JniHelper::callStaticVoidMethod("org/cocos2dx/cpp/AppActivity", "clearLaunchString");
#endif
    
}

Scene* MainScene::createBookScene(std::string bookfolder)
{

    
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Size fixedSize = Size(1800.f*visibleSize.width/visibleSize.height, 1800.f);
    float sceneScale = visibleSize.height / 1800.f;
    
    TodoBook *book = new TodoBook;
    std::string bookPath = getDocumentsPath()+bookfolder+"/";
    bool ret = book->readFile(bookPath);
    if (ret)
    {
        auto bv = BookView::create(fixedSize, bookPath);
        bv->setLibraryMode(true);
        bv->setScale(sceneScale);
        bv->viewTitle();
        scene->addChild(bv);
    } else {
        exit(0);
    }
    
    return scene;
    
    

}


// on "init" you need to initialize your instance
bool MainScene::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }
    
    
    
    if (MainScene::getLaunchString().length()>0) {
        
        CCLOG("init with launch string...");
        BookView::setLibraryMode(true);
        showLoading();
    } else {
        CCLOG("init without launch string...");
        BookView::setLibraryMode(false);
        showChooser();
    }
    return true;
}


void MainScene::menuCloseCallback(Ref* pSender)
{
    

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
}

void MainScene::showLoading()
{
    CCLOG("showLoading...");
    
    auto winSize = Director::getInstance()->getVisibleSize();
    
    //    auto eng = LanguageManager::getInstance()->isEnglish();
    //    Sprite* loadingImage = Sprite::create(eng ? "System/todoschool_BI.png" : "System/todoshule_BI.png");
    Sprite *loadingImage = Sprite::create("Common/xprize_image_loading.png");
    loadingImage->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    loadingImage->setPosition(winSize/2.f);
    addChild(loadingImage);
    
    Size imageSize = loadingImage->getContentSize();
    
    /*
    Sprite *loadingMark = Sprite::create("Common/transition_ABC123.png");
    loadingMark->setAnchorPoint(Vec2::ANCHOR_MIDDLE_RIGHT);
    loadingMark->setPosition(Vec2(winSize.width/2-imageSize.width/2 - 20, winSize.height/2+imageSize.height/2));
    addChild(loadingMark);
    
    loadingMark->runAction(RepeatForever::create(
                                                 Sequence::create(DelayTime::create(0.2),
                                                                  MoveBy::create(0.0, Vec2(10, 0)),
                                                                  DelayTime::create(0.2),
                                                                  MoveBy::create(0.0, Vec2(-10, 0)), nullptr)));
    */
    
    std::string loadingText = IS_ENGLISH ? "Loading..." : "Tafadhali subiri...";
    
    Label *loadingLabel = Label::createWithTTF(loadingText, "fonts/TodoSchoolV2.ttf", 56);
    loadingLabel->setTextColor(Color4B(255, 240, 222, 255));
    loadingLabel->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
    loadingLabel->setPosition(Vec2(winSize.width/2, winSize.height/2 - imageSize.height/2 - 50));
    addChild(loadingLabel);

}

void MainScene::showChooser()
{
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Size fixedSize = Size(1800.f*visibleSize.width/visibleSize.height, 1800.f);
    float sceneScale = visibleSize.height / 1800.f;
    
    
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    auto path = getDocumentsPath();
    
    auto listFile = path + "booklist.txt";
    
    auto innerView = Node::create();
    
    
    vector<string> dirs;
    {
        auto l = Label::createWithSystemFont("path: " + getDocumentsPath(), "fonts/Seshat.otf", 20);
        l->setPosition(Vec2(visibleSize.width/2, -20));
        innerView->addChild(l);
    }
    {
        auto l = Label::createWithSystemFont("launch: " + getLaunchString(), "fonts/Seshat.otf", 20);
        l->setPosition(Vec2(visibleSize.width/2, -40));
        innerView->addChild(l);
    }

    string filedata = FileUtils::getInstance()->getStringFromFile(listFile);
    if (filedata.length()<=0) {
        auto l = Label::createWithSystemFont("cannot find "+listFile, "fonts/Seshat.otf", -20);
        
        l->setPosition(Vec2(visibleSize.width/2, -100));
        innerView->addChild(l);
    }
    
    
    
    auto dirlist = TodoUtil::readCSV(filedata);
    for (auto row : dirlist) {
        if (row.size()==0) continue;
        string dir = TodoUtil::trim(row[0]);
        if (dir.length()>=1) {
            dirs.push_back(dir);
        }
    }
    
    float y = -80;
    
    for (int i=0; i<dirs.size(); i++) {
        auto dir = dirs[i];
        
        auto button = Button::create();
        button->setTitleFontSize(50);
        button->setTitleColor(Color3B::WHITE);
        button->setTitleText(dir);
        button->setPosition(Vec2(visibleSize.width/2, y-=80));
        innerView->addChild(button);
        
        button->addClickEventListener([this, path, dir, button](Ref*) {
            if (!showBook(dir)) {
                auto l = Label::createWithSystemFont("cannot find "+path+dir+"/bookinfo.csv", "fonts/Seshat.otf", 20);
                l->setPosition(button->getPosition() + Vec2(300, -10));
                addChild(l);
            }
        });
    }

    
    auto height = -y + 100;
    auto scroll = ui::ScrollView::create();
    scroll->setContentSize(visibleSize);
    scroll->setInnerContainerSize(Size(visibleSize.width, height));
    innerView->setPosition(Vec2(0, height));
    scroll->addChild(innerView);
    addChild(scroll);
    
    
    {
        auto backBtn = ui::Button::create();
        backBtn->loadTextures("Common/Controls/back_arrow_inactive.png", "Common/Controls/back_arrow_active.png");
        
        auto keyListener = EventListenerKeyboard::create();
        keyListener->onKeyReleased = [this](EventKeyboard::KeyCode keyCode, Event *event) {
            if (keyCode == EventKeyboard::KeyCode::KEY_ESCAPE) {
                (Director::getInstance())->popScene();
            }
        };
        backBtn->getEventDispatcher()->addEventListenerWithSceneGraphPriority(keyListener, backBtn);
        backBtn->addClickEventListener([this](Ref*){
            (Director::getInstance())->popScene();
        });
        
        backBtn->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
        backBtn->setPosition(Vec2(25, visibleSize.height-25));
        addChild(backBtn);
    }
    
}

bool MainScene::showBook(std::string bookfolder)
{
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Size fixedSize = Size(1800.f*visibleSize.width/visibleSize.height, 1800.f);
    float sceneScale = visibleSize.height / 1800.f;
    
    TodoBook *book = new TodoBook;
    std::string bookPath = getDocumentsPath()+bookfolder+"/";
    bool ret = book->readFile(bookPath);
    if (ret)
    {
        

        
        auto scene = Scene::create();
        auto bv = BookView::create(fixedSize, bookPath);
        bv->setScale(sceneScale);
        bv->viewTitle();
        scene->addChild(bv);
        Director::getInstance()->pushScene(scene);
        
#if  (CC_TARGET_PLATFORM == CC_PLATFORM_MAC)
        auto err = book->checkData();
        if (err.length()>0) {
            
            auto layer = Node::create();
            layer->setContentSize(fixedSize);
            auto c = LayerColor::create(Color4B(0, 0, 0, 200));
            layer->addChild(c);
            auto w = TodoUtil::createLabel(err, 50, Size::ZERO, "fonts/Seshat.otf", Color4B::WHITE);
            auto wsize = w->getContentSize();
            w->setPosition(Vec2(fixedSize.width/2, fixedSize.height/2));
            layer->addChild(w);
            
            auto b = Button::create();
            b->setTitleText("OK");
            b->setTitleFontName("fonts/Seshat.otf");
            b->setTitleColor(Color3B::WHITE);
            b->setTitleFontSize(150);
            b->setPosition(Vec2(fixedSize.width/2, 100));
            b->addClickEventListener([layer](Ref*) {
                layer->removeFromParent();
            });
            
            layer->addChild(b);
            
            
            
            scene->addChild(layer);
            
        }
        
#endif
        
        return true;
    }
    
    return false;
}

