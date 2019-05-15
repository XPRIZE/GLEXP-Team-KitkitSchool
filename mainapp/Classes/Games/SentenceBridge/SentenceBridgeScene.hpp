//
//  SentenceBridgeScene.hpp
//  KitkitSchool
//
//  Created by MilliSoftGames on 7/26/18.
//
//

#ifndef SentenceBridgeScene_hpp
#define SentenceBridgeScene_hpp

#include "SentenceBridgeHeader.hpp"
#include "Common/Controls/ProgressIndicator.hpp"

BEGIN_NS_SENTENCEBRIDGE;

struct SentenceBridgeProblemStruct;
class NodeBridgeMain;
class NodeBlockMain;
class NodeBlock;

class SentenceBridgeScene : public Layer
{
public:
	CREATE_FUNC(SentenceBridgeScene);
    static Scene* createScene(string levelID);

    virtual bool init() override;
    virtual void onEnter() override;
    virtual void onEnterTransitionDidFinish() override;
	virtual void onExit() override;

private:
    void startGame();
	void makeSpeakerButton(Size winSize);
	bool touchSpeakerButton(float delayTime);
	void processBlockTouch();

	void showSpeakerButton();
	void hideSpeakerButton();
	void showBridge(int index);
	void onCorrectAnswer(NodeBlock* blockNode, int foundSentenceIdx);
	void onBridgeComplete();
	void onBridgeComplete_step2();

	void createShiningParticle();
	void startParticle(Vec2 worldPos);
	void stopParticle();


    void setLevel(int level);

	void loadData(int level,int *worksheetID);
	void loadDurationsheet();
	float getDuration(string name);
    float getCurrentDuration();

    

private:
	Node* _gameNode;
	NodeBlockMain *_blockMainNode;
	NodeBridgeMain *_bridgeMainNode;

	Node* _shiningParticleNode;

	vector<SentenceBridgeProblemStruct> _problems;

    int _currentLevel;
	int _currentWorkSheet;
	int _currentProblemIndex;

    ProgressIndicator *_progressBar;
    std::map<std::string,float> _duration;
    
	Sprite* _speakerButton;
	bool _touchEnabled;
	bool _speakerPlaying;
    
    bool _blockMoving;
};

END_NS_SENTENCEBRIDGE;

#endif /* SentenceBridgeScene_hpp */
