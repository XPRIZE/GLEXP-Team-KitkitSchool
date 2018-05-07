//
//  SentenceMakerScene.hpp
//  KitkitSchool
//
//  Created by timewalker on 30/12/2016.
//
//

#ifndef SentenceMakerScene_hpp
#define SentenceMakerScene_hpp

#include <stdio.h>
#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "WordItem.hpp"
#include "Common/Controls/ProgressIndicator.hpp"
#include "3rdParty/json/json.h"
#include "GreenDashedRect.hpp"

USING_NS_CC;
using namespace cocos2d::ui;
using namespace std;

class SentenceMakerScene : public Layer
{
public:
    SentenceMakerScene();
    static Scene* createScene(int levelID);
    virtual bool init() override;
    CREATE_FUNC(SentenceMakerScene);
    virtual void onEnter() override;
    void setLevel(int level);
    vector<int> getCandidateLevelIDs();
    
    // (s) ProblemData
    class Word
    {
    public:
        std::string value;
        bool bBlank;
        
    };
    
    class ProblemData
    {
    public:
        std::vector<Word*> wordSet;
        std::vector<std::string> wrongWordSet;
        std::string imagePath;
        std::string soundPath;
        
        static ProblemData* parse(std::vector<std::string> rawData);
    };
    // (e) ProblemData

private:
    typedef tuple<string, int, int> ProblemSetKey;
    
    ProgressIndicator *_progressBar;
    int _progressBarSize;
    Node *_gameNode;
    Sprite* _sketchbookPage;
    LayerColor* _bottomAreaLayer;
    int _currentLevel;
    int _currentProblem;
    int _maxLevel;
    ProblemData* _problemData;
    EventListenerTouchOneByOne* _blocker;
    
    std::vector<std::string> _bottomItemVector;
    map<ProblemSetKey, ProblemData*> _problemDataMap;
    vector<GreenDashedRect*> _blanks;
    Node *_blankLine;

    Json::Value _wordSoundDurationJson;
    Json::Value _sentenceSoundDurationJson;
    
    float _blockScaleFactor;
    void onStart();
    void parseSoundDurationJson();
    void createGame(int problemId);
    bool isSolved();
    void onSolve();
    void onSolvePostProcess();
    void initData();
    void drawPage();
    void drawQuestionImage();
    void drawBottomItems();
    void playWordSound(string word);
    WordItem* createWordItem(string word);
    Node* createSoundButton();
    vector<string> getRefinedWordSoundName(string value);
    string getLowerString(string value);
    double getWordDuration(string soundName);
    double getSentenceDuration(string soundName);

    void repositionBlankLine(int resizedIndex, float preWidth, float postWidth);

};


#endif /* SentenceMakerScene_hpp */
