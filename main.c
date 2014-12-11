// CMPET 456
// Final Lab - Fall 2014
// Simon Game
// ---------------------
// Zach Cross
// Greg Pohodich
//
// MAIN


// INCLUDES
#include <stdio.h>
#include <stdlib.h>
#include "GamePlayFunctions.c"
#include "Main.h"
#include "ntwk.h"
#include "Threads.h"





int main()
{
	// Variable Declarations
	char user_input;
	int  ui_flag		= 0;
	char remote_IP[20] 	= {"127.0.0.1"};

	// Initialization
		// Game Variables
		game_data.round_num = 1;


		// Determine If Master or Slave
		PrintHeader();


		do
		{
			// If ui_flag has been set, the loop is no longer on its first iteration; print error message.
			if (ui_flag > 0)
			{
				printf("Sorry, '%s' is not a valid player number, please try again: \n", user_input);
			}

			// Get User Input
			user_input = getchar();

			// Increment ui_flag
			ui_flag += 1;

		} while ((user_input != '1') | (user_input != '2'));


		if (user_input == '1')
		{
			// Open Connection With Remote Host
			SendInit(remote_IP, TCP_PORT);
		}
		else if (user_input == '2')
		{
			// Initialize Recieve
			RecvInit(TCP_PORT);
		}

	// Main Game Play Loop
	while (game_data.round_num <= MAX_ROUNDS | game_data.round_num == 0)
	{

		// If player 1
		if (user_input == 1)
		{
			// Record New Pattern Based On User Input
			SetPattern();

			// Send Current Pattern To Remote Host
			TxPattern();

			// Recieve Pattern From Remote Host and Store As Current Pattern
			RxPattern();

			if (game_data.round_num == 0)
			{
				// Other game terminated
				break;
			}

			// Play Back Current Pattern
			PlayPattern();

			// Let the player repeat the pattern
			PlayerRepeatPattern();

			game_data.round_num++;

		}
		else
		{
			// Recieve Pattern From Remote Host and Store As Current Pattern
			RxPattern();

			if (game_data.round_num == 0)
			{
				// Other game terminated
				break;
			}

			// Play Back Current Pattern
			PlayPattern();

			// Let the player repeat the pattern
			PlayerRepeatPattern();

			// Record New Pattern Based On User Input
			SetPattern();

			// Send Current Pattern To Remote Host
			TxPattern();

			game_data.round_num++;

		}

	} // END Main Game Play Loop


	// Display End Game Info Based On Round Number
	if (game_data.round_num > MAX_ROUNDS)
	{
		printf("Max number of rounds has been reached.  Congratulations, you beat the game!\n");

	}
	else
	{
		if (game_data.winner == 1)
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
