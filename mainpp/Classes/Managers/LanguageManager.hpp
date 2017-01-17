//
//  LanguageManager.hpp
//  enumaXprize
//
//  Created by Sungwoo Kang on 6/30/16.
//
//

#ifndef LanguageManager_hpp
#define LanguageManager_hpp

#include <string>
#include <map>
#include "Common/Basic/SoundEffect.h"




class LanguageManager {
    
    
    static LanguageManager* _instance;
public:
    enum LanguageType {
        ENGLISH = 0,
        SWAHILI
    };

    static LanguageManager* getInstance();
    void init();
    void setCurrentLanguage(LanguageType type);
    
    bool isEnglish() { return getCurrentLanguage()==ENGLISH; }
    bool isSwahili() { return getCurrentLanguage()==SWAHILI; }
    
    LanguageType getCurrentLanguage();
    std::string getCurrentLanguageString();
    std::string getCurrentLanguageCode();
    std::string getCurrentLanguageTag();
    
    
    std::string soundPathForWordFile(std::string& wordFile);
    
    std::string getLocalizedString(std::string str);
    
    
private:
    LanguageType _langType;
    
    std::map<std::string, std::string> _localizationMap;
    void initLocalizationMap();
    
    
    
    
};

#endif /* LanguageManager_hpp */
