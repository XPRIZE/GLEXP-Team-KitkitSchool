### Localization steps for all the apps
___
#### Launcher
1. Change the values for each element in the [strings.xml](https://github.com/XPRIZE/GLEXP-Team-KitkitSchool/blob/master/launcher/app/src/main/res/values/strings.xml) file except for the following element:  
  `<string name="app_name">Kitkit Launcher</string>`
___
#### Main App
1. GLEXP-TodoSchool\kitkitschoollogger\kitkitlogger\src\main\res\values\configuration.xml
2. Replace content in the existing asset folder with locale specific content
___
#### Book Viewer
___
#### Writing Board
1.  Change the language at following places in _onCreate_ function of [MainActivity.java](https://github.com/XPRIZE/GLEXP-Team-KitkitSchool/blob/master/writingboard/app/src/main/java/com/enuma/writingboard/activity/MainActivity.java) file:  
   `mAppLanguage = extras.getString("LANGUAGE", "sw-TZ").toLowerCase();`  
   `mAppLanguage = "sw-tz";` 
     
    For example, replace `"sw-TZ"` with `"hi-IN"` for Hindi India language.  

2.  Create a new folder in [assets/image/](https://github.com/XPRIZE/GLEXP-Team-KitkitSchool/tree/master/writingboard/app/src/main/assets/image) with locale specific language.
    
    For example, create a folder named _"hi-in"_ for Hindi India language.

3.  Create locale specific images similar to the images available in [en-us](https://github.com/XPRIZE/GLEXP-Team-KitkitSchool/tree/master/writingboard/app/src/main/assets/image/en-us) folder and place them in the newly created locale specific folder.
___
#### Sea World
___
#### Library
___
#### KitkitSchoolLogger - NA
___
#### Voice Engine (for English only) - NA
___
#### Lock Screen - NA
___
#### Blackboard - NA
___
#### Drawing - NA
___
#### Drum - NA
___
#### Marimba - NA
___
