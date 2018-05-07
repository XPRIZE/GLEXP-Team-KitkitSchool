//
//  LanguageManager.cpp
//  KitkitSchool
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
    //auto defaultLang = LanguageType::SWAHILI;
    auto defaultLang = "sw-TZ";//UserDefault::getInstance()->getStringForKey("appLanguage", "en") == "en" ? "en-US" : "sw-TZ";

    auto localeCode = defaultLang;//UserDefault::getInstance()->getStringForKey("LocaleCode", defaultLang);

    auto localeType = convertLocaleCodeToType(localeCode);
    if (localeType>=LocaleType_MAX) localeType = sw_TZ;
    
    
    _supportedLocales.clear();
    for (int i=0; i<LocaleType_MAX; i++) {
        LocaleType l = (LocaleType)i;
        _supportedLocales.push_back(l);
        
    }
    
    if (std::find(_supportedLocales.begin(), _supportedLocales.end(), localeType)==_supportedLocales.end()) {
        localeType = _supportedLocales.front();
    }
    
    

    setCurrentLocale(localeType);

    
    initLocalizationMap();
    
    
    
}

LanguageManager::LocaleType LanguageManager::convertLocaleCodeToType(std::string localeCode)
{
    if (localeCode.length()<5) return LocaleType_MAX;
    auto lang = localeCode.substr(0, 2);
    auto region = localeCode.substr(3, 2);
    
    if (lang=="en") {
        if (region=="US") return en_US;
        if (region=="KE") return en_KE;
        if (region=="GB") return en_GB;
    } else if (lang=="sw") {
        if (region=="TZ") return sw_TZ;
    }
    
    return LocaleType_MAX;
}

std::string LanguageManager::convertLocaleTypeToCode(LanguageManager::LocaleType localeType)
{
    switch (localeType) {
        case en_US: return "en-US"; break;
        case en_GB: return "en-GB"; break;
        case en_KE: return "en-KE"; break;
        case sw_TZ: return "sw-TZ"; break;
        default: break;
    }
    
    return "sw-TZ";
    
}

void LanguageManager::setCurrentLocale(LocaleType type)
{
    bool dirty = false;
    if (_currentLocale != type) dirty = true;
    
    
    _currentLocale = type;

    if (dirty) {
        auto langCode = convertLocaleTypeToCode(type);
        UserDefault::getInstance()->setStringForKey("LocaleCode", langCode);
        UserDefault::getInstance()->flush();
    }
    
    _localizedResourcePaths.clear();
    switch (_currentLocale) {
        default:
            CCLOGERROR("No proper language is found in %s", __PRETTY_FUNCTION__);
            // fall through
        case sw_TZ: _localizedResourcePaths = { "sw-TZ" }; break;
        case en_US: _localizedResourcePaths = { "en-US" }; break;
        case en_GB: _localizedResourcePaths = { "en-GB", "en-US" }; break;
        case en_KE: _localizedResourcePaths = { "en-KE", "en-US" }; break;
            
    }

    
    std::vector<std::string> paths = {};
    
    for (auto p : _localizedResourcePaths) {
        auto localizedPath = "Localized/"+p;
        paths.push_back(localizedPath);
        paths.push_back(localizedPath+"/Games");
    }
    paths.push_back("Games");
    
    
    FileUtils::getInstance()->setSearchPaths(paths);
    
    
}

LanguageManager::LocaleType LanguageManager::getCurrentLocaleType()
{
    return _currentLocale;
}

LanguageManager::LocaleType LanguageManager::findNextLocale()
{
    LocaleType next = (LocaleType)((int)_currentLocale+1);
  
    while (next!=_currentLocale) {
        if (std::find(_supportedLocales.begin(), _supportedLocales.end(), next)!=_supportedLocales.end()) return next;
        next = (LocaleType)((int)next+1);
        if (next>=LocaleType_MAX) next = (LocaleType)0;
        
    }
    
    return _currentLocale;
}


std::string LanguageManager::getCurrentLanguageCode()
{
    
    std::string langCode = convertLocaleTypeToCode(_currentLocale);
    return langCode.substr(0, 2);
    
}

std::string LanguageManager::getCurrentLanguageTag()
{
    return getCurrentLocaleCode();
}

std::string LanguageManager::getCurrentLocaleCode()
{
    std::string langCode=convertLocaleTypeToCode(_currentLocale);
    return langCode;
}


std::string LanguageManager::soundPathForWordFile(std::string& wordFile)
{
    std::string folder;
    
//    switch (_langType) {
//        case ENGLISH: folder = "Common/Sounds/Pam.en_US/"; break;
//        case SWAHILI: folder = "Common/Sounds/Imma.sw_TZ/"; break;
//    }
    
    std::string path = findLocalizedResource("LetterVoice/"+wordFile);
    if (path!="") return path;
    path = findLocalizedResource("WordVoice/"+wordFile);
    if (path!="") return path;
    
    return "";
    
}

std::string LanguageManager::getLocalizedString(std::string str)
{
    
    std::string localized;
    
    switch (_currentLocale) {
        case en_US: localized = _localizationMapEnglish[str]; break;
        case sw_TZ: localized = _localizationMapSwahili[str]; break;
    }
    
    if (localized.empty()) return str;
    
    return localized;
    
}

std::string LanguageManager::findLocalizedResource(std::string path)
{
    
    // handled by Cocos...
    
    return path;
//    
//    for (auto p : _localizedResourcePaths) {
//        auto localizedPath = "Localized/"+p+"/"+path;
//        if (FileUtils::getInstance()->isFileExist(localizedPath)) return localizedPath;
//    }
//
//
//    return "";
}

void LanguageManager::initLocalizationMap()
{
    _localizationMapEnglish["Stop the test"] = "Stop the test";
    _localizationMapEnglish["Go back to test"] = "Go back to test";
    
    
    _localizationMapSwahili["Great!"] = "Vizuri!";
    
    _localizationMapSwahili["Are you ready for"] = "Je, uko tayari kwa";
    _localizationMapSwahili["Prove it!"] = "Thibitisha!";
    _localizationMapSwahili["Try and get 8 questions correct!"] = "Jaribu na toa majibu sahihi manane!";
    _localizationMapSwahili["Challenge"] = "Jaribu";
    _localizationMapSwahili["Congratulations!"] = "Hongera!";
    _localizationMapSwahili["You passed!"] = "Umefaulu!";
    _localizationMapSwahili["You failed"] = "Umeshindwa";
    _localizationMapSwahili["Practice more and try again later."] = "Fanya mazoezi zaidi na rudi baadae.";
    _localizationMapSwahili["Success!"] = "Mafanikio!";
    _localizationMapSwahili["You are not ready."] = "Hauko tayari.";
    _localizationMapSwahili["You need more practice."] = "Unahitaji mazoezi zaidi.";
    _localizationMapSwahili["Welcome!"] = "Karibu!";
    
    _localizationMapSwahili["Start"] = "Anza";
    _localizationMapSwahili["Next"] = "Nenda mbele";
    _localizationMapSwahili["Back"] = "Rudi nyuma";
    _localizationMapSwahili["OK"] = "OK";
    _localizationMapSwahili["Error"] = "Hitilafu";
    _localizationMapSwahili["Enter"] = "Chomeka";
    _localizationMapSwahili["Clear"] = "Futa";
    
    _localizationMapSwahili["English"] = "Kiswahili";
    _localizationMapSwahili["Math"] = "Hesabu";
    
    _localizationMapEnglish["TutorialTrace"] = "Line Tracing";
    _localizationMapSwahili["TutorialTrace"] = "Kufuatisha Mstari";
    
    _localizationMapEnglish["FindTheMatch"] = "Find the Pair";
    _localizationMapSwahili["FindTheMatch"] = "Tafuta Sare";
    
    _localizationMapEnglish["NumberMatching"] = "Number Matching";
    _localizationMapSwahili["NumberMatching"] = "Kufananisha Nambari";
    
    _localizationMapEnglish["Tapping"] = "Bubble Pop";
    _localizationMapSwahili["Tapping"] = "Pasua Povu la Sabuni";
    
    _localizationMapEnglish["LetterMatching"] = "Literacy Matching";
    _localizationMapSwahili["LetterMatching"] = "Kufananisha Kusoma na Kuandika";
    
    _localizationMapEnglish["AnimalPuzzle"] = "Animal Puzzle";
    _localizationMapSwahili["AnimalPuzzle"] = "Fumbo la Picha";
    
    _localizationMapEnglish["PatternTrain"] = "Pattern Train";
    _localizationMapSwahili["PatternTrain"] = "Reli ya Garimoshi";
    
    _localizationMapEnglish["Video"] = "Video";
    _localizationMapSwahili["Video"] = "Video";
    
    _localizationMapEnglish["Counting"] = "Counting";
    _localizationMapSwahili["Counting"] = "Kuhesabu";
    
    _localizationMapEnglish["EquationMaker"] = "Equation Maker";
    _localizationMapSwahili["EquationMaker"] = "Kiumba Mlinganyo";
    
    _localizationMapEnglish["NumberTrain"] = "Number Train";
    _localizationMapSwahili["NumberTrain"] = "Nambari ya Garimoshi";
    
    _localizationMapEnglish["AlphabetPuzzle"] = "Alphabet Puzzle";
    _localizationMapSwahili["AlphabetPuzzle"] = "Fumbo la Alfabeti";
    
    _localizationMapEnglish["Book"] = "Book";
    _localizationMapSwahili["Book"] = "Kitabu";
    
    _localizationMapEnglish["Comprehension"] = "Comprehension Questions";
    _localizationMapSwahili["Comprehension"] = "Maswali ya Ufahamu";
    
    _localizationMapEnglish["DoubleDigit"] = "Double Digit Math";
    _localizationMapSwahili["DoubleDigit"] = "Hisabati ya Tarakimu Mbili";
    
    _localizationMapEnglish["FishTank"] = "Fish Tank";
    _localizationMapSwahili["FishTank"] = "Tangi ya Samaki";
    
    _localizationMapEnglish["HundredPuzzle"] = "100 Puzzle";
    _localizationMapSwahili["HundredPuzzle"] = "Fumbo la Nambari 100";
    
    _localizationMapEnglish["LetterTracing"] = "Letter Tracing";
    _localizationMapSwahili["LetterTracing"] = "Kufuatisha Herufi";
    
    _localizationMapEnglish["MovingInsects"] = "Bug Math";
    _localizationMapSwahili["MovingInsects"] = "Mchezo wa Mdudu";
    
    _localizationMapEnglish["SentenceMaker"] = "Sentence Maker";
    _localizationMapSwahili["SentenceMaker"] = "Kiumba Sentensi";
    
    _localizationMapEnglish["ShapeMatching"] = "Shape Matching";
    _localizationMapSwahili["ShapeMatching"] = "Kufananisha Maumbo";
    
    _localizationMapEnglish["SoundTrain"] = "Sound Train";
    _localizationMapSwahili["SoundTrain"] = "Sauti ya Garimoshi";
    
    _localizationMapEnglish["Spelling"] = "Spelling";
    _localizationMapSwahili["Spelling"] = "Matamshi";
    
    _localizationMapEnglish["WordTracing"] = "Word Tracing";
    _localizationMapSwahili["WordTracing"] = "Kufuatisha Maneno";
    
    _localizationMapEnglish["NumberTracing"] = "Learn to 10";
    _localizationMapSwahili["NumberTracing"] = "Jifunze Mpaka 10";
    
    _localizationMapEnglish["StarFall"] = "Typing";
    _localizationMapSwahili["StarFall"] = "Kuchapa";
    
    _localizationMapEnglish["WordMachine"] = "Word Machine";
    _localizationMapSwahili["WordMachine"] = "Mashine ya Maneno";
    
    _localizationMapEnglish["NumberTracingExt"] = "Number Tracing";
    _localizationMapSwahili["NumberTracingExt"] = "Kufuatisha Nambari";
    
    _localizationMapEnglish["LetterTracingCard"] = "Trace 3 Times";
    _localizationMapSwahili["LetterTracingCard"] = "Fuatisha Mara 3";
    
    _localizationMapEnglish["NumberPuzzle"] = "Number Blocks";
    _localizationMapSwahili["NumberPuzzle"] = "Fumbo la Nambari";
    
    _localizationMapEnglish["Arrange the numbers in order from smallest to largest"] = "Arrange the numbers in order from smallest to largest";
    _localizationMapSwahili["Arrange the numbers in order from smallest to largest"] = "panga kwa mpangilio kutoka ndogo zaidi kwenda kubwa zaidi";

    _localizationMapEnglish["Largest number"] = "Largest number";
    _localizationMapSwahili["Largest number"] = "Namba ipi ndiyo kubwa zaidi?";

}
