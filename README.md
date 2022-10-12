# CSpaceEngine - A static library and toolbox for SpeceEngine

Using C++ to define, modify, complete Celestral Objects in SpaceEngine.

[中文](README_CN.md)

Astronomical calculations is also supported(in future...).

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
 * A Julian epoch calculator based on this library is provided, See [Here](https://github.com/StellarDX/CSpaceEngine-Project/blob/main/CSE_Core/datetime/JulianCalculator.cpp) to get more info.

**Math functions(Based on STD Math)**
 * Expands of Trigonometric functions like chords, versines and exsecants.
 * Basic math functions for vecs
 * Solving polynomial equations
 * Algorithms from GLSL
 * A basic matrix class and linear algebra functions

**Star Luminosity calculations**
 * A Star Luminosity and Magnitude calculator based on this library is provided, See [Here](https://github.com/StellarDX/CSpaceEngine-Project/blob/main/CSE_Core/lumine/StarMagLumCalculator.cpp) to get more info.

**Random Number Generator**
 * Based on std random, more easy to use.
 * A default random engine "random" is provided.

**Generate a star**
```generating
_CSE_Random_Engine<mt19937_64> Engine;
MainSequenceStarModel StarModel("G2V");
object_generator<_CSE_Random_Engine<mt19937_64>, MainSequenceStarModel> gen(Engine, StarModel);
Object Obj = gen();
```
 * Support stars:
 * - Main sequence stars(Specified, Random)
 * - Wolf-Rayet star(Specified)
 * - Substellar Objects(Specified)
 * - Subgiants(Based on Mass, 0.4 - 12MSun Supported)

## TODO
 * Procedural planet generator
 * Orbit calculator and generator
 * ...and more
