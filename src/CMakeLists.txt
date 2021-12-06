cmake_minimum_required(VERSION 3.6)

if(ANDROID OR APPLE) # Supports Android, iOS, and macOS

set(BUGSNAG_LIB bugsnag-cocos2dx)
project(${BUGSNAG_LIB})
set(EXTRA_LINKING_FLAGS "")

if(ANDROID)
    add_library(${BUGSNAG_LIB} STATIC android/Bugsnag.cpp)
    set(EXTRA_LINKING_FLAGS
        # Ensure JNI functions are exported from the host shared library, in
        # case the host is configured to hide all symbols through linker options
        -uJava_com_bugsnag_android_BugsnagCocos2dxPlugin_getCocos2dVersion
        -uJava_com_bugsnag_android_BugsnagCocos2dxPlugin_configureNativeComponents)

    target_include_directories(${BUGSNAG_LIB}
        PUBLIC include
        PRIVATE android/private)
    target_link_libraries(${BUGSNAG_LIB} cocos2d)

    set(BUGSNAG_NDK_DIR
        ${CMAKE_SOURCE_DIR}/bugsnag/android/bugsnag-plugin-android-cocos2dx/build/unpacked/bugsnag-plugin-android-ndk)

    add_library(bugsnag_ndk SHARED IMPORTED)
    set_target_properties(bugsnag_ndk PROPERTIES IMPORTED_LOCATION
        ${BUGSNAG_NDK_DIR}/jni/${ANDROID_ABI}/libbugsnag-ndk.so)
    target_link_libraries(${BUGSNAG_LIB} bugsnag_ndk)
elseif(APPLE)
    # Marked as 'global' because imported (pre-compiled) libraries are local to
    # the current file by default, unlike source libraries
    add_library(${BUGSNAG_LIB} STATIC IMPORTED GLOBAL)
    if(MACOSX)
        set_target_properties(${BUGSNAG_LIB} PROPERTIES
            IMPORTED_LOCATION ${CMAKE_SOURCE_DIR}/bugsnag/cocoa/libbugsnag-cocos2dx-macosx.a)
    elseif(IOS)
        set_target_properties(${BUGSNAG_LIB} PROPERTIES
            IMPORTED_LOCATION ${CMAKE_SOURCE_DIR}/bugsnag/cocoa/libbugsnag-cocos2dx-ios.a)
    endif()
endif()

set_target_properties(${BUGSNAG_LIB} PROPERTIES
    INTERFACE_INCLUDE_DIRECTORIES ${CMAKE_SOURCE_DIR}/bugsnag/include)

# Export settings into parent project
set(BUGSNAG_LINKER_FLAGS ${EXTRA_LINKING_FLAGS} PARENT_SCOPE)
endif()
