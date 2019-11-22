# bugsnag-cocos2d-x

* Android installed via gradle
* iOS/macOS installed via CocoaPods
* C++ helpers for init and notify, etc

(Most of the) Cross-platform API:

```cpp
bugsnag::Bugsnag::init("your-api-key");

// or:

auto config = new bugsnag::Configuration("your-api-key");
config->releaseStage = "beta";
bugsnag::Bugsnag::init(config);
```

```cpp
using namespace bugsnag;

// ...

Bugsnag::notify(cppException);

Bugsnag::startSession();

Bugsnag::leaveBreadcrumb("Log out", BreadcrumbType.user, {
  {"location", "pause menu"}
});

Bugsnag::setContext("High Scores");

Bugsnag::addMetadata("device", "AR support", true);

Bugsnag::setUser("1234", "bob@example.com", "Bob Hoskins");
```

