# Changelog

## 1.0.0 (2020-01-07)

First public release! Supports automatic crash reporting on Android, iOS, and
macOS for Cocos2d-x apps written in C++. Requires Cocos2d-x 3.1 and above. To
get started, see the [integration guide](https://docs.bugsnag.com/platforms/cocos2dx).

## 0.3.5 (2020-01-07)

### Bug fixes

* (Android) Fix issue where initial sessions were reported with incorrect
  Bugsnag library info

## 0.3.4 (2020-01-06)

### Bug fixes

* (Android) Bump the bugsnag-android dependency version to v4.22.2
* (Android) Remove use of the Kotlin Gradle plugin - the version previously in
  use used deprecated APIs, emitting a warning. Updating to the latest version
  caused a Gradle version conflict with stock installs of Cocos2d-x 3.x, so
  removing the plugin altogether was easier than resolving the conflict.

## 0.3.3 (2019-12-12)

### Bug fixes

* (Android) Suppress erroneous unchecked cast warning
* (Android) Fix possible crash in release mode due to obfuscating classes used
  by the JNI

## 0.3.2 (2019-12-09)

### Bug fixes

* (iOS) Fix out-of-memory errors not being detected

## 0.3.1 (2019-12-06)

### Bug Fixes

* (Android) Fix warnings during compilation
* (Android) Fix missing build dependency if the Bugsnag plugin is initialized
  prior to the cocos2d module
* (Android) Fix starting, stopping, resuming sessions manually so they actually
  do something
* (Android) Improve runtime performance by caching references in JNI hot paths

## 0.3.0 (2019-12-04)

### Enhancements

* Support [Cocos2d-x v4](https://github.com/cocos2d/cocos2d-x/blob/3ac72c076d638cc0f2a12784d031544406792640/CHANGELOG#L1)

## 0.1.0 (26-11-2019)

Initial release
