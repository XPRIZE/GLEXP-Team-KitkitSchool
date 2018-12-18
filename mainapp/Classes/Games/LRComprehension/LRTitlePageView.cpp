//
//  LRTitlePageView.cpp
//  KitkitSchool
//
//  Created by HyeonGyu Yu on 12/10/2018.
//

#include "LRTitlePageView.hpp"
#include "Utils/TodoUtil.h"

LRTitlePageView::LRTitlePageView()
{
    
}

LRTitlePageView* LRTitlePageView::create(string titleContent, LRComprehensionScene* parent)
{
    LRTitlePageView *pRet = new(std::nothrow) LRTitlePageView();
    if (pRet && pRet->init(titleContent, parent))
    {
        pRet->autorelease();
        return pRet;
    }
    else
    {
        delete pRet;
        pRet = nullptr;
        return nullptr;
    }
}


bool LRTitlePageView::init(string titleContent, LRComprehensionScene* parent)
{
    if (!Layer::init()) return false;
    
    _titleContent = titleContent;
    _comprehensionScene = parent;
    
    _gameNode = Node::create();
    _gameNode->setContentSize(Size(2560, 1800));
    _gameNode->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
    _gameNode->setPosition(getContentSize().width / 2, 0.f);
    addChild(_gameNode);
    
    auto title = TodoUtil::createLabel(titleContent, 140, Size::ZERO, FONT_ANDIKA_BOLD, Color4B(77,77,77,255));
    title->setPosition(_gameNode->getContentSize()/2);
    _gameNode->addChild(title);
    
    auto line = Sprite::create("lrcomprehension/common/comprehention-question-cover-highlight.png");
    line->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
    line->setPosition(Vec2(title->getContentSize().width/2, 38.f));
    title->addChild(line);
    
    return true;
}

void LRTitlePageView::onEnter()
{
    Layer::onEnter();
    _comprehensionScene->onSolveWithDelay(2.f, false);
}
