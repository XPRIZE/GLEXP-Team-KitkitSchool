//
//  MultipleChoicesScene.hpp
//  KitkitSchool
//
//  Created by timewalker on 26/12/2016.
//
//

#ifndef MultipleChoicesScene_hpp
#define MultipleChoicesScene_hpp

#include <stdio.h>
#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "../ComprehensionScene.hpp"

#include <string>

USING_NS_CC;
using namespace std;
using namespace cocos2d::ui;

namespace ComprehensionTest
{
    namespace MultipleChoices
    {
        static Size fixedDeviceSize = Size(2560, 1800);

        enum class QuestionType
        {
            None = -1,
            TextQuestionImageAnswer,
            TextQuestionTextAnswer,
            ImageQuestionTextAnswer,
            ImageQuestionImageAnswer,
        };
        
        class MultipleChoicesScene : Layer
        {
        public:
            bool init() override;
            CREATE_FUNC(MultipleChoicesScene);
            static cocos2d::Layer* createLayer(ComprehensionScene *parent);
            MultipleChoicesScene();
            void onEnter() override;
            
            std::vector<std::pair<std::string, bool>> answerPairVector;
            
            bool isSolved();
            void onSolve();
            bool isCorrect(std::string answer);
            void solve(std::string answer);
            int getSolvedCount();
            void determineItemType();
            
        private:
            Node *_gameNode;
            ComprehensionScene* _comprehensionScene;
            QuestionType _currentType;
            
            Vec2 _layerPosition;
            std::string _questionText;
            float _questionLabelFontSize;
            std::string _questionImageFile;
            std::vector<std::string> _answers;
            std::vector<std::string> _solutions;
            
            void initData();
            void createFixedResources();
            Node* createMultiLine(string text, float fontSize, Size dim, Color4B fontColor);
            Node* createWord(string word, float fontSize, Color4B fontColor);
            std::string lineWrappnig(std::string original);
            
            Node* questionLabel;
            Vec2 _questionLabelPosition;
            
            std::string makeWorkPath() const;
        };
    }
}

#endif /* MultipleChoicesScene_hpp */
