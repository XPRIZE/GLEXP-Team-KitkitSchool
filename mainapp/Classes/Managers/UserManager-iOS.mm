//
//  UserManager-iOS.mm
//  todomath
//
//
//

#import "UserManager.hpp"
#import <Foundation/Foundation.h>




const std::string UserManager::getAppVersion()
{
    return [[NSString stringWithFormat:@"%@", [[[NSBundle mainBundle] infoDictionary] objectForKey:@"CFBundleShortVersionString"]] UTF8String];
}
