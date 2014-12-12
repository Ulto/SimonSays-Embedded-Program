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

#include "Main.h"


int main()
{
	// Variable Declarations
	char user_input;
	int errorNtwk = 0;
	int  ui_flag		= 0;
	char remote_IP[20] 	= {"127.0.0.1"};
	struct game_data_struct *game_data;

	game_data = (struct game_data_struct*)malloc(sizeof(struct game_data_struct));

	// Initialization
		// Game Variables
		//memset();
		game_data->round_num = 1;


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

			if (user_input == '1')
			{
				printf("SUCCESS\n");
			}

			// Increment ui_flag
			ui_flag += 1;

		} while ((user_input < '1') | (user_input > '2'));


		if (user_input == '1')
		{
			// Open Connection With Remote Host
			printf("CALL SendInit\n");
			SendInit(remote_IP, TCP_PORT);
		}
		else if (user_input == '2')
		{
			// Initialize Recieve
			printf("CALL RecvInit\n");
			errorNtwk = RecvInit(TCP_PORT);
			if (errorNtwk == 0)
				NtwkWait();
			else
				printf("Network Error: %d\n", errorNtwk);
		}

	// Main Game Play Loop
	while (game_data->round_num <= MAX_ROUNDS | game_data->round_num == 0)
	{

		// If player 1
		if (user_input == '1')
		{
			// Record New Pattern Based On User Input
			printf("CALL SetPattern\n");
			SetPattern(game_data);

			// Send Current Pattern To Remote Host
			printf("CALL TxPattern\n");
			TxPattern(game_data);

			// Recieve Pattern From Remote Host and Store As Current Pattern
			printf("CALL RxPattern\n");
			RxPattern(game_data);

			if (game_data->round_num == 0)
			{
				// Other game terminated
				break;
			}

			// Play Back Current Pattern
			printf("CALL PlayPattern\n");
			PlayPattern(game_data);

			// Let the player repeat the pattern
			printf("CALL PlayerRepeatPattern\n");
			PlayerRepeatPattern(game_data);

			game_data->round_num++;

		}
		else
		{
			printf("CALL RxPattern\n");
			// Recieve Pattern From Remote Host and Store As Current Pattern
			RxPattern(game_data);

			if (game_data->round_num == 0)
			{
				// Other game terminated
				break;
			}

			// Play Back Current Pattern
			PlayPattern(game_data);

			// Let the player repeat the pattern
			PlayerRepeatPattern(game_data);

			// Record New Pattern Based On User Input
			SetPattern(game_data);

			// Send Current Pattern To Remote Host
			TxPattern(game_data);

			game_data->round_num++;

		}

	} // END Main Game Play Loop


	// Display End Game Info Based On Round Number
	if (game_data->round_num > MAX_ROUNDS)
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
