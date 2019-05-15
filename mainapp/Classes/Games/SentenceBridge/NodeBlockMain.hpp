//
//  NodeBlockMain.hpp
//  KitkitSchool
//
//  Created by MilliSoftGames on 7/26/18.
//

#pragma once

#include "SentenceBridgeHeader.hpp"

BEGIN_NS_SENTENCEBRIDGE;

class NodeBlock;
class NodeBlockMain : public Node
{
public:
	CREATE_FUNC(NodeBlockMain);
	virtual bool init() override;
	void removeObjects();

	void createBlockList();
    
    void onStartStage(bool showUpperCase, bool hasAlphabet);
    void showCapslockBlinkAnimation(float fDelay);
    void turnoffCapslock();

	function<bool()> onBlockTouchBegan;
	function<void(string letter, Touch* touch)> onBlockTouchMoved;
	function<bool(NodeBlock* blockNode, Touch* touch)> onBlockTouchEnded;
    function<bool(NodeBlock* blockNode, Touch* touch)> onBlockTouchEnded_cancel;

private:
	void processBlockTouch(NodeBlock* blockNode);

	void makeCapslockButton();
	void toggleCapslock();

	string getKeyboardLayout();

public:
	vector<NodeBlock*> _blockNodes;

	Sprite* _capslockButton;
	bool _isCapslockOn;

private:

};

END_NS_SENTENCEBRIDGE;
