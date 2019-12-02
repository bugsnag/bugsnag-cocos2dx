cmake_minimum_required(VERSION 3.6)

set(BUGSNAG_LIB bugsnag-cocos2dx)
if(ANDROID)
    add_library(${BUGSNAG_LIB} STATIC android/Bugsnag.cpp)
    target_include_directories(${BUGSNAG_LIB} PUBLIC include)
    target_include_directories(${BUGSNAG_LIB} PRIVATE android/private)
    target_link_libraries(${BUGSNAG_LIB} cocos2d)

    set(BUGSNAG_NDK_DIR
        ${CMAKE_SOURCE_DIR}/bugsnag/android/bugsnag-plugin-android-cocos2dx/build/unpacked/bugsnag-plugin-android-ndk)

    add_library(bugsnag_ndk SHARED IMPORTED)
    set_target_properties(bugsnag_ndk PROPERTIES IMPORTED_LOCATION
        ${BUGSNAG_NDK_DIR}/jni/${ANDROID_ABI}/libbugsnag-ndk.so)
    target_link_libraries(${BUGSNAG_LIB} bugsnag_ndk)
endif()
