// This file is only loaded on Android
#if defined(ANDROID) || defined(__ANDROID__)

#include <BugsnagCocos2dx/Bugsnag.hpp>

#include <cocos/platform/android/jni/JniHelper.h>
#include <cocos/cocos2d.h>
#include <jni.h>
#include <bugsnag.h>

namespace bugsnag {

/** Cache global JNI references for classes/methods used by the interface */
static jclass BugsnagClass;
static jmethodID BugsnagResumeSession;
static jmethodID BugsnagStartSession;
static jmethodID BugsnagStopSession;

static jclass NativeInterfaceClass;
static jmethodID NativeInterfaceAddToTab;
static jmethodID NativeInterfaceClearTab;
static jmethodID NativeInterfaceLeaveBreadcrumb;
static jmethodID NativeInterfaceSetContext;
static jmethodID NativeInterfaceSetUser;

static jclass HashMapClass;
static jmethodID HashMapInit;
static jmethodID HashMapPut;

/**
 * Call a method on Bugsnag which returns boolean
 */
void _callBugsnagBooleanVoidMethod(jmethodID method);

/**
 * Call a method on Bugsnag which returns void
 */
void _callBugsnagVoidMethod(jmethodID method);

/**
 * Call a method on NativeInterface which returns void
 */
void _callNativeInterfaceVoidMethod(jmethodID method);

/**
 * Call a method on NativeInterface which returns void and takes a single
 * string argument
 */
void _callNativeInterfaceVoidStringMethod(jmethodID method, const char *value);

/**
 * Coerce breadcrumb type into JNI value
 */
jstring _getBreadcrumbType(JNIEnv *env, BreadcrumbType type);

void Bugsnag::notify(string name, string message) {
  JNIEnv *env = cocos2d::JniHelper::getEnv();
  if (env == nullptr) {
    return; // something has gone very wrong here.
  }
  bugsnag_notify_env(env, (char *)name.c_str(), (char *)message.c_str(), BSG_SEVERITY_WARN);
}

void Bugsnag::setUser(const char *id, const char *name, const char *email) {
  JNIEnv *env = cocos2d::JniHelper::getEnv();
  if (env == nullptr) {
    return; // something has gone very wrong here.
  }
  jobject jid = id == nullptr ? NULL : env->NewStringUTF(id);
  jobject jname = name == nullptr ? NULL : env->NewStringUTF(name);
  jobject jemail = email == nullptr ? NULL : env->NewStringUTF(email);

  env->CallStaticVoidMethod(NativeInterfaceClass, NativeInterfaceSetUser, jid, jemail, jname);

  env->DeleteLocalRef(jid);
  env->DeleteLocalRef(jname);
  env->DeleteLocalRef(jemail);
}

void Bugsnag::startSession() {
  _callBugsnagVoidMethod(BugsnagStartSession);
}

void Bugsnag::pauseSession() {
  _callBugsnagVoidMethod(BugsnagStopSession);
}

void Bugsnag::resumeSession() {
  _callBugsnagBooleanVoidMethod(BugsnagResumeSession);
}

void Bugsnag::leaveBreadcrumb(string name, BreadcrumbType type,
                              map<string, string> metadata) {
  JNIEnv *env = cocos2d::JniHelper::getEnv();
  if (env == nullptr) {
    return; // something has gone very wrong here.
  }

  jobject jmetadata = env->NewObject(HashMapClass, HashMapInit);
  for (pair<string, string> item : metadata) {
    jstring key = env->NewStringUTF(item.first.c_str());
    jstring value = env->NewStringUTF(item.second.c_str());
    env->CallObjectMethod(jmetadata, HashMapPut, key, value);
    env->DeleteLocalRef(key);
    env->DeleteLocalRef(value);
  }

  jstring jname = env->NewStringUTF(name.c_str());
  jstring jtype = _getBreadcrumbType(env, type);
  env->CallStaticVoidMethod(NativeInterfaceClass, NativeInterfaceLeaveBreadcrumb, jname, 
                            jtype, jmetadata);

  env->DeleteLocalRef(jname);
  env->DeleteLocalRef(jtype);
  env->DeleteLocalRef(jmetadata);
}

void _addMetadata(JNIEnv *env, string section, string key, void *jvalue) {
  jstring jsection = env->NewStringUTF(section.c_str());
  jstring jkey = env->NewStringUTF(key.c_str());

  env->CallStaticVoidMethod(NativeInterfaceClass, NativeInterfaceAddToTab, jsection, jkey, jvalue);

  env->DeleteLocalRef(jsection);
  env->DeleteLocalRef(jkey);
}

void Bugsnag::addMetadata(string section, string key, string value) {
  JNIEnv *env = cocos2d::JniHelper::getEnv();
  if (env == nullptr) {
    return; // something has gone very wrong here.
  }
  jobject jvalue = env->NewStringUTF(value.c_str());
  _addMetadata(env, section, key, jvalue);
  env->DeleteLocalRef(jvalue);
}

void Bugsnag::clearMetadata(string section, string key) {
  JNIEnv *env = cocos2d::JniHelper::getEnv();
  if (env == nullptr) {
    return; // something has gone very wrong here.
  }
  _addMetadata(env, section, key, NULL);
}

void Bugsnag::clearMetadata(string section) {
  _callNativeInterfaceVoidStringMethod(NativeInterfaceClearTab, section.c_str());
}

void Bugsnag::setContext(string context) {
  _callNativeInterfaceVoidStringMethod(NativeInterfaceSetContext, context.c_str());
}

jstring _getBreadcrumbType(JNIEnv *env, BreadcrumbType type) {
  switch (type) {
  case Request:
    return env->NewStringUTF("request");
  case Navigation:
    return env->NewStringUTF("navigation");
  case Log:
    return env->NewStringUTF("log");
  case User:
    return env->NewStringUTF("user");
  case State:
    return env->NewStringUTF("state");
  case Process:
    return env->NewStringUTF("process");
  case Error:
    return env->NewStringUTF("error");
  case Manual:
  default:
    return env->NewStringUTF("manual");
  }
}

void _callBugsnagBooleanVoidMethod(jmethodID method) {
  JNIEnv *env = cocos2d::JniHelper::getEnv();
  if (env == nullptr) {
    return; // something has gone very wrong here.
  }

  env->CallStaticBooleanMethod(BugsnagClass, method);
}

void _callBugsnagVoidMethod(jmethodID method) {
  JNIEnv *env = cocos2d::JniHelper::getEnv();
  if (env == nullptr) {
    return; // something has gone very wrong here.
  }

  env->CallStaticVoidMethod(BugsnagClass, method);
}

void _callNativeInterfaceVoidMethod(jmethodID method) {
  JNIEnv *env = cocos2d::JniHelper::getEnv();
  if (env == nullptr) {
    return; // something has gone very wrong here.
  }

  env->CallStaticVoidMethod(NativeInterfaceClass, method);
}

void _callNativeInterfaceVoidStringMethod(jmethodID method, const char *value) {
  JNIEnv *env = cocos2d::JniHelper::getEnv();
  if (env == nullptr) {
    return; // something has gone very wrong here.
  }

  jobject jvalue = value == NULL ? NULL : env->NewStringUTF(value);
  env->CallStaticVoidMethod(NativeInterfaceClass, method, jvalue);
  env->DeleteLocalRef(jvalue);
}

} // namespace bugsnag

extern "C" {
JNIEXPORT void JNICALL Java_com_bugsnag_android_BugsnagCocos2dxPlugin_configureNativeComponents(
        JNIEnv *env, jobject _this) {
  if (bugsnag::HashMapClass != nullptr && bugsnag::BugsnagClass != nullptr && bugsnag::NativeInterfaceClass != nullptr) {
    return; // Initialization has already occurred
  }
  jclass HashMap = env->FindClass("java/util/HashMap");
  bugsnag::HashMapClass = (jclass)env->NewGlobalRef(HashMap);
  env->DeleteLocalRef(HashMap);

  bugsnag::HashMapInit = env->GetMethodID(
      bugsnag::HashMapClass, "<init>", "()V");
  bugsnag::HashMapPut = env->GetMethodID(
      bugsnag::HashMapClass, "put",
      "(Ljava/lang/Object;Ljava/lang/Object;)Ljava/lang/Object;");

  jclass bugsnag = env->FindClass("com/bugsnag/android/Bugsnag");
  bugsnag::BugsnagClass = (jclass)env->NewGlobalRef(bugsnag);
  env->DeleteLocalRef(bugsnag);
  bugsnag::BugsnagResumeSession = env->GetStaticMethodID(
      bugsnag::BugsnagClass, "resumeSession", "()Z");
  bugsnag::BugsnagStartSession = env->GetStaticMethodID(
      bugsnag::BugsnagClass, "startSession", "()V");
  bugsnag::BugsnagStopSession = env->GetStaticMethodID(
      bugsnag::BugsnagClass, "stopSession", "()V");

  jclass interface = env->FindClass("com/bugsnag/android/NativeInterface");
  bugsnag::NativeInterfaceClass = (jclass)env->NewGlobalRef(interface);
  env->DeleteLocalRef(interface);

  bugsnag::NativeInterfaceSetContext = env->GetStaticMethodID(
      bugsnag::NativeInterfaceClass, "setContext",
      "(Ljava/lang/String;)V");
  bugsnag::NativeInterfaceClearTab = env->GetStaticMethodID(
      bugsnag::NativeInterfaceClass, "clearTab",
      "(Ljava/lang/String;)V");
  bugsnag::NativeInterfaceSetUser = env->GetStaticMethodID(
      bugsnag::NativeInterfaceClass, "setUser",
      "(Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;)V");
  bugsnag::NativeInterfaceAddToTab = env->GetStaticMethodID(
      bugsnag::NativeInterfaceClass, "addToTab",
      "(Ljava/lang/String;Ljava/lang/String;Ljava/lang/Object;)V");
  bugsnag::NativeInterfaceLeaveBreadcrumb = env->GetStaticMethodID(
      bugsnag::NativeInterfaceClass, "leaveBreadcrumb",
      "(Ljava/lang/String;Ljava/lang/String;Ljava/util/Map;)V");
}

JNIEXPORT jstring JNICALL Java_com_bugsnag_android_BugsnagCocos2dxPlugin_getCocos2dVersion(
        JNIEnv *env, jobject _this) {
  return env->NewStringUTF(cocos2d::cocos2dVersion());
}
}

#endif // Android-only
