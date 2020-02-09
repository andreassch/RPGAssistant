
# RPGAssistant: the role playing game master's assistance tool

This is a small tool to assist a role playing game master. It contains the following modules:

* Turn order list: Manage turn order and NPCs during combat.
* NPC Generator: Randomly generate non-player characters from different region, race, etc.
* Calendar: A calendar with moon phases, taylored to the system The Dark Eye.
  As the calendar computations are separated from the GUI, it can easily be adapted to other systems.
* more modules to come ...

## Compilation

The preferred way to build is using QtCreator. Just open rpgassistant.pro and click on build.

You can also configure and compile on the command line with the following steps:

    qmake
    make

Data files are currently not included. However, included are scripts to convert DSASuite's data files, which can be obtained from the web archive at
[https://web.archive.org/web/20010419203408/http://www.dirkoz.de/dsa/tools/bin/_daten.EXE](URL).
See also: [https://web.archive.org/web/20010307023059/http://www.dirkoz.de/dsa/tools/index.htm](URL)
