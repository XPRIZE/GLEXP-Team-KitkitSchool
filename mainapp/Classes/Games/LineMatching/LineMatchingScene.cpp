//
//  LineMatchingScene.cpp
//  todomath
//
//  Created by KimDonggyu on 2016. 5. 4..
//
//

#include "LineMatchingScene.h"
#include "Common/Controls/CompletePopup.hpp"
#include "Common/Controls/TodoSchoolBackButton.hpp"
#include "Managers/UserManager.hpp"
#include "Managers/StrictLogManager.h"
#include "CCAppController.hpp"

namespace
{
    //const Size gameSize = Size(2560, 1800);
    const Size gameSize = Size(2560, 1800);
    const Color4F kYellowColor = Color4F(255.f/255.f, 200.f/255.f, 36.f/255.f, 1.f);
    const Color4F kBlueColor = Color4F(36.f/255.f, 197.f/255.f, 255.f/255.f, 1.f);
    const float kLineThickness = 20.f;
}

LineMatchingScene::LineMatchingScene():
_currentProblemIndex(0)
{
    
}

LineMatchingScene::~LineMatchingScene(){
    // 벡터 비움
    m_vecUseObject.clear();
    m_vecUseColor.clear();
    m_vecSuccessObject.clear();
    m_vecEssentialObject.clear();
    m_vecMultiObject.clear();
    m_vecGameObject.clear();
    m_vecParseData.clear();
    m_vecLerpPosition.clear();
    m_vecInDrawNode.clear();
    m_vecOutDrawNode.clear();
    m_vecTutorialDrawNode.clear();
    m_vecDrawIndex.clear();
    m_vecStartPosition.clear();
    m_vecFinishPosition.clear();
    
    m_vecRemoveObject.clear();
    
    // action release
    m_actionSeq->release();
};

void LineMatchingScene::onEnter(){
    Layer::onEnter();
    m_listener = EventListenerTouchOneByOne::create();
    m_listener->setEnabled(false);
    m_listener->onTouchBegan = CC_CALLBACK_2(LineMatchingScene::onTouchBegan, this);
    m_listener->onTouchMoved = CC_CALLBACK_2(LineMatchingScene::onTouchMoved, this);
    m_listener->onTouchEnded = CC_CALLBACK_2(LineMatchingScene::onTouchEnded, this);
    this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(m_listener, this);
}

void LineMatchingScene::onExit(){
    Layer::onExit();
    
    SpriteFrameCache::getInstance()->removeUnusedSpriteFrames();
    Director::getInstance()->getTextureCache()->removeUnusedTextures();
    GameSoundManager::getInstance()->unloadAllEffect();
    
    this->removeAllChildren();
    this->unscheduleAllCallbacks();
    Director::getInstance()->getEventDispatcher()->removeEventListenersForTarget(this);
}

bool LineMatchingScene::init(){
    if (!Layer::init())
        return false;
    m_winSize = Director::getInstance()->getWinSize();
    this->setContentSize(m_winSize);
    
    {
        auto background = Sprite::create("LineMatching/line-matching_image_wooden-bgpng.png");
        background->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        background->setPosition(m_winSize / 2);
        this->addChild(background);
    }

    // 힌트 액션 생성
    ScaleBy *actionScaleBy = ScaleBy::create(0.25f, 1.07f);
    m_actionSeq = Sequence::create(actionScaleBy, actionScaleBy->reverse(), NULL);
    m_actionSeq->retain();
    
    m_firstObject = nullptr;
    m_secondObject = nullptr;
    
    return true;
}

#pragma mark - check stage clear condition
void LineMatchingScene::checkLevelClearDelay()
{
    // 1. m_successCount == m_problemCount : all level clear condition
    // 2. m_successCount == m_problemCount/2 : each level clear condition (not all level)
    if(m_successCount == m_problemCount/2 || m_successCount == m_problemCount)
    {
        this->scheduleOnce(CC_SCHEDULE_SELECTOR(LineMatchingScene::checkLevelClear), 1);
    }
}

void LineMatchingScene::checkLevelClear(float)
{
    if(m_successCount == m_problemCount)
    {
        showComplete(0.f);
    }
    else if(m_successCount == m_problemCount/2)
    {
        // 게임 구성 오브젝트들 삭제(박스, 도트, 오브젝트, 노드)
        for(int i=0; i<(int)m_vecRemoveObject.size(); i++)
        {
            this->removeChild(m_vecRemoveObject.at(i), true);
        }
        
        // 변수 초기화
        _currentProblemIndex++;
        m_getIndex++;
        m_drawIndex = 0;
        m_lerpIndex = 0;
        
        // 벡터 초기화
        m_vecGameObject.clear();
        m_vecUseObject.clear();
        m_vecUseColor.clear();
        m_vecSuccessObject.clear();
        m_vecEssentialObject.clear();
        m_vecMultiObject.clear();
        m_vecRemoveObject.clear();
        
        // 다음 레벨 생성
        this->setProblem(0);
        m_listener->setEnabled(true);
    }
}

void LineMatchingScene::prepareNewGameWithLevel(Json::Value problemParameter){
    // 도트의 개수, 또는 클래스 생성 개수
    m_objectCount = problemParameter["ObjectCount"].asInt();
    m_problemCount = problemParameter["ProblemCount"].asInt();
    m_isTutorial = problemParameter["IsTutorial"].asBool();
    
    // m_objectCount 8 -> ilnes 4*(8/4-1), m_objectCount 12 -> lines 4*(12/4-1)
    m_problemGroupCount = m_problemCount / ((m_objectCount/4 - 1)*4);
    
    // JsonParseData 클래스를 생성 후 벡터에 저장
    for(int i=0; i<2; i++){
        JsonParseData *object = new JsonParseData();
        object->m_setJson = problemParameter["Set_" + TodoUtil::itos(i)];
        m_vecParseData.push_back(object);
    }
 
    _progressBar = ProgressIndicator::create();
    _progressBar->setPosition(Vec2(m_winSize.width/2, m_winSize.height - _progressBar->getContentSize().height));
    _progressBar->setMax(m_problemGroupCount);
    addChild(_progressBar);
    
    {
        auto backButton = TodoSchoolBackButton::create();
        backButton->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
        backButton->setPosition(Point(25.f, m_winSize.height - 25.f));
        backButton->addClickEventListener([this](Ref*) {
            
        });
        this->addChild(backButton);
    }
    
    if (UserManager::getInstance()->isDebugMode()) {
        auto skip = ui::Button::create();
        skip->setTitleFontSize(100);
        skip->setTitleText("Skip");
        skip->setAnchorPoint(Vec2::ANCHOR_TOP_RIGHT);
        skip->setPosition(Vec2(m_winSize.width-25, m_winSize.height-25));
        addChild(skip);
        skip->addTouchEventListener([this](Ref*,ui::Widget::TouchEventType e) {
            if (e == ui::Widget::TouchEventType::ENDED) {
                m_successCount += (m_problemCount / 2);
                resultTheorem();
            }
        });
    }
    
    // 문제 생성
    this->scheduleOnce(schedule_selector(LineMatchingScene::setProblemInit), 1.f);
}

// ======================================================================================================================================================//

void LineMatchingScene::setProblemInit(float){
    
    this->setProblem(0);
    if(m_isTutorial)
        this->setTutorial();
    
    GameSoundManager::getInstance()->preloadEffect("LineMatching/Sounds/Boom.m4a");
    GameSoundManager::getInstance()->preloadEffect("LineMatching/Sounds/LineBack.m4a");
    GameSoundManager::getInstance()->preloadEffect("LineMatching/Sounds/LineStart.m4a");
    m_listener->setEnabled(true);
}

void LineMatchingScene::setProblem(float){
    
    //GameController::getInstance()->onReady();
    _progressBar->setCurrent(_currentProblemIndex + 1);
    
    JsonParseData *parseObject = m_vecParseData.at(m_getIndex);
    m_isRandomLine = parseObject->m_setJson["LineMatchingRandom"].asBool();
    
    // * 2단 문제 Json데이터
    if(m_objectCount == 8){
        int random = 0;
        bool isDuplication = true;
        std::vector<int> vecObjectDuplication;
        std::vector<int> vecColorDuplication;
        
        // 기본 카테고리
        int useObjectConut = parseObject->m_setJson["UseObjectCount"].asInt();          // 오브젝트 갯수
        bool isRandomCategory = parseObject->m_setJson["UseRandomCategory"].asBool();   // 카테고리 랜덤 섞기
        parseObject->m_categoryJson = parseObject->m_setJson["BasicCategory"];          // 사용되는 오브젝트 리스트
        
        // 사용할 오브젝트 벡터에 포함 (기본)
        for(int i=0; i<useObjectConut; i++){
            random = RandomHelper::random_int(0, (int)parseObject->m_categoryJson.size()-1);
            // 카테고리 선택 방식 (순차, 랜덤)
            if(!isRandomCategory)
                random = i % useObjectConut;
            
            // 오브젝트의 중복 방지
            isDuplication = true;
            for(int j=0; j<vecObjectDuplication.size(); j++){
                if(random == vecObjectDuplication.at(j)){
                    i--;
                    isDuplication = false;
                    break;
                }
            }
            if(isDuplication){
                vecObjectDuplication.push_back(random);
                Json::Value objectKeyValue = parseObject->m_categoryJson;
                Json::Value objectArray = objectKeyValue[random];
                
                // normal image
                m_vecUseObject.push_back(objectArray["nor"].asString());
                
                // [중요] success image
                m_vecSuccessObject.push_back(objectArray["succ"].asString());
            }
        }
        
        // ****************************************************************************************************************** //
        
        // 색상
        int useColorCount = parseObject->m_setJson["UseColorCount"].asInt();      // 색상 갯수
        bool isSameColor  = parseObject->m_setJson["Nor_Succ_Same"].asBool();     // nor, succ 색상 이름이 똑같은가? (기본 true로 설정)
        parseObject->m_colorJson = parseObject->m_setJson["UseColors"];           // 사용되는 색상 리스트
        
        // 1 스테이지 .. 등등 : "UseColors" keyName은 1차원 배열
        if(isSameColor){
            for(int i=0; i<useColorCount; i++){
                isDuplication = true;
                random = RandomHelper::random_int(0, (int)parseObject->m_colorJson.size()-1);
                for(int j=0; j<vecColorDuplication.size(); j++){
                    if(random == vecColorDuplication.at(j)){
                        i--;
                        isDuplication = false;
                        break;
                    }
                }
                if(isDuplication){
                    Json::Value colorKeyValue = parseObject->m_colorJson;
                    Json::Value colorArray = colorKeyValue[random];
                    
                    vecColorDuplication.push_back(random);
                    m_vecUseColor.push_back(colorArray.asString());
                }
            }
        }
        // 11, 12스테이지 : "UseColors" keyName은 key가 여러개 있는 2차원 배열
        else if(!isSameColor){
            Json::Value colorKeyValue = parseObject->m_colorJson;
            
            // 1. Max값에서 Choice만큼의 랜덤 값을 뽑는다.(중복X)
            int max = colorKeyValue["Max"].asInt();
            int choice = colorKeyValue["Choice"].asInt();
            std::vector<int> vecNotSameColor;
            while(vecNotSameColor.size() < choice){
                isDuplication = true;
                random = RandomHelper::random_int(0, max-1);
                for(int i=0; i<vecNotSameColor.size(); i++){
                    if(random == vecNotSameColor.at(i)){
                        isDuplication = false;
                        break;
                    }
                }
                if(isDuplication)
                    vecNotSameColor.push_back(random);
            }
            
            // 2. choice의 두배만큼 색상을 선택하여, m_vecUseColoe에 넣는다.
            int succIndex = 0;
            int randomIndex = 0;
            int vecObjectDupIndex = 0;
            
            // 2-1. vecSuccessObject에는 (기본)에서 경로가 저장이 되어있기 때문에, 임시로 다른 벡터에 저장해둔다.
            std::vector<std::string> vecTemp;
            vecTemp.resize((int)m_vecSuccessObject.size());
            std::copy(m_vecSuccessObject.begin(), m_vecSuccessObject.end(), vecTemp.begin());
            m_vecSuccessObject.clear();
            
            for(int i=0; i<choice * 2; i++){
                if(i == choice){
                    randomIndex = 0;
                    succIndex++;
                    vecObjectDupIndex++;
                }
                // 2-2. 고르는 순서는 vecObjectDuplication의 앞부터 고른다.
                int random = vecNotSameColor.at(randomIndex);
                int objectNum = vecObjectDuplication.at(vecObjectDupIndex);
                std::string _strObjectNum = TodoUtil::itos(objectNum);
                std::string norName = colorKeyValue[_strObjectNum][random]["nor"].asString();
                std::string succName = colorKeyValue[_strObjectNum][random]["succ"].asString();
                
                // [중요] push
                m_vecUseColor.push_back(norName);
                m_vecSuccessObject.push_back(vecTemp.at(succIndex) + succName);
                randomIndex++;
            }
            vecTemp.clear();
        }
        
        // ****************************************************************************************************************** //
        
        // 다중 오브젝트
        parseObject->m_categoryJson = parseObject->m_setJson["MultipleCategory"];
        for(int i=0; i<parseObject->m_categoryJson.size(); i++){
            m_vecMultiObject.push_back(parseObject->m_categoryJson[i]);
        }
        
        // 필수 오브젝트
        parseObject->m_categoryJson = parseObject->m_setJson["EssentialCategory"];
        for(int i=0; i<parseObject->m_categoryJson.size(); i++){
            m_vecEssentialObject.push_back(parseObject->m_categoryJson[i]);
        }
        vecObjectDuplication.clear();
        vecColorDuplication.clear();
        
        // 오브젝트 생성
        this->set_2Dan(parseObject, useObjectConut, useColorCount);
    }
    
    // * 3단 문제 Json데이터
    else if(m_objectCount == 12){
        int maxObjectCount  = parseObject->m_setJson["MaxObjectCount"].asInt();
        int useObjectCount  = parseObject->m_setJson["UseObjectCount"].asInt();
        int maxColorCount   = parseObject->m_setJson["MaxColorCount"].asInt();
        int useColorCount   = parseObject->m_setJson["UseColorCount"].asInt();
        
        // Json파일 데이터
        std::vector<Json::Value> vecImages;
        vecImages.push_back(parseObject->m_setJson["LeftImages"]);
        vecImages.push_back(parseObject->m_setJson["CenterImages"]);
        vecImages.push_back(parseObject->m_setJson["RightImages"]);
        
        std::vector<Json::Value> vecColors;
        vecColors.push_back(parseObject->m_setJson["LeftColors"]);
        vecColors.push_back(parseObject->m_setJson["CenterColors"]);
        vecColors.push_back(parseObject->m_setJson["RightColors"]);
        
        // 오브젝트 순서 값 생성
        // 초기에 한번만 생성, Left, Center, RightUseColor 개수는 항상 동일해야 한다.
        int random = 0;
        bool isDuplication = true;
        std::vector<int> vecObjectDuplication;
        for(int i=0; i<useObjectCount; i++){
            random = RandomHelper::random_int(0, maxObjectCount-1);
            isDuplication = true;
            for(int j=0; j<vecObjectDuplication.size(); j++){
                if(random == vecObjectDuplication.at(j)){
                    i--;
                    isDuplication = false;
                    break;
                }
            }
            if(isDuplication){
                vecObjectDuplication.push_back(random);
            }
        }
        
        // 색상 순서 값 생성 (조건부)
        isDuplication = true;
        std::vector<int> vecColorDuplication;
        if(maxColorCount != 0){
            for(int i=0; i<useColorCount; i++){
                random = RandomHelper::random_int(0, maxColorCount-1);
                isDuplication = true;
                for(int j=0; j<vecColorDuplication.size(); j++){
                    if(random == vecColorDuplication.at(j)){
                        i--;
                        isDuplication = false;
                        break;
                    }
                }
                if(isDuplication){
                    vecColorDuplication.push_back(random);
                }
            }
        }
        
        // 3단 Json 데이터 설정
        int objectDuplicationNum = 0;
        int colorDuplicationNum  = 0;
        std::string norObjectPath = "";
        std::string succObjectPath = "";
        for(int i=0; i<vecImages.size(); i++){
            Json::Value objectKeyValue = vecImages.at(i);
            Json::Value colorkeyValue  = vecColors.at(i);
            
            for(int j=0; j<useObjectCount; j++){
                // object는 필수
                objectDuplicationNum = vecObjectDuplication.at(j);
                Json::Value objectArray = objectKeyValue[objectDuplicationNum];
                
                // 색상 값이 필요 없는 완성형 ("nor", "succ") 오브젝트 경로
                if(useColorCount == 0){
                    m_vecUseObject.push_back(objectArray["nor"].asString());
                    m_vecSuccessObject.push_back(objectArray["succ"].asString());
                }
                // 색상 값이 필요한 조합형 경로
                else{
                    colorDuplicationNum = vecColorDuplication.at(j);
                    Json::Value colorArray = colorkeyValue[colorDuplicationNum];
                    if(colorArray.size() == 0){
                        // colorArray JsonValue값에 1차원 배열로 되어있는 경우
                        norObjectPath = objectArray["nor"].asString() + colorArray.asString();
                        succObjectPath = objectArray["succ"].asString() + colorArray.asString();
                    }
                    else{
                        // colorArray JsonValue값에 2차원 배열로 되어있는 경우("nor", "succ")
                        norObjectPath = objectArray["nor"].asString() + colorArray["nor"].asString();
                        succObjectPath = objectArray["succ"].asString() + colorArray["succ"].asString();
                    }
                    m_vecUseObject.push_back(norObjectPath);
                    m_vecSuccessObject.push_back(succObjectPath);
                }
            }
            this->set_3Dan(parseObject, useObjectCount, i);
            m_vecUseObject.clear();
            m_vecSuccessObject.clear();
        }
        vecImages.clear();
        vecColors.clear();
        vecObjectDuplication.clear();
        vecColorDuplication.clear();
    }
    
    // * 정답 객체 설정
    int addCount = 0;
    char answerType = 'A';
    for(int i=0; i<m_objectCount; i++){
        GameObject *gameObject = m_vecGameObject.at(i);
        gameObject->m_objectIndex = i;
        if(gameObject->m_answer.empty()){
            string name = [&] {
                // NB(xenosoz, 2018): This game is using this 'node' for answer identity.
                //   Give it a proper name for gameplay logging.
                stringstream ss;
                ss << answerType << "-" << i;
                answerType += 1;
                return ss.str();
            }();
            string answer = string{answerType};

            while(i + (4 * addCount) < m_objectCount){
                m_vecGameObject.at(i + (4 * addCount))->m_name = name;
                m_vecGameObject.at(i + (4 * addCount))->m_answer = answer;
                addCount++;
            }
            addCount = 0;
        }
    }
    
    // 문제 UI 생성
    this->createBox();
    this->createDot();
    this->createDrawNode();
    this->drawObject();
    
    _isMissed = false;
}

void LineMatchingScene::set_2Dan(JsonParseData *parseObject, int _useObjectCount, int _useColorCount){
    int matrixRow = m_objectCount/4;
    int matrixColumn = m_objectCount/2;
    bool isDrawBasicObject = true; // 초기화(x)
    
    // basic
    int basicIndex  = 0;
    int objectIndex = 0;
    int colorIndex  = 0;
    bool isOneObject = parseObject->m_setJson["UseOneObject"].asBool();
    bool isOneColor  = parseObject->m_setJson["UseOneColor"].asBool();
    bool isSuccSame  = parseObject->m_setJson["Nor_Succ_Same"].asBool();
    bool isSuccFullPath = parseObject->m_setJson["IsSuccFullPath"].asBool();
    
    // multi
    int multiIndex  = 0;
    int duplicationArray[8];
    for(int i=0; i<8; i++)
        duplicationArray[i] = -1;
    
    // essential
    int essentialIndex = 0;
    int essentialCommentIndex = 0;
    
    // 2행 4열
    for(int i=0; i<matrixRow; i++){
        for(int j=0; j<matrixColumn; j++){
            // 게임 오브젝트 클래스 저장
            GameObject *object = new GameObject();
            m_vecGameObject.push_back(object);
            
            // 도트 방향 설정
            if(i == 0)  object->m_vecDotAnchorPosition.push_back(Vec2::ANCHOR_MIDDLE_TOP);
            else        object->m_vecDotAnchorPosition.push_back(Vec2::ANCHOR_MIDDLE_BOTTOM);
            
            // 기본 오브젝트
            if(basicIndex < m_vecUseObject.size() && isDrawBasicObject){
                if(isOneObject) objectIndex = i;
                if(isOneColor)  colorIndex = i;
                
                // "nor"
                if(m_vecUseColor.size() > 0) object->m_fileName = m_vecUseObject.at(objectIndex) + m_vecUseColor.at(colorIndex);
                else                         object->m_fileName = m_vecUseObject.at(objectIndex);
                
                // "succ" (fullPath, !fullPath && isSuccSame, !fullPath && !isSuccSame)
                if(isSuccFullPath){
                    // 정답 이미지의 전체 경로
                    object->m_successFileName = m_vecSuccessObject.at(objectIndex);
                }
                else if(!isSuccFullPath && isSuccSame){
                    // 정답 이미지 조합(오브젝트 이름 + 단순 색상)
                    object->m_successFileName = m_vecSuccessObject.at(objectIndex) + m_vecUseColor.at(colorIndex);
                }
                else if(!isSuccFullPath && !isSuccSame){
                    // 조합형(오브젝트 이름 + 2차원 배열 색상 선택)
                    object->m_successFileName = m_vecSuccessObject.at(colorIndex);
                }
                basicIndex++;
                objectIndex++;
                colorIndex++;
                
                // 기본 Index 초기화
                if(objectIndex >= _useObjectCount)  objectIndex = 0;
                if(colorIndex >= _useColorCount)    colorIndex = 0;
                if(basicIndex >= m_vecUseObject.size() && m_vecMultiObject.size() == 0 && m_vecEssentialObject.size() == 0){
                    basicIndex = 0;
                }
            }
            
            // 다중 오브젝트
            else if(multiIndex < m_vecMultiObject.size()){
                // 2차원 배열 파싱
                // basic과는 다른형식으로 정답 이미지를 저장한다.
                Json::Value array = m_vecMultiObject.at(multiIndex);
                int random = RandomHelper::random_int(0, array["maxNum"].asInt());
                if((i * matrixColumn) + j < matrixColumn){
                    duplicationArray[i+j] = random;
                }
                else{
                    // 중복제거
                    do{
                        random = RandomHelper::random_int(0, array["maxNum"].asInt());
                    }while(random == duplicationArray[j]);
                }
                object->m_fileName = array[TodoUtil::itos(random)].asString();
                object->m_successFileName = array["succ_" + TodoUtil::itos(random)].asString();
                multiIndex++;
            }
            
            // 필수 오브젝트
            else if(essentialIndex < m_vecEssentialObject.size()){
                // 2차원 배열 파싱
                // basic과는 다른형식으로 정답 이미지를 저장한다.
                Json::Value array = m_vecEssentialObject.at(essentialIndex);
                object->m_fileName = array[TodoUtil::itos(essentialCommentIndex)].asString();
                object->m_successFileName = array["succ"].asString();
                essentialCommentIndex++;
                if(array[TodoUtil::itos(essentialCommentIndex)].asString() == ""){
                    essentialIndex++;
                    essentialCommentIndex = 0;
                }
            }
        }
        // 4개의 기본 오브젝트와 4개의 다중 오브젝트로 이뤄진 문제(Ex. 5번문제)
        // Ex) i가 0이면 기본 오브젝트만 4개 생성, i가 1이면 다중 오브젝트만 4개 생성
        if(_useObjectCount + m_vecMultiObject.size() + m_vecEssentialObject.size() == m_objectCount){
            isDrawBasicObject = false;
        }
        
        // 필수, 멀티 Index 초기화
        basicIndex = 0;
        multiIndex = 0;
        essentialIndex = 0;
    }
}

void LineMatchingScene::set_3Dan(JsonParseData *parseObject, int _useObjectCount, int _position){
    std::vector<std::string> stringSplit;
    Json::Value objectJson;
    std::string jsonName = "";
    std::string objectName = "";
    std::string filePath = "";
    
    for(int i=0; i<_useObjectCount; i++){
        // 게임 오브젝트 클래스 저장
        GameObject *object = new GameObject();
        m_vecGameObject.push_back(object);
        
        // 도트 이미지 방향 설정
        // DotPosition Right
        switch (_position) {
                // Left
            case 0:{
                object->m_vecDotAnchorPosition.push_back(Vec2::ANCHOR_MIDDLE_RIGHT);
                break;
            }
                // Center
            case 1:{
                object->m_vecDotAnchorPosition.push_back(Vec2::ANCHOR_MIDDLE_LEFT);
                object->m_vecDotAnchorPosition.push_back(Vec2::ANCHOR_MIDDLE_RIGHT);
                break;
            }
                // Right
            case 2:{
                object->m_vecDotAnchorPosition.push_back(Vec2::ANCHOR_MIDDLE_LEFT);
                break;
            }
        }
        
        // 파일이름 생성
        object->m_fileName = m_vecUseObject.at(i);
        object->m_successFileName = m_vecSuccessObject.at(i);
    }
}

// ======================================================================================================================================================//

void LineMatchingScene::createBox(){
    //Sprite *spriteBox = Sprite::create("LineMatching/Box.png");
    Sprite *spriteBox = Sprite::create("LineMatching/line-matching_image_stone.png");
    
    Size boxSize = spriteBox->getContentSize();
    
    int addCount = 0;
    int widthLineBoxNum = 0;
    float boxDistance, boxStartX, boxStartY, x, y = 0;
    std::vector<int> vecDistanceNum;
    
    // 초기 설정
    if(m_objectCount == 8){
        boxStartY = 272;
        widthLineBoxNum = 4;
        boxDistance = (m_winSize.width - (boxSize.width * widthLineBoxNum)) / (widthLineBoxNum + 1);
        boxStartX = boxDistance + (boxSize.width/2);
    }
    else if(m_objectCount == 12){
        boxStartY = 213;
        widthLineBoxNum = 3;
        boxDistance = (m_winSize.width - (boxSize.width * widthLineBoxNum)) / (widthLineBoxNum - 1);
        boxStartX = boxSize.width/2;
    }
    
    // 거리 랜덤 값 설정
    bool isLineRandom = m_isRandomLine;
    for(int i=0; i<4; i++)
        vecDistanceNum.push_back(i);
    
    // 박스 이미지 생성
    for(int i=0; i<m_objectCount; i++){
        GameObject *object = m_vecGameObject.at(i);
        object->m_spriteBox = Sprite::createWithTexture(spriteBox->getTexture());
        object->m_spriteBox->setLocalZOrder(eZOrder::Z_BOX);
        
        if(m_objectCount == 8){
            if(i % 4 == 0){
                // 랜덤매칭
                if(isLineRandom){
                    vecDistanceNum = this->boxSetRandom(vecDistanceNum);
                }
                
                if(i == 0)
                    y = boxStartY;
                else
                    y += 916;
            }
            x = boxStartX + ((boxDistance + boxSize.width) * vecDistanceNum.at(i % 4));
        }
        else if(m_objectCount == 12){
            if(i % 4 == 0){
                // 랜덤매칭
                if(isLineRandom){
                    vecDistanceNum = this->boxSetRandom(vecDistanceNum);
                }
                x = boxStartX + (boxSize.width + boxDistance) * addCount;
                addCount++;
            }
            y = boxStartY + (337 * vecDistanceNum.at(i % 4));
            object->m_spriteBox->setScale(0.75f);
        }
        object->m_distanceIndex = vecDistanceNum.at(i%4);
        object->m_spriteBox->setPosition(x, y);
        m_vecRemoveObject.pushBack(object->m_spriteBox);
        this->addChild(object->m_spriteBox);
    }
}

std::vector<int> LineMatchingScene::boxSetRandom(std::vector<int> vecDistanceNum){
    int nDust, nSour, nTemp;
    for(int i=0; i<(int)vecDistanceNum.size() * 2;  i++){
        nDust = rand() % 4;
        nSour = rand() % 4;
        
        nTemp = vecDistanceNum.at(nDust);
        vecDistanceNum.at(nDust) = vecDistanceNum.at(nSour);
        vecDistanceNum.at(nSour) = nTemp;
    }
    return vecDistanceNum;
}

void LineMatchingScene::createDot(){
    int vecSize = 0;
    Sprite *spriteDot;
    for(int i=0; i<m_objectCount; i++){
        GameObject *object = m_vecGameObject.at(i);
        Vec2 boxPosition = object->m_spriteBox->getPosition();
        for(int j=0; j<object->m_vecDotAnchorPosition.size(); j++){
            vecSize = (int)object->m_vecDotAnchorPosition.size();
            
            // 1. m_vecDotAnchorPosition.Size가 1이고, ANCHOR_MIDDLE_RIGHT 경우 Yellow.png
            if(vecSize == 1){
                if(object->m_vecDotAnchorPosition.at(j) != Vec2::ANCHOR_MIDDLE_LEFT)
                    spriteDot = Sprite::create("LineMatching/Dot_Yellow.png");
                else
                    spriteDot = Sprite::create("LineMatching/Dot_Red.png");
            }
            // 2. m_vecDotAnchorPosition.Size가 1인경우 Yellow
            else if(vecSize == 2){
                if(j == 0)
                    spriteDot = Sprite::create("LineMatching/Dot_Yellow.png");
                else
                    spriteDot = Sprite::create("LineMatching/Dot_Red.png");
            }
            spriteDot->setLocalZOrder(eZOrder::Z_DOT);
            spriteDot->setTag(eDotTouch::Z_ENABLE);
            
            // 앵커포인트 체크
            Vec2 dotAnchorPoint = object->m_vecDotAnchorPosition.at(j);
            if(dotAnchorPoint == Vec2::ANCHOR_MIDDLE_TOP){
                spriteDot->setPosition(boxPosition.x, boxPosition.y + 270);
            }
            else if(dotAnchorPoint == Vec2::ANCHOR_MIDDLE_BOTTOM){
                spriteDot->setPosition(boxPosition.x, boxPosition.y - 270);
            }
            else if(dotAnchorPoint == Vec2::ANCHOR_MIDDLE_RIGHT){
                spriteDot->setPosition(boxPosition.x + 190, boxPosition.y);
            }
            else if(dotAnchorPoint == Vec2::ANCHOR_MIDDLE_LEFT){
                spriteDot->setPosition(boxPosition.x - 190, boxPosition.y);
            }
            object->m_vecDot.pushBack(spriteDot);
            m_vecRemoveObject.pushBack(spriteDot);
            this->addChild(spriteDot);
        }
    }
}

void LineMatchingScene::createDrawNode(){
    // 기본 하얀색 DrawNode
    m_whiteDrawNode = DrawNode::create();
    m_whiteDrawNode->setLocalZOrder(eZOrder::Z_DRAW);
    m_vecRemoveObject.pushBack(m_whiteDrawNode);
    this->addChild(m_whiteDrawNode);
    
    // In, Out DrawNode
    m_vecInDrawNode.clear();
    m_vecOutDrawNode.clear();
    for(int i=0; i<m_objectCount-4; i++){
        // out
        DrawNode *outDrawNode = DrawNode::create();
        outDrawNode->setLocalZOrder(eZOrder::Z_DRAW);
        m_vecOutDrawNode.pushBack(outDrawNode);
        this->addChild(outDrawNode);
        
        // in
        DrawNode *inDrawNode = DrawNode::create();
        inDrawNode->setLocalZOrder(eZOrder::Z_DRAW);
        m_vecInDrawNode.pushBack(inDrawNode);
        this->addChild(inDrawNode);
        
        m_vecRemoveObject.pushBack(outDrawNode);
        m_vecRemoveObject.pushBack(inDrawNode);
    }
}

void LineMatchingScene::drawObject(){
    // 오브젝트 이미지 생성
    for(int i=0; i<m_objectCount; i++){
        GameObject *object = m_vecGameObject.at(i);
        object->m_spriteObject = Sprite::create(object->m_fileName);
        object->m_spriteObject->setPosition(object->m_spriteBox->getPosition().x - 5.f, object->m_spriteBox->getPosition().y + 20.f);
        //object->m_spriteObject->setPosition(object->m_spriteBox->getPosition());
        m_vecRemoveObject.pushBack(object->m_spriteObject);
        if (m_objectCount == 12)
        {
            m_objectScale = 0.60f;
            object->m_spriteObject->setScale(m_objectScale);
        }
        else
        {
            m_objectScale = 0.80f;
            object->m_spriteObject->setScale(m_objectScale);
        }
        this->addChild(object->m_spriteObject);
    }
}

// ======================================================================================================================================================//

Rect LineMatchingScene::setDotRect(Sprite *spriteDot){
    // 도트 터치 영역
    Size dotSize = spriteDot->getContentSize();
    Vec2 position = spriteDot->getPosition();
    float x = position.x - dotSize.width * 2.0f;
    float y = position.y - dotSize.height * 2.0f;
    return Rect(x, y, dotSize.width * 4.0f, dotSize.height * 4.0f);
}

void LineMatchingScene::setTutorial(){
    int firstIndex  = 0;
    int secondIndex = 0;
    
    Vec2 startPosition      = Vec2::ZERO; // 2단, 3단 공통 시작점
    Vec2 startPosition_2    = Vec2::ZERO;
    Vec2 finishPosition     = Vec2::ZERO; // 2단, 3단 공통 도착점
    Vec2 finishPosition_2   = Vec2::ZERO;
    
    if(m_objectCount == 12)
        m_distanceNum = 3;
    
    // 위치 (2단, 3단 공통 시작점 도착점)
    for(int i=0; i<m_objectCount; i++){
        if(m_vecGameObject.at(i)->m_distanceIndex == m_distanceNum){
            firstIndex = i;
            break;
        }
    }
    for(int i=0; i<m_objectCount; i++){
        if(i != firstIndex && m_vecGameObject.at(firstIndex)->m_answer == m_vecGameObject.at(i)->m_answer){
            secondIndex = i;
            break;
        }
    }
    // 시작, 도착 위치
    if(m_objectCount == 8){
        startPosition = m_vecGameObject.at(secondIndex)->m_vecDot.front()->getPosition();
        finishPosition = m_vecGameObject.at(firstIndex)->m_vecDot.front()->getPosition();
    }
    else{
        startPosition = m_vecGameObject.at(firstIndex)->m_vecDot.front()->getPosition();
        finishPosition = m_vecGameObject.at(secondIndex)->m_vecDot.front()->getPosition();
    }
    
    // DrawNode(기본)
    DrawNode *tutorialDrawLine = DrawNode::create();
    tutorialDrawLine->setLocalZOrder(0);
    tutorialDrawLine->setLineWidth(kLineThickness);
    tutorialDrawLine->drawLine(startPosition, finishPosition, Color4F(0, 0, 1, 0.2f));
    m_vecTutorialDrawNode.pushBack(tutorialDrawLine);
    this->addChild(tutorialDrawLine);
    
    if(m_objectCount == 12){
        // startPosition_2, finishPosition_2 값 구하기
        for(int i=0; i<m_objectCount; i++){
            GameObject *firstObject = m_vecGameObject.at(firstIndex);
            GameObject *object = m_vecGameObject.at(i);
            
            if(i != firstIndex && firstObject->m_answer == object->m_answer){
                // Center, vecDot Back()
                if(object->m_vecDot.size() > 1)
                    startPosition_2 = object->m_vecDot.back()->getPosition();
                // Right, vecDot Front()
                else
                    finishPosition_2 = object->m_vecDot.front()->getPosition();
            }
        }
        
        // 추가 DrawNode 생성
        DrawNode *addDrawNode = DrawNode::create();
        addDrawNode->setLocalZOrder(0);
        addDrawNode->setLineWidth(18);
        addDrawNode->drawLine(startPosition_2, finishPosition_2, Color4F(0, 0, 1, 0.2f));
        m_vecTutorialDrawNode.pushBack(addDrawNode);
        this->addChild(addDrawNode);
    }
    
    // 손가락 아이콘
    m_spriteTutorialHand = Sprite::create("LineMatching/tutorial_image_guideHand_normal.png");
    m_spriteTutorialHand->setLocalZOrder(eZOrder::Z_TUTORIAL);
    m_spriteTutorialHand->setAnchorPoint(Vec2(0, 1));
    m_spriteTutorialHand->setPosition(startPosition);
    this->addChild(m_spriteTutorialHand);
    
    // Action
    MoveTo *actionMoveTo = MoveTo::create(1, finishPosition);
    MoveTo *actionMoveReturn = MoveTo::create(0, startPosition);
    Sequence *actionSeq;
    
    if(m_objectCount == 8){
        m_spriteTutorialHand->setAnchorPoint(Vec2(0.15f, 1));
        actionSeq = Sequence::create(DelayTime::create(0.2f), actionMoveTo, DelayTime::create(1), actionMoveReturn, NULL);
    }
    else{
        MoveTo *actionMoveTo_2 = MoveTo::create(0, startPosition_2);
        MoveTo *actionMoveTo_3 = MoveTo::create(1, finishPosition_2);
        actionSeq = Sequence::create(DelayTime::create(0.2f), actionMoveTo, DelayTime::create(0.3f), actionMoveTo_2,
                                     DelayTime::create(0.2f), actionMoveTo_3, DelayTime::create(1), actionMoveReturn, NULL);
    }
    Repeat *actionRepeat = Repeat::create(actionSeq, -1);
    m_spriteTutorialHand->runAction(Sequence::createWithTwoActions(DelayTime::create(1.5f), actionRepeat));
}

float LineMatchingScene::playParticleEffect(){
    // * 파티클
    ParticleSystemQuad* particleStar = ParticleSystemQuad::create("LineMatching/star.plist");
    particleStar->setLocalZOrder(eZOrder::Z_STAR);
    this->addChild(particleStar);
    
    // 이동경로 설정
    // 거리, 속력, 시간 공식
    // 속력 = 거리 / 시간
    // 시간 = 거리 / 속력
    float dx = m_vecLerpPosition.back().x - m_startPosition.x;
    float dy = m_vecLerpPosition.back().y - m_startPosition.y;
    float rad = atan2(dx, dy);
    float distance = sqrt(pow(m_vecLerpPosition.back().x - m_startPosition.x, 2) + pow(m_vecLerpPosition.back().y - m_startPosition.y, 2));
    particleStar->setStartColor(Color4F(1, 1, 1, 1));
    particleStar->setPosition(m_startPosition);
    
    // 각도
    float degree = (rad * 180) / M_PI;
    float standardDistance = 725;
    particleStar->setRotation(degree);
    particleStar->setVisible(true);
    
    // 액션
    float speed = standardDistance / 0.3f;
    float duration = distance / speed;
    MoveTo *actionMoveTo = MoveTo::create(duration, m_vecLerpPosition.back());
    CallFuncN *callFunc = CallFuncN::create(CC_CALLBACK_1(LineMatchingScene::setChangeParticleColor, this));
    particleStar->runAction(Sequence::createWithTwoActions(actionMoveTo, callFunc));
    
    return actionMoveTo->getDuration() + particleStar->getLife();
}

void LineMatchingScene::setChangeParticleColor(Node *pSender){
    // 파티클이 사라지는 효과
    ParticleSystemQuad *particle = (ParticleSystemQuad *)pSender;
    particle->setStartColor(Color4F(1, 1, 1, 0));
}

#pragma mark - solving effect
void LineMatchingScene::resultTheorem(){
    ScaleBy *actionScaleBy = ScaleBy::create(0.15f, 1.05f);
    Sequence *actionSeq = Sequence::create(Repeat::create(Sequence::createWithTwoActions(actionScaleBy, actionScaleBy->reverse()), 2), NULL);
    for(int i=0; i<m_objectCount; i++){
        // scale action
        GameObject *object = m_vecGameObject.at(i);
        object->m_spriteBox->runAction(actionSeq->clone());
        object->m_spriteObject->runAction(actionSeq->clone());
    }
    CallFunc *callFunc = CallFunc::create(CC_CALLBACK_0(LineMatchingScene::changeObjectTexture, this));
    CallFunc *callFunc_check = CallFunc::create(CC_CALLBACK_0(LineMatchingScene::checkLevelClearDelay, this));
    this->runAction(Sequence::create(DelayTime::create(actionScaleBy->getDuration() * 5), callFunc, callFunc_check, NULL));
}

void LineMatchingScene::changeObjectTexture(){
    log("call changeObjectTexture");
    bool isSoundPlay = false;
    // 박스, 오브젝트 텍스쳐 변경, 이펙트 애니메이션, 노드 변경
    for(int i=0; i<m_objectCount; i++){
        GameObject *object = m_vecGameObject.at(i);
        
//        object->m_spriteBox->setTexture("LineMatching/Box_Green.png");
//        object->m_spriteObject->setTexture(object->m_successFileName);
//        object->m_spriteObject->setOpacity(100);
        
        Node *nodeEffect = static_cast<Node *>(CSLoader::createNode("LineMatching/waterdrop.csb"));
        nodeEffect->setPosition(object->m_spriteBox->getPosition());
        this->addChild(nodeEffect, 999);
        
        // 애니메이션 값 설정
        ActionTimeline *actionEffect = CSLoader::createTimeline("LineMatching/waterdrop.csb");
        actionEffect->gotoFrameAndPlay(0, false);
        AnimationInfo info = actionEffect->getAnimationInfo("ani1");
        float aniPlayTime = (info.endIndex - info.startIndex) / 60.f;
        
        // 재생
        nodeEffect->runAction(actionEffect);
        nodeEffect->runAction(Sequence::createWithTwoActions(DelayTime::create(aniPlayTime), RemoveSelf::create()));
        if(!isSoundPlay){
            isSoundPlay = true;
            GameSoundManager::getInstance()->playEffectSound("LineMatching/Sounds/Boom.m4a");
        }
        
        // 노드 변경
//        for(int i=0; i<m_vecStartPosition.size(); i++){
//            m_vecInDrawNode.at(m_vecDrawIndex.at(i))->clear();
//            DrawNode *outDrawNode = m_vecOutDrawNode.at(m_vecDrawIndex.at(i));
//            outDrawNode->clear();
//            outDrawNode->setLocalZOrder(eZOrder::Z_SUCCESS_DRAW);
//            outDrawNode->drawSolidCircle(m_vecStartPosition.at(i), 25, CC_DEGREES_TO_RADIANS(90), 50, 1.0f, 1.0f, m_successColor);
//            outDrawNode->setLineWidth(kLineThickness);
//            outDrawNode->drawLine(m_vecStartPosition.at(i), m_vecFinishPosition.at(i), m_successColor);
//            outDrawNode->drawSolidCircle(m_vecFinishPosition.at(i), 25, CC_DEGREES_TO_RADIANS(90), 50, 1.0f, 1.0f, m_successColor);
//        }
    }
}

void LineMatchingScene::drawLine(float dt){
    if(m_lerpIndex == (int)m_vecLerpPosition.size()){
        this->unschedule(CC_SCHEDULE_SELECTOR(LineMatchingScene::drawLine));
        m_isTouchFlag = false;
        
        // DrawNode 사용변수 초기화
        m_drawIndex++;
        m_lerpIndex = 0;
        m_whiteDrawNode->clear();
        m_outDrawNode->setLocalZOrder(eZOrder::Z_HALF_SUCCESS_DRAW);
        m_inDrawNode->setLocalZOrder(eZOrder::Z_HALF_SUCCESS_DRAW);
        
        // 터치 활성화
        m_listener->setEnabled(true);
        return;
    }
    else{
        // Out DrawNode
        m_outDrawNode->clear();
        m_outDrawNode->drawSolidCircle(m_startPosition, 25, CC_DEGREES_TO_RADIANS(90), 50, 1.0f, 1.0f, m_selectColor);
        m_outDrawNode->setLineWidth(kLineThickness);
        m_outDrawNode->drawLine(m_startPosition, m_vecLerpPosition.at(m_lerpIndex), m_selectColor);
        m_outDrawNode->drawSolidCircle(m_vecLerpPosition.at(m_lerpIndex), 25, CC_DEGREES_TO_RADIANS(90), 50, 1.0f, 1.0f, m_selectColor);
        
        // In DrawNode
        m_inDrawNode->clear();
        m_inDrawNode->drawSolidCircle(m_startPosition, 15, CC_DEGREES_TO_RADIANS(90), 50, 1.0f, 1.0f, m_selectColor);
        m_inDrawNode->setLineWidth(12);
        m_inDrawNode->drawLine(m_startPosition, m_vecLerpPosition.at(m_lerpIndex), m_selectColor);
        m_inDrawNode->drawSolidCircle(m_vecLerpPosition.at(m_lerpIndex), 15, CC_DEGREES_TO_RADIANS(90), 50, 1.0f, 1.0f, m_selectColor);
        
        m_lerpIndex++;
    }
}

void LineMatchingScene::removeLine(float dt){
    if((int)m_vecLerpPosition.size() == 0){
        this->unschedule(CC_SCHEDULE_SELECTOR(LineMatchingScene::removeLine));
        m_isTouchFlag = false;
        m_whiteDrawNode->clear();
        m_listener->setEnabled(true);
        return;
    }
    else{
        // 기본 DrawNode를 하나씩 지우는 작업.
        m_whiteDrawNode->clear();
        m_whiteDrawNode->drawSolidCircle(m_startPosition, 25, CC_DEGREES_TO_RADIANS(90), 50, 1.0f, 1.0f, Color4F::WHITE);
        m_whiteDrawNode->setLineWidth(kLineThickness);
        m_whiteDrawNode->drawLine(m_startPosition, m_vecLerpPosition.back(), Color4F::WHITE);
        m_whiteDrawNode->drawSolidCircle(m_vecLerpPosition.back(), 25, CC_DEGREES_TO_RADIANS(90), 50, 1.0f, 1.0f, Color4F::WHITE);
        m_vecLerpPosition.pop_back();
    }
}

// ======================================================================================================================================================//

bool LineMatchingScene::onTouchBegan(cocos2d::Touch *touch, cocos2d::Event *event){
    // 터치 중 이라면?
    if(m_isTouchFlag){
        return false;
    }
    
    int selectIndex = 0;
    bool isTouch    = false;
    bool isDotState = false;
    Vec2 touchPoint = touch->getLocation();
    
    for(int i=0; i<m_objectCount; i++){
        m_firstObject = m_vecGameObject.at(i);
        // Box Touch
        if(m_firstObject->m_spriteBox->getBoundingBox().containsPoint(touchPoint)){
            m_firstObject->m_spriteDot = m_firstObject->m_vecDot.front();
            m_isDotTouch = false;
            isTouch = true;
            break;
        }
        // Dot Touch
        else{
            for(int j=0; j<(int)m_firstObject->m_vecDot.size(); j++){
                Rect rectDot = this->setDotRect(m_firstObject->m_vecDot.at(j));
                if(rectDot.containsPoint(touchPoint)){
                    m_isDotTouch = true;
                    m_firstObject->m_spriteDot = m_firstObject->m_vecDot.at(j);
                    // 정답이 연결되어있는 도트는 return 시킨다.
                    if(m_firstObject->m_spriteDot->getTag() == eDotTouch::Z_DISABLE){
                        m_isTouchFlag = false;
                        return false;
                    }
                    
                    isDotState = true;
                    selectIndex = j;
                    break;
                }
            }
            if(isDotState){
                isTouch = true;
                break;
            }
        }
    }
    if(isTouch){
        m_isTouchFlag = true;
        m_isDrawSound = false;
        m_hintObject = m_firstObject;
        
        // 도트
        m_startPosition = m_firstObject->m_spriteDot->getPosition();
        m_firstDotAnchorPoint = m_firstObject->m_vecDotAnchorPosition.at(selectIndex);
        return true;
    }
    return false;
}

void LineMatchingScene::onTouchMoved(cocos2d::Touch *touch, cocos2d::Event *event){
    // 도트 선택이 아니고, 선이 그려지고 있지 않는 상태
    if(!m_isDotTouch && !m_isTouchMoved){
        // touchPoint 체크
        bool isDrawLine = false;
        Sprite *spriteDot = m_firstObject->m_spriteDot;
        if(m_firstObject->m_vecDotAnchorPosition.size() == 1){
            Vec2 anchorPoint = m_firstObject->m_vecDotAnchorPosition.front();
            if(anchorPoint == Vec2::ANCHOR_MIDDLE_TOP)          isDrawLine = touch->getLocation().y > spriteDot->getPositionY() ? true : false;
            else if(anchorPoint == Vec2::ANCHOR_MIDDLE_BOTTOM)  isDrawLine = touch->getLocation().y < spriteDot->getPositionY() ? true : false;
            else if(anchorPoint == Vec2::ANCHOR_MIDDLE_LEFT)    isDrawLine = touch->getLocation().x < spriteDot->getPositionX() ? true : false;
            else if(anchorPoint == Vec2::ANCHOR_MIDDLE_RIGHT)   isDrawLine = touch->getLocation().x > spriteDot->getPositionX() ? true : false;
            
            // 터치 좌표가 지나가는 도트의 상태가 DISABLE 상태라면 return
            if(spriteDot->getTag() == eDotTouch::Z_DISABLE){
                isDrawLine = false;
            }
        }
        else{
            // 3단
            Vec2 delta = touch->getDelta();
            Sprite *spriteDot_Left = m_firstObject->m_vecDot.front();
            Sprite *spriteDot_Right = m_firstObject->m_vecDot.back();
            
            // 터치가 가능한 상태에만 실행된다.
            if(spriteDot_Left->getTag() == eDotTouch::Z_ENABLE && delta.x < 0 && touch->getLocation().x < spriteDot_Left->getPositionX()){
                isDrawLine = true;
                m_startPosition = m_firstObject->m_vecDot.front()->getPosition();
                m_firstObject->m_spriteDot = m_firstObject->m_vecDot.front();
                m_firstDotAnchorPoint = m_firstObject->m_vecDotAnchorPosition.front();
            }
            else if(spriteDot_Right->getTag() == eDotTouch::Z_ENABLE && delta.x > 0 && touch->getLocation().x > spriteDot_Right->getPositionX()){
                isDrawLine = true;
                m_startPosition = m_firstObject->m_vecDot.back()->getPosition();
                m_firstObject->m_spriteDot = m_firstObject->m_vecDot.back();
                m_firstDotAnchorPoint = m_firstObject->m_vecDotAnchorPosition.back();
            }
        }
        if(!isDrawLine){
            return;
        }
    }
    m_isTouchMoved = true;

    // 튜토리얼 오브젝트 삭제
    if(m_isTutorial){
        m_isTutorial = false;
        this->removeChild(m_spriteTutorialHand, true);
        for(int i=0; i<m_vecTutorialDrawNode.size(); i++){
            this->removeChild(m_vecTutorialDrawNode.at(i), true);
        }
    }
    
    // 사운드 재생
    if(!m_isDrawSound){
        m_isDrawSound = true;
        GameSoundManager::getInstance()->playEffectSound("LineMatching/Sounds/LineStart.m4a");
    }
    
    // 지우고 새로그리기 반복
    m_whiteDrawNode->clear();
    m_whiteDrawNode->setLocalZOrder(eZOrder::Z_DRAW);
    m_whiteDrawNode->drawSolidCircle(m_startPosition, 25, CC_DEGREES_TO_RADIANS(90), 50, 1.0f, 1.0f, Color4F::WHITE);
    m_whiteDrawNode->setLineWidth(kLineThickness);
    m_whiteDrawNode->drawLine(m_startPosition, touch->getLocation(), Color4F::WHITE);
    m_whiteDrawNode->drawSolidCircle(touch->getLocation(), 25, CC_DEGREES_TO_RADIANS(90), 50, 1.0f, 1.0f, Color4F::WHITE);
}

void LineMatchingScene::onTouchEnded(cocos2d::Touch *touch, cocos2d::Event *event){
    // TouchEnded 함수를 처리하지 못하는 조건
    if(!m_isTouchMoved){
        m_isTouchFlag = false;
        return;
    }
    
    int answerIndex = 0;
    bool isTouch = false;
    bool isDotState = false;
    m_isTouchMoved = false;
    m_listener->setEnabled(false);
    m_resultType = eAnswerType::T_NONE;

    Sprite *spriteDot = NULL;
    Vec2 touchPoint = touch->getLocation();
    m_finishPosition = touchPoint;
    
    for(int i=0; i<m_objectCount; i++){
        m_secondObject = m_vecGameObject.at(i);
        // Box Touch
        if(m_secondObject->m_spriteBox->getBoundingBox().containsPoint(touchPoint)){
            for(int j=0; j<m_secondObject->m_vecDot.size(); j++){
                spriteDot = m_secondObject->m_vecDot.at(j);
                // 첫번째 선택 오브젝트의 앵커포인트와, 두번째 선택 앵커포인트가 다를 경우
                if(m_firstDotAnchorPoint != m_secondObject->m_vecDotAnchorPosition.at(j)){
                    m_secondDotAnchorPoint = m_secondObject->m_vecDotAnchorPosition.at(j);
                    // 해당 도트의 태그가 ENABLE 상태
                    if(spriteDot->getTag() == eDotTouch::Z_ENABLE){
                        isDotState = true;
                        break;
                    }
                    // 해당 도트의 태그가 DISABLE 상태(오답)
                    else{
                        m_resultType = eAnswerType::T_INCORRECT;
                        break;
                    }
                }
            }
            if(isDotState){
                isTouch = true;
                break;
            }
        }
        
        // Dot Touch
        else{
            for(int j=0; j<(int)m_secondObject->m_vecDot.size(); j++){
                spriteDot = m_secondObject->m_vecDot.at(j);
                Rect rectDot = this->setDotRect(spriteDot);
                if(rectDot.containsPoint(touchPoint)){
                    isDotState = true;
                    m_secondDotAnchorPoint = m_secondObject->m_vecDotAnchorPosition.at(j);
                    break;
                }
            }
            if(isDotState){
                isTouch = true;
                break;
            }
        }
    }
    
    if(isTouch){
        // isIndexCheck = 인덱스 차이가 4보다 작거나 같은지 체크
        // isNodeAnswer = nodeAnswer 값이 같은가?
        // isAnchorPoint = 앵커포인트의 위치가 달라야 한다.
        // isObjectSame = 같은 인덱스이면 안된다.
        bool isIndexCheck  = abs(m_firstObject->m_objectIndex - m_secondObject->m_objectIndex) <= 4;
        bool isNodeAnswer  = m_firstObject->m_answer == m_secondObject->m_answer;
        bool isAnchorPoint = m_firstDotAnchorPoint != m_secondDotAnchorPoint;
        bool isObjectSame  = isAnchorPoint && m_firstObject->m_objectIndex != m_secondObject->m_objectIndex;
        if(isIndexCheck && isNodeAnswer && isObjectSame){
            answerIndex = m_secondObject->m_objectIndex;
            m_resultType = eAnswerType::T_ANSWER;
            m_answer = m_firstObject->m_answer;
            
            // 도트의 앵커포인트 확인
            m_firstObject->m_spriteDot->setTag(eDotTouch::Z_DISABLE);
            m_secondObject->m_spriteDot = spriteDot;
            m_secondObject->m_spriteDot->setTag(eDotTouch::Z_DISABLE);
            
            // 도트 오브젝트의 마지막 위치
            m_finishPosition = spriteDot->getPosition();
        }
        else if(m_firstObject == m_secondObject || m_firstDotAnchorPoint == m_secondDotAnchorPoint){
            m_resultType = eAnswerType::T_NONE;
        }
        else{
            m_resultType = eAnswerType::T_INCORRECT;
        }
        
        if (m_resultType == eAnswerType::T_ANSWER || m_resultType == eAnswerType::T_INCORRECT) {
            string workPath = [&] {
                stringstream SS;
                SS << "/" << "LineMatching";
                SS << "/" << "level-" << m_getIndex;
                SS << "/" << "work-" << _currentProblemIndex;
                return SS.str();
            }();

            string firstName = (m_firstObject ? m_firstObject->m_name : "");
            string secondName = (m_secondObject ? m_secondObject->m_name : "");
            StrictLogManager::shared()->game_Peek_Answer("LineMatching", workPath, secondName, firstName);
        }
    }
    
    // 직선의 거리를 0 ~ 1, 0.05% 기준으로 위치를 저장
    m_vecLerpPosition.clear();
    int lerpCount = 20;
    int maxDistance = 0;
    int minDistance = 0;
    if(m_objectCount == 8){
        maxDistance = 1900;
    }
    else{
        maxDistance = 1285;
    }
    minDistance = maxDistance / lerpCount;
    lerpCount = (int)m_startPosition.getDistance(m_finishPosition) / minDistance;
    
    float lerpDelay = (float)1 / lerpCount;
    if(m_resultType == eAnswerType::T_NONE || m_resultType == eAnswerType::T_INCORRECT){
        for(int i=0; i<lerpCount; i++)
            m_vecLerpPosition.push_back(m_startPosition.lerp(touchPoint, i * lerpDelay));
    }
    else{
        for(int i=0; i<lerpCount; i++)
            m_vecLerpPosition.push_back(m_startPosition.lerp(spriteDot->getPosition(), i * lerpDelay));
        m_vecLerpPosition.push_back(m_finishPosition);
    }
    
    // * 결과 타입 처리
    GameSoundManager::getInstance()->stopAllEffects();
    switch (m_resultType) {
        case eAnswerType::T_NONE:{
            GameSoundManager::getInstance()->playEffectSound("LineMatching/Sounds/LineBack.m4a");
            this->schedule(CC_SCHEDULE_SELECTOR(LineMatchingScene::removeLine));
            break;
        }
        case eAnswerType::T_INCORRECT:{
            if(!_isMissed){
                //*//GameController::getInstance()->onMiss();
                _isMissed = true;
            }
            this->schedule(CC_SCHEDULE_SELECTOR(LineMatchingScene::removeLine));
            break;
        }
        case eAnswerType::T_ANSWER:{
            m_successCount++;
            
            GameSoundManager::getInstance()->playEffectSound("UI_StarAdded.m4a");
            
            // DrawNode
            {
                // 기본 색상 (하얀색)
                m_whiteDrawNode->clear();
                m_whiteDrawNode->drawSolidCircle(m_startPosition, 25, CC_DEGREES_TO_RADIANS(90), 50, 1.0f, 1.0f, Color4F::WHITE);
                m_whiteDrawNode->setLineWidth(kLineThickness);
                m_whiteDrawNode->drawLine(m_startPosition, m_finishPosition, Color4F::WHITE);
                m_whiteDrawNode->drawSolidCircle(m_finishPosition, 25, CC_DEGREES_TO_RADIANS(90), 50, 1.0f, 1.0f, Color4F::WHITE);
                
                // In, Out 색상
                m_inDrawNode = m_vecInDrawNode.at(m_drawIndex);
                m_outDrawNode = m_vecOutDrawNode.at(m_drawIndex);
                if(m_objectCount == 8 || (m_objectCount == 12 && (m_firstObject->m_objectIndex < 4 || m_secondObject->m_objectIndex < 4))){
                    // Yellow
                    m_selectColor = kYellowColor;
                }else{
                    // Red
                    m_selectColor = kBlueColor;
                }
                this->schedule(CC_SCHEDULE_SELECTOR(LineMatchingScene::drawLine), 0.03f, (int)m_vecLerpPosition.size(), 0);
            }
            
            // 파티클 애니메이션
            float delay = this->playParticleEffect();
            
            // DrawNode 경로 저장
            m_vecDrawIndex.push_back(m_drawIndex);
            m_vecStartPosition.push_back(m_startPosition);
            m_vecFinishPosition.push_back(m_finishPosition);
            
            // (return) - 도트의 태그값이 전부 eDotTouch::Z_DISABLE 상태가 아닌 경우
            for(int i=0; i<m_objectCount; i++){
                GameObject *object = m_vecGameObject.at(i);
                for(int j=0; j<object->m_vecDot.size(); j++){
                    spriteDot = object->m_vecDot.at(j);
                    if(spriteDot->getTag() == eDotTouch::Z_ENABLE){
                        return;
                    }
                }
            }
            
            // 정리
            CallFunc *callFunc_Theorem = CallFunc::create(CC_CALLBACK_0(LineMatchingScene::resultTheorem, this));
            this->runAction(Sequence::createWithTwoActions(DelayTime::create(delay), callFunc_Theorem));
            break;
        }
    }
}

void LineMatchingScene::showComplete(float)
{
    CompletePopup::create()->show(1.0, [](){
        CCAppController::sharedAppController()->handleGameComplete(1);
    });
}

// ======================================================================================================================================================//

void LineMatchingScene::handlePause() {}
void LineMatchingScene::handleResume() {
    
    if(m_isTouchMoved){
        m_isTouchFlag = false;
        m_isTouchMoved = false;
        removeLine(0);
    }
}
void LineMatchingScene::handleStart(){
    //*//GameController::getInstance()->onBeginWithTotalProblems(m_problemGroupCount);
}

void LineMatchingScene::handleQuit(){
    this->stopAllActions();
    this->unscheduleAllCallbacks();
}
void LineMatchingScene::handleAnswerEntered(std::string &answer){
    //GameController::getInstance()->onGameComplete();
    CompletePopup::create()->show(1.0, [](){
        CCAppController::sharedAppController()->handleGameComplete(1);
    });
}

void LineMatchingScene::showHint() {
    // 스케일효과로 힌트를 표현
    GameObject *object;
    for(int i=0; i<m_objectCount; i++){
        object = m_vecGameObject.at(i);
        if(m_hintObject->m_answer == object->m_answer){
            object->m_spriteBox->runAction(RepeatForever::create(m_actionSeq)->clone());
            object->m_spriteObject->runAction(RepeatForever::create(m_actionSeq)->clone());
        }
    }
}
void LineMatchingScene::hideHint() {
    GameObject *object;
    for(int i=0; i<m_objectCount; i++){
        object = m_vecGameObject.at(i);
        object->m_spriteBox->setScale(m_objectScale);
        object->m_spriteObject->setScale(m_objectScale);
        object->m_spriteBox->stopAllActions();
        object->m_spriteObject->stopAllActions();
    }
}
