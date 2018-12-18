//
//  TextAndImageLayer.hpp
//  KitkitSchool
//
//  Created by timewalker on 26/12/2016.
//
//

#include "LRDividedTypeQuestionHeader.hpp"
#include "LRDividedTypeQuestionScene.hpp"

BEGIN_NS_LR_DIVIDED_TYPE_QUESTION;

class TextAndImageLayer : public Node
{
public:
    CREATE_FUNC(TextAndImageLayer);
    virtual bool init() override;
    
    void setAnswers(std::vector<std::string> imageFiles);
    LRDividedTypeQuestionScene* parentScene;
};

END_NS_LR_DIVIDED_TYPE_QUESTION;
