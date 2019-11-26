# Bugsnag error reporter for Cocos2d-x
[![Documentation](https://img.shields.io/badge/documentation-latest-blue.svg)](http://docs.bugsnag.com/platforms/cocos2dx/)

The Bugsnag Notifier for Cocos2d-x gives you instant notification of uncaught
exceptions and native crashes thrown from your Cocos2d-x games on iOS, macOS and
Android devices.

[Bugsnag](https://www.bugsnag.com) captures errors in real-time from your web
and mobile apps, helping you to understand and resolve them as fast as possible.
[Create a free account](https://www.bugsnag.com) to start monitoring and reporting errors today.

## Features

* Automatically report unhandled exceptions and crashes
* Report [handled exceptions](https://docs.bugsnag.com/platforms/cocos2dx/#reporting-handled-errors)
* [Log breadcrumbs](https://docs.bugsnag.com/platforms/cocos2dx/#logging-breadcrumbs) which are attached to crash reports and add insight to users' actions
* [Attach user information](https://docs.bugsnag.com/platforms/cocos2dx/#identifying-users) to determine how many people are affected by a crash

## Getting started

1. [Create a Bugsnag account](https://bugsnag.com)
1. Complete the instructions in the [integration guide](https://docs.bugsnag.com/platforms/cocos2dx/) to report unhandled exceptions thrown from your app
1. Report handled exceptions using [`Bugsnag::notify()`](https://docs.bugsnag.com/platforms/cocos2dx/#reporting-handled-errors)
1. Customize your integration using the [configuration options](https://docs.bugsnag.com/platforms/cocos2dx/configuration-options/)

## Support

* [Read the integration guide](https://docs.bugsnag.com/platforms/cocos2dx/) or [configuration options documentation](https://docs.bugsnag.com/platforms/cocos2dx/configuration-options/)
* [Search open and closed issues](https://github.com/bugsnag/bugsnag-cocos2dx/issues?utf8=✓&q=is%3Aissue) for similar problems
* [Report a bug or request a feature](https://github.com/bugsnag/bugsnag-cocos2dx/issues/new)

## Contributing

All contributors are welcome! For information on how to build, test
and release `bugsnag-cocos2dx`, see our
[contributing guide](https://github.com/bugsnag/bugsnag-cocos2dx/blob/master/CONTRIBUTING.md).

## License

The Bugsnag Cocos2d-x library is free software released under the MIT License.
See [LICENSE.txt](https://github.com/bugsnag/bugsnag-cocos2dx/blob/master/LICENSE.txt)
for details.
