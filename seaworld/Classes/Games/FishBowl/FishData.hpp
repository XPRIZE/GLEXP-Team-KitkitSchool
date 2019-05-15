//
//  FishObject.hpp
//  KitKit_FB
//
//  Created by jesun kim on 28/08/2018.
//

#ifndef FishData_hpp
#define FishData_hpp

#include "cocos2d.h"

USING_NS_CC;

class FishData : public Ref{
    
public:
    FishData();
    ~FishData();
    
public:
    std::string strFishID;
    std::string strName;
    bool isAvailable;
    float idleTime;
    float speed;
    std::vector<int> moveRange;
    bool isRare;
    bool isBottom;
    bool isHorizontal;
    bool isLookTop;
    std::vector<int> vecSoundFrame;
    
    std::string strFileName;
    std::string strDirectory;
    bool isFish;
};

#endif /* FishData_hpp */
