//
//  ImageAnswerItem.hpp
//  KitkitSchool
//
//  Created by timewalker on 27/12/2016.
//
//

#include "LRDividedTypeQuestionHeader.hpp"

BEGIN_NS_LR_DIVIDED_TYPE_QUESTION;

enum class ImageAnswerState
{
    Normal,
    Right,
    Wrong,
};

class ImageAnswerItem : public Node
{
public:
    bool init() override;
    ImageAnswerItem();
    CREATE_FUNC(ImageAnswerItem);
    
    bool initImage(std::string image, float marginX = 0.f, float marginY = 0.f);
    void setLetterByIndex(int index);
    std::string getId();
    std::function<void()> onCheckTarget;
    bool isCorrect();
    void setNormal();
    void setRight();
    void setWrong();
    ImageAnswerState currentState;
    
private:
    std::string _id;
    Label* _letterLabel;
    Sprite* _answerSprite;
    Scale9Sprite* _normalAnswerSprite;
    Scale9Sprite* _rightAnswerSpriteBg;
    Scale9Sprite* _rightAnswerSprite;
    Scale9Sprite* _wrongAnswerSprite;
};

END_NS_LR_DIVIDED_TYPE_QUESTION;
