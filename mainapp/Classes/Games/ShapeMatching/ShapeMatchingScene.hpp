//
//  GameScene.hpp
//  Matching
//
//  Created by sugar on 5/18/16.
//
//

#ifndef ShapeMatchingScene_hpp
#define ShapeMatchingScene_hpp


#include "ShapeMatchingCard.hpp"
#include "Models/LevelData.hpp"
#include "Models/Worksheet.hpp"
#include "Models/Problem.hpp"

#include "cocos2d.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
#include "3rdParty/json/json.h"

//#include "Base/CommonHeader.h"
#include <array>
#include <unordered_set>
//#include "Controls/TodoSpeechButtonCC.hpp"

#include "Common/Controls/ProgressIndicator.hpp"

USING_NS_CC;
using namespace cocos2d::ui;
using namespace std;

namespace ShapeMatching {
    extern const char* SOLVE_EFFECT_SOUND;
    extern const string CARD_MATCHING_EFFECT_SOUND;
    extern const string STAR_EFFECT_SOUND;
    extern string swTZ_of_enUS(const string& enUS);
    extern string localizeString(const string& enUS);
    extern string localizedSound(const string& enUS);

}

class ShapeMatchingScene : public Layer
{
public:
    //static Scene* createScene(int levelID);
    
    CREATE_FUNC(ShapeMatchingScene);
    static Scene* createScene(int levelID);

    void setLevel(int level);

    virtual bool init() override;
    //virtual void update(float) override;
    virtual void onEnter() override;
    //virtual void onEnterTransitionDidFinish() override;
    virtual void prepareNewGameWithLevel();
    virtual void handleStart();
    virtual void handlePause();
    virtual void handleQuit();
    virtual void handleResume();
    
    
    
private:
    float _defaultScaleFactor;
    float _upScaleFactor;
    
    std::map<std::string,float> loadDurationsheet();
    std::map<std::string,float> _duration;

    int _currentLevelID;
    ShapeMatching::Worksheet _currentWorksheet;
    int _currentProblemID;
    ShapeMatching::Problem _currentProblem;
    ProgressIndicator *_progressBar;

    //    ProgressIndicator *_progressBar;
    Node* _gameNode;
    //bool bActiveParticle = false;
    //bool bLinked = false;
    //ShapeMatchingCard* linkedSourceCard = nullptr;
    //ShapeMatchingCard* linkedTargetCard = nullptr;
    int _currentLevelMaxCardCount = 0;
    int _matchCount = 0;
    int _zOrder = 10;
    ParticleSystemQuad* _particleEffect;
    //Node* _shiningParticleArray[2];
    //    cocos2d::ui::Button* _nextButton;
    
    std::array<std::string, 7> _basic2D;
    std::array<std::string, 5> _advanced2D;
    std::array<std::string, 4> _basic3D;
    std::array<std::string, 3> _advanced3D;
    std::array<std::string, 2> _colors;
    std::array<std::string, 3> _sizes;
    std::array<int, 4> _rotation;
    std::vector<std::string> _advanced;
    std::map<std::string, std::string> _attribute;
    std::multimap<std::string, std::string> attr2D;
    std::multimap<std::string, std::string> attr3D;
    std::vector<std::string> attr2DKeys;
    std::vector<std::string> attr3DKeys;
    std::vector<std::string> _shapeCandidates;
    
    
    
    ShapeMatchingCard* createMatchingCard(int zOrder, Point pos,
                                          int level, int id, int type,
                                          const std::string& cardImageName, int rotation, float scale, bool hasTouchSound, bool hasMatchingSound);
    
    void bindingEvents(ShapeMatchingCard* card);
    void initCardList();
    /*
     void preloadShiningParticle();
     void startParticle();
     void stopParticle();
     Node* createShiningParticle();
     */
    void addStarParticle(Node* targetNode);
    
    Point safePointForBoundary(Point point);
    void stompByNode(Node* node);
    bool isExistPair();
    int getMinimumLocalZOrder();
    
    bool _nextTransition;
    
    cocos2d::ValueMap pickCards(ShapeMatching::Piece pieceInfo, int i);
    std::string generateCardImageFilename(std::string shape, std::string type, std::string color, std::string size);
    
    // TodoSpeechButtonCC* _speechButton;
    
    Size _gameSize;
    bool isAnimating;
    
    LayerColor* _animationBG;
    
public:
    std::vector<ShapeMatchingCard*> matchingCardList;
    
    void onStart();
    void onSolve();
    void advanceToNextProblem();
    bool touchBeganHandler(Touch* touch, Event* event);
    void touchMovedHandler(Touch* touch, Event* event);
    void touchEndedHandler(Touch* touch, Event* event);
};
#endif /* ShapeMatchingScene_hpp */
