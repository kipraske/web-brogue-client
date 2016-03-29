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

// This function is used for pausing, but also returns if true if there are
// input events to process
static boolean web_pauseForMilliseconds(short milliseconds)
{
  emscripten_sleep_with_yield(milliseconds);
  int input_ready = EM_ASM_INT_V({
    return (brogue.state.eventQueue.length > 0);
  });

  return input_ready;
}

#define PAUSE_BETWEEN_EVENT_POLLING		36//17

// This function is used to wait for new key or mouse events.
// It will not return until we have executed an event
static void web_nextKeyOrMouseEvent(rogueEvent *returnEvent, boolean textInput, boolean colorsDance)
{
  // because we will halt execution until we get more input, we definitely cannot have any dancing colors from the server side.
  //colorsDance = false;
  // TODO - implement color dancing, see tcod-platform...

  int input_ready = EM_ASM_INT_V({
    return (brogue.state.eventQueue.length > 0);
  });

  if (!input_ready){
    emscripten_sleep_with_yield(PAUSE_BETWEEN_EVENT_POLLING);
    web_nextKeyOrMouseEvent(returnEvent, textInput, colorsDance);
  }

  if (noMenu && rogue.nextGame == NG_NOTHING) rogue.nextGame = NG_NEW_GAME;

  // Get next event from queue
  EM_ASM({
    brogue.state.nextEvent = brogue.state.eventQueue.shift();
  });

  returnEvent->eventType = EM_ASM_INT_V({
    return brogue.state.nextEvent.eventType;
  });

  // param1 is the keyChar for Keystroke or xCoord for Mouse
  returnEvent->param1 = EM_ASM_INT_V({
    return brogue.state.nextEvent.param1;
  });

  if (returnEvent->eventType != KEYSTROKE){ //so a mouse event
    // param2 is y coordinate
    returnEvent->param2 = EM_ASM_INT_V({
      return brogue.state.nextEvent.param2;
    });
  }
  returnEvent->controlKey = EM_ASM_INT_V({
    return brogue.state.nextEvent.ctrlKey;
  });
  returnEvent->shiftKey = EM_ASM_INT_V({
    return brogue.state.nextEvent.shiftKey;
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
