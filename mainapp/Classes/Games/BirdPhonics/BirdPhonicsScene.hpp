//
//  BirdPhonicsScene.hpp
//
//  Created by Gunho Lee on 6/07/17.
//
//

#pragma once

#include <stdio.h>
#include "cocos2d.h"
#include "ui/CocosGUI.h"

#include <vector>
#include <string>
#include <functional>

#include "Common/Controls/ProgressIndicator.hpp"
#include "Common/Basic/SoundEffect.h"

USING_NS_CC;
using namespace cocos2d::ui;
using namespace std;


namespace BirdPhonicsSceneSpace
{
    typedef std::pair<std::string, std::string> WordPair;
    
    struct Problem {
        int workSheet;
        std::vector<std::string> phonics_owner;
        std::vector<std::string> phonics_sound;
        std::vector<WordPair> words;
        std::vector<std::string> words_sound;
        std::vector<std::string> phonic_positions;
    };
    
    
    class Bird;
    
    class Bread : public Node {
    public:
        CREATE_FUNC(Bread);
        Bread(): onGoodDown(nullptr), onBadDown(nullptr) {}
    
        void setWord(string word, string phonic_owner, string word_sound, string phonic_position);
        void setTargetBird(Bird* bird);
        
        std::function<void()> onGoodDown;
        std::function<void()> onBadDown;

        
        std::string _word;
        string _phonic_owner;
        SoundEffect _wordEffect;
        Vec2 _orgPos;
        Bird *_targetBird;
        bool _touchEnabled;
        
        bool _isEaten;
        
    };
    
    class Bird : public Node {
    public:
        CREATE_FUNC(Bird);

        
        void setType(int type, std::function<void(void)> func);
        void setPhonic(string phonic_owner, string phonic_sound);
        void setLight(Sprite *light) { _light = light; }
        void setTouchEnabled(bool enabled) { _touchEnabled = enabled; }
        
        void turnLight(bool isOn);
        
        
        void chirp(bool autocalm = true);
        void calm();
        void readyToEat(bool ready);
        void eatBread(Bread *bread);
        
        
        string _phonic_owner;
        
    protected:
        Sprite *_light;
        Sprite *_body;
        int _type;

        bool _isChirping;
        bool _touchEnabled;
        

        
        std::string _normalName, _activeName, _correctName, _silhouetteName;
        SoundEffect _phonicEffect;
        std::function<void(void)> _onEat;
        

    };
    

}


class BirdPhonicsScene : public Layer
{
public:
    static Scene* createScene(int levelID);
    virtual bool init() override;
    CREATE_FUNC(BirdPhonicsScene);


    virtual void onEnter() override;
    virtual void onEnterTransitionDidFinish() override;
    void onStart();
    void onStageComplete();
    

    
    
    void setLevel(int level);
    void setTouchEnabled(bool enabled);
    
    void showProblem(int index);
    void createBread();
    static vector<int> getLevelIDs();
    
    
private:

    Node *_gameNode;

    std::vector<BirdPhonicsSceneSpace::Bird*> _birds;
    std::vector<BirdPhonicsSceneSpace::Bread*> _breads;
    int _numEatenBreads;
    
    std::vector<BirdPhonicsSceneSpace::Problem> _problems;
    
    
    int _currentLevel;
    int _currentProblemIndex;
    ProgressIndicator *_progressBar;
    

};



