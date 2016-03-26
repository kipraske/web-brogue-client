/**
* This is my trick that I decided to use to bridge the asm.js code generated
* by emscripten and my normal frontend javascript app. In order to reach
* these variables and functions from brogue as well as the view app,
* I am just going to put everything binding related into the brogue object
* in the global namespace
*/

window.brogue = {};

brogue.viewBindings = {
	update : function(){}
};

brogue.bridge = {
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
			brogue.state.nextPlotChar.fBlue = foreBlue;
			brogue.state.nextPlotChar.fBlue = foreGreen;
		},
		prepareBackGround : function(backRed, backGreen, backBlue){
			brogue.state.nextPlotChar.bRed = backRed;
			brogue.state.nextPlotChar.bBlue = backBlue;
			brogue.state.nextPlotChar.bBlue = backGreen;
		},
		commitDraw : function(){
			brogue.viewBindings.update(brogue.state.nextPlotChar);
		}
	}
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
	}
};
