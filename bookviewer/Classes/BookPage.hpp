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
    void startReading();
    void stopReading();
    
    void setTitle(string title, string titleImagePath, string audioPath, TDBookLayout layout, float delay);
    void setPage(TodoPage *page, string folder, TDBookLayout layout, bool withAudio);
    void setCreditPage();
    
    
    
    void showRightHalf(bool animate);
    void showLeftHalf(bool animate);
    
    void hideRightHalf(bool animate);
    void hideLeftHalf(bool animate);
    
    
    
    
    
protected:
    

    bool isFirstPage() { return _page->pageNum == 1; }
    bool isLastPage() { return _page->lastPage; }
    bool isCreditPage() { return _page->creditPage; }
    bool isTitlePage() { return _isTitle; }
    
    Node* createTextViewOneLine(Size size, float fontSize);
    Node* createTextViewMultiLine(Size size, float fontSize);
    
    
    Node *_leftView, *_rightView;
    Node *_leftCover, *_rightCover;
    
    Node *_contentsView;

    
    Node *_imageView;
    Node *_textView;



    
    std::vector<ui::Button*> _wordButtons;
    std::vector<TodoWord> _words;
    
    TodoPage *_page;
    
    string _resourceFolder;
    int _readingSentenceIndex;
    int _readingAudioID;
    
    bool _isTitle;
    string _titleAudioPath;
    
    
    bool _hasBackCover;
    TDBookLayout _bookLayout;
    bool _withAudio;
    float _timePage, _timeSentence;
    
    bool _pauseReading;
    float _pauseLength;
    
    
    
};


#endif /* BookPage_hpp */
