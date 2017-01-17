//
//  TextAndTextLayer.hpp
//  enumaXprize
//
//  Created by timewalker on 26/12/2016.
//
//

#ifndef TextAndTextLayer_hpp
#define TextAndTextLayer_hpp

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
        class TextAndTextLayer : public Node
        {
        public:
            CREATE_FUNC(TextAndTextLayer);
            virtual bool init() override;
            
            void setAnswers(std::vector<std::string> answerTexts);
            MultipleChoicesScene* parentScene;
        };
    }
}

#endif /* TextAndTextNode_hpp */
