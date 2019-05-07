# Localization steps for KitkitSchool Applications
Last updated: May 6, 2019
## Launcher
1. The Launcher application is used for launching applications in KitkitSchool world.
2. The [strings.xml](https://github.com/XPRIZE/GLEXP-Team-KitkitSchool/blob/master/launcher/app/src/main/res/values/strings.xml) file defines the strings being used in the Launcher to display names for various elements. Change the value for each element in strings.xml file except for the following element:  
    `<string name="app_name">Kitkit Launcher</string>`
___
## Main App
1. The Main App is the primary learning application in KitkitSchool with a tailored curriculum. This application consists of content for literacy and numeracy training.  
2. Each section of the application consists of section-specific audios, videos, and images. These assets can be localized as follows:  
    1. Set locale in the [configuration.xml](https://github.com/XPRIZE/GLEXP-Team-KitkitSchool/blob/master/kitkitschoollogger/kitkitlogger/src/main/res/values/configuration.xml) file.  
    2. The PocketSphinx library is used for speech recognition. Languages supported by PocketSphinx can be found [here](https://cmusphinx.github.io/wiki/download/). Replace the contents in the [assets](https://github.com/XPRIZE/GLEXP-Team-KitkitSchool/tree/master/mainapp/proj.android-studio/models/src/main/assets) folder with a desired language.
    3. Replace content in the [Resources](https://github.com/XPRIZE/GLEXP-Team-KitkitSchool/tree/master/mainapp/Resources) folder with locale-specific content.  
___
## Writing Board
The Writing Board application uses images containing alphabets and words. Users trace the images to learn to write. To localize this application, the images containing alphabets and words must be replaced with locale-specific images as follows:

  1.  Change the language in following places in the _onCreate_ method of the [MainActivity.java](https://github.com/XPRIZE/GLEXP-Team-KitkitSchool/blob/master/writingboard/app/src/main/java/com/enuma/writingboard/activity/MainActivity.java) file:  
          `mAppLanguage = extras.getString("LANGUAGE", "sw-TZ").toLowerCase();`  
          `mAppLanguage = "sw-tz";`
     
       For example, replace `"sw-TZ"` with `"hi-IN"` and `"sw-tz"` with `"hi-in"` for the Hindi language.  

  2.  Create a new folder in [assets/image/](https://github.com/XPRIZE/GLEXP-Team-KitkitSchool/tree/master/writingboard/app/src/main/assets/image) with locale-specific language:
    
       For example, create a folder named _"hi-in"_ for the Hindi language.

  3.  Create locale-specific images similar to the images available in the [en-us](https://github.com/XPRIZE/GLEXP-Team-KitkitSchool/tree/master/writingboard/app/src/main/assets/image/en-us) folder and place them in the newly created locale-specific folder.
___
## Library

  1. The Library application consists of a collection of storybooks and videos.  
  		1. A storybook is comprised of images located at [library/localized](https://github.com/XPRIZE/GLEXP-Team-KitkitSchool/tree/master/library/localized)/en-us/assets/en-us.  
  		2. The videos are located at [library/localized](https://github.com/XPRIZE/GLEXP-Team-KitkitSchool/tree/master/library/localized)/en-us/res/raw.  
        
      Replace these files with locale-specific content.  
  2. Strings are used in the Library to display names for various elements. To localize the strings, change the string values at the following locations:   
  		1. Change the value for each element in [strings.xml](https://github.com/XPRIZE/GLEXP-Team-KitkitSchool/blob/master/library/app/src/main/res/values/strings.xml) file except for the following element (i.e., the app title):  
  `<string name="app_name">Kitkit School Library</string>`
  		2. Navigate to the [activity_bookdetail.xml](https://github.com/XPRIZE/GLEXP-Team-KitkitSchool/blob/master/library/app/src/main/res/layout/activity_bookdetail.xml) file. Replace the value of `android:text` attribute in TextView element with the localized text.  
___
## Blackboard - Not required
Blackboard is an app where the child can draw or write on a blackboard using a chalk. Localization is not needed for this application.  
___
## Bookviewer - Not required
Bookviewer app reads storybooks aloud. Localization is not needed for this application.  
___
## Drawing - Not required
Drawing is an app where a child can draw on a digital paper using crayons. Localization is not needed for this application.  
___
## Drum - Not required
Drum is an app where a child can tap on the screen to hear a drum sound. Localization is not needed for this application.
___
## Marimba - Not required
Marimba is an app where the child can tap on the screen to hear a marimba sound. Localization is not needed for this application. 
___
## KitkitSchoolLogger - Not required
KitkitSchoolLogger is used for logging details from all the applications. Localization is not needed for this application.
___
## Lock Screen - Not required
The Lock Screen app appears when the device wakes up. The device will be unlocked by touching the KitkitSchool icon. Localization is not needed for this application.
___
## Seaworld - Not required
Seaworld is an app where various fish appear on the screen. Localization is not needed for this application.  
___
## Voice Engine (for English only) - Does not support localization
Voice Engine is used by "Word Note" activity in the Main App. This app does not support localization.  
