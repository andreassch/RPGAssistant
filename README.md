# RPGAssistant: the role playing game master's assistance tool

This is a small tool to assist a role playing game master. It contains the following modules:

* Turn order list: Manage turn order and NPCs during combat.
* NPC Generator: Randomly generate non-player characters from different regions.
* Calendar: A calendar with moon phases and holidays. Calendar computations are separated from the GUI, thus the use for different settings and worlds is possible. The calendar also contains a diary.
* Throw dices.
* more modules to come ...

RPGAssistant is partly inspired by Dirk Oetmann's DSASuite from the early 2000s ([archived homepage](https://web.archive.org/web/20010307023059/http://www.dirkoz.de/dsa/tools/index.htm)).


## License information

This program is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, version 3.

This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more details.

This program uses the open source version of the Qt toolkit. The source code and pre-built versions of Qt can be obtained from www.qt.io.

Data and 3rd party material that you may use in combination with this program are not part of the program itself and thus do NOT fall under the GNU General Public License. You use such 3rd party materials solely under your own responsibility.


## Compilation

1. Select in `rpgassistant.pro` for which roleplaying system you want to build.

2. Build the program.

      a. The preferred way is by using QtCreator. Just open `rpgassistant.pro` and click on build.

      b. You can also configure and compile on the command line with the following steps:

```
          qmake
          make
```


## Data files

The software needs data files with lists of names, holidays, etc. These files depend on the role playing system you want to use. Due to legal issues, such files are not included in this package.

### The Dark Eye/Aventuria, German version (Das Schwarze Auge/Aventurien)

This program can use the data files from Dirk Oetmann's DSASuite. These can be obtained from [the web archive](https://web.archive.org/web/20010419203408/http://www.dirkoz.de/dsa/tools/bin/_daten.EXE). The self-extracting zip archive can also be extracted with unzip. Unpack it to a temporary folder inside the data folder. Under Linux:
```
  mkdir DSASuite
  unzip -d DSASuite _daten.EXE
```
The ages in Zwerge.prs seem to be erroneous, I corrected the numbers in the 3rd and 4th line (minimal and maximal age) to 20 and 400, respectively, and the numbers in the 9th and 10th line (age limits for youngster and elderly person) to 40 and 300, respectively.

Some name lists do not contain surnames as these are constructed from a parent's name in some cultures, e.g. Thorwal or among dwarves. To handle that, I have made an extension to DSASuite's name lists. The conversion script support star-prefixed keywords in the `---Nachnamen---` section of DSASuite's `.dat` files. In `DSASuite/Namen/Thorwal.dat` you can add the following lines after `---Nachnamen---`:
```
*postfixM son
*postfixW dottir
```
In `DSASuite/Namen/Zwerge.dat` you can add the following lines after `---Nachnamen---`:
```
*prefixM groscho 
*postfixM in
*prefixW groschna 
*postfixW xin
```
Mind the traling space after `groscho` and `groschna`.

Now run the conversion scripts:
```
  python dsasuitenames2xml.py DSASuite/Namen >names.xml
  python dsasuiteprs2xml.py DSASuite/Daten >persons.xml
  python dsasuiteholidays2xml.py DSASuite/Daten/feiertage.dat >holidays.xml
```
Since I didn't figure out DSASuite's format in its entirety, you have to correct two entries in `holidays.xml` after conversion: "Immanmeisterschaften" and "Sklavenauktion in Al'Anfa" begin at the second weekday in the month, thus you have to set the `number` attribute of the `day` tag to `2` for these two entries.

The desktop version of RPGAssistant expects the data files in the same folder as the executable. For the Android version they are packed into the APK.

The Dark Eye, its respective logos, as well as Aventuria, Dere, Myranor, Riesland, Tharun and Uthuria, and their respective logos, are trademarks of Ulisses Spiele GmbH, Waldems.


## Customisation

RPGAssistant can be adapted to different role playing systems To facilitate that, the UI has been separated from the backend as much as possible. There are two system-dependent modules:
1. Calendar computations. Classes Calendar, Date and MoonPhase and enums Month and Reckoning are expected. Please refer to the implementations for The Dark Eye in TDECalendar for details. Include the new module in CalendarWidget and use its namespace.
2. Computation of modifiers due to bad health. Expected is a function computeModifier which takes current hit points and total hit points as parameters and returns a modifier. Please refer to the TDEModifier module for details.


## Author

This program has been written by Andreas Schneider (andreas underscore schn at web dot de).
