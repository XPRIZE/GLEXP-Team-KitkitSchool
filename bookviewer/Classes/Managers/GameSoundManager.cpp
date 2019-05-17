#include "GameSoundManager.h"
#include "SimpleAudioEngine.h"



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

    
    return true;
}

void GameSoundManager::playEffectSoundVoiceOnly(std::string name) {
    stopBGM();
    SimpleAudioEngine::getInstance()->playBackgroundMusic(name.c_str(), false);
}

unsigned int GameSoundManager::playEffectSound(std::string name,
                                               bool isLoop, float pitch, float pan, float gain)
{
    unsigned int ret = 0;

    if (!name.empty()) {
        ret = SimpleAudioEngine::getInstance()->playEffect(name.c_str(),
                                                           isLoop, pitch, pan, gain);
        m_SetLoadedList.insert(name);

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



float GameSoundManager::getBGMSoundVolume()
{
    return SimpleAudioEngine::getInstance()->getBackgroundMusicVolume();
}
void GameSoundManager::setBGMSoundVolume(float volume)
{
    return SimpleAudioEngine::getInstance()->setBackgroundMusicVolume(volume);
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

}

void GameSoundManager::preloadEffect(std::string name){
    if (name.empty()) { return; }

    std::string fullname = name;

    SET_STRING::iterator find = m_SetLoadedList.find(fullname);
    if (find == m_SetLoadedList.end()) {
        SimpleAudioEngine::getInstance()->preloadEffect(fullname.c_str());
        m_SetLoadedList.insert(fullname);
    }
}

void GameSoundManager::preloadBGM(std::string name)
{
    SimpleAudioEngine::getInstance()->preloadBackgroundMusic(name.c_str());
}

void GameSoundManager::playBGM(std::string name)
{
//    if (m_currentBGM == name) return;
//    m_currentBGM = name;
    SimpleAudioEngine::getInstance()->playBackgroundMusic(name.c_str());
}

void GameSoundManager::stopBGM()
{
//    m_currentBGM = "";
    SimpleAudioEngine::getInstance()->stopBackgroundMusic();
}

void GameSoundManager::pauseBGM()
{
    SimpleAudioEngine::getInstance()->pauseBackgroundMusic();
}

void GameSoundManager::resumeBGM()
{
    SimpleAudioEngine::getInstance()->resumeBackgroundMusic();
}
