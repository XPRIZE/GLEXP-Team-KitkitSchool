//
//  TextBlock.hpp
//  KitkitSchool
//
//  Created by HyeonGyu Yu on 20/12/2016.
//
//

#pragma once

#include "LRDividedTypeReorderingHeader.hpp"

BEGIN_NS_LR_DIVIDED_TYPE_REORDERING;

class TextBlock : public Node
{
public:
    TextBlock();
    CREATE_FUNC(TextBlock);
    virtual bool init() override;
    
    static Size wordBlockSize;
    
    void initAsSlot(std::string text);
    void initAsBlock(std::string text);
    
    std::string getValue();
    std::string getSolutionValue();
    
    void returnToOrigin();
    
    void setPicked(bool picked);
    void setSnapped(bool snapped);
    
    std::function<void()> onCheckTarget;
    
    TextBlock* _pair; // 블록은 들어간 칸, 빈칸은 끼워진 블록을 참조합니다.
    
protected:
    bool _bSlot;
    std::string _text;
    std::string _solutionText;
    Point _originPos;
    
    Node *_face, *_shadow;
    bool _snapped;
};

END_NS_LR_DIVIDED_TYPE_REORDERING;
