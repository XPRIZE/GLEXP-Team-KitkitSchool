//
//  FreightCar.hpp
//  KitkitSchool
//
//  Created by JungJaehun on 08/09/2017.
//
//

#pragma once
#include "../PatternTrainHeader.h"

BEGIN_NS_PATTERNTRAIN;

class Box;
class Slot;

class FreightCar : public Node {
public:
    enum carType {
        FOR2BOX,
        FOR3BOX
    } _carType;
    
    CREATE_FUNC(FreightCar);
    virtual bool init() override;
    void setType(int carLength);
    void loadBox(Box *newBox, int index);
    vector<Slot*> _slotsInFreightCar;
    
private:
    int _spacePointer;
    
};


END_NS_PATTERNTRAIN;
