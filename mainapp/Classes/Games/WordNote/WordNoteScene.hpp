//
//  WordNoteScene.hpp
//  KitkitSchool-mobile
//
//  Created by JungJaehun on 21/02/2018.
//

#pragma once
#include "WordNoteHeader.h"
#include "WordNotePiece.hpp"
#include "PlateSlot.hpp"
#include "NoteSlot.hpp"
#include "WordNoteProblemBank.hpp"
#include "Common/Controls/ProgressIndicator.hpp"

BEGIN_NS_WORDNOTE;

class WordNoteScene : public Layer {
public:
    CREATE_FUNC(WordNoteScene);
    static Scene* createScene(int levelID);
    
    virtual bool init() override;
    virtual void onEnter() override;
    virtual void onExit() override;
    virtual void onEnterTransitionDidFinish() override;
    
    void setLevel(int level);
    void setProblem();
    void goNextProblem();
    void loadData(int level);
    void loadSoundDurationSheet();
    void drawQuestion(WordNoteLevelStruct* problem);
    void drawNoteSlot();
    void drawPlateSlot(WordNoteLevelStruct* problem);
    void hiddenSelectedEffect();
    bool checkAnswer();
    void removeObjects();
    void answerFadeIn();
    void onCorrect(vector<NoteSlot*> noteSlots);
    bool touchSoundButton();
    float speech(string input);
    float speechWithFiles(string input, float timing=0);
    float speechWithTTS(string input);
    
    vector<NoteSlot*> getAffectedSlots(NoteSlot* noteSlot, Piece* piece, bool effect, bool forRelease = false);
    vector<NoteSlot*> getHaveToSpeechSlots(NoteSlot* noteSlot);
    void speechSlots(vector<NoteSlot*> noteSlots);
    NoteSlot* placeableNoteSlotNearTouch(Touch* touch, Piece* piece);
    NoteSlot* getNoteSlot(int x, int y);
    vector<NoteSlot*> getNoteSlots(int Xstart, int Xend, int y);
    NoteSlot* getNoteSlotMiddle(int x, int y);
    NoteSlot* isPlaceableNoteSlot(int x, int y);
    void releasePiece(Piece* piece);
    Piece* createPiece(string letter, PlateSlot* plateSlot);

private:
    enum ProblemType {
        TEXT_WITH_IMAGE,
        TEXT_ONLY,
        SOUND_ONLY
    };
        
    int _currentLevel, _currentSheetNo,  _currentProblemIndex, _totalProblemCount, _zIndex, _fadeInCount;
    Node *_gameNode, *_problemNode, *_plate;
    Size _winSize, _gameSize;
    float _gameScale;
    float _onCorrectSpeechDuration;
    float _ttsAlert = false;
    vector<WordNoteLevelStruct> _problems;
    WordNoteLevelStruct _problem;
    vector<PlateSlot*> _plateSlots;
    vector<NoteSlot*> _noteSlots;
    vector<Piece*> _correctPieces;
    vector<SoundDuration> _soundDurationSheet;
    
    Sprite *_bgCorrect, *_soundButton, *_soundButtonActive, *_selectedEffect1, *_selectedEffect2, *_selectedEffect3, *_selectedEffect4;

    ProgressIndicator *_progressBar;
    void printLine(int coordinateY);
    std::string makeWorkPath();

};

END_NS_WORDNOTE;

