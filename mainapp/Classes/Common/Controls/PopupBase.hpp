//
//  PopupBase.hpp
//  KitkitSchool
//
//  Created by Sungwoo Kang on 11/1/16.
//
//

#ifndef PopupBase_hpp
#define PopupBase_hpp

#include "cocos2d.h"
#include "extensions/GUI/CCControlExtension/CCControlExtensions.h"


USING_NS_CC;
USING_NS_CC_EXT;

class PopupBase : public Node
{
public:
    
    ~PopupBase();
    
    static PopupBase* create(Node* parent);
    static PopupBase* create(Node* parent, Size size);
    
    
    bool init(Node* parent);
    bool init(Node* parent, Size size);
    
    void close();
    
    virtual void show(Node *_parentModal, bool animate);
    virtual void show(Node *_parentModal, bool animate, float delay);
    virtual void show(Node *_parentModal, bool animate, cocos2d::CallFunc *callback);
    void dismiss(bool animate);
    void backviewTouched(Ref *ref, Control::EventType eventType);
    
    void backKeyPressed();
    
    void setAllowDismissByTouchingOutside(bool allow);
    
    Node *parentView;
    Node *backgroundView;
    LayerColor *bgColor;
    CallFunc *dismissCallback;
    CallFunc *onloadCallback;
    bool showFromTop;
    
private:
    bool allowDismissByTouchingOutside;
    bool _isActive;
    
    EventListenerTouchOneByOne* _listener;
    
};

#endif /* PopupBase_hpp */
