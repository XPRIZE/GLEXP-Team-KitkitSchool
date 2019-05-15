//
//  CCAppController.hpp
//  booktest
//
//  Created by Gunho Lee on 11/14/16.
//
//

#ifndef CCAppController_hpp
#define CCAppController_hpp

#include <stdio.h>

class CCAppController {
public:
    
    static CCAppController *sharedAppController();
    
    void handleGameQuit(bool bImmediately = false);
    void handleGameComplete(int result);
};

#endif /* CCAppController_hpp */
