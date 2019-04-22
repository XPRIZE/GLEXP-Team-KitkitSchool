# Localization steps for all the apps

## Launcher
1. The Launcher application is used for launching different applications in KitkitSchool world.
2. The [strings.xml](https://github.com/XPRIZE/GLEXP-Team-KitkitSchool/blob/master/launcher/app/src/main/res/values/strings.xml) file defines the strings being used in the Launcher to display names for various elements. Change the value for each element in this file except for the following element:  
    `<string name="app_name">Kitkit Launcher</string>`
___
## Main App
1. The Main App is the main learning application of KitkitSchool with tailored curriculum. This application consists of content for learning literacy and numeracy.  
2. Each section of the application consists of section specific audios, videos and images. These assets can be localized as follows:  
    1. Set locale in [configuration.xml](https://github.com/XPRIZE/GLEXP-Team-KitkitSchool/blob/master/kitkitschoollogger/kitkitlogger/src/main/res/values/configuration.xml) file.  
    2. Replace content in the [assets](https://github.com/XPRIZE/GLEXP-Team-KitkitSchool/tree/master/mainapp/proj.android-studio/models/src/main/assets) folder with locale specific content.  
    3. Replace content in [Resources](https://github.com/XPRIZE/GLEXP-Team-KitkitSchool/tree/master/mainapp/Resources) folder with locale specific content.  
___
## Writing Board
Writing Board application uses images containing alphabets and words and the user is expected to trace them to learn to write. For localizing this application, the images containing alphabets and words needs to be replaced with locale specific images as follows:

  1.  Change the language at following places in _onCreate_ function of [MainActivity.java](https://github.com/XPRIZE/GLEXP-Team-KitkitSchool/blob/master/writingboard/app/src/main/java/com/enuma/writingboard/activity/MainActivity.java) file:  
          `mAppLanguage = extras.getString("LANGUAGE", "sw-TZ").toLowerCase();`  
          `mAppLanguage = "sw-tz";`
     
       For example, replace `"sw-TZ"` with `"hi-IN"` and `"sw-tz"` with `"hi-in"` for Hindi India language.  

  2.  Create a new folder in [assets/image/](https://github.com/XPRIZE/GLEXP-Team-KitkitSchool/tree/master/writingboard/app/src/main/assets/image) with locale specific language:
    
       For example, create a folder named _"hi-in"_ for Hindi India language.

  3.  Create locale specific images similar to the images available in [en-us](https://github.com/XPRIZE/GLEXP-Team-KitkitSchool/tree/master/writingboard/app/src/main/assets/image/en-us) folder and place them in the newly created locale specific folder.
___
## Library

  1. The Library application consists of a collection story books and videos that need to be localized:  
  		1. The story book comprises of images located at [library/localized](https://github.com/XPRIZE/GLEXP-Team-KitkitSchool/tree/master/library/localized)/en-us/assets/en-us.  
  		2. The videos are located at [library/localized](https://github.com/XPRIZE/GLEXP-Team-KitkitSchool/tree/master/library/localized)/en-us/res/raw.    
  2. Strings are being used in the Library to display names for various elements. These can be localized by changing string values at the following locations:   
  		1. Change the value for each element in [strings.xml](https://github.com/XPRIZE/GLEXP-Team-KitkitSchool/blob/master/library/app/src/main/res/values/strings.xml) file except for the following element:  
  `<string name="app_name">Kitkit School Library</string>`
  		2. Replace the value of `android:text` attribute of TextView with the localized text in [activity_bookdetail.xml](https://github.com/XPRIZE/GLEXP-Team-KitkitSchool/blob/master/library/app/src/main/res/layout/activity_bookdetail.xml).
___
## Blackboard - Not required
___
## Bookviewer - Not required
___
## Drawing - Not required
___
## Drum - Not required
___
## Marimba - Not required
___
## KitkitSchoolLogger - Not required
___
## Lock Screen - Not required
___
## Seaworld - Not required
___
## Voice Engine (for English only) - Does not support localization
___
