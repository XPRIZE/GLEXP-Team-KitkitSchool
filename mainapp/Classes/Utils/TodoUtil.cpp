/*
 * TodoUtil.cpp
 *
 *  Created on: Jan 9, 2015
 *      Author: jooddang
 */

#include "TodoUtil.h"
#include <regex>


using namespace std;



vector<string> TodoUtil::split(const std::string &s, char delim) {
	vector<string> elems;
	string item;
	stringstream ss(s);
//	split(s, delim, elems);
	while (getline(ss, item, delim)) {
		elems.push_back(item);
	}
	return elems;
}


int TodoUtil::stoi(const std::string &s) {
    if (s.length() == 0) {
        return INT_MIN;
    }
	int i;
	istringstream buf(s);
	buf >> i;
	return i;
}

string TodoUtil::itos(const int& i) {
	ostringstream s;
	s << i;
	return s.str();
}



double TodoUtil::stod(const std::string &s) {
    if (s.length() == 0) {
        return 0;
    }
    double d;
    istringstream buf(s);
    buf >> d;
    return d;
    
}

std::string TodoUtil::dtos(const double& d) {
    ostringstream s;
    s << d;
    return s.str();
}




// http://stackoverflow.com/questions/6089231/getting-std-ifstream-to-handle-lf-cr-and-crlf
std::istream& TodoUtil::safegetline(std::istream& is, std::string& t)
{
    t.clear();

    // The characters in the stream are read one-by-one using a std::streambuf.
    // That is faster than reading them one-by-one using the std::istream.
    // Code that uses streambuf this way must be guarded by a sentry object.
    // The sentry object performs various tasks,
    // such as thread synchronization and updating the stream state.

    std::istream::sentry se(is, true);
    std::streambuf* sb = is.rdbuf();

    for (;;) {
        int c = sb->sbumpc();
        switch (c) {
        case '\n':
            return is;
        case '\r':
            if(sb->sgetc() == '\n')
                sb->sbumpc();
            return is;
        case EOF:
            // Also handle the case when the last line has no line ending
            if(t.empty())
                is.setstate(std::ios::eofbit);
            return is;
        default:
            t += (char)c;
        }
    }
}

std::vector<std::string> TodoUtil::splitCSV(std::string& line)
{
    std::vector<std::string> columns;
    
    bool escaping = false;
    int escapedTo = -100;
    bool escapingOne = false;
    
    string column = "";
    
    auto length = line.length();
    for (int i=0; i<length; i++) {
        char c = line[i];
        
        if (escapingOne) {
            escapingOne = false;
            column+=c;
            continue;
        }
        
        if (c=='\\') {
            escapingOne = true;
            continue;
        }
            
        if (c=='"') {
            if (escaping) {

                escaping = false;
                escapedTo = i;
            } else {
                if (escapedTo==i-1) {
                    column+="\"";
                   
                }
                escaping = true;
                
            }
        } else if (c==',' && !escaping) {
            columns.push_back(column);
            column="";
        } else {
            column+=c;
        }
    }
    
    if (column.length()>0) columns.push_back(column);
    
    //columns.push_back("");

    return columns;
    
    
}


std::vector<std::vector<std::string>> TodoUtil::readCSV(std::string& filedata)
{
    std::vector<std::vector<std::string>> ret;
    
    std::istringstream iss(filedata);
    std::string line;
    std::string word;
    
    while(iss.good())
    {
        safegetline(iss, line);
        if(line.length() == 0 && !iss.good())
        {
            break;
        }
        //CCLOG("csv line: %s", line.c_str());
        std::vector<std::string> columns;
        columns = splitCSV(line);
        ret.push_back(columns);
    }
    
    return ret;
}

std::vector<std::vector<std::string>> TodoUtil::readTSV(std::string& filedata)
{
    std::vector<std::vector<std::string>> ret;
    
    std::istringstream iss(filedata);
    std::string line;
    std::string word;
    
    while(iss.good())
    {
        safegetline(iss, line);
        if(line.length() == 0 && !iss.good())
        {
            break;
        }
        //CCLOG("csv line: %s", line.c_str());
        std::vector<std::string> columns;
        std::istringstream iss2(line);
        while(iss2.good())
        {
            std::getline(iss2, word, '\t');
            columns.push_back(word);
        }
        ret.push_back(columns);
    }
    
    return ret;
}

std::string TodoUtil::trim(std::string& str)
{
    size_t first = str.find_first_not_of(" \t");
    if (first == std::string::npos)
    {
        return std::string("");
    }
    size_t last = str.find_last_not_of(" \t");
    return str.substr(first, (last-first+1));
}

std::string TodoUtil::removeSpaces(std::string input) {
    input.erase(std::remove(input.begin(),input.end(),' '),input.end());
    return input;
}

std::string TodoUtil::returnStringOrNullString(std::string& str)
{
    return str.empty() ? "NULL" : str;
}

void TodoUtil::replaceAll(std::string& str, const std::string& from, const std::string& to)
{
    if(from.empty())
        return;
    size_t start_pos = 0;
    while((start_pos = str.find(from, start_pos)) != std::string::npos) {
        str.replace(start_pos, from.length(), to);
        start_pos += to.length(); // In case 'to' contains 'from', like replacing 'x' with 'yx'
    }
}

bool TodoUtil::replace(std::string& str, const std::string& from, const std::string& to) {
    size_t start_pos = str.find(from);
    if(start_pos == std::string::npos)
        return false;
    str.replace(start_pos, from.length(), to);
    return true;
}


Label* TodoUtil::createLabel(const std::string &text, float maxFontSize, Size boxSize,const std::string &fontName, const Color4B &color, TextHAlignment hAlignment /*= TextHAlignment::LEFT*/, TextVAlignment vAlignment/* = TextVAlignment::CENTER*/) {
    
    TTFConfig ttfConfig(fontName.c_str(),maxFontSize,GlyphCollection::DYNAMIC);
    Label *label = Label::createWithTTF(ttfConfig, text);
    label->setTextColor(color);
    label->setDimensions(boxSize.width, boxSize.height);
    label->setAlignment(hAlignment, vAlignment);
    
    return label;
}

Label* TodoUtil::createLabelMultiline(const std::string &text, float maxFontSize, cocos2d::Size boxSize, const std::string &fontName, const cocos2d::Color4B &color, TextHAlignment hAlignment/* = TextHAlignment::LEFT*/, TextVAlignment vAlignment/* = TextVAlignment::CENTER*/)
{
    Label* label;
    

    label = Label::createWithTTF(text, fontName, maxFontSize, Size(boxSize.width, 0), hAlignment, vAlignment);
    label->setTextColor(color);
    label->setVerticalAlignment(cocos2d::TextVAlignment::CENTER);
    
    return label;
}

Label* TodoUtil::createLabelMultilineToFit(const std::string &text, float maxFontSize, cocos2d::Size boxSize, const std::string &fontName, const cocos2d::Color4B &color, TextHAlignment hAlignment/* = TextHAlignment::LEFT*/, TextVAlignment vAlignment/* = TextVAlignment::CENTER*/)
{
    Label* label;
    
    
    label = Label::createWithTTF(text, fontName, maxFontSize, Size(boxSize.width, 0), hAlignment, cocos2d::TextVAlignment::CENTER);
    label->setTextColor(color);

    
    if (boxSize.height<1) return label;
    
    if (label->getContentSize().height > boxSize.height) {
        auto smallFontSize = maxFontSize * (boxSize.height /label->getContentSize().height);
        auto adjustedFontSize = (smallFontSize + maxFontSize)/2.0;
        
        label = Label::createWithTTF(text, fontName, adjustedFontSize, Size(boxSize.width, 0), hAlignment, cocos2d::TextVAlignment::CENTER);
        label->setTextColor(color);
        
        if (label->getContentSize().height > boxSize.height) {
            label = Label::createWithTTF(text, fontName, smallFontSize, Size(boxSize.width, 0), hAlignment, cocos2d::TextVAlignment::CENTER);
            label->setTextColor(color);
        }
        
        return label;
                                            
    }
    
    return label;
}
