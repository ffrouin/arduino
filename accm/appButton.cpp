#include "appButton.h"

appButton::appButton(int pin, int mode) : ezButton(pin, mode) {	
	pressedTimeLast = 0;
	pressedTime = 0;
	releasedTime = 0;
	trigger = false;
	level = 0;
}

bool appButton::doublePress(void) {
	if (level == 4) {
		level = 0;
		return(true);
	}
	return(false);
}

bool appButton::shortPress(void) {
	if (level == 1) {
		level = 0;
		return(true);
	}
	return(false);
}

bool appButton::longPress(void) {
	if (level == 2) {
		level = 0;
		return(true);
	}
	return(false);
}

bool appButton::veryLongPress(void) {
	if (level == 3) {
		level = 0;
		return(true);
	}
	return(false);
}

void appButton::loop(void) {
	ezButton::loop();
	
	if(ezButton::isReleased())  {      // button is pressed
		pressedTimeLast = pressedTime;
		pressedTime = millis();
		level = 0;
	} else if(ezButton::isPressed()) { // button is released
		releasedTime = millis();
		
		trigger = true;
	}
	
	if (trigger) {
		long pressDuration = releasedTime - pressedTime;
		long doublePressDuration = releasedTime - pressedTimeLast;

		if (doublePressDuration < BTN_DOUBLE_PRESS_THRESHOLD) {
			level = 4;
		} else if ( pressDuration < BTN_DOUBLE_PRESS_THRESHOLD ) {
			level = 0;
		} else if ( pressDuration < BTN_PRESS_THRESHOLD1 ) {
			level = 1;
		} else if (pressDuration < BTN_PRESS_THRESHOLD2 ) {
			level = 2;
		} else {
			level = 3;
		}
		trigger = false;
	}
}
