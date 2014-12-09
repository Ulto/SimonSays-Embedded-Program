/* Sleep(int msec)
*    delays for "msec" number of milliseconds
*
* input:
*    msec = number of milliseconds to "sleep"
* return:
*    none
*/
void Sleep(int msec);

/* CheckForKey(void)
*    check for a keypress: combination of kbhit and getch
*
* input:
*    none
* return:
*    0 = no keyboard entry
*    else, Ascii character from the keyboard
*/
int CheckForKey(void);

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
void PrgmInit(void);

/* PrgmExit(void)
*    restores the console attributes
*    Must be called before exiting the program
*
* input:
*    none
* return:
*    none
*/
void PrgmExit(void);


