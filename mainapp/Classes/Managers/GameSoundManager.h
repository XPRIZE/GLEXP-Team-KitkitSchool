#ifndef GAME_SOUND_MANAGER_H
#define GAME_SOUND_MANAGER_H

#include <string>
#include "cocos2d.h"

class GameSoundManager {
public:
    enum eAdultVoice{
        K_ADULT_VOIDE_NONE = 0,
        K_ADULT_VOICE_PLUS,
        K_ADULT_VOICE_MINUS,
        K_ADULT_VOICE_EQ,
        K_ADULT_VOICE_EQ_KOREAN_E,
        K_ADULT_VOICE_EQ_KOREAN_N,
        K_ADULT_VOICE_TIMES,
    };
    
    // Child voices
    enum eChildVoice{
        K_CHILD_VOIDE_NONE = 0,
        K_CHILD_VOICE_PLUS,
        K_CHILD_VOICE_MINUS,
        K_CHILD_VOICE_EQ,
        K_CHILD_VOICE_EQ_KOREAN_E,
        K_CHILD_VOICE_EQ_KOREAN_N,
        K_CHILD_VOICE_TIMES,
    };

    typedef std::set<std::string> SET_STRING;
    
public:
    static GameSoundManager *getInstance();
    GameSoundManager(){};
    virtual bool init();
    virtual ~GameSoundManager(){};
    
public:
    void playBGM(std::string name, bool isLoop=false);
    void stopBGM();
    void pauseBGM();
    void resumeBGM();

    float getBGMSoundVolume();
    void setBGMSoundVolume(float volume);

    void fadeOutBGM( float duration );


    unsigned int playChildVoice(int num);
    unsigned int playChildVoiceWithType(int num, eChildVoice vType = K_CHILD_VOIDE_NONE);
    unsigned int playAdultVoice(int num, eAdultVoice vType = K_ADULT_VOIDE_NONE);
    unsigned int playEffectSound(std::string name,
                                 bool isLoop = false, float pitch = 1.f,
                                 float pan = 0.f, float gain = 1.f);
    
    void playEffectSoundForAutoStart(std::string name);
    void playEffectSoundVoiceOnly(std::string name);

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
    void setFilePath(std::string path);
    
    
    std::string getAdultVoiceName(int num, eAdultVoice vType = K_ADULT_VOIDE_NONE);
    std::string getChildVoiceName(int num, eChildVoice vType);

    
    
private:

    SET_STRING m_SetLoadedList;
    std::string m_FilePath;
    
    std::string m_currentBGM;
    
};


#endif 
