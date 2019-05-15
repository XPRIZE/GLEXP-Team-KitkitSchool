# NB(xenosoz, 2018): Build settings for libsvoxpico.so

# For more information about using CMake with Android Studio, read the
# documentation: https://d.android.com/studio/projects/add-native-code.html

# Sets the minimum version of CMake required to build the native library.

cmake_minimum_required(VERSION 3.4.1)

# Creates and names a library, sets it as either STATIC
# or SHARED, and provides the relative paths to its source code.
# You can define multiple libraries, and CMake builds them for you.
# Gradle automatically packages shared libraries with your APK.

add_library(# Sets the name of the library.
            svoxpico

            # Sets the library as a shared library.
            STATIC

            # Provides a relative path to your source file(s).
            picoacph.c
            picoapi.c
            picobase.c
            picocep.c
            picoctrl.c
            picodata.c
            picodbg.c
            picoextapi.c
            picofftsg.c
            picokdbg.c
            picokdt.c
            picokfst.c
            picoklex.c
            picoknow.c
            picokpdf.c
            picokpr.c
            picoktab.c
            picoos.c
            picopal.c
            picopam.c
            picopr.c
            picorsrc.c
            picosa.c
            picosig.c
            picosig2.c
            picospho.c
            picotok.c
            picotrns.c
            picowa.c
           )
