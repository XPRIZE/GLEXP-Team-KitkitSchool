//
//  BookManager.hpp
//  KitkitSchool
//
//  Created by Gunho Lee on 11/3/16.
//
//

#ifndef BookManager_hpp
#define BookManager_hpp

#include <stdio.h>

#include <string>
#include <map>
#include <vector>

#include "LanguageManager.hpp"

using namespace std;

class BookInfo {
    
public:
    string bookTitle;
    LanguageManager::LocaleType bookLanguage;
    string bookPath;
    string bookImagePath;
};






class BookManager {
    
    map<string, BookInfo*> _bookMap;
    
public:
    
    static BookManager *getInstance();
    BookManager() { _bookMap.clear(); }
    

    void addBookInfo(BookInfo *info);
    void addBookInfo(string title, LanguageManager::LocaleType lang, string path, string imagePath = "");
    
    
    BookInfo* findBookInfo(string title);
    vector<BookInfo*> getBookVector(LanguageManager::LocaleType lang);

    
};



#endif /* BookManager_hpp */




