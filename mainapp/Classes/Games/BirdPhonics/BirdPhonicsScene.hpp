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
    typedef std::pair<char, std::string> WordPair;
    
    struct Problem {
        std::vector<char> phonics;
        std::vector<WordPair> words;
    };
    
    
    class Bird;
    
    class Bread : public Node {
    public:
        CREATE_FUNC(Bread);
        void setWord(string word, char phonic);
        void setTargetBird(Bird* bird);
        
        std::string _word;
        char _phonic;
        SoundEffect _wordEffect;
        Vec2 _orgPos;
        Bird *_targetBird;
        bool _touchEnabled;
        
        
        
    };
    
    class Bird : public Node {
    public:
        CREATE_FUNC(Bird);

        
        void setType(int type, std::function<void(void)> func);
        void setPhonic(char phonic);
        void setLight(Sprite *light) { _light = light; }
        void setTouchEnabled(bool enabled) { _touchEnabled = enabled; }
        
        void turnLight(bool isOn);
        
        
        void chirp(bool autocalm = true);
        void calm();
        void readyToEat(bool ready);
        void eatBread(Bread *bread);
        
        
        char _phonic;
        
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



