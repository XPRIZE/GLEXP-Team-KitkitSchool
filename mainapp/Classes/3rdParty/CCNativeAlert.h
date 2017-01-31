//
//  CCNativeAlert.h
//
//  Created by Justin Hawkwood on 2015/02/24.
//
//

#ifndef __COCOS__NativeAlert__
#define __COCOS__NativeAlert__

#include "cocos2d.h"

NS_CC_BEGIN

class CC_DLL NativeAlert
{
public:
	enum class ButtonType
	{
		CANCEL = -1,
		OTHER = 0,
		RETURN
	};

	static void show( std::string title, std::string message, std::string cancelButtonTitle );

	static void showWithCallback( std::string title, std::string message, std::string cancelButtonTitle, std::string returnButtonTitle, std::string otherButtonTitle, int tag, const std::function<void( int tag, ButtonType type)> &callback );

	static void alertDidDismiss( std::string alertID, int buttonIndex);
};

NS_CC_END

#endif /* defined(__COCOS__NativeAlert__) */
