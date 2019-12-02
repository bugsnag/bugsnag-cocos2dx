#import <Foundation/Foundation.h>

@class BugsnagConfiguration;

@interface BugsnagCocos2dxPlugin : NSObject

/** Start listening for crashes.
 *
 * This method initializes Bugsnag with the default configuration. Any uncaught
 * NSExceptions, C++ exceptions, mach exceptions or signals will be logged to
 * disk before your app crashes. The next time your app boots, we send any such
 * reports to Bugsnag.
 *
 * @param apiKey  The API key from your Bugsnag dashboard.
 */
+ (void)startBugsnagWithApiKey:(NSString *_Nonnull)apiKey
                cocos2dVersion:(const char *_Nonnull)cocos2dVersion;

/** Start listening for crashes.
 *
 * This method initializes Bugsnag. Any uncaught NSExceptions, uncaught
 * C++ exceptions, mach exceptions or signals will be logged to disk before
 * your app crashes. The next time your app boots, we send any such
 * reports to Bugsnag.
 *
 * @param configuration  The configuration to use.
 */
+ (void)startBugsnagWithConfiguration: (BugsnagConfiguration *_Nonnull)configuration
                       cocos2dVersion:(const char *_Nonnull)cocos2dVersion;

@end
