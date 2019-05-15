# NB(xenosoz, 2018): Build settings for libaosp_monkey_patch.so

cmake_minimum_required(VERSION 3.4.1)

include_directories(.)

add_library(aosp-monkey-patch STATIC 
    cutils/strdup8to16.c
    cutils/strdup16to8.c)

#find_library(liblog log)

target_link_libraries(aosp-monkey-patch)
#${liblob})

