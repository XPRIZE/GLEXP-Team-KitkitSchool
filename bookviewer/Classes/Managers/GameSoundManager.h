#ifndef GAME_SOUND_MANAGER_H
#define GAME_SOUND_MANAGER_H

#include <string>
#include "cocos2d.h"

class GameSoundManager {
public:
    typedef std::set<std::string> SET_STRING;
    
public:
    static GameSoundManager *getInstance();
    GameSoundManager(){};
    virtual bool init();
    virtual ~GameSoundManager(){};
    
public:
   
    unsigned int playEffectSound(std::string name,
                                 bool isLoop = false, float pitch = 1.f,
                                 float pan = 0.f, float gain = 1.f);
    
    void pauseEffect(unsigned int uSoundId);
    void resumeEffect(unsigned int uSoundId);
    
    
    void stopEffectSound(unsigned int soundID);
    void stopAllEffects();
    
    
    
    float getEffectSoundVolume();
    void setEffectSoundVolume(float volume);
    
    void preloadAdultVoice();
    void preloadChildVoice();

    void unloadAllEffect();
    void preloadEffect(std::string name);
    
    
    void playBGM(std::string name);
    void stopBGM();
    void pauseBGM();
    void resumeBGM();
    
    
private:
    SET_STRING m_SetLoadedList;

    
};


#endif 
