Bugsnag Cocos2d-x Example Project
=================================

This is a standard cocos2d-x project built under cocos2d-x 3.17.2 using the command line `cocos new -l cpp example-cpp-app`, and then following the [bugsnag installation instructions](https://docs.bugsnag.com/platforms/cocos2dx/).

**Note**: The `bugsnag` directory is actually a symlink, and so you must first run `./scripts/export-package.sh` from the root of bugsnag-cocos2dx to generate the files needed to compile the example project (so that you're always building the example app using the current commit of bugsnag-cocos2dx). These files are what you'd normally download as a zip archive from the [releases page](https://github.com/bugsnag/bugsnag-cocos2dx/releases/latest).

Projects in `proj.android` and `proj.ios_mac` can be opened, built, and run in their respective IDEs.
