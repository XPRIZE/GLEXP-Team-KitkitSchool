//
//  Fish.hpp
//  KitkitSchool
//
//  Created by YongSoo Hong on 2018. 9. 7..
//

#ifndef Fish_hpp
#define Fish_hpp

#include <stdio.h>
#include "cocos2d.h"

using namespace std;
USING_NS_CC;

class Fish : public Node {
public:
    static Fish* create(char category, int level, bool isRare, int skin = 1, bool isGray = false);
    bool init(char category, int level, bool isRare, int skin = 1, bool isGray = false);
    
    static string getFishID(char category, int level, bool isRare);
    static vector<string> getFishPosition(string fishID);
    void setContentSize(const Size &size) override;
    
    string getID() {return _fishID;};
    int getSkin() {return _skin;};

private:
    string _fishID;
    int _skin;
    
    Sprite* _spriteFish;
    void convertSpriteToGrayScale(Sprite* sprite);
    void fishScale(Size holdSize);
};

#endif /* Fish_hpp */
