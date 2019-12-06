# Contributing

Thanks for stopping by! This document should cover most topics surrounding contributing to `bugsnag-cocos2dx`.

* [How to contribute](#how-to-contribute)
  * [Reporting issues](#reporting-issues)
  * [Fixing issues](#fixing-issues)
  * [Adding features](#adding-features)
* [Architecture](#architecture)
* [Building](#building)
* [Releasing](#releasing)

## Reporting issues

Are you having trouble getting started? Please [contact us directly](mailto:support@bugsnag.com?subject=%5BGitHub%5D%20Cocos2d%20-%20having%20trouble%20getting%20started%20with%20Bugsnag) for assistance with integrating Bugsnag into your application.
If you have spotted a problem with this module, feel free to open a [new issue](https://github.com/bugsnag/bugsnag-cocos2dx/issues/new?template=Bug_report.md). Here are a few things to check before doing so:

* Are you using the latest version of `bugsnag-cocos2dx`? If not, does updating to the latest version fix your issue?
* Has somebody else [already reported](https://github.com/bugsnag/bugsnag-cocos2dx/issues?utf8=%E2%9C%93&q=is%3Aissue%20is%3Aopen) your issue? Feel free to add additional context to or check-in on an existing issue that matches your own.
* Is your issue caused by this module? Only things related to the `bugsnag-cocos2dx` module should be reported here. For anything else, please [contact us directly](mailto:support@bugsnag.com) and we'd be happy to help you out.

### Fixing issues

If you've identified a fix to a new or existing issue, we welcome contributions!
Here are some helpful suggestions on contributing that help us merge your PR quickly and smoothly:

* [Fork](https://help.github.com/articles/fork-a-repo) the
  [library on GitHub](https://github.com/bugsnag/bugsnag-cocos2dx)
* Build and test your changes.
* Commit and push until you are happy with your contribution
* [Make a pull request](https://help.github.com/articles/using-pull-requests)
* Ensure the automated checks pass (and if it fails, please try to address the cause)

### Adding features

Unfortunately we’re unable to accept PRs that add features or refactor the library at this time.
However, we’re very eager and welcome to hearing feedback about the library so please contact us directly to discuss your idea, or open a
[feature request](https://github.com/bugsnag/bugsnag-cocos2dx/issues/new?template=Feature_request.md) to help us improve the library.

Here’s a bit about our process designing and building the Bugsnag libraries:

* We have an internal roadmap to plan out the features we build, and sometimes we will already be planning your suggested feature!
* Our open source libraries span many languages and frameworks so we strive to ensure they are idiomatic on the given platform, but also consistent in terminology between platforms. That way the core concepts are familiar whether you adopt Bugsnag for one platform or many.
* Finally, one of our goals is to ensure our libraries work reliably, even in crashy, multi-threaded environments. Oftentimes, this requires an intensive engineering design and code review process that adheres to our style and linting guidelines.

## Architecture

The bugsnag-cocos2dx package provides:

* Native crash reporting for Android, iOS, and macOS
* A "native" (Java/Objective-C) interface for initialization - capturing crashes
  from the earliest moment after app launch
* A C++ interface for logging breadcrumb, reporting non-fatal errors, recording
  report diagnostics, and session management

Since there is no holistic package management solution for either Cocos2d or
mobile/desktop platforms, bugsnag-cocos2dx is distributed as a zip file
containing a linkable package for Android, iOS, and macOS in the following
structure:

```
bugsnag/
├── CMakeLists.txt
├── android
│   ├── Bugsnag.cpp
│   ├── bugsnag-plugin-android-cocos2dx
│   │   ├── build.gradle
│   │   └── src/main
│   │           ├── AndroidManifest.xml
│   │           └── java/com/bugsnag/android/BugsnagCocos2dxPlugin.kt
│   ├── libs
│   │   ├── bugsnag-android-core.aar
│   │   ├── bugsnag-android.aar
│   │   ├── bugsnag-plugin-android-anr.aar
│   │   └── bugsnag-plugin-android-ndk.aar
│   └── private
│       ├── bugsnag.h
│       └── report.h
├── cocoa
│   ├── libbugsnag-cocos2dx-ios.a
│   └── libbugsnag-cocos2dx-macosx.a
└── include
    └── BugsnagCocos2dx
        ├── Bugsnag.hpp
        └── cocoa
            ├── BSG_KSCrashReportWriter.h
            ├── Bugsnag.h
            ├── BugsnagBreadcrumb.h
            ├── BugsnagCocos2dxPlugin.h
            ├── BugsnagConfiguration.h
            ├── BugsnagCrashReport.h
            ├── BugsnagMetaData.h
            └── BugsnagPlugin.h

```

Each platform includes a plugin named "BugsnagCocos2dxPlugin", which is
initialized prior to initializing a Bugsnag crash reporting package and provides
Cocos2d-specific functionality.

```java
import com.bugsnag.android.BugsnagCocos2dxPlugin;
import com.bugsnag.android.Bugsnag;

public class AppActivity extends Cocos2dxActivity {

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        // ...
        BugsnagCocos2dxPlugin.register();
        Bugsnag.init(this);
    }
}
```

```objc
#import <BugsnagCocos2dx/cocoa/BugsnagCocos2dxPlugin.h>
#import <BugsnagCocos2dx/cocoa/Bugsnag.h>

int main(int argc, char *argv[]) {
    [BugsnagCocos2dxPlugin registerWithCocos2dVersion:cocos2dVersion()];
    [Bugsnag startBugsnagWithApiKey:@"YOUR-API-KEY-HERE"];

    // ...
}
```

### Android

The Android package includes pre-compiled artifacts for bugsnag-android crash
reporting, which are linked by the plugin package and transitively included in
the host application. The host application must add a `flatDirs` repository to
find them but otherwise does not need to interact with the artifacts themselves.

The host application instead needs to identify and include the plugin package,
which is included in the module-level gradle build configuration.

The C++ interface is provided as a source file to link to the copy of
Cocos2d-x being used in the application via CMake (the primary build system). It
also links to the native components of bugsnag-plugin-android-ndk, to provide
stacktrace collection for `notify()` (This is why there are `private` headers in
the package).

The C++ interface also provides a JNI method to return the version of Cocos2d-x
being used to the Java-layer plugin, for inclusion in the runtime versions
report diagnostics.

### iOS / macOS

The iOS / macOS package includes pre-compiled artifacts for the entire plugin,
which includes bugsnag-cocoa in the static library binaries. The libraries are
then linked by the host application and the interface is then provided through
the `Bugsnag.hpp` header file. (Note: The iOS device and simulator libraries are
built and then lipo-ed together into a single binary)

The public bugsnag-cocoa headers are included with the library under a `cocoa`
directory, since the two libraries are packaged into one.

The Cocos2d-x version is passed in manually rather than linking to Cocos2d
directly because of the difficulty in doing so in cocos2d v3.x, which would
require library consumers to alter the installed Bugsnag package's Xcode project
to point to the location of their cocos2d installation. This would make
upgrading require more manual steps than deleting the package and adding a new
one in its place. Going forward, cocos2d v4.x uses CMake to generate Xcode
projects as a build artifact, so the package could instead use a similar
approach to Android a load the version automatically in a distant future
version.

## Building

`bugsnag-cocos2dx` depends on
[bugsnag-android](https://github.com/bugsnag/bugsnag-android) and
[bugsnag-cocoa](https://github.com/bugsnag/bugsnag-cocoa) as well as Cocos2d-x
itself. Both are included as submodules, so building requires:

```
git submodule update --init --recursive
```

Because of these dependencies, it is easiest to hack on the library as a part of
an example project where all of these components are integrated.

To assemble a release package, run `./scripts/export-package.sh`

The assembled package can then be added to a project using the [integration
guide](https://docs.bugsnag.com/platforms/cocos2dx/).

## Releasing

### Upgrading the vendored libraries

To update `bugsnag-cocoa` or `bugsnag-android`, open each directory in a
terminal and run `git fetch` then `git checkout {tag name}`. Commit the change
with an update to the changelog with the new fixes and features.

On Android, there are additional declared dependencies in
`src/android/bugsnag-plugin-android-cocos2dx` which must be synchronized with
the versions used in `bugsnag-android/gradle/dependencies.gradle`, to ensure
that the library compiles correctly and runs in the host application. At the
time of writing, these are:

* kotlin-stdlib
* androidx.annotation

### Making the release

1. Update the changelog with new features and fixes
2. Bump the version number using `./scripts/bump-version.sh [number]`
3. Commit, tag the changes, and push
4. Wait for the CI build, which will draft a new release on GitHub
5. Edit the release on GitHub, copying the changelog entry

#### Post-release checks

- [ ] Have all Docs PRs been merged?
- [ ] Do the installation instructions work using the released artifact?
- [ ] Can a freshly created example app send an error report from a release build, using the released artifact?

