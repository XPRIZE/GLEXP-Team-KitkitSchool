//
//  LRDividedTypeQuestionScene.cpp
//  KitkitSchool
//
//  Created by timewalker on 26/12/2016.
//
//

#include "LRDividedTypeQuestionScene.hpp"
#include "TextAndImageLayer.hpp"
#include "TextAndTextLayer.hpp"
#include "ImageAndTextLayer.hpp"
#include "ImageAndImageLayer.hpp"
#include "Managers/StrictLogManager.h"

namespace
{
//    const bool bDebug = false;
    const char* defaultFont = FONT_ANDIKA_BOLD;
}

BEGIN_NS_LR_DIVIDED_TYPE_QUESTION;

LRDividedTypeQuestionScene::LRDividedTypeQuestionScene():
_currentType(QuestionType::None)
{
    
}

bool LRDividedTypeQuestionScene::init()
{
    if (!Layer::init()) return false;
    
    _gameNode = Node::create();
    _gameNode->setContentSize(fixedDeviceSize);
    _gameNode->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
    _gameNode->setPosition(getContentSize().width / 2, 0.f);
    addChild(_gameNode);
    
    return true;
}

Layer* LRDividedTypeQuestionScene::createLayer(QuestionType questionType, LRComprehensionScene* parent)
{
    auto layer = LRDividedTypeQuestionScene::create();
    layer->_currentType = questionType;
    layer->_comprehensionScene = parent;
    return layer;
}

void LRDividedTypeQuestionScene::onEnter()
{
    Layer::onEnter();
    
    initData();
    //determineItemType();
    
    switch (_currentType)
    {
        case QuestionType::TextAndText:
        {
            _questionLabelFontSize = _questionText.length() > 20 ? 65.f : 80.f;
            _questionLabelPosition = Vec2(_gameNode->getContentSize().width / 2, _gameNode->getContentSize().height - 600.f);
            _layerPosition = Vec2(0.f, -194.f);
            
            auto layer = TextAndTextLayer::create();
            layer->setPosition(_layerPosition);
            layer->parentScene = this;
            layer->setAnswers(_answers);
            _gameNode->addChild(layer);
            break;
        }
        default:
            break;
    }
    
    string directionContent = LanguageManager::getInstance()->isEnglish() ? "Select the correct answer." : "Chagua jibu sahihi.";
    
    if (_questionText.find("Choose:") != std::string::npos)
    {
        directionContent = "Select the picture to match the text";
        TodoUtil::replaceAll(_questionText, "Choose:", "");
        _questionText = TodoUtil::trim(_questionText);
    }
    
    _comprehensionScene->drawQuestionTitleWithSoundButton(directionContent, _questionSoundPath, _gameNode);
    
    createFixedResources();
}

void LRDividedTypeQuestionScene::initData()
{
    answerPairVector.clear();
    
    LRProblem* problem;
    
    for (int i = 0; i < (int)_comprehensionScene->problemSet.size(); i++)
    {
        if (i == _comprehensionScene->getCurrentProblem())
        {
            problem = _comprehensionScene->problemSet[i];
            break;
        }
    }
    
    _questionText = problem->question;
    _questionSoundPath = problem->audio;
    
    _answers.clear();
//    if (TodoUtil::trim(problem->option1).empty() == false) _answers.push_back(lineWrappnig(TodoUtil::trim(problem->option1)));
//    if (TodoUtil::trim(problem->option2).empty() == false) _answers.push_back(lineWrappnig(TodoUtil::trim(problem->option2)));
//    if (TodoUtil::trim(problem->option3).empty() == false) _answers.push_back(lineWrappnig(TodoUtil::trim(problem->option3)));
//    if (TodoUtil::trim(problem->option4).empty() == false) _answers.push_back(lineWrappnig(TodoUtil::trim(problem->option4)));
    if (TodoUtil::trim(problem->option1).empty() == false) _answers.push_back(TodoUtil::trim(problem->option1));
    if (TodoUtil::trim(problem->option2).empty() == false) _answers.push_back(TodoUtil::trim(problem->option2));
    if (TodoUtil::trim(problem->option3).empty() == false) _answers.push_back(TodoUtil::trim(problem->option3));
    if (TodoUtil::trim(problem->option4).empty() == false) _answers.push_back(TodoUtil::trim(problem->option4));

    
    answerPairVector.clear();
    for (auto answer : _answers)
    {
        answerPairVector.push_back(std::pair<std::string, bool>(answer, false));
    }
    _solutions.clear();
    _solutions.push_back(TodoUtil::trim(problem->answer));
}

void LRDividedTypeQuestionScene::createFixedResources()
{
    Size labelSize = Size(1800, 500);
    auto questionLabel = createMultiLine(_questionText, _questionLabelFontSize, labelSize, Color4B(77, 77, 77, 255));
    questionLabel->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    questionLabel->setPosition(_questionLabelPosition);
    _gameNode->addChild(questionLabel);
    
    //DRAW_DEBUG_AREA(questionLabel);
}

bool LRDividedTypeQuestionScene::isSolved()
{
    return _solutions.size() == getSolvedCount();
}

void LRDividedTypeQuestionScene::onSolve()
{
    if (_comprehensionScene)
    {
        _comprehensionScene->onSolve();
    }
}

bool LRDividedTypeQuestionScene::isCorrect(std::string answer)
{
    TodoUtil::replaceAll(answer, "\n", " ");
    for (auto solution : _solutions)
    {
        if (solution == answer)
        {
            StrictLogManager::shared()->game_Peek_Answer("LRComprehension", makeWorkPath(), answer, solution);
            return true;
            break;
        }
    }
    
    StrictLogManager::shared()->game_Peek_Answer("LRComprehension", makeWorkPath(), answer, "None");
    return false;
}

void LRDividedTypeQuestionScene::solve(std::string answer)
{
    for (int i = 0; i < answerPairVector.size(); i++)
    {
        if (answerPairVector[i].first == answer)
        {
            answerPairVector[i].second = true;
            break;
        }
    }
}

int LRDividedTypeQuestionScene::getSolvedCount()
{
    int totalCount = 0;
    for (auto answerPair : answerPairVector)
    {
        if (answerPair.second == true) totalCount++;
    }
    return totalCount;
}

Node* LRDividedTypeQuestionScene::createMultiLine(string text, float fontSize, Size dim, Color4B fontColor)
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
        auto lineSpacing = fontSize*0.25;
        auto wordSpacing = fontSize*0.25;
        
        for (auto word : words)
        {
            auto slot = createWord(word, fontSize, fontColor);
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

Node* LRDividedTypeQuestionScene::createWord(string word, float fontSize, Color4B fontColor)
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
            
            auto label = TodoUtil::createLabel(currentSyl, fontSize, Size::ZERO, defaultFont, fontColor);
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

std::string LRDividedTypeQuestionScene::lineWrappnig(std::string inputString)
{
    int lineWrapCount = 35;
    int lastIndex = 0;
    for (int i = 0; i < inputString.length(); i++)
    {
        if (inputString[i] == ' ')
        {
            if (i - lastIndex >= lineWrapCount)
            {
                if (lastIndex != 0)
                {
                    inputString[lastIndex] = '#';
                }
                else
                {
                    inputString[i] = '#';
                }
                lastIndex = i;
            }
        }
    }
    TodoUtil::replaceAll(inputString, "#", "\n");
    return inputString;
}

std::string LRDividedTypeQuestionScene::makeWorkPath() const
{
    stringstream ss;
    ss << "/" << "LRComprehension";
    ss << "/" << _comprehensionScene->getBookName();
    ss << "/" << "multiplechoices";
    ss << "-" << _comprehensionScene->getCurrentProblem();
    return ss.str();
}

END_NS_LR_DIVIDED_TYPE_QUESTION;
