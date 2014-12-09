
#ifndef MAIN_H
#define MAIN_H


// DEFINES
#define MAX_PATTERN_LENGTH 	25
#define MAX_ROUNDS			10

// Assign pins
#define	RED_OUT		0
#define GREEN_OUT	1
#define BLUE_OUT	2

#define RED_PB		3
#define GREEN_PB	4
#define BLUE_PB		5
#define YELLOW_PB	6



// FUNCTION PROTOTYPES
void PrintHeader(void);


// GLOBAL VARIABLES
struct game_data_struct
{
	char 	pattern[MAX_PATTERN_LENGTH];
	int 	round_num;


} game_data;



#endif /* MAIN_H */