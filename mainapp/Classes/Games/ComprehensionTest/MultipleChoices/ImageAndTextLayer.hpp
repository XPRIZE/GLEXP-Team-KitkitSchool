//
//  ImageAndTextLayer.hpp
//  KitkitSchool
//
//  Created by timewalker on 26/12/2016.
//
//

#ifndef ImageAndTextLayer_hpp
#define ImageAndTextLayer_hpp

#include <stdio.h>
#include "cocos2d.h"
#include "ui/CocosGUI.h"
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
            
            void setQuestionImage(std::string folder, std::string imageFile);
            void setAnswers(std::vector<std::string> answerTexts);
            MultipleChoicesScene* parentScene;
            
        private:
            Sprite* _backgroundSprite;
        };
    }
}

#endif /* ImageAndTextLayer_hpp */
