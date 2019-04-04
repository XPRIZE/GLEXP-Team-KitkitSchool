## Installation steps for all the apps
1. Install [Android Studio 3.3.2](https://dl.google.com/dl/android/studio/install/3.3.2.0/android-studio-ide-182.5314842-mac.dmg) on a Mac OS device.
2. Download Android [NDK 14](https://dl.google.com/android/repository/android-ndk-r14b-darwin-x86_64.zip) and [NDK 16](https://dl.google.com/android/repository/android-ndk-r16b-darwin-x86_64.zip) (required only for voice-engine project).
3. Download the [_assets_](https://github.com/XPRIZE/GLEXP-Team-KitkitSchool/releases) folder of the project.
4. Replace the _cocos2d_ folder from the _assets_ with the one present in the _mainapp_, _bookviewer_ and _seaworld_ project.
5. Replace the _Resources_ folder from the _assets_ with the one present in _mainapp_ project and ensure that only one locale specific folder is present in the _localize_ folder i.e. keep either _en-us_ or _sw-tz_ folder and its related files.
6. Replace _localized_ folder from the _assets_ with the one present in the _mainapp_ project and ensure that only one locale specific folder is present inside it i.e. keep either _en-us_ or _en-ke_ or _sw-tz_ folder and its related files.
7. Open the applications in Android Studio and select build option from Build menu.
8. The build process may prompt to install various dependencies related to Android SDK and Build Tool. Make sure to install them.
9. Do not update the _Gradle_ version used in the project.
10. Install the applications from Android Studio using _Run app_ option from _Run menu_ or using the APK generated in _app/build/outputs/apk_ folder.

#### Note:
1. The _mainapp_, _seaworld_ and _bookviewer_ projects build correctly on Mac OS only. Rest others build on both Mac and Windows OS.
2. If any of the applications throw error related to signing the APK then just simply comment the signing configuration from the app's gradle file or create your own _keystore_ file.
2. The _KitkitSchoolLogger_ is not a standalone application. Instead, it is shared amongst other projects.
3. Set the build variant to __debug__ for all project apps except for the _mainapp_ which is described in point 4 of FAQs.
2. There is no need to install _cocos_ application separately for these projects.

## FAQs
1. There is a cross mark on the app cofiguration button. How to fix that?

    >The default/main activity of the application is not set for the project. Either just go to _app/build/outputs/apk_ folder and install the APK which is generated right after the build or, set the start activity in the app configuration settings of the project.

2. How to enable the _Build variant_ option in the _Build_ menu?
	>The _Build variant_ option is enabled only when the module level gradle file is opened.

3. The applications throws signing error with _keystore_ file not found error. How to fix this?

	>_keystore_ file is used to digitally sign an Android application and hence, it is not provided with the project. One must create its own _keystore_ file using the [Android keytool](https://developer.android.com/studio/publish/app-signing) which comes with the Android Studio itself in order to sign the APK.

4. How to fix the the mainapp application which throws a run time error related to the package name _com.enuma.xprize_?
	> The cause of this error is the incorrect setting of build variant of the applications. Kindly, refer to the below table which has the build variant settings.
	> 
	> |App name| Build variant|
	> |--------|--------------|
	> |KitkitSchool|generalDebug-armeabi|
	> |kitkitlogger|release|
	> |libcocos2dx|debug|
	> models|debug|

5. How to fix the error for NDK not found?

	> 1. Unzip the NDK folder.
	> 2. Rename the unzipped folder to _ndk-bundle_ and place it in Android SDK's root directory.
	> 3. Ensure the _ndk-bundle_ folder contains its child files and folders straightaway.
