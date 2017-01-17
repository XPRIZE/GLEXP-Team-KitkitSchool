# Environmental Setup #

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




# Move Resources Files #

* Move Xprize Resources
* copy Resources Directory to xprize directory

* Move Xprize Book Data
* copy game_books Directory data to xprize/Resources/Games/Books/BookData directory

* Move Library Book Data
* copy library_books Directory data to book/Resources/BookData directory

* Move Library Assets
* copy assets data to library/app/src/main/assets
* copy assets-sw data to library/app/src/main/assets-sw



# Compile APKs #

* Compile XPRIZE apk
* In the xprize directory
* cocos compile -p android -m release
* Please input the absolute/relative path of '.keystore' file:
* todoschool.keystore
* Please input the alias:
* todoschool
* Please input the password of key store:
* TodoSchool
* Please input the password of alias:
* TodoSchool
* built apk is located in bin/release/android

* Compile BookTest apk
* In the book directory
* cocos compile -p android -m release
* built apk is located in bin/release/android

* Compile Library, Launcher, Lockscreen apk (unsigned)
* Android Studio - Edit Configuration
* Add Gradle Task - assemble
* Run this task for building unsigned release apk
* built apk is located in app/build/outputs/apk/app-release-unsigned.apk

* For Swahili version Library
Rename assets to assets-en and assets-sw to assets, and Build


# Build AOSP to make image #

step 1.
Follow this 
https://source.android.com/source/requirements.html
https://source.android.com/source/downloading.html
https://source.android.com/source/building.html
need at least 150G of empty space. 

step 2.
make a folder to <aosp root>/package/app/<app name>
: BookTest / TodoSchool / TodoSchoolLauncher / TodoSchoolLibrary / TodoSchoolLockScreen

put *.apk file to the folder and make Android.mk and fill with

LOCAL_PATH := $(call my-dir)
include $(CLEAR_VARS)
LOCAL_MODULE_TAGS := optional
LOCAL_MODULE := < your app folder name >
LOCAL_CERTIFICATE := < desired key >
LOCAL_SRC_FILES := < app apk filename >
LOCAL_MODULE_CLASS := APPS
LOCAL_MODULE_SUFFIX := $(COMMON_ANDROID_PACKAGE_SUFFIX)
include $(BUILD_PREBUILT)

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
BOARD_USERDATAIMAGE_PARTITION_SIZE := 474316800
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
#and edit the following
PRODUCT_PROPERTY_OVERRIDES += \
persist.sys.language=en \
persist.sys.country=TZ \
persist.sys.timezone=Africa/Dar_es_Salaam
