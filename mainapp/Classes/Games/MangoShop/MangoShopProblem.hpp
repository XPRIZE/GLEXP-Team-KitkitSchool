//
//  Problem.hpp
//  KitkitSchool-mobile
//
//  Created by HyeonGyu Yu on 3/14/18.
//

#ifndef Problem_hpp
#define Problem_hpp

#include <stdio.h>
#include <string>

namespace MangoShop
{
    class Problem
    {
    public:
        std::string lang;
        std::string level;
        std::string worksheet;
        std::string problem;
        std::string category;
        std::string question;
        std::string answer;
        std::string isRandom;
    };
}
#endif /* Problem_hpp */
