#import <Foundation/Foundation.h>

@interface BugsnagCocos2dxPlugin : NSObject

/**
 * Register the Bugsnag Cocos2d-x plugin, prior to calling [Bugsnag start]
 *
 * @param cocos2dVersion The version cocos2d being used, available via
 *                       cocos2d::cocos2dVersion()
 */
+ (void)registerWithCocos2dVersion:(const char *_Nonnull)cocos2dVersion;

@end
