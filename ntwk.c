// if this is to be compiled under linux,
// then add:  #define _UNIX_

// if this is to be compiled under windows,
// then add:  wsock32.lib  to the project linker/input files/additional dependencies

#include "ntwk.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define _UNIX_

#ifndef _UNIX_
#include <windows.h>
#include <winsock.h>
#else
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

#define SOCKET       int
#define closesocket  close
#endif


/*
 * list of general error codes
 * used with the below network functions
 */
#define ERR_NONE          0   // no errors
#define ERR_BAD_IP       -1   // invalid network ip address
#define ERR_BAD_SEQ      -2   // wrong sequence / no socket
#define ERR_NTWK_FAIL    -3   // misc network failure
#define ERR_CONN_FAIL    -4   // can not connect / bind / accept
#define ERR_SEND_RECV    -5   // send / recv fail

/*
 * global prototypes
 */
int NtwkInit(int port);
int SendInit(char * ip_addrs, int port);
int RecvInit(int port);
int WebInit(char * web);
int NtwkWait(void);
int NtwkExit(void);
int NtwkSend(int length, unsigned char *datap);
int NtwkRecv(int length, unsigned char *datap);

/*
 * variables used within this module
 */
static SOCKET sock_fh = 0;
static SOCKET recv_fh = 0;
static struct sockaddr_in addr;



/* initialize network interface variables */
int NtwkInit(int port)
{
#ifndef _UNIX_
   WSADATA WSAData;
   int ret;
#endif

   /* see if a connection is already open */
   if (sock_fh > 0)
      return(ERR_BAD_SEQ);

#ifndef _UNIX_
   /* ...win32 only... */
   ret = WSAStartup(0x202, &WSAData);
   if (ret != 0)
   {
      WSACleanup();
      return(ERR_NTWK_FAIL);
   }
#endif

   /* initialize the addr data structure */
   memset((char *) &addr, 0, sizeof(addr));
   addr.sin_port = htons(port);
   addr.sin_family = AF_INET;
   addr.sin_addr.s_addr = inet_addr("0.0.0.0");

   return(ERR_NONE);
}


/* initialize network to transmit */

/* calling sequence (to connect to a destination):
 * SendInit(ip address, port number)
 *    where the ip address is the ascii text "192.168.0.141" or similar
*     and port number (for file transfer) can be 1500
 *    ..to "ping" a system, use port 7
 * NtwkSend(length, buffer)
 *    and pass a buffer to be sent to the destination
 * call either NtwkRecv()  to wait for a response,
 * or NtwkExit()  to terminate the connection
 */

int SendInit(char *ip_addrs, int port)
{
   int ret;

   ret = NtwkInit(port);
   if (ret != 0)
      return(ret);

   /* open the socket resource */
   sock_fh = socket(AF_INET, SOCK_STREAM, 0);
   if (sock_fh <= 0)
   {
#ifndef _UNIX_
      /* ...win32 only... */
      WSACleanup();
#endif
      return(ERR_NTWK_FAIL);
   }

   /* connect to destination */
   addr.sin_addr.s_addr = inet_addr(ip_addrs);
   ret = connect(sock_fh, (struct sockaddr*)&addr, sizeof(struct sockaddr_in));
   if (ret == -1)
   {
      closesocket(sock_fh);
      sock_fh = 0;
#ifndef _UNIX_
      WSACleanup();
#endif
      return(ERR_BAD_IP);
   }

   return(ERR_NONE);
}


/* initialize network to receive */

/* calling sequence (to wait for a connection):
 * RecvInit(port number)
 *    where port number matches the transmiting application
 *    note: you will need to initiate the "receive" before starting the "transmit"
 * NtwkWait()  will block and wait for someone to connect to us
 * NtwkRecv(length,buffer)
 *    pass in a pointer to a buffer, and the size of the largest possible block
 *    the function will return the number of bytes that have been transferred
 *    if there is more data, you can call NtwkRecv() again
 * call either NtwkSend()  to send a response,
 * or NtwkExit()  to terminate the connection
 */

int RecvInit(int port)
{
   int ret;

   ret = NtwkInit(port);
   if (ret != 0)
      return(ret);

   /* open the socket resource */
   recv_fh = socket(AF_INET, SOCK_STREAM, 0);
   if (recv_fh <= 0)
   {
#ifndef _UNIX_
      WSACleanup();
#endif
      return(ERR_NTWK_FAIL);
   }

   /* network receive, attach our ip address to the socket */
   ret = bind(recv_fh, (struct sockaddr *)&addr, sizeof(addr));
   if (ret == -1)
       return(ERR_CONN_FAIL);

   ret = listen(recv_fh, 5);
   if (ret == -1)
      return(ERR_CONN_FAIL);

   return(ERR_NONE);
}


/* block and wait for someone to connect to us */
/* called after RecvInit */
int NtwkWait(void)
{
   SOCKET rc;
   int len;

   if (recv_fh <= 0)
      return(ERR_NONE);

   /* wait for someone to send us something */
   len = sizeof(struct sockaddr);
   rc = accept(recv_fh, (struct sockaddr *)&addr, &len);
   if (rc == -1)
       return(ERR_CONN_FAIL);

   sock_fh = rc;

   /* ..in UNIX, we would typically fork and process the connection */
   return(ERR_NONE);
}


/* connect to the internet and get a web page */

/* sequence:
 * WebInit(web page)
 *    where webpage is something like:  behrend.psu.edu
 *    ..port number is always 80 for web pages
 *    ..will go to the internet to get the actual ip address
 * NtwkSend(length,buffer)
 *    and pass a text string to request a web page
 *    "GET /academic/engineering/directory.htm HTTP/1.0\r\n\r\n"
 * then repetitively call NtwkRecv()
 *    and get all of the web page text
 * NtwkExit()  to terminate the connection
 */

#ifndef _UNIX_
int WebInit(char *web)
{
   struct hostent *host_ptr;
   int port = 80;
   int ret;

   ret = NtwkInit(port);
   if (ret != 0)
      return(ret);

   /* open the socket resource */
   sock_fh = socket(AF_INET, SOCK_STREAM, 0);
   if (sock_fh <= 0)
   {
#ifndef _UNIX_
      WSACleanup();
#endif
      return(ERR_NTWK_FAIL);
   }

   /* converts a web name (like www.xxx.com) to an ip address */
   host_ptr = gethostbyname(web);
   if (host_ptr == NULL)
      return(ERR_CONN_FAIL);

   /* ..there may be multiple ip address choices for this name */
   addr.sin_addr.s_addr = *(u_long *)host_ptr->h_addr_list[0];
   printf("IP Address: %lx\n", addr.sin_addr.s_addr);

   /* connect to destination */
   ret = connect(sock_fh, (struct sockaddr*)&addr, sizeof(struct sockaddr_in));
   if (ret == -1)
   {
      closesocket(sock_fh);
      sock_fh = 0;
#ifndef _UNIX_
      WSACleanup();
#endif
      return(ERR_BAD_IP);
   }

   return(ERR_NONE);
}
#endif


/* closes the socket connection */
int NtwkExit(void)
{
   /* close the socket */
   if (sock_fh <= 0)
      return(ERR_BAD_SEQ);

   shutdown(sock_fh, 0);
   closesocket(sock_fh);
   sock_fh = 0;
#ifndef _UNIX_
   WSACleanup();
#endif
   return(ERR_NONE);
}

static unsigned char _temp[8*1024];

/* send data to the destination */
int NtwkSend(int length, unsigned char *datap)
{
   int rc;

   /* make sure that we have a valid socket handle */
   if (sock_fh <= 0)
      return(ERR_BAD_SEQ);

   /* move the data to an internal buffer */
   /* add in a length indicator */
   _temp[0] = (unsigned char)(length >> 8);
   _temp[1] = (unsigned char)(length);
   memcpy(&_temp[2], datap, length);
   length += 2;
   datap = _temp;

   /* set a minimum block size */
   if (length < 16)
       length = 16;

   /* send out the data block */
   while (length > 0)
   {
      /* allow for partial outgoing blocks */
      rc = send(sock_fh, (char *)datap, length, 0);
      if (rc <= 0)
         return(ERR_SEND_RECV);

      length -= rc;
      datap += rc;
   }

   return(0);
}


/* recv data from the destination */
/* if the return length is the same as the size of the buffer */
/* then, it is possible that there is more data to receive */
int NtwkRecv(int length, unsigned char *datap)
{
   int max;

   /* make sure that we have a valid socket handle */
   if (sock_fh <= 0)
       return(ERR_BAD_SEQ);

   /* the first two bytes is a length indicator */
   max = recv(sock_fh, (char *)_temp, 2, 0);
   if (max < 2)
       return(ERR_SEND_RECV);

   max = (int)_temp[0] << 8;
   max |= (int)_temp[1];

   /* set a minimum block length */
   if (max < 16-2)
       max = 16-2;

   if (max > length)
      max = length;

   /* get an incoming data block up to size "length" */
   length = recv(sock_fh, (char *)datap, max, 0);

   if (length < 0)
       return(ERR_SEND_RECV);

   return(length);
}
