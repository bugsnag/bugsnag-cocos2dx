// This file is only loaded on Android
#if defined(ANDROID) || defined(__ANDROID__)

#include <BugsnagCocos2dx/Bugsnag.hpp>

#include <cocos/platform/android/jni/JniHelper.h>
#include <jni.h>

namespace bugsnag {

void _callNativeInterfaceVoidMethod(const char *name, const char* sig) {
  // TODO: cache reusable class and method refs
  JNIEnv *env = cocos2d::JniHelper::getEnv();
  if (env == nullptr) {
    return; // The environment could not be initialized
  }
  jclass NativeInterface = (*env).FindClass("com/bugsnag/android/NativeInterface");
  if (NativeInterface == 0) {
    return; // The classes could not be loaded
  }
  jmethodID method =
      (*env).GetStaticMethodID(NativeInterface, name, sig);
  if (method == 0) {
    return; // No matching method on the class
  }
  (*env).CallStaticVoidMethod(NativeInterface, method);
  (*env).DeleteLocalRef(NativeInterface);
}

void Bugsnag::notify(std::string name, std::string message) {
}

void Bugsnag::setUser(std::string id, std::string name, std::string email) {}

void Bugsnag::startSession() {
  _callNativeInterfaceMethod("startSession", "()V");
}

void Bugsnag::pauseSession() {
  _callNativeInterfaceMethod("stopSession", "()V");
}

void Bugsnag::resumeSession() {
  _callNativeInterfaceMethod("resumeSession", "()V");
}

void Bugsnag::leaveBreadcrumb(string name, BreadcrumbType type,
                              map<string, string> metadata) {}

void Bugsnag::addMetadata(string section, string key, void *value) {}

void Bugsnag::clearMetadata(string section, string key) {}

void Bugsnag::clearMetadata(string section) {}

void Bugsnag::setContext(string context) {}
} // namespace bugsnag

#endif // Android-only
