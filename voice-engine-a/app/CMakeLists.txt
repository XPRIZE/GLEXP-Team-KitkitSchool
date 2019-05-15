# For more information about using CMake with Android Studio, read the
# documentation: https://d.android.com/studio/projects/add-native-code.html

# Sets the minimum version of CMake required to build the native library.

cmake_minimum_required(VERSION 3.4.1)

# NB(xenosoz, 2018): See also: libs/**/CMakeLists.txt
add_subdirectory(libs/aosp-monkey-patch)
add_subdirectory(libs/svoxpico)
add_subdirectory(libs/ttscompat)
add_subdirectory(libs/ttspico)
add_subdirectory(libs/libexpat/expat)
add_subdirectory(libs/libnativehelper)

