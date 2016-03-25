#ifdef BROGUE_WEB

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <emscripten.h>

#include "platform.h"

extern playerCharacter rogue;

static void gameLoop()
{
    rogueMain();
}

static void web_plotChar(uchar inputChar,
			  short xLoc, short yLoc,
			  short foreRed, short foreGreen, short foreBlue,
			  short backRed, short backGreen, short backBlue) {

EM_ASM_ARGS({
  console.log('I got (x,y): ' + [$0, $1]);
}, xLoc, yLoc);

}

static void sendStatusUpdate(){
  EM_ASM({
    console.log('status update!');
  });
}

// This function is used both for checking input and pausing
    static boolean web_pauseForMilliseconds(short milliseconds)
{
    emscripten_sleep(milliseconds);
    EM_ASM({
      console.log('checking for input...');
    });
}

static void web_nextKeyOrMouseEvent(rogueEvent *returnEvent, boolean textInput, boolean colorsDance)
{
    // because we will halt execution until we get more input, we definitely cannot have any dancing colors from the server side.
    colorsDance = false;

    // We must avoid the main menu, so we spawn this process with noMenu, and quit instead of going to the menu
    if (noMenu && rogue.nextGame == NG_NOTHING) rogue.nextGame = NG_QUIT;

    // Send a status update of game variables we want on the client
    sendStatusUpdate();

    EM_ASM({
      console.log('injecting input into brogue variables...');
    });

}

static void web_remap(const char *input_name, const char *output_name) {
    // Not needed
}

static boolean modifier_held(int modifier) {
    // Not needed, I am passing in the modifiers directly with the event data
	return 0;
}

struct brogueConsole webConsole = {
	gameLoop,
	web_pauseForMilliseconds,
	web_nextKeyOrMouseEvent,
	web_plotChar,
	web_remap,
	modifier_held
};

#endif
