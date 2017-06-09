#include <jagcore.h>
#include <string.h>
#include "globals.h"
#include "grid.h"
#include "screens.h"

unsigned long skunkTimeout=1; /* No Skunkboard needed */
void jag_welcome_message() { /*No welcome message*/ }
void jag_init_message() { /*No init message*/ }

volatile uint32_t ticks;
uint16_t jag_custom_interrupt_handler()
{
	if (*INT1&C_VIDENA) // Video Interrupt?
	{
		switch (switchScreen)
		{
		case WelcomeScreen: // Welcome: No Tiles
			tilesbranch->cc=2; // branch never
			break;
		case GridScreen:// Grid: With Tiles
			tilesbranch->cc=1; // branch always
			GridGetObjectList(listbufshadow); // fill shadowlist fi necessary
			memcpy(listbuf,listbufshadow,41*8); // copy to listbuffer
			break;
		}
		++ticks;
		return C_VIDCLR; // flag interrupt video handled
	}
	return 0;
}

static uint8_t gameOverVisible = FALSE;
void ShowGameOver(uint8_t show)
{
	gameOverVisible = show;
	gameoverbmp->p1.xpos = jag_console_bmp->p1.xpos + (show ? 80 : -200);
}

static uint8_t wonVisible = FALSE;
void ShowWon(uint8_t show)
{
	wonVisible = show;
	youwinbmp->p1.xpos = jag_console_bmp->p1.xpos + (show ? 96 : -200);
}

int main()
{
	welcome();
	for(;;)
	{
		if (switchScreen == GridScreen)
		{
			switch(jag_read_stick0(STICK_READ_ALL))
			{
				case STICK_UP:
					GridMoveTiles(fromBottom);
					break;
				case STICK_DOWN:
					GridMoveTiles(fromTop);
					break;
				case STICK_LEFT:
					GridMoveTiles(fromRight);
					break;
				case STICK_RIGHT:
					GridMoveTiles(fromLeft);
					break;
				case STICK_STAR:
					ShowGameOver(FALSE);
					ShowWon(FALSE);
					GridNewGame();
					// Waste time
					for (int wait=0;wait<20;++wait)
						jag_wait_vbl();
					break;
				case STICK_HASH:
					ShowGameOver(FALSE);
					ShowWon(FALSE);
					welcome();
					break;
				case STICK_A:
					if (wonVisible)
					{
						ShowWon(FALSE);
						GridKeepPlaying();
					}
					break;
			}

			switch (GridGameState())
			{
			case Won:
				ShowWon(TRUE);
				break;
			case Over:
				ShowGameOver(TRUE);
				break;
			}

		}
		else
		{
			if (jag_read_stick0(STICK_READ_DIRECTIONS_A_ONLY)==STICK_A)
			{
				srand(ticks);
				grid();
			}
		}
	}
}
