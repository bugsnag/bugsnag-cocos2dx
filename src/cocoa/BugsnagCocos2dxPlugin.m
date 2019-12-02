#import <BugsnagStatic/Bugsnag.h>

#import "BugsnagCocos2dxPlugin.h"

@interface Bugsnag ()
+ (id)notifier;
+ (BOOL)bugsnagStarted;
@end

NSString *const BUGSNAG_COCOS2DX_VERSION = @"0.1.0";

@implementation BugsnagCocos2dxPlugin

+ (void)startBugsnagWithApiKey:(NSString *)apiKey cocos2dVersion:(const char *)cocos2dVersion {
    BugsnagConfiguration *config = [BugsnagConfiguration new];
    config.apiKey = apiKey;
    [self startBugsnagWithConfiguration:config cocos2dVersion:cocos2dVersion];
}

+ (void)startBugsnagWithConfiguration:(BugsnagConfiguration *)configuration cocos2dVersion:(const char *)raw {
    [self setNotifierDetails];
    NSString *cocos2dVersion = raw == NULL ? nil : [NSString stringWithCString:raw encoding:NSUTF8StringEncoding];
    cocos2dVersion = [[cocos2dVersion componentsSeparatedByCharactersInSet:[NSCharacterSet characterSetWithCharactersInString:@"- "]] lastObject];
    [configuration addBeforeSendBlock:^bool(NSDictionary *_Nonnull rawEventData, BugsnagCrashReport *_Nonnull report) {
        if (cocos2dVersion != nil) {
            report.device = [self addDeviceRuntimeVersion:report.device cocos2dVersion:cocos2dVersion];
        }

        return YES;
    }];
    [Bugsnag startBugsnagWithConfiguration:configuration];
    [self setNotifierDetails];
}

+ (NSDictionary *)addDeviceRuntimeVersion:(NSDictionary *)device
                           cocos2dVersion:(NSString *)cocos2dVersion {
    NSMutableDictionary *copy = [device mutableCopy];
    NSMutableDictionary *runtimeVersions = [copy[@"runtimeVersions"] mutableCopy];

    if (runtimeVersions == nil) {
        runtimeVersions = [NSMutableDictionary new];
    }
    runtimeVersions[@"cocos2dx"] = cocos2dVersion;
    copy[@"runtimeVersions"] = runtimeVersions;
    return copy;
}


+ (void)setNotifierDetails {
    if (![Bugsnag bugsnagStarted]) {
        return;
    }
    id notifier = [Bugsnag notifier];
    NSDictionary *newDetails = @{
        @"version": BUGSNAG_COCOS2DX_VERSION,
        @"name": @"Bugsnag Cocos2d-x",
        @"url": @"https://github.com/bugsnag/bugsnag-cocos2dx"
    };
    [notifier setValue:newDetails forKey:@"details"];
}

@end
