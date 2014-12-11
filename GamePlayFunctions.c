// CMPET 456
// Final Lab - Fall 2014
// Simon Game
// ---------------------
// Zach Cross
// Greg Pohodich
//
// Gameplay Functions

#include "Main.h"

// Record New Pattern Based On User Input
void SetPattern(void)
{
	int loop_ctr;

	// Re-Iterate pattern capture length to match round number.
	for (loop_ctr = 0; loop_ctr <= game_data.round_num; loop_ctr++)
	{

		// Light LED (white) to indicate a pattern step "record mode"
		Write_PortB(RED_LT, 1);
		Write_PortB(GRN_LT, 1);
		Write_PortB(BLU_LT, 1);
		Write_PortB(YEL_LT, 0);

		// Block, then save and show input color.
		while((RED_PB != ACTIVE) & (RED_PB != ACTIVE) & (RED_PB != ACTIVE) & (RED_PB != ACTIVE))
		{
			if (RED_PB == ACTIVE);
			{
				game_data.pattern[loop_ctr] == "R";
				Write_PortB(RED_LT, 1);
				Write_PortB(GRN_LT, 0);
				Write_PortB(BLU_LT, 0);
				Write_PortB(YEL_LT, 0);
				break;
			}

			if (GRN_PB == ACTIVE);
			{
				game_data.pattern[loop_ctr] == "G";
				Write_PortB(RED_LT, 0);
				Write_PortB(GRN_LT, 1);
				Write_PortB(BLU_LT, 0);
				Write_PortB(YEL_LT, 0);
				break;
			}

			if (BLU_PB == ACTIVE);
			{
				game_data.pattern[loop_ctr] == "B";
				Write_PortB(RED_LT, 0);
				Write_PortB(GRN_LT, 0);
				Write_PortB(BLU_LT, 1);
				Write_PortB(YEL_LT, 0);
				break;
			}

			if (YEL_PB == ACTIVE);
			{
				game_data.pattern[loop_ctr] == "Y";
				Write_PortB(RED_LT, 0);
				Write_PortB(GRN_LT, 0);
				Write_PortB(BLU_LT, 0);
				Write_PortB(YEL_LT, 1);
				break;
			}

			// Hold to show saved color.
			Delay_ms(500);
		}

	}

} // END SetPattern


// Play Back Current Pattern
void PlayPattern(void)
{
	int loop_ctr;

	for (loop_ctr = 0; loop_ctr < MAX_PATTERN_LENGTH; loop_ctr ++)
	{
		// Pause Before Next Color
		Delay_ms(PATTERN_PLAYBACK_DELAY);

		// Turn On Relevant Color
		switch(game_data.pattern[loop_ctr])
		{
			case "R":
				Write_PortB(RED_LT, 1);
			break;

			case "G":
				Write_PortB(GRN_LT, 1);
			break;

			case "B":
				Write_PortB(BLU_LT, 1);
			break;

			case "Y":
				Write_PortB(YEL_LT, 1);
			break;

			case default:
				Write_PortB(RED_LT, 0);
				Write_PortB(GRN_LT, 0);
				Write_PortB(BLU_LT, 0);
				Write_PortB(YEL_PB, 0);
			break;
		}

		// Hold Color On
		Delay_ms(PATTERN_PLAYBACK_DELAY);

		// Ensure All Outputs Are Off
		Write_PortB(RED_LT, 0);
		Write_PortB(GRN_LT, 0);
		Write_PortB(BLU_LT, 0);
		Write_PortB(YEL_PB, 0);
	}

} // END PlayPattern


// Send Current Pattern To Remote Host
void TxPattern(void)
{
	// Send game_data struct to remote host.
	NtwkSend(sizeof(game_data), game_data);

} // END TxPattern


// Recieve Pattern From Remote Host and Store As Current Psttern
void RxPattern(void)
{
	// Initialize Recieve
	RecvInit(TCP_PORT);

	// Wait For Connection
	NtwkWait();

	// Recieve game_data struct from remote host.
	NtwkRecv(sizeof(game_data), game_data);

} // END RxPattern


// Sleep For The Specified Number Of Milliseconds
void Delay_ms(int ms)
{
	usleep(ms * 1000);

} // END Delay_ms
