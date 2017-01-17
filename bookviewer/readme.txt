

This mac app is for testing book configuration file for TodoSchool. 
It is roughly made, which means that the app will just crash when the data file has an error (e.g., wrong/missing file name)

The app can run anywhere, but the data file must be in ~/Documents/booktest/ folder. 

* todobooklist.txt

This file contains the list of folders for each book. 
Under each folder, there must be bookinfo.csv that describes media files and word list used in the book. 
Media files for each page/word need to be in page/word folder, respectively. 

* bookinfo.csv

This file is a CSV (comma seperated value) file. 
You can load it into excel/numbers and save as/export to CSV. 

Each row can be one of these three types: title, page, and word

The first column of each row specifies the type. 
Following columns of each type are...

title, [book title], [audio file for the title], [image file for the title]
page, [page number], [image file for the page]
sentence, [audio file for the sentence], [start time of the sentence in the page]
word, [start time of the word in the sentence audio], [end time of the word in the sentence audio], [word text], [audio file for the word, which is played when the word is touched], [length of the word audio file]

All the text/filenames are CASE SENSITIVE. 
Please be careful 

