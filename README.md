# SpaceEngine SDK(CSpaceEngine) static library for C++

Using C++ to define, modify, complete Celestral Objects in SpaceEngine.

Astronomical calculations is also supported(in future...).

## Examples
**Loading a sc file**
```asp
ISCStream is = ParseFile("Catalog.sc");
Object obj = GetSEObject(is, "StarName");
```

**Writing object to file**
```asp
ofstream fout("out.sc"); // Create a fstream. This will be used to construct sc file writer
OSCStream os(fout);
os << obj1 << obj2 << ...; // push objects
os.Write(); // export to file
```

**Time Support(Windows only)**
 * A Julian epoch calculator based on this library is provided, See [Here](https://github.com/StellarDX/CSpaceEngine-Project/blob/main/CSE_Core/datetime/JulianCalculator.cpp) to get more info.

## TODO
 * Some math functions
 * Analyzing Star's spectal type
 * Rotation models calculations
 * Orbit models calculations
 * ...and more
