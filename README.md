Web Brogue Client
==================

An attempt to port Brogue to javascript to be played on the client. Brogue is a game for Mac OS X, Windows, and Linux by Brian Walker.  For more information go https://sites.google.com/site/broguegame/.

The current plan is to compile the original Brogue into asm.js using [emscripten](https://kripken.github.io/emscripten-site/index.html) and then build hooks into the existing output client that we built for the web-brogue project. The idea here is to create a more robust client which doesn't rely on massive amounts of server data being sent over the pipeline to update.

This may or may not work, but I have a promising design at the moment, and with the first commit of this repository officially got brogue to compile to javascript and accept I/O asynchronously.

Proof Of Concept Results
-------------------------
The current branch has the completed proof of concept for using emscripten to compile brogue. It is compiled in the repository so you don't have to worry so much about that at the moment. Since it is javascript here it will work on any server. To try it, all you have to do is put the client folder on the server and navigate to index.js.

Observe that we do in fact have nice dancing colors as well as a pretty functional homepage. However I/O is quite laggy which is unfortunate. We may be able to improve this experience yet, but this is a good proof of concept for a first release. It is clear from the performance testing results that this lag is coming from the DOM-updating-javascript blocking the main loop of the compiled brogue javascript. With our web server, these two pieces different processes with the brogue game whirring away on the server and the javascript solely responsible for processing HTML and outputting information so they couldn't interfere with one another.

We can probably help this by doing one or all of the following:

* Make the visual output more efficent. The DOM is notorious for being a resource hog.
* Give the brogue engine priority over the backbone by re-implementing some sort of frame-update-queue using ByteArrays rather than rendering a single cell at a time.
* Figure out how to whitelist less files for EMTERPRETIFY. Functions ran this way are much slower than without. Theoretically we only need those files which call `emscripten_sleep_with_yield`, but I get a weird error if I try that.
* Get libtcod compiled in emscripten and let the internal SDL implementation take care of everything. This of course would be the best course of action, but I have established it is horribly difficult (and probably would require implementing emscripten SDL functions in that project).

Compiling Brogue with emscripten
--------------------------------
Download and install the emscripten sdk. Then all we need to do is

`emmake make web` to compile the asm.js
`./copy.sh` to move the output files to the client folder

Upgrading Brogue
------------------------------

To upgrade brogue, grab the latest version of the brogue source code from https://sites.google.com/site/broguegame/ for linux and update the platform code to include my changes.  I have intentionally tried to keep my updates separate from the original brogue game logic as much as possible so my updates are limited to the following 4 files:

* Add platform/web-platform.c
* Update platform.h to define webConsole
* Update platform/main.c to set the currentConsole to webConsole if we are compiling with web
* Update the makefile

Future updates to brogue will likely not prevent any of these updates from being added, though care must be given if the platform-dependant logic changes for some reason.
