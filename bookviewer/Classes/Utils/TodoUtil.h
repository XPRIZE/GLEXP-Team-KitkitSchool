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

class TodoUtil {

public:
	static std::vector<std::string> split(const std::string &s, char delim);
      
	static int stoi(const std::string &s);
	static std::string itos(const int& i);
    
    static double stod(const std::string &s);
    static std::string dtos(const double& d);
    
    static std::istream& safegetline(std::istream& is, std::string& t);
    static std::vector<std::string> splitCSV(std::string& line);
    static std::vector<std::vector<std::string>> readCSV(std::string& filedata);

    static std::string trim(std::string& str);
    static std::string removeSpaces(std::string input);
    
    static std::string returnStringOrNullString(std::string& str);
    
    static void replaceAll(std::string& str, const std::string& from, const std::string& to);
    static bool replace(std::string& str, const std::string& from, const std::string& to);
    

    static cocos2d::Label* createLabel(const std::string &text, float maxFontSize, Size boxSize,const std::string &fontName, const cocos2d::Color4B &color, cocos2d::TextHAlignment hAlignment = cocos2d::TextHAlignment::LEFT, cocos2d::TextVAlignment vAlignment = cocos2d::TextVAlignment::CENTER);
  
};


#endif /* TODOUTIL_H_ */
