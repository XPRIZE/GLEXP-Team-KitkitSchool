//
//  GameObject.h
//  todomath
//
//  Created by KimDonggyu on 2016. 7. 20..
//
//

#ifndef GameObject_h
#define GameObject_h

#include <stdio.h>
#include "cocos2d.h"

#include "cocostudio/CocoStudio.h"
#include "Managers/GameSoundManager.h"

USING_NS_CC;
using namespace std;
using namespace ui;
using namespace cocostudio;
using namespace cocostudio::timeline;

class GameObject : public Ref {
    
public:
    GameObject();
    ~GameObject();
    
    Node* getEffectNode();
    void changeObjectTexture(Node *pSender, Node *nodeEffect,
                             Vector<DrawNode*> _vecInDrawNode, Vector<DrawNode*> _vecOutDrawNode, Color4F _successColor, int _zOrder);
    
private:
    void effectPlay(Node *nodeEffect);
    
public:
    // 게임 오브젝트 인덱스
    int m_objectIndex;
    int m_distanceIndex = -1;
    std::string m_fileName;                     // Object FileName
    std::string m_successFileName;
    
    // NB(xenosoz, 2018): Node* m_nodeAnswer가 항상 dangling pointer를 가리키고 있어서 string으로 치환하였습니다 (_ _)
    std::string m_name;
    std::string m_answer;
    
    Sprite *m_spriteBox;
    Sprite *m_spriteDot;
    Sprite *m_spriteObject;
    
    // 도트 관리
    Vector<Sprite *> m_vecDot;                  // DotObject
    std::vector<Vec2> m_vecDotAnchorPosition;   // Dot AnchorPosition
};

#endif /* GameObject_h */
