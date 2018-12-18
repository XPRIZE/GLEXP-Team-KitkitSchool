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
#include "ui/CocosGUI.h"
#include "TodoBook.hpp"
#include "BookPage.hpp"


USING_NS_CC;


class ProgressIndicator;


class BookView : public Node
{
public:
    static BookView* create(const Size &size, std::string &bookPath);
    static BookView* create(const Size &size, std::string &bookPath, bool checkCompleteCondition);
    virtual bool init(const Size &size, std::string &bookPath, bool checkCompleteCondition);
    virtual void onExit() override;
    void onEnter() override;
    
    void setBook(TodoBook *book);
    TodoBook* getBook() { return _book; }
    
    void viewTitle(float delay = 0.0);
    void viewPage(int page);

    
    void nextPage();
    void previousPage();
    

    void showPageButton();
    void hidePageButton();
    
    static bool getReadAll() { return _isReadAll; }
    static void setLibraryMode(bool libraryMode) { _libraryMode = libraryMode; }
    static void setBookLanguage(std::string lang) { _languageCode = lang; }
    
    void popBookScene();
//    void setSoundButton(bool isSelect, ui::ImageView * imageButton, Label * textLabel);
//    void showSoundButton();
//    void hideSoundButton();
    
    bool getSoundSetting();
    void setSoundSetting(bool enable);
    static string getCurrentBook();
    static void setCurrentBook(string book);

protected:
    bool _checkCompleteCondition = false;
    
    //Label *_pageLabel;
    
    ProgressIndicator *_progressBar;

    Node *_bgView;
    Node *_contentsView;
    float _pageScale;
    BookPage *_currentPageView;
    TDBookLayout _bookLayout;
    
    //bool _isPortrait;
    
    
    ui::Button *_prevButton, *_nextButton;
    Vec2 _prevButtonPos, _nextButtonPos;
    bool _buttonEnabled;

    ui::Button *_soundBtn;
//    Sprite * _soundView;
//    ui::ImageView * _soundOnButton;
//    ui::ImageView * _soundOffButton;
//    Label * _soundOnLabel;
//    Label * _soundOffLabel;

    TodoBook *_book;
    int _currentPage;

    bool _finishReading;
    
    static bool _isReadAll;
    static bool _libraryMode;
    static std::string _languageCode;
    
    bool _soundSetting;

    //float _timePage, _timeSentence;
    
    
    //bool _pauseReading;
    //float _pauseLength;
    
    
    
};


#endif /* BookView_hpp */
