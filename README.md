Web Brogue Client
==================

An attempt to port Brogue to javascript to be played on the client. Brogue is a game for Mac OS X, Windows, and Linux by Brian Walker.  For more information go https://sites.google.com/site/broguegame/.

The current plan is to compile the original Brogue into asm.js using [emscripten](https://kripken.github.io/emscripten-site/index.html) and then build hooks into the existing output client that we built for the web-brogue project. The idea here is to create a more robust client which doesn't rely on massive amounts of server data being sent over the pipeline to update.

This may or may not work, but I have a promising design at the moment, and with the first commit of this repository officially got brogue to compile to javascript and accept I/O asynchronously.


Upgrading Brogue
------------------------------

To upgrade brogue, grab the latest version of the brogue source code from https://sites.google.com/site/broguegame/ for linux and update the platform code to include my changes.  I have intentionally tried to keep my updates separate from the original brogue game logic as much as possible so my updates are limited to the following 4 files:

* Add platform/web-platform.c
* Update platform.h to define webConsole
* Update platform/main.c to set the currentConsole to webConsole if we are compiling with web
* Update the makefile

Future updates to brogue will likely not prevent any of these updates from being added, though care must be given if the platform-dependant logic changes for some reason.
