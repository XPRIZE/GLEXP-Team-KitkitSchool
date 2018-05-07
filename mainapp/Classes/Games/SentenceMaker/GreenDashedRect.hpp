//
//  GreenDashedRect.hpp
//  KitkitSchool-mobile
//
//  Created by JungJaehun on 13/02/2018.
//

#ifndef GreenDashedRect_hpp
#define GreenDashedRect_hpp

#include <stdio.h>
#include <string>
#include "cocos2d.h"

using namespace cocos2d;
using namespace std;

class WordItem;
class GreenDashedRect : public Node
{
public:
    static GreenDashedRect* create();
    virtual bool init();
    void initUI();
    void setBodyWidth(float width);
    float getBodyWidth(float width);
    int _index;
    bool _stretched;
    WordItem* _pair;
    string _word;
    
    float _defaultWidth, _currentBodyWidth;

private:
    Sprite *_left, *_right, *_body;
    Texture2D* _horiTex;
};

#endif /* GreenDashedRect_hpp */
