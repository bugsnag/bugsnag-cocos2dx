#!/usr/bin/env bash
# Release archiver
# Depends on: jq
#
# Assembles the release archive in the following format:
#
# bugsnag-cocos2dx.zip
# ├── CMakeLists.txt
# ├── android
# │   ├── Bugsnag.cpp
# │   ├── bugsnag-plugin-android-cocos2dx
# │   ├── private
# │   │   ├── bugsnag.h
# │   │   └── report.h
# │   └── libs
# │       ├── bugsnag-android.aar
# │       ├── bugsnag-android-core.aar
# │       ├── bugsnag-plugin-android-ndk.aar
# │       └── bugsnag-plugin-android-anr.aar
# ├── cocoa
# │   └── BugsnagCocos2dx.xcframework
# └── include
#     └── BugsnagCocos2dx
#         └── Bugsnag.hpp

PROJ_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")"/.. && pwd)"
BUILD_DIR="$PROJ_DIR/build"
PKG_DIR="$BUILD_DIR/bugsnag"
ZIPFILE="$BUILD_DIR/bugsnag-cocos2dx.zip"

# Stop the script when things go wrong
set -xue pipefail

if [ -d "$BUILD_DIR" ]; then
    echo "Deleting old artifacts"
    rm -rf "$BUILD_DIR"
fi
rm -f "$ZIPFILE"

echo "Building staging directory..."
mkdir -p "$PKG_DIR"/{android,cocoa,include/BugsnagCocos2dx}
mkdir -p "$PKG_DIR"/android/{private,libs}

# Copy in headers that users will need
install "$PROJ_DIR"/src/Bugsnag.hpp \
    "$PKG_DIR"/include/BugsnagCocos2dx

# Copy in CMakeLists.txt
install "$PROJ_DIR"/src/CMakeLists.txt "$PKG_DIR"/


###########
# Android #
###########

echo "Assembling android artifacts..."
mkdir -p "$BUILD_DIR"/android
pushd src/android/bugsnag-android
GRADLE="./gradlew --quiet -PABI_FILTERS=x86,x86_64,armeabi-v7a,arm64-v8a"
    $GRADLE bugsnag-android-core:assembleRelease
    $GRADLE bugsnag-plugin-android-ndk:assembleRelease
    $GRADLE bugsnag-plugin-android-anr:assembleRelease
    $GRADLE bugsnag-android:assembleRelease
popd

ANDROID_ARTIFACTS=$(ls "$PROJ_DIR"/src/android/bugsnag-android/bugsnag-*/build/outputs/aar/bugsnag-{android,android-core,plugin-*}-release.aar)

# Copy in android artifacts
for file in $ANDROID_ARTIFACTS
do
    cp "$file" "$PKG_DIR"/android/libs/$(basename "${file/-release/}")
done

# Copy in Android source file
install "$PROJ_DIR"/src/android/Bugsnag.cpp "$PKG_DIR"/android

# Copy in Android private headers
install "$PROJ_DIR"/src/android/bugsnag-android/bugsnag-plugin-android-ndk/src/main/assets/include/{bugsnag,event}.h \
    "$PKG_DIR"/android/private

# Copy in Android plugin sources
mkdir -p "$PKG_DIR"/android/bugsnag-plugin-android-cocos2dx
cp -r "$PROJ_DIR"/src/android/bugsnag-plugin-android-cocos2dx/src \
    "$PKG_DIR"/android/bugsnag-plugin-android-cocos2dx/src
install "$PROJ_DIR"/src/android/bugsnag-plugin-android-cocos2dx/build.gradle \
    "$PKG_DIR"/android/bugsnag-plugin-android-cocos2dx


#########
# Cocoa #
#########

echo "Assembling cocoa artifacts..."

# Build cocoa artifacts for each SDK
for sdk in iphoneos iphonesimulator macosx
do
    xcodebuild -quiet -scheme BugsnagCocos2dx \
        -project "$PROJ_DIR"/src/cocoa/BugsnagCocos2dx.xcodeproj \
        -configuration Release \
        -derivedDataPath build/$sdk \
        -sdk $sdk build
done

# Get the cocoa-specific headers
rm -rf "$BUILD_DIR"/include
mkdir -p "$BUILD_DIR"/include/{Bugsnag,BugsnagCocos2dx}
install $(cat "$PROJ_DIR"/src/cocoa/bugsnag-cocoa/Bugsnag.podspec.json \
    | jq .public_header_files[] \
    | xargs -n 1 -I@ bash -c "ls \"$PROJ_DIR\"/src/cocoa/bugsnag-cocoa/@") \
    "$BUILD_DIR"/include/Bugsnag
install "$PROJ_DIR"/src/cocoa/BugsnagCocos2dxPlugin.h \
    "$BUILD_DIR"/include/BugsnagCocos2dx
install "$PROJ_DIR"/src/Bugsnag.hpp \
    "$BUILD_DIR"/include/BugsnagCocos2dx

# Join artifacts into a single library
xcodebuild -create-xcframework \
    -library "$BUILD_DIR"/iphoneos/Build/Products/Release-iphoneos/libBugsnagCocos2dx.a \
    -headers "$BUILD_DIR"/include \
    -library "$BUILD_DIR"/iphonesimulator/Build/Products/Release-iphonesimulator/libBugsnagCocos2dx.a \
    -headers "$BUILD_DIR"/include \
    -library "$BUILD_DIR"/macosx/Build/Products/Release/libBugsnagCocos2dx.a \
    -headers "$BUILD_DIR"/include \
    -output "$PKG_DIR"/cocoa/BugsnagCocos2dx.xcframework


###########
# Zipfile #
###########

# Zip everything up
pushd "$BUILD_DIR"
    zip -r "$ZIPFILE" bugsnag
popd
