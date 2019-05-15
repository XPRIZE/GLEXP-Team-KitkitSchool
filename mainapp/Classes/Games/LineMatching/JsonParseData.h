//
//  JsonParseData.h
//  todomath
//
//  Created by KimDonggyu on 2016. 7. 20..
//
//

#ifndef JsonParseData_h
#define JsonParseData_h

#include <stdio.h>
#include "cocos2d.h"
#include "3rdParty/json/json.h"

USING_NS_CC;

class JsonParseData : public Ref {
    
public:
    JsonParseData(){};
    ~JsonParseData(){};
    
public:
    Json::Value m_setJson;
    Json::Value m_categoryJson;
    Json::Value m_colorJson;
};

#endif /* JsonParseData_h */
