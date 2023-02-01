/*
 * by Freddy Frouin <freddy@linuxtribe.fr>
 */

#ifndef APPBUTTON_h
#define APPBUTTON_h

#include "ezButton.h"

#define BTN_DOUBLE_PRESS_THRESHOLD 450
#define BTN_PRESS_THRESHOLD1 900
#define BTN_PRESS_THRESHOLD2 1800

class appButton : public ezButton {
	private:
		unsigned long int pressedTimeLast;
		unsigned long int pressedTime;
		unsigned long int releasedTime;
		
		int level;
		bool trigger;
		
	public:	
		appButton(int pin, int mode);
		
		bool doublePress(void);
		bool shortPress(void);
		bool longPress(void);
		bool veryLongPress(void);
		
		virtual void loop(void);
};

#endif
