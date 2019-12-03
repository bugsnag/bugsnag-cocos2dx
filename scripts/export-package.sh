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
# │   ├── libbugsnag-cocos2dx-ios.a
# │   └── libbugsnag-cocos2dx-macosx.a
# └── include
#     └── BugsnagCocos2dx
#         ├── cocoa/{bugsnag-cocoa public headers}
#         ├── BugsnagCocos2dxPlugin.h
#         └── Bugsnag.hpp

PROJ_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")"/.. && pwd)"

# Stop the script when things go wrong
set -e pipefail

if [ -d "$PROJ_DIR"/build/pkg ]; then
    echo "Deleting old artifacts"
    rm -rf "$PROJ_DIR"/build/pkg
fi

echo "Building staging directory..."
mkdir -p "$PROJ_DIR"/build/pkg/{android,cocoa,include/BugsnagCocos2dx/cocoa}
mkdir -p "$PROJ_DIR"/build/pkg/android/{private,libs}

# Copy in headers that users will need
install "$PROJ_DIR"/src/Bugsnag.hpp \
    "$PROJ_DIR"/build/pkg/include/BugsnagCocos2dx

# Copy in CMakeLists.txt
install "$PROJ_DIR"/src/CMakeLists.txt "$PROJ_DIR"/build/pkg/

echo "Assembling android artifacts..."
mkdir -p "$PROJ_DIR"/build/android
pushd src/android/bugsnag-android
    ./gradlew --quiet bugsnag-android-core:assembleRelease
    ./gradlew --quiet bugsnag-plugin-android-ndk:assembleRelease
    ./gradlew --quiet bugsnag-plugin-android-anr:assembleRelease
    ./gradlew --quiet bugsnag-android:assembleRelease
popd

ANDROID_ARTIFACTS=$(ls "$PROJ_DIR"/src/android/bugsnag-android/bugsnag-*/build/outputs/aar/bugsnag-{android,android-core,plugin-*}-release.aar)

# Copy in android artifacts
for file in $ANDROID_ARTIFACTS
do
    cp "$file" "$PROJ_DIR"/build/pkg/android/libs/$(basename "${file/-release/}")
done

# Copy in Android source file
install "$PROJ_DIR"/src/android/Bugsnag.cpp "$PROJ_DIR"/build/pkg/android

# Copy in Android private headers
install "$PROJ_DIR"/src/android/bugsnag-android/bugsnag-plugin-android-ndk/src/main/assets/include/{bugsnag,report}.h \
    "$PROJ_DIR"/build/pkg/android/private

# Copy in Android plugin sources
mkdir -p "$PROJ_DIR"/build/pkg/android/bugsnag-plugin-android-cocos2dx
cp -r "$PROJ_DIR"/src/android/bugsnag-plugin-android-cocos2dx/src \
    "$PROJ_DIR"/build/pkg/android/bugsnag-plugin-android-cocos2dx/src
install "$PROJ_DIR"/src/android/bugsnag-plugin-android-cocos2dx/build.gradle \
    "$PROJ_DIR"/build/pkg/android/bugsnag-plugin-android-cocos2dx

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

# Join iOS artifacts into a single library
lipo -create "$PROJ_DIR"/build/iphone*/Build/Products/Release/libBugsnagCocos2dx.a \
    -output "$PROJ_DIR"/build/pkg/cocoa/libbugsnag-cocos2dx-ios.a

cp "$PROJ_DIR"/build/macosx/Build/Products/Release/libBugsnagCocos2dx.a \
    "$PROJ_DIR"/build/pkg/cocoa/libbugsnag-cocos2dx-macosx.a

# Copy the cocoa-specific headers in
install $(cat "$PROJ_DIR"/src/cocoa/bugsnag-cocoa/Bugsnag.podspec.json \
    | jq .public_header_files[] \
    | xargs -n 1 -I@ bash -c "ls \"$PROJ_DIR\"/src/cocoa/bugsnag-cocoa/@") \
    "$PROJ_DIR"/build/pkg/include/BugsnagCocos2dx/cocoa
install "$PROJ_DIR"/src/cocoa/BugsnagCocos2dxPlugin.h \
    "$PROJ_DIR"/build/pkg/include/BugsnagCocos2dx/cocoa

# Zip everything up
pushd "$PROJ_DIR"/build/pkg
    zip -r ../bugsnag-cocos2dx.zip .
popd

