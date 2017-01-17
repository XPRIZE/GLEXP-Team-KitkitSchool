//
//  ImageAndTextLayer.hpp
//  enumaXprize
//
//  Created by timewalker on 26/12/2016.
//
//

#ifndef ImageAndTextLayer_hpp
#define ImageAndTextLayer_hpp

#include <stdio.h>
#include "cocos2d.h"
#include "ui/cocosGUI.h"
#include "MultipleChoicesScene.hpp"

USING_NS_CC;
using namespace std;
using namespace cocos2d::ui;

namespace ComprehensionTest
{
    namespace MultipleChoices
    {
        class ImageAndTextLayer : public Node
        {
        public:
            CREATE_FUNC(ImageAndTextLayer);
            virtual bool init() override;
            
            void setQuestionImage(std::string imagePath);
            void setAnswers(std::vector<std::string> answerTexts);
            MultipleChoicesScene* parentScene;
            
        private:
        };
    }
}

#endif /* ImageAndTextLayer_hpp */
