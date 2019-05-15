//
//  CCNativeAlert_Android.cpp
//
//  Created by Justin Hawkwood on 2015/02/24.
//
//

#include "CCNativeAlert.h"

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
#include "platform/android/jni/JniHelper.h"
#include <jni.h>

#define  CLASS_NAME "org/cocos2dx/lib/Cocos2dxNativeAlert"

extern "C" {
	JNIEXPORT void JNICALL Java_org_cocos2dx_lib_Cocos2dxNativeAlert_alertDidDismiss(JNIEnv *env, jclass, jstring jid, jint jindex) {
		// LOGD("didFinishLoading");
		auto charID = env->GetStringUTFChars(jid, NULL);
		std::string alertID = charID;
		env->ReleaseStringUTFChars(jid, charID);
		// int index = jindex;
		
		cocos2d::NativeAlert::alertDidDismiss(alertID, jindex);
	}
}


NS_CC_BEGIN

static std::unordered_map<std::string, std::pair<int, std::function<void( int tag, cocos2d::NativeAlert::ButtonType type)>>> s_AlertViewIDs;

void NativeAlert::show( std::string title, std::string message, std::string cancelButtonTitle )
{
	NativeAlert::showWithCallback( title, message, cancelButtonTitle, "", "", 0, nullptr );
}

void NativeAlert::showWithCallback( std::string title, std::string message, std::string cancelButtonTitle, std::string returnButtonTitle, std::string otherButtonTitle, int tag, const std::function<void( int tag, ButtonType type)> &callback )
{
	JniMethodInfo t;
	std::string alertID("");
	
	if (! JniHelper::getStaticMethodInfo(t, CLASS_NAME, "showNativeAlert", "(Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;)Ljava/lang/String;"))
	{
		return ;
	}
	
	jstring jTitle = t.env->NewStringUTF( title.c_str() );
	jstring jMsg = t.env->NewStringUTF( message.c_str() );
	jstring jCanBtn = t.env->NewStringUTF( cancelButtonTitle.c_str() );
	jstring jRetBtn = t.env->NewStringUTF( returnButtonTitle.c_str() );
	jstring jOthBtn = t.env->NewStringUTF( otherButtonTitle.c_str() );
	jstring str = (jstring)t.env->CallStaticObjectMethod(t.classID, t.methodID, jTitle, jMsg, jCanBtn, jRetBtn, jOthBtn);
	t.env->DeleteLocalRef(t.classID);
	alertID = JniHelper::jstring2string(str);
	t.env->DeleteLocalRef(str);

	log( "nativeAlert showing %s", alertID.c_str() );
	
	if (callback) {
		std::pair<int, std::function<void( int tag, ButtonType type)>> tnf( tag, callback );
		s_AlertViewIDs[ alertID ] = tnf;
	}
}

void NativeAlert::alertDidDismiss( std::string alertID, int buttonIndex)
{
	std::pair<int, std::function<void( int tag, ButtonType type)>> tnf; // = s_AlertViewIDs.at( alertID );
	try {
		// myvector.at(20)=100;      // vector::at throws an out-of-range
		tnf = s_AlertViewIDs.at( alertID );
	}
	catch (const std::out_of_range& oor) {
		// std::cerr << "Out of Range error: " << oor.what() << '\n';
		log( "nativeAlert alertDidDismiss NO CALLBACK" );
		return;
	}
	
	ButtonType type = ButtonType::OTHER;
	if (buttonIndex == -1) type = ButtonType::RETURN;
	else if (buttonIndex == -2) type = ButtonType::CANCEL;
	
	log( "nativeAlert alertDidDismiss %i (%s) with %i", tnf.first, alertID.c_str(), type );
	
	// TODO: might have a threading problem
	tnf.second( tnf.first, type );
	
	s_AlertViewIDs.erase( alertID );
}

NS_CC_END

#else
NS_CC_BEGIN
void NativeAlert::show( std::string title, std::string message, std::string cancelButtonTitle )
{
    
}

void NativeAlert::showWithCallback( std::string title, std::string message, std::string cancelButtonTitle, std::string returnButtonTitle, std::string otherButtonTitle, int tag, const std::function<void( int tag, ButtonType type)> &callback )
{
    
}

void NativeAlert::alertDidDismiss( std::string alertID, int buttonIndex)
{
    
}
NS_CC_END
#endif
