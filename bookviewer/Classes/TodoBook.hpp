//
//  TodoBook.hpp
//  booktest
//
//  Created by Gunho Lee on 6/29/16.
//
//

#ifndef TodoBook_hpp
#define TodoBook_hpp

#include <stdio.h>
#include <string>
#include <vector>

using namespace std;

enum class TDBookLayout
{
    Portrait,
    Portrait_Traditional,
    Landscape,
};

enum class TDBookType
{
    Unknown = -1,
    WithAudio = 0,
    TextOnly = 1,
    
};

class TodoWord
{
public:
    string word;
    double startTimingInSentence, endTimingInSentence;
    double startTimingInPage, endTimingInPage;
    
    string wordAudioFilename;
    double wordAudioLength;
};


class TodoSentence
{
public:
    string sentenceAudioFilename;
    double startTimingInPage;
    vector<TodoWord> words;
};


class TodoParagraph
{
public:
    vector<TodoSentence> sentences;
};


class TodoPage
{
public:
    int pageNum;
    string pageImageFilename;
    vector<TodoParagraph> paragraphs;
    bool wordwrap;
    bool creditPage;
    bool lastPage;
    
    float fontSize;
    
};


class TodoBook
{
public:
    string bookTitle;
    string titleAudioFilename;
    string titleImageFilename;
    TDBookLayout bookLayout;
    TDBookType bookType;
    string credit;
    
    
    float fontSize;
    
    
    string filePrefix;
    string imagePrefix;
    
    const string pagePrefix = "page/";
    const string wordPrefix = "word/";
    
    vector<TodoPage> pages;
    
    TDBookType getBookType(string &filedata);
    
    void readData(string &data);
    void readCredit(string &data);
    
    bool readFile(string prefix);
    
    void addCreditPage();
    
    string checkData();
    
    
};
#endif /* TodoBook_hpp */
