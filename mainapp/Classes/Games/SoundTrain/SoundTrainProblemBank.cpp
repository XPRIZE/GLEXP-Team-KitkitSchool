//
//  SoundTrainProblemBank.cpp
//  enumaXprize
//
//  Created by JungJaehun on 29/08/2017.
//
//

#include "SoundTrainProblemBank.hpp"

BEGIN_NS_SOUNDTRAIN;

Json::StyledWriter writer;

SoundTrainProblemBank::SoundTrainProblemBank() {
    
}


Json::Value SoundTrainProblemBank::generateProblems(int level) {
    Json::Value problems;
    
    vector<string> wordsType1Eng = {
        "b-a", "c-a", "d-a", "f-a", "g-a", "h-a", "j-a", "l-a", "l-a", "m-a", "p-a", "r-a", "s-a", "t-a", "v-a", "y-a"
    };
    
    vector<string> wordsType2Eng = {
        "f-e", "g-e", "j-e", "l-e", "n-e", "p-e", "s-e", "v-e", "y-e"
    };
    vector<string> wordsType3Eng = {
        "b-i", "f-i", "h-i", /*"k-i",*/ "l-i", "z-i"
    };
    vector<string> wordsType4Eng = {
        "b-o", "j-o", "p-o", "c-o", "m-o", "l-o"
    };
    vector<string> wordsType5Eng = {
        "b-u", "d-u", "g-u", "h-u", "j-u", "c-u", "m-u", "p-u", "r-u"
    };
    vector<string> wordsType6Eng = {
        "h-e", "m-e", "b-e", "w-e", "h-i", "s-o", "d-o", "t-o"
    };
    vector<string> wordsType7Eng = {
        "d-a-d", "m-a-d", "s-a-d", "b-a-g", "m-a-p", "b-e-t", "g-e-t", "b-e-d", "r-e-d", "l-e-g",
        "b-i-n", "f-i-n", "w-i-n", "d-i-d", "h-i-d", "h-o-p", "t-o-p", "n-o-d", "d-o-t", "n-o-t",
        "b-u-g", "h-u-g", "f-u-n", "r-u-n", "n-u-t"
    };
    vector<string> wordsType8Eng = {
        "th-i-s", "wh-i-p", "wh-i-z", "sh-i-p", "ch-i-n", "th-a-t", "wh-a-t", "ch-a-t", "ch-a-p", "th-e-m",
        "th-e-n", "th-e-y", "wh-e-n", "ch-e-f", "sh-o-p", "sh-o-t", "ch-o-p", "sh-u-t"
    };
    vector<string> wordsType9Eng = {
        "n-e-ck", "s-i-ck", "p-i-ck", "w-i-sh", "f-i-sh", "d-i-sh", "w-i-th", "r-i-ch", "b-a-sh", "d-a-sh",
        "b-a-ck", "c-a-sh", "m-a-sh", "p-a-th", "b-a-th", "m-a-th", "b-a-ck", "s-a-ck", "t-a-ck", "m-a-sh",
        "r-u-sh", "d-u-ck", "s-u-ch", "m-u-ch", "g-o-sh"
    };
    vector<string> wordsType10Eng = {
        "bl-ue", "bl-o-b", "cl-ue", "cl-ay", "fl-i-p", "fl-a-p", "dr-i-p", "dr-a-b", "pl-ay", "sc-ar",
        "sk-i-p", "sk-i-n", "sw-i-m", "sn-ow", "sn-i-p", "sn-a-p", "sp-a-n", "sp-o-t", "sp-i-n", "sl-ow",
        "gr-a-b", "gr-ow", "cr-a-b", "cr-i-b", "st-e-m", "st-e-p"
    };
    vector<string> wordsType11Eng = {
        "m-oo-n", "s-oo-n", "r-oo-m", "f-oo-t", "m-ee-t", "m-ea-t", "s-ea-l", "s-ee-d", "sh-ee-p", "b-ee-p",
        "ea-ch", "p-ai-d", "p-ai-n", "s-ai-l", "f-ai-l", "p-ai-r", "st-ay", "gr-ay", "b-oa-t", "g-oa-t",
        "r-oa-d", "fl-oa-t", "b-ow-l", "m-ou-th", "ou-ch", "sh-ow", "gr-ow", "b-oi-l", "s-oi-l", "tr-ay",
        "pl-ay"
    };
    vector<string> wordsType12Eng = {
        "c-ar", "st-ar", "j-ar", "ar-t", "ar-m", "d-ar-k", "m-ar-k", "f-ar", "b-ar-n", "f-er-n",
        "st-ir", "sh-ir-t", "sk-ir-t", "f-ir-st"
    };
    
    vector<string> wordsType13Eng = {
        "s-ing", "r-ing", "k-ing", "s-ong", "l-ong", "l-ung", "s-ung", "r-ung", "h-ang", "b-ang",
        "r-ang"
    };
    
    vector<string> wordsType1Swa = {
        "b-a", "d-a", "f-a", "g-a", "h-a", "j-a", "k-a", "l-a", "m-a",
        "b-e", "d-e", "f-e", "g-e", "h-e", "j-e", "k-e", "l-e", "m-e",
        "b-i", "d-i", "f-i", "g-i", "h-i", "j-i", "k-i", "l-i", "m-i",
        "b-o", "d-o", "f-o", "g-o", "h-o", "j-o", "k-o", "l-o", "m-o",
        "b-u", "d-u", "f-u", "g-u", "h-u", "j-u", "k-u", "l-u", "m-u",
        "n-a", "p-a", "r-a", "s-a", "t-a", "v-a", "w-a", "y-a", "z-a",
        "n-e", "p-e", "r-e", "s-e", "t-e", "v-e", "w-e", "y-e", "z-e",
        "n-i", "p-i", "r-i", "s-i", "t-i", "v-i", "w-i", "y-i", "z-i",
        "n-o", "p-o", "r-o", "s-o", "t-o", "v-o", "w-o", "y-o", "z-o",
        "n-u", "p-u", "r-u", "s-u", "t-u", "v-u", "w-u", "y-u", "z-u"
    };
    
    vector<string> wordsType2Swa = {
        "ch-a", "dh-a", "gh-a", "sh-a", "th-a", "ng-a", "ng'-a", "ny-a",
        "ch-e", "dh-e", "gh-e", "sh-e", "th-e", "ng-e", "ng'-e", "ny-e",
        "ch-i", "dh-i", "gh-i", "sh-i", "th-i", "ng-i", /*"ng'-i",*/ "ny-i",
        "ch-o", "dh-o", "gh-o", "sh-o", "th-o", "ng-o", /*"ng'-o",*/ "ny-o",
        "ch-u", "dh-u", "gh-u", "sh-u", "th-u", "ng-u", /*"ng'-u",*/ "ny-u"
    };
    
    vector<string> wordsType3Swa = {
        "b-wa", "g-wa", "m-wa", /*"p-wa",*/ "s-wa", "t-wa",
        "b-we", "k-we", "m-we", "t-we",
        "k-wi", "m-wi", "s-wi", "t-wi",
        "m-wo", "m-wu",
        "ch-wa", "sh-wa", "ny-wa",
        "ny-we",
        "ny-wi",
        "m-ya", "p-ya", "v-ya",
        "m-ye", "v-ye",
        "f-yo", "m-yo", "v-yo",
        "f-yu", "k-yu", "m-yu", "p-yu", "v-yu"
    };
    
    vector<string> wordsType4Swa = {
        "m-ba", "m-be", /*"m-bi",*/ "m-bo", "m-bu", "m-da", "m-de", "m-di", "m-do", "m-du",
        /*"n-da",*/ /*"n-de", "n-di", "n-do", "n-du", "n-ja",*/ "n-je", /*"n-ji", "n-jo", "n-ju"*/
    };

    vector<string> wordsType5Swa = {
        "ba-ba", "ba-bu", "ba-ta","da-fu","du-ka","he-ri","ma-ji","chu-ra","shu-le","nyo-ta",
        "ya-i","ju-a","ju-u","le-o","da-u","fu-a","li-a","cha-i","cho-o","ngu-o",
        /*"ndi-zi",*/"mbe-le",/*"mbi-li",*/"mbo-ga",/*"kwe-li",*/"mbu-zi",/*"pwa-ni",*//*"pwe-za",*/"vyu-ra","nywe-le",
        /*"mvu-a",*//*"ndo-o",*//*"njo-o",*//*"nja-a","nji-a",*//*"ndo-o","sto-o","mka-a",*/
        "te-mbo","tu-mbo","si-mba","ma-mba","pe-mbe","cho-mbo","chu-mba","sha-mba","nyu-mba",/*"ng'o-mbe",*/
        "sa-ma-ki","ki-ta-bu","ka-la-mu","ha-di-thi","wa-to-to",
        "vi-zu-ri","so-ko-ni","pa-mo-ja","the-lu-ji","ngu-ru-we",
        "fa-gi-o","pa-pa-i","li-ma-u","nu-nu-a","ba-ru-a","re-di-o","ma-ya-i","si-ki-o","cha-gu-a",
        "ki-a-tu","vi-a-tu","mi-a-ka","ga-u-ni","ta-u-si","ki-a-zi","vi-a-zi",
        /*"sa-ndu-ku",*/"bu-sta-ni",
        /*"ste-she-ni",*/"mwa-li-mu",
        /*"mwa-mvu-li",*/
        "the-ma-ni-ni","pi-ki-pi-ki","po-le-po-le",
        /*"ho-spi-ta-li",*/
        /*"mwa-na-fun-zi"*/
    };
    
    Json::Value words(Json::arrayValue);
    vector<string> wordsTypeMerge = {};
    
    auto Lang = LanguageManager::getInstance()->getCurrentLanguageTag();
    
    if (Lang == "en-US") {
        switch(level) {
            case 1: random_shuffle(wordsType1Eng.begin(), wordsType1Eng.end(), [](int n) { return rand() % n; }); for (int i=0; i<5; i++) { words.append(wordsType1Eng[i]); } break;
            case 2: random_shuffle(wordsType2Eng.begin(), wordsType2Eng.end(), [](int n) { return rand() % n; }); for (int i=0; i<5; i++) { words.append(wordsType2Eng[i]); } break;
            case 3: random_shuffle(wordsType3Eng.begin(), wordsType3Eng.end(), [](int n) { return rand() % n; }); for (int i=0; i<5; i++) { words.append(wordsType3Eng[i]); } break;
            case 4: random_shuffle(wordsType4Eng.begin(), wordsType4Eng.end(), [](int n) { return rand() % n; }); for (int i=0; i<5; i++) { words.append(wordsType4Eng[i]); } break;
            case 5: random_shuffle(wordsType5Eng.begin(), wordsType5Eng.end(), [](int n) { return rand() % n; }); for (int i=0; i<5; i++) { words.append(wordsType5Eng[i]); } break;
            case 6: random_shuffle(wordsType6Eng.begin(), wordsType6Eng.end(), [](int n) { return rand() % n; }); for (int i=0; i<5; i++) { words.append(wordsType6Eng[i]); } break;
            case 7: random_shuffle(wordsType7Eng.begin(), wordsType7Eng.end(), [](int n) { return rand() % n; }); for (int i=0; i<5; i++) { words.append(wordsType7Eng[i]); } break;
            case 8: random_shuffle(wordsType8Eng.begin(), wordsType8Eng.end(), [](int n) { return rand() % n; }); for (int i=0; i<5; i++) { words.append(wordsType8Eng[i]); } break;
            case 9: random_shuffle(wordsType9Eng.begin(), wordsType9Eng.end(), [](int n) { return rand() % n; }); for (int i=0; i<5; i++) { words.append(wordsType9Eng[i]); } break;
            case 10: random_shuffle(wordsType10Eng.begin(), wordsType10Eng.end(), [](int n) { return rand() % n; }); for (int i=0; i<5; i++) { words.append(wordsType10Eng[i]); } break;
            case 11: random_shuffle(wordsType11Eng.begin(), wordsType11Eng.end(), [](int n) { return rand() % n; }); for (int i=0; i<5; i++) { words.append(wordsType11Eng[i]); } break;
            case 12: random_shuffle(wordsType12Eng.begin(), wordsType12Eng.end(), [](int n) { return rand() % n; }); for (int i=0; i<5; i++) { words.append(wordsType12Eng[i]); } break;
            case 13: random_shuffle(wordsType13Eng.begin(), wordsType13Eng.end(), [](int n) { return rand() % n; }); for (int i=0; i<5; i++) { words.append(wordsType13Eng[i]); } break;
            default: break;
        }
    } else if (Lang == "sw-TZ") {
        switch(level) {
            case 1: random_shuffle(wordsType1Swa.begin(), wordsType1Swa.end(), [](int n) { return rand() % n; }); for (int i=0; i<5; i++) { words.append(wordsType1Swa[i]); } break;
            case 2: random_shuffle(wordsType2Swa.begin(), wordsType2Swa.end(), [](int n) { return rand() % n; }); for (int i=0; i<5; i++) { words.append(wordsType2Swa[i]); } break;
            case 3: random_shuffle(wordsType3Swa.begin(), wordsType3Swa.end(), [](int n) { return rand() % n; }); for (int i=0; i<5; i++) { words.append(wordsType3Swa[i]); } break;
            case 4: random_shuffle(wordsType4Swa.begin(), wordsType4Swa.end(), [](int n) { return rand() % n; }); for (int i=0; i<5; i++) { words.append(wordsType4Swa[i]); } break;
            case 5: random_shuffle(wordsType5Swa.begin(), wordsType5Swa.end(), [](int n) { return rand() % n; }); for (int i=0; i<5; i++) { words.append(wordsType5Swa[i]); } break;
            default: break;
        }
        
    }
    
    
    /*
     CCLOG("");
    CCLOG("LEVEL 13");
    for(auto word : wordsType13Eng) {
        string fullSylls;
        vector<string> sylls = TodoUtil::split(word.c_str(), '-');
        
        bool wordCheck = true;
        for (auto syll : sylls) {
            if (!FileUtils::getInstance()->isFileExist("SoundTrain/VoiceEN/"+syll+".m4a") && !FileUtils::getInstance()->isFileExist("SoundTrain/VoiceEN/"+syll+".wav")) {
                CCLOG("none: %s",syll.c_str());
                //CCLOG("SoundTrain/VoiceSW/%s.wav",syll.c_str());
                wordCheck = false;
            }
        }
        TodoUtil::replaceAll(word, "-", "");
        if (!FileUtils::getInstance()->isFileExist("SoundTrain/VoiceEN/"+word+".m4a") && !FileUtils::getInstance()->isFileExist("SoundTrain/VoiceEN/"+word+".wav")) {
            CCLOG("none: %s",word.c_str());
            //CCLOG("SoundTrain/VoiceSW/%s.wav",word.c_str());
            
            wordCheck = false;
        }
        if (!wordCheck) {
            CCLOG("   Failed: %s", word.c_str());
        }
    }
    CCLOG("");
     */

    problems["words"] = words;
    
    CCLOG("%s", writer.write(problems).c_str());
    
    return problems;
}



END_NS_SOUNDTRAIN;
