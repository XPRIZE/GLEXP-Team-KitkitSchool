//
//  TWApplication.cpp
//  Frankenstein
//
//  Created by Justin Hawkwood on 2014/10/03.
//
//

#include "CCNativeAlert.h"

@interface NativeAlertDelegate : NSObject <UIAlertViewDelegate>
@property (nonatomic) std::function<void(int tag, cocos2d::NativeAlert::ButtonType type)> didDismiss;
- (void)alertView:(UIAlertView *)alertView didDismissWithButtonIndex:(NSInteger)buttonIndex;  // after animation
@end

@implementation NativeAlertDelegate {
}
- (void)alertView:(UIAlertView *)alertView didDismissWithButtonIndex:(NSInteger)buttonIndex
{
	if (self.didDismiss) {
		cocos2d::NativeAlert::ButtonType type = cocos2d::NativeAlert::ButtonType::OTHER;
		if (buttonIndex == 1) type = cocos2d::NativeAlert::ButtonType::RETURN;
		else if (buttonIndex == alertView.cancelButtonIndex)
			type = cocos2d::NativeAlert::ButtonType::CANCEL;
		
		self.didDismiss(alertView.tag, type);
	}
	[self autorelease];
}

@end
NS_CC_BEGIN

void NativeAlert::show( std::string title, std::string message, std::string cancelButtonTitle )
{
	NativeAlert::showWithCallback( title, message, cancelButtonTitle, "", "", 0, nullptr );
}

void NativeAlert::showWithCallback( std::string title, std::string message, std::string cancelButtonTitle, std::string returnButtonTitle, std::string otherButtonTitle, int tag, const std::function<void( int tag, ButtonType type)> &callback )
{
	UIAlertView *alert = [[UIAlertView alloc] init];
	if (! title.empty() ) {
		NSString *nTitle = [NSString stringWithUTF8String: title.c_str() ];
		alert.title = nTitle;
	}
	if (! message.empty() ) {
	NSString *nMsg = [NSString stringWithUTF8String: message.c_str() ];
		alert.message = nMsg;
	}
	if (! cancelButtonTitle.empty() ) {
		NSString *nCBT = [NSString stringWithUTF8String: cancelButtonTitle.c_str() ];
		[alert addButtonWithTitle: nCBT];
		alert.cancelButtonIndex = 0;
	}

	if (! returnButtonTitle.empty() ) {
		NSString *nRBT = [NSString stringWithUTF8String: returnButtonTitle.c_str() ];
		[alert addButtonWithTitle: nRBT];
	}

	if (! otherButtonTitle.empty() ) {
		NSString *nOBT = [NSString stringWithUTF8String: otherButtonTitle.c_str() ];
		[alert addButtonWithTitle: nOBT];
	}
	
	
	/*
	for (auto it = otherButtonTitles.begin(); it != otherButtonTitles.end(); ++it) {
		std::string obt = *it;
		// if (obt.empty()) continue;
		NSString *nOBT = [NSString stringWithUTF8String: obt.c_str() ];
		[alert addButtonWithTitle: nOBT];
	}
	*/
	
	if (callback) {
		alert.tag = tag;
	
		NativeAlertDelegate *nald = [[NativeAlertDelegate alloc] init]; // not autoreleased yet
		nald.didDismiss = callback;
		alert.delegate = nald;
	}
	
	[alert show];
}

void NativeAlert::alertDidDismiss( std::string alertID, int buttonIndex)
{
	// This is an Android method only
}

NS_CC_END
