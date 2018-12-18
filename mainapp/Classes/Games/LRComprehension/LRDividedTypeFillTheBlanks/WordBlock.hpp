//
//  WordBlock.hpp
//  KitkitSchool
//
//  Created by Gunho Lee on 12/18/16.
//
//

#include "LRDividedTypeFillTheBlanksHeader.hpp"

BEGIN_NS_LR_DIVIDED_TYPE_FILL_THE_BLANKS;

class WordBlock : public Node
{
public:
    virtual bool init() override;
    CREATE_FUNC(WordBlock);
    
public:
    static Size wordBlockSize;
    static void setWordBlockWidth(float width);
    
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

END_NS_LR_DIVIDED_TYPE_FILL_THE_BLANKS;
