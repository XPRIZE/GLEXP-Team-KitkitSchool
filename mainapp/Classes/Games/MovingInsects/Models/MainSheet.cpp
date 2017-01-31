//
//  MainSheet.cpp -- Main worksheet
//  TodoSchool on Sep 30, 2016
//
//  Copyright (c) 2016 Enuma, Inc. All rights reserved.
//  See LICENSE.md for more details.
//

#include "MainSheet.h"


BEGIN_NS_MOVINGINSECTS

size_t MainSheet::size() const {
    return Works.size();
}

size_t MainSheet::beginWorkID() const {
    return Works.begin_index();
}

size_t MainSheet::endWorkID() const {
    return Works.end_index();
}

MainWork& MainSheet::workByID(size_t WorkID) {
    return Works.at(WorkID);
}

istream& operator>>(istream& IS, MainSheet& Sheet) {
    size_t WorkID;
    
    if (IS >> WorkID) {
        Sheet.Works.resize_for_index(WorkID);
        auto& Work = Sheet.workByID(WorkID);
        IS >> Work;
    }

    return IS;
}

END_NS_MOVINGINSECTS
