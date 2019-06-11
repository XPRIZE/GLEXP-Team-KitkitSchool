//
//  WordNoteScene.cpp
//  KitkitSchool-mobile
//
//  Created by JungJaehun on 21/02/2018.
//

#include "WordNoteScene.hpp"
#include <Common/Basic/CommentStream.h>
#include <Common/Basic/CustomFacet.h>

#include "CCAppController.hpp"
#include "Managers/GameSoundManager.h"
#include "Managers/LanguageManager.hpp"
#include "Managers/UserManager.hpp"
#include "Managers/VoiceMoldManager.h"
#include "Managers/StrictLogManager.h"

#include "Common/Controls/TodoSchoolBackButton.hpp"
#include "Common/Controls/CompletePopup.hpp"
#include "3rdParty/CCNativeAlert.h"

BEGIN_NS_WORDNOTE;

const int noteSizeX = 16;
const int noteSizeY = 3;
const string questionFontPath = "fonts/mukta-bold.ttf";

Scene* WordNoteScene::createScene(int levelID)
{
    auto scene = Scene::create();
    auto layer = WordNoteScene::create();
    layer->setLevel(levelID);
    scene->addChild(layer);
    return scene;
}

void WordNoteScene::setLevel(int level) {
    LOGFN();
    _currentLevel = level;
}

void WordNoteScene::removeObjects() {
    _problemNode->removeAllChildren();
}

bool WordNoteScene::init() {
    
    LOGFN();
    
    if (!Layer::init()) return false;

    _zIndex = 10;
    _totalProblemCount = 0;

    _soundDurationSheet.clear();
    
    if (!LanguageManager::getInstance()->isEnglish()) {
        _soundDurationSheet = ProblemBank::getInstance()->loadSoundDurationSheet();
    }

    srand((unsigned)time(NULL));
    
    _progressBar = ProgressIndicator::create();
    
    _winSize = getContentSize();
    
    auto bg = Sprite::create("WordNote/Images/WordNote_image_effect_BG.png");
    auto bgSize = bg->getContentSize();
    auto bgScale = MAX(_winSize.width / bgSize.width, _winSize.height / bgSize.height);
    bg->setScale(bgScale);
    bg->setPosition(_winSize/2);
    bg->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    addChild(bg);

    _bgCorrect = Sprite::create("WordNote/Images/WordNote_image_effect_BG.png");
    _bgCorrect->setScale(bgScale);
    _bgCorrect->setPosition(_winSize/2);
    _bgCorrect->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    
    _gameSize = Size(2560, 1800);
    //_gameSize = Size(3200, 1800);
    _gameScale = _winSize.height / 1800;
    
    _gameNode = Node::create();
    _gameNode->setContentSize(_gameSize);
    _gameNode->setScale(_gameScale);
    _gameNode->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    _gameNode->setPosition(Vec2(_winSize.width/2, _winSize.height/2));
    addChild(_gameNode);
    
    auto backButton = TodoSchoolBackButton::create();
    backButton->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
    backButton->setPosition(Vec2(25, _winSize.height-25));
    addChild(backButton);
    
    _progressBar->setPosition(Vec2(_winSize.width/2, _winSize.height - _progressBar->getContentSize().height-50));
    addChild(_progressBar);
    
    auto paperBG = Sprite::create("WordNote/Images/WordNote_image_paper_BG.png");
    paperBG->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
    paperBG->setPosition(_gameNode->getContentSize().width/2, _gameNode->getContentSize().height);
    _gameNode->addChild(paperBG);
    
    auto guideFilter = LayerColor::create(Color4B(0, 0, 0, 128), 2560, 690);
    //guideFilter->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
    guideFilter->setPosition(0, 0);
    guideFilter->setName("guideFilter");
    guideFilter->setVisible(false);
    _gameNode->addChild(guideFilter);
    
    auto plate = Sprite::create("WordNote/Images/WordNote_image_wooden-plate.png");
    plate->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
    plate->setPosition(paperBG->getPositionX(), 0);
    _gameNode->addChild(plate);
    
    _selectedEffect1 = Sprite::create("WordNote/Images/WordNote_image_slot-selected-effect_single.png");
    _selectedEffect1->setVisible(false);
    _gameNode->addChild(_selectedEffect1);
    
    _selectedEffect2 = Sprite::create("WordNote/Images/WordNote_image_slot-selected-effect_double.png");
    _selectedEffect2->setVisible(false);
    _gameNode->addChild(_selectedEffect2);

    _selectedEffect3 = Sprite::create("WordNote/Images/WordNote_image_slot-selected-effect_triple.png");
    _selectedEffect3->setVisible(false);
    _gameNode->addChild(_selectedEffect3);
    
    _selectedEffect4 = Sprite::create("WordNote/Images/WordNote_image_slot-selected-effect_quad.png");
    _selectedEffect4->setVisible(false);
    _gameNode->addChild(_selectedEffect4);
    
    _problemNode = Node::create();
    _problemNode->setContentSize(_gameSize);
    _problemNode->setScale(_gameScale);
    _problemNode->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    _problemNode->setPosition(Vec2(_gameNode->getContentSize()/2));
    _gameNode->addChild(_problemNode);
    
    _plate = plate;
    
    if (UserManager::getInstance()->isDebugMode()) {
        auto skip = Button::create();
        skip->setTitleFontSize(100);
        skip->setTitleText("Skip");
        skip->setAnchorPoint(Vec2::ANCHOR_TOP_RIGHT);
        skip->setPosition(Vec2(_winSize.width-25, _winSize.height-25));
        addChild(skip);
        skip->addTouchEventListener([this](Ref*,Widget::TouchEventType e) {
            if (e == Widget::TouchEventType::ENDED) {
                goNextProblem();
            }
        });
        
    }
    /*
    GameSoundManager::getInstance()->playEffectSound("Common/Sounds/Effect/SFX_Counting_Win.m4a", false, 1.0f, 0, 0);
    GameSoundManager::getInstance()->playEffectSound("Common/Sounds/Effect/SFX_ChildrenCheerNew.m4a", false, 1.0f, 0, 0);
    GameSoundManager::getInstance()->playEffectSound("Common/Sounds/Effect/SFX_Wood_Correct.m4a", false, 1.0f, 0, 0);
    GameSoundManager::getInstance()->playEffectSound("Common/Sounds/Effect/SFX_Wood_SlideOut.m4a", false, 1.0f, 0, 0);
     */
    return true;
}

void WordNoteScene::onEnter(){
    LOGFN();
    Layer::onEnter();
    _problems = ProblemBank::getInstance()->loadData(_currentLevel, &_currentSheetNo);
    _currentProblemIndex = 0;
    _totalProblemCount = (int)_problems.size();
    _progressBar->setMax(_totalProblemCount);

    setProblem();
}

void WordNoteScene::onEnterTransitionDidFinish() {
    Layer::onEnterTransitionDidFinish();
    /*string text = "Welcome to Word Note";
    float duration = VoiceMoldManager::shared()->guessSpeakDuration(text);
    VoiceMoldManager::shared()->speak(text);
    CCLog("Voice speak duration in second: %f", duration);*/
}

float WordNoteScene::speech(string input) {
    if (LanguageManager::getInstance()->isEnglish()) {
       if (!_ttsAlert && CC_TARGET_PLATFORM == CC_PLATFORM_MAC) {
            _ttsAlert = true;
            NativeAlert::show("TTS engine is not supported in mac version", "", "OK");
      } else {
            VoiceMoldManager::shared()->speak(input);
      }
      return 0.5f;
    } else
   VoiceMoldManager::shared()->speak(input);
   // return speechWithFiles(input);
//   }
    
}

float WordNoteScene::speechWithFiles(string input, float timing) {

    //CCLOG("speechWithFiles:%s", input.c_str());

    if (input == "") return 0;
    float duration = 0;
    bool match = false;
    
    for (auto sd : _soundDurationSheet) {
        if (input.find(sd.word) == 0) {
            
            this->runAction(Sequence::create(
                DelayTime::create(timing),
                CallFunc::create([this, sd, timing](){
                //CCLOG("timing:%f", timing);
                //CCLOG("sound:%s", sd.word.c_str());
           //     GameSoundManager::getInstance()->playEffectSound("WordNote/Sounds/"+sd.word+".m4a");
         VoiceMoldManager::shared()->speak(sd.word);
                  //  VoiceMoldManager::shared()->speak("क्या");

            }), nullptr));

            duration += sd.duration-0.17f;
            timing += sd.duration-0.17f;
            duration += speechWithFiles(input.substr(sd.word.size()), timing);
            match = true;
            break;
        }
    }
    if (!match) CCLOG("failed: %s", input.c_str());
    return duration;
}

NoteSlot* WordNoteScene::getNoteSlot(int x, int y) {
    for (auto noteSlot : _noteSlots) {
        if (noteSlot->getCoordinateX() != x || noteSlot->getCoordinateY() != y) continue;
        return noteSlot;
    }
    return nullptr;
}

NoteSlot* WordNoteScene::getNoteSlotMiddle(int x, int y) {
    for (auto noteSlot : _noteSlots) {
        if (!noteSlot->isMiddle()) continue;
        if (noteSlot->getPrevious() == x && noteSlot->getCoordinateY() == y) return noteSlot;
    }
    return nullptr;
}

vector<NoteSlot*> WordNoteScene::getNoteSlots(int Xstart, int Xend, int y) {
    vector<NoteSlot*> noteSlots;
    noteSlots.clear();
    
    for (int i=Xstart; i<=Xend; i++) {
        auto checkNoteSlot = getNoteSlot(i,y);
        if (checkNoteSlot != nullptr) noteSlots.push_back(checkNoteSlot);
        
        auto checkMiddleSlot = getNoteSlotMiddle(i, y);
        if (checkMiddleSlot != nullptr) noteSlots.push_back(checkMiddleSlot);
    }
    
    return noteSlots;
}

NoteSlot* WordNoteScene::isPlaceableNoteSlot(int x, int y) {
    auto noteSlot = getNoteSlot(x, y);
    if (noteSlot && noteSlot->getLetter() == "") return noteSlot;
    else return nullptr;
}

vector<NoteSlot*> WordNoteScene::getAffectedSlots(NoteSlot* noteSlot, Piece* piece, bool effect, bool forRelease) {
    vector<NoteSlot*> noteSlots;
    noteSlots.clear();
    int letterLength = (int)piece->getLetter().size();
    bool is2XPiece = letterLength%2 ? false: true;

    switch(letterLength) {
        case 1:
            if (is2XPiece) break;
            if (noteSlot->isMiddle()) break;
            if (forRelease) { noteSlots.push_back(noteSlot); break; }
            if (noteSlot->getLetter() == "") {
                noteSlots.push_back(noteSlot);
                if (effect) {
                    _selectedEffect1->setPosition(noteSlot->getPosition());
                    _selectedEffect1->setVisible(true);
                }
            }
            break;
        case 2: {
            if (!is2XPiece) break;
            if (!noteSlot->isMiddle()) break;
            if (forRelease) {
                noteSlots.push_back(getNoteSlot(noteSlot->getPrevious(), noteSlot->getCoordinateY()));
                noteSlots.push_back(getNoteSlot(noteSlot->getNext(), noteSlot->getCoordinateY()));
                break;
            }
            auto slot1 = isPlaceableNoteSlot(noteSlot->getPrevious(), noteSlot->getCoordinateY());
            auto slot2 = isPlaceableNoteSlot(noteSlot->getNext(), noteSlot->getCoordinateY());
            if (slot1 && slot2) {
                noteSlots.push_back(slot1);
                noteSlots.push_back(slot2);
                if (effect) {
                    _selectedEffect2->setPosition(noteSlot->getPosition());
                    _selectedEffect2->setVisible(true);
                }
            }
            break;
        }
        case 3: {
            if (is2XPiece) break;
            if (noteSlot->isMiddle()) break;
            if (forRelease) {
                noteSlots.push_back(getNoteSlot(noteSlot->getCoordinateX()-1, noteSlot->getCoordinateY()));
                noteSlots.push_back(getNoteSlot(noteSlot->getCoordinateX(), noteSlot->getCoordinateY()));
                noteSlots.push_back(getNoteSlot(noteSlot->getCoordinateX()+1, noteSlot->getCoordinateY()));
                break;
            }
            auto slot1 = isPlaceableNoteSlot(noteSlot->getCoordinateX()-1, noteSlot->getCoordinateY());
            auto slot2 = isPlaceableNoteSlot(noteSlot->getCoordinateX(), noteSlot->getCoordinateY());
            auto slot3 = isPlaceableNoteSlot(noteSlot->getCoordinateX()+1, noteSlot->getCoordinateY());
            if (slot1 && slot2 && slot3) {
                noteSlots.push_back(slot1);
                noteSlots.push_back(slot2);
                noteSlots.push_back(slot3);
                if (effect) {
                    _selectedEffect3->setPosition(noteSlot->getPosition());
                    _selectedEffect3->setVisible(true);
                }
            }
            break;
        }
            
        case 4: {
            if (!is2XPiece) break;
            if (!noteSlot->isMiddle()) break;
            if (forRelease) {
                noteSlots.push_back(getNoteSlot(noteSlot->getPrevious()-1, noteSlot->getCoordinateY()));
                noteSlots.push_back(getNoteSlot(noteSlot->getPrevious(), noteSlot->getCoordinateY()));
                noteSlots.push_back(getNoteSlot(noteSlot->getNext(), noteSlot->getCoordinateY()));
                noteSlots.push_back(getNoteSlot(noteSlot->getNext()+1, noteSlot->getCoordinateY()));
                break;
            }
            auto slot1 = isPlaceableNoteSlot(noteSlot->getPrevious()-1, noteSlot->getCoordinateY());
            auto slot2 = isPlaceableNoteSlot(noteSlot->getPrevious(), noteSlot->getCoordinateY());
            auto slot3 = isPlaceableNoteSlot(noteSlot->getNext(), noteSlot->getCoordinateY());
            auto slot4 = isPlaceableNoteSlot(noteSlot->getNext()+1, noteSlot->getCoordinateY());
            if (slot1 && slot2 && slot3 && slot4) {
                noteSlots.push_back(slot1);
                noteSlots.push_back(slot2);
                noteSlots.push_back(slot3);
                noteSlots.push_back(slot4);
                if (effect) {
                    _selectedEffect4->setPosition(noteSlot->getPosition());
                    _selectedEffect4->setVisible(true);
                }
            }
            break;
        }

    }
    
    return noteSlots;
}

NoteSlot* WordNoteScene::placeableNoteSlotNearTouch(Touch *touch, Piece *piece) {
    //auto pos = piece->getParent()->convertToNodeSpace(touch->getLocation());
    auto pos = piece->getPosition();

    for (auto noteSlot : _noteSlots) {
        if (noteSlot->getBoundingBox().containsPoint(pos)) {
            auto affectedSlots = getAffectedSlots(noteSlot, piece, true);
            if (affectedSlots.size()) return noteSlot;
        }
    }
    hiddenSelectedEffect();
    return nullptr;
}

void WordNoteScene::releasePiece(Piece* piece) {
    auto noteSlot = piece->_placedNoteSlot;
    if (!noteSlot) return;
    
    noteSlot->_placedPiece = nullptr;
    auto affectedNoteSlots = getAffectedSlots(noteSlot, piece, false, true);
    for (auto affectedNoteSlot : affectedNoteSlots) {
        affectedNoteSlot->setLetter("");
        affectedNoteSlot->_placedPiece = nullptr;
    }
    piece->_placedNoteSlot = nullptr;
}


void WordNoteScene::setProblem() {
    _problem = _problems[_currentProblemIndex];
    _progressBar->setCurrent(_currentProblemIndex+1, false);
    
    _bgCorrect = Sprite::create("WordNote/Images/WordNote_image_effect_BG.png");
    auto bgSize = _bgCorrect->getContentSize();
    auto bgScale = MAX(_winSize.width / bgSize.width, _winSize.height / bgSize.height);

    _bgCorrect->setScale(bgScale);
    _bgCorrect->setPosition(_winSize/2);
    _bgCorrect->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    _bgCorrect->setOpacity(0);
    _problemNode->addChild(_bgCorrect);
    
    drawQuestion(&_problem);
    drawNoteSlot();
    drawPlateSlot(&_problem);

    this->runAction(Sequence::create(
                                     DelayTime::create(_currentProblemIndex == 0 ? 0.6 : 0.2),
                                     CallFunc::create([this](){
        touchSoundButton();
    }), nullptr));

}

bool WordNoteScene::checkAnswer() {
    vector<int> coordinatesX;
    auto problem = _problems[_currentProblemIndex];
    int xStart = 0, xEnd = 0;
    int yLine = 0;
    bool onCorrectFlag = false;
    
    for (int y=0; y<noteSizeY; y++) {
        
        coordinatesX.clear();
        string checkLine = "";
        string lastLetter = "";
        for (int x=0; x<noteSizeX; x++) {
            auto noteSlot = getNoteSlot(x,y);
            if (!noteSlot) continue;
            auto checkLetter = noteSlot->getLetter();
            if (checkLetter == "") checkLetter = " ";
            if (lastLetter == " " && checkLetter == " ") continue;
            checkLine += checkLetter;
            coordinatesX.push_back(noteSlot->getCoordinateX());
            lastLetter = checkLetter;
        }
        auto niddle = checkLine.find(problem.wordSentence);
        if (niddle != string::npos) {
            xStart = coordinatesX[niddle];
            xEnd = coordinatesX[niddle+problem.wordSentence.size()-1];
            yLine = y;
            
            auto correctNoteSlots = getNoteSlots(xStart, xEnd, yLine);
            onCorrect(correctNoteSlots);
            onCorrectFlag = true;
            
            StrictLogManager::shared()->game_Peek_Answer("WordNote", makeWorkPath(), problem.wordSentence, problem.wordSentence);
            break;
        }
    }
    
    return onCorrectFlag ? true : false;
}

void WordNoteScene::onCorrect(vector<NoteSlot*> noteSlots) {
    LOGFN();
    
    //GameSoundManager::getInstance()->playEffectSound("Common/Sounds/Effect/Result_RightAnswer.m4a", false, 1.0f, 0, 0);
    
    for (auto plateSlot : _plateSlots) {
        plateSlot->setDisable();
    }

    _bgCorrect->setLocalZOrder(_zIndex++);
    _bgCorrect->runAction(FadeIn::create(0.7));
    _correctPieces.clear();
    
    string word = "";
    
    for (auto noteSlot : noteSlots) {
        if (noteSlot->_placedPiece) word += noteSlot->_placedPiece->getLetter();
    }
    
    _onCorrectSpeechDuration = 0;

    GameSoundManager::getInstance()->playEffectSound("Common/Sounds/Effect/Success.m4a");
    
    for (auto noteSlot : _noteSlots) {
        noteSlot->setFadeOutFilter();
    }

	auto seq1 = Sequence::create(
		DelayTime::create(0.3),
		CallFunc::create([this, noteSlots]() {

		float totalWidth = 0, startXPos = 0;

		for (auto noteSlot : noteSlots) {
			if (noteSlot->_placedPiece) {
				noteSlot->_placedPiece->setLocalZOrder(_zIndex++);
				_correctPieces.push_back(noteSlot->_placedPiece);
				noteSlot->_placedPiece->_correctActionFlag = true;
				totalWidth += noteSlot->_placedPiece->getLetter().size() * 160;

			}
		}

		for (auto noteSlot : _noteSlots) {
			auto piece = noteSlot->_placedPiece;
			if (piece && !piece->_correctActionFlag) {
				piece->runAction(EaseIn::create(MoveTo::create(0.5, piece->_registeredPlateSlot->getPosition()), 3.0));
				piece->_isOnPlateSlot = true;
			}
		}

		startXPos = _gameSize.width / 2 - totalWidth / 2;

		for (auto correctPiece : _correctPieces) {
			startXPos += correctPiece->getLetter().size() * 160 / 2;
			correctPiece->runAction(EaseIn::create(MoveTo::create(0.5, Vec2(startXPos, _gameSize.height/2+10)), 2.0));
			startXPos += correctPiece->getLetter().size() * 160 / 2;
		}
	}),
		DelayTime::create(0.7),
		CallFunc::create([this, word]() {

		for (auto plateSlot : _plateSlots) {
			plateSlot->cleanPiecesOnPlateSlot();
		}
		_onCorrectSpeechDuration = speech(word);
		for (auto correctPiece : _correctPieces) {
			correctPiece->runAction(FadeOut::create(1));
			correctPiece->fadeOutLetter();
			correctPiece->startParticle();
		}
	}), nullptr);
	 
	auto seq2 = Sequence::create(DelayTime::create(_onCorrectSpeechDuration),
		CallFunc::create([this]() {
		answerFadeIn();
	}),
		DelayTime::create(1.5),
		CallFunc::create([this]() {
		for (auto correctPiece : _correctPieces) {
			correctPiece->stopParticle();
		}
	}),
		DelayTime::create(0.5),
		CallFunc::create([this]() {
		goNextProblem();
	}),nullptr);
    
	this->runAction(Sequence::create(seq1, seq2, nullptr));
}

void WordNoteScene::goNextProblem() {
    LOGFN();
    _currentProblemIndex++;
    if (_currentProblemIndex >= _totalProblemCount) {
        _progressBar->setCurrent(_totalProblemCount+1, false);
        this->scheduleOnce([this](float){
            auto CP = CompletePopup::create();
            CP->show(0.3, [this] {
                CCAppController::sharedAppController()->handleGameComplete(1);
            });
        }, 0.3, "onGameComplete");
    } else {
        _problemNode->removeAllChildren();
        _plateSlots.clear();
        _noteSlots.clear();
        _correctPieces.clear();
        setProblem();
    }
}

void WordNoteScene::answerFadeIn() {
    _fadeInCount = 0;
    auto answer = TodoUtil::createLabel(string(_problems[_currentProblemIndex].wordSentence), 130, Size::ZERO, "fonts/AndikaNewBasic-B.ttf", Color4B(65,64,62,0), TextHAlignment::CENTER);
    answer->setPosition(_problemNode->getContentSize().width/2, _problemNode->getContentSize().height/2+20);
    _problemNode->addChild(answer);
    answer->setLocalZOrder(++_zIndex);
    
    this->runAction(Repeat::create(
        Sequence::create(
        CallFunc::create([this, answer](){
            _fadeInCount++;
            Color3B letterColor = (Color3B)answer->getTextColor();
            answer->setTextColor(Color4B(letterColor, (int)255*(_fadeInCount)/10));
        }),
        DelayTime::create(0.1),
        nullptr
    ), 10));
}

void WordNoteScene::drawQuestion(WordNoteLevelStruct* problem) {
    ProblemType problemType;

    if (problem->image != "") problemType = TEXT_WITH_IMAGE;
    else if (!problem->hintWordSentence) problemType = SOUND_ONLY;
    else problemType = TEXT_ONLY;
    
    switch(problemType) {
        case TEXT_ONLY: {
            break;
        }
        case SOUND_ONLY: {
            break;
        }
        case TEXT_WITH_IMAGE: {
            
            auto frame = Sprite::create("WordNote/Images/WordNote_image_photo_frame.png");
            frame->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
            frame->setPosition(_gameSize.width/2-750, _gameSize.height-240);
            _problemNode->addChild(frame);
            
            auto picture = Sprite::create("WordNote/Images/"+problem->image);
            picture->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
            picture->setPosition(frame->getContentSize().width/2, frame->getContentSize().height/2+2);
            frame->addChild(picture);
            
            frame->setRotation(-10);
            
            
            break;
        }
    }
    
    auto questionText = TodoUtil::createLabel(problem->wordSentence, 200, Size::ZERO, questionFontPath, Color4B(45,45,52,229), TextHAlignment::CENTER);
    questionText->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    questionText->setPosition(_problemNode->getContentSize().width/2, _problemNode->getContentSize().height - 288);
    _problemNode->addChild(questionText);
    
    
    _soundButton = Sprite::create("WordNote/Images/sound_normal.png");
    _soundButton->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    _soundButton->setPosition(_problemNode->getContentSize().width/2+835, _problemNode->getContentSize().height - 297);
    _problemNode->addChild(_soundButton);
    
    _soundButtonActive = Sprite::create("WordNote/Images/sound_active.png");
    _soundButtonActive->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    _soundButtonActive->setPosition(_problemNode->getContentSize().width/2+835, _problemNode->getContentSize().height - 297);
    _soundButtonActive->setVisible(false);
    _problemNode->addChild(_soundButtonActive);
    
    auto *listener = EventListenerTouchOneByOne::create();
    listener->setSwallowTouches(true);
    listener->onTouchBegan = [this](Touch* T, Event* E) {
        auto pos = _soundButton->getParent()->convertToNodeSpace(T->getLocation());
        
        if (_soundButton->getBoundingBox().containsPoint(pos)) {
            return touchSoundButton();
        }
        return false;
    };
    _soundButton->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);
}

bool WordNoteScene::touchSoundButton() {
    if (_soundButtonActive->isVisible()) return false;
    _soundButtonActive->setVisible(true);
    _soundButton->setVisible(false);
    auto duration = speech(_problems[_currentProblemIndex].wordSentence);
    
    this->runAction(Sequence::create(
                                     DelayTime::create(duration),
                                     CallFunc::create([this](){
        _soundButton->setVisible(true);
        _soundButtonActive->setVisible(false);
    }), nullptr));
    return true;
}

void WordNoteScene::drawNoteSlot() {
    
    float startPosX = _gameNode->getContentSize().width/2 - 160*7.5;
    float startPosY = _gameNode->getContentSize().height/2 + 239;

    bool guide = false;
    
    int enableXStart = -1;
    int enableXEnd = -1;
    int enableY = -1;
    int answerSize = (int)_problem.wordSentence.size();
    
    switch(_problem.level) {
        case 1: case 2: case 3:
            guide = true;
            enableY = 1;
            enableXStart = (noteSizeX-answerSize)/2-1;
            enableXEnd = enableXStart + answerSize;
            break;
        case 4: case 5: case 6:
            guide = true;
            enableY = 1;
            enableXStart = 0;
            enableXEnd = enableXStart + noteSizeX-2;
            break;
        default:break;
    }
    
    if (guide) {
        auto guideFilter = _gameNode->getChildByName("guideFilter");
        guideFilter->setVisible(true);
    }
    
    for (int y=0; y<noteSizeY; y++) {
        for (int x=0; x<noteSizeX; x++) {
            auto slot = NoteSlot::create();
            slot->setCoordinate(x, y);
            slot->setPosition(startPosX + x*160, startPosY - y*180);
            
            if (guide) {
                if (y == enableY && x > enableXStart && x <= enableXEnd) { }
                else slot->setDisable();
            }
            
            _problemNode->addChild(slot);
            _noteSlots.push_back(slot);
            
            if (x == noteSizeX-1) break;
            
            auto middleSlot = NoteSlot::create();
            middleSlot->setMiddle(x, x+1, y);
            middleSlot->setPosition(startPosX + x*160 + 80, startPosY - y*180);
            _problemNode->addChild(middleSlot);
            _noteSlots.push_back(middleSlot);
            
        }
    }
}

void WordNoteScene::drawPlateSlot(WordNoteLevelStruct* problem) {
    
    vector<string> pieceLines;
    vector<string> pieceLetters;
    vector<string> activeCards;
    pieceLines.clear();
    pieceLetters.clear();
    activeCards.clear();
    
    stringstream ssLine(problem->card);
    while(ssLine.good()) {
        string substr;
        getline(ssLine, substr, '/');
        pieceLines.push_back(substr);
    }
    
    stringstream ssLineAC(problem->activeCard);
    while(ssLineAC.good()) {
        string substr;
        getline(ssLineAC, substr, ',');
        if (TodoUtil::trim(substr).size() <= 0) continue;
        activeCards.push_back(substr);
    }
    
    int totalLine = (int)pieceLines.size();
    int currentLine = 0;
    
    bool plateShuffle = totalLine>1 ? false : true;
    
    float startPointY = _plate->getContentSize().height/2 + totalLine*185/2-16;
    for (auto pieceLine : pieceLines) {
        
        //CCLOG("pieceLine: %s", pieceLine.c_str());
        pieceLetters.clear();
        
        stringstream ss(pieceLine);
        while(ss.good()) {
            string substr;
            getline(ss, substr, ',');
            pieceLetters.push_back(substr);
        }
        
        vector<PlateSlot*> plateSlotsForThisLine;
        plateSlotsForThisLine.clear();
        int totalLettersForThisLine = 0;
        
        if (plateShuffle) random_shuffle(pieceLetters.begin(), pieceLetters.end(), [](int n) { return rand() % n; });
        
        for (auto pieceLetter : pieceLetters) {
            //CCLOG("pieceLetter: %s", pieceLetter.c_str());
            
            int pieceLetterSize = (int)pieceLetter.size();
            auto plateSlot = PlateSlot::create();
            
            plateSlot->setSlotSize(pieceLetterSize);
            //plateSlot->setPosition(_plate->getContentSize()/2);
            _problemNode->addChild(plateSlot);
            
            auto piece = createPiece(pieceLetter, plateSlot);
            if (activeCards.size() != 0 && find(activeCards.begin(), activeCards.end(), pieceLetter) == activeCards.end()) {
                piece->setTouchEnable(false);
            }
            _problemNode->addChild(piece);
            plateSlot->registerPiece(piece);
            
            _plateSlots.push_back(plateSlot);
            plateSlotsForThisLine.push_back(plateSlot);
            totalLettersForThisLine += pieceLetterSize;
        }
        
        int currentLetter = 0;
        float totalLineWidth = totalLettersForThisLine * 170;
        float startPointX = _gameNode->getContentSize().width / 2 - totalLineWidth / 2;
        float positionYForSlot = startPointY - ((float)currentLine/totalLine + (float)(currentLine+1)/totalLine)/2 * (190*totalLine);
        currentLine++;
        
        //CCLOG("positionYForSlot: %f", positionYForSlot);
        
        for (auto plateSlot : plateSlotsForThisLine) {
            float positionXForSlot = ((float)currentLetter/totalLettersForThisLine + (float)(currentLetter+plateSlot->getLetterLength())/totalLettersForThisLine) / 2 * totalLineWidth;
            plateSlot->move(Vec2(startPointX+positionXForSlot, positionYForSlot));
            currentLetter += plateSlot->getLetterLength();
            //CCLOG("currentLetter/totalLettersForThisLine: %f", (float)currentLetter/totalLettersForThisLine);
            //CCLOG("positionXForSlot: %f", positionXForSlot);
            //CCLOG("getCharacterLength: %d", plateSlot->getCharacterLength());
            //CCLOG("currentLetter: %d", currentLetter);
            //CCLOG("totalLettersForThisLine: %d", totalLettersForThisLine);
        }
    }

    
}

vector<NoteSlot*> WordNoteScene::getHaveToSpeechSlots(NoteSlot* noteSlot) {
    vector<NoteSlot*> haveToSpeechSlots;
    int startX, coordinateY;
    string word = "";

    haveToSpeechSlots.clear();
    
    coordinateY = noteSlot->getCoordinateY();
    
    if (noteSlot->isMiddle()) {
        startX = noteSlot->getPrevious();
    } else {
        startX = noteSlot->getCoordinateX();
    }
    for (; startX>-1; startX--) {
        auto checkNoteSlot = getNoteSlot(startX, coordinateY);

        if (checkNoteSlot->getLetter() == "") {
            startX++;
            break;
        }
        if (startX == 0) break;
    }
    
    for (int i=startX; i<noteSizeX; i++) {
        auto checkNoteSlot = getNoteSlot(i, coordinateY);
        if (checkNoteSlot) {
            if (checkNoteSlot->getLetter() == "") break;
            if (checkNoteSlot->_placedPiece) haveToSpeechSlots.push_back(checkNoteSlot);
        } else break;

        auto checkNoteSlotMiddle = getNoteSlotMiddle(i, coordinateY);
        if (checkNoteSlotMiddle && checkNoteSlotMiddle->_placedPiece) haveToSpeechSlots.push_back(checkNoteSlotMiddle);
        word += checkNoteSlot->getLetter();
    }
    
    //CCLOG("word: %s", word.c_str());
    speechSlots(haveToSpeechSlots);
    return haveToSpeechSlots;
}

void WordNoteScene::speechSlots(vector<NoteSlot*> noteSlots) {
    string word;

    for (auto slot : noteSlots) {
        word += slot->_placedPiece->getLetter();
        auto actionSpeech = Sequence::create(ScaleTo::create(0.1, 1.2),DelayTime::create(1),ScaleTo::create(0.1, 1),nullptr);
        slot->_placedPiece->_paper->runAction(actionSpeech);
    }
    
    speech(word);
}


Piece* WordNoteScene::createPiece(string letter, PlateSlot* plateSlot) {
    auto piece = Piece::create();
    piece->setLetter(letter);
    piece->setLocalZOrder(_zIndex++);
    
    piece->onTouchBegan = [this, piece, plateSlot]() {
        piece->togglePicked(true);
        if (piece->_placedNoteSlot) {
            //getHaveToSpeechSlots(piece->_placedNoteSlot);
            releasePiece(piece);
        }
        if (piece->_isOnPlateSlot && _problem.canDuplicate) {
            CCLOG("?");
            //speech(piece->getLetter());
            auto newPiece = createPiece(piece->getLetter(), plateSlot);
            plateSlot->registerPiece(newPiece);
            newPiece->setPosition(plateSlot->getPosition());
            _problemNode->addChild(newPiece);
        }
        piece->setLocalZOrder(_zIndex++);
    };
    
    piece->onTouchMoved = [this, piece](Touch* touch){
        placeableNoteSlotNearTouch(touch, piece);
    };
    
    piece->onTouchEnded = [this, piece](Touch* touch){
        
        auto targetNoteSlot = placeableNoteSlotNearTouch(touch, piece);
        if (targetNoteSlot) {
            auto affectedSlots = getAffectedSlots(targetNoteSlot, piece, false);
            if (affectedSlots.size() == piece->getLetter().size()) {
                for (int i=0; i<affectedSlots.size(); i++) {
                    affectedSlots[i]->setLetter(string(1,piece->getLetter()[i]));
                }
            }
            //printLine(targetNoteSlot->getCoordinateY());
            piece->place(targetNoteSlot);

            auto problem = _problems[_currentProblemIndex];
            StrictLogManager::shared()->game_Peek_Answer("WordNote", makeWorkPath(), piece->getLetter(), problem.wordSentence);

            if (!checkAnswer()) getHaveToSpeechSlots(targetNoteSlot);
            piece->_isOnPlateSlot = false;
        } else {
            piece->_isOnPlateSlot = true;
            piece->_registeredPlateSlot->cleanPiecesOnPlateSlot();
            piece->runAction(EaseIn::create(MoveTo::create(0.1, piece->_registeredPlateSlot->getPosition()),3.0));
        }
        hiddenSelectedEffect();
        piece->togglePicked(false);
    };
    return piece;
}

void WordNoteScene::printLine(int coordinateY) {
    
    for (auto noteSlot : _noteSlots) {
        if (noteSlot->getCoordinateY() == coordinateY && !noteSlot->isMiddle()) {
            CCLOG("x:%d, y:%d = %s", noteSlot->getCoordinateX(), noteSlot->getCoordinateY(), noteSlot->getLetter().c_str());
        }
    }

}

void WordNoteScene::hiddenSelectedEffect() {
    _selectedEffect1->setVisible(false);
    _selectedEffect2->setVisible(false);
    _selectedEffect3->setVisible(false);
    _selectedEffect4->setVisible(false);
}

void WordNoteScene::onExit() {
    LOGFN();
    Layer::onExit();
    
    removeObjects();
    this->removeAllChildren();
    this->unscheduleAllCallbacks();
    GameSoundManager::getInstance()->stopAllEffects();
    SpriteFrameCache::getInstance()->removeUnusedSpriteFrames();
    Director::getInstance()->getTextureCache()->removeUnusedTextures();
    
}

string WordNoteScene::makeWorkPath() {
    stringstream ss;
    ss << "/" << "WordNote";
    ss << "/" << "level-" << _currentLevel << "-" << _currentSheetNo;
    ss << "/" << "work-" << _currentProblemIndex;
    return ss.str();
}

END_NS_WORDNOTE;
