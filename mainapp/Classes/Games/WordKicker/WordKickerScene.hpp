//
//  WordKickerScene.hpp
//  KitkitSchool
//
//  Created by MilliSoftGames on 8/14/18.
//
//

#ifndef WordKickerScene_hpp
#define WordKickerScene_hpp

#include "WordKickerHeader.hpp"
#include "Common/Controls/ProgressIndicator.hpp"

BEGIN_NS_WORDKICKER;

struct WordKickerProblemStruct;

class WordKicker_NodeGoalpost;
class WordKicker_NodeBall;
class WordKicker_NodeBirdGoalkeeper;
class WordKicker_NodeBirdKicker;

enum GAMETYPE {
    WORDKICKER,
    MATHKICKER
};

class WordKickerScene : public Layer
{
public:
	CREATE_FUNC(WordKickerScene);
    static Scene* createScene(string levelID, GAMETYPE type=WORDKICKER);

    virtual bool init() override;
    virtual void onEnter() override;
    virtual void onEnterTransitionDidFinish() override;
	virtual void onExit() override;

private:
    void startGame();
	void startGame_show();
	void startGame_NextProblem();

	void makeSpeakerButton();
	void resetBirdMarkInBalloon();
	bool touchSpeakerButton(bool bIsKicker, bool bResetBirdMark);
	//void showAnimation_kicker_comein();
	
	void makeExampleButtons();
	void makeExampleButtons_sub(Scale9Sprite* spriteParent, Label* labelExample, float fPosX, float fPosY);
	
	void showProblem();
	void showAnswerBalloon();
	void showTimeoverBalloon();
	void calcBalloonSize(float fLabelsWidth);
	float makeProblemLabels(string strSentence, int nAnswerStartPos, int nAnswerLength, float fLabelPosY, bool bShowAnswer, bool bIsTimeover);
	void makeExamples();
	void resetExampleButtons(bool bOn);

	void startTimer();
	void stopTimer();
	void onTimerTick(float fDeltaTime);


	void onExampleSelected(Scale9Sprite* spriteParent, bool bMakeLog);
	void start_wronganswer(Scale9Sprite* wrongAnswer);
	void start_timeover();
	void start_correctanswer_1();
	void start_correctanswer_2();
	void start_correctanswer_final();

	void onTick_wronganswer(float fDeltaTime);
	void onTick_timeover(float fDeltaTime);
	void onTick_correctanswer_1(float fDeltaTime);
	void onTick_correctanswer_2(float fDeltaTime);
	void onTick_correctanswer_final(float fDeltaTime);

    void setLevel(int level);
    
    void loadData(int level,int *worksheetID);
	void loadDurationsheet();
	float getDuration(string name);
    
    

private:
	Node* _gameNode;

	enum eGameState {
		WORDKICKER_NONE = 0,
		WORDKICKER_PLAYING,
		WORDKICKER_TIMEOVER,
		WORDKICKER_WRONGANSWER,
		WORDKICKER_CORRECTANSWER_1,
		WORDKICKER_CORRECTANSWER_2,
		WORDKICKER_CORRECTANSWER_FINAL,
	};
	eGameState _gameState = WORDKICKER_NONE;
	float _gameStateTime = 0;


	WordKicker_NodeGoalpost* _nodeGoalpost;
	WordKicker_NodeBall* _nodeBall;
	WordKicker_NodeBirdGoalkeeper* _nodeBirdGoalkeeper;
	WordKicker_NodeBirdKicker* _nodeBirdKicker;

	Node* _speachballoonNode;
	Scale9Sprite* _speachballoonSprite;
	Sprite* _sprite_speaker;
	bool _speakerPlaying;
	unsigned int _speakerSoundID;
	Action* _actionSpeakerPlaying;
	Sprite* _sprite_balloonMark_kicker;
	Sprite* _sprite_balloonMark_goalkeeper;
	Node* _node_speachballoon_labels;

	float _maxBalloonLabelWidth;
	float _minExampleLabelWidth;
	float _maxExampleLabelWidth;
	float _curExampleButtonWidth;

	Node* _examplesNode;
	Scale9Sprite* _sprite_example_1;
	Scale9Sprite* _sprite_example_2;
	Scale9Sprite* _sprite_example_3;
	Scale9Sprite* _sprite_example_4;
	Scale9Sprite* _sprite_example_answer;
	vector<string> _exampleStringList;
	Label* _label_example_1;
	Label* _label_example_2;
	Label* _label_example_3;
	Label* _label_example_4;



	Label* _label_debug_timer;
	int _remainTime;


	vector<WordKickerProblemStruct> _problems;

    int _currentLevel;
	int _currentWorkSheet;
	int _currentProblemIndex;

    ProgressIndicator *_progressBar;
    std::map<std::string,float> _duration;
    
    string _gameName;
    string _resourcePath;
    
};

END_NS_WORDKICKER;

#endif /* WordKickerScene_hpp */
