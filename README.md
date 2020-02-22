# RPGAssistant: the role playing game master's assistance tool

This is a small tool to assist a role playing game master. It contains the following modules:

* Turn order list: Manage turn order and NPCs during combat.
* NPC Generator: Randomly generate non-player characters from different region, race, etc.
* Calendar: A calendar with moon phases and holidays, taylored to the system The Dark Eye.
  As the calendar computations are separated from the GUI, it can easily be adapted to other systems.
* more modules to come ...

RPGAssistant is inspired by Dirk Oetmann's DSASuite from the early 2000s ([archived homepage](https://web.archive.org/web/20010307023059/http://www.dirkoz.de/dsa/tools/index.htm)).


## License information

The software itself is licensed under the GNU General public license version 3.

Data are yet included due to unknown legal status. RPGAssistant's data files are based on those by DSASuite. Provided are scripts to convert these data files. The original author of these files, Dirk Oetmann, gives permission to use and modify his files for non-commercial purposes, provided that they are used under a different project name, see [webpage](https://web.archive.org/web/20091012042038/http://www.dirkoz.de/downloads.php).


## Compilation

The preferred way to build is by using QtCreator. Just open rpgassistant.pro and click on build.

You can also configure and compile on the command line with the following steps:

```
  qmake
  make
```

## Data files

This program can use the data files from Dirk Oetmann's DSASuite. These can be obtained from [the web archive](https://web.archive.org/web/20010419203408/http://www.dirkoz.de/dsa/tools/bin/_daten.EXE). The self-extracting zip archive can also be extracted with unzip. Unpack it to a temporary folder inside the data folder. Under Linux:
```
  mkdir DSASuite
  unzip -d DSASuite _daten.EXE
```
Now run the conversion scripts:
```
  python dsasuitenames2xml.py DSASuite/Namen >names.xml
  python dsasuiteprs2xml.py DSASuite/Daten >persons.xml
  python dsasuiteholidays2xml.py DSASuite/Daten/feiertage.dat >holidays.xml
```
The ages in Zwerge.prs seem to be erroneous, I corrected the numbers in the 3rd and 4th line (minimal and maximal age) to 20 and 400, respectively, and the numbers in the 9th and 10th line (age limits for youngster and elderly person) to 40 and 300, respectively. Moreover, since I didn't figure out DSASuite's format in its entirety, you have to correct two entries in `holidays.xml` after conversion: "Immanmeisterschaften" and "Sklavenauktion in Al'Anfa" begin at the second weekday in the month, thus you have to set the `number` attribute of the `day` tag to `2` for these two entries.

Some name lists do not contain surnames as these are constructed from a parent's name in some cultures, e.g. Thorwal or among dwaves. To handle that, I have made an extension to DSASuite's name lists. The conversion script support star-prefixed keywords in the `---Nachnamen---` section of DSASuite's `.dat` files. In `DSASuite/Namen/Thorwal.dat` you can add the following lines after `---Nachnamen---`:
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
Mind the traling space after `groscho` and `groschna`. Now rerun the conversion script for the names.

The desktop version of RPGAssistant expects the data files in the same folder as the executable. For the Android version they are packed into the APK.


## Customisation

RPGAssistant can be adapted to role playing systems other than The Dark Eye (or Das Schwarze Auge in German). To facilitate that, the UI has been separated from the backend that depends on the system as much as possible. You are welcome to share extensions that you have created.


## Author

This program is written by Andreas Schneider (andreas underscore schn at web dot de).
