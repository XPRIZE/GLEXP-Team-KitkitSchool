//
//  LanguageManager.cpp
//  enumaXprize
//
//  Created by Sungwoo Kang on 6/30/16.
//
//

#include "LanguageManager.hpp"
#include "cocos2d.h"

USING_NS_CC;

LanguageManager* LanguageManager::_instance = 0;

LanguageManager* LanguageManager::getInstance()
{
    if(!_instance) {
        _instance = new LanguageManager();
        _instance->init();
    }
    return _instance;
}


void LanguageManager::init()
{
#if false
    auto defaultLang = LanguageType::ENGLISH;
#endif
    auto defaultLang = LanguageType::SWAHILI;

    _langType = static_cast<LanguageType>(UserDefault::getInstance()->getIntegerForKey("Language", defaultLang));

    

    
    initLocalizationMap();
    
    
    
}

void LanguageManager::setCurrentLanguage(LanguageManager::LanguageType type)
{
    _langType = type;
    UserDefault::getInstance()->setIntegerForKey("Language", static_cast<int>(_langType));
    UserDefault::getInstance()->flush();
}

LanguageManager::LanguageType LanguageManager::getCurrentLanguage()
{
    return _langType;
}

std::string LanguageManager::getCurrentLanguageString()
{
    std::string lang="";
    switch (LanguageManager::getInstance()->getCurrentLanguage()) {
        case LanguageManager::ENGLISH:
            lang = "English";
            break;
            
        case LanguageManager::SWAHILI:
            lang = "Swahili";
            break;
        default:
            break;
    }
    
    return lang;
    
}

std::string LanguageManager::getCurrentLanguageCode()
{
    switch (LanguageManager::getInstance()->getCurrentLanguage()) {
        case LanguageManager::ENGLISH: return "en"; break;
        case LanguageManager::SWAHILI: return "sw"; break;
        default:
            return "";
            break;
    }

}

std::string LanguageManager::getCurrentLanguageTag()
{
    // NB(xenosoz, 2016): Please refer to IETF language tag.
    switch (_langType) {
        case ENGLISH: return "en-US";
        case SWAHILI: return "sw-TZ";
    }

    return "";
}

std::string LanguageManager::soundPathForWordFile(std::string& wordFile)
{
    std::string folder;
    
    switch (_langType) {
        case ENGLISH: folder = "Common/Sounds/Pam.en_US/"; break;
        case SWAHILI: folder = "Common/Sounds/Imma.sw_TZ/"; break;
    }
    
    std::string path = folder+"LetterNames/"+wordFile;
    if (cocos2d::FileUtils::getInstance()->isFileExist(path)) return path;
    path = folder+"Words/"+wordFile;
    if (cocos2d::FileUtils::getInstance()->isFileExist(path)) return path;
    
    return "";
    
}

std::string LanguageManager::getLocalizedString(std::string str)
{
    
    switch (_langType) {
        case ENGLISH: return str;  break;
        case SWAHILI: return _localizationMap[str]; break;
    }
    return "";
    
}

void LanguageManager::initLocalizationMap()
{
    _localizationMap["Stop the test"] = "Stop the test";
    _localizationMap["Go back to test"] = "Go back to test";
    
    
    _localizationMap["Great!"] = "Vizuri!";
    
    _localizationMap["Are you ready for"] = "Je, uko tayari kwa";
    _localizationMap["Prove it!"] = "Thibitisha!";
    _localizationMap["Try and get 8 questions correct!"] = "Jaribu na toa majibu sahihi manane!";
    _localizationMap["Challenge"] = "Jaribu";
    _localizationMap["Congratulations!"] = "Hongera!";
    _localizationMap["You passed!"] = "Umefaulu!";
    _localizationMap["You failed"] = "Umeshindwa";
    _localizationMap["Practice more and try again later."] = "Fanya mazoezi zaidi na rudi baadae.";
    _localizationMap["Success!"] = "Mafanikio!";
    _localizationMap["You are not ready."] = "Hauko tayari.";
    _localizationMap["You need more practice."] = "Unahitaji mazoezi zaidi.";
    _localizationMap["Welcome!"] = "Karibu!";
    
    _localizationMap["Start"] = "Anza";
    _localizationMap["Next"] = "Nenda mbele";
    _localizationMap["Back"] = "Rudi nyuma";
    _localizationMap["OK"] = "OK";
    _localizationMap["Error"] = "Hitilafu";
    _localizationMap["Enter"] = "Chomeka";
    _localizationMap["Clear"] = "Futa";
}
