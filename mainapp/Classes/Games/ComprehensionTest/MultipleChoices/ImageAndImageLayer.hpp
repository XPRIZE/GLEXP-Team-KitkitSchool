//
//  ImageAndImageLayer.hpp
//  KitkitSchool
//
//  Created by timewalker on 26/12/2016.
//
//

#ifndef ImageAndImageLayer_hpp
#define ImageAndImageLayer_hpp

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
        class ImageAndImageLayer : public Node
        {
        public:
            CREATE_FUNC(ImageAndImageLayer);
            virtual bool init() override;
            
            void setQuestionImage(std::string folder, std::string imageFile);
            void setAnswers(std::string folder, std::vector<std::string> imageFiles);
            
            MultipleChoicesScene* parentScene;
            
        private:
        };
    }
}

#endif /* ImageAndImageLayer_hpp */
