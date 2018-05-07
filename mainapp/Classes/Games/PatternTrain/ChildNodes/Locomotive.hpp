//
//  Locomotive.hpp
//  KitkitSchool
//
//  Created by JungJaehun on 08/09/2017.
//
//

#pragma once
#include "../PatternTrainHeader.h"

BEGIN_NS_PATTERNTRAIN;

class Locomotive : public Node {
public:
    CREATE_FUNC(Locomotive);
    virtual bool init() override;
};

END_NS_PATTERNTRAIN;
