//
//  MultipleChoicesScene.cpp
//  enumaXprize
//
//  Created by timewalker on 26/12/2016.
//
//

#include "MultipleChoicesScene.hpp"
#include "TextAndImageLayer.hpp"
#include "TextAndTextLayer.hpp"
#include "ImageAndTextLayer.hpp"
#include "ImageAndImageLayer.hpp"
#include "Utils/TodoUtil.h"
#include "Managers/LanguageManager.hpp"
#include "3rdParty/CCNativeAlert.h"

namespace MultipleChoicesSceneSpace
{
//    const bool bDebug = false;
    const char* defaultFont = "fonts/TodoSchoolV2.ttf";
}

using namespace MultipleChoicesSceneSpace;

namespace ComprehensionTest
{
    namespace MultipleChoices
    {
        MultipleChoicesScene::MultipleChoicesScene():
        _currentType(QuestionType::None)
        {
            
        }
        
        bool MultipleChoicesScene::init()
        {
            if (!Layer::init()) return false;
            
            _gameNode = Node::create();
            _gameNode->setContentSize(fixedDeviceSize);
            _gameNode->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
            _gameNode->setPosition(getContentSize().width / 2, 0.f);
            addChild(_gameNode);
            
            return true;
        }
        
        Layer* MultipleChoicesScene::createLayer(ComprehensionScene *parent)
        {
            auto layer = MultipleChoicesScene::create();
            layer->_parent = parent;
            layer->_comprehensionScene = parent;
            return layer;
        }
        
        void MultipleChoicesScene::onEnter()
        {
            Layer::onEnter();
            
            initData();
            determineItemType();
            
            string directionContent = LanguageManager::getInstance()->isEnglish() ? "Select the correct answer." : "Chagua jibu sahihi.";
            
            if (_questionText.find("Choose:") != std::string::npos)
            {
                directionContent = "Select the picture to match the text";
                TodoUtil::replaceAll(_questionText, "Choose:", "");
                _questionText = TodoUtil::trim(_questionText);
                _questionTextFontSize = 80.f;
            }
            else
            {
                _questionTextFontSize = 65.f;
            }
            
            _comprehensionScene->drawQuestionTitle(directionContent, _gameNode);
            
            createFixedResources();
            
            switch (_currentType) {
                case QuestionType::TextQuestionTextAnswer:
                {
                    auto layer= TextAndTextLayer::create();
                    layer->parentScene = this;
                    layer->setAnswers(_answers);
                    _gameNode->addChild(layer);
                    break;
                }
                case QuestionType::TextQuestionImageAnswer:
                {
                    auto layer= TextAndImageLayer::create();
                    layer->parentScene = this;
                    layer->setAnswers(_answers);
                    _gameNode->addChild(layer);
                    break;
                }
                case QuestionType::ImageQuestionTextAnswer:
                {
                    auto layer= ImageAndTextLayer::create();
                    layer->parentScene = this;
                    layer->setQuestionImage(_questionImagePath);
                    layer->setAnswers(_answers);
                    _gameNode->addChild(layer);
                    break;
                }
                case QuestionType::ImageQuestionImageAnswer:
                {
                    auto layer= ImageAndImageLayer::create();
                    layer->parentScene = this;
                    layer->setQuestionImage(_questionImagePath);
                    layer->setAnswers(_answers);
                    _gameNode->addChild(layer);
                    break;
                }
                default:
                    break;
            }
        }
        
        void MultipleChoicesScene::determineItemType()
        {
            if (_questionImagePath == "")
            {
                if (_answers.size()<=0) {
                    NativeAlert::show("Error in MulpleChoice data", "answers empty", "OK");
                    return;
                }
                _currentType = _answers[0].find(".png") != std::string::npos ? QuestionType::TextQuestionImageAnswer : QuestionType::TextQuestionTextAnswer;
            }
            else
            {
                if (_answers.size()<=0) {
                    NativeAlert::show("Error in MulpleChoice data", "answers empty", "OK");
                    return;
                }
                _currentType = _answers[0].find(".png") != std::string::npos ? QuestionType::ImageQuestionImageAnswer : QuestionType::ImageQuestionTextAnswer;
            }
        }
        
        void MultipleChoicesScene::initData()
        {
            answerPairVector.clear();
            
            std::vector<std::string> rawData;
            std::vector<std::string> multipleChoiceData;
            
            
            for (int i = 0; i < _comprehensionScene->problemSet.size(); i++)
            {
                if (i == _comprehensionScene->getCurrentProblem())
                {
                    rawData = _comprehensionScene->problemSet[i].second;
                    break;
                }
            }
            
            if (rawData.size()<=4) {
             
                NativeAlert::show("Wrong data in Comp - MultipleChoice", "missing data column", "OK");
                return;
            }
            _questionText = rawData[1];
            _questionImagePath = rawData[2];
            
            _answers.clear();
            answerPairVector.clear();
//            for (auto splitAnswer : TodoUtil::split(rawData[3], ','))
            for (auto splitAnswer : TodoUtil::splitCSV(rawData[3]))
            {
                _answers.push_back(TodoUtil::trim(splitAnswer));
                answerPairVector.push_back(std::pair<std::string, bool>(TodoUtil::trim(splitAnswer), false));
            }
            _solutions.clear();
//            for (auto splitSolution : TodoUtil::split(rawData[4], ',')) { _solutions.push_back(TodoUtil::trim(splitSolution)); }
            for (auto splitSolution : TodoUtil::splitCSV(rawData[4])) { _solutions.push_back(TodoUtil::trim(splitSolution)); }
        }
        
        void MultipleChoicesScene::createFixedResources()
        {
            
            Size labelSize = Size(1800, 500);
//            auto questionLabel = TodoUtil::createLabelMultiline(_questionText, _questionTextFontSize, labelSize, "fonts/Aileron-Regular.otf", Color4B(56, 56, 56, 255));
            
            
            auto questionLabel = createMultiLine(_questionText, _questionTextFontSize, labelSize, Color4B(56, 56, 56, 255));
//            questionLabel->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
//            questionLabel->setPosition(300.f, _gameNode->getContentSize().height - 600.f);
            questionLabel->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
            questionLabel->setPosition(_gameNode->getContentSize().width / 2, _gameNode->getContentSize().height - 450.f);
            _gameNode->addChild(questionLabel);
        }
        
        bool MultipleChoicesScene::isSolved()
        {
            return _solutions.size() == getSolvedCount();
        }
        
        void MultipleChoicesScene::onSolve()
        {
            if (_comprehensionScene)
                _comprehensionScene->onSolve();
        }
        
        bool MultipleChoicesScene::isCorrect(std::string answer)
        {
            for (auto solution : _solutions)
            {
                if (solution == answer)
                {
                    return true;
                    break;
                }
            }
            return false;
        }
        
        void MultipleChoicesScene::solve(std::string answer)
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
        
        int MultipleChoicesScene::getSolvedCount()
        {
            int totalCount = 0;
            for (auto answerPair : answerPairVector)
            {
                if (answerPair.second == true) totalCount++;
            }
            return totalCount;
        }
        
        Node* MultipleChoicesScene::createMultiLine(string text, float fontSize, Size dim, Color4B fontColor)
        {
            vector<string> words = TodoUtil::split(text, ' ');
            
            bool complete = false;
            Node *page;
            
            int trial = 5;
            
            
            do {
                
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
                
                for (auto word : words) {
                    
                    auto slot = createWord(word, fontSize, fontColor);
                    auto slotSize = slot->getContentSize();
                    slot->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
                    if (pageWidth>0 && pos.x + slotSize.width > pageWidth) {
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
                if (!complete) {
                    auto scale = sqrt(dim.height / pageSize.height)* 0.9;
                    fontSize *= scale;
                }
                
                trial--;
                
                
                
            } while (!complete && trial>0);
            
            
            return page;
        }

        Node* MultipleChoicesScene::createWord(string word, float fontSize, Color4B fontColor)
        {
            auto underlineSize = fontSize*0.5;
            auto wordHeight = fontSize;
            
            Node *node = Node::create();
            Size wordSize = Size(0, wordHeight);
            
            while (word.length()>0) {
                if (word[0]=='_')
                {
                    
                    int underlineLength;
                    
                    size_t last = word.find_first_not_of("_");
                    if (last==string::npos) {
                        underlineLength = word.length();
                        word = "";
                        
                    } else {
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
                
                
                if (word.length()>0) {
                    
                    string currentSyl;
                    
                    size_t nextUnder = word.find_first_of("_");
                    if (nextUnder!=string::npos) {
                        currentSyl = word.substr(0, nextUnder);
                        word = word.substr(nextUnder, word.length()-nextUnder);
                        
                    } else {
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
        
    }
}
