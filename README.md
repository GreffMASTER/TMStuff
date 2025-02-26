# TMStuff  
Multipurpose tool for TrackMania 1.0
![TMStuffScreenshot](https://github.com/user-attachments/assets/fd05815a-d2ae-43a7-a433-e6359ec773dd)

## About
TMStuff allows for direct access to the GameBox internals inside TrackMania 1.0.  
It functions as a ImGui overlay.


## Features
- Nod browser
- Modify values
- Create nods
- Load/Save Gbx files
- Custom ImGui styles


## Installation
A StarForce patched version of TrackMania 1.0 is required, specifically patch by iMMERSION. This tool will not work on any other executable.  
The compiled TMStuff DLL needs to be injected into TrackMania.exe during runtime using a third party tool, like Cheat Engine.  
Alternatively it is possible to modify TrackMania.exe import table and add the TMStuff DLL,  
or modify the TrackMania.exe bytecode to call `LoadLibraryA()`.


## Usage  
When running TMStuff for the first time, a menu bar should be visible at the top of the screen.  
You can toggle the ImGui overlay by pressing the F3 key.  
To open the main TrackMania nod window, click on `Windows`->`TrackMania Nod`.  
  
To open a new dynamic nod window, click on `Windows`->`New Window`.  
Inside the custom window, you can input memory address of a desired nod and click `Lookup` to see the nod contents.  
You can copy address of a nod by clicking on a `Copy` button next to a nod item inside the browser.  


## Compile Requirements
- GCC Compiler (32-bit, this is important!)
- DirectX 9 SDK


## Compile Instructions  
TODO
