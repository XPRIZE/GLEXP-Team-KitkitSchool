//
//  AnimalPuzzleScene.hpp
//
//  Created by Gunho Lee on 12/10/16.
//
//

#ifndef AnimalPuzzleScene_hpp
#define AnimalPuzzleScene_hpp

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


struct AnimalPuzzleLevelStruct;
class AnimalPiece;

class AnimalPuzzleScene : public Layer
{
public:
    static Scene* createScene(string levelID);
    virtual bool init() override;
    CREATE_FUNC(AnimalPuzzleScene);

    virtual void onEnter() override;
    virtual void onEnterTransitionDidFinish() override;
    void onStart();
    void showPuzzle(int index);
    void onPuzzleComplete();
    
    void setLevel(int level);
    
    void createPuzzle(int index);
    void loadData(int level);
    
    void playSound(string name);
    
private:

    void loadDurationsheet();
    float getDuration(string name);
    
    
private:
    Node *_gameNode;


    Node *_pieceNode;
    Node *_depthNode;
    
    

    vector<AnimalPiece*> _animalPieces;
    int _numSnappedPieces;

    int _currentLevel;
    int _currentPuzzleIndex;

    ProgressIndicator *_progressBar;

    vector<AnimalPuzzleLevelStruct> _puzzles;


    std::map<std::string,float> _duration;
    
    

};

struct AnimalPuzzleLevelStruct {
    string text;
    string backgroundFilename;
    string folderName;
    string maskFilename;
    string soundFilename;
    vector<string> pieceFaceVector;
    vector<string> pieceDepthVector;
    vector<string> pieceShadowVector;
    vector<Point> piecePosVector;
};

class AnimalPiece : public Node
{
public:

    CREATE_FUNC(AnimalPiece);
    
    virtual bool init() override;
    
    void setTexture(string face, string body, string shadow);
    
    void setPicked(bool picked);
    
    void snapTarget();
    void layTarget();
    std::function<void()> onSnapped;
    std::function<void()> onLaid;

    
    
    int _pieceId;
    Sprite *_face, *_body, *_shadow;
    Vec2 _targetPos;
    bool _snapped;
    
    Sprite *_ghost;
    bool _touchEnabled;
    
    
    
};


#endif /* AnimalScene_hpp */
