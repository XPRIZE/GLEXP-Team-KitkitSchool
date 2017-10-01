//
//  WordItem.hpp
//  enumaXprize
//
//  Created by Gunho Lee on 12/18/16.
//
//

#ifndef WordItem_hpp
#define WordItem_hpp

#include <stdio.h>
#include "cocos2d.h"
#include "ui/CocosGUI.h"

#include <vector>
#include <string>
#include <functional>

USING_NS_CC;
using namespace cocos2d::ui;
using namespace std;

class WordItem : public Node
{
public:
    static Size wordItemSize;
    
    static void setWordItemWidth(float width);
    
    CREATE_FUNC(WordItem);
    
    virtual bool init() override;
    void initBlank(string word);
    void initWord(string word);
    
    void setPicked(bool picked);
    void setSnapped(bool snapped);
    
    void returnToOrigin();
    
    std::function<void()> onCheckTargetBegan;
    std::function<void()> onCheckTargetEnded;
    
    string _word;
    WordItem *_pair; // 블록은 들어간 칸, 빈칸은 끼워진 블록을 참조합니다.
    
protected:
    Node *_face, *_shadow;
    bool _snapped;
    Point _originPos;
};


#endif /* WordItem_hpp */
