# Changelog

## TBD

### Enhancements

* Updated to the latest Bugsnag Cocoa library. This brings a number of changes. Please see the updated docs at https://docs.bugsnag.com/platforms/cocos2dx/
  - The cocoa library is now distributed as an xcframework.
  - All required headers are also inside the xcframework, so it is no longer necessary to have custom header search paths.
  - Initializing the Bugsnag Cocos2d-x plugin for Cocoa is now done using a similar API to the Bugsnag Cocoa library.
  - The distributed zipfile now keeps everything inside a top-level "bugsnag" folder.

## 1.1.0 (2021-12-06)

### Enhancements

* Backport `persistenceDirectory` config option to Android [bugsnag-cocos2dx#6](https://github.com/bugsnag/bugsnag-cocos2dx/pull/6)

### Bug fixes

* (Android) Fix UTF-8 encoding errors in Bugsnag::notify [bugsnag-android#1542](https://github.com/bugsnag/bugsnag-android/pull/1542)
* Update bugsnag-android to v4.22.3
  * Allow disabling previous signal handler invocation for Unity ANRs [bugsnag-android#743]
  * Avoid polling when detecting ANRs by invoking JNI from SIGQUIT handler [bugsnag-android#741]

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
