// CMPET 456
// Final Lab - Fall 2014
// Simon Game
// ---------------------
// Zach Cross
// Greg Pohodich
//
// Gameplay Functions

#include <stdio.h>
#include "Main.h"

// Record New Pattern Based On User Input
void SetPattern(struct game_data_struct *game_data)
{
	int loop_ctr;
	char redState, greenState, blueState, yellowState;

	// Re-Iterate pattern capture length to match round number.
	for (loop_ctr = 0; loop_ctr < game_data->round_num; loop_ctr++)
	{

		// Light all LEDs to indicate a pattern step "record mode"
		Write_PortB(RED_LT, 1);
		Write_PortB(GRN_LT, 1);
		Write_PortB(BLU_LT, 1);
		Write_PortB(YEL_LT, 1);

		redState = Read_PortB(RED_PB);
		greenState = Read_PortB(GRN_PB);
		blueState = Read_PortB(BLU_PB);
		yellowState = Read_PortB(YEL_PB);

		// Block, then save and show input color.
		while ((redState != ACTIVE) & (greenState != ACTIVE) & (blueState != ACTIVE) & (yellowState != ACTIVE))
		{
			redState = Read_PortB(RED_PB);
			greenState = Read_PortB(GRN_PB);
			blueState = Read_PortB(BLU_PB);
			yellowState = Read_PortB(YEL_PB);
			Delay_ms(5);
		}


		if (redState == ACTIVE)
		{
			printf("RED_PB\n");
			game_data->pattern[loop_ctr] = 'R';
			Write_PortB(RED_LT, 1);
			Write_PortB(GRN_LT, 0);
			Write_PortB(BLU_LT, 0);
			Write_PortB(YEL_LT, 0);
		}
		else if (greenState == ACTIVE)
		{
			printf("GRN_PB\n");
			game_data->pattern[loop_ctr] = 'G';
			Write_PortB(RED_LT, 0);
			Write_PortB(GRN_LT, 1);
			Write_PortB(BLU_LT, 0);
			Write_PortB(YEL_LT, 0);
		}
		else if (blueState == ACTIVE)
		{
			printf("BLU_PB\n");
			game_data->pattern[loop_ctr] = 'B';
			Write_PortB(RED_LT, 0);
			Write_PortB(GRN_LT, 0);
			Write_PortB(BLU_LT, 1);
			Write_PortB(YEL_LT, 0);
		}
		else if (yellowState == ACTIVE)
		{
			printf("YEL_PB\n");
			game_data->pattern[loop_ctr] = 'Y';
			Write_PortB(RED_LT, 0);
			Write_PortB(GRN_LT, 0);
			Write_PortB(BLU_LT, 0);
			Write_PortB(YEL_LT, 1);
		}
		else
		{
			//We somehow exited the while loop without a button being pressed
			printf("Error, no buttons pressed\n");
		}

		// Hold to show saved color.
		Delay_ms(500);

		// Ensure All Outputs Are Off
		Write_PortB(RED_LT, 0);
		Write_PortB(GRN_LT, 0);
		Write_PortB(BLU_LT, 0);
		Write_PortB(YEL_PB, 0);
		
	}

} // END SetPattern


// Play Back Current Pattern
void PlayPattern(struct game_data_struct *game_data)
{
	int loop_ctr;

	for (loop_ctr = 0; loop_ctr < game_data->round_num; loop_ctr ++)
	{

		// Pause Before Next Color
		Delay_ms(PATTERN_PLAYBACK_DELAY);

		// Turn On Relevant Color
		switch(game_data->pattern[loop_ctr])
		{
			case 'R':
				Write_PortB(RED_LT, 1);
				break;

			case 'G':
				Write_PortB(GRN_LT, 1);
				break;

			case 'B':
				Write_PortB(BLU_LT, 1);
				break;

			case 'Y':
				Write_PortB(YEL_LT, 1);
				break;

			default:
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
int TxPattern(struct game_data_struct *game_data)
{
	// Send game_data struct to remote host.
	NtwkSend(sizeof(struct game_data_struct), game_data);

} // END TxPattern


// Recieve Pattern From Remote Host and Store As Current Psttern
int RxPattern(struct game_data_struct *game_data)
{
	// Recieve game_data struct from remote host.
	do
	{
		Delay_ms(10);

	} while (NtwkRecv(sizeof(struct game_data_struct), game_data) == 0);

} // END RxPattern


// Waits for the user to input the last pattern played for them
void PlayerRepeatPattern(struct game_data_struct *game_data)
{
	char Repeatpattern[MAX_PATTERN_LENGTH] = { 0 };
	char loop_ctr;
	char redState, greenState, blueState, yellowState;

	// Re-Iterate pattern capture length to match round number.
	for (loop_ctr = 0; loop_ctr < game_data->round_num; loop_ctr++)
	{

		redState = Read_PortB(RED_PB);
		greenState = Read_PortB(GRN_PB);
		blueState = Read_PortB(BLU_PB);
		yellowState = Read_PortB(YEL_PB);

		// Block, then save and show input color.
		while ((redState != ACTIVE) & (greenState != ACTIVE) & (blueState != ACTIVE) & (yellowState != ACTIVE))
		{
			redState = Read_PortB(RED_PB);
			greenState = Read_PortB(GRN_PB);
			blueState = Read_PortB(BLU_PB);
			yellowState = Read_PortB(YEL_PB);
			Delay_ms(5);
		}

		if (redState == ACTIVE)
		{
			Repeatpattern[loop_ctr] = 'R';
			Write_PortB(RED_LT, 1);
			Write_PortB(GRN_LT, 0);
			Write_PortB(BLU_LT, 0);
			Write_PortB(YEL_LT, 0);
		}
		else if (greenState == ACTIVE)
		{
			Repeatpattern[loop_ctr] = 'G';
			Write_PortB(RED_LT, 0);
			Write_PortB(GRN_LT, 1);
			Write_PortB(BLU_LT, 0);
			Write_PortB(YEL_LT, 0);
		}
		else if (blueState == ACTIVE)
		{
			Repeatpattern[loop_ctr] = 'B';
			Write_PortB(RED_LT, 0);
			Write_PortB(GRN_LT, 0);
			Write_PortB(BLU_LT, 1);
			Write_PortB(YEL_LT, 0);
		}
		else if (yellowState == ACTIVE)
		{
			Repeatpattern[loop_ctr] = 'Y';
			Write_PortB(RED_LT, 0);
			Write_PortB(GRN_LT, 0);
			Write_PortB(BLU_LT, 0);
			Write_PortB(YEL_LT, 1);
		}


		// Hold to show saved color then turn off
		Delay_ms(500);

		Write_PortB(RED_LT, 0);
		Write_PortB(GRN_LT, 0);
		Write_PortB(BLU_LT, 0);
		Write_PortB(YEL_LT, 0);

		// Check if the new input matches the correct pattern
		if (Repeatpattern[loop_ctr] != game_data->pattern[loop_ctr])
		{ 
			// The player was incorrect
			// Set round to 0 and break
			game_data->round_num = 0;
			break;
		}


		// Game is over, player lost
		if (game_data->round_num == 0)
		{
			printf("game over\n");
			// Transmit to other side that the game is over and that they won
			// Set them to be winner first before doing so, then set winner back to 2
			game_data->winner = 1;
			TxPattern(game_data);
			game_data->winner = 2;
		}

	}

}

// Sleep For The Specified Number Of Milliseconds
void Delay_ms(int ms)
{
	usleep(ms * 1000);

} // END Delay_ms
