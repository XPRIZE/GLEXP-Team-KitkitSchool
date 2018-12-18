//
//  ComprehensionScene.hpp
//
//  Created by Gunho Lee on 12/10/16.
//
//

#ifndef ComprehensionScene_hpp
#define ComprehensionScene_hpp

#include <stdio.h>
#include "cocos2d.h"
#include "ui/CocosGUI.h"

#include <vector>
#include <map>

#include "Common/Controls/ProgressIndicator.hpp"

USING_NS_CC;
using namespace std;
using namespace cocos2d::ui;

class ComprehensionScene : public Layer
{
    typedef tuple<string, int> ComprehensionProblemKey;
    typedef pair<string, vector<string>> ComprehensionProblem;
    
public:
    static Scene* createScene(string bookFolder, int set);
    static Scene* createScene(string bookFolder, int set, bool checkCompleteCondition);
    static int getNumSet(string bookFolder);
    
    virtual bool init() override;
    CREATE_FUNC(ComprehensionScene);
    virtual void onEnter() override;
    virtual void onEnterTransitionDidFinish() override;
    
    vector<ComprehensionProblem> problemSet;
    
    void onStart();
    void onSolve();
    void showBookChooser();
    void showSetChooser();
    
    static bool getSolveAll() { return _isSolveAll; }
    
    void setCheckCompleteCondition(bool checkCompleteCondition);
    
    void setBookData(string bookFolder, int set);
    std::string getBookFolder() { return _bookFolder; }
    std::string getBookName();
    int getCurrentProblem();
    Button* drawSoundButton(Node* targetNode);
    void drawQuestionTitle(string titleText, Node* parentNode, float addLeftPadding = 0.f);

private:
    bool _checkCompleteCondition = true;
    
    Node *_pageNode;
    NodeGrid *_pageGrid;
    int _currentPageGridZ;
    Node *_gameNode;
    Node *_paperBottom;
    
    string _bookFolder;
    int _problemSetIndex;
    //int _bookNo;
    //string _languageTag;
    
    static bool _isSolveAll;
    int _currentProblem;
    ProgressIndicator *_progressBar;
    EventListenerTouchOneByOne *_blocker;
    
    NodeGrid* createPageGrid();
    void convertToText();
    void createProblemMap();
    void selectProblem();
    void showProblem();
};

#endif /* TappingScene_hpp */
