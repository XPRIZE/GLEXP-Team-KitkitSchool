//
//  MultiplicationBoardScene.hpp
//
//  Created by harunom on 7/21/18.
//
//

#ifndef MultiplicationBoardScene_hpp
#define MultiplicationBoardScene_hpp

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


struct MultiplicationBoardLevelStruct;
class MultiplicationBoardPiece;

class MultiplicationBoardScene : public Layer
{
public:
    static Scene* createScene(string levelID);
    virtual bool init() override;
    CREATE_FUNC(MultiplicationBoardScene);

    virtual void onEnter() override;
    virtual void onEnterTransitionDidFinish() override;
    void onStart();
    void showBoard(int index);
    void createBoard(int index);
    void onBoardComplete();
    
    void makeExpression(bool isPick, bool isComplete);
    void makeCompleteExpression();
    float getCompleteExpressionAniTime();
    
    void setLevel(int level);
    void loadData(int level, int *workSheetID);
    void playOnLampSound();
    
    void createShiningParticle();
    void startParticle();
    void stopParticle();
    void setLink();
    
private:

    void loadDurationsheet();
    float getDuration(string name);
    
public:
    Node *getCurGuide();
    
private:
    Node *_gameNode;
    Node *_shiningParticleNode;
    Node *_expressionNode;

    vector<MultiplicationBoardPiece*> _multiplePieces;
    vector<Node*> _guides;
    
    int _numSnappedPieces;
    
    int _multiplicand;
    int _multiplier;
    int _product;

    int _currentLevel;
    int _currentSheetNo;
    int _currentBoardIndex;

    ProgressIndicator *_progressBar;

    vector<MultiplicationBoardLevelStruct> _boards;

    std::map<std::string,float> _duration;
    
    bool _isSkip;

};


struct MultiplicationBoardLevelStruct {
    int level;
    int stage;
    int worksheet;
    int multiplicand;
    int multiplier;
    int product;
};

class MultiplicationBoardPiece : public Node
{
public:

    CREATE_FUNC(MultiplicationBoardPiece);
    
    virtual bool init() override;
    void setTexture(int multiplicand, int multiplier);
    void setPicked(bool picked);
    
    void setLightOn();
    void setLightOff();
    bool CheckAllLightOn();
    
    void pickTarget();
    void snapTarget();
    void layTarget();
    
    std::function<void()> onPicked;
    std::function<void()> onSnapped;
    std::function<void()> onLaid;

    
    int _count;
    
    Node *_bulb;
    Node *_bulb_light;
    Node *_shadow;
    
    Vec2 _originPos;
    Vec2 _targetPos;
    
    Node *_targetNode;
    
    bool _snapped;
    bool _picked;
    
    bool _touchEnabled;
};


#endif /* MultiplicationBoardScene_hpp */
