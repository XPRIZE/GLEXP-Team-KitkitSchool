//
//  LineMatchingScene.h
//  todomath
//
//  Created by KimDonggyu on 2016. 5. 4..
//
//

#ifndef LineMatchingScene_h
#define LineMatchingScene_h

#include <stdio.h>
#include "cocos2d.h"
#include "Managers/GameSoundManager.h"
#include "GameObject.h"
#include "JsonParseData.h"
#include "Utils/TodoUtil.h"
#include "Common/Controls/ProgressIndicator.hpp"

USING_NS_CC;
using namespace std;
using namespace ui;

class LineMatchingScene : public Layer {
public:
    CREATE_FUNC(LineMatchingScene);
    
    LineMatchingScene();
    ~LineMatchingScene();
    
    virtual bool init() override;
    virtual void onEnter() override;
    virtual void onExit() override;
    
    virtual void prepareNewGameWithLevel(Json::Value problemParameter);
    virtual void handleStart();
    virtual void handlePause();
    virtual void handleQuit();
    virtual void handleResume();
    virtual void handleAnswerEntered(std::string &answer);
    virtual void showHint();
    virtual void hideHint();
    
    virtual bool onTouchBegan(cocos2d::Touch *touch, cocos2d::Event *event) override;
    virtual void onTouchMoved(cocos2d::Touch *touch, cocos2d::Event *event) override;
    virtual void onTouchEnded(cocos2d::Touch *touch, cocos2d::Event *event) override;
    
private:
    ProgressIndicator *_progressBar;
    
    void checkLevelClearDelay();
    void checkLevelClear(float);
    
    // 문제 생성
    void setProblemInit(float);
    void setProblem(float);
    void set_2Dan(JsonParseData *parseObject, int _useObjectCount, int _useColorCount);
    void set_3Dan(JsonParseData *parseObject, int _useObjectCount, int _position);
    
    // 오브젝트 생성
    void createBox();
    std::vector<int> boxSetRandom(std::vector<int> vecDistanceNum);
    void createDot();
    void createDrawNode();
    void drawObject();
    
    // 터치 영역 검사
    Rect setDotRect(Sprite *spriteDot);
    
    // 튜토리얼
    void setTutorial();
    
    // 파티클
    float playParticleEffect();
    void setChangeParticleColor(Node *pSender);
    
    // 정리
    void resultTheorem();
    void changeObjectTexture();
    
    // DrawNode 생성, 삭제
    void drawLine(float dt);
    void removeLine(float dt);
    
    void showComplete(float);
    
private:
    Size m_winSize;
    EventListenerTouchOneByOne *m_listener;
    
    // 벡터
    std::vector<std::string> m_vecUseObject;        // 기본적으로 사용될 오브젝트 리스트
    std::vector<std::string> m_vecUseColor;         // 기본적으로 사용될 색상 리스트
    std::vector<std::string> m_vecSuccessObject;    // 정답을 맞춘 오브젝트
    std::vector<Json::Value> m_vecEssentialObject;  // 필수 오브젝트 리스트
    std::vector<Json::Value> m_vecMultiObject;      // 다중 오브젝트 리스트
    std::vector<GameObject*> m_vecGameObject;       // 실제 게임에 필요한 오브젝트들 (Box, Dot, Object 등등)
    std::vector<JsonParseData*> m_vecParseData;     // 게임을 구성하기 위한 Json 파일을 파싱한 클래스
    std::vector<Vec2> m_vecLerpPosition;
    Vector<DrawNode*> m_vecInDrawNode;
    Vector<DrawNode*> m_vecOutDrawNode;
    Vector<DrawNode*> m_vecTutorialDrawNode;
    std::vector<int> m_vecDrawIndex;                // 정답 DrawNode Index
    std::vector<Vec2> m_vecStartPosition;           // 정답 DrawNode 시작점
    std::vector<Vec2> m_vecFinishPosition;          // 정답 DrawNode 도착점
    Vector<Node*> m_vecRemoveObject;                // 삭제해야 할 오브젝트
    
    // ================================================================================================================= //
    
    // 게임관련
    int m_objectCount = 0;                          // 생성할 클래스 갯수(2x4 or 3x4)
    int m_problemCount = 0;                         // 문제 수
    int m_problemGroupCount = 0;                    // 한 화면에 보여지는 라인을 전부 맞추는 것을 하나의 문제라고 했을 때의 문제 갯수
    int m_successCount = 0;
    int m_resultType = 0;                           // onTouchEnded 결과 값(None, Incorrect, Answer)
    int m_distanceNum = 0;
    float m_objectScale = 1;
    
    // 인덱스
    int m_getIndex = 0;                             // m_vecParseData 벡터의 인덱스
    int m_drawIndex = 0;                            // DrawNode 인덱스 (vector)
    int m_lerpIndex = 0;                            // DrawNode 보간 인덱스 (vector)
    int m_tutorialIndex = 0;                        // 튜토리얼 인덱스
    
    // 비교
    bool m_isTutorial = false;                      // 튜토리얼이 있는지?
    bool m_isRandomLine = false;                    // 문제의 유형이 단순형인지, 랜덤형인지
    bool m_isTouchFlag = false;
    bool m_isDotTouch = false;
    bool m_isTouchMoved = false;                    // 터치 후 움직였는지
    bool m_isMoveDrawLine = false;
    bool m_isDrawSound = false;                     // 사운드가 재생되었는지
    
    // 객체
    std::string m_answer;  // NB(xenosoz, 2018): Node* m_nodeAnswer가 항상 dangling pointer를 가리키고 있어서 string으로 치환하였습니다 (_ _)

    Sprite *m_spriteTutorialHand;
    GameObject *m_firstObject;                      // 처음 선택된 GameObject class 객체
    GameObject *m_secondObject;                     // 마지막으로 선택된 GameObject class 객체
    GameObject *m_hintObject;
    Sequence *m_actionSeq;                          // 힌트 액션
    
    // 그리기
    DrawNode *m_whiteDrawNode;
    DrawNode *m_inDrawNode;
    DrawNode *m_outDrawNode;
    
    Color4F m_selectColor;
    Color4F m_successColor  = Color4F(0.31f, 0.53f, 0.24f, 1);
    
    // 위치
    Vec2 m_startPosition;
    Vec2 m_finishPosition;
    Vec2 m_firstDotAnchorPoint;
    Vec2 m_secondDotAnchorPoint;
    
    // 라인매칭 실패 여부
    bool _isMissed;
    
    // ================================================================================================================= //
    
    enum eDotTouch{
        Z_DISABLE = 0,
        Z_ENABLE,
    };
    
    enum eZOrder{
        Z_BOX = 0,
        Z_PICTURE,
        Z_DOT,
        Z_SUCCESS_DRAW,
        Z_HALF_SUCCESS_DRAW,
        Z_DRAW,
        Z_STAR,
        Z_TUTORIAL
    };
    
    enum eAnswerType{
        T_NONE = 0,
        T_ANSWER,
        T_INCORRECT
    };
    
    // 추가요소
    int _currentProblemIndex;
    Node* _gameNode;
    Node* _problemViewNode;
};

#endif /* LineMatchingScene_h */
