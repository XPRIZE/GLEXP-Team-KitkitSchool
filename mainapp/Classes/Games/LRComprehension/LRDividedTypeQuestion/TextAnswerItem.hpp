//
//  TextAnswerItem.hpp
//  KitkitSchool
//
//  Created by timewalker on 27/12/2016.
//
//

#include "LRDividedTypeQuestionHeader.hpp"

BEGIN_NS_LR_DIVIDED_TYPE_QUESTION;

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

END_NS_LR_DIVIDED_TYPE_QUESTION;
