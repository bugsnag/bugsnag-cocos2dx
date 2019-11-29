// This file is only loaded on Android
#if defined(ANDROID) || defined(__ANDROID__)

#include <BugsnagCocos2dx/Bugsnag.hpp>

#include <cocos/platform/android/jni/JniHelper.h>
#include <jni.h>

namespace bugsnag {

static const string NativeInterface = "com.bugsnag.android.NativeInterface";

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

void Bugsnag::notify(string name, string message) {
  // TODO
}

void Bugsnag::setUser(const char *id, const char *name, const char *email) {
  JNIEnv *env = cocos2d::JniHelper::getEnv();
  if (env == nullptr) {
    return; // something has gone very wrong here.
  }
  jclass interface = env->FindClass("com/bugsnag/android/NativeInterface");
  if (interface == nullptr) {
    return;
  }
  jmethodID setUser = env->GetStaticMethodID(
      interface, "setUser", 
      "(Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;)V");
  jobject jid = id == nullptr ? NULL : env->NewStringUTF(id);
  jobject jname = name == nullptr ? NULL : env->NewStringUTF(name);
  jobject jemail = email == nullptr ? NULL : env->NewStringUTF(email);

  env->CallStaticVoidMethod(interface, setUser, jid, jemail, jname);

  env->DeleteLocalRef(jid);
  env->DeleteLocalRef(jname);
  env->DeleteLocalRef(jemail);
}

void Bugsnag::startSession() {
  cocos2d::JniHelper::callStaticVoidMethod(NativeInterface, "startSession");
}

void Bugsnag::pauseSession() {
  cocos2d::JniHelper::callStaticVoidMethod(NativeInterface, "stopSession");
}

void Bugsnag::resumeSession() {
  cocos2d::JniHelper::callStaticBooleanMethod(NativeInterface, "resumeSession");
}

void Bugsnag::leaveBreadcrumb(string name, BreadcrumbType type,
                              map<string, string> metadata) {
  JNIEnv *env = cocos2d::JniHelper::getEnv();
  if (env == nullptr) {
    return; // something has gone very wrong here.
  }
  jclass HashMap = env->FindClass("java/util/HashMap");
  if (HashMap == nullptr) {
    return;
  }
  jclass interface = env->FindClass("com/bugsnag/android/NativeInterface");
  if (interface == nullptr) {
    return;
  }
  jmethodID leaveBreadcrumb = env->GetStaticMethodID(
      interface, "leaveBreadcrumb", 
      "(Ljava/lang/String;Ljava/lang/String;Ljava/util/Map;)V");

  jmethodID init = env->GetMethodID(HashMap, "<init>", "()V");
  jmethodID put = env->GetMethodID(
      HashMap, "put",
      "(Ljava/lang/Object;Ljava/lang/Object;)Ljava/lang/Object;");

  jobject jmetadata = env->NewObject(HashMap, init);
  for (pair<string, string> item : metadata) {
    jstring key = env->NewStringUTF(item.first.c_str());
    jstring value = env->NewStringUTF(item.second.c_str());
    env->CallObjectMethod(jmetadata, put, key, value);
    env->DeleteLocalRef(key);
    env->DeleteLocalRef(value);
  }

  jstring jname = env->NewStringUTF(name.c_str());
  jstring jtype = _getBreadcrumbType(env, type);
  env->CallStaticVoidMethod(interface, leaveBreadcrumb, jname, jtype, jmetadata);

  env->DeleteLocalRef(jname);
  env->DeleteLocalRef(jtype);
  env->DeleteLocalRef(jmetadata);
  env->DeleteLocalRef(HashMap);
  env->DeleteLocalRef(interface);
}

void _addMetadata(JNIEnv *env, string section, string key, void *jvalue) {
  jclass interface = env->FindClass("com/bugsnag/android/NativeInterface");
  if (interface == nullptr) {
    return;
  }
  jstring jsection = env->NewStringUTF(section.c_str());
  jstring jkey = env->NewStringUTF(key.c_str());
  jmethodID addToTab = env->GetStaticMethodID(
      interface, "addToTab", 
      "(Ljava/lang/String;Ljava/lang/String;Ljava/lang/Object;)V");

  env->CallStaticVoidMethod(interface, addToTab, jsection, jkey, jvalue);

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
  cocos2d::JniHelper::callStaticVoidMethod(NativeInterface, "clearTab",
                                           section);
}

void Bugsnag::setContext(string context) {
  cocos2d::JniHelper::callStaticVoidMethod(NativeInterface, "setContext",
                                           context);
}
} // namespace bugsnag

#endif // Android-only
