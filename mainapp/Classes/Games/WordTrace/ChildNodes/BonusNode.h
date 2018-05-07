//
//  BonusNode.h on Jul 1, 2016
//  TodoSchool
//
//  Copyright (c) 2016 Enuma, Inc. All rights reserved.
//  See LICENSE.md for more details.
//


#pragma once

#include <Games/LetterTrace/Common/BonusVideoPlayer/BonusVideoPlayer.h>
#include <Common/ADT/Chain.h>
#include "Common/Basic/BigLabel.h"
#include <cocos/cocos2d.h>
#include <cocos/ui/CocosGUI.h>
#include <functional>


namespace todoschool {
namespace wordtrace {

class BonusNode: public cocos2d::Node {
    typedef cocos2d::Node Super;
    
public:
    BonusVideoPlayer* BonusPlayer;
    BigLabel* TextLabel;
    
    Chain<std::string> VideoFilePath;
    Chain<std::string> Text;
    
    std::function<void(BonusNode&)> OnBonusWorkDidFinish;
    
public:
    CREATE_FUNC(BonusNode);
    bool init() override;
    void setVisible(bool Visible) override;
    
    void clear();
    void startBonusWork();
    
private:
    void clearInternals();
    void refreshTextNodes();
};

}  // namespace wordtrace
}  // namespace todoschool
