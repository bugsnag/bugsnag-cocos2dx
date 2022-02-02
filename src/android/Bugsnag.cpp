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
static jmethodID BugsnagPauseSession;
static jmethodID BugsnagAddMetadata;
static jmethodID BugsnagClearMetadata;
static jmethodID BugsnagLeaveBreadcrumb;
static jmethodID BugsnagSetContext;
static jmethodID BugsnagSetUser;

static jclass BreadcrumbTypeClass;

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
 * Call a method on Bugsnag which returns void
 */
void _callBugsnagVoidMethod(jmethodID method);

/**
 * Call a method on Bugsnag which returns void and takes a single
 * string argument
 */
void _callBugsnagVoidStringMethod(jmethodID method, const char *value);

/**
 * Coerce breadcrumb type into JNI value
 */
jobject _getBreadcrumbType(JNIEnv *env, BreadcrumbType type);

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

  env->CallStaticVoidMethod(BugsnagClass, BugsnagSetUser, jid, jemail, jname);

  env->DeleteLocalRef(jid);
  env->DeleteLocalRef(jname);
  env->DeleteLocalRef(jemail);
}

void Bugsnag::startSession() {
  _callBugsnagVoidMethod(BugsnagStartSession);
}

void Bugsnag::pauseSession() {
  _callBugsnagVoidMethod(BugsnagPauseSession);
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
  jobject jtype = _getBreadcrumbType(env, type);
  env->CallStaticVoidMethod(BugsnagClass, BugsnagLeaveBreadcrumb, jname,
                            jmetadata, jtype);

  env->DeleteLocalRef(jname);
  env->DeleteLocalRef(jtype);
  env->DeleteLocalRef(jmetadata);
}

void _addMetadata(JNIEnv *env, string section, string key, void *jvalue) {
  jstring jsection = env->NewStringUTF(section.c_str());
  jstring jkey = env->NewStringUTF(key.c_str());

  env->CallStaticVoidMethod(BugsnagClass, BugsnagAddMetadata, jsection, jkey, jvalue);

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
  _callBugsnagVoidStringMethod(BugsnagClearMetadata, section.c_str());
}

void Bugsnag::setContext(string context) {
  _callBugsnagVoidStringMethod(BugsnagSetContext, context.c_str());
}

static jobject breadcrumbTypes[Manual+1];

jobject _getBreadcrumbType(JNIEnv *env, BreadcrumbType type) {
  switch (type) {
  case Request:
  case Navigation:
  case Log:
  case User:
  case State:
  case Process:
  case Error:
  case Manual:
    return breadcrumbTypes[type];
  default:
    return breadcrumbTypes[Manual];
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

void _callBugsnagVoidStringMethod(jmethodID method, const char *value) {
  JNIEnv *env = cocos2d::JniHelper::getEnv();
  if (env == nullptr) {
    return; // something has gone very wrong here.
  }

  jobject jvalue = value == NULL ? NULL : env->NewStringUTF(value);
  env->CallStaticVoidMethod(BugsnagClass, method, jvalue);
  env->DeleteLocalRef(jvalue);
}

static void _mapBreadcrumbType(JNIEnv *env, BreadcrumbType type, const char* javaName) {
  jfieldID field = env->GetStaticFieldID(BreadcrumbTypeClass, javaName, "Lcom/bugsnag/android/BreadcrumbType;");
  jobject obj = env->GetStaticObjectField(BreadcrumbTypeClass, field);
  bugsnag::breadcrumbTypes[type] = env->NewGlobalRef(obj);
}

} // namespace bugsnag

extern "C" {
JNIEXPORT void JNICALL Java_com_bugsnag_android_BugsnagCocos2dxPlugin_configureNativeComponents(
        JNIEnv *env, jobject _this) {
  if (bugsnag::HashMapClass != nullptr && bugsnag::BugsnagClass != nullptr) {
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

  bugsnag::BreadcrumbTypeClass = env->FindClass("com/bugsnag/android/BreadcrumbType");
  _mapBreadcrumbType(env, bugsnag::Navigation, "ERROR");
  _mapBreadcrumbType(env, bugsnag::Request, "REQUEST");
  _mapBreadcrumbType(env, bugsnag::Process, "PROCESS");
  _mapBreadcrumbType(env, bugsnag::Log, "LOG");
  _mapBreadcrumbType(env, bugsnag::User, "USER");
  _mapBreadcrumbType(env, bugsnag::State, "STATE");
  _mapBreadcrumbType(env, bugsnag::Error, "ERROR");
  _mapBreadcrumbType(env, bugsnag::Manual, "MANUAL");

  jclass bugsnag = env->FindClass("com/bugsnag/android/Bugsnag");
  bugsnag::BugsnagClass = (jclass)env->NewGlobalRef(bugsnag);
  env->DeleteLocalRef(bugsnag);
  bugsnag::BugsnagResumeSession = env->GetStaticMethodID(
      bugsnag::BugsnagClass, "resumeSession", "()Z");
  bugsnag::BugsnagStartSession = env->GetStaticMethodID(
      bugsnag::BugsnagClass, "startSession", "()V");
  bugsnag::BugsnagPauseSession = env->GetStaticMethodID(
      bugsnag::BugsnagClass, "pauseSession", "()V");
  bugsnag::BugsnagSetContext = env->GetStaticMethodID(
      bugsnag::BugsnagClass, "setContext",
      "(Ljava/lang/String;)V");
  bugsnag::BugsnagClearMetadata = env->GetStaticMethodID(
      bugsnag::BugsnagClass, "clearMetadata",
      "(Ljava/lang/String;)V");
  bugsnag::BugsnagSetUser = env->GetStaticMethodID(
      bugsnag::BugsnagClass, "setUser",
      "(Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;)V");
  bugsnag::BugsnagAddMetadata = env->GetStaticMethodID(
      bugsnag::BugsnagClass, "addMetadata",
      "(Ljava/lang/String;Ljava/lang/String;Ljava/lang/Object;)V");
  bugsnag::BugsnagLeaveBreadcrumb = env->GetStaticMethodID(
      bugsnag::BugsnagClass, "leaveBreadcrumb",
      "(Ljava/lang/String;Ljava/util/Map;Lcom/bugsnag/android/BreadcrumbType;)V");
}

JNIEXPORT jstring JNICALL Java_com_bugsnag_android_BugsnagCocos2dxPlugin_getCocos2dVersion(
        JNIEnv *env, jobject _this) {
  return env->NewStringUTF(cocos2d::cocos2dVersion());
}
}

#endif // Android-only
