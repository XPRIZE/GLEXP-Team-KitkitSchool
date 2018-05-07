//
//  BookManager.cpp
//  KitkitSchool
//
//  Created by Gunho Lee on 11/3/16.
//
//

#include "BookManager.hpp"


static BookManager* _instance;

BookManager* BookManager::getInstance()
{
    if (!_instance) {
        _instance = new BookManager();
    }
    return _instance;
    
    
}

void BookManager::addBookInfo(BookInfo *info)
{
    _bookMap[info->bookTitle] = info;
    
}



void BookManager::addBookInfo(string title, LanguageManager::LocaleType lang, string path, string imagePath)
{
    BookInfo *info = new BookInfo;
    info->bookTitle = title;
    info->bookLanguage = lang;
    if (path.back()!='/') path += "/";
    
    info->bookPath = path;
    info->bookImagePath = imagePath=="" ? path+"page/" : imagePath;
    
    addBookInfo(info);
}




BookInfo* BookManager::findBookInfo(string title)
{
    auto it = _bookMap.find(title);
    return (it==_bookMap.end()) ? nullptr : it->second;
}


vector<BookInfo*> BookManager::getBookVector(LanguageManager::LocaleType lang)
{
    vector<BookInfo*> ret;
    
    for (auto it : _bookMap) {
        if (it.second->bookLanguage == lang) {
            ret.push_back(it.second);
        }
    }
    
    return ret;
    
    
}
