//
//  TestPopup.cpp
//  enumaXprize
//
//  Created by Sungwoo Kang on 11/1/16.
//
//

#include "TestPopup.hpp"
#include <array>
#include "cocos2d.h"
#include "CCAppController.hpp"
#include "Utils/TodoUtil.h"
#include "3rdParty/CCNativeAlert.h"
#include "MainScene2.hpp"

#include "Managers/UserManager.hpp"

#include <time.h>
#include <algorithm>
#include <iterator>

using namespace std;

TestPopup* TestPopup::create(Node* parentView, Size size)
{
    TestPopup *popup = new (std::nothrow) TestPopup();
    if (popup && popup->init(parentView, size))
    {
        popup->autorelease();
        return popup;
    }
    CC_SAFE_DELETE(popup);
    return nullptr;
}


bool TestPopup::init(cocos2d::Node *parentView, Size size)
{
    if (!PopupBase::init(parentView, size)) {
        return false;
    }
    
    auto bg = LayerColor::create(Color4B::WHITE, size.width, size.height);
    addChild(bg);
    
    //Color4B darkGray = Color4B(166, 164, 162, 255);
    Color4B lightBlackColor = Color4B(65, 64, 62, 255);
    Color4B darkSkyColor = Color4B(23, 163, 232, 255);
    float labelFontSize = 50;
    
    Label* versionLabel = Label::createWithTTF("Ver 1.0", "fonts/OpenSans-Bold.ttf", 30);
    versionLabel->setTextColor(darkSkyColor);
    versionLabel->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
    versionLabel->setPosition(Vec2(10, size.height - 50));

    addChild(versionLabel);
    
    time_t rawtime;
    //struct tm * timeinfo;
    time(&rawtime);
    //timeinfo = localtime(&rawtime);
    
    string timeFormatted;
    char buf[100];
    strftime(buf, sizeof(buf), "%m-%d-%Y", localtime(&rawtime));
    timeFormatted = buf;
    
    Label* dateLabel = Label::createWithTTF(timeFormatted, "fonts/OpenSans-Bold.ttf", 30);
    dateLabel->setTextColor(darkSkyColor);
    dateLabel->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
    dateLabel->setPosition(Vec2(10, size.height - 100));
    addChild(dateLabel);
    
    
    
    
    Label* classTitle = Label::createWithTTF("ClassRoom", "fonts/OpenSans-Bold.ttf", labelFontSize);
    classTitle->setTextColor(darkSkyColor);
    classTitle->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    classTitle->setPosition(Vec2(size.width/2.f, 900));
    addChild(classTitle);
    
    std::array<std::string, 10> classes = {"A","B","C","D","E","F","G","H","I","J"};
    
    string previosClass = UserManager::getInstance()->getClassId();
    if (previosClass.empty()) {
        _classIdx = 0;
    }
    else {
        auto it = std::find(classes.begin(), classes.end(), previosClass);
        _classIdx = (int)std::distance(classes.begin(), it);
    }
    
    
    classLabel = Label::createWithTTF(classes[_classIdx], "fonts/OpenSans-Bold.ttf", labelFontSize);
    classLabel->setPosition(Vec2(size.width/2.f, 850));
    classLabel->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    classLabel->setTextColor(lightBlackColor);
    addChild(classLabel);
    
    
    
    ui::Button* classLeftButton = ui::Button::create();
    classLeftButton->setTitleText("<  <  ");
    classLeftButton->setTitleFontSize(labelFontSize*2);
    classLeftButton->setTitleColor(Color3B(lightBlackColor));
    classLeftButton->setPosition(Vec2(size.width/2.f-300, classLabel->getPositionY()));
    classLeftButton->addTouchEventListener( [this, classes](Ref* pSender, ui::Widget::TouchEventType type) {
        if (type == ui::Widget::TouchEventType::ENDED)
        {
            if (_classIdx > 0) {
                _classIdx--;
                classLabel->setString(classes[_classIdx]);
            }
        }
    } );
    addChild(classLeftButton);

    ui::Button* classRightButton = ui::Button::create();
    classRightButton->setTitleText("  >  >");
    classRightButton->setTitleFontSize(labelFontSize*2);
    classRightButton->setTitleColor(Color3B(lightBlackColor));
    classRightButton->setPosition(Vec2(size.width/2.f+300, classLabel->getPositionY()));
    classRightButton->addTouchEventListener( [this, classes](Ref* pSender, ui::Widget::TouchEventType type) {
        if (type == ui::Widget::TouchEventType::ENDED)
        {
            if (_classIdx < classes.size()-1) {
                _classIdx++;
                classLabel->setString(classes[_classIdx]);
            }
        }
    } );
    
    addChild(classRightButton);

    
    
    
    
    Label* numberTitle = Label::createWithTTF("Number", "fonts/OpenSans-Bold.ttf", labelFontSize);
    numberTitle->setTextColor(darkSkyColor);
    numberTitle->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    numberTitle->setPosition(Vec2(size.width/2.f, 650));
    addChild(numberTitle);
    
    numberField = ui::TextField::create("Enter student id", "fonts/OpenSans-Bold.ttf", labelFontSize);
    numberField->setTextColor(lightBlackColor);
    numberField->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    numberField->setPosition(Vec2(size.width/2.f, 600));
    addChild(numberField);
    
    string previosStudentId = UserManager::getInstance()->getStudentId();
    if (!previosStudentId.empty()) {
        numberField->setString(previosStudentId);
    }
    
    ui::Button* numberLeftButton = ui::Button::create();
    numberLeftButton->setTitleText("<  <  ");
    numberLeftButton->setTitleFontSize(labelFontSize*2);
    numberLeftButton->setTitleColor(Color3B(lightBlackColor));
    
    numberLeftButton->setPosition(Vec2(size.width/2.f-300, numberField->getPositionY()));
    numberLeftButton->addTouchEventListener( [this ](Ref* pSender, ui::Widget::TouchEventType type) {
        if (type == ui::Widget::TouchEventType::ENDED)
        {
            string studentNumberString = numberField->getString();
            
            int studentNumber = TodoUtil::stoi(studentNumberString);
            if (studentNumberString=="") studentNumber = 0;
            
            studentNumber--;
            if (studentNumber<1) studentNumber = 99;
            numberField->setString(StringUtils::format("%d", studentNumber));
    
            
        }
    } );
    addChild(numberLeftButton);
    
    ui::Button* numberRightButton = ui::Button::create();
    numberRightButton->setTitleText("  >  >");
    numberRightButton->setTitleFontSize(labelFontSize*2);
    numberRightButton->setTitleColor(Color3B(lightBlackColor));
    numberRightButton->setPosition(Vec2(size.width/2.f+300, numberField->getPositionY()));
    numberRightButton->addTouchEventListener( [this](Ref* pSender, ui::Widget::TouchEventType type) {
        if (type == ui::Widget::TouchEventType::ENDED)
        {
            string studentNumberString = numberField->getString();
            
            int studentNumber = TodoUtil::stoi(studentNumberString);
            if (studentNumberString=="") studentNumber = 0;
            
            studentNumber++;
            if (studentNumber>99) studentNumber = 1;
            numberField->setString(StringUtils::format("%d", studentNumber));
            
        }
    } );
    addChild(numberRightButton);

    
    
    
    std::array<std::string, 14> courses = {"Pre-Test","1","2","3","4","5","6","7","8","9","10","11","12","Post-Test"};
    _courseIdx = 0;
    
    Label* courseTitle = Label::createWithTTF("Course", "fonts/OpenSans-Bold.ttf", labelFontSize);
    courseTitle->setTextColor(darkSkyColor);
    courseTitle->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    courseTitle->setPosition(Vec2(size.width/2.f, 400));
    addChild(courseTitle);
    
    string previosCourse = UserManager::getInstance()->getCourseId();
    if (previosClass.empty()) {
        _courseIdx = 0;
    }
    else {
        auto it = std::find(courses.begin(), courses.end(), previosCourse);
        _courseIdx = (int)std::distance(courses.begin(), it);
    }
    
    courseLabel = Label::createWithTTF(courses[_courseIdx], "fonts/OpenSans-Bold.ttf", labelFontSize);
    courseLabel->setPosition(Vec2(size.width/2.f, 350));
    courseLabel->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    courseLabel->setTextColor(lightBlackColor);
    addChild(courseLabel);
    
    ui::Button* courseLeftButton = ui::Button::create();
    courseLeftButton->setTitleText("<  <  ");
    courseLeftButton->setTitleFontSize(labelFontSize*2);
    courseLeftButton->setTitleColor(Color3B(lightBlackColor));
    
    courseLeftButton->setPosition(Vec2(size.width/2.f-300, courseLabel->getPositionY()));
    courseLeftButton->addTouchEventListener( [this, courses](Ref* pSender, ui::Widget::TouchEventType type) {
        if (type == ui::Widget::TouchEventType::ENDED)
        {
            if (_courseIdx > 0 ) {
                _courseIdx--;
                courseLabel->setString(courses[_courseIdx]);
            }
            
        }
    } );
    addChild(courseLeftButton);
    
    ui::Button* courseRightButton = ui::Button::create();
    courseRightButton->setTitleText("  >  >");
    courseRightButton->setTitleFontSize(labelFontSize*2);
    courseRightButton->setTitleColor(Color3B(lightBlackColor));
    courseRightButton->setPosition(Vec2(size.width/2.f+300, courseLabel->getPositionY()));
    courseRightButton->addTouchEventListener( [this, courses](Ref* pSender, ui::Widget::TouchEventType type) {
        if (type == ui::Widget::TouchEventType::ENDED)
        {
            if (_courseIdx < courses.size()-1) {
                _courseIdx++;
                courseLabel->setString(courses[_courseIdx]);
            }
            
        }
    } );
    addChild(courseRightButton);
    
    

    auto nextButton = ui::Button::create();
//    nextButton->loadTextures("NumberMatching/Images/matchgame_next_inactivated.png", "NumberMatching/Images/matchgame_next_activated.png");

    nextButton->setTitleText("Next");
    nextButton->setTitleFontSize(100);
    nextButton->setTitleColor(Color3B(lightBlackColor));
    nextButton->setPosition(Vec2(size.width/2.f, 100));
    nextButton->addTouchEventListener(CC_CALLBACK_2(TestPopup::onNextPressed, this));
    
    addChild(nextButton);
    
    
    return true;
}

void TestPopup::onNextPressed(cocos2d::Ref *sender, ui::Widget::TouchEventType event)
{
    if (event == ui::Widget::TouchEventType::ENDED)
    {
        string classroom = classLabel->getString();
        string studentNumberString = numberField->getString();
        
        if (studentNumberString.empty()) {
            NativeAlert::show("Warning", "Please Enter student number.", "OK");
            return;
        }
        
        int studentNumber = TodoUtil::stoi(studentNumberString);
        
        if (studentNumber<1 || studentNumber>99) {
            NativeAlert::show("Warning", "Invalid student number.", "OK");
            return;
        }
        
        
        string course = courseLabel->getString();
        
        UserManager::getInstance()->setCurrentUser(classroom, studentNumberString, course);
        /*
         UserDefault::getInstance()->setStringForKey("currentClassroom", classroom);
         UserDefault::getInstance()->setStringForKey("currentCourse", course);
         UserDefault::getInstance()->setIntegerForKey("currentStudent", studentNumber);
         UserDefault::getInstance()->flush();
         */
        
        if (course == "Pre-Test" || course == "Post-Test") {
            CCAppController::sharedAppController()->startQuiz(classroom, studentNumber, course);
            dismiss(false);
        }
        else {
            _delegate->setMode(MainScene2::MODE_SELECT_COOP);
            dismiss(true);
        }
        
    }
    
}


