UltraGrid plugin for Unreal Engine 5
===========================================

Building
--------------------------

This plugin is meant to be used with the UltraGrid dynamic library build from the [library_build branch](https://github.com/mpiatka/UltraGrid/tree/library_build).

After building the library place the following files into the correct paths:
1. `libultragrid.dll` into `Binaries/ThirdParty/UltraGridLibrary/Win64`
2. All the dll dependencies obtained with the UltraGrid `get_dll_depends.sh` script into `Binaries/ThirdParty/UltraGridLibrary/Win64/Bundled`
3. `libultragrid.lib` into `Source/UltraGridLibrary/x64/Release`
4. Contents of the UltraGrid `src` directory into `Source/UltraGridLibrary/Public/libultragrid`


