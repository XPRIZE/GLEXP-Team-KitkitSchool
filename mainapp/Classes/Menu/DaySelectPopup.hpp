//
//  DaySelectPopup.hpp
//  enumaXprize
//
//  Created by Gunho Lee on 12/25/16.
//
//

#ifndef DaySelectPopup_hpp
#define DaySelectPopup_hpp

#include "Common/Controls/PopupBase.hpp"
#include <cocos/ui/CocosGUI.h>
#include <functional>
#include <string>

USING_NS_CC;

class LightBall;

class DaySelectPopup : public Node
{
public:
    
    DaySelectPopup() : onSelectDay(nullptr), onDismiss(nullptr) {}
    
    static DaySelectPopup* create(Node* parentView, std::string levelID);
    bool init(Node* parentView, std::string levelID);
    
    std::function<void(int day)> onSelectDay;
    std::function<void()> onDismiss;
    
    
    void show();
    void showClear(int day);
    
    void dayChosen(int day);
    void dismiss();
    
    

private:
    
    void lightFall(LightBall *l, bool first);
    

    Node *_parentView;
    
    Node *_backView;
    Node *_bgView;
    Node *_mainView;
    
    Vec2 _boardPos;
    
    Node *_board;
    Node *_stage;
    
    std::string _levelID;
    
    bool _touchEnabled;
    

    

};

class LightBall : public Node
{
public:
    CREATE_FUNC(LightBall);
    
    void setup(int type);
    
    Sprite *_ball;
    int _type;
    
    
    
};

#endif /* DaySelectPopup_hpp */
