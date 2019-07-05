
#include <Managers/VoiceMoldManager.h>
#include "WordMatrixScene.h"

#include "Common/Controls/CompletePopup.hpp"

#include "Managers/GameSoundManager.h"
#include "Managers/StrictLogManager.h"
#include "Managers/UserManager.hpp"

#include "Utils/TodoUtil.h"

#include "CCAppController.hpp"

int RandomInt(int min, int max);
float RandomFloat(float min, float max);

WordMatrixScene *_thisScene;
RESOURCES_DATA _resourcesInfo;
WordMatrixBlock *_soundBlock = NULL;
WordMatrixAnswerBlock *_selectedBlock = NULL;
WordMatrixSlotBlock *_pointedBlock = NULL;
map<string, float> _durationMap;

Scene* WordMatrixScene::createScene(string levelID)
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();

    // 'layer' is an autorelease object
    auto layer = WordMatrixScene::create();
    layer->setLevel(TodoUtil::stoi(levelID));

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

void WordMatrixScene::setLevel(int level)
{
    _currentLevel = level;
}

bool WordMatrixScene::init()
{
    //SpriteFrameCache::getInstance()->removeUnusedSpriteFrames();
    Director::getInstance()->purgeCachedData();

    if (!Layer::init())
    {
        return false;
    }
    _touchEnabled = false;

    _currentStageIdx = 0;

    _clearSound = new SoundEffect("games/wordmatrix/sound/problem_clear.m4a");
    _clearSound->preload();

    setScreenSize();
    _thisScene = this;

    return true;
}

void WordMatrixScene::onEnter()
{
    Layer::onEnter();

    if(_moveBlockPanArray == NULL)
    {
        _moveBlockPanArray = __Array::createWithCapacity(8);
        _moveBlockPanArray->retain();
    }

    loadStageData(&_currentSheetID);
    makeStage();
}

void WordMatrixScene::onEnterTransitionDidFinish()
{
    startGame();
}

void WordMatrixScene::onExit()
{
    if(_particleNode)        _particleNode->release();
    if(_moveBlockPanArray)   _moveBlockPanArray->release();

    _durationMap.clear();
    _selectedStageInfos.clear();

    Layer::onExit();
}

#pragma mark -
#pragma mark Game Setting
void WordMatrixScene::setScreenSize()
{
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    _screenW = visibleSize.width;
    _screenH = visibleSize.height;

    Size gameSize = _resourcesInfo._gameSize;
    _gameScale = MIN(_screenW/gameSize.width, _screenH/gameSize.height);

    _gamePan = Node::create();
    _gamePan->setContentSize(visibleSize);
    _gamePan->setAnchorPoint(Vec2(0,0));
    _gamePan->setPosition(Vec2(origin.x,origin.y));
    _gamePan->setScale(_gameScale);
    this->addChild(_gamePan, 1);

    _uiPan = Node::create();
    _uiPan->setContentSize(visibleSize);
    _uiPan->setAnchorPoint(Vec2(0,0));
    _uiPan->setPosition(Vec2(origin.x,origin.y));
    _uiPan->setScale(_gameScale);
    this->addChild(_uiPan, 2);

    //bg,
    Sprite *bg = Sprite::create(_resourcesInfo._bgPath.c_str());
    bg->setAnchorPoint(Vec2(0.5f, 0.5f));
    float bgScale = MAX(_screenW/bg->getContentSize().width, _screenH/bg->getContentSize().height);
    bg->setScale(bgScale);
    bg->setPosition(visibleSize/2);
    this->addChild(bg, 0);

    //log("gameScale : %.2f", _gameScale);
    //log("ScreenSize : %.2f, %.2f", _screenW, _screenH);

    //basic UI
    _backButton = TodoSchoolBackButton::create();
    _backButton->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
    _backButton->setPosition(Vec2(25, _screenH-25));
    _uiPan->addChild(_backButton);

    _progressBar = ProgressIndicator::create();
    _progressBar->setPosition(Vec2(_screenW*0.5f, _screenH - _progressBar->getContentSize().height));
    _uiPan->addChild(_progressBar);

    if(UserManager::getInstance()->isDebugMode())
    {
        auto skip = Button::create();
        skip->setTitleFontSize(100);
        skip->setTitleText("Skip");
        skip->setAnchorPoint(Vec2::ANCHOR_TOP_RIGHT);
        skip->setPosition(Vec2(_screenW-25, _screenH-25));
        _uiPan->addChild(skip);
        skip->addTouchEventListener([this](Ref*,Widget::TouchEventType e) {
            if (e == Widget::TouchEventType::ENDED) {
                endStage();
            }
        });
    }

    //Particle
    _particleNode = Node::create();
    auto createParticleEffect = [this](string name, string plist){
        ParticleSystemQuad * particleEffect = ParticleSystemQuad::create(plist);
        particleEffect->setName(name);
        particleEffect->setScale(1.8f);
        particleEffect->setAnchorPoint(Vec2(0.5f, 0.5f));
        particleEffect->setPosition(Vec2(0,0));
        //particleEffect->setSpeed(particleEffect->getSpeed()*3);
        //particleEffect->setSpeedVar(particleEffect->getSpeedVar()*3);
        particleEffect->stopSystem();

        _particleNode->addChild(particleEffect);
    };
    createParticleEffect("particle1", "common/effects/particle/shining_particle_blur.plist");
    createParticleEffect("particle2", "common/effects/particle/shining_particle_circle.plist");
    createParticleEffect("particle3", "common/effects/particle/shining_particle_star.plist");
    _particleNode->retain();
}

void WordMatrixScene::loadStageData(int *sheetNum)
{
    string S = FileUtils::getInstance()->getStringFromFile(_resourcesInfo._tsvPath.c_str());
    auto data = TodoUtil::readTSV(S);

    int i = 0;

    map<int, vector<WORD_MATRIX_DATA>> questionInfoMap;
    while (i < data.size())
    {
        auto row = data[i++];
        int level = TodoUtil::stoi(row[1]);

        if(level==_currentLevel)
        {
            int workSheetNo = TodoUtil::stoi(row[2]);
            WORD_MATRIX_DATA stageInfo;
            stageInfo._quesionNo = TodoUtil::stoi(row[3]);
            stageInfo._verticalWords = TodoUtil::split(row[4], ',');
            stageInfo._horizontalWords = TodoUtil::split(row[5], ',');

            if(questionInfoMap.count(workSheetNo) > 0)
            {
                vector<WORD_MATRIX_DATA> datas = questionInfoMap[workSheetNo];
                datas.push_back(stageInfo);
                questionInfoMap[workSheetNo] = datas;
            }
            else
            {
                vector<WORD_MATRIX_DATA> datas;
                datas.push_back(stageInfo);
                questionInfoMap[workSheetNo] = datas;
            }
        }
    }

    int maxSheetNum = (int)questionInfoMap.size();
    *sheetNum= RandomInt(1, maxSheetNum);

    _selectedStageInfos = questionInfoMap[*sheetNum];
    _progressBar->setMax((int)_selectedStageInfos.size());
    _progressBar->setCurrent(_currentStageIdx+1, false);

    //sound duration, priority row->high
    string rawString = FileUtils::getInstance()->getStringFromFile("lettervoice/durations.tsv");
    data = TodoUtil::readTSV(rawString);
    for (auto row : data)
    {
        if (TodoUtil::trim(row[0]).size() <= 0) continue;
        if (row.size()==1) continue;
        auto row1Vec = TodoUtil::split(row[1], ':');
        TodoUtil::replaceAll(row1Vec[2], ".", "");
        auto rowDuration = (float)TodoUtil::stoi(row1Vec[2])/100;

        auto row2Vec = TodoUtil::split(row[0], '.');
        _durationMap[row2Vec[0]] = rowDuration;
    }
    rawString = FileUtils::getInstance()->getStringFromFile("phonicvoice/durations.tsv");
    data = TodoUtil::readTSV(rawString);
    for (auto row : data)
    {
        if (TodoUtil::trim(row[0]).size() <= 0) continue;
        if (row.size()==1) continue;
        auto row1Vec = TodoUtil::split(row[1], ':');
        TodoUtil::replaceAll(row1Vec[2], ".", "");
        auto rowDuration = (float)TodoUtil::stoi(row1Vec[2])/100;

        auto row2Vec = TodoUtil::split(row[0], '_');
        _durationMap[row2Vec[0]] = rowDuration;
    }
    rawString = FileUtils::getInstance()->getStringFromFile("wordmatrix/sound/durations.tsv");
    data = TodoUtil::readTSV(rawString);
    for (auto row : data)
    {
        if (TodoUtil::trim(row[0]).size() <= 0) continue;
        if (row.size()==1) continue;
        auto row1Vec = TodoUtil::split(row[1], ':');
        TodoUtil::replaceAll(row1Vec[2], ".", "");
        auto rowDuration = (float)TodoUtil::stoi(row1Vec[2])/100;

        auto row2Vec = TodoUtil::split(row[0], '.');
        _durationMap[row2Vec[0]] = rowDuration;
    }
}

void WordMatrixScene::makeStage()
{
    WORD_MATRIX_DATA stageInfo = _selectedStageInfos[_currentStageIdx];

    int verCnt = (int)stageInfo._verticalWords.size();
    int horCnt = (int)stageInfo._horizontalWords.size();

    vector<string> verticalWordList;
    vector<string> horizontalWordList;

    //random
    bool verCheck[verCnt];
    bool holCheck[horCnt];
    for(int i=0; i<verCnt; i++) verCheck[i] = false;
    for(int i=0; i<horCnt; i++) holCheck[i] = false;

    for(int i=0; i<verCnt; i++)
    {
        int random = RandomInt(0, verCnt);
        while(true)
        {
            if(random >= verCnt) random = 0;

            if(verCheck[random] == false)
            {
                verCheck[random] = true;
                verticalWordList.push_back(stageInfo._verticalWords[random]);
                break;
            }
            random++;
        }
    }
    for(int i=0; i<horCnt; i++)
    {
        int random = RandomInt(0, horCnt);
        while(true)
        {
            if(random >= horCnt) random = 0;

            if(holCheck[random] == false)
            {
                holCheck[random] = true;
                horizontalWordList.push_back(stageInfo._horizontalWords[random]);
                break;
            }
            random++;
        }
    }

    float maxBoardHeight = _screenH-_backButton->getContentSize().height;
    Vec2 blockBoardPos = Vec2(_screenW/2, _screenH-(550-100*(verCnt-1)));
    if(blockBoardPos.y >= maxBoardHeight)
        blockBoardPos.y = maxBoardHeight;

    _blockBoard = WordMatrixBlockBoard::create(verCnt, horCnt);
    _blockBoard->setAnchorPoint(Vec2(0.5f, 1.0f));
    _blockBoard->setPosition(blockBoardPos);
    _gamePan->addChild(_blockBoard);

    //Ver Block
    for(int i=0; i<verCnt; i++)
    {
        TEXT_INFO verFontInfo;
        verFontInfo._word = verticalWordList[i];
        verFontInfo._color = Color3B(255,209,25);  //ffd119

        vector<TEXT_INFO> textInfos;
        textInfos.push_back(verFontInfo);

        auto verWordBox = WordMatrixBlock::create(_resourcesInfo._verBoxPath, textInfos);
        _blockBoard->addVerWord(verWordBox);
    }

    //Hor Block
    for(int i=0; i<horCnt; i++)
    {
        TEXT_INFO horFontInfo;
        horFontInfo._word = horizontalWordList[i];
        horFontInfo._color = Color3B(255,234,191);  //ffeabf

        vector<TEXT_INFO> textInfos;
        textInfos.push_back(horFontInfo);

        auto horWordBox = WordMatrixBlock::create(_resourcesInfo._horBoxPath, textInfos);
        _blockBoard->addHorWord(horWordBox);
    }

    //Make BlockBoard
    _blockBoard->makeBlocks();

    //Make MoveBlocks
    int moveBlockLineCnt = verCnt;
    int blockType[5] = {1,2,3,4,5};
    bool blockTypeChecker[5];
    for(int i=0; i<moveBlockLineCnt; i++)
    {
        blockTypeChecker[i] = false;
    }
    for(int i=0; i<moveBlockLineCnt; i++)
    {
        WordMatrixMoveBlockLine *line = WordMatrixMoveBlockLine::create(horCnt);
        _gamePan->addChild(line);
        _moveBlockPanArray->addObject(line);

        int random = RandomInt(0, 4);
        while(true)
        {
            if(random >= 5)
                random = 0;

            if(blockTypeChecker[random] == false)
            {
                blockTypeChecker[random] = true;
                break;
            }
            random++;
        }

        eMoveBlockType selectType = TYPE_1;
        switch(blockType[random])
        {
            case 1: selectType = TYPE_1;  break;
            case 2: selectType = TYPE_2;  break;
            case 3: selectType = TYPE_3;  break;
            case 4: selectType = TYPE_4;  break;
                //case 5: selectType = TYPE_5;  break;
            case 5:
            {
                switch(RandomInt(0,3))
                {
                    case 0: selectType = TYPE_1;  break;
                    case 1: selectType = TYPE_2;  break;
                    case 2: selectType = TYPE_3;  break;
                    case 3: selectType = TYPE_4;  break;
                }
            }
                break;
        }

        line->init(selectType);
    }

    int maxR = (int)_moveBlockPanArray->count();
    int blockCnt = 0;
    WordMatrixMoveBlockLine *blockLine = (WordMatrixMoveBlockLine *)_moveBlockPanArray->getObjectAtIndex(0);
    for(int i=0; i<verCnt; i++)
    {
        for(int j=0; j<horCnt; j++)
        {
            int random = RandomInt(0, maxR-1);
            while(true)
            {
                if(random >= maxR)  random = 0;

                blockLine = (WordMatrixMoveBlockLine *)_moveBlockPanArray->getObjectAtIndex(random);
                if(blockLine->canAddBlock()) break;

                random++;
            }

            TEXT_INFO verFontInfo;
            verFontInfo._word = verticalWordList[i];
            verFontInfo._color = Color3B(255,209,25);  //ffd119

            TEXT_INFO horFontInfo;
            horFontInfo._word = horizontalWordList[j];
            horFontInfo._color = Color3B(255,234,191);  //ffeabf

            vector<TEXT_INFO> moveTextInfos;
            moveTextInfos.push_back(verFontInfo);
            moveTextInfos.push_back(horFontInfo);

            auto moveBlock = WordMatrixAnswerBlock::create(blockLine->getBlockType(), moveTextInfos);
            blockLine->addWordBlock(moveBlock);
            blockCnt++;
        }
    }
    for(int i=0; i<moveBlockLineCnt; i++)
    {
        WordMatrixMoveBlockLine *line = (WordMatrixMoveBlockLine *)_moveBlockPanArray->getObjectAtIndex(i);
        line->setAutoPosition();
    }

    _blockBoard->addChild(_particleNode);
}

#pragma mark -
#pragma mark Game Logic
void WordMatrixScene::startGame()
{
    this->showMoveBlockPan();
    _selectedBlock = NULL;
    schedule(schedule_selector(WordMatrixScene::gameUpdate));
}

void WordMatrixScene::gameUpdate(float dt)
{
    if(_selectedBlock)
    {
        bool colWall = checkCollideWall(_selectedBlock);
        if(colWall) _selectedBlock->autoMove();
    }
}

bool WordMatrixScene::checkCollideWall(WordMatrixAnswerBlock *block)
{
    if(block->_isAutoMove) return false;

    Vec2 worldPos = block->getParent()->convertToWorldSpace(block->getPosition());
    Size rectSize = block->getContentSize();
    Rect rect = Rect(worldPos.x-rectSize.width/2, worldPos.y-rectSize.height/2,
                     rectSize.width, rectSize.height);
    float adjRate = 10.0f;

    //WALL
    if(rect.getMaxX() >= _screenW + adjRate)
    {
        return true;
    }
    else if(rect.getMinX() <= -adjRate)
    {
        return true;
    }
    if(rect.getMaxY() >= _screenH-_progressBar->getContentSize().height)
    {
        return true;
    }
    else if(rect.getMinY() <= -adjRate)
    {
        return true;
    }

    return false;
}

void WordMatrixScene::checkEnterAnswerBlock(Vec2 currentPos)
{
    if(_selectedBlock)
    {
        _pointedBlock = NULL;
        for(int i=0; i<_blockBoard->_answerBlockArray->count(); i++)
        {
            WordMatrixSlotBlock *answerBlock = (WordMatrixSlotBlock *)_blockBoard->_answerBlockArray->getObjectAtIndex(i);
            answerBlock->onOffConnectedBlockEffectImg(false);

            Vec2 locationInParent = answerBlock->getParent()->convertToNodeSpace(currentPos);
            if(answerBlock->getBoundingBox().containsPoint(locationInParent))
            {
                _pointedBlock = answerBlock;
            }
        }
        if(_pointedBlock)
        {
            _pointedBlock->onOffConnectedBlockEffectImg(true);
        }
    }
}

void WordMatrixScene::checkAnswerWord(Vec2 currentPos)
{
    if(_selectedBlock)
    {
        for(int i=0; i<_blockBoard->_answerBlockArray->count(); i++)
        {
            WordMatrixSlotBlock *answerBlock = (WordMatrixSlotBlock *)_blockBoard->_answerBlockArray->getObjectAtIndex(i);
            Vec2 locationInParent = answerBlock->getParent()->convertToNodeSpace(currentPos);

            if(answerBlock->getBoundingBox().containsPoint(locationInParent))
            {
                string workPath = [this] {
                    stringstream SS;
                    SS << "/" << "WordMatrix";
                    SS << "/" << "level-" << _currentLevel << "-" << _currentSheetID;
                    SS << "/" << "work-" << _currentStageIdx;
                    return SS.str();
                }();

                StrictLogManager::shared()->game_Peek_Answer("WordMatrix", workPath, _selectedBlock->_textInfo._word, answerBlock->_textInfo._word);

                if(answerBlock->_textInfo._word.compare(_selectedBlock->_textInfo._word) == 0)
                {
                    int zOrder = answerBlock->_zOrder;

                    Vec2 pos = answerBlock->getPosition();
                    if(_blockBoard->_verCnt-1 == zOrder)
                        pos += Vec2(0, 10.0f);
                    else
                        pos += Vec2(0, 20.0f);

                    vector<TEXT_INFO> textInfos;
                    textInfos.push_back(answerBlock->_textInfo);

                    auto moveBlock = WordMatrixAnswerBlock::create(TYPE_SLOT, textInfos);
                    moveBlock->setAnchorPoint(Vec2(0.5f, 0.5f));
                    moveBlock->setPosition(pos);
                    _blockBoard->addChild(moveBlock, 99);

                    showParticle(true, 98, pos);
                    auto aA = CallFunc::create([this, moveBlock, zOrder] () {
                        this->showParticle(false, zOrder);
                        moveBlock->setLocalZOrder(zOrder);
                    });
                    this->runAction(Sequence::create(DelayTime::create(0.1f), aA, NULL));

                    if(_currentMoveBlockPan->removeWordBlock(_selectedBlock))
                    {
                        _moveBlockPanArray->removeObject(_currentMoveBlockPan);
                        if(_moveBlockPanArray->count() > 0)
                        {
                            showMoveBlockPan();
                        }
                        else
                        {
                            _currentMoveBlockPan = NULL;
                        }
                    }
                    _selectedBlock->playWordSound();
                    _selectedBlock->removeFromParent();
                    _selectedBlock = NULL;

                    _blockBoard->_answerBlockArray->removeObject(answerBlock);
                    break;
                }
                else
                {
                    _selectedBlock->autoMove();
                    break;
                }
            }
        }
    }

    //stage clear
    if(_blockBoard->_answerBlockArray->count() == 0)
    {
        endStage();
    }
}

void WordMatrixScene::showParticle(bool onOff, int zOrder, Vec2 pos)
{
    if(zOrder < 0)  zOrder = 0;
    _particleNode->setLocalZOrder(zOrder);
    if(onOff)
    {
        //_particleNode->setVisible(true);
        _particleNode->setPosition(pos);
        _particleNode->getChildByName<ParticleSystemQuad*>("particle1")->resetSystem();
        _particleNode->getChildByName<ParticleSystemQuad*>("particle2")->resetSystem();
        _particleNode->getChildByName<ParticleSystemQuad*>("particle3")->resetSystem();
    }
    else
    {
        _particleNode->getChildByName<ParticleSystemQuad*>("particle1")->stopSystem();
        _particleNode->getChildByName<ParticleSystemQuad*>("particle2")->stopSystem();
        _particleNode->getChildByName<ParticleSystemQuad*>("particle3")->stopSystem();
        //_particleNode->setVisible(false);
    }
}

void WordMatrixScene::showMoveBlockPan()
{
    WordMatrixMoveBlockLine *temp;
    for(int i=0; i<_moveBlockPanArray->count(); i++)
    {
        temp = (WordMatrixMoveBlockLine *)_moveBlockPanArray->getObjectAtIndex(i);
        if(i==0)
            _currentMoveBlockPan = temp;

        int cnt = temp->getBlockCnt();
        if(cnt >= _currentMoveBlockPan->getBlockCnt())
        {
            _currentMoveBlockPan = temp;
        }
    }
    _currentMoveBlockPan->showLine();
}

void WordMatrixScene::endStage()
{
    unschedule(schedule_selector(WordMatrixScene::gameUpdate));

    _currentStageIdx++;
    if(_currentStageIdx == (int)_selectedStageInfos.size())
    {
        auto CP = CompletePopup::create();
        CP->show(0.5f, [this] {
            CCAppController::sharedAppController()->handleGameComplete(1);
        });
    }
    else
    {
        float delayTime = 0.5f;
        if(_durationMap.count("problem_clear"))
        {
            delayTime = _durationMap["problem_clear"];
            delayTime *= 0.5f;
        }

        _clearSound->play();
        auto aA = CallFunc::create([this] () {
            this->showParticle(false);
            this->setNextStage();
        });
        this->runAction(Sequence::create(DelayTime::create(delayTime), aA, NULL));
    }
}

void WordMatrixScene::setNextStage()
{
    _moveBlockPanArray->removeAllObjects();
    _blockBoard->_answerBlockArray->removeAllObjects();
    _gamePan->removeAllChildren();

    _progressBar->setCurrent(_currentStageIdx, true);
    _progressBar->setCurrent(_currentStageIdx+1, false);

    makeStage();
    startGame();
}

//////////////////////////////////////////////////////////////////////////

#pragma mark -
#pragma mark WordMatrixBlock
WordMatrixBlock::WordMatrixBlock()
{
    _effectImg = NULL;

    _wordLabel1 = NULL;
    _wordLabel2 = NULL;

    playSound = false;
    _currentSound = NULL;
    _soundIdx = 0;
    _wordSounds.clear();
    _soundDurations.clear();

    _swallowTouch = true;
}

WordMatrixBlock *WordMatrixBlock::create(string fileName, vector<TEXT_INFO> textInfos, bool swallowTouch)
{
    WordMatrixBlock *wordMatrixBlock = new WordMatrixBlock();
    if(wordMatrixBlock && wordMatrixBlock->initWithFile(fileName))
    {
        wordMatrixBlock->init(fileName, textInfos);
        wordMatrixBlock->_swallowTouch = swallowTouch;
        wordMatrixBlock->autorelease();

        return wordMatrixBlock;
    }

    CC_SAFE_DELETE(wordMatrixBlock);
    return NULL;
}

void WordMatrixBlock::onEnter()
{
    Sprite::onEnter();

    auto listener = EventListenerTouchOneByOne::create();
    listener->setSwallowTouches(this->_swallowTouch);
    listener->onTouchBegan = CC_CALLBACK_2(WordMatrixBlock::onTouchBegan, this);

    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
}

void WordMatrixBlock::onExit()
{
    _wordSounds.clear();
    _soundDurations.clear();
    Sprite::onExit();
}

void WordMatrixBlock::init(string fileName, vector<TEXT_INFO> textInfos)
{
    int cutPoint = (int)fileName.find('.');
    string selectedFile = fileName.substr(0, cutPoint);
    selectedFile += "_selected.png";

    if(FileUtils::getInstance()->isFileExist(selectedFile.c_str()))
    {
        _effectImg = Sprite::create(selectedFile);
        _effectImg->setAnchorPoint(Vec2(0.5f, 0.5f));
        _effectImg->setPosition(this->getContentSize()/2);
        this->addChild(_effectImg);

        _effectImg->setVisible(false);
    }
    else
    {
        _effectImg = NULL;
    }

    //TEXT

    setLabel(textInfos);

    //find _01, _02sound...
    bool findContinueSound = false;
  for(int i=1; i<=99; i++){
        if(setSound(__String::createWithFormat("%s_%02d", _textInfo._word.c_str(), i)->getCString()))
       {
            findContinueSound = true;        }
       else
        {
           break;
        }
    }
    _speakWord.clear();
    if(!findContinueSound){
        setSound(_textInfo._word.c_str());
    }
}


bool WordMatrixBlock::setSound(const char* fileName)
{
    SoundEffect *wordSound = NULL;
    std::string _storeFileName = "";
    _speakWord.clear();
    __String *tt = __String::createWithFormat("games/wordmatrix/sound/%s.m4a", fileName);
    if(FileUtils::getInstance()->isFileExist(tt->getCString()))
    {
        wordSound = new SoundEffect(tt->getCString());
    }
    else
    {
        tt =  __String::createWithFormat("phonicvoice/%s_sound.m4a", fileName);
        if (FileUtils::getInstance()->isFileExist(tt->getCString()))
        {
            wordSound = new SoundEffect(tt->getCString());
        }
        else
        {
            tt =  __String::createWithFormat("LetterVoice/%s.m4a", fileName);
            if (FileUtils::getInstance()->isFileExist(tt->getCString()))
                wordSound = new SoundEffect(tt->getCString());
            else {
                _storeFileName = fileName;
            }

        }
    }

    if(wordSound != NULL )
    {
        wordSound->preload();
        _wordSounds.push_back(wordSound);

        if(_durationMap.count(fileName))
            _soundDurations.push_back(_durationMap[fileName]);

        return true;
    }
    else {
        _speakWord.push_back(_storeFileName);
        return false;
    }
}

void WordMatrixBlock::setLabel(vector<TEXT_INFO> textInfos)
{

    float fontSize = WM_MAX_FONT_SIZE;
    int wordCnt = (int)textInfos.at(0)._word.size();
    if(textInfos.size() == 2)
    {
        wordCnt += (int)textInfos.at(1)._word.size();
    }
    fontSize -= (wordCnt-1)*1.5f;
    fontSize = MAX(fontSize, WM_MIN_FONT_SIZE);
    fontSize = MIN(fontSize, WM_MAX_FONT_SIZE);

    Vec2 fontPos = Vec2(this->getContentSize()/2);
    fontPos.y += 4.0f;

    _textInfo = textInfos.at(0);
    _wordLabel1 = Label::createWithTTF(_textInfo._word.c_str(), _resourcesInfo._fontName, fontSize);
    _wordLabel1->setHorizontalAlignment(TextHAlignment::CENTER);
    _wordLabel1->setVerticalAlignment(TextVAlignment::CENTER);
    _wordLabel1->setColor(_textInfo._color);
    _wordLabel1->setAnchorPoint(Vec2(0.5f, 0.5f));
    _wordLabel1->setPosition(fontPos);
    this->addChild(_wordLabel1, 99);

    if(textInfos.size() == 2)
    {
        _wordLabel1->setHorizontalAlignment(TextHAlignment::RIGHT);
        _wordLabel1->setAnchorPoint(Vec2(1.0f, 0.5f));

        TEXT_INFO tempInfo = textInfos.at(1);
        _wordLabel2 = Label::createWithTTF(tempInfo._word.c_str(), _resourcesInfo._fontName, fontSize);
        _wordLabel2->setHorizontalAlignment(TextHAlignment::LEFT);
        _wordLabel2->setVerticalAlignment(TextVAlignment::CENTER);
        _wordLabel2->setColor(tempInfo._color);
        _wordLabel2->setAnchorPoint(Vec2(0.0f, 0.5f));
        this->addChild(_wordLabel2, 99);

        //set pos
        float leftSize = _wordLabel1->getContentSize().width;
        float rightSize = _wordLabel2->getContentSize().width;
        float diff = (leftSize-rightSize)*0.5f;

        _wordLabel2->setPosition(Vec2(fontPos.x+diff, fontPos.y));
        _wordLabel1->setPosition(Vec2(fontPos.x+diff, fontPos.y));

        _textInfo._word += tempInfo._word;
    }
}

void WordMatrixBlock::setShowText(bool enable)
{
    _wordLabel1->setVisible(enable);
    if(_wordLabel2)     _wordLabel2->setVisible(enable);
}
void WordMatrixBlock::onOffEffectImg(bool onOff)
{
    this->setOpacity(!onOff ? 255:0);
    if(_effectImg)  _effectImg->setVisible(onOff);
    if(!onOff)
    {
        if(_currentSound != NULL)
        {
            _currentSound->stop();
            _currentSound = NULL;
        }
        if(_soundBlock == this) _soundBlock = NULL;
    }
}

#pragma mark -
#pragma mark Block Touch
bool WordMatrixBlock::onTouchBegan(Touch *pTouch, Event *pEvent)
{
    Vec2 locationInParent = this->getParent()->convertToNodeSpace(pTouch->getLocation());
    if( !this->getBoundingBox().containsPoint(locationInParent) || !isVisible())
        return false;

    if( !containsTouchLocation(pTouch) )
    {
        return false;
    }

    if(playSound)   return false;

    int soundCnt = (int)_wordSounds.size();
    if(soundCnt >= 1)
    {
        if(_soundBlock != NULL)
        {
            _soundBlock->onOffEffectImg(false);
        }
        _soundBlock = this;

        float soundDelay = _soundDurations.at(_soundIdx);
        _currentSound = _wordSounds.at(_soundIdx++);
        _currentSound->play();
        if(_soundIdx >= soundCnt)
            _soundIdx = 0;

        onOffEffectImg(true);
        playSound = true;
        auto aA = CallFunc::create([this] () {
            this->onOffEffectImg(false);
            playSound = false;
        });
        this->runAction(Sequence::create(DelayTime::create(soundDelay), aA, NULL));
    }

    return true;
}

Rect WordMatrixBlock::rect()
{
    Size s = getContentSize();
    return Rect(-s.width*getAnchorPoint().x, -s.height*getAnchorPoint().y, s.width, s.height);
}

bool WordMatrixBlock::containsTouchLocation(Touch* touch)
{
    return rect().containsPoint(convertTouchToNodeSpaceAR(touch));
}

void WordMatrixBlock::touchDelegateRetain()
{
    retain();
}

void WordMatrixBlock::touchDelegateRelease()
{
    release();
}

#pragma mark -
#pragma mark WordMatrixAnswerBlock
WordMatrixAnswerBlock::WordMatrixAnswerBlock()
{
    _midImg = NULL;
    _shadowImg = NULL;

    _isTouched = false;
    _isCanMove = true;
    _isAutoMove = false;
    _swallowTouch = true;
}

WordMatrixAnswerBlock *WordMatrixAnswerBlock::create(eMoveBlockType blockType, vector<TEXT_INFO> textInfos, bool swallowTouch)
{
    WordMatrixAnswerBlock *wordMatrixAnswerBlock = new WordMatrixAnswerBlock();
    if(wordMatrixAnswerBlock)
    {
        wordMatrixAnswerBlock->init(blockType, textInfos);
        wordMatrixAnswerBlock->_swallowTouch = swallowTouch;
        wordMatrixAnswerBlock->autorelease();

        return wordMatrixAnswerBlock;
    }

    CC_SAFE_DELETE(wordMatrixAnswerBlock);
    return NULL;
}

void WordMatrixAnswerBlock::onEnter()
{
    Sprite::onEnter();

    auto listener = EventListenerTouchOneByOne::create();
    listener->setSwallowTouches(this->_swallowTouch);
    listener->onTouchBegan = CC_CALLBACK_2(WordMatrixAnswerBlock::onTouchBegan, this);
    listener->onTouchMoved = CC_CALLBACK_2(WordMatrixAnswerBlock::onTouchMoved, this);
    listener->onTouchEnded = CC_CALLBACK_2(WordMatrixAnswerBlock::onTouchEnded, this);
    listener->onTouchCancelled = CC_CALLBACK_2(WordMatrixAnswerBlock::onTouchCancelled, this);

    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
}

void WordMatrixAnswerBlock::onExit()
{
    WordMatrixBlock::onExit();
}

void WordMatrixAnswerBlock::init(eMoveBlockType blockType, vector<TEXT_INFO> textInfos)
{
    _isCanMove = true;
    string blockFileName = "wordmatrix/block_top_";
    switch (blockType)
    {
        case TYPE_1:        { blockFileName += "01.png";  } break;
        case TYPE_2:        { blockFileName += "02.png";  } break;
        case TYPE_3:        { blockFileName += "03.png";  } break;
        case TYPE_4:        { blockFileName += "04.png";  } break;
        case TYPE_5:        { blockFileName += "05.png";  } break;
        case TYPE_SLOT:     { blockFileName += "slot.png"; _isCanMove = false; }   break;
    }
    this->initWithFile(blockFileName);
    this->setAnchorPoint(Vec2(0.5f, 0.5f));

    if(_isCanMove)
    {
        _midImg = Sprite::create("wordmatrix/block_middle.png");
    }
    else
    {
        _midImg = Sprite::create("wordmatrix/block_middle_slot.png");
    }
    Vec2 midPos = Vec2(this->getContentSize().width/2, this->getContentSize().height/2-10.0f);
    _midImg->setAnchorPoint(Vec2(0.5f, 0.5f));
    _midImg->setPosition(midPos);
    this->addChild(_midImg,-1);

    if(_isCanMove)
    {
        Vec2 shadowPos = Vec2(this->getContentSize().width/2, this->getContentSize().height/2-20.0f);
        _shadowImg = Sprite::create("wordmatrix/block_shadow.png");
        _shadowImg->setAnchorPoint(Vec2(0.5f, 0.5f));
        _shadowImg->setPosition(shadowPos);
        this->addChild(_shadowImg,-2);
    }

    //TEXT
    setLabel(textInfos);

    setSound(_textInfo._word.c_str());


    _wrongSound = new SoundEffect("common/sounds/effect/matrix_wrongmove.m4a");
    _wrongSound->preload();
}

void WordMatrixAnswerBlock::setPrevPos()
{
    _prevPos = this->getPosition();
}

void WordMatrixAnswerBlock::autoMove()
{
    if(_isAutoMove)   return;
    _isAutoMove = true;
    checkEndTouch();

    float diffLength = fabs((this->getPosition()-_prevPos).length());
    if(diffLength >= 200.0f)    _wrongSound->play();

    if(_selectedBlock == this)  _selectedBlock = NULL;
    auto aA = CallFunc::create([this] () {
        this->_isAutoMove = false;
        Vec2 pos = this->getPosition();
    });
    this->runAction(Sequence::create(EaseIn::create(MoveTo::create(0.15f, _prevPos), 3.0f), aA, NULL));
}

void WordMatrixAnswerBlock::playWordSound() {
    int soundCnt = (int) _wordSounds.size();
    int soundCnt2 = (int) _speakWord.size();
    if (soundCnt >= 1) {
        if (_currentSound != NULL) {
            _currentSound->stop();
            _currentSound = NULL;
        }
        _currentSound = _wordSounds.at(_soundIdx++);
        _currentSound->play();
    }
    if (soundCnt2 >= 1) {
        _speakWordSound = _speakWord.at(_soundIdx++);
        VoiceMoldManager::shared()->speak(
                _speakWordSound);  //Implementation of tts for word matrix module
        if (_soundIdx >= soundCnt)
            _soundIdx = 0;
    }
}

#pragma mark -
#pragma mark Move Block Touch
bool WordMatrixAnswerBlock::onTouchBegan(Touch *pTouch, Event *pEvent)
{
    Vec2 locationInParent = this->getParent()->convertToNodeSpace(pTouch->getLocation());
    if( !this->getBoundingBox().containsPoint(locationInParent) || !isVisible())
        return false;

    if( !containsTouchLocation(pTouch) )
    {
        _isTouched = false;
        return false;
    }
    if(_isAutoMove)   return false;

    if(_isCanMove)
    {
        this->runAction(EaseIn::create(ScaleTo::create(0.2f, 1.07f), 3.0f));
        this->setLocalZOrder(2);
        _selectedBlock = this;
    }

    playWordSound();

    _isTouched = true;
    return true;
}

void WordMatrixAnswerBlock::onTouchMoved(Touch *pTouch, Event *pEvent)
{
    if(!_isCanMove)   return;
    if(_isAutoMove)   return;

    if(_isTouched)
    {
        this->setPosition(this->getPosition() + pTouch->getDelta());
        //_thisScene->checkEnterAnswerBlock(pTouch->getLocation());

        Vec2 worldPos = this->getParent()->convertToWorldSpace(this->getPosition());
        _thisScene->checkEnterAnswerBlock(worldPos);
        return;
    }
}

void WordMatrixAnswerBlock::onTouchEnded(Touch *pTouch, Event *pEvent)
{
    if(_pointedBlock)
    {
        _pointedBlock->onOffConnectedBlockEffectImg(false);
        _pointedBlock = NULL;
    }

    if(!_isCanMove)   return;

    if(_isTouched)
    {
        checkEndTouch();
    }
}

void WordMatrixAnswerBlock::onTouchCancelled(Touch *pTouch, Event *pEvent)
{
    checkEndTouch();
}

void WordMatrixAnswerBlock::checkEndTouch()
{
    _isTouched = false;
    this->runAction(EaseIn::create(ScaleTo::create(0.2f, 1.0f), 3.0f));
    this->setLocalZOrder(1);

    if(_selectedBlock == this)
    {
        //_thisScene->checkAnswerWord(pTouch->getLocation());
        Vec2 worldPos = this->getParent()->convertToWorldSpace(this->getPosition());
        _thisScene->checkAnswerWord(worldPos);
        if(_selectedBlock)
        {
            this->autoMove();
        }
    }
}

#pragma mark -
#pragma mark WordMatrixAnswerBlockLine
WordMatrixMoveBlockLine::WordMatrixMoveBlockLine()
{
    if(_childBlockArray == NULL)
    {
        _childBlockArray = __Array::createWithCapacity(8);
        _childBlockArray->retain();
    }
    _maxBlockCnt = 5;
}

WordMatrixMoveBlockLine *WordMatrixMoveBlockLine::create(int maxBlockCnt)
{
    WordMatrixMoveBlockLine *wordMatrixAnswerBlockLine = new WordMatrixMoveBlockLine();
    if(wordMatrixAnswerBlockLine)
    {
        wordMatrixAnswerBlockLine->_maxBlockCnt = maxBlockCnt;
        wordMatrixAnswerBlockLine->autorelease();
        return wordMatrixAnswerBlockLine;
    }

    CC_SAFE_DELETE(wordMatrixAnswerBlockLine);
    return NULL;
}

void WordMatrixMoveBlockLine::onEnter()
{
    Sprite::onEnter();
}

void WordMatrixMoveBlockLine::onExit()
{
    if(_childBlockArray)   _childBlockArray->release();
    Sprite::onExit();
}

void WordMatrixMoveBlockLine::init(eMoveBlockType blockType)
{
    _blockType = blockType;

    __String *tt = __String::createWithFormat("common/sounds/effect/card_hit_%d.m4a", RandomInt(0, 5));
    _showSound = new SoundEffect(tt->getCString());
    _showSound->preload();
}

eMoveBlockType WordMatrixMoveBlockLine::getBlockType()
{
    return _blockType;
}

bool WordMatrixMoveBlockLine::canAddBlock()
{
    if(_childBlockArray->count() < _maxBlockCnt)
        return true;
    else
        return false;
}

void WordMatrixMoveBlockLine::addWordBlock(WordMatrixAnswerBlock *answerBlock)
{
    _childBlockArray->addObject(answerBlock);
}

void WordMatrixMoveBlockLine::setAutoPosition()
{
    float diff = 38.0f;
    int childCnt = (int)_childBlockArray->count();

    float width = diff*(childCnt-1);
    float height = 300.0f;

    bool randomChecker[childCnt];
    for(int i=0; i<childCnt; i++)
        randomChecker[i] = false;

    int cnt = 0;
    while(true)
    {
        if(cnt == childCnt) break;

        int random = RandomInt(0, childCnt-1);
        while(true)
        {
            if(random >= childCnt)  random = 0;
            if(randomChecker[random] == false)
            {
                randomChecker[random] = true;
                break;
            }
            random++;
        }

        WordMatrixAnswerBlock *child = (WordMatrixAnswerBlock *)_childBlockArray->getObjectAtIndex(random);
        child->setAnchorPoint(Vec2(0.5f, 0.5f));
        child->setPosition(Vec2(child->getContentSize().width*(cnt+0.5f)+diff*cnt, height/2));
        child->setPrevPos();
        this->addChild(child);

        width += child->getContentSize().width;

        cnt++;
    }

    this->setContentSize(Size(width, height));

    Node *parent = this->getParent();
    this->setAnchorPoint(Vec2(0.5f, 0.5f));
    this->setPosition(Vec2(parent->getContentSize().width/2, -height));
    this->setVisible(false);
}

void WordMatrixMoveBlockLine::showLine()
{
    this->setVisible(true);
    this->runAction(EaseIn::create(MoveTo::create(0.2f, Vec2(this->getPositionX(), this->getContentSize().height/2)), 3.0f));
    _showSound->play();
}

bool WordMatrixMoveBlockLine::removeWordBlock(WordMatrixAnswerBlock *answerBlock)
{
    _childBlockArray->removeObject(answerBlock);
    if(_childBlockArray->count() == 0)
        return true;
    else
        return false;
}

int WordMatrixMoveBlockLine::getBlockCnt()
{
    return (int)_childBlockArray->count();
}

#pragma mark -
#pragma mark WordMatrixSlotBlock
WordMatrixSlotBlock::WordMatrixSlotBlock()
{
    _selectedImg = NULL;
    _connectedVerBlock = NULL;
    _connectedHorBlock = NULL;
}

WordMatrixSlotBlock *WordMatrixSlotBlock::create(bool showImg, bool isBottom, TEXT_INFO textInfo)
{
    WordMatrixSlotBlock *wordMatrixSlotBlock = new WordMatrixSlotBlock();
    if(wordMatrixSlotBlock)
    {
        wordMatrixSlotBlock->init(showImg, isBottom, textInfo);
        wordMatrixSlotBlock->autorelease();
        return wordMatrixSlotBlock;
    }

    CC_SAFE_DELETE(wordMatrixSlotBlock);
    return NULL;
}

void WordMatrixSlotBlock::init(bool showImg, bool isBottom, TEXT_INFO textInfo)
{
    _textInfo = textInfo;

    string fileName = isBottom ? _resourcesInfo._slotBottomPath:_resourcesInfo._slotPath;
    auto slotImg = Sprite::create(fileName);
    this->setContentSize(slotImg->getContentSize());

    slotImg->setAnchorPoint(Vec2(0.5f, 0.5f));
    slotImg->setPosition(this->getContentSize()/2);

    this->addChild(slotImg);
    slotImg->setVisible(showImg);

    int cutPoint = (int)fileName.find('.');
    string selectedFile = fileName.substr(0, cutPoint);
    selectedFile += "_selected.png";
    if(FileUtils::getInstance()->isFileExist(selectedFile.c_str()))
    {
        _selectedImg = Sprite::create(selectedFile);
        _selectedImg->setAnchorPoint(Vec2(0.5f, 0.5f));
        _selectedImg->setPosition(this->getContentSize()/2);
        this->addChild(_selectedImg);
        _selectedImg->setVisible(false);
    }
}

void WordMatrixSlotBlock::setConnectedBlock(WordMatrixBlock *verBlock, WordMatrixBlock *horBlock)
{
    _connectedVerBlock = verBlock;
    _connectedHorBlock = horBlock;
}

void WordMatrixSlotBlock::onOffConnectedBlockEffectImg(bool onOff)
{
    if(_connectedVerBlock)  _connectedVerBlock->onOffEffectImg(onOff);
    if(_connectedHorBlock)  _connectedHorBlock->onOffEffectImg(onOff);
    if(_selectedImg)        _selectedImg->setVisible(onOff);
}

#pragma mark -
#pragma mark WordMatrixBlockBoard
WordMatrixBlockBoard::WordMatrixBlockBoard()
{
    if(_answerBlockArray == NULL)
    {
        _answerBlockArray = __Array::createWithCapacity(32);
        _answerBlockArray->retain();
    }
}

WordMatrixBlockBoard *WordMatrixBlockBoard::create(int verCnt, int horCnt)
{
    WordMatrixBlockBoard *wordMatrixBlockBoard = new WordMatrixBlockBoard();
    if(wordMatrixBlockBoard)
    {
        wordMatrixBlockBoard->init(verCnt, horCnt);
        wordMatrixBlockBoard->autorelease();
        return wordMatrixBlockBoard;
    }

    CC_SAFE_DELETE(wordMatrixBlockBoard);
    return NULL;
}

void WordMatrixBlockBoard::onEnter()
{
    Sprite::onEnter();
}

void WordMatrixBlockBoard::onExit()
{
    if(_answerBlockArray)   _answerBlockArray->release();

    _verBlocks.clear();
    _horBlocks.clear();

    Sprite::onExit();
}

void WordMatrixBlockBoard::init(int verCnt, int horCnt)
{
    float addWidth = 0.0f;
    if(horCnt >= 1)  addWidth += ((horCnt-1)*368.0f);

    float addHeight = 0.0f;
    if(verCnt >= 2)  addHeight += 184.0f;
    if(verCnt >= 3)  addHeight += (verCnt-2)*204.0f;

    auto loadImg = Sprite::create(_resourcesInfo._boardPath);
    Size basicSize = loadImg->getContentSize();
    basicSize.width += addWidth;
    basicSize.height += addHeight;

    Rect capInset = Rect(310,250,260,140);
    _boardPan = Scale9Sprite::create();
    _boardPan->Scale9Sprite::initWithFile(capInset, _resourcesInfo._boardPath);
    _boardPan->setAnchorPoint(Vec2(0.5f, 0.5f));
    _boardPan->setContentSize(basicSize);
    this->setContentSize(basicSize);

    _boardPan->setRenderingType(Scale9Sprite::RenderingType::SLICE);
    _boardPan->setPosition(Vec2(basicSize/2));
    this->addChild(_boardPan);
}

void WordMatrixBlockBoard::addVerWord(WordMatrixBlock *verWord)
{
    _verBlocks.push_back(verWord);
}

void WordMatrixBlockBoard::addHorWord(WordMatrixBlock *horWord)
{
    _horBlocks.push_back(horWord);
}

void WordMatrixBlockBoard::makeBlocks()
{
    int verCnt = (int)_verBlocks.size();

    int adjHeight = 0.0f;
    if(verCnt >= 2)  adjHeight += 184.0f;
    if(verCnt >= 3)  adjHeight += (verCnt-2)*204.0f;

    Vec2 verFirstPos = Vec2(55, 147);
    verFirstPos.y += adjHeight;
    float verDiff = 48.0f;
    for(int i=0; i<verCnt; i++)
    {
        WordMatrixBlock *verBlock = _verBlocks.at(i);
        verBlock->setAnchorPoint(Vec2(0.5f, 0.5f));

        Vec2 addPos = Vec2(verBlock->getContentSize()/2);
        addPos += Vec2(0, -(verBlock->getContentSize().height+verDiff)*i);
        verBlock->setPosition(verFirstPos+addPos);

        _boardPan->addChild(verBlock);
    }

    int horCnt = (int)_horBlocks.size();
    Vec2 horFirstPos = Vec2(278, 375);
    horFirstPos.y += adjHeight;
    float horDiff = 4.0f;
    for(int i=0; i<horCnt; i++)
    {
        WordMatrixBlock *horBlock = _horBlocks.at(i);
        horBlock->setAnchorPoint(Vec2(0.5f, 0.5f));

        Vec2 addPos = Vec2(horBlock->getContentSize()/2);
        addPos += Vec2((horBlock->getContentSize().width+horDiff)*i, 0);
        horBlock->setPosition(horFirstPos+addPos);

        _boardPan->addChild(horBlock);
    }

    //Answer Slots
    Vec2 slotFirstPos = Vec2(276, 142);
    slotFirstPos.y += adjHeight;
    int slotCnt = 1;
    float firstHeight = 0.0f;
    for(int i=0; i<verCnt; i++)
    {
        for(int j=0; j<horCnt; j++)
        {
            WordMatrixBlock *verBlock = _verBlocks.at(i);
            WordMatrixBlock *horBlock = _horBlocks.at(j);

            TEXT_INFO answerTextInfo;
            answerTextInfo._word = verBlock->_textInfo._word+horBlock->_textInfo._word;
            answerTextInfo._color = Color3B(247,212,141); //f7d48d

            int checker = i+j+2;
            bool showImg = false;
            if(checker%2 == 1)  showImg = true;

            bool isBottom = false;
            if(i == verCnt-1 && verCnt >= 2)
                isBottom = true;

            WordMatrixSlotBlock *slotBlock = WordMatrixSlotBlock::create(showImg, isBottom, answerTextInfo);
            slotBlock->setAnchorPoint(Vec2(0.5f, 0.5f));

            if(i==0 && j==0)
                firstHeight = slotBlock->getContentSize().height;
            Vec2 addPos = Vec2(slotBlock->getContentSize()/2);
            addPos += Vec2(slotBlock->getContentSize().width*j, -firstHeight*i);
            if(verCnt == 1 || i != verCnt-1)
                addPos.y -= 20.0f;

            slotBlock->setPosition(slotFirstPos+addPos);
            slotBlock->setConnectedBlock(_verBlocks.at(i), _horBlocks.at(j));
            slotBlock->_zOrder = i;

            _boardPan->addChild(slotBlock);
            _answerBlockArray->addObject(slotBlock);
            slotCnt++;
        }
    }

    _verCnt = verCnt;
}


#pragma mark -
#pragma mark Global UTIL
int RandomInt(int min, int max)
{
    srand( (unsigned)time(NULL)+(unsigned)getpid() + rand());
    return min + (rand() % (int)(max - min + 1));
}


float RandomFloat(float min, float max)
{
    return ((float(rand()) / float(RAND_MAX)) * (max - min)) + min;
}
