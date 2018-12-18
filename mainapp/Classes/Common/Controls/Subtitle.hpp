//
//  Subtitle.hpp
//  KitkitSchool
//
//  Created by Jaehun Jung on 17/09/2018.
//

#pragma once

#include <stdio.h>
#include "cocos2d.h"

using namespace cocos2d;
using namespace std;

struct SubtitleStruct {
    float start;
    float end;
    string content;
    
};

class Subtitle : public Node {
public:
    static Subtitle* create();
    void setFileName(string fileName);
    void start();
    float getSecondFromTimeCode(string timeCode);
    
    
private:
    vector<SubtitleStruct> _subtitleV;
    float _currentTime = 0;
    string _currentContent;
    
    void setContent(string content);

};
