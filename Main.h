
#ifndef MAIN_H
#define MAIN_H


// DEFINES
// Internal Constants
#define MAX_PATTERN_LENGTH 		25
#define MAX_ROUNDS				10
#define PATTERN_PLAYBACK_DELAY	250		// ms
#define TCP_PORT				1500
#define ACTIVE					1

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


// FUNCTION PROTOTYPES
void PrintHeader(void);
void PlayPattern(void);
void TxPattern(void);
void RxPattern(void);
void Delay_ms(int);


// GLOBAL VARIABLES
struct game_data_struct
{
	char 	pattern[MAX_PATTERN_LENGTH];
	int 	round_num;

} game_data;


#endif /* MAIN_H */
