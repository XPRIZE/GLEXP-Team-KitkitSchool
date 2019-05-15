//
//  GameObject.cpp
//  todomath
//
//  Created by KimDonggyu on 2016. 9. 30..
//
//

#include "GameObject.h"

GameObject::GameObject(){
    m_objectIndex = 0;
    m_fileName = "";
    m_successFileName = "";
    m_answer = "";
    
    m_spriteBox = NULL;
    m_spriteDot = NULL;
    m_spriteObject = NULL;
};
GameObject::~GameObject(){
    // 도트 관리
    m_vecDot.clear();
    m_vecDotAnchorPosition.clear();
};

Node* GameObject::getEffectNode(){
    // 이펙트 생성
    Node *nodeEffect = static_cast<Node *>(CSLoader::createNode("LineMatching/waterdrop.csb"));
    nodeEffect->setPosition(m_spriteBox->getPosition());
    return nodeEffect;
}

void GameObject::changeObjectTexture(Node *pSender, Node *nodeEffect,
                                     Vector<DrawNode*> _vecInDrawNode, Vector<DrawNode*> _vecOutDrawNode,  Color4F _successColor, int _zOrder){
    
    /*
     // 박스, 오브젝트 텍스쳐 변경, 이펙트 애니메이션, 노드 변경
     m_spriteBox->setTexture("LineMatching/Box_Green.png");
     m_spriteObject->setTexture(m_successFileName);
     m_spriteObject->setOpacity(100);
     this->effectPlay(nodeEffect);
     
     // 노드 변경
     for(int i=0; i<m_vecStartPosition.size(); i++){
     _vecInDrawNode.at(m_vecDrawIndex.at(i))->clear();
     DrawNode *outDrawNode = _vecOutDrawNode.at(m_vecDrawIndex.at(i));
     outDrawNode->clear();
     outDrawNode->setLocalZOrder(_zOrder);
     outDrawNode->drawSolidCircle(m_vecStartPosition.at(i), 20, CC_DEGREES_TO_RADIANS(90), 50, 1.0f, 1.0f, _successColor);
     outDrawNode->setLineWidth(18);
     outDrawNode->drawLine(m_vecStartPosition.at(i), m_vecFinishPosition.at(i), _successColor);
     outDrawNode->drawSolidCircle(m_vecFinishPosition.at(i), 20, CC_DEGREES_TO_RADIANS(90), 50, 1.0f, 1.0f, _successColor);
     }
     */
}

void GameObject::effectPlay(Node *nodeEffect){
    // 애니메이션 값 설정
    ActionTimeline *actionEffect = CSLoader::createTimeline("LineMatching/waterdrop.csb");
    actionEffect->gotoFrameAndPlay(0, false);
    AnimationInfo info = actionEffect->getAnimationInfo("ani1");
    float aniPlayTime = (info.endIndex - info.startIndex) / 60.f;
    
    // 재생
    nodeEffect->runAction(actionEffect);
    nodeEffect->runAction(Sequence::createWithTwoActions(DelayTime::create(aniPlayTime), RemoveSelf::create()));
    
    // 사운드 재생
    GameSoundManager::getInstance()->playEffectSound("LineMatching/Sounds/Boom.m4a");
}
