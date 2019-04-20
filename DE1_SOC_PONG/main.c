#include "DE1Soc_VGA/DE1SoC_VGA.h"
#include "pongDisplay/pongDisplay.h"
#include "pongEngine/pongSprites.h"
#include "pongEngine/pongEngine.h"
#include "pongInputs/pongInputs.h"
#include "pongSound/pongSound.h"
#include "pongScreens/pongScreens.h"
#include "HPS_Watchdog/HPS_Watchdog.h"


int main(void) {
	int FS = 3;
	int paddleX;

	// Initialise displays
	Displays_init(0xC8000000,0xC9000000,0xFF200060,0xFF200080);
	Displays_mode(0);
	Displays_frameSkip(FS);
	ResetWDT();

    // Initialise keyboard/pushbuttons
	inputsInitialise();

	// Initialise sounds
	pongSound_Init();

	// Run loading screen
	startScreen();

	while(1){
		// Run start menu
		gameMenu();
		// Run test screen
		testScreen();
	}
}
