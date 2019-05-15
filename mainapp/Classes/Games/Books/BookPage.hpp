//
//  BookPage.hpp
//  booktest
//
//  Created by Gunho Lee on 12/27/16.
//
//

#ifndef BookPage_hpp
#define BookPage_hpp

#include <stdio.h>
#include "cocos2d.h"
#include <vector>
#include "ui/CocosGUI.h"
#include "TodoBook.hpp"


USING_NS_CC;



class BookPage : public Node
{
public:
    
    static BookPage* create();
    virtual bool init() override;
    virtual void onExit() override;
    virtual void update(float delta) override;
    void highlightWordButton(ui::Button *btn, bool highlight);
    void startReading();
    void stopReading();
    
    void setTitle(string title, string titleImagePath, string audioPath, TDBookLayout layout, float delay);
    void setPage(TodoBook *book, TodoPage *page, TDBookLayout layout, bool withAudio);
    void setCreditPage();
    void setPageNum(int page, Node *_pageView, Vec2 pos);
    
    
    
    void showRightHalf(bool animate);
    void showLeftHalf(bool animate);
    
    void hideRightHalf(bool animate);
    void hideLeftHalf(bool animate);
    
    
    
    
    
protected:
    

    bool isFirstPage() { return _page->pageNum == 1; }
    bool isLastPage() { return _page->lastPage; }
    bool isCreditPage() { return _page->creditPage; }
    bool isTitlePage() { return _isTitle; }

    void playWordSound(ui::Button *button, string path, float length);
    Node* createTextViewOneLine(Size size, float fontSize);
    Node* createTextViewMultiLine(Size size, float fontSize);
    
    
    Node *_leftView, *_rightView;
    Node *_leftCover, *_rightCover;
    
    Node *_contentsView;

    
    Node *_imageView;
    Node *_textView;



    
    std::vector<ui::Button*> _wordButtons;
    std::vector<TodoWord> _words;
    
    TodoBook *_book;
    TodoPage *_page;
    
    //string _resourceFolder;
    int _readingSentenceIndex;
    //int _readingAudioID; -> change as BGM
    
    bool _isTitle;
    string _titleAudioPath;
    
    
    bool _hasBackCover;
    TDBookLayout _bookLayout;
    bool _withAudio;
    float _timePage, _timeSentence;
    bool _isReading;
    
    bool _pauseReading;
    float _pauseLength;

    
    
};


#endif /* BookPage_hpp */
