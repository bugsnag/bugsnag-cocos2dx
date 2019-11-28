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

void Bugsnag::setUser(string *id, string *name, string *email) {
  JNIEnv *env = JniHelper::getEnv();
  if (env == nullptr) {
    return; // something has gone very wrong here.
  }
  jobject jid = id == nullptr ? NULL : env->NewStringUTF(id);
  jobject jname = name == nullptr ? NULL : env->NewStringUTF(name);
  jobject jemail = email == nullptr ? NULL : env->NewStringUTF(email);

  JniHelper::callStaticVoidMethod(NativeInterface, "setUser", jid, jemail,
                                  jname);

  env->DeleteLocalRef(jid);
  env->DeleteLocalRef(jname);
  env->DeleteLocalRef(jemail);
}

void Bugsnag::startSession() {
  JniHelper::callStaticVoidMethod(NativeInterface, "startSession");
}

void Bugsnag::pauseSession() {
  JniHelper::callStaticVoidMethod(NativeInterface, "stopSession");
}

void Bugsnag::resumeSession() {
  JniHelper::callStaticBooleanMethod(NativeInterface, "resumeSession");
}

void Bugsnag::leaveBreadcrumb(string name, BreadcrumbType type,
                              map<string, string> metadata) {
  if (env == nullptr) {
    return; // something has gone very wrong here.
  }
  jclass HashMap = env->FindClass("java/util/HashMap");
  if (HashMap == nullptr) {
    return;
  }

  jmethodID init = env->GetMethodID(HashMap, "<init>", "()V");
  jobject jmetadata = env->NewObject(HashMap, init);

  jmethodID put = env->GetMethodID(
      HashMap, "put",
      "(Ljava/lang/Object;Ljava/lang/Object;)Ljava/lang/Object;");

  for (pair<string, string> item : metadata) {
    jstring key = env->NewStringUTF(item.first.c_str());
    jstring value = env->NewStringUTF(item.second.c_str());
    env->CallObjectMethod(jmetadata, put, key, value);
    env->DeleteLocalRef(key);
    env->DeleteLocalRef(value);
  }

  jstring jname = env->NewStringUTF(name);
  jstring jtype = _getBreadcrumbType(env, type);
  JniHelper::callStaticVoidMethod(NativeInterface, "leaveBreadcrumb", jname, jtype, jmetadata);

  env->DeleteLocalRef(jname);
  env->DeleteLocalRef(jtype);
  env->DeleteLocalRef(jmetadata);
  env->DeleteLocalRef(HashMap);
}

void Bugsnag::addMetadata(string section, string key, string *value) {
  JNIEnv *env = JniHelper::getEnv();
  if (env == nullptr) {
    return; // something has gone very wrong here.
  }
  jstring jsection = env->NewStringUTF(section);
  jstring jkey = env->NewStringUTF(key);
  jobject jvalue = value == nullptr ? NULL : env->NewStringUTF(value);

  JniHelper::callStaticVoidMethod(NativeInterface, "addToTab", jsection, jkey,
                                  jvalue);

  env->DeleteLocalRef(jsection);
  env->DeleteLocalRef(jkey);
  env->DeleteLocalRef(jvalue);
}

void Bugsnag::clearMetadata(string section) {
  JNIEnv *env = JniHelper::getEnv();
  if (env == nullptr) {
    return; // something has gone very wrong here.
  }
  jstring jsection = env->NewStringUTF(section);
  JniHelper::callStaticVoidMethod(NativeInterface, "clearTab", jsection);
  env->DeleteLocalRef(jsection);
}

void Bugsnag::setContext(string context) {
  JNIEnv *env = JniHelper::getEnv();
  if (env == nullptr) {
    return; // something has gone very wrong here.
  }
  jstring jcontext = env->NewStringUTF(context);
  JniHelper::callStaticVoidMethod(NativeInterface, "setContext", jcontext);
  env->DeleteLocalRef(jcontext);
}
} // namespace bugsnag

#endif // Android-only
