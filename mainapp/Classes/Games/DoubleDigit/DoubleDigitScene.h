//
//  DoubleDigitScene.h
//  todomath
//
//  Created by Sunmi Seol on 12/14/15.
//
//

#ifndef __todomath__DoubleDigitScene__
#define __todomath__DoubleDigitScene__


#include "JotPlane.h"
#include <string>

#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
#include "3rdParty/json/json.h"
#include "Common/Controls/ProgressIndicator.hpp"


class DoubleDigitScene : public Layer {

public:
    DoubleDigitScene();
    virtual ~DoubleDigitScene();
    
    CREATE_FUNC(DoubleDigitScene);
    virtual void prepareNewGameWithLevel(Json::Value problemParameter);
    
    virtual bool init() override;
    
    void handleStart();
    void handlePause();
    void handleResume();
    void handleQuit();
    
    virtual void onExit() override;

    
    virtual void handleAnswerEntered(std::string &answer);
    
    
private:
    
    int currentLevel;
    Json::Value problems;
    
    int numSolved;
    
    //float offset_x;
    //float bg_x, bg_y;
    
    
    
    
    float gameScale;
    
    
    Node *notePad;
    Node *notePadBG, *notePadPage, *notePadBinding;
    
    
    
    NodeGrid *notePageGrid;
        Node *notePageProblemView;
        JotPlane *notePageJotPlane;
    
    
    NodeGrid *notePageGridFading;

    Node *answerPad;
    std::string answerString;
    Label *answerLabel;
    Point answerLabelPos;
    
    ProgressIndicator *_progressBar;
    
private:
    
    void initNotePad();
    void createNotePage();
    
    void initAnswerPad();
    
    
    void refreshAnswerLabel();
    void startNewStage();
    void showProblem(Node *view, int indexProblem);
    void showAnswer(Node *view, int indexProblem);
    
    void showComplete(float);
 
    
    void onSolved(float);
    void turningPageAnimation();
    void gotoNextPage();
    void makeFakeView();
    

};



#endif /* defined(__todomath__DoubleDigitScene__) */
