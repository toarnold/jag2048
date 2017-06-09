#include <stdint.h>
#include <stdio.h>
#include <jaglib.h>
#include "screens.h"
#include "grid.h"
#include "blitter.h"

volatile enum Screen switchScreen = WelcomeScreen;

void welcome()
{
	switchScreen = WelcomeScreen;
	jag_console_clear();
	show_logo();
	jag_console_set_cursor(80, 16 * 8);
	puts("Join the numbers and");
	jag_console_set_cursor(76, 17 * 8);
	puts("get to the 2048 tile!");
	jag_console_set_cursor(52, 4 + 18 * 8);
	puts("Created by Gabriele Cirulli");
	jag_console_set_cursor(44, 4 + 19 * 8);
	puts("Based on 1024 by Veewo Studio");
	jag_console_set_cursor(52, 4 + 20 * 8);
	puts("ATARI Jaguar version (V1.0)");
	jag_console_set_cursor(96, 4 + 21 * 8);
	puts("by toarnold 2015");
	jag_console_set_cursor(88, 23 * 8);
	puts("press <A> to begin");
}

void grid()
{
	switchScreen = GridScreen;
	jag_console_clear();
	fill_background();
	for (uint16_t x = 0; x < 4; ++x)
		for (uint16_t y = 0; y < 4; ++y)
			fill_backgrid(y * 15360 + x * 48 + 2628);

	jag_console_set_cursor(8, 18);
	puts("Score");
	jag_console_set_cursor(12, 66);
	puts("Best");
	jag_console_set_cursor(272, 9);
	puts("press");
	jag_console_set_cursor(264, 19);
	puts("<*> for");
	jag_console_set_cursor(280, 29);
	puts("new");
	jag_console_set_cursor(276, 39);
	puts("game");
	jag_console_set_cursor(272, 60);
	puts("press");
	jag_console_set_cursor(268, 70);
	puts("<#> to");
	jag_console_set_cursor(276, 80);
	puts("quit");

	GridNewGame();
}
