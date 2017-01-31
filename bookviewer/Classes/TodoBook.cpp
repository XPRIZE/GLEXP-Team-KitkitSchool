//
//  TodoBook.cpp
//  booktest
//
//  Created by Gunho Lee on 6/29/16.
//
//

#include "TodoBook.hpp"
#include "Utils/TodoUtil.h"
#include "cocos2d.h"

TDBookType TodoBook::getBookType(string &filedata)
{
    auto data = TodoUtil::readCSV(filedata);
    
    for (auto row : data) {
        if (row.size() < 1) continue;
        string type = row[0];
        if (type=="title") {
            return (row[5] == "text") ? TDBookType::TextOnly : TDBookType::WithAudio;
        }
    }
    
    return TDBookType::Unknown;
}

void TodoBook::readData(string &filedata)
{
    auto data = TodoUtil::readCSV(filedata);
    
    fontSize = -1;
    
    for (auto row : data)
    {
        if (row.size() < 1) continue;
        string type = row[0];
        if (type.front()=='#') continue;
        
        
        if (type == "title")
        {
            bookTitle = row[1];
            titleAudioFilename = row[2];
            titleImageFilename = row[3];
            
            auto layout = row[4];
            if (layout == "portrait") bookLayout = TDBookLayout::Portrait;
            else if (layout == "portrait_traditional") bookLayout = TDBookLayout::Portrait_Traditional;
            else bookLayout = TDBookLayout::Landscape;
            bookType = (row[5] == "text") ? TDBookType::TextOnly : TDBookType::WithAudio;
            
            if (row.size()>6) fontSize = TodoUtil::stoi(row[6]);
            
        }
        else if (type == "page")
        {
            TodoPage page;
            page.pageNum = TodoUtil::stoi(row[1]);
            page.pageImageFilename = row[2];
            page.wordwrap = (row[3]=="wordwrap") ? true : false;
            page.creditPage = false;
            page.lastPage = false;
            page.fontSize = fontSize;
            pages.push_back(page);
            
        }
        else if (type == "paragraph")
        {
            TodoPage &currentPage = pages.back();
            
            TodoParagraph paragraph;
            currentPage.paragraphs.push_back(paragraph);
            
        }
        else if (type == "sentence")
        {
            TodoPage &currentPage = pages.back();
            TodoParagraph &currentParagraph = currentPage.paragraphs.back();
            TodoSentence sentence;
            
            if (bookType == TDBookType::TextOnly)
            {
                
                auto ws = TodoUtil::split(row[3], ' ');
                for (auto w : ws) {
                    TodoWord word;
                    word.word = w;
                    sentence.words.push_back(word);
                }
                
            }
            else
            {
                sentence.sentenceAudioFilename = row[1];
                sentence.startTimingInPage = TodoUtil::stod(row[2]);
            }
            
            currentParagraph.sentences.push_back(sentence);
        }
        else if (type == "word")
        {
            if (bookType == TDBookType::WithAudio)
            {
                TodoPage &currentPage = pages.back();
                TodoParagraph &currentParagraph = currentPage.paragraphs.back();
                TodoSentence &currentSentence = currentParagraph.sentences.back();
                
                TodoWord word;
                word.startTimingInSentence = TodoUtil::stod(row[1]);
                word.startTimingInPage = currentSentence.startTimingInPage + word.startTimingInSentence;
                word.endTimingInSentence = TodoUtil::stod(row[2]);
                word.endTimingInPage = currentSentence.startTimingInPage + word.endTimingInSentence;
                word.word = row[3];
                word.wordAudioFilename = row[4];
                word.wordAudioLength = 0;
                if (row.size()>5) {
                    word.wordAudioLength = TodoUtil::stod(row[5]);
                }
                
                if (word.wordAudioLength<0.001) {
                    word.wordAudioLength = word.endTimingInSentence - word.startTimingInSentence + 0.2;
                }
                
                currentSentence.words.push_back(word);
            }
        }
    }
}



void TodoBook::readCredit(string &filedata)
{
    
    TodoPage page;
    page.pageNum = pages.size();
    page.pageImageFilename = "";
    page.wordwrap = false;
    page.creditPage = true;
    
    TodoParagraph titleP, creditP, licenseP;
    
    auto makeSentence = [&](string& s){
        TodoSentence sentence;
        
        TodoWord word;
        word.word = s;
        
        sentence.words.push_back(word);
        return sentence;
        
    };
    
    titleP.sentences.push_back(makeSentence(this->bookTitle));
    
    std::istringstream iss(filedata);
    std::string line;
    
    bool isCredit = false;
    bool isLicense = false;
    
    
    while(iss.good())
    {
        TodoUtil::safegetline(iss, line);
        if(line.length() == 0)
        {
            if (!iss.good()) break;
            continue;
        }

        if (line.find("#credit")==0) {
            isCredit = true;
            isLicense = false;
            continue;
        }
        
        if (line.find("#license")==0) {
            isCredit = false;
            isLicense = true;
            continue;
        }
        
        if (isCredit) creditP.sentences.push_back(makeSentence(line));
        if (isLicense) licenseP.sentences.push_back(makeSentence(line));

    }
    
    page.paragraphs.push_back(titleP);
    page.paragraphs.push_back(creditP);
    page.paragraphs.push_back(licenseP);
    
    pages.push_back(page);
    
}


bool TodoBook::readFile(string prefix)
{
    string bookdataPath = prefix + "bookinfo.csv";
    string bookdata = cocos2d::FileUtils::getInstance()->getStringFromFile(bookdataPath);
    
    if (bookdata.length() == 0) return false;
    
    filePrefix = prefix;
    imagePrefix = prefix+pagePrefix;
    
    readData(bookdata);
    
    pages.back().lastPage = true;
    
    string creditPath = prefix + "credit.txt";
    string creditData = cocos2d::FileUtils::getInstance()->getStringFromFile(creditPath);
    readCredit(creditData);
    
    
    return true;
}

string TodoBook::checkData()
{
    string ret = "";
    
    auto titleImage = imagePrefix + titleImageFilename;
    if (!FileUtils::getInstance()->isFileExist(titleImage)) ret+= "missing title image ["+titleImageFilename+"]\n";
    
    auto titleAudio = imagePrefix + titleAudioFilename;
    if (!FileUtils::getInstance()->isFileExist(titleImage)) ret+= "missing title audio ["+titleAudioFilename+"]\n";
    
    for (auto p : pages) {
        auto pageImage = imagePrefix + p.pageImageFilename;
        if (!FileUtils::getInstance()->isFileExist(pageImage)) ret += "missing page image ["+p.pageImageFilename+"]\n";
        
        for (auto para : p.paragraphs) {
            for (auto s : para.sentences) {
                
                auto audioPath = filePrefix+"page/"+s.sentenceAudioFilename;
                if (!FileUtils::getInstance()->isFileExist(audioPath)) ret += "missing sentence audio ["+s.sentenceAudioFilename+"]\n";
                
                for (auto w : s.words) {
                    if (w.wordAudioFilename=="-") continue;
                    auto wordAudioPath = filePrefix+"word/"+w.wordAudioFilename;
                    if (!FileUtils::getInstance()->isFileExist(wordAudioPath)) ret += "missing word audio ["+w.wordAudioFilename+"]\n";
                    
                }
                
                
            }
        }
    }
    
    return ret;
    
}
