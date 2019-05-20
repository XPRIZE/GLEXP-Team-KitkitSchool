//
//  TextAndTextLayer.cpp
//  KitkitSchool
//
//  Created by timewalker on 26/12/2016.
//
//

#include "TextAndTextLayer.hpp"
#include "TextAnswerItem.hpp"

namespace
{
    const int kAnswerCount = 3;
    const float kGapBetweenEachItem = 30.f;
    const string kNormalSpeakerButton = "lrcomprehension/common/comprehension-speaker-normal.png";
    const string kPressedSpeakerButton = "lrcomprehension/common/comprehension-speaker-active.png";
    
    const string kNormalSpeakerGuideButton = "lrcomprehension/common/comprehension-speaker-b-normal.png";
    const string kPressedSpeakerGuideButton = "lrcomprehension/common/comprehension-speaker-b-active.png";
}

BEGIN_NS_LR_ALLINONE_TYPE_QUESTION;

bool TextAndTextLayer::init()
{
    if (!Node::init()) return false;
    setContentSize(fixedDeviceSize);
    return true;
}

void TextAndTextLayer::setGuideView(QuestionType questionType, string guideTextOrSoundPath)
{
    _questionType = questionType;
    _guideTextOrSoundPath = guideTextOrSoundPath;
}

void TextAndTextLayer::setQuestionAnswers(std::string questionText, std::string questionSoundPath, std::vector<std::string> answerTexts)
{
    random_shuffle(answerTexts.begin(), answerTexts.end());
    
    auto guideArea = Scale9Sprite::create("lrcomprehension/common/comprehensivequiz-picture-box.png");
    guideArea->setContentSize(Size(874.f, 1218.f));
   
    if (_questionType == QuestionType::Listening)
    {
        auto guideSoundButton = LRSoundButton::createLarge(_guideTextOrSoundPath);
        guideSoundButton->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        guideSoundButton->setPosition(guideArea->getContentSize()/2);
        guideArea->addChild(guideSoundButton);
    }
    else
    {
        auto guideText = createMultiLine(_guideTextOrSoundPath, 60.f, guideArea->getContentSize() * 0.88f, FONT_MUKTA_BOLD, Color4B(77, 77, 77, 255), 2.8f);
        guideText->setPosition(guideArea->getContentSize()/2);
        guideArea->addChild(guideText);
    }
    
    auto answerArea = Node::create();
    if (answerTexts.size() > 0)
    {
        auto item = TextAnswerItem::create();
        item->initText(answerTexts[0]);
        Size totalSize = Size(item->getContentSize().width, item->getContentSize().height * kAnswerCount) + Size(0.f, kGapBetweenEachItem * (kAnswerCount - 1));
        answerArea->setContentSize(totalSize);
        CCLOG("total : %f, %f", totalSize.width, totalSize.height);
    }
    
    float posX = 0.f;
    float posY = answerArea->getContentSize().height;
    
    for (int i = 0; i < answerTexts.size(); i++)
    {
        TextAnswerItem* item = TextAnswerItem::create();
        item->initText(answerTexts[i]);
        item->setLetterByIndex(i);
        item->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
        item->setPosition(posX, posY);
        answerArea->addChild(item);
        
        posY -= item->getContentSize().height + kGapBetweenEachItem;
        
        item->onCheckTarget = [this, item]() {
            if(item->currentState == TextAnswerState::Right || item->currentState == TextAnswerState::Wrong)
                return;
            
            if (parentScene->isCorrect(item->getId()))
            {
                parentScene->solve(item->getId());
                item->setState(TextAnswerState::Right);
                
                if (parentScene->isSolved())
                    parentScene->onSolve();
            }
            else
            {
                item->setState(TextAnswerState::Wrong);
            }
        };
    }
    
    //this->setContentSize(Size(guideArea->getContentSize().width + 70.f + answerArea->getContentSize().width, guideArea->getContentSize().height));
    
    guideArea->setAnchorPoint(Vec2::ANCHOR_MIDDLE_RIGHT);
    guideArea->setPosition(this->getContentSize().width/2-196.f, this->getContentSize().height/2);
    this->addChild(guideArea);
    
    answerArea->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
    answerArea->setPosition(this->getContentSize().width/2-120.f, this->getContentSize().height/2-184.f);
    this->addChild(answerArea);
    
    _questionSoundButton = LRSoundButton::createSmall(questionSoundPath);
    _questionSoundButton->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
    _questionSoundButton->setPosition(Vec2(0.f, answerArea->getContentSize().height) + Vec2(5, 90));
    answerArea->addChild(_questionSoundButton);
    
    auto questionLabel = createMultiLine(questionText, 80.f, Size(749.f, 268.f), FONT_ANDIKA_BOLD, Color4B(77, 77, 77, 255));
    questionLabel->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
    questionLabel->setPosition(Vec2(_questionSoundButton->getContentSize().width, _questionSoundButton->getContentSize().height/2) + Vec2(32.f, -10.f));
    _questionSoundButton->addChild(questionLabel);
    //DRAW_DEBUG_AREA(questionLabel);
}

Node* TextAndTextLayer::createMultiLine(string text, float fontSize, Size dim, string fontName, Color4B fontColor, float lineSpacingRatio)
{
    vector<string> words = TodoUtil::split(text, ' ');
    
    bool complete = false;
    Node *page;
    
    int trial = 5;
    
    do
    {
        page = Node::create();
        page->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        Node *innerPage = Node::create();
        
        Point pos = Point::ZERO;
        Size pageSize = Size::ZERO;
        
        float pageTopMargin = 0;
        
        auto pageWidth = dim.width;
        auto wordHeight = fontSize;
        auto lineSpacing = fontSize*0.25 * lineSpacingRatio;
        auto wordSpacing = fontSize*0.25;
        
        for (auto word : words)
        {
            auto slot = createWord(word, fontSize, fontName, fontColor);
            auto slotSize = slot->getContentSize();
            slot->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
            if (pageWidth > 0 && pos.x + slotSize.width > pageWidth)
            {
                pos.x = 0;
                pos.y -= wordHeight + lineSpacing;
            }
            
            slot->setPosition(pos);
            
            pageSize.width = MAX(pageSize.width, pos.x+slotSize.width);
            pageSize.height = MIN(pageSize.height, pos.y-slotSize.height);
            
            pageTopMargin = MAX(pageTopMargin, pos.y);
            
            pos.x += wordSpacing + slotSize.width;
            
            innerPage->addChild(slot);
        }
        
        pageSize.height *= -1.0;
        pageSize.height += pageTopMargin;
        
        page->setContentSize(pageSize);
        innerPage->setPosition(Vec2(0, pageSize.height-pageTopMargin));
        
        page->addChild(innerPage);
        
        complete = pageSize.height <= dim.height;
        if (!complete)
        {
            auto scale = sqrt(dim.height / pageSize.height)* 0.9;
            fontSize *= scale;
        }
        
        trial--;
    } while (!complete && trial > 0);
    
    return page;
}

Node* TextAndTextLayer::createWord(string word, float fontSize, string fontName, Color4B fontColor)
{
    auto underlineSize = fontSize*0.5;
    auto wordHeight = fontSize;
    
    Node *node = Node::create();
    Size wordSize = Size(0, wordHeight);
    
    while (word.length()>0)
    {
        if (word[0]=='_')
        {
            int underlineLength;
            
            size_t last = word.find_first_not_of("_");
            if (last==string::npos)
            {
                underlineLength = word.length();
                word = "";
            }
            else
            {
                underlineLength = last;
                word = word.substr(last, word.length()-last);
            }
            
            auto width = underlineLength*underlineSize;
            
            auto slot = DrawNode::create();
            slot->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
            
            
            slot->drawLine(Vec2::ZERO, Vec2(width, 0), Color4F(fontColor));
            auto slotSize = Size(width, wordHeight);
            slot->setContentSize(slotSize);
            slot->setPosition(Vec2(wordSize.width, 0));
            
            wordSize.width += slotSize.width;
            
            node->addChild(slot);
        }
        
        
        if (word.length() > 0)
        {
            string currentSyl;
            
            size_t nextUnder = word.find_first_of("_");
            if (nextUnder!=string::npos)
            {
                currentSyl = word.substr(0, nextUnder);
                word = word.substr(nextUnder, word.length()-nextUnder);
            }
            else
            {
                currentSyl = word;
                word = "";
            }
            
            auto label = TodoUtil::createLabel(currentSyl, fontSize, Size::ZERO, fontName, fontColor);
            label->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
            auto labelSize = label->getContentSize();
            
            label->setPosition(Vec2(wordSize.width, 0));
            
            wordSize.width += labelSize.width;
            node->addChild(label);
        }
    }
    
    node->setContentSize(wordSize);
    
    return node;
}

END_NS_LR_ALLINONE_TYPE_QUESTION;
