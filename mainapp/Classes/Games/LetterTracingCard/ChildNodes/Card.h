//
//  Card.h
//  enumaXprize
//
//  Created by HyeonGyu Yu on 9/5/17.
//
//

#ifndef Card_h
#define Card_h

#include "../Utils/MainNS.h"
#include "../Models/Problem.h"
#include <Common/Controls/TraceField/TraceField.h>

BEGIN_NS_LETTERTRACINGCARD

class Card : public Node
{
public:
    CREATE_FUNC(Card);
    bool init() override;
    Card();
    
public:
    void drawCardWithProblem(Problem* problem);
    bool isComplete;
    function<void()> onComplete;
    tracefield::TraceField* traceWord;
    ImageView* frontFace;
    ImageView* backFace;
    Label* label;
    void FlipToBack();
private:
    string word;
};

END_NS_LETTERTRACINGCARD

#endif /* Card_h */
