cmake_minimum_required(VERSION 3.6)

set(BUGSNAG_LIB bugsnag-cocos2dx)
if(ANDROID)
    add_library(${BUGSNAG_LIB} STATIC android/Bugsnag.cpp)
    target_include_directories(${BUGSNAG_LIB} PUBLIC include)
    target_link_libraries(${BUGSNAG_LIB} cocos2d)
endif()
