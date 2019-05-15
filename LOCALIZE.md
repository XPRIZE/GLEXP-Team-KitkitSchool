# Localization steps for KitkitSchool Applications
Last updated: May 13, 2019


## Overview

Kitkit School consists of several apps that work together to have kids learn basic literacy and numeracy skills.
Currently KitkitSchool supports English and Swahili, which can be extended to support more.
However, it is worth noting that simply translating the contents will not be enough.
It will require substantial effort and content modification to adapt to different language systems and cultures.
That being said, here is a guide for localization.


## Launcher
1. The Launcher application is used for launching applications in KitkitSchool world.


2. It also sets the default language (locale) that other apps will be using, defined as "defaultLanguage" in "configuration.xml" of each build variant.

en-US for English (/launcher/app/src/englishUS/res/values/configuration.xml)
sw-TZ for Swahili (/launcher/app/src/swahili/res/values/configuration.xml)

To add language build variant, modify /launcher/app/build.gradle and create a matching folder under /launcher/app/src/
For example, "hi-IN" for Hindi.
(And with all the apps, use "hi-IN" in place of "sw-TZ", "hi-in" for "sw-tz", and "hi" for "sw" respectively)

3. The strings used in the Launcher app is defined in
launcher/app/src/main/res/values/strings.xml (english, which is default) or
launcher/app/src/main/res/values-sw/strings.xml (swahili, overriden by Android according to the language setting)

Change the value for each element in strings.xml file or add a new strings.xml file in the additional language folder


___
## Main App
1. The Main App is the primary learning application in KitkitSchool with a tailored curriculum. This application consists of content for literacy and numeracy training.  
2. Each section of the application consists of section-specific audios, videos, and images. These assets can be localized as follows:  
    1. Modify LanguageManager (/mainapp/Classes/Managers/LanguageManager.[cpp,hpp]) to support new language locale
    2. Put the locale-specific content in the /mainapp/Resources/localized/[locale-code] folder.
        - Files in the locale-specific folder (/mainapp/Resource/localized/[locale-code]) will supersede the file with the same name in the general resource folder (/mainapp/Resources)
        - "curriculumdata.tsv" must exist in the locale-specific folder to be recognized by the app
    3. The PocketSphinx library is used for speech recognition. Languages supported by PocketSphinx can be found [here](https://cmusphinx.github.io/wiki/download/). Replace the contents in the [assets](https://github.com/XPRIZE/GLEXP-Team-KitkitSchool/tree/master/mainapp/proj.android-studio/models/src/main/assets) folder with a desired language.

___
## Bookviewer
The Bookviewer app reads storybooks aloud.
The Bookviewer refers to the resources in the main app folder (/mainapp/Resources/bookviewer  and /mainapp/Resources/localized/[locale]/games/books)
Thus, localization is not needed for this application as long as the Main App is set up accordingly.

___
## Library

  1. The Library application consists of a collection of storybooks and videos.  
      1. The list of the books with the metadata is stored in [library/localized/[locale]/assets/[locale]/library_book_data.tsv]
      2. The list of the videos with the metadata is stored in [library/localized/[locale]/assets/[locale]/library_video_data.tsv]
  		3. The thumbnail images of the storybooks and the vidoes are located at [library/localized/[locale]/assets/[locale]]
  		4. The videos are located at [library/localized/[locale]/res/raw]
      Replace these files with locale-specific content.  
      * The Bookviewer app will be launched when a book is selected

  2. The strings used in the Library app are defined in library/app/src/main/res/values[-language]/strings.xml. Change the value for each element in strings.xml file or add a new strings.xml file in the additional language folder

___
## Writing Board
The Writing Board application uses images containing alphabets and words. Users trace the images to learn to write. To localize this application, the images containing alphabets and words must be replaced with locale-specific images as follows:

  1.  Create a new folder in [/writingboard/app/src/main/assets/image] with locale-specific language:
       For example, create a folder named _"hi-in"_ for the Hindi language.

  2.  Create locale-specific images similar to the images available in the [en-us](https://github.com/XPRIZE/GLEXP-Team-KitkitSchool/tree/master/writingboard/app/src/main/assets/image/en-us) folder and place them in the newly created locale-specific folder.

___
## Drawing

Drawing is an app where a child can draw on a digital paper using crayons.

The strings used in the Drawing app (mainly the names of colors) are defined in launcher/app/src/main/res/values[-language]/strings.xml
Change the value for each element in strings.xml file or add a new strings.xml file in the additional language folder

___
## Lock Screen
The Lock Screen app appears when the device wakes up. The device will be unlocked by touching the KitkitSchool icon.

The strings used in the Lock Screen app are defined in launcher/app/src/main/res/values[-language]/strings.xml
Change the value for each element in strings.xml file or add a new strings.xml file in the additional language folder


___
## Drum - Not required
Drum is an app where a child can tap on the screen to hear a drum sound. Localization is not needed for this application.
___
## Blackboard - Not required
Blackboard is an app where the child can draw or write on a blackboard using a chalk. Localization is not needed for this application.  
___
## Marimba - Not required
Marimba is an app where the child can tap on the screen to hear a marimba sound. Localization is not needed for this application.
___
## Seaworld - Not required
Seaworld is an app where various fish appear on the screen. Localization is not needed for this application.  
___
## KitkitSchoolLogger - Not required
KitkitSchoolLogger is not a stand-alone app, but the code shared among all the applications to log details and handle shared data. Localization is not needed for this application.
___
## Voice Engine (for English only) - Does not support localization
Voice Engine is used by "Word Note" activity in the Main App. This app does not support localization.  
