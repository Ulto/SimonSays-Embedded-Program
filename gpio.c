#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <fcntl.h>
#include <pthread.h>

#include "gpio.h"
#include "Lab5.h"

static FILE *InitInput(int numb);
static FILE *InitOutput(int numb);
static FILE *InitAdc(int numb);
static int PortRead(FILE *fd);
static void PortWrite(FILE *fd,int data);


static FILE *fd[32] = { NULL };


/* read the status of an input port */
/* int port (0-31) corresponds to PB0 to PB31 */
/* returns 0 or 1 */
int Read_PortB(int port)
{
    int state = 0;

    /* see if this port has been opened */
    if (fd[port] == NULL)
        fd[port] = InitInput(port);

    /* once opened, read the data */
    if (fd[port] != NULL)
        state = PortRead(fd[port]);

    return(state);
}


int Read_PortC(int port)
{
	int state = 0;

	/* see if this port has been opened */
	if (fd[port] == NULL)
		fd[port] = InitAdc(port);

	/* once opened, read the data */
	if (fd[port] != NULL)
		state = PortRead(fd[port]);

	return(state);
}

/* write the status of an output port */
/* int port (0-31) corresponds to PB0 to PB31 */
/* data is 0 or 1 */
void Write_PortB(int port, int data)
{
    /* see if this port has been opened */
    if (fd[port] == NULL)
        fd[port] = InitOutput(port);

    /* once opened, write the data */
    if (fd[port] != NULL)
        PortWrite(fd[port], data);
}


/* InitInput
 * configure a digital I/O pin (PB0-PB31) for input
 *
 * input:
 *    pin number (0-31) of port B (this will get converted to gpio 64-95)
 * returns
 *    NULL = error, can not access the pin
 *    else file stream to read from
 */
static FILE *InitInput(int numb)
{
   char text[80];
   int fd_export;
   int fd_dir;
   int cnt, id = 64+numb;
   FILE *fd_port = NULL;

   while (1)
   {
      fd_export = open("/sys/class/gpio/export", O_WRONLY);
      if (fd_export < 0)
      {
         printf("open for export failed\n");
         break;
      }

      /* configure the port for digital i/o */
      cnt = sprintf(text,"%d\n",id);
      write(fd_export, text, cnt);
      close(fd_export);


      /* go to the selected port and mark it as an input */
      sprintf(text,"/sys/class/gpio/gpio%d/direction",id);
      fd_dir = open(text, O_WRONLY);
      if (fd_dir < 0)
      {
         printf("open for direction failed\n");
         break;
      }

      write(fd_dir, "in\n", 3);
      close(fd_dir);


      /* open a file stream to the i/o port */
      sprintf(text,"/sys/class/gpio/gpio%d/value",id);
      fd_port = fopen(text, "r");
      if (fd_port == NULL)
      {
         printf("open for value failed\n");
      }
      break;
   }

   return(fd_port);
}


/* InitOutput
 * configure a digital I/O pin (PB0-PB31) for output
 *
 * input:
 *    pin number (0-31) of port B (this will get converted to gpio 64-95)
 * returns
 *    NULL = error, can not access the pin
 *    else file stream to write to
 */
static FILE *InitOutput(int numb)
{
   char text[80];
   int fd_export;
   int fd_dir;
   int cnt, id = 64+numb;
   FILE *fd_port = NULL;

   while (1)
   {
      fd_export = open("/sys/class/gpio/export", O_WRONLY);
      if (fd_export < 0)
      {
         printf("open for export failed\n");
         break;
      }

      /* configure the port for digital i/o */
      cnt = sprintf(text,"%d\n",id);
      write(fd_export, text, cnt);
      close(fd_export);


      /* go to the selected port and mark it as an output */
      sprintf(text,"/sys/class/gpio/gpio%d/direction",id);
      fd_dir = open(text, O_WRONLY);
      if (fd_dir < 0)
      {
         printf("open for direction failed\n");
         break;
      }

      write(fd_dir, "out\n", 4);
      close(fd_dir);


      /* open a file stream to the i/o port */
      sprintf(text,"/sys/class/gpio/gpio%d/value",id);
      fd_port = fopen(text, "w");
      if (fd_port == NULL)
      {
         printf("open for value failed\n");
      }
      break;
   }

   return(fd_port);
}


/* InitAdc
 * configure an analog pin (PC0-PC3) for input
 *
 * input:
 *    pin number (0-3) of port C
 * returns
 *    NULL = error, can not access the pin
 *    else file stream to read from
 */
static FILE *InitAdc(int numb)
{
   char text[80];
   int fd_enb;
   FILE *fd_port = NULL;

   while (1)
   {
      /* configure the port for a-to-d use */
      sprintf(text,"/sys/class/misc/adc/ch%d_enable",numb);
      fd_enb = open(text, O_WRONLY);
      if (fd_enb < 0)
      {
         printf("open for adc enable failed\n");
         break;
      }

      write(fd_enb, "1\n", 2);
      close(fd_enb);


      /* open a file stream to the i/o port */
      sprintf(text,"/sys/class/misc/adc/ch%d_value",numb);
      fd_port = fopen(text, "r");
      if (fd_port == NULL)
      {
         printf("open for value failed\n");
      }
      break;
   }

   return(fd_port);
}


/* read from the selected file stream */
static int PortRead(FILE *fd)
{
   char text[12];

   /* make sure to do a rewind after every read/write */
   memset(text, 0, sizeof(text));
   fread(text, sizeof(char), 10, fd);
   rewind(fd);
   return(atoi(text));
}


/* write to the selected file stream */
static void PortWrite(FILE *fd, int data)
{
   if (data == 0)
      fwrite("0\n", sizeof(char), 2, fd);
   else
      fwrite("1\n", sizeof(char), 2, fd);

   rewind(fd);
}

