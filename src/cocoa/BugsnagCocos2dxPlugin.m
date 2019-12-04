#import <BugsnagStatic/Bugsnag.h>
#import <BugsnagStatic/BugsnagNotifier.h>
#import <BugsnagStatic/BugsnagPlugin.h>

#import "BugsnagCocos2dxPlugin.h"

NSString *const BUGSNAG_COCOS2DX_VERSION = @"0.1.0";

@interface Bugsnag ()
+ (BugsnagNotifier *)notifier;
+ (void)registerPlugin:(id<BugsnagPlugin>)plugin;
@end

@interface BugsnagCocos2dxPlugin () <BugsnagPlugin>
@property(nonatomic, getter=isStarted) BOOL started;
@property(nonatomic) NSString *cocos2dVersion;
@end

@implementation BugsnagCocos2dxPlugin

+ (void)registerWithCocos2dVersion:(const char *)rawVersion {
  BugsnagCocos2dxPlugin *plugin = [BugsnagCocos2dxPlugin new];
  plugin.cocos2dVersion = [self parseCocos2dVersion:rawVersion];
  [Bugsnag registerPlugin:plugin];
}

+ (NSString *)parseCocos2dVersion:(const char *)rawVersion {
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

- (void)start {
  [self setNotifierDetails];
  [self addPayloadModifiers];

  self.started = YES;
}

/** Set the plugin's notifier info over the default */
- (void)setNotifierDetails {
  id notifier = [Bugsnag notifier];
  NSDictionary *newDetails = @{
    @"version" : BUGSNAG_COCOS2DX_VERSION,
    @"name" : @"Bugsnag Cocos2d-x",
    @"url" : @"https://github.com/bugsnag/bugsnag-cocos2dx"
  };
  [notifier setValue:newDetails forKey:@"details"];
}

/** Add runtime version to the payload */
- (void)addPayloadModifiers {
  BugsnagConfiguration *config = [[Bugsnag notifier] configuration];
  [config addBeforeSendBlock:^bool(NSDictionary *_Nonnull rawEventData,
                                   BugsnagCrashReport *_Nonnull report) {
    if (self.cocos2dVersion != nil) {
      report.device = [self addDeviceRuntimeVersion:report.device
                                     cocos2dVersion:self.cocos2dVersion];
    }

    return YES;
  }];
  [config addBeforeSendSession:^void(NSMutableDictionary *_Nonnull data) {
    if (self.cocos2dVersion != nil) {
      data[@"device"] = [self addDeviceRuntimeVersion:data[@"device"]
                                       cocos2dVersion:self.cocos2dVersion];
    }
  }];
}

- (NSDictionary *)addDeviceRuntimeVersion:(NSDictionary *)device
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

@end
