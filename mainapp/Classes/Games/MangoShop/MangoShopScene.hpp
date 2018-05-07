//
//  MangoShopScene.hpp
//  KitkitSchool
//
//  Created by HyeonGyu Yu on 3/6/18.
//

#ifndef MangoShopScene_hpp
#define MangoShopScene_hpp

#include "cocos2d.h"
#include <string>
#include "Box.hpp"
#include "Mango.hpp"
#include "MangoSlot.hpp"
#include "NumberCard.hpp"
#include "Tray.hpp"
#include "BlankSlot.hpp"
#include "Problem.hpp"
#include "Common/Controls/ProgressIndicator.hpp"

USING_NS_CC;
using namespace std;

namespace MangoShop
{
    class MangoShopScene : Layer
    {
    // Essential variables, methods
    private:
    public:
        MangoShopScene();
        static Scene* createScene(int levelID);
        virtual bool init() override;
        virtual void onEnter() override;
        virtual void onEnterTransitionDidFinish() override;
        CREATE_FUNC(MangoShopScene);
        
    // Custom variables, methods
    private:
        BlankSlot* targetBlankSlot;
        BlankSlot* candidateBlankSlot;
        
        Node* _gameNode;
        void initData();
        void initUI();
        NumberCard *_pickedNumberCard;
        NumberCard *_dummyNumberCard;
        vector<BlankSlot*> _blankSlots;
        vector<NumberCard*> _numberCards;
        
        vector<string> _problemNumberStrings;
        string _problemSignString;
        string _solutionNumberString;
        
        bool _isLandscape;
        bool _isSumExpression; // sum or sub expression
        
        Tray* _tray;
        
        Sprite* _emptySlot1;
        Sprite* _emptySlot2;

        Node* boxRootNode;
        Node* numberCardsRoot;
        Node* numberExpressionRoot;
        
        vector<Box*> boxes;
        vector<Mango*> mangos;

        vector<Problem*> _problems;
        int _currentLevel;
        int _currentProblemIndex;
        ProgressIndicator *_progressBar;
        
        Sprite* _tutorialHand;
        Node* _tutorialLayer1;
        Node* _tutorialLayer2;
        
        void drawTutorialLayerStep1();
        void drawTutorialLayerStep2();
        
        Vec2 getFistLeftBoxWorldPositionForTutorial();
        Vec2 getLeftWorldPositionForTutorial();
        Vec2 getRightWorldPositionForTutorial();
        
        Node* _shiningParticleNode;
        
    public:
        void setPreventTouches(bool value);
        void animateMove(Node* target, float duration, Vec2 destination, function<void()> onComplete);
        void setLevel(int level);
        void onStart();
        void setProblem();
        bool addMango(Box *box);
        void addMangos(Box *box, int count, bool isAutoShowCover);
        void setBoxes();
        void setNumberExpression();
        void setNumberCards();
        void bindingBoxEvent(Box *box);
        void bindingMangoEvent(Mango *mango);
        void bindingCardEvent(NumberCard *card);
        bool trayIsFull();
        void moveTray();
        void resetBlankSlotEffect();
        bool isTrayFull();
        bool isSolved();
        void onSolve();
        void createShiningParticle();
        void startParticle(Vec2 worldPosition);
        void stopParticle();
        
        void writeGameLog(NumberCard* card, BlankSlot* candidateSlot);
    };

} // end namespace

#endif /* MangoShopScene_hpp */
