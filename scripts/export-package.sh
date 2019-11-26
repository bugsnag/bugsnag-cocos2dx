#!/usr/bin/env bash
#
# Assembles the release archive in the following format:
#
# bugsnag-cocos2dx.zip
# ├── CMakeLists.txt
# ├── android
# │   ├── Bugsnag.cpp
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
#         └── Bugsnag.hpp

PROJ_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")"/.. && pwd)"

if [ -d "$PROJ_DIR"/build/pkg ]; then
    echo "Deleting old artifacts"
    rm -rf "$PROJ_DIR"/pkg
fi

echo "Building staging directory..."
mkdir -p "$PROJ_DIR"/build/pkg/{android,cocoa,include/BugsnagCocos2dx}
mkdir -p "$PROJ_DIR"/build/pkg/android/libs

# Copy in headers that users will need
install "$PROJ_DIR"/src/Bugsnag.hpp \
    "$PROJ_DIR"/src/Configuration.hpp \
    "$PROJ_DIR"/build/pkg/include/BugsnagCocos2dx

# Copy in CMakeLists.txt
install "$PROJ_DIR"/src/CMakeLists.txt "$PROJ_DIR"/build/pkg/

echo "Assembling android artifacts..."
mkdir -p "$PROJ_DIR"/build/android
$(cd src/android/bugsnag-android && \
    ./gradlew bugsnag-android-core:assembleRelease && \
    ./gradlew bugsnag-plugin-android-ndk:assembleRelease && \
    ./gradlew bugsnag-plugin-android-anr:assembleRelease && \
    ./gradlew bugsnag-android:assembleRelease)

ANDROID_ARTIFACTS=$(ls "$PROJ_DIR"/src/android/bugsnag-android/bugsnag-*/build/outputs/aar/bugsnag-{android,android-core,plugin-*}-release.aar)

# Copy in android artifacts
for file in $ANDROID_ARTIFACTS
do
    cp "$file" "$PROJ_DIR"/build/pkg/android/libs/$(basename "${file/-release/}")
done

# Copy in Android source file
install "$PROJ_DIR"/src/android/Bugsnag.cpp "$PROJ_DIR"/build/pkg/android

echo "Assembling cocoa artifacts..."

# Build cocoa artifacts for each SDK
for sdk in iphoneos iphonesimulator macosx
do
    xcodebuild -scheme BugsnagCocos2dx \
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

# Zip everything up
$(cd "$PROJ_DIR"/build/pkg && zip -r ../bugsnag-cocos2dx.zip .)

