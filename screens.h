#ifndef _SCREENS_H
#define _SCREENS_H

enum Screen {
	WelcomeScreen,
	GridScreen
};

extern volatile enum Screen switchScreen;

void welcome();
void grid();

#endif