//
//  LRTitlePageView.hpp
//  KitkitSchool
//
//  Created by HyeonGyu Yu on 12/10/2018.
//

#ifndef LRTitlePageView_hpp
#define LRTitlePageView_hpp

#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include <stdio.h>
#include "LRComprehensionScene.hpp"

USING_NS_CC;
using namespace ui;
using namespace std;

class LRTitlePageView : public Layer
{
public:
    bool init(string titleContent, LRComprehensionScene* parent);
    
    virtual void onEnter() override;
    
    LRTitlePageView();
    static LRTitlePageView* create(string titleContent, LRComprehensionScene* parent);
    
    //
private:
    string _titleContent;
    Label* _titleLabel;
    Node *_gameNode;
    LRComprehensionScene* _comprehensionScene;
};

#endif /* LRTitlePageView_hpp */
