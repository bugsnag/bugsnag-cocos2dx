// This file is only loaded on iOS, tvOS, and macOS
#if TARGET_OS_IPHONE || TARGET_OS_TV || TARGET_OS_MAC

#include "Bugsnag.hpp"
#include <Bugsnag/Bugsnag.h>
#include <iostream>

using namespace bugsnag;

Configuration::Configuration(std::string apiKey) {
    this->apiKey = apiKey;
    this->autoDetectErrors = true;
    this->maxBreadcrumbs = 25;
    this->autoTrackSessions = true;
#if DEBUG
    this->autoDetectOOMs = true;
    this->releaseStage = "development";
#else
    this->releaseStage = "production";
#endif
}

void bugsnag::Bugsnag::init(Configuration *raw) {
    BugsnagConfiguration *config = [BugsnagConfiguration new];
    config.apiKey = [NSString stringWithCString:raw->apiKey.c_str()
                                       encoding:NSUTF8StringEncoding];
    config.autoNotify = raw->autoDetectErrors == true;
    config.reportOOMs = raw->autoDetectOOMs == true;
    config.breadcrumbs.capacity = raw->maxBreadcrumbs;
    config.shouldAutoCaptureSessions = raw->autoTrackSessions == true;
    // releaseStage
    // appVersion
    // enabledReleaseStages
    // endpoints

    // avoid class name ambiguity between c++ "Bugsnag" and objc "Bugsnag"
    Class Bugsnag = NSClassFromString(@"Bugsnag");
    [Bugsnag performSelector:@selector(startBugsnagWithConfiguration:)
                  withObject:config];
}

#endif // Cocoa-only
