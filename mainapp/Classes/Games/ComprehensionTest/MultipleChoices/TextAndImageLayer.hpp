//
//  TextAndImageLayer.hpp
//  enumaXprize
//
//  Created by timewalker on 26/12/2016.
//
//

#ifndef ImageAndTextNode_hpp
#define ImageAndTextNode_hpp

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
        class TextAndImageLayer : public Node
        {
        public:
            CREATE_FUNC(TextAndImageLayer);
            virtual bool init() override;
            
            void setAnswers(std::vector<std::string> imagePaths);
            MultipleChoicesScene* parentScene;
        };
    }
}

#endif /* ImageAndTextNode_hpp */
