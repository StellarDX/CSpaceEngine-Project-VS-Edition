﻿# CSpaceEngine - A static library and toolbox for SpaceEngine

[中文](README_CN.md)

Using C++ to define, modify, complete Celestral Objects in SpaceEngine.

License: [GPLv2](https://www.gnu.org/licenses/old-licenses/gpl-2.0.html)

## NOTICE：
This project has been refactored and no longer update, [Click here](https://github.com/StellarDX/CSpaceEngine-Project) to enter the new repository.

## Examples
**Loading a sc file**
```loading file
ISCStream is = ParseFile("Catalog.sc");
Object obj = GetSEObject(is, "StarName");
```

**Writing object to file**
```writing file
ofstream fout("out.sc"); // Create a fstream. This will be used to construct sc file writer
OSCStream os(fout);
os << obj1 << obj2 << ...; // push objects
os.Write(); // export to file
```

**Time Support(Windows only)**
 * A Julian epoch calculator based on this library is provided, See [Here](Applications/JulianConvert/JulianCalculator.cpp) to get more info.

**Math functions(Based on STD Math)**
 * Expands of Trigonometric functions like chords, versines and exsecants.
 * Basic math functions for vecs
 * Solving polynomial equations
 * Algorithms from GLSL
 * A basic matrix class and linear algebra functions

**Star Luminosity calculations**
 * A Star Luminosity and Magnitude calculator based on this library is provided, See [Here](Applications/StarMagLumCalculator/StarMagLumCalculator.cpp) to get more info.

**Random Number Generator**
 * Based on std random, more easy to use.
 * A default random engine "random" is provided.

**Creating a star**
```generating
_CSE_Random_Engine<mt19937_64> Engine;
MainSequenceStarModel StarModel("G2V");
object_generator<_CSE_Random_Engine<mt19937_64>, MainSequenceStarModel> gen(Engine, StarModel);
Object Obj = gen();
```
 * Support stars:
 * - Main sequence stars
 * - Red Giant Branch
 * - Horizontal Branch(Red Clump Giants)
 * - Asymptotic giant branch(Super-AGB)
 * - Subdwarfs(Cool, O, B)
 * - Wolf-Rayet stars
 * - Subgiants
 * - Blue Giants
 * - Supergiants and hypergiants
 * - Carbon Stars
 * - White Dwarfs
 * - Substellar Objects

**Creating a planet**

Codes are almost same as above. Support rocky planets and gas giants.

An Asteroid belt creator C++ version is provided: [AsterBeltCreator](Applications/AsterBeltCreator/AsterBeltCreator.cpp)

**Orbit Calculating**
 * Some engines to generate distance array, Support Titius-Bode law, Blagg Fomula, Dermott's law and exponential fit.
 * TLE data parsing
 * calcualte Roche limit and Hill sphere, and make orbital bond to two objects.

## Compile and Install
Open the solution file and turn to the "ALL_BUILD" under "Targets", Run the project to complie.
The compiled libraries are under the "Libraries" folder.
If you want to merge all the module libraries into one library, Run "INSTALL" project.

## TODO
 * ...and more?
