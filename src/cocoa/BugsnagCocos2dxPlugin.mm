#import "BugsnagCocos2dxPlugin.h"
#import "BugsnagClient+Private.h"
#import "BugsnagNotifier.h"

static NSString *const BUGSNAG_COCOS2DX_VERSION = @"2.0.1";

namespace cocos2d {
    extern const char* cocos2dVersion();
}

@implementation BugsnagCocos2dxPlugin

+ (NSString *)parseCocos2dVersion:(nonnull const char *)rawVersion {
    // cocos2d::cocos2dVersion() is in the format "cocos2d-x-3.1.0" or
    // "cocos2d-x 3.17.0", so we make some small adjustments to correctly format
    // the value to a semantic version.
    NSString *ns_rawVersion =
        rawVersion == NULL ? nil
                           : [NSString stringWithCString:rawVersion
                                                encoding:NSUTF8StringEncoding];
    NSCharacterSet *separators =
        [NSCharacterSet characterSetWithCharactersInString:@"- "];
    return [[ns_rawVersion
        componentsSeparatedByCharactersInSet:separators] lastObject];
}

- (void)load:(nonnull BugsnagClient *)client {
    NSString *cocos2dVersion = [BugsnagCocos2dxPlugin parseCocos2dVersion:cocos2d::cocos2dVersion()];
    [client addRuntimeVersionInfo:cocos2dVersion withKey:@"cocos2dx"];
    client.notifier.version = BUGSNAG_COCOS2DX_VERSION;
    client.notifier.name = @"Bugsnag Cocos2d-x";
    client.notifier.url = @"https://github.com/bugsnag/bugsnag-cocos2dx";
    client.notifier.dependencies = [NSArray arrayWithObject:[[NSClassFromString(@"BugsnagNotifier") alloc] init]];
}

- (void)unload {
    // Nothing to do
}

@end
