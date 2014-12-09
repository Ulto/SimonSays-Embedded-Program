#if 0

TCP/IP Network Module


General Overview:
- Provides a library of routines to access the ethernet connection


Specific Operations:
- For use by a slave system to block / wait for an incoming connection
- For use by a host to connect to a slave
- For use by both host/slave to send/recv data packets of any length or format


Notes:
- if this is to be compiled under linux,
  then add:  #define _UNIX_

- if this is to be compiled under windows,
  then add:  wsock32.lib  to the project linker/input files/additional dependencies


Initialize slave to receive:
- call:
  RecvInit(port number)
     where port number matches the transmiting application
     note: you will need to initiate the "slave" before starting the "host"

  NtwkWait()
     will block and wait for a host to connect to us

  NtwkRecv(length,buffer)
     pass in a pointer to a buffer, and the size of the largest possible block
     the function will return the number of bytes that have been transferred
     if there is more data, you should call NtwkRecv() again

- call:
  NtwkSend()
     to send a response,

- If error, or to close the connection
  call:
  NtwkExit()
     to terminate the connection


Initialize host to transmit:
- determine the ip address of the destination
  use:  ipconfig
  examine the IPv4 address

- determine the port number to connect to
  for general purpose host-slave messages, use 1500
  to "ping", use port 7

- call:
  SendInit(ip address, port number)
     where the ip address is the ascii text "192.168.0.141" or similar
     port number is (typically) 1500

  NtwkSend(length, buffer)
     and pass the buffer/size to be sent to the destination

- call:
  NtwkRecv()
     to wait for a response,

- If error, or to close the connection
  call:
  NtwkExit()
     to terminate the connection


#endif


/*
 * list of general error codes
 * used with the below network functions
 */
#define ERR_NONE          0   // no errors
#define ERR_BAD_IP       -1   // invalid network ip address
#define ERR_BAD_SEQ      -2   // wrong sequence / no socket conenction
#define ERR_NTWK_FAIL    -3   // misc network failure
#define ERR_CONN_FAIL    -4   // can not connect / bind / accept
#define ERR_SEND_RECV    -5   // send / recv fail


/* SendInit(char *ip_addrs, int port)
 * initialize the ethernet to connect to a slave
 * (will generate an error if the slave is not already up and running)
 *
 * input:
 *    ip_addrs = destination ip address (ascii text "192.168.0.141" or similar)
 *    int port = destination port number
 * returns:
 *    0 if successful
 */
int SendInit(char *ip_addrs, int port);


/* RecvInit(int port)
 * initialize the ethernet to wait for a connection
 * (will not block)
 *
 * input:
 *    int port = port number
 * returns:
 *    0 if successful
 */
int RecvInit(int port);


/* NtwkWait(void)
 * block and wait for an incoming connection request
 * (assumes that RecvInit() has already been called)
 *
 * input:
 *    none
 * returns:
 *    0 if successful
 */
int NtwkWait(void);


#ifndef _UNIX_
/* WebInit(char *wen)
 * initialize the ethernet to connect to a web page
 *
 * input:
 *    web = web page (ascii text such as www.google.com)
 * returns:
 *    0 if successful
 */
int WebInit(char *web);
#endif


/* NtwkExit(void)
 * closes the socket connection (for send or recv)
 *
 * input:
 *    none
 * returns:
 *    0 if successful
 */
int NtwkExit(void);


/* NtwkSend(int length, unsigned char *datap)
 * send data to a destination
 * (can be called by the host or slave)
 *
 * input:
 *    int length = number of bytes to send
 *    uchar *datap = pointer to the buffer to send out
 * returns:
 *    0 if successful
 */
int NtwkSend(int length, unsigned char *datap);


/* NtwkRecv(int length, unsigned char *datap)
 * get data from a destination
 * (can be called by the host or slave)
 * (if the incoming data is greater than the value "length")
 * (then, the function will only return a partial buffer)
 *
 * input:
 *    int length = max number of bytes to wait for
 *    uchar *datap = pointer to the receive buffer
 * returns:
 *    the number of bytes read into the user buffer
 *    or negative number (if failure)
 */
int NtwkRecv(int length, unsigned char *datap);
