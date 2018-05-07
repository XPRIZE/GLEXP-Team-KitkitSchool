//
//  EggQuizButton.hpp
//  KitkitSchool
//
//  Created by Gunho Lee on 1/8/17.
//
//

#ifndef EggQuizButton_hpp
#define EggQuizButton_hpp

#include <stdio.h>
#include <cocos2d.h>
#include "ui/CocosGUI.h"

#include <string>


USING_NS_CC;

class EggQuizButton : public ui::Button {
public:
    CREATE_FUNC(EggQuizButton);
    virtual bool init() override;
    
    
    void setNumber(int num);
    
    void setImage(std::string path);
    void setWord(std::string word, Size btnSize = Size(562, 446));
    void setSentence(std::string sentence, Size btnSize = Size(1948, 180));
    void setParagraph(std::string paragraph, Size btnSize = Size(918, 268));
    
    void setSound(std::string soundPath);
    
    float getLabelScale() { return (_label) ? _labelScale : 1.0; }
    void setLabelScale(float scale) { _labelScale = scale; if (_label) _label->setScale(scale); }
    
protected:
    float _labelScale;
    Label *_label;
        
    
    

    
    
    
};

#endif /* EggQuizAnswerButton_hpp */
