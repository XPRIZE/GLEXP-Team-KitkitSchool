//
//  FillTheBlanksScene.hpp
//  KitkitSchool
//
//  Created by Gunho on 13/12/2016.
//
//

#ifndef FillTheBlanksScene_hpp
#define FillTheBlanksScene_hpp

#include <stdio.h>
#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "CompTestFTBProblemData.hpp"
#include "../ComprehensionScene.hpp"
#include "WordBlock.hpp"

#include <string>

USING_NS_CC;

using namespace cocos2d::ui;

namespace ComprehensionTest
{
    namespace FillTheBlanks
    {
        class FillTheBlanksScene : Layer
        {
        public:
            bool init() override;
            CREATE_FUNC(FillTheBlanksScene);
            static cocos2d::Scene* createScene();
            static cocos2d::Layer* createLayer(ComprehensionScene *parent);
            FillTheBlanksScene();
            void onEnter() override;
            void onExit() override;
            bool isSolved();
            void onSolve();
            
        private:
            ComprehensionScene* _comprehensionScene;

            Size _maxWordSize;
            float _pageScale;
            
            Node *_gameNode;
            vector<WordBlock*> _slots;
            vector<Point> _randomPoints;
            
            Size calculateMaxWordSize();
            Node* createPage(float pageWidth);
            Node* createWordBlock(std::string word);
   
            void initData();
            void drawFixedResources();
            void createRandomAreas();
            
            void writePlayLog();
            std::string makeWorkPath();
        };
    }
}

#endif /* FillTheBlanksScene_hpp */
