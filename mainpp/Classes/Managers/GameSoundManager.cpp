#include "GameSoundManager.h"
#include "SimpleAudioEngine.h"
#include "LanguageManager.hpp"



#ifndef MAKE_STRING 
#   define MAKE_STRING(_Text) "" #_Text
#endif

USING_NS_CC;
using namespace CocosDenshion;


static GameSoundManager *s_SharedSoundManager = nullptr;
GameSoundManager* GameSoundManager::getInstance()
{
    if (!s_SharedSoundManager)
    {
        s_SharedSoundManager = new (std::nothrow) GameSoundManager();
        CCASSERT(s_SharedSoundManager, "FATAL: Not enough memory");
        s_SharedSoundManager->init();
    }
    
    return s_SharedSoundManager;
}

bool GameSoundManager::init(void)
{
    m_FilePath = "";
    m_currentBGM = "";
    
    return true;
}

void GameSoundManager::playBGM(std::string name)
{
    if (m_currentBGM == name) return;
    m_currentBGM = name;
    SimpleAudioEngine::getInstance()->playBackgroundMusic(name.c_str());
}

void GameSoundManager::stopBGM()
{
    m_currentBGM = "";
    SimpleAudioEngine::getInstance()->stopBackgroundMusic();
}


void GameSoundManager::preloadGameEffect()
{
    
    
    SimpleAudioEngine::getInstance()->preloadEffect("GameSounds/SFX_LEVELCOMPLETE.m4a");
    SimpleAudioEngine::getInstance()->preloadEffect("Game_WrongAnswer_SlideBack1.m4a");
    SimpleAudioEngine::getInstance()->preloadEffect("UI_Star_Collected.m4a");
}


unsigned int GameSoundManager::playHelpSound(){
    unsigned int ret = 0;
    
    ret = playEffectSound("Help.m4a");
    
    return ret;
}

unsigned int GameSoundManager::playMissEffectSound(){
    unsigned int ret = 0;
    
    ret = playEffectSound("Game_WrongAnswer_SlideBack1.m4a");
    
    return ret;
}

unsigned int GameSoundManager::playFlyingStartEffectSound(){
    unsigned int ret = 0;
    
    ret = playEffectSound("UI_Star_Collected.m4a");
    
    return ret;
}




unsigned int GameSoundManager::playAdultVoice(int num, eAdultVoice vType){
    unsigned int ret = 0;
    
    std::string fileName = getAdultVoiceName(num, vType);
    std::string prefix = "Common/AdultVoice/" + LanguageManager::getInstance()->getCurrentLanguageCode()+"/";
    
    ret = playEffectSound(prefix+fileName);

    return ret;
}


std::string GameSoundManager::getAdultVoiceName(int num, eAdultVoice vType){
    char ret[256] = "";
    
//    LanguageType eType = Application::getInstance()->getCurrentLanguage();
//    std::string subDir = "AdultVoice";
//    std::string path = getLocalePath(eType, subDir);
    char filename[256] = "";
    if(vType == K_ADULT_VOIDE_NONE){
        
        sprintf(filename, "D_%d.m4a",num);
//        } else {
//            return getChildVoiceName(num, K_CHILD_VOIDE_NONE);
//        }
        
    }
    
    else{
        if(vType == K_ADULT_VOICE_PLUS)
            sprintf(filename, "Plus.m4a");
        else if(vType == K_ADULT_VOICE_EQ)
            sprintf(filename, "Equals.m4a");
        else if(vType == K_ADULT_VOICE_EQ_KOREAN_E)
            sprintf(filename, "equalsE.m4a");
        else if(vType == K_ADULT_VOICE_EQ_KOREAN_N)
            sprintf(filename, "equalsN.m4a");
        else if(vType == K_ADULT_VOICE_TIMES)
            sprintf(filename, "Times.m4a");
        else sprintf(filename, "Minus.m4a");

    }
    
//#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    sprintf(ret, "%s",filename);
//#else
//    sprintf(ret, "%s/%s",path.c_str(),filename);
//#endif
    
    return ret;
}

std::string GameSoundManager::getChildVoiceName(int num, eChildVoice vType)
{
    
    char ret[256] = "";
    char filename[256] = "";
    
    if(vType == K_CHILD_VOIDE_NONE){
        sprintf(filename, "C_%03d.m4a",num);
    } else{
        
        if(vType == K_CHILD_VOICE_PLUS)
            sprintf(filename, "Plus.m4a");
        else if(vType == K_CHILD_VOICE_EQ)
            sprintf(filename, "Equals.m4a");
        else if(vType == K_CHILD_VOICE_EQ_KOREAN_E)
            sprintf(filename, "equalsE.m4a");
        else if(vType == K_CHILD_VOICE_EQ_KOREAN_N)
            sprintf(filename, "equalsN.m4a");
        else if(vType == K_CHILD_VOICE_TIMES)
            sprintf(filename, "Times.m4a");
        else sprintf(filename, "Minus.m4a");
        
    }
    
    sprintf(ret, "%s",filename);
    
    return ret;
}

unsigned int GameSoundManager::playChildVoiceWithType(int num, eChildVoice vType)
{
    
    unsigned int ret = 0;
    
    std::string fileName = getChildVoiceName(num, vType);
    
    ret = playEffectSound(fileName);
    
    return ret;
}

unsigned int GameSoundManager::playChildVoice(int num){
    
    return playChildVoiceWithType(num, K_CHILD_VOIDE_NONE);
//    
//    unsigned int ret = 0;
//    
//    char buf[128];
//
//    sprintf(buf, "C_%03d.m4a", num);
//    ret = playEffectSound(buf);
//    
//    return ret;
}

unsigned int GameSoundManager::playEffectSound(std::string name,
                                               bool isLoop, float pitch, float pan, float gain)
{
    unsigned int ret = 0;

    if (!name.empty()) {
        ret = SimpleAudioEngine::getInstance()->playEffect(name.c_str(),
                                                           isLoop, pitch, pan, gain);
        m_SetLoadedList.insert(name);


//        std::string fullPath = FileUtils::getInstance()->fullPathForFilename(name);
//        if (fullPath.empty()) {
//            fullPath = FileUtils::getInstance()->fullPathForFilename("GameSounds/"+name);
//        }
//
//        if (!fullPath.empty()) {
//            ret = SimpleAudioEngine::getInstance()->playEffectSound(name.c_str(), isLoop, pitch, pan, gain);
//        }
    }
    
    return ret;
}

void GameSoundManager::pauseEffect(unsigned int uSoundId)
{
    SimpleAudioEngine::getInstance()->pauseEffect(uSoundId);
}

void GameSoundManager::resumeEffect(unsigned int uSoundId)
{
    SimpleAudioEngine::getInstance()->resumeEffect(uSoundId);
}



void GameSoundManager::stopAllEffects()
{
    SimpleAudioEngine::getInstance()->stopAllEffects();
}

void GameSoundManager::stopEffectSound(unsigned int soundID){
    if (soundID>0) {
        SimpleAudioEngine::getInstance()->stopEffect(soundID);
    }
}

float GameSoundManager::getEffectSoundVolume()
{
    return SimpleAudioEngine::getInstance()->getEffectsVolume();
}

void GameSoundManager::setEffectSoundVolume(float volume)
{
    SimpleAudioEngine::getInstance()->setEffectsVolume(volume);
}

void GameSoundManager::unloadAllEffect(){
    SET_STRING::iterator itr = m_SetLoadedList.begin();
    SET_STRING::iterator itr_end = m_SetLoadedList.end();
    
    for(; itr != itr_end; ++itr){
        const std::string& name = *itr;
        SimpleAudioEngine::getInstance()->unloadEffect(name.c_str());
    }
    
    m_SetLoadedList.clear();
    m_FilePath = "";
}

void GameSoundManager::preloadEffect(std::string name){
    if (name.empty()) { return; }

    std::string fullname = m_FilePath + name;

    SET_STRING::iterator find = m_SetLoadedList.find(fullname);
    if (find == m_SetLoadedList.end()) {
        SimpleAudioEngine::getInstance()->preloadEffect(fullname.c_str());
        m_SetLoadedList.insert(fullname);
    }
}

void GameSoundManager::preloadAdultVoice(){
    std::string plusFileName = getAdultVoiceName(0, K_ADULT_VOICE_PLUS);
    std::string minusFileName = getAdultVoiceName(0, K_ADULT_VOICE_MINUS);
    std::string eqFileName = getAdultVoiceName(0, K_ADULT_VOICE_EQ);
    std::string timesFileName = getAdultVoiceName(0, K_ADULT_VOICE_TIMES);

    preloadEffect(plusFileName);
    preloadEffect(minusFileName);
    preloadEffect(eqFileName);
    preloadEffect(timesFileName);
    
}

void GameSoundManager::preloadChildVoice(){
    
    std::string plusFileName  = getChildVoiceName(0, K_CHILD_VOICE_PLUS);
    std::string minusFileName = getChildVoiceName(0, K_CHILD_VOICE_MINUS);
    std::string eqFileName    = getChildVoiceName(0, K_CHILD_VOICE_EQ);
    std::string timesFileName = getChildVoiceName(0, K_CHILD_VOICE_TIMES);
    
    preloadEffect(plusFileName);
    preloadEffect(minusFileName);
    preloadEffect(eqFileName);
    preloadEffect(timesFileName);
    
}

void GameSoundManager::setFilePath(std::string path){
    m_FilePath = path;
}


