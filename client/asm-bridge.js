/**
* This is my trick that I decided to use to bridge the asm.js code generated
* by emscripten and my normal frontend javascript app. In order to reach
* these variables and functions from brogue as well as the view app,
* I am just going to put everything binding related into the brogue object
* in the global namespace.
*
* The brogue.bridge object is used in our brogue/src/platform/web-platform.c
* to connect the c code with the javascript views
*/

window.brogue = {};

brogue.viewBindings = {
	updateCell : function(){}
};

brogue.bridge = {
	// used in web_plotChar to send console cell update information to the view
	plotChar : {
		// can only pass up to 8 arguments at a time, so we split up these
		// functions into these three parts and recombine here
		prepareLoc : function (xLoc, yLoc) {
			brogue.state.nextPlotChar.x = xLoc;
			brogue.state.nextPlotChar.y = yLoc;
		},
		prepareForeGround : function(inputChar, foreRed, foreGreen, foreBlue){
			brogue.state.nextPlotChar.char = inputChar;
			brogue.state.nextPlotChar.fRed = foreRed;
			brogue.state.nextPlotChar.fGreen = foreGreen;
			brogue.state.nextPlotChar.fBlue = foreGreen;
		},
		prepareBackGround : function(backRed, backGreen, backBlue){
			brogue.state.nextPlotChar.bRed = backRed;
			brogue.state.nextPlotChar.bGreen = backGreen;
			brogue.state.nextPlotChar.bBlue = backBlue;
		},
		commitDraw : function(){
			brogue.viewBindings.updateCell(brogue.state.nextPlotChar);
		}
	},
	// Set input event information. Checked in web_pauseForMilliseconds
	sendInput: {
		keypress: function(eventCharCode, keyCode, ctrlKey, shiftKey){
			brogue.state.nextKeyOrMouseReady = true;
			brogue.state.nextKeyEvent = {
				eventCode: eventCharCode,
				keyCode: keyCode,
				ctrlKey: ctrlKey,
				shiftKey: shiftKey
			};
		},
		mouse: function(eventCharCode, xCoord, yCoord, ctrlKey, shiftKey){
			brogue.state.nextKeyOrMouseReady = true;
			brogue.state.nextMouseEvent = {
				eventCode: eventCharCode,
				x: xCoord,
				y: yCoord,
				ctrlKey: ctrlKey,
				shiftKey: shiftKey
			};
		}
	},
	// Reads the key event information into the c code from web_nextKeyOrMouseEvent
	processInput: {}
};

brogue.state = {
	nextPlotChar : {
		char : 0,
		x : 0,
		y : 0,
		fRed : 0,
		fGreen : 0,
		fBlue : 0,
		bRed : 0,
		bGreen : 0,
		bBlue : 0
	},
	nextKeyOrMouseReady : false,
	nextKeyEvent : {
		eventCode: 0,
		keyCode: 0,
		ctrlKey: 0,
		shiftKey: 0
	},
	nextMouseEvent : {
		eventCode: 1,
		x: 0,
		y: 0,
		ctrlKey: 0,
		shiftKey: 0
	}
};
