//
//  LanguageManager.hpp
//  KitkitSchool
//
//  Created by Sungwoo Kang on 6/30/16.
//
//

#ifndef LanguageManager_hpp
#define LanguageManager_hpp

#include <string>
#include <vector>
#include <map>
#include "Common/Basic/SoundEffect.h"




class LanguageManager {
    
    
    static LanguageManager* _instance;
public:
    enum LocaleType {
        sw_TZ = 0,
        en_US,
        en_KE,
        en_GB,
        LocaleType_MAX
    };

    static LanguageManager* getInstance();
    void init();
    
    LocaleType convertLocaleCodeToType(std::string localeCode);
    std::string convertLocaleTypeToCode(LocaleType localeType);
    
    void setCurrentLocale(LocaleType type);
    LocaleType getCurrentLocaleType();
    LocaleType findNextLocale();

    
    std::string getCurrentLanguageCode();
    std::string getCurrentLanguageTag();
    std::string getCurrentLocaleCode();
    
    
    std::string soundPathForWordFile(std::string& wordFile);
    
    std::string getLocalizedString(std::string str);
    
    std::string findLocalizedResource(std::string path);
    
    bool isEnglish() { return getCurrentLanguageCode()=="en"; }
    bool isSwahili() { return getCurrentLanguageCode()=="sw"; }
    
    std::vector<std::string> getLocalizationMapKeys();
    
private:
    LocaleType _currentLocale;

    
    std::vector<std::string> _localizedResourcePaths;
    std::vector<LocaleType> _supportedLocales;
    
    
    std::map<std::string, std::string> _localizationMap;
    std::map<std::string, std::string> _localizationMapEnglish;
    std::map<std::string, std::string> _localizationMapSwahili;
    
    void initLocalizationMap();
    
    
    
    
};

#endif /* LanguageManager_hpp */
