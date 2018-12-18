//
//  KitkitGameInfo.cpp
//  KitkitSchool
//
//  Created by Gunho Lee on 7/14/18.
//

#include "KitkitGameInfo.hpp"

std::map<std::string, KitkitGameInfo*> KitkitGameInfo::_infos;


std::vector<KitkitGameInfo*> KitkitGameInfo::getRegisteredGames()
{
    std::vector<KitkitGameInfo*> v;
    for (auto info : _infos) {
        v.push_back(info.second);
    }
    return v;
}
