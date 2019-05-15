//
//  TextAndTextLayer.hpp
//  KitkitSchool
//
//  Created by timewalker on 26/12/2016.
//
//

#include "LRAllInOneTypeQuestionHeader.hpp"
#include "LRAllInOneTypeQuestionScene.hpp"

BEGIN_NS_LR_ALLINONE_TYPE_QUESTION;

class TextAndTextLayer : public Node
{
public:
    CREATE_FUNC(TextAndTextLayer);
    virtual bool init() override;

    void setGuideView(QuestionType questionType, string textOrSoundPath);
    void setQuestionAnswers(std::string questionText, std::string questionSoundPath, std::vector<std::string> answerTexts);
    Node* createMultiLine(string text, float fontSize, Size dim, string fontName, Color4B fontColor, float lineSpacingRatio = 1.f);
    Node* createWord(string word, float fontSize, string fontName, Color4B fontColor);
    LRAllInOneTypeQuestionScene* parentScene;
    
private:
    QuestionType _questionType;
    string _guideTextOrSoundPath;
    //Button* _questionSoundButton;
    LRSoundButton* _questionSoundButton;
};

END_NS_LR_ALLINONE_TYPE_QUESTION;
