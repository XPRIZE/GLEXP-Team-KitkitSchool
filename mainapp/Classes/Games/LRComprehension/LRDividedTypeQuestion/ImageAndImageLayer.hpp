//
//  ImageAndImageLayer.hpp
//  KitkitSchool
//
//  Created by timewalker on 26/12/2016.
//
//

#include "LRDividedTypeQuestionHeader.hpp"
#include "LRDividedTypeQuestionScene.hpp"

BEGIN_NS_LR_DIVIDED_TYPE_QUESTION;

class ImageAndImageLayer : public Node
{
public:
    CREATE_FUNC(ImageAndImageLayer);
    virtual bool init() override;
    
    void setQuestionImage(std::string imageFile);
    void setAnswers(std::vector<std::string> imageFiles);
    
    LRDividedTypeQuestionScene* parentScene;
    
private:
};

END_NS_LR_DIVIDED_TYPE_QUESTION;
