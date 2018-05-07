//
//  Egg.hpp
//  KitkitSchool
//
//  Created by YongSoo Hong on 03/29/18
//

#ifndef Egg_hpp
#define Egg_hpp

#include "cocos2d.h"
#include <stdio.h>
#include <string>
#include "NumberCard.hpp"
USING_NS_CC;
using namespace std;

namespace MissingNumber
{
    class Egg : public Node
    {
    public:
        CREATE_FUNC(Egg);
        bool init() override;
        
    private:
        string mNumberString;
        bool mbQuestion;
        string mAnswer;
        
        Node* mEggRoot;
        Node* mNumberRoot;
        Node* mQuestionRoot;
        Sprite* mQuestion0;
        Sprite* mQuestion1;
        Sprite* mQuestionMark0;
        Sprite* mQuestionMark1;
        NumberCard* mNumber0;
        NumberCard* mNumber1;
        Node* mShiningParticleNode;
        
        void createShiningParticle();
        void setNumberString(string numberString);
        
    public:
        void initWithNumberString(int resourceIndex, string numberString, string answerString, bool isLast);
        string getNumberString(){return mNumberString;};
        bool isQuestion() {return mbQuestion;};
        void selectedQuestion(int index, bool bSelected);
        void showNumber(int index);
        string getNumberString(int index);
        string getNumberShowing(int index);
        int checkCollisionWithQuestion(Node* node);
        bool isSolved();
        void startParticle();
        void stopParticle();
        void showNumberString();
        bool hasQuestion2();
        bool hasCorrectQuestion();
        void startJumpAnimation(int audioType, int index, float duration, std::function<void()> callback);
        void startExplosionAnimation(std::function<void()> callback);
    };
    
} // end namespace

#endif /* Egg_hpp */
