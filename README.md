# Kitkit School

Kitkit School (phase 3) consists of 12 apps as following:

1. Launcher
* A home screen app, from where other apps are launched.

2. Lock Screen
* This app appears when the device wakes up.
* The device will be unlocked by touching the Kitkit School icon in the middle.

3. Library
* A collection of videos and books for children to enjoy.
* Video will play in place, while a separate book viewer will open to read a book.

4. Book Viewer
* Book viewer app, which reads aloud most of books. The app will also read each word when one is touched.   

5. Main App
* Main learning app with tailored curriculum.

6. Blackboard
7. Drawing
8. Drum
9. Marimba
10. Writing Board
11. Sea World
12. Voice Engine (for English only) 
* Additional tools/toys as rewards

Most of the apps above share code from kitkitshcoollogger

13. KitkitSchoolLogger
* not a stand-alone app, but the code is shared among other apps


Book Viewer and MainApp are made upon cocos2d-x engine, while others are native Android apps written in Java


# Build Environment #

Android studio and Android NDK is necessary to build the APKs.
Be sure to use the specified versions mentioned below, to avoid errors. 
Android studio will also ask to install the relavent SDKs and build tools that are missing - please install them along the way. 


1. Download Android Studio (2.2.3 or higher, but not 3.x)
* https://developer.android.com/studio/archive
* Install Android Studio


2. Download Android NDK (r14b, except for voice-engine which uses r16b) 
* https://developer.android.com/ndk/downloads/older_releases.html
* https://dl.google.com/android/repository/android-ndk-r14b-darwin-x86_64.zip
* https://dl.google.com/android/repository/android-ndk-r16b-darwin-x86_64.zip






# Copy Resources/Engine Files #

Download the relavent resources/3rd party files from the release assets (v1.3) 

* to build "mainapp", place the resources/3rd party files in the following folders
  * cocos2d
   ROOT/mainapp/cocos2d
  * common resources (mainapp_resources)
   ROOT/mainapp/Resources
  * language specific resources (pick only one) 
    * mainapp_en_us (for English)
    ROOT/mainapp/Resources/localized/en-us
    * mainapp_sw_tz (for Swahili) 
    ROOT/mainapp/Resources/localized/sw-tz

* to build "seaworld", place the cocos files in the following folder
  * cocos2d
  ROOT/seaworld/cocos2d

* to build "bookviewer",  place the cocos files in the following folder
  * cocos2d
  ROOT/bookviewer/cocos2d

* to build "library", pick (only) one of the language specific resource files in the following folder
  * library_en_us (for English) 
  ROOT/library/localized/en-us
  * library_sw_tz (for Swahili) 
  ROOT/library/localized/sw-tz




# Build the APKs #


Build with Android Studio 

- don't forget to set the NDK folder
- set the appropriate 'Build Variant' 
   - mainapp : generalDebug
   - launcher, library, bookviewer : swahiliDebug or englishUSDebug (or englishDebug)
   - others : debug
- build variant of kitkitlogger will be set automatically (release)
- from the top menu, choose Build > Build APK

The resulting APK will be generated in app/build/outputs/apk/[appname]-[language]-debug.apk



# Install & Run the APKs #

Install all the APKs, then start the Kitkit Launcher to explorer. 

- Please make sure there is enough storage on the device. 
- Most of the APKs will not show up in the android (default) launcher. 


