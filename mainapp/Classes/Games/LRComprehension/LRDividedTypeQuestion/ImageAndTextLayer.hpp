//
//  ImageAndTextLayer.hpp
//  KitkitSchool
//
//  Created by timewalker on 26/12/2016.
//
//

#include "LRDividedTypeQuestionHeader.hpp"
#include "LRDividedTypeQuestionScene.hpp"

BEGIN_NS_LR_DIVIDED_TYPE_QUESTION;

class ImageAndTextLayer : public Node
{
public:
    CREATE_FUNC(ImageAndTextLayer);
    virtual bool init() override;
    
    void setQuestionImage(std::string folder, std::string imageFile);
    void setAnswers(std::vector<std::string> answerTexts);
    
    LRDividedTypeQuestionScene* parentScene;
    
private:
    Sprite* _backgroundSprite;
};

END_NS_LR_DIVIDED_TYPE_QUESTION;
