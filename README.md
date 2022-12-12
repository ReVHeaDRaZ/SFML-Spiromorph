# SFML-SpiroMorph
Spiromorph - A port of [mickjc750/Spiromorph](https://github.com/mickjc750/Spiromorph) to SFML C++ Rewritten as a class (Can spawn multiple Spiromorphs) with bloom shader effects, HUD and runtime control of parameters

# Controls

|  |  |
| ------------- | ------------- |
| <b>LMB</b> | Spawn a Spiromorph |
| <b>UP and DOWN-</b> | Select Spiromorph |
| <b>PAGEUP and PAGEDOWN-</b> | Change Speed |
| <b>HOME and END-</b> | Change Amount of Envelopes In Phase|
| <b>INS and DEL-</b> | Change Amplitude|
| <b>H-</b> | HUD ON/OFF |
  
# Executable for WIN32
Just download the release, unzip and run SFML-Spiromorph.exe
To see command line options run:
```
SFML-Spiromorph --help
```
# Building Source
Written in VSCode setup with seperate debug and production release building.  SFML and MinGW32 need to be installed in default root directories
unless you want to edit the paths in .vscode.
