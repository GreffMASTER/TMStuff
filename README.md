# TMStuff  
Multipurpose tool for TrackMania 1.0


## About
TMStuff allows for direct access to the GameBox internals inside TrackMania 1.0.  
It functions as a ImGui overlay.


## Features
- Nod browser
- Set values
- Create nods
- Load/Save Gbx files
- Custom ImGui Styles


## Installation
A patched version of TrackMania 1.0 is required. This tool will not work on any other executable.  
The compiled TMStuff DLL needs to be injected into TrackMania.exe during runtime using a third party tool, like Cheat Engine.  
Alternatively it is possible to modify TrackMania.exe import table and add the TMStuff DLL,  
or modify the TrackMania.exe bytecode to call `LoadLibraryA()`.


## Compile Requirements
- GCC Compiler
- DirectX 9 SDK


## Compile Instructions  
TODO