# Contributing

Thanks for stopping by! This document should cover most topics surrounding contributing to `bugsnag-cocos2dx`.

* [How to contribute](#how-to-contribute)
  * [Reporting issues](#reporting-issues)
  * [Fixing issues](#fixing-issues)
  * [Adding features](#adding-features)
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

### Making the release

1. Update the changelog with new features and fixes
2. Increment the version number in src/android/bugsnag-plugin-android-cocos2dx/build.gradle
3. Update the CHANGELOG.md and add the current version and date
4. Run `./scripts/export-package.sh`
5. Create a new release on GitHub, copying the changelog entry and
   `build/bugsnag-cocos2dx.zip`

#### Post-release checks

- [ ] Have all Docs PRs been merged?
- [ ] Do the installation instructions work using the released artifact?
- [ ] Can a freshly created example app send an error report from a release build, using the released artifact?

