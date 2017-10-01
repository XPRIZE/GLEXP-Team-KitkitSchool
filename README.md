# Kitkit School

Kitkit School consists of nine apps as following:

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
* Additional tools/toys as rewards

Most of the apps above share code from kitkitshcoollogger
10. KitkitSchoolLogger
* not a stand-alone app, but the code is shared among other apps


Book Viewer and MainApp are made upon cocos2d-x engine, while others are native Android apps written in Java


# Build Environment #

This section describes the build environment we were using.
Please note that we've used macOS version 10.11 and 10.12.
It may work with other OS/Tool versions, but not verified.


1. Download Cocos2dx-3.10
* http://www.cocos2d-x.org/filedown/cocos2d-x-3.10.zip
* Extract zip file and move the folder to ~/Documents/


2. Download Android Studio (2.2.3)
* https://developer.android.com/studio/index.html
* Install Android Studio


3. Download Android NDK
* http://dl.google.com/android/ndk/android-ndk-r10d-darwin-x86.bin
* Extract bin file and move the folder to ~/Documents/


4. Download Apache Ant
* http://mirror.symnds.com/software/Apache//ant/binaries/apache-ant-1.9.8-bin.zip
* Extract zip file and move the folder to ~/Documents/


5. Download Java SE Runtime 1.8
* http://download.oracle.com/otn-pub/java/jdk/8u112-b16/jdk-8u112-macosx-x64.dmg
* Install JRE 8u112
* /usr/libexec/java_home -v 1.8


6. Run cocos setup
* ~/Documents/cocos2d-x-3.10/setup.py
* add NDK Root /Users/xxxxx/Documents/android-ndk-r10d
* add SDK Root /Users/xxxxx/Library/Android/sdk
* add ANT Root /Users/xxxxx/Documents/apache-ant-1.9.8/bin


7. Add UTF-8 Settings to bash_profile
* vi ~/.bash_profile
* add two lines
* export LC_ALL=en_US.UTF-8
* export LANG=en_US.UTF-8



# Copy Resources Files #

Now the environment is setup, but we are not ready to build the apps yet.  
Due to the large amount of resources, we could not put all the necessary resources into Githug. Instead, they are stored in Box folder.

1. Library
* target directory is library/app/src/main/assets and library/app/src/main/res
* copy files from each language pack into the target directory

2. Book Viewer
* target directory is bookviewer/Resources
* copy files from each language pack into the target directory

3. Main App
* target directory is mainapp/Resources
* copy files from each language pack into the target directory
* also copy pre-built library from cocos2d.tar.gz to mainapp/cocos2d



# Build APKs #


1. Apps built with Android Studio (Launcher, Lock Screen, Library, and other tools)

- open the "Build Variants" tab at left-bottom
- set the 'Build Variant' of 'app' to swahiliDebug or englishDebug, respectively
- build variant of kitkitlogger will be set automatically (swahiliRelease or englishRelease)
- from the top menu, choose Build > Build APK


The resulting APK will be generated in app/build/outputs/apk/[appname]-[language]-debug.apk


2. Apps based on cocos2d-x (Book Viewer, Main App)

For those apps, a command line tool is used to generate APKs.

The command is:
> cocos compile -p android --android-studio

in each app's directory.

APK will be generated in proj.android-studio/app/build/outputs/apk/[appname]-[language]-debug.apk



# Language Settings #

Some apps need to change a few lines of the source code or resource files to build a binary for the other language.


1. Launcher and Lock Screen
* choose build variants respectively.

2. Library
* choose build variants respectively.
* copy the resources as described above

3. Book Viewer
* copy the resources as described above
* change line 7 of Classes/MainScene.h
    * for English
    > #define IS_ENGLISH (true)

    * for Swahili
    > #define IS_ENGLISH (false)


4. Main App
* copy the resources as described above
* change line 31 of Classes/Managers/LanguageManager.cpp
    * for English
    > auto defaultLang = LanguageType::ENGLISH;

    * for Swahili
    > auto defaultLang = LanguageType::SWAHILI;



# Build AOSP to make image #

With all the APKs, it is time to build an image.
We used a machine with ubuntu 16.04 LTS for that purpose.



1.
Follow this
https://source.android.com/source/requirements.html
https://source.android.com/source/downloading.html
https://source.android.com/source/building.html
need at least 150G of empty space.

2.
all the apps are signed with the platform key
only mainapp will be installed in userdata.img, while other apps will be placed in system.img

make a folder to <aosp root>/package/app/<app name>

put *.apk file to the folder and make Android.mk and fill with

LOCAL_PATH := $(call my-dir)
include $(CLEAR_VARS)
LOCAL_MODULE_TAGS := optional
LOCAL_MODULE := < your app folder name >
LOCAL_CERTIFICATE := platform
LOCAL_SRC_FILES := < app apk filename >
LOCAL_MODULE_CLASS := APPS
LOCAL_MODULE_SUFFIX := $(COMMON_ANDROID_PACKAGE_SUFFIX)
include $(BUILD_PREBUILT)

#< desired key > for BookTest and TodoSchool : presigned
#< desired key > for TodoSchoolLibrary and TodoSchoolLockScreen : shared
#< desired key > for TodoSchoolLauncher : package

to put the apk in the /data/app, add this line
LOCAL_MODULE_PATH := $(TARGET_OUT_DATA)/app

for system privileged app add this line (apk will be in /system/priv-app)
LOCAL_PRIVILEGED_MODULE := true

step 3.
edit device/google/dragon/device.mk
add package name to
PRODUCT_PACKAGES += < package name >

step 4.
to create userdata.img
edit device/google/dragon/BoardConfig.mk
add

#userdata
#for 32GB device
BOARD_USERDATAIMAGE_PARTITION_SIZE := 26557284352
#for 64GB device
#BOARD_USERDATAIMAGE_PARTITION_SIZE := 57825820672
#TARGET_USERIMAGES_SPARSE_EXT_DISABLED := true

step 5.
test by doing following
https://source.android.com/source/running.html

Troubleshooting

Fix for Xcode 8 or newer
Download Xcode 7.3 and copy and paste MacOSX10.11.sdk to <Xcode8 or newer.app>/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/

Fix for Marshmallow on ubuntu

art/build/Android.common_build.mk
line 75
change
ifneq ($(Without_HOST_CLANG),true)
to
ifeq ($(WITHOUT_HOST_CLANG),false)


reference : http://stackoverflow.com/questions/10579827/add-apk-files-in-aosp/20137625#…
http://stackoverflow.com/questions/19868138/aosp-privileged-vs-system-app



# Build Android Image #

make clobber
source build/envsetup.sh
lunch   (aosp_dragon-eng 16 )
make -j8


#after build

adb reboot bootloader
fastboot flash userdata '/usr/local/aosp/NMF26H/out/target/product/dragon/userdata.img'
fastboot flashall

#then, device will boot



#change the wallpaper
#put the image files to following directories
#path for default_wallpaper.png
device/google/dragon/overlay/frameworks/base/core/res/res/drawable-nodpi (1243x1024)
device/google/dragon/overlay/frameworks/base/core/res/res/drawable-sw600dp-nodpi (2331x1920)
device/google/dragon/overlay/frameworks/base/core/res/res/drawable-sw720dp-nodpi (2880x2560)

#path for bootanimation.zip
system/media

#making bootanimation.zip
cd kitkitbootanimation
zip -0r bootanimation.zip desc.txt part0 part1 part2

#To change language, edit /device/google/dragon/device.mk
#and edit the following, en for english and sw for swahilli
PRODUCT_PROPERTY_OVERRIDES += \
persist.sys.language=en \
persist.sys.country=TZ \
persist.sys.timezone=Africa/Dar_es_Salaam
