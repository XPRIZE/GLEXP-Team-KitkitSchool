# NB(xenosoz, 2018): Build settings for libnativehelper.a

# For more information about using CMake with Android Studio, read the
# documentation: https://d.android.com/studio/projects/add-native-code.html

# Sets the minimum version of CMake required to build the native library.

cmake_minimum_required(VERSION 3.4.1)

include_directories(../aosp-monkey-patch)
include_directories(include/nativehelper)

add_library(nativehelper STATIC
            JNIHelp.cpp
            JniConstants.cpp
            toStringArray.cpp

            JniInvocation.cpp

            #AsynchronousCloseMonitor.cpp
            #tests/JniInvocation_test.cpp
           )

target_link_libraries(nativehelper
    aosp-monkey-patch)
