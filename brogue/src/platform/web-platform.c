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

  // Reprocess colors to be on scale of 0-255 instead of 0-100
  foreRed = foreRed * 255 / 100;
  foreGreen = foreGreen * 255 / 100;
  foreBlue = foreBlue * 255 / 100;
  backRed = backRed * 255 / 100;
  backGreen = backGreen * 255 / 100;
  backBlue = backBlue * 255 / 100;

  // EM_ASM_ARGS only allows a max of 8 arguments. So we need to split these up
  EM_ASM_ARGS({
    brogue.bridge.plotChar.prepareLoc($0, $1);
  }, xLoc, yLoc);

  EM_ASM_ARGS({
    brogue.bridge.plotChar.prepareForeGround($0, $1, $2, $3);
  }, inputChar, foreRed, foreGreen, foreBlue);

  EM_ASM_ARGS({
    brogue.bridge.plotChar.prepareBackGround($0, $1, $2);
  }, backRed, backGreen, backBlue);

  EM_ASM({
    brogue.bridge.plotChar.commitDraw();
  });

}

// This function is used both for checking input and pausing
static boolean web_pauseForMilliseconds(short milliseconds)
{
  emscripten_sleep_with_yield(milliseconds);
  int input_ready = EM_ASM_INT_V({
    return brogue.state.nextKeyOrMouseReady;
  });

  return input_ready;
}

static void web_nextKeyOrMouseEvent(rogueEvent *returnEvent, boolean textInput, boolean colorsDance)
{
  // because we will halt execution until we get more input, we definitely cannot have any dancing colors from the server side.
  colorsDance = false;

  // We must avoid the main menu, so we spawn this process with noMenu, and quit instead of going to the menu
  if (noMenu && rogue.nextGame == NG_NOTHING) rogue.nextGame = NG_QUIT;

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
