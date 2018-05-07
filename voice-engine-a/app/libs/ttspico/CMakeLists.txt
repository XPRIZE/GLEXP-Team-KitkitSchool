# NB(xenosoz, 2018): Build settings for libttspico.so

cmake_minimum_required(VERSION 3.4.1)

include_directories(../aosp-monkey-patch)
include_directories(../svoxpico)
include_directories(../ttscompat/include)
include_directories(../libexpat/expat/lib)

add_library(ttspico SHARED
    com_svox_picottsengine.cpp
    svox_ssml_parser.cpp)

#find_library(libcutils cutils)
#find_library(libexpat expat)
#find_library(libutils utils)
#find_library(liblog log)

target_link_libraries(ttspico
    svoxpico
    aosp-monkey-patch
    expat)
#${libcutils}
#${libexpat}
#${libutils}
#${liblog})

