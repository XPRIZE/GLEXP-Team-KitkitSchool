# NB(xenosoz, 2018): Build settings for libttscompat.so

cmake_minimum_required(VERSION 3.4.1)

include_directories(
    ../aosp-monkey-patch
    ../base/include
    ../libnativehelper/include
)

add_library(ttscompat SHARED
    jni/com_android_tts_compat_SynthProxy.cpp)

target_link_libraries(ttscompat
    aosp-monkey-patch
    nativehelper)

#find_library(libandroid_runtime android_runtime)
#find_library(libnativehelper nativehelper)
#find_library(libmedia media)
#find_library(libutils utils)
#find_library(libcutils cutils)
#find_library(liblog log)
#find_library(libdl dl)

#target_link_libraries(ttscompat
#${libandroid_runtime}
#${libnativehelper}
#${libmedia}
#${libutils}
#${libcutils}
#${liblog}
#${libdl})

