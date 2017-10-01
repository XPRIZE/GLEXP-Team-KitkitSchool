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
    //auto defaultLang = UserDefault::getInstance()->getStringForKey("appLanguage", "sw") == "en" ? LanguageType::ENGLISH : LanguageType::SWAHILI;

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
    
    std::string localized;
    
    switch (_langType) {
        case ENGLISH: localized = _localizationMapEnglish[str]; break;
        case SWAHILI: localized = _localizationMapSwahili[str]; break;
    }
    
    if (localized.empty()) return str;
    
    return localized;
    
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
}
