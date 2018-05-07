//
//  ReorderingScene.hpp
//  KitkitSchool
//
//  Created by HyeonGyu Yu on 20/12/2016.
//
//

#ifndef ReorderingScene_hpp
#define ReorderingScene_hpp

#include <stdio.h>
#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "../ComprehensionScene.hpp"
#include "ImageBlock.hpp"
#include "TextBlock.hpp"

#include <string>

USING_NS_CC;
using namespace std;
using namespace cocos2d::ui;

namespace ComprehensionTest
{
    namespace Reordering
    {
        enum class GameType
        {
            None = -1,
            Image,
            Text,
        };
        
        class ReorderingScene : Layer
        {
        public:
            bool init() override;
            CREATE_FUNC(ReorderingScene);
            static cocos2d::Layer* createLayer(ComprehensionScene *parent);
            void onEnter() override;
            
        private:
            ComprehensionScene* _comprehensionScene;
            //string _questionText;
            vector<string> _itemVector;
            Node* _gameNode;
            vector<ImageBlock*> _imageSlots;
            vector<TextBlock*> _textSlots;
            GameType _currentType;
            
            void initData();
            void createImageSlots();
            void createTextSlots(float wrapperNodeCorrectionY, float scaleFactor);
            Node* createImageBlock(string imageFile);
            Node* createTextBlock(int index, string text, float scaleFactor);
            bool isSolved();
            void onSolve();
            void createFixedResources();
            void determineItemType();
            void drawBlocksByGameType();
            
            void writePlayLog();
            string makeWorkPath();
        };
    }
}


#endif /* ReorderingScene_hpp */
