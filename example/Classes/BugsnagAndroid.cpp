// This file is only loaded on Android
#if defined(ANDROID) || defined(__ANDROID__)

#include "Bugsnag.hpp"

#include <cocos/platform/android/jni/JniHelper.h>
#include <jni.h>

using namespace bugsnag;

Configuration::Configuration(std::string apiKey) {
  this->apiKey = apiKey;
  this->autoDetectErrors = true;
  this->autoDetectANRs = true;
  this->autoDetectNDKCrashes = true;
  this->maxBreadcrumbs = 25;
  this->autoTrackSessions = true;
#if defined(NDEBUG)
  this->releaseStage = "development";
#else
  this->releaseStage = "production";
#endif
}

void Bugsnag::init(Configuration *config) {
  // TODO: load config values
  // TODO: cache reusable class and method refs
  JNIEnv *env = cocos2d::JniHelper::getEnv();
  jclass Bugsnag = (*env).FindClass("com/bugsnag/android/Bugsnag");
  jmethodID init =
      (*env).GetStaticMethodID(Bugsnag, "init",
                                "(Landroid/content/Context;Ljava/lang/"
                                "String;)Lcom/bugsnag/android/Client;");
  jstring apiKey = (*env).NewStringUTF(config->apiKey.c_str());
  jobject activity = cocos2d::JniHelper::getActivity();
  (*env).CallStaticObjectMethod(Bugsnag, init, activity, apiKey);
}

void Bugsnag::notify(std::string name, std::string message) {}

void Bugsnag::setUser(std::string id, std::string name, std::string email) {}

void Bugsnag::startSession() {}

void Bugsnag::pauseSession() {}

void Bugsnag::resumeSession() {}

void Bugsnag::leaveBreadcrumb(string name, BreadcrumbType type,
                              map<string, string> metadata) {}

void Bugsnag::addMetadata(string section, string key, void *value) {}

void Bugsnag::clearMetadata(string section, string key) {}

void Bugsnag::clearMetadata(string section) {}

void Bugsnag::setContext(string context) {}

#endif // Android-only
