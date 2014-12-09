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

// DEFINES
#define MAX_PATTERN_LENGTH 	25
#define MAX_ROUNDS			10


// FUNCTION PROTOTYPES
void PrintHeader(void);


// GLOBAL VARIABLES
struct game_data_struct
{
	char 	pattern[MAX_PATTERN_LENGTH];
	int 	round_num;


} game_data;


int main()
{
	// Variable Declarations
	char user_input;
	int  ui_flag		= 0;

	// Initialization
		// Game Variables
		game_data.round_num = 1;

		// Network Connection


		// Determine If Master or Slave
		PrintHeader();
		do
		{
			// If ui_flag has been set, the loop is no longer on its forst iteration; print error message.
			if (ui_flag > 0)
			{
				printf("Sorry, '%s' is not a valid player number, please try again: \n", user_input);
			}

			// Get User Input
			user_input = getch();

			// Increment ui_flag
			ui_flag += 1;

		} while((user_input != '1') | (user_input != '2'))



	// Main Game Play Loop
	while (game_data.round_num <= MAX_ROUNDS | game_data.round_num == 0)
	{
		// If player 1, set pattern.
		if (user_input == 1 | game_data.round_num != 1)
		{
			// Record New Pattern Based On User Input
			SetPattern();

			// Send Current Pattern To Remote Host
			TxPattern();

		}

		// Recieve Pattern From Remote Host and Store As Current Psttern
		RxPattern();

		// Play Back Current Pattern
		PlayPattern();

	} // END Main Game Play Loop

	// Display End Game Info Based On Round Number
	if (game_data.round_num > MAX_ROUNDS)
	{
		printf("Max number of rounds has been reached.  Congradulations, you beat the game!\n");

	}
	else
	{
		// Loser
		printf("")

	}

	

	exit(0);
} // END main


// Print UI Header Info to Console
void PrintHeader(void)
{
	printf("- SIMON -\n");
	printf("\n");
	printf("Player 1 or 2:");

} // END PrintHeader
