//
//  LetterWheel.hpp
//  enumaXprize
//
//  Created by Gunho Lee on 6/28/16.
//
//

#ifndef LetterWheel_hpp
#define LetterWheel_hpp

#include <stdio.h>
#include "cocos2d.h"
#include <string>
#include <functional>

USING_NS_CC;

class LetterWheel : public Node
{
public:
    virtual ~LetterWheel();
    
    virtual bool init() override;
    virtual void update(float) override;
    
    CREATE_FUNC(LetterWheel);
    
    
    void setup(std::string wheelString, Size viewSize, float numShowingLetters, float fontSize, float skew, bool playTick);
    
    void start(float velocity);
    void stop(char targetChar, float slowdownAfter, CallFunc *callback);
    

protected:
    

    CallFunc *_stopCallback;
    
    std::string _wheelString;
    int _wheelLength;
    
    int _currentLetterIndex;
    int _headLetterIndex;
    
    float _wheelOffset;
    float _letterHeight;
    float _centerX, _centerY;

    float _initVelocity;
    float _velocity;
    float _accel;
    float _skew;

    std::vector<Label*> _lables;
    int _labelNum;
    
    Label *_currentLabel;
    
    bool _toStop, _stopping;
    char _stopLetterIndex;
    float _slowdownTimer;
    
    int _effectID;
    bool _playTick;
    
    
    
    
    
        
};


#endif /* LetterWheel_hpp */
