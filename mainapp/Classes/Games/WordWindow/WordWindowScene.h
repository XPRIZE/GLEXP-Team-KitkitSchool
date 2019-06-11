#ifndef WordWindow2Scene_hpp
#define WordWindow2Scene_hpp

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

struct WordWindowLevelStruct
{
	string m_languageTag;
	int m_level;
	int m_worksheet;
	int m_problemNo;
	int m_sequenceType;
	string m_soundFilename;
	string m_text;
	string m_text1;
	string m_exampleNumber[4];
	string m_exampleWord[4];
	string m_rightFormula;
	string m_rightFormulaShadow;
	int m_rightAnswer;
	int m_objectValue[4];
 	int m_equationValueA;
 	int m_equationValueB;
 	string m_equationValueSign;
 	string m_equationUnknown;
	string m_objectSource[4];

	WordWindowLevelStruct();
	int getTotalObjectCount();
	int getSubtractionResult();
	int getLastObjectCountIndex();
};

struct WordWindowRightFormulaText
{
	bool m_isBold;
	string m_text;

	WordWindowRightFormulaText();
};

class WordWindowScene : public Layer
{
public:
	enum
	{
		SEQUENCE_TYPE_NONE,

		SEQUENCE_TYPE_ADDITION,		// 덧셈
		SEQUENCE_TYPE_SUBTRACTION,	// 뺄셈
		SEQUENCE_TYPE_MULTIPLY,		// 곱셈
		SEQUENCE_TYPE_COMPARISION,	// 비교
		SEQUENCE_TYPE_INCLUSION,	// 포함
		SEQUENCE_TYPE_EQUATION,		// 방정식

		SEQUENCE_TYPE_MAX,
	};
    void speech1(string input);

public:
    static Scene* createScene(string levelID);
    virtual bool init() override;
    CREATE_FUNC(WordWindowScene);

	WordWindowScene();
	virtual ~WordWindowScene();

    virtual void onEnter() override;
    virtual void onEnterTransitionDidFinish() override;
	virtual void update(float dt) override;
    void onStart();
    void showPuzzle(int index);
	void resetPuzzle();
    void onPuzzleComplete();
	bool isFinishedLevel();
	void onFinishedLevel(float dt);
    void setLevel(int level);
    void createPuzzle(int index);
    void loadData(int level);
    
private:
	// 미리 리소스 로딩
	void preloads();

	// 배경
	void createBackground();

	// 칠판
	void createBoard();
	void releaseBoard();
	void onTouchedBoard();
	void playBoardShake();
	void playBoardFlip1();
	void onEndPlayBoardFlip1();
	void playBoardFlip2(float delay);
	void onStartPlayBoardFlip2();
	void onEndPlayBoardFlip2();
	void showBoardContents();
	void hideBoardContents();

	// 사운드 버튼 생성
	void createSoundButton();
	void enableSoundButton();
	void disableSoundButton();
	void releaseSoundButton();
	void updateSound(float dt);
	void stopSoundQuestion();

	// 문제 생성
	void createQuestion();
	void releaseQuestion();

	// 보기 생성
	void createExamples();
	void releaseExamples();
	void hideExamples();
	void enableExamples();
	void disableExamples();
	void transparentExamples();
	void setOpacityExamples(int index, int opacity);
	void onAnswer(int index);
	void hideExamplesLabel();
	void showExamplesLabel(bool isRunAction);
	void startFirstDisableExamples();
	void endFirstDisableExamples();
	void updateFirstDisableExamples(float dt);

	// 아무런 터치가 없는 시간 업데이트
	void updateNoTouchTime(float dt);

	// 시퀀스
	void resetSequence();
	void playHintSimpleSequence();
	void playHintSequence();
	void onEndSequence(float dt);
	void releaseSequence();

	// 고양이 손 선택
	void selectCatType();
	bool isSelectedCatType(int catType);
	bool isCat();
	bool isTiger();
	bool isBear();
	std::string getCatFilename();
	std::string getCatFilename2();
	
	// 연출용 함수들
	void calculateStartPosY();
	void calculateCurLine();
	void calculateCurLineSub();
	void addAccumulateCount(int add);
	float getCurLinePosY();
	float getTopPosY();
	float getBottomPosY();
	void setSpriteStone(Sprite* pSpriteStone);

	// 덧셈
	void playHintSimpleSequenceAddition();
	void playHintSequenceAddition();
	void playAddStoneAll(int index, int objectCount);
	void playAddStone(const std::string& resourceName);
	void playAddStonePack(const std::string& resourceName);
	bool isPackAddition(int objectCount);

	// 뺄셈
	void playHintSimpleSequenceSubtraction();
	void playHintSequenceSubtraction();
	void playSubStoneAll(int index, int objectCount);
	void playSubStone(const std::string& resourceName);
	void playSubStonePack(const std::string& resourceName);
	bool isPackSubtraction(int index, int objectCount);

	// 곱셈
	void playHintSimpleSequenceMultiply();
	void playHintSequenceMultiply();

	// 비교
	void playHintSimpleSequenceComparison();
	void playHintSequenceComparison();
	void playAddStoneAllComparisonSimple(int index, int objectCount, bool isTopOver);
	void playAddStoneComparisonSimple(int index, bool isTopOver, const std::string& resourceName);
	void playAddStonePackComparisonSimple(int index, bool isTopOver, const std::string& resourceName);
	void playAddStoneAllComparison(int index, int objectCount, bool isTopOver);
	void playAddStoneComparison(int index, bool isTopOver, const std::string& resourceName);
	void playAddStonePackComparison(int index, bool isTopOver, const std::string& resourceName);

	// 포함
	void playHintSimpleSequenceInclusion();
	void playHintSequenceInclusion();

	// 방정식
	void playHintSimpleSequenceEquation();
	void playHintSequenceEquation();
	void addStoneAtCard(Sprite* pSpriteCard, int count);
	void selectStone();
	std::string getStoneName();

	// 정답
	void playRightAnswerEffect(int index, float time);
	void onFinishAnswer(float dt);
	void onPlaySoundAnswer(float dt);
	void createRightFormula();
	void releaseRightFormula();
	void createShiningParticle(const Vec2& pos);
	void releaseShiningParticle();
	void onStopParticle(float dt);

	// 게임 속도
	void setGameSpeed(float speed);
	void resetGameSpeed();

	// 정답 효과음
	void playSoundSolve();
	void playSoundBoardFlip();
	void playSoundBoardShake();
	void playSoundAddObject();
	void playSoundAddObjectPack();
	void playSoundSubObject();
	void playSoundSubObjectPack();
	void playSoundPutObjectRow();
	void stopSoundPutObjectRow();
	void playSoundFlipCard();
	void playSoundAddCard();
	void playSoundCat(float dt=0.f);
	void playSoundTiger(float dt = 0.f);
	void playSoundBear(float dt = 0.f);
	void playSoundAnimal(int index, float delay);

	// 사운드 플레이 시간 
	void loadDurationsheet();
	float getDuration(string name);

	// 사운드 플레이
	void playSoundQuestion(string name);

	// 유틸
	std::vector<std::string> componentsSeparatedByString(const std::string& str, const std::string& seperator);
	void getAutoFontSize(const std::string& strText, const std::string& fontName, const Size boxSize, bool isLineBreakWithoutSpace, int& fontSize);
	void getRightFormulaTexts(const std::string& text, std::vector<WordWindowRightFormulaText>& vecOutTexts);

	void resetTouch();
    
private:
	// 설정 변수들
	float m_totalAnimationTime;			// 총 에니메이션 소요되는 시간
	
	float m_boardFlipDelayPerUnit;		// 칠판 플립될 때 프레임당 시간
	float m_boardShakeDelayPerUnit;		// 칠판 흔들리는 프레임당 시간

	float m_sequenceBeforeTerm;			// 시퀀스 시작 전 대기 시간

	float m_catDownTime;				// 고양이 손 내려가는 시간
	float m_catUpTime;					// 고양이 손 올라가는 시간
	float m_catSoundTerm;				// 고양이 효과음 이후 텀

	float m_stoneIntervalDelay;			// 다음 돌을 놓을 때 약간의 텀
	float m_stoneSimpleDelay;			// 돌 놓을 때 딜레이 값(약식)
	float m_stoneDelay;					// 돌 놓을 때 딜레이 값(고양이 손 포함)

	float m_equationCardFlipTime;		// 방정식 카드 뒤집어 지는 시간

	float m_backBoardDelay;				// 다시 칠판으로 돌아가는 텀

	int m_otherCatRandom;				// 고양이 손 아닌 다른 손이 선택될 확률
	float m_rightEffectTime;			// 정답 이펙트 시간
	float m_shiningParticleInterval;	// 정답 수식 위에 반짝이는 연출 간격
	float m_rightFormulaEffectTime;		// 정답 수식 위에 연출 시간

	int m_currentLevel;
	int m_currentPuzzleIndex;
	vector<WordWindowLevelStruct> m_vecPuzzles;
	WordWindowLevelStruct m_curLevelStruct;
	float m_noTouchTime;

	std::map<std::string, float> m_mapSoundDuration;
	unsigned int m_soundQuestionID;
	
	// 프로그래스 인디게이트
	ProgressIndicator* m_pProgressBar;

	Node *_gameNode;

	Sprite* m_pSpriteBackground;
	Sprite* m_pSpriteBoard;
	Button* m_pButtonBoard;
	Button* m_pButtonSound;
	Label* m_pLabelQuestion;
	Button* m_pButtonExample[4];
	Label* m_pLabelExampleTop[4];
	Label* m_pLabelExampleBottom[4];
	float m_soundDuraton;

	bool m_isFirstDisableExamples;
	float m_firstDisableExamplesTime;

	// 힌트 시퀀스
	bool m_isHintSequence;
	bool m_isSimpleSequence;
	bool m_isStonePack;
	int m_remainder;
	int m_accumulateCount;
	float m_accumulateDelay;
	std::vector<int> m_vecSelectedCat;
	int m_curSelectedCatType;
	int m_curSelectedCatType2;
	Node* m_pNodeSequence;
	Sprite* m_pSpriteStone[40];
	int m_spriteStoneIndex;
	float m_sequenceStartY;
	int m_curSequenceLine;
	int m_selectedStone;

	// 결과
	int m_wrongAnswer;
	bool m_isRightAnswer;
	Node* m_pNodeRightFormula;
	ui::RichText* m_pRichText;
	ui::RichText* m_pRichTextShadow;

	// 파티클
	std::vector<Node*> m_vecParticleNodes;

	// 사운드
	SoundEffect m_soundEffectPutObjectRow;

	bool m_isTouchBoard;
	bool m_isTouchAnswer;
	int m_touchAnswerIndex;

};

#endif /* WordWindow2Scene_hpp */
