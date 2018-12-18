//
//  Subtitle.cpp
//  KitkitSchool
//
//  Created by Jaehun Jung on 18/09/2018.
//

#include "Subtitle.hpp"

#include "Utils/TodoUtil.h"

Subtitle* Subtitle::create() {
    Subtitle *subtitle = new (std::nothrow) Subtitle();
    if (subtitle && subtitle->init())
    {
        subtitle->autorelease();
        return subtitle;
    }
    CC_SAFE_DELETE(subtitle);
    return nullptr;
}

void Subtitle::setFileName(string fileName) {
    
    _subtitleV.clear();
    
    std::string rawString = FileUtils::getInstance()->getStringFromFile(fileName);

    istringstream iss(rawString);
    string line;
    string word;
    float start, end;
    bool pushFlag = false;
    
    while(iss.good())
    {
        TodoUtil::safegetline(iss, line);
        line = TodoUtil::trim(line);
        if (line.find("-->") != string::npos) {
            start = getSecondFromTimeCode(line.substr(0,12));
            end = getSecondFromTimeCode(line.substr(17,12));
            pushFlag = true;
            continue;
        }
        
        if (pushFlag) {
            _subtitleV.push_back({start, end, line});
            pushFlag = false;
        }
    }
    
}

float Subtitle::getSecondFromTimeCode(string timeCode) {
    auto secondsV = TodoUtil::split(timeCode, ':');
    int h = TodoUtil::stoi(secondsV[0]);
    int m = TodoUtil::stoi(secondsV[1]);
    TodoUtil::replaceAll(secondsV[2], ",","");
    int s = TodoUtil::stoi(secondsV[2]);
    return (float)h*60*60 + (float)m*60 + (float)s/1000;
    
}

void Subtitle::start() {
    if (!_subtitleV.size()) return;
    runAction(RepeatForever::create(Sequence::create(
        CallFunc::create([this](){
            bool blank = true;
            for (auto it : _subtitleV) {
                if (it.start < _currentTime && _currentTime < it.end) {
                    blank = false;
                    if (_currentContent == it.content) break;
                    setContent(it.content);
                    CCLOG("%f:%s", _currentTime, it.content.c_str());
                    break;
                }
            }
            if (blank && _currentContent != "") {
                _currentContent = "";
                this->removeAllChildren();
            }
            _currentTime += 0.1;
        }),
        DelayTime::create(0.1),
        nullptr
    )));
    
}

void Subtitle::setContent(string content) {

    _currentContent = content;
    
    this->removeAllChildren();
    
    const string font = "fonts/andika-r.ttf";
    const float shadow = 8;
    
    auto drawShadow = [this, font, content](float posX, float posY){
        auto shadow = TodoUtil::createLabel(content, 100, Size::ZERO, font, Color4B::BLACK);
        shadow->setPosition(posX,posY);
        this->addChild(shadow);
    };
    
    drawShadow(shadow*-1, shadow*-1);
    drawShadow(shadow*-1, 0);
    drawShadow(shadow*-1, shadow);

    drawShadow(0, shadow*-1);
    //drawShadow(0, 0);
    drawShadow(0, shadow);

    drawShadow(shadow, shadow*-1);
    drawShadow(shadow, 0);
    drawShadow(shadow, shadow);

    auto label = TodoUtil::createLabel(content, 100, Size::ZERO, font, Color4B::WHITE);
    this->addChild(label);
    
}
