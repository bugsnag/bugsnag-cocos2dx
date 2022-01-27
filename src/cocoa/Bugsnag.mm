#include "Bugsnag.hpp"

#include <Bugsnag/Bugsnag.h>

namespace bugsnag {

BSGBreadcrumbType _getBreadcrumbType(BreadcrumbType type) {
    switch (type) {
        case Request:
            return BSGBreadcrumbTypeRequest;
        case Navigation:
            return BSGBreadcrumbTypeNavigation;
        case Log:
            return BSGBreadcrumbTypeLog;
        case User:
            return BSGBreadcrumbTypeUser;
        case State:
            return BSGBreadcrumbTypeState;
        case Process:
            return BSGBreadcrumbTypeProcess;
        case Error:
            return BSGBreadcrumbTypeError;
        case Manual:
        default:
            return BSGBreadcrumbTypeManual;
    }
}

void Bugsnag::notify(std::string name, std::string message) {
    NSString *errorClass = [NSString stringWithCString:name.c_str() encoding:NSUTF8StringEncoding];
    NSString *errorMessage = [NSString stringWithCString:message.c_str() encoding:NSUTF8StringEncoding];
    NSException *exception = [NSException exceptionWithName:errorClass reason:errorMessage userInfo:nil];
    [NSClassFromString(@"Bugsnag") notify:exception];
}

void Bugsnag::setUser(const char *ID, const char *name, const char *email) {
    NSString *ns_ID = ID != nullptr ? [NSString stringWithCString:ID encoding:NSUTF8StringEncoding] : nil;
    NSString *ns_name = name != nullptr ? [NSString stringWithCString:name encoding:NSUTF8StringEncoding] : nil;
    NSString *ns_email = email != nullptr ? [NSString stringWithCString:email encoding:NSUTF8StringEncoding] : nil;
    [NSClassFromString(@"Bugsnag") setUser:ns_ID withEmail:ns_email andName:ns_name];
}

void Bugsnag::startSession() {
    [NSClassFromString(@"Bugsnag") startSession];
}

void Bugsnag::pauseSession() {
    [NSClassFromString(@"Bugsnag") pauseSession];
}

void Bugsnag::resumeSession() {
    [NSClassFromString(@"Bugsnag") resumeSession];
}

void Bugsnag::leaveBreadcrumb(string name, BreadcrumbType type, map<string, string> metadata) {
    NSMutableDictionary *ns_metadata = [NSMutableDictionary new];
    BSGBreadcrumbType ns_type = _getBreadcrumbType(type);
    NSString *ns_name = [NSString stringWithCString:name.c_str() encoding:NSUTF8StringEncoding];

    for (pair<string, string> item : metadata) {
        NSString *key = [NSString stringWithCString:item.first.c_str() encoding:NSUTF8StringEncoding];
        NSString *obj = [NSString stringWithCString:item.second.c_str() encoding:NSUTF8StringEncoding];
        [ns_metadata setObject:obj forKey:key];
    }
    [NSClassFromString(@"Bugsnag") leaveBreadcrumbWithMessage:ns_name metadata:ns_metadata andType:ns_type];
}

void Bugsnag::addMetadata(string section, string key, string value) {
    NSString *ns_section = [NSString stringWithCString:section.c_str() encoding:NSUTF8StringEncoding];
    NSString *ns_key = [NSString stringWithCString:key.c_str() encoding:NSUTF8StringEncoding];
    NSString *ns_value = [NSString stringWithCString:value.c_str() encoding:NSUTF8StringEncoding];
    [NSClassFromString(@"Bugsnag") addMetadata:ns_value withKey:ns_key toSection:ns_section];
}

void Bugsnag::clearMetadata(string section, string key) {
    NSString *ns_section = [NSString stringWithCString:section.c_str() encoding:NSUTF8StringEncoding];
    NSString *ns_key = [NSString stringWithCString:key.c_str() encoding:NSUTF8StringEncoding];
    [NSClassFromString(@"Bugsnag") clearMetadataFromSection:ns_section withKey:ns_key];
}

void Bugsnag::clearMetadata(string section) {
    NSString *ns_section = [NSString stringWithCString:section.c_str() encoding:NSUTF8StringEncoding];
    [NSClassFromString(@"Bugsnag") clearMetadataFromSection:ns_section];
}

void Bugsnag::setContext(string context) {
    NSString *ns_context = [NSString stringWithCString:context.c_str() encoding:NSUTF8StringEncoding];
    [NSClassFromString(@"Bugsnag") setContext:ns_context];
}

}
