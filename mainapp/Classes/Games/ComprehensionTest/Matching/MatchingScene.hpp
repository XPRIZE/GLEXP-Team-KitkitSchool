//
//  Matching.hpp
//  enumaXprize
//
//  Created by timewalker on 05/12/2016.
//
//

#ifndef MatchingScene_hpp
#define MatchingScene_hpp

#include <stdio.h>
#include "cocos2d.h"
#include "ui/cocosGUI.h"
#include "../ComprehensionScene.hpp"
#include "ProblemData.hpp"

USING_NS_CC;
using namespace cocos2d::ui;

namespace ComprehensionTest
{
    namespace Matching
    {
        class ImageObject;
        class TextObject;
        
        enum class DotDirection
        {
            Left,
            Right,
        };

        enum class ObjectLocation
        {
            LeftSide,
            RightSide,
        };

        class MatchingScene : Layer
        {
        public:
            bool init() override;
            CREATE_FUNC(MatchingScene);
            static cocos2d::Layer* createLayer(ComprehensionScene *parent);
            MatchingScene();
            void onEnter() override;
            
            void initData();
            void createFixedResources();
            
            bool onTouchBegan(cocos2d::Touch *touch, cocos2d::Event *event) override;
            void onTouchMoved(cocos2d::Touch *touch, cocos2d::Event *event) override;
            void onTouchEnded(cocos2d::Touch *touch, cocos2d::Event *event) override;
            
            std::vector<ImageObject*> _imageObjectVector;
            std::vector<TextObject*> _textObjectVector;
            
        private:
            Node *_gameNode;
            LayerColor* _leftLayer;
            LayerColor* _rightLayer;
            bool _bDrag;
            std::string _selectedId;
            ObjectLocation _selectedLocation;
            Vec2 _selectedStarPoint;
            Vec2 _selectedEndPoint;
            DrawNode* _drawNodeLine;
            DrawNode* _drawNodeCircle;
            EventListenerTouchOneByOne *_eventListener;
            ComprehensionScene *_comprehensionScene;
            
            void createLeftLayer();
            void createRightLayer();
            
            void canceled();
            void matched(Vec2 endPoint);
            bool isSolved();
            
            void onStart();
            void onSolve();
        };

    }  // namespace Matching
}  // namespace ComprehensionTest

#endif /* MatchingScene_hpp */
