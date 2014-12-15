// CMPET 456
// Final Lab - Fall 2014
// Simon Game
// ---------------------
// Zach Cross
// Greg Pohodich
//
// MAIN

/* SUMMARY

The purpose of this program is to act as a network-able Simon says game. 
The user is prompted for their player number (1 or 2), then the game begins 
with player 1 setting a pattern (red, green, blue, or yellow) with push buttons. 
The pattern is transmitted to player 2, where it is played and waits for the 
player to repeat it on the push buttons. If successful, player 2 will set a 
pattern and player 1 will repeat it. Once both player have successfully repeated 
a pattern, the round number increments, also increasing the size of the pattern by 
1 color. Each round the player can set the pattern from scratch, adding difficulty 
to the normal Simon game.

Once the maximum amount of rounds is reached (25) or a player inputs a wrong pattern, 
the game is over and the players are notified of the winner.
*/


// INCLUDES
#include <stdio.h>
#include <stdlib.h>

#include "Main.h"


int main()
{
	// Variable Declarations
	char user_input;
	int errorNtwk		= 0;
	int  ui_flag		= 0;
	char remote_IP[20] 	= {"127.0.0.1"};
	struct game_data_struct *game_data;

	// Allocate memory for game_data
	game_data = (struct game_data_struct*)malloc(sizeof(struct game_data_struct));

	// Initialization
	// Game Variables
	game_data->round_num = 1;
	game_data->winner = 0;

	TestLeds();

	// Print Game Info
	PrintHeader();

	// Determine Player (1/2)
	do
	{
		// If ui_flag has been set, the loop is no longer on its first iteration; print error message.
		if (ui_flag > 0)
		{
			printf("Sorry, that is not a valid player number, please try again: \n");
			fflush(stdout);
		}

		// Get User Input
		user_input = getchar();

		// Increment ui_flag
		ui_flag += 1;

	} while ((user_input < '1') | (user_input > '2'));

	printf("Initializing Network connection\n");
	if (user_input == '1')
	{
		// Open Connection With Remote Host
		SendInit(remote_IP, TCP_PORT);
	}
	else if (user_input == '2')
	{
		// Initialize Recieve
		errorNtwk = RecvInit(TCP_PORT);
		if (errorNtwk == 0)
			NtwkWait();
		else
			printf("Network Error: %d\n", errorNtwk);
	}

	// Main Game Play Loop
	while (game_data->round_num <= MAX_ROUNDS | game_data->round_num != 0)
	{

		// If player 1
		if (user_input == '1')
		{
			// Record New Pattern Based On User Input
			printf("Set a pattern for the other player to repeat\n");
			SetPattern(game_data);

			// Send Current Pattern To Remote Host
			errorNtwk = TxPattern(game_data);

			if (errorNtwk != 0)
			{
				printf("Network Failure, Exiting game\n");
				exit(0);
			}

			// Recieve Pattern From Remote Host and Store As Current Pattern
			printf("Waiting for player 1 to make a pattern\n");
			errorNtwk = RxPattern(game_data);

			if (errorNtwk != 0)
			{
				printf("Network Failure, Exiting game\n");
				exit(0);
			}

			if (game_data->round_num == 0)
			{
				// Other game terminated
				break;
			}

			// Play Back Current Pattern
			printf("Watch the pattern closely\n");
			PlayPattern(game_data);

			// Let the player repeat the pattern
			printf("Time to repeat that pattern\n");
			PlayerRepeatPattern(game_data);

			game_data->round_num++;
			printf("Round Number: %d", game_data->round_num);

		}
		else
		{
			printf("Waiting for player 1 to make a pattern\n");
			// Recieve Pattern From Remote Host and Store As Current Pattern
			errorNtwk = RxPattern(game_data);

			if (errorNtwk != 0)
			{
				printf("Network Failure, Exiting game\n");
				exit(0);
			}

			if (game_data->round_num == 0)
			{
				// Other game terminated
				break;
			}

			// Play Back Current Pattern
			printf("Watch the pattern closely\n");
			PlayPattern(game_data);

			printf("Time to repeat that pattern\n");
			PlayerRepeatPattern(game_data);

			// Record New Pattern Based On User Input
			printf("Set a pattern for the other player to repeat\n");
			SetPattern(game_data);

			// Send Current Pattern To Remote Host
			errorNtwk = TxPattern(game_data);

			if (errorNtwk != 0)
			{
				printf("Network Failure, Exiting game\n");
				exit(0);
			}

			game_data->round_num++;
			printf("Round Number: %d", game_data->round_num);

		}

	} // END Main Game Play Loop


	// Display End Game Info Based On Round Number
	if (game_data->round_num >= MAX_ROUNDS)
	{
		printf("Max number of rounds has been reached.  Congratulations, you beat the game!\n");

	}
	else
	{
		if (game_data->winner == 1)
		{
			// The player won
			printf("The other player couldn't repeat the pattern. You win!\n");
		}
		else
		{
			// The player lost
			printf("Sorry, wrong pattern\n");
		}

	}

	printf("Game Over\n");

	free(game_data);
	// Terminate Network Connection
	NtwkExit();

	exit(0);
} // END main


// Print UI Header Info to Console
void PrintHeader(void)
{
	printf("- SIMON -\n");
	printf("\n");
	printf("Player 1 or 2:");

} // END PrintHeader

// Test all LEDs in order
void TestLeds(void)
{

	// Test all of the LEDs for proper operation
	Write_PortB(RED_LT, 1);
	Delay_ms(500);
	Write_PortB(RED_LT, 0);
	Delay_ms(500);
	Write_PortB(GRN_LT, 1);
	Delay_ms(500);
	Write_PortB(GRN_LT, 0);
	Delay_ms(500);
	Write_PortB(BLU_LT, 1);
	Delay_ms(500);
	Write_PortB(BLU_LT, 0);
	Delay_ms(500);
	Write_PortB(YEL_LT, 1);
	Delay_ms(500);
	Write_PortB(YEL_LT, 0);
	Delay_ms(500);

} // END TestLeds
