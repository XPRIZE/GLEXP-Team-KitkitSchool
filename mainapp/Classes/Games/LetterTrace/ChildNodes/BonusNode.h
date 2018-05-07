//
//  BonusNode.h on Jul 1, 2016
//  TodoSchool
//
//  Copyright (c) 2016 Enuma, Inc. All rights reserved.
//  See LICENSE.md for more details.
//


#ifndef TODOSCHOOL_LETTERTRACE_BONUSNODE_H
#define TODOSCHOOL_LETTERTRACE_BONUSNODE_H

#include <Games/LetterTrace/Common/BonusVideoPlayer/BonusVideoPlayer.h>
#include <Common/ADT/Chain.h>
#include <cocos/cocos2d.h>
#include <cocos/ui/CocosGUI.h>
#include <functional>


namespace todoschool {
namespace lettertrace {

class BonusNode: public cocos2d::Node {

public:
    
    CREATE_FUNC(BonusNode);
    
    BonusVideoPlayer* BonusPlayer;
    cocos2d::Label* FullTextNode;
    cocos2d::Label* TraceTextNode;
    
    Chain<std::string> VideoFilePath;
    Chain<std::string> FullText;
    Chain<std::string> TraceText;
    
    std::function<void(BonusNode*)> OnBonusWorkDidEnd;

public:
    bool init() override;
    void setVisible(bool Visible) override;

    void clear();
    void startPlay();

private:
    void clearInternals();
    void updateTextNodes();
};

}  // namespace lettertrace
}  // namespace todoschool

#endif  // !defined(TODOSCHOOL_LETTERTRACE_BONUSNODE_H)
