//
//  HundredPuzzleScene.hpp
//
//  Created by Gunho Lee on 12/10/16.
//
//

#ifndef HundredPuzzleScene_hpp
#define HundredPuzzleScene_hpp

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


class HundredPiece;

class HundredPuzzleScene : public Layer
{
public:
    static Scene* createScene(int levelID);
    virtual bool init() override;
    CREATE_FUNC(HundredPuzzleScene);


    virtual void onEnter() override;
    virtual void onEnterTransitionDidFinish() override;
    void onStart();


    
    
    void setLevel(int level);
    void setTouchEnabled(bool enabled);
    
private:


    Point getNumPoint(int num);
    
    void setTargetForPiece(HundredPiece *piece);

    
    void createPieces();
    vector<int> pickNumbers(int num);
    vector<int> pickRect(int w, int h);
    vector<int> pickCross();
    
    
    void fillBoard();
    
    
private:
    Node *_backNode;
    Node *_gameNode;
    
    Node *_gameBoard;
    vector<HundredPiece*> _pieces;
    

    int _currentLevel;
    ProgressIndicator *_progressBar;
    
    bool _picked[100];
    
    int _currentColorIndex;
    vector<Color3B> _colors = { Color3B(0, 135, 60), Color3B(240, 123, 36), Color3B(26, 220, 178),
        Color3B(219, 42, 42), Color3B(125, 55, 204), Color3B(240, 188, 0),
        Color3B(129, 240, 0), Color3B(85, 163, 247), Color3B(45, 90, 227), Color3B(241, 108, 195)};

};


class HundredPiece : public Node
{
public:
    CREATE_FUNC(HundredPiece);
    
    virtual bool init() override;
    

    void setPieces(vector<int> &numbers);
    void setPicked(bool picked);
    void setSnapped(bool snapped);
    bool isSnapped() { return _snapped; }
    
    void setColor(Color3B color);
    

    int _originNum;
    
    Vec2 _targetPos;

    std::function<void()> onSnapped;
    
protected:
    Node *_colorMask, *_face, *_body, *_shadow;
    Vec2 _spriteCenter;
    
    bool _snapped;
    
        
    
    
    
    
    
};


#endif /* HundredScene_hpp */
