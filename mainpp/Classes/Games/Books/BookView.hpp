//
//  BookView.hpp
//  booktest
//
//  Created by Gunho Lee on 6/29/16.
//
//

#ifndef BookView_hpp
#define BookView_hpp

#include <stdio.h>
#include "cocos2d.h"
#include <vector>
#include "ui/cocosGUI.h"
#include "TodoBook.hpp"
#include "BookPage.hpp"


USING_NS_CC;


class ProgressIndicator;


class BookView : public Node
{
public:
    static BookView* create(const Size &size, std::string &bookPath);
    virtual bool init(const Size &size, std::string &bookPath);
    virtual void onExit() override;
    
    void setBook(TodoBook *book);
    TodoBook* getBook() { return _book; }
    
    void viewTitle(float delay = 0.0);
    void viewPage(int page);

    
    void nextPage();
    void previousPage();
    

    void showPageButton();
    void hidePageButton();
    
    static void setLibraryMode(bool libraryMode) { _libraryMode = libraryMode; }
    void popBookScene();
    
protected:
    

    
    //Label *_pageLabel;
    
    ProgressIndicator *_progressBar;

    Node *_bgView;
    Node *_contentsView;
    float _pageScale;
    BookPage *_currentPageView;
    TDBookLayout _bookLayout;
    
    bool _isPortrait;
    
    
    ui::Button *_prevButton, *_nextButton;
    Vec2 _prevButtonPos, _nextButtonPos;
    

    
    
    
    
    TodoBook *_book;
    int _currentPage;

    bool _finishReading;
    
    static bool _libraryMode;
    
    
    
    //float _timePage, _timeSentence;
    
    
    //bool _pauseReading;
    //float _pauseLength;
    
    
    
};


#endif /* BookView_hpp */
