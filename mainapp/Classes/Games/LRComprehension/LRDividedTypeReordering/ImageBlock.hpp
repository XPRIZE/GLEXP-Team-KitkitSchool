//
//  ImageBlock.hpp
//  KitkitSchool
//
//  Created by HyeonGyu Yu on 20/12/2016.
//
//

#pragma once

#include "LRDividedTypeReorderingHeader.hpp"

BEGIN_NS_LR_DIVIDED_TYPE_REORDERING;

class ImageBlock : public Node
{
public:
    ImageBlock();
    CREATE_FUNC(ImageBlock);
    virtual bool init() override;
    
    static Size wordBlockSize;
    
    void initAsSlot(std::string imageFile);
    void initAsBlock(std::string imageFile);
    
    std::string getValue();
    std::string getSolutionValue();
    
    void returnToOrigin();
    
    void setPicked(bool picked);
    void setSnapped(bool snapped);
    
    std::function<void()> onCheckTarget;
    
    ImageBlock* _pair; // 블록은 들어간 칸, 빈칸은 끼워진 블록을 참조합니다.
    
private:
    bool _bSlot;
    std::string _imageFile;
    std::string _solutionImageFile;
    Point _originPos;
    Point _faceOriginPos;
    Node *_background, *_face, *_shadow;
    bool _snapped;
};

END_NS_LR_DIVIDED_TYPE_REORDERING;
