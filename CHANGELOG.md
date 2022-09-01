# Changelog

## TBD

### Enhancements

* Updates the bugsnag-cocoa dependency from v6.16.5 to [v6.22.3](https://github.com/bugsnag/bugsnag-cocoa/blob/master/CHANGELOG.md#6223-2022-09-01)

## 2.0.1 (2022-04-11)

### Enhancements

* Updates the bugsnag-android dependency from v5.19.1 to [v5.22.0](https://github.com/bugsnag/bugsnag-android/blob/master/CHANGELOG.md#5220-2022-03-31)
* Updates the bugsnag-cocoa dependency from v6.16.1 to [v6.16.5](https://github.com/bugsnag/bugsnag-cocoa/blob/master/CHANGELOG.md#6165-2022-03-30)

## 2.0.0 (2022-02-23)

This release bumps the native Bugsnag Android and iOS/macOS libraries to their latest major versions (`bugsnag-android` [v5.19.1](https://github.com/bugsnag/bugsnag-android/releases/tag/v5.19.1) and `bugsnag-cocoa` [v6.16.1](https://github.com/bugsnag/bugsnag-cocoa/releases/tag/v6.16.1)) to take advantage of the latest enhancements and bug fixes.

**Please note**, this release contains a breaking change to the setup instructions of the library: it is now initialized as a plugin in the Bugsnag configuration object. For example, in Android:

```diff
- BugsnagCocos2dxPlugin.register();
- Bugsnag.init(this.getApplicationContext());
+ Configuration config = Configuration.load(this);
+ config.addPlugin(new BugsnagCocos2dxPlugin());
+ Bugsnag.start(this, config);
```

In addition, the iOS/macOS library is now distributed as an `xcframework` so it is no longer necessary to have custom header search paths. The distributed `.zip` file now keeps everything inside a top-level "bugsnag" folder.

For more information, please see the updated docs at https://docs.bugsnag.com/platforms/cocos2dx/

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
