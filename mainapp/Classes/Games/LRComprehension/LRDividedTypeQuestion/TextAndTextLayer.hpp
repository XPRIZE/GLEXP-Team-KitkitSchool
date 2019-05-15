//
//  TextAndTextLayer.hpp
//  KitkitSchool
//
//  Created by timewalker on 26/12/2016.
//
//

#include "LRDividedTypeQuestionHeader.hpp"
#include "LRDividedTypeQuestionScene.hpp"

BEGIN_NS_LR_DIVIDED_TYPE_QUESTION;

class TextAndTextLayer : public Node
{
public:
    CREATE_FUNC(TextAndTextLayer);
    virtual bool init() override;
    
    void setAnswers(std::vector<std::string> answerTexts);
    LRDividedTypeQuestionScene* parentScene;
};

END_NS_LR_DIVIDED_TYPE_QUESTION;
