// This file is only loaded on Android
#if defined(ANDROID) || defined(__ANDROID__)

#include <BugsnagCocos2dx/Bugsnag.hpp>

#include <cocos/platform/android/jni/JniHelper.h>
#include <jni.h>

namespace bugsnag {
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
  // TODO: detect app version
}

void Bugsnag::init(Configuration *raw) {
  // TODO: load config values
  // TODO: cache reusable class and method refs
  JNIEnv *env = cocos2d::JniHelper::getEnv();
  if (env == nullptr) {
    return; // The environment could not be initialized
  }

  jclass Configuration = (*env).FindClass("com/bugsnag/android/Configuration");
  jclass Bugsnag = (*env).FindClass("com/bugsnag/android/Bugsnag");
  if (Configuration == 0 || Bugsnag == 0) {
    return; // The classes could not be loaded
  }
  jmethodID newConfig =
      (*env).GetMethodID(Configuration, "<init>", "(Ljava/lang/String;)V");
  jmethodID setReleaseStage = (*env).GetMethodID(
      Configuration, "setReleaseStage", "(Ljava/lang/String;)V");
  jmethodID setDetectNdkCrashes =
      (*env).GetMethodID(Configuration, "setDetectNdkCrashes", "(Z)V");
  jmethodID setEnableExceptionHandler =
      (*env).GetMethodID(Configuration, "setEnableExceptionHandler", "(Z)V");
  jmethodID setDetectAnrs =
      (*env).GetMethodID(Configuration, "setDetectAnrs", "(Z)V");
  jmethodID init =
      (*env).GetStaticMethodID(Bugsnag, "init",
                               "(Landroid/content/Context;Lcom/bugsnag/android/"
                               "Configuration;)Lcom/bugsnag/android/Client;");

  jstring apiKey = (*env).NewStringUTF(raw->apiKey.c_str());
  jstring releaseStage = (*env).NewStringUTF(raw->releaseStage.c_str());
  jobject config = (*env).NewObject(Configuration, newConfig, apiKey);
  (*env).CallVoidMethod(config, setDetectNdkCrashes,
                        raw->autoDetectNDKCrashes ? JNI_TRUE : JNI_FALSE);
  (*env).CallVoidMethod(config, setDetectAnrs,
                        raw->autoDetectANRs ? JNI_TRUE : JNI_FALSE);
  (*env).CallVoidMethod(config, setEnableExceptionHandler,
                        raw->autoDetectErrors ? JNI_TRUE : JNI_FALSE);
  (*env).CallVoidMethod(config, setReleaseStage, releaseStage);

  jobject activity = cocos2d::JniHelper::getActivity();
  (*env).CallStaticObjectMethod(Bugsnag, init, activity, config,
                                raw->autoDetectErrors);

  (*env).DeleteLocalRef(Configuration);
  (*env).DeleteLocalRef(Bugsnag);
  (*env).DeleteLocalRef(apiKey);
  (*env).DeleteLocalRef(releaseStage);
  (*env).DeleteLocalRef(config);
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
} // namespace bugsnag

#endif // Android-only
