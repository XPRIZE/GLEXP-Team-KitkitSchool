## Localization steps for all the apps

#### Launcher
1. Change the values for each element in the [strings.xml](https://github.com/XPRIZE/GLEXP-Team-KitkitSchool/blob/master/launcher/app/src/main/res/values/strings.xml) file except for the following element:  
  `<string name="app_name">Kitkit Launcher</string>`
___
#### Main App
1. Set locale in [configuration.xml](https://github.com/XPRIZE/GLEXP-Team-KitkitSchool/blob/master/kitkitschoollogger/kitkitlogger/src/main/res/values/configuration.xml) file.
2. Replace content in the [assets](https://github.com/XPRIZE/GLEXP-Team-KitkitSchool/tree/master/mainapp/proj.android-studio/models/src/main/assets) folder with locale specific content.
3. Replace content in [Resources](https://github.com/XPRIZE/GLEXP-Team-KitkitSchool/tree/master/mainapp/Resources) folder with locale specific content.
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
#### Library

  1.  [activity_bookdetail.xml](https://github.com/XPRIZE/GLEXP-Team-KitkitSchool/blob/master/library/app/src/main/res/layout/activity_bookdetail.xml)   
    Replace the value of the attribute of TextView, `android:text` with the localized text.  
  2.  [activity_main.xml](https://github.com/XPRIZE/GLEXP-Team-KitkitSchool/blob/master/library/app/src/main/res/layout/activity_main.xml)     
    Replace the values with the localized text.    
  3.  [activity_video_player.xml](https://github.com/XPRIZE/GLEXP-Team-KitkitSchool/blob/master/library/app/src/main/res/layout/activity_video_player.xml)    
    Replace the value of the attribute of Button, `android:text` with the localized text.    
  4.  [book_item.xml](https://github.com/XPRIZE/GLEXP-Team-KitkitSchool/blob/master/library/app/src/main/res/layout/book_item.xml)      
	  Replace the value of the attribute of TextView, `android:text` with the localized text.  
___
#### Blackboard - Not required
___
#### Bookviewer - Not required
___
#### Drawing - Not required
___
#### Drum - Not required
___
#### Marimba - Not required
___
#### KitkitSchoolLogger - Not required
___
#### Lock Screen - Not required
___
#### Seaworld - Not required
___
#### Voice Engine (for English only) - Does not support localization
___
