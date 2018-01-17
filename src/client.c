#include <stdio.h>		//puts, printf, etc.
#include <string.h>		//strlen
#include <sys/socket.h>		//socket, connect, send, recv, etc.
#include <arpa/inet.h>		//inet_addr
#include <unistd.h>		//close
#include <netdb.h>
#include <stdlib.h>
#include <myutils.h>
#include <ncurses.h>

#define MAXCHARS 4096		//just a large buffer for data chunks
#define _MAIN_C_


#include "myclient.h"

int
main (int argc, char *argv[])
{
  struct timeval tv;
  srv_status retval;


  char message[20];
  int port = 50000;
  char addr[20];

  sprintf (addr, "192.168.0.10");

  tv.tv_sec = 0;
  tv.tv_usec = 600000;
  retval = pari_TestServer (addr, port, &tv);

  char *Dir1 = malloc (1);
  char *Dir2 = malloc (1);
  int *left = malloc (sizeof (int));
  int *right = malloc (sizeof (int));
  if (!Dir1 || !Dir2 || !left || !right)
    return 0;

  *Dir1 = 'F';
  *Dir2 = 'F';
  *left = 0;
  *right = 0;

  sprintf (message, "MM%s%d-%s%d>", Dir1, *left, Dir2, *right);


#if 1
  int ret;
  int ch;

  initscr ();			/* Start curses mode              */
noecho();

  switch (retval)
    {
    case NOTAVAILABLE:
      printw ("%s:%d - Offline\n", addr, port);
      return 0;
      break;
    case PORTCLOSED:
      printw ("%s:%d - Closed\n", addr, port);
      break;
      return 0;
    case PORTOPEN:
      printw ("%s:%d - Open\n", addr, port);
      break;
    }


  printw ("Waiting for you!!!\n");	// Print Message
  refresh ();			/* Print it on to the real screen */

  do
    {

      char server_reply[MAXCHARS + 1] = { 0 };




      if (!ch)
	{
	  ch = getch ();	/* Wait for user input */
	}
      /*
         if(ch>0)  //a key was pressed
         {
         printw("Input Char Is :%c \n",ch); // Print Message

         } */

      if (ch == '\033')
	{			// if the first value is esc
	  getch ();		// skip the [
	  switch (getch ())
	    {			// the real value
	    case 'A':
	      mvprintw (1,0,"arrow up\n");
		
		CreateNewMessage ('A', Dir1, Dir2, left, right, message);
	      //sprintf (message, "MMF20-F20>");
		mvprintw (2,0,"message:%s\n",message);
	      ch =
		pari_AskDataFromServer (addr, port, message, server_reply,
					MAXCHARS);
	      break;
	    case 'B':
	     mvprintw (1,0,"arrow down\n");
		
	      CreateNewMessage ('B', Dir1, Dir2, left, right, message);
	      //sprintf (message, "MMB20-B20>");
		mvprintw (2,0,"message:%s\n",message);
	      ch =
		pari_AskDataFromServer (addr, port, message, server_reply,
					MAXCHARS);
	      break;
	    case 'C':
	      mvprintw (1,0,"code for arrow right\n");
	      CreateNewMessage ('C', Dir1, Dir2, left, right, message);
	      //sprintf (message, "MMB20-B20>");
		mvprintw (2,0,"message:%s\n",message);
	      ch =
		pari_AskDataFromServer (addr, port, message, server_reply,
					MAXCHARS);
	      break;
	    case 'D':
	      mvprintw (1,0," code for arrow left\n");
	      CreateNewMessage ('D', Dir1, Dir2, left, right, message);
	      //sprintf (message, "MMB20-B20>");
		mvprintw (2,0,"message:%s\n",message);
	      ch =
		pari_AskDataFromServer (addr, port, message, server_reply,
					MAXCHARS);
	      break;
	    }
	}
	else if(ch==' ')
	{
		mvprintw (1,0," code for encorder\n");
		pari_AskDataFromServer (addr, port, "EN>", server_reply,
					MAXCHARS);
		
	}
      refresh ();
    }
  while (ch != 'q');


  printf ("Input Char Is :%c\n", ch);




  //if (ret)
  //printf ("%s\n", server_reply);

#endif
  free (Dir1);
  free (Dir2);
  free (left);
  free (right);
	endwin();
  return 0;
}
