/*
 * TodoUtil.h
 *
 *  Created on: Jan 9, 2015
 *      Author: jooddang
 */

#ifndef TODOUTIL_H_
#define TODOUTIL_H_

#include <string>
#include <vector>

#include "cocos2d.h"
USING_NS_CC;

#define FONT_ANDIKA_REGULAR     "fonts/Andika-R.ttf"
#define FONT_ANDIKA_BOLD        "fonts/AndikaNewBasic-B.ttf"

#define DELAYED_CALLFUNC_TARGET(TARGET, TIME, FUNCTION_CONTENT)\
TARGET->runAction(Sequence::create(DelayTime::create(TIME),CallFunc::create([this](){FUNCTION_CONTENT;}), nullptr))

#define DELAYED_CALLFUNC(TIME, FUNCTION_CONTENT)\
this->runAction(Sequence::create(DelayTime::create(TIME),CallFunc::create([this](){FUNCTION_CONTENT;}), nullptr))

#define DELAYED_CALLFUNC_1(TIME, PARAM1, FUNCTION_CONTENT)\
this->runAction(Sequence::create(DelayTime::create(TIME),CallFunc::create([PARAM1](){FUNCTION_CONTENT;}), nullptr))

#define DELAYED_CALLFUNC_2(TIME, PARAM1, PARAM2, FUNCTION_CONTENT)\
this->runAction(Sequence::create(DelayTime::create(TIME),CallFunc::create([PARAM1, PARAM2](){FUNCTION_CONTENT;}), nullptr))

#define DELAYED_CALLFUNC_3(TIME, PARAM1, PARAM2, PARAM3, FUNCTION_CONTENT)\
this->runAction(Sequence::create(DelayTime::create(TIME),CallFunc::create([PARAM1, PARAM2, PARAM3](){FUNCTION_CONTENT;}), nullptr))

#define DRAW_DEBUG_AREA(TARGET)\
{\
auto lc = LayerColor::create(Color4B(random(0, 255), random(0, 255), random(0, 255), 128));\
lc->setIgnoreAnchorPointForPosition(false);\
lc->setContentSize(TARGET->getContentSize());\
lc->setAnchorPoint(Vec2::ANCHOR_MIDDLE);\
lc->setPosition(TARGET->getContentSize()/2);\
TARGET->addChild(lc);\
}

class TodoUtil {

public:
	static std::vector<std::string> split(const std::string &s, char delim);
      
	static int stoi(const std::string &s);
	static std::string itos(const int& i);
    
    static double stod(const std::string &s);
    static std::string dtos(const double& d);
    
    static std::istream& safegetline(std::istream& is, std::string& t);
    static std::string combineCSV(std::vector<std::string> csv);
    static std::vector<std::string> splitCSV(std::string& line);
    static std::vector<std::vector<std::string>> readCSV(std::string& filedata);
    static std::vector<std::vector<std::string>> readTSV(std::string& filedata);

    

    static std::string trim(std::string& str);
    static std::string removeSpaces(std::string input);
    
    static std::string returnStringOrNullString(std::string& str);
	static bool startsWith(const std::string& s, const std::string& prefix);
    static bool endsWith(const std::string& s, const std::string& suffix);
	static bool contains(const std::string& s, const std::string& findStr);
    
    static void replaceAll(std::string& str, const std::string& from, const std::string& to);
    static bool replace(std::string& str, const std::string& from, const std::string& to);
    

    static cocos2d::Label* createLabel(const std::string &text, float maxFontSize, Size boxSize,const std::string &fontName, const cocos2d::Color4B &color, cocos2d::TextHAlignment hAlignment = cocos2d::TextHAlignment::LEFT, cocos2d::TextVAlignment vAlignment = cocos2d::TextVAlignment::CENTER);

    static cocos2d::Label* createLabelMultiline(const std::string &text, float maxFontSize, cocos2d::Size boxSize, const std::string &fontName, const cocos2d::Color4B &color, TextHAlignment hAlignment = TextHAlignment::LEFT, TextVAlignment vAlignment = TextVAlignment::CENTER);
    
    static cocos2d::Label* createLabelMultilineToFit(const std::string &text, float maxFontSize, cocos2d::Size boxSize, const std::string &fontName, const cocos2d::Color4B &color, TextHAlignment hAlignment = TextHAlignment::LEFT, TextVAlignment vAlignment = TextVAlignment::CENTER);

    static cocos2d::Label* createLabelMultilineToFitWidth(const std::string &text, float maxFontSize, cocos2d::Size boxSize, const std::string &fontName, const cocos2d::Color4B &color, TextHAlignment hAlignment = TextHAlignment::LEFT, TextVAlignment vAlignment = TextVAlignment::CENTER);
    
};


#endif /* TODOUTIL_H_ */
