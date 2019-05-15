//
//  LabelingScene.hpp
//
//  Created by Seungbum Son on 9/5/18.
//
//

#ifndef LabelingScene_hpp
#define LabelingScene_hpp

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


struct LabelingLevelStruct;
class LabelingPiece;
class SlotPiece;

class LabelingScene : public Layer
{
public:
    static Scene* createScene(string levelID);
    virtual bool init() override;
    CREATE_FUNC(LabelingScene);

    virtual void onEnter() override;
    virtual void onEnterTransitionDidFinish() override;
    void onStart();
    void showPuzzle(int index);
    void onPuzzleComplete();
    
    void blockTouches(int targetId);
    void unblockTouches();
    
    void setLevel(int level);
    
    void createPuzzle(int index);
    void loadData(int level);
    
    void playSound(string name);
    
    void sendLog(int userPick, int correctPick);

    bool isWrongAnswer(LabelingPiece* targetPiece);
    int isWithinBoundary(LabelingPiece* targetPiece);
    
    vector<LabelingPiece*> _labelingPieces;
    vector<SlotPiece*> _slotPieces;
    
    int _currentLevel;
    int _currentPuzzleIndex;
    
private:

    void loadDurationsheet();
    float getDuration(string name);
    
    
private:
    Node *_gameNode;
    Node *_bgNode;

    Node *_pieceNode;
    Node *_depthNode;
    
    vector<LabelingLevelStruct> _labels;

    
    int _numSnappedPieces;

    ProgressIndicator *_progressBar;


    std::map<std::string,float> _duration;
    
    

};




struct LabelingLevelStruct {
    string mainImageFilename;
    vector<string> wordTextVector;
    vector<Point> answerPosVector;
    vector<string> voiceFilenameVector;
};

class LabelingPiece : public Node
{
public:

    CREATE_FUNC(LabelingPiece);
    
    virtual bool init() override;
    
    void setText(string text);
    
    void setPicked(bool picked);
    
    void snapTarget();
    void returnTarget();
    void layTarget();
    void showFlash();
    void scaleTo(float scaleValue);
    std::function<void()> onTouchStart;
    std::function<void()> onSnapped;
    std::function<void()> onReturn;
    std::function<void()> onMoved;
    
    int _labelId;
    Sprite *_labelSprite, *_tagSprite, *_flashBodySprite, *_flashTagSprite;
    Label *_textLabel;
    Vec2 _startPos;
    Vec2 _targetPos;
    Size _targetSize;
    bool _snapped;
    bool _scaled;
    
    Color3B _labelColor;
    bool _touchEnabled;
    
    string _voiceFilename;
    
};

class SlotPiece : public Node
{
public:
    
    CREATE_FUNC(SlotPiece);
    
    virtual bool init() override;
    
    void setText(string text);
    void setTagColor(Color3B color);
    void revealText();
    void showHighlight(bool _show);
    
    int _slotId;
    Size _slotSize;
    Sprite *_headSprite, *_headColorSprite, *_bodySprite, *_animationSprite, *_flashSlotSprite, *_highlightSprite;
    Label *_textSlotLabel;
    bool _isTheme;
    bool _isHighlighted;
    
};

#endif /* LabelingScene_hpp */
