# DMPY - A MicroPython port for the SwissMicros DMCP platform

This is a port of MicroPython to the SwissMicros DMCP platform.  
It is still in its early stages, so don't expect too much of it yet. In particular, there is no Python prompt (REPL) yet, and probably won't be for a while.

So, how does it work then?
* Copy dmpy.pgm and your python file(s) to your calculator's Filesystem
* Use "Load Program" in the DMCP menu to load dmpy.pgm
* Run Program

When run, DMPY will try to load and execute the file "main.py". When the script ends, DMPY ends, and you are back to the DMCP menu. When an uncaught exception occurs, you will be presented with the exception screen. Then, DMPY ends, and you are back to the DMCP menu. And that's currently all there is to it.

In your python code, you can use the modules dmcp and dmpy:  
* The dmcp module tries to represent the DMCP C API as closely as possible. It is far from complete yet.  
* The dmpy module contains helper functions that I found useful to have, but that are not strictly part of the DMCP API.

## Current status
### What's working already

* General
    * Loading and running main.py from filesystem
    * Importing modules from filesystem
    * Handling uncaught exceptions
* dmcp module
    * Basic graphics functions
    * Basic keyboard functions
* dmpy module
    * basic text output helpers

### TODO
* file access using open()
* more dmcp api functions
* time module

### Nice to have, maybe later
* Python prompt (REPL) via USB-Serial
