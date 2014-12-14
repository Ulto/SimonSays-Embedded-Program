
#ifndef MAIN_H
#define MAIN_H


// DEFINES
// Internal Constants
#define MAX_PATTERN_LENGTH 		25
#define MAX_ROUNDS				10
#define PATTERN_PLAYBACK_DELAY	250		// ms
#define TCP_PORT				1500
#define ACTIVE					1

#define FIRST

#ifdef FIRST
	// LED Output(s)
	#define	RED_LT		0
	#define GRN_LT		1
	#define BLU_LT		2
	#define YEL_LT		3

	// Push-button Input(s)
	#define RED_PB		4
	#define GRN_PB		5
	#define BLU_PB		6
	#define YEL_PB		7

#else

	// LED Output(s)
	#define	RED_LT		15
	#define GRN_LT		16
	#define BLU_LT		17
	#define YEL_LT		18

	// Push-button Input(s)
	#define RED_PB		12
	#define GRN_PB		20
	#define BLU_PB		21
	#define YEL_PB		22

#endif

// GLOBAL VARIABLES
struct game_data_struct
{
	char 	pattern[MAX_PATTERN_LENGTH];
	int 	round_num;
	int		winner;		// 1 is winner, 2 is loser
};


// FUNCTION PROTOTYPES
void PrintHeader(void);
void TestLeds(void);
void PlayPattern(struct game_data_struct*);
int TxPattern(struct game_data_struct*);
int RxPattern(struct game_data_struct*);
void Delay_ms(int);
void PlayerRepeatPattern(struct game_data_struct*);


#endif /* MAIN_H */
