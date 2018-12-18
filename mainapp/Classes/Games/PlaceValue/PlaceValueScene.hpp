//
//  PlaceValueScene.hpp
//
//  Created by harunom on 8/31/18.
//
//

#ifndef PlaceValueScene_hpp
#define PlaceValueScene_hpp

#include <stdio.h>
#include "cocos2d.h"
#include "ui/CocosGUI.h"

#include <vector>
#include <string>
#include <functional>

#include "Common/Controls/ProgressIndicator.hpp"

USING_NS_CC;
using namespace cocos2d::ui;
using namespace std;


struct PlaceValueLevelStruct;
class WaterDropLandPiece;
class WaterDropTankPiece;

class PlaceValueScene : public Layer
{
public:
    static Scene* createScene(string levelID);
    virtual bool init() override;
    CREATE_FUNC(PlaceValueScene);

    virtual void onEnter() override;
    virtual void onEnterTransitionDidFinish() override;
    void onStart();
    void showBoard(int index);
    void createBoard(int index);
    void onStageClear();
    void onLevelComplete();
    
    void setTank100Count(int count);
    void setTank10Count(int count);
    void setTank1Count(int count);
    
    WaterDropLandPiece *createWaterDropLandPiece(int count);
    WaterDropTankPiece *createWaterDropTankPiece(int count);
    
    void upgradeTankPiece1();
    void upgradeTankPiece10();

    void adjustWaterDropInTank100(bool playAction, bool isInc=true, int removedIndex=0);
    void adjustWaterDropInTank10(bool playAction, bool isInc=true, int removedIndex=0);
    void adjustWaterDropInTank1(bool playAction, bool isInc=true, int removedIndex=0);

        
    void setLevel(int level);
    void loadData(int level, int *workSheet);
    
    Node* createShiningParticle(Vec2 size, int totalParticles);
    void startParticle(Node *particleNode);
    void stopParticle(Node *particleNode);
    
    virtual void update(float dt);
    
private:
    void loadDurationsheet();
    float getDuration(string name);
    float getDurationResult100();
    float getDurationResultAnd();
    float getDurationResult10();
    
public:
    int _tank100Count;
    int _tank10Count;
    int _tank1Count;
    
    WaterDropTankPiece* _currentPickedTankPiece;
    WaterDropLandPiece* _currentPickedLandPiece;

    
private:
    Node *_gameNode;
    Node *_tank100Particle;
    Node *_tank10Particle;
    Node *_tank1Particle;
    Node *_answerParticle;
    
    vector<WaterDropTankPiece*> _tank100Pieces;
    vector<WaterDropTankPiece*> _tank10Pieces;
    vector<WaterDropTankPiece*> _tank1Pieces;
    
    int _objectNo;
    int _suggestNo;
    
    int _currentLevel;
    int _currentSheetNo;
    int _currentBoardIndex;

    ProgressIndicator *_progressBar;

    vector<PlaceValueLevelStruct> _boards;

    std::map<std::string,float> _duration;
    
    bool _isSkip;
    
    Vec2 cloud1_pos;
    Vec2 cloud2_pos;
    Vec2 cloud3_pos;

};


struct PlaceValueLevelStruct {
    int level;
    int worksheet;
    int problem;
    int objectNo;
    int suggestNo;
};

class WaterDropLandPiece : public Node
{
public:

    CREATE_FUNC(WaterDropLandPiece);
    
    virtual bool init() override;
    void setTexture(int count);
    
    void setPicked(bool picked);
        
    void pickTarget();
    void snapTarget();
    void layTarget();
    void hoverTarget();
    void hoverOutTarget();
    
    std::function<void()> onPicked;
    std::function<void()> onSnapped;
    std::function<void()> onLaid;
    std::function<void()> onHover;
    std::function<void()> onHoverOut;
    
    int _count;
    
    Sprite *_dropLand;
    Sprite *_shadowLand;
    
    Vec2 _startPos;
    Vec2 _originPos;
    Vec2 _touchPos;
    Node *_targetNode;
    
    bool _snapped;
    bool _picked;
    
    bool _touchEnabled;
};

class WaterDropTankPiece : public Node
{
public:
    
    CREATE_FUNC(WaterDropTankPiece);
    
    virtual bool init() override;
    void setTexture(int count);
    
    void setPicked(bool picked);
    
    void pickTarget();
    void snapTarget();
    void layTarget();
    void hoverTarget();
    void hoverOutTarget();
    
    std::function<void()> onPicked;
    std::function<void()> onSnapped;
    std::function<void()> onLaid;
    std::function<void()> onHover;
    std::function<void()> onHoverOut;
    
    int _count;
    
    Sprite *_dropTank;
    Sprite *_shadowTank;
    
    Vec2 _originPos;
    Vec2 _touchPos;
    Node *_targetNode;
    
    bool _snapped;
    bool _picked;
    
    bool _touchEnabled;
};


#endif /* PlaceValueScene_hpp */
