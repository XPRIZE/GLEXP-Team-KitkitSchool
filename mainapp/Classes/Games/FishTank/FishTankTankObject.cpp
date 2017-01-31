//
//  FishTankTankObject.cpp
//  todomath
//
//  Created by Jesun Kim on 2/19/16.
//
//

#include "FishTankTankObject.h"

FishTankTankObject::FishTankTankObject(std::string strFileName)
{
    m_fishCount = 0;
    m_fishFirstCount = 0;
    
    m_isSource = false;
    m_isClear = false;
    
    m_spriteCircle = NULL;
    
    m_node = static_cast<Node *>(CSLoader::createNode(strFileName));
    
    // Text 크기 180 만들기(cocos stduio에서는 사이즈가 100까지밖에 안되므로...)
    m_text = static_cast<Text *>(m_node->getChildByName("text"));
    if (m_text) {
        m_text->setFontName(defaultFont);
        m_text->setFontSize(180);
        m_textPhase2 = static_cast<Text *>(m_node->getChildByName("textPhase2"));
        if (m_textPhase2) {
            m_textPhase2->setFontName(defaultFont);
            m_textPhase2->setFontSize(180);
        }else{
            // minus 문제
            m_spriteCircle = static_cast<Sprite *>(m_node->getChildByName("spriteCircle"));
            m_textPhase2 = static_cast<Text *>(m_spriteCircle->getChildByName("text"));
            auto p = m_textPhase2->getPosition();
            p.y-=5;
            m_textPhase2->setPosition(p);
            m_textPhase2->setFontName(defaultFont);
        }
    }
}

FishTankTankObject::~FishTankTankObject()
{
}

void FishTankTankObject::initData()
{
    m_fishCount = 0;
    m_isClear = false;
}

void FishTankTankObject::setBoundary()
{
    Node *nodeBoundaryLT = static_cast<Node *>(m_node->getChildByName("nodeBoundaryLT"));
    Node *nodeBoundaryRB = static_cast<Node *>(m_node->getChildByName("nodeBoundaryRB"));
    
    
    auto pos = m_node->getPosition();
    int startX = pos.x + nodeBoundaryLT->getPosition().x;
    int rangeX = nodeBoundaryRB->getPositionX() - nodeBoundaryLT->getPositionX();
    int startY = pos.y + nodeBoundaryRB->getPosition().y;
    int rangeY = nodeBoundaryLT->getPositionY() - nodeBoundaryRB->getPositionY();
//    
//    int startX = m_node->convertToWorldSpace(nodeBoundaryLT->getPosition()).x;
//    int rangeX = nodeBoundaryRB->getPositionX() - nodeBoundaryLT->getPositionX();
//    int startY = m_node->convertToWorldSpace(nodeBoundaryRB->getPosition()).y;
//    int rangeY = nodeBoundaryLT->getPositionY() - nodeBoundaryRB->getPositionY();
    m_leftBoundary = startX;
    m_rightBoundary = startX + rangeX;
    m_bottomBoundary = startY;
    m_topBoundary = startY + rangeY;
    
    Node *nodeEnterLT = static_cast<Node *>(m_node->getChildByName("nodeEnterLT"));
    Node *nodeEnterRB = static_cast<Node *>(m_node->getChildByName("nodeEnterRB"));
    
    m_leftEnter = pos.x + nodeEnterLT->getPosition().x;
    m_topEnter = pos.y + nodeEnterLT->getPosition().y;
    m_rightEnter = pos.x + nodeEnterRB->getPosition().x;
    m_bottomEnter = pos.y + nodeEnterRB->getPosition().y;
    

    
}

void FishTankTankObject::setFishCount(int number)
{
    m_fishCount += number;
    // 뺄셈 문제가 아닐때만...
    if (m_isSource && !m_spriteCircle) {
        return;
    }
    
    if (m_fishCount == m_needCount && !m_isSource) {
        m_text->runAction(FadeOut::create(0.3f));
        m_isClear = true;
    }else{
        if (!m_isSource) {
            this->stopZeroAnimation();
            m_text->runAction(FadeIn::create(0.3f));
            m_isClear = false;
        }
    }
    
    // 뺄셈 문제일 경우...
    if (m_spriteCircle) {
        std::ostringstream s;
        s << m_fishCount;
        m_textPhase2->setString(s.str());
        if (m_isSource) {
            if (m_fishFirstCount == m_fishCount) {
                m_spriteCircle->setVisible(false);
            }else{
                m_spriteCircle->setVisible(true);
            }
        }else{
            if (m_fishCount == 0) {
                m_spriteCircle->setVisible(false);
            }else{
                m_spriteCircle->setVisible(true);
            }
        }
    }
}

void FishTankTankObject::drawZeroAnimation()
{
    m_text->setOpacity(255);
    Blink *blink = Blink::create(3.0, 3);
    Sequence *seq = Sequence::createWithTwoActions(blink, Hide::create());
    seq->setTag(111);
    m_text->runAction(seq);
}

void FishTankTankObject::stopZeroAnimation()
{
    m_text->stopActionByTag(111);
    m_text->setVisible(true);
}
