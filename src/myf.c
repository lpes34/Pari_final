/**
 * @brief Test if a server is responding in a given port
 *
 * @param  adrr - String with the IP address
 * @param  port - Integet with the port number
 * @param  tv -   Address of a struct timeval with the timeout
 *                NB. On returning, the tv structure holds the time
 *                left after the connection attempt. Can be useful to assess
 *                the speed of the connection.
 * @return srv_status value (NOTAVAILABLE, PORTOPEN, PORTCLOSED, ...)
 */

#include <ncurses.h>
#include "myclient.h"
#define myTIME 1		/*in ms */

srv_status
pari_TestServer (char *addr, int port, struct timeval *tv)
{
  struct sockaddr_in address;	/* the libc network address data structure */
  int sock;			/* file descriptor for the network socket */
  srv_status ret = NOTAVAILABLE;	/*default value */
  fd_set fdset;			/* set of sockets to test. Here only 1 */
  int so_error;
  int len = sizeof (so_error);

  sock = socket (AF_INET, SOCK_STREAM, 0);
  if (!sock)
    {
      printf ("Could not create socket\n");
      return ret;
    }
  fcntl (sock, F_SETFL, O_NONBLOCK);
  address.sin_family = AF_INET;
  address.sin_port = htons (port);	/* translate int2port num */
  address.sin_addr.s_addr = inet_addr (addr);	/* assign the address */

  connect (sock, (struct sockaddr *) &address, sizeof (address));

  FD_ZERO (&fdset);		//clear set of sockets to test
  FD_SET (sock, &fdset);	//insert the socket to test

  if (select (sock + 1, NULL, &fdset, NULL, tv) == 1)
    {
      getsockopt (sock, SOL_SOCKET, SO_ERROR, &so_error, &len);
      if (so_error == 0)
	ret = PORTOPEN;
      else
	ret = PORTCLOSED;
    }

  close (sock);
  return ret;
}


/**
 * @brief  Sends a message to server and feteches the reply
 *
 * @param  ip string with the server IP (xxx.xxx.xxx.xxx)
 * @param  port Integer with the port number
 * @param  message String with message to send to server
 * @param  server_reply String with message given by the server
 * @param  maxChars maximal size of server reply
 *
 * @return 1 in case of sucess. 0 for error.
 */


int
pari_AskDataFromServer (char *ip, int port, char *message, char *server_reply,
			int maxChars)
{
  int sock;
  int ret;
  int ch= -1;
  struct sockaddr_in server;

  bzero (&server, sizeof (server));	//fill with zeros
  server.sin_addr.s_addr = inet_addr (ip);
  server.sin_family = AF_INET;
  server.sin_port = htons (port);

  //Create a socket for communications
  sock = socket (AF_INET, SOCK_STREAM, 0);
  if (sock == -1)
    {
      printf ("Could not create socket\n");
      return 0;
    }

  //Connect to remote server using the created socket
  ret = connect (sock, (struct sockaddr *) &server, sizeof (server));
  if (ret < 0)
    {
      printf ("connect error\n");
      return 0;
    }

  //send the query message
  ret = send (sock, message, strlen (message), 0);
  if (ret < 0)
    {
      printf ("Send failed\n");
      return 0;
    }
  int n = 4;
	int i=0;
  //receive the answer up to a maximal size...
  while (i<200)
    {

      bzero (server_reply, maxChars);	//important when reading repeatidely
      
      //printw ("%d:teste\n", n);
//printw("nunber of cicles:%d\n",n);
      ret = recv (sock, server_reply, maxChars, MSG_DONTWAIT);
      //printw ("->%d<-", ret);
      /*if (ret < 0)
	{
	  printw ("recv failed\n");
	  return 0;
	}*/
	if(ret>0)
     { //printw ("cicle=%d:ret=%d\n", n,ret);
	n++;
      mvprintw (n,0,"%d:%s\n",n, server_reply);
	i=0;}
else
{i++;}
      //      printw("chi=%d\n",ch);
      //
      //printw("ch=%d\n",ch);
      /*if (n > 50)
	{
	  break;
	}*/

      refresh ();
    }

  //while (1);
ch = getch ();
printw("%d\n",ch);
  //close the socket before exiting
  close (sock);
  return ch;			//success
}

int
CreateNewMessage (int a, char *Dir1, char *Dir2, int *left, int *right,
		  char *message)
{

  int increment = 5;

  if (a == 'A')			//arrow up
    {
      if (*Dir1 == 'F')
	{
	  *left += increment;
	}
      else if (*left > increment)	//Dir1=='B'
	{
	  *left -= increment;
	}
      else
	{
	  *left = 0;
	  *Dir1 = 'F';
	}


      if (*Dir2 == 'F')
	{
	  *right += increment;
	}
      else if (*right > increment)	//Dir2=='B'
	{
	  *right -= increment;
	}
      else
	{
	  *right = 0;
	  *Dir2 = 'F';
	}
    }



  if (a == 'B')			//arrow down
    {
      if (*Dir1 == 'B')
	{
	  *left += increment;
	}
      else if (*left >= increment)	//Dir1=='F'
	{
	  *left -= increment;
	}
      else
	{
	  *left = 0;
	  *Dir1 = 'B';
	}


      if (*Dir2 == 'B')
	{
	  *right += increment;
	}
      else if (*right >= increment)	//Dir2=='B'
	{
	  *right -= increment;
	}
      else
	{
	  *right = 0;
	  *Dir2 = 'B';
	}
    }




  if (a == 'C')			//arrow right
    {
      if (*Dir1 == 'F')
	{
	  *left += increment;
	}
      else if (*left > increment)	//Dir1=='B'
	{
	  *left -= increment;
	}
      else
	{
	  *left = 0;
	  *Dir1 = 'F';
	}


      if (*Dir2 == 'B')
	{
	  *right += increment;
	}
      else if (*right >= increment)	//Dir2=='B'
	{
	  *right -= increment;
	}
      else
	{
	  *right = 0;
	  *Dir2 = 'B';
	}
    }

  if (a == 'D')			//arrow left
    {
      if (*Dir1 == 'B')
	{
	  *left += increment;
	}
      else if (*left >= increment)	//Dir1=='F'
	{
	  *left -= increment;
	}
      else
	{
	  *left = 0;
	  *Dir1 = 'B';
	}


      if (*Dir2 == 'F')
	{
	  *right += increment;
	}
      else if (*right > increment)	//Dir2=='B'
	{
	  *right -= increment;
	}
      else
	{
	  *right = 0;
	  *Dir2 = 'F';
	}
    }

  sprintf (message, "MM%s%d-%s%d>", Dir1, *left, Dir2, *right);
  return 1;
}
