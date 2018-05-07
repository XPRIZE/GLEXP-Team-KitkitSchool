//
//  TextAnswerItem.hpp
//  KitkitSchool
//
//  Created by timewalker on 27/12/2016.
//
//

#ifndef TextAnswerItem_hpp
#define TextAnswerItem_hpp

#include <stdio.h>
#include "cocos2d.h"

USING_NS_CC;

namespace ComprehensionTest
{
    namespace MultipleChoices
    {
        enum class TextAnswerState
        {
            Normal,
            Right,
            Wrong,
        };
        
        class TextAnswerItem : public Node
        {
        public:
            bool init() override;
            TextAnswerItem();
            CREATE_FUNC(TextAnswerItem);
            
            TextAnswerState currentState;
            std::string getId();
            std::function<void()> onCheckTarget;
            
            void initText(std::string text, bool bLongText = true);
            bool isCorrect();
            void setState(TextAnswerState state);
            void setLetterByIndex(int index);
            
        private:
            std::string _id;
            Sprite* _checkSprite;
            
            Label* _normalLetterLabel;
            Label* _wrongLetterLabel;
            
            Label* _normalAnswerLabel;
            Label* _rightAnswerLabel;
            Label* _wrongAnswerLabel;
            
            Sprite* _normalAnswerSprite;
            Sprite* _rightAnswerSprite;
            Sprite* _wrongAnswerSprite;
        };
    }
}

#endif /* TextAnswerItem_hpp */
