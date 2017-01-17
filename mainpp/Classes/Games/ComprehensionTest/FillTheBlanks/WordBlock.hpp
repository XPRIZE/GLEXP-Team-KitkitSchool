//
//  WordBlock.hpp
//  enumaXprize
//
//  Created by Gunho Lee on 12/18/16.
//
//

#ifndef WordBlock_hpp
#define WordBlock_hpp

#include <stdio.h>
#include "cocos2d.h"
#include "ui/cocosGUI.h"

#include <vector>
#include <string>
#include <functional>

USING_NS_CC;
using namespace cocos2d::ui;
using namespace std;

class WordBlock : public Node
{

    
public:
    static Size wordBlockSize;
    
    static void setWordBlockWidth(float width);
    
    CREATE_FUNC(WordBlock);
    
    virtual bool init() override;
    void initBlank(string word);
    void initWord(string word);
    
    
    void setPicked(bool picked);
    void setSnapped(bool snapped);
    
    void returnToOrigin();
    
    std::function<void()> onCheckTargetBegan;
    std::function<void()> onCheckTargetEnd;
    
    string _word;
    WordBlock *_pair; // 블록은 들어간 칸, 빈칸은 끼워진 블록을 참조합니다.
    
protected:
    Node *_face, *_shadow;
    bool _snapped;
    Point _originPos;
    

    
    
    
    
};


#endif /* WordBlock_hpp */
