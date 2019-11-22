// This file is only loaded on iOS, tvOS, and macOS
#if TARGET_OS_IPHONE || TARGET_OS_TV || TARGET_OS_MAC

#include "Bugsnag.hpp"

#include <Bugsnag/Bugsnag.h>

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
    // TODO: releaseStage
    // TODO: appVersion
    // TODO: enabledReleaseStages
    // TODO: endpoints

    // avoid class name ambiguity between c++ "Bugsnag" and objc "Bugsnag"
    Class Bugsnag = NSClassFromString(@"Bugsnag");
    [Bugsnag performSelector:@selector(startBugsnagWithConfiguration:)
                  withObject:config];
}

void Bugsnag::notify(std::string name, std::string message) {
}

void Bugsnag::setUser(std::string id, std::string name, std::string email) {

}

void Bugsnag::startSession() {

}

void Bugsnag::pauseSession() {

}

void Bugsnag::resumeSession() {

}

void Bugsnag::leaveBreadcrumb(string name, BreadcrumbType type, map<string, string> metadata) {

}

void Bugsnag::addMetadata(string section, string key, void *value) {

}

void Bugsnag::clearMetadata(string section, string key) {

}

void Bugsnag::clearMetadata(string section) {

}

void Bugsnag::setContext(string context) {

}

#endif // Cocoa-only
