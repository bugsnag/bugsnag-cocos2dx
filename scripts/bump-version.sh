#!/usr/bin/env bash
# Update package version numbers
# The version number is currently stored in three places:
# * src/cocoa/version.xcconfig
# * src/android/bugsnag-plugin-android-cocos2dx/build.gradle
# * CHANGELOG.md

if (( $# != 1 )); then
    echo "Usage: $0 [VERSION]"
fi
RELEASE_VERSION="$1"
echo Bumping the version number to $RELEASE_VERSION
sed -i '' "s/versionName \'.*\'/versionName \'$RELEASE_VERSION\'/" src/android/bugsnag-plugin-android-cocos2dx/build.gradle
sed -i '' "s/BUGSNAG_COCOS2DX_VERSION =.*/BUGSNAG_COCOS2DX_VERSION = @\"$RELEASE_VERSION\";/" src/cocoa/BugsnagCocos2dxPlugin.m
sed -i '' "s/## TBD/## $RELEASE_VERSION ($(date '+%Y-%m-%d'))/" CHANGELOG.md
