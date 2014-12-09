#include <stdio.h>
#include <stdlib.h>
#include <termios.h>
#include <unistd.h>
#include <pthread.h>

#include "Threads.h"
#include "Main.h"


void *GPIOThread(void *ptr);

static int _key = 0;
static struct termios _save;


/* Sleep(int msec)
*    delays for "msec" number of milliseconds
*
* input:
*    msec = number of milliseconds to "sleep"
* return:
*    none
*/
void Sleep(int msec)
{
	usleep(1000 * msec);
}

/* CheckForKey(void)
*    check for a keypress: combination of kbhit and getch
*
* input:
*    none
* return:
*    0 = no keyboard entry
*    else, Ascii character from the keyboard
*/
int CheckForKey(void)
{
	int temp = 0;

	/* see if a key has been pressed */
	if (_key != 0)
	{
		/* if so, get the key and zero the flag */
		temp = _key;
		_key = 0;
	}
	return(temp);
}


/* secondary thread to block waiting for a keypress */
void *KeyThread(void *ptr)
{
	while (1)
	{
		_key = getchar();
		usleep(1000);
	}
}

/* secondary thread to check io  */
void *GPIOThread(void *ptr)
{


}

/* PrgmInit(void)
*    sets the console attributes
*    launches a secondary thread
*    Must be called at the start of the program
*
* input:
*    none
* return:
*    none
*/
void PrgmInit(void)
{
	struct termios ios;
	pthread_t tid;
	int std, tres;
	char *arg = "";

	std = fileno(stdin);

	/* get the current terminal flags */
	tcgetattr(std, &_save);
	ios = _save;
	ios.c_lflag &= ~(ICANON | ECHO);

	/* apply the new settings */
	tcsetattr(std, TCSANOW, &ios);

	/* create independent thread to monitor the console */
	tres = pthread_create(&tid, NULL, GPIOThread, (void *)arg);
	tres = pthread_create(&tid, NULL, KeyThread, (void *)arg);

}


/* PrgmExit(void)
*    restores the console attributes
*    Must be called before exiting the program
*
* input:
*    none
* return:
*    none
*/
void PrgmExit(void)
{
	int std;

	/* restore the previous console settings */
	std = fileno(stdin);
	_save.c_lflag |= ICANON | ECHO;
	tcsetattr(std, TCSANOW, &_save);
}
