#include <arpa/inet.h> //inet_addr
#include <myutils.h>
#include <ncurses.h>
#include <netdb.h>
#include <stdio.h> //puts, printf, etc.
#include <stdlib.h>
#include <string.h>     //strlen
#include <sys/socket.h> //socket, connect, send, recv, etc.
#include <sys/types.h>
#include <sys/un.h>
#include <unistd.h> //close

#define MAXCHARS 4096 // just a large buffer for data chunks
#define _MAIN_C_

#include "myclient.h"

int main(int argc, char *argv[]) {
  struct timeval tv;
  srv_status retval;

  char tmpfile[64];
  mkstemp(tmpfile);

  // Connect to arduino
  char message[20];
  int port = 50000;
  char addr2[20];
  sprintf(addr2, "192.168.0.10");

  tv.tv_sec = 0;
  tv.tv_usec = 600000;
  retval = pari_TestServer(addr2, port, &tv);

  //------------------

  // int ret;

  switch (retval) {
  case NOTAVAILABLE:
    printf("%s:%d - Offline\n", addr2, port);
    endwin();
    return 0;
    break;
  case PORTCLOSED:
    printf("%s:%d - Closed\n", addr2, port);
    endwin();
    break;
    return 0;
  case PORTOPEN:
    printf("%s:%d - Open\n", addr2, port);
    break;
  }

  printf("going to fork\n");
  pid_t pid = fork();

  int sock = socket(AF_UNIX, SOCK_STREAM, 0);

  struct sockaddr_un addr;
  memset(&addr, 0, sizeof(addr));
  addr.sun_family = AF_UNIX;
  strncpy(addr.sun_path, tmpfile, strlen(tmpfile));
  printf("created tmp file\n");

  if (sock == -1) {
    perror("error creating socket");
  }

  if (pid < 0) {
    perror("Fork faild");
  }

  // ------------CHILD-------------------

  if (pid == 0) // I am the child
  {
    char *Dir1 = malloc(1);
    char *Dir2 = malloc(1);
    int *left = malloc(sizeof(int));
    int *right = malloc(sizeof(int));
    if (!Dir1 || !Dir2 || !left || !right)
      return 0;

    *Dir1 = 'F';
    *Dir2 = 'F';
    *left = 0;
    *right = 0;

    printf("I am the child\n");
    sprintf(message, "MM%s%d-%s%d>", Dir1, *left, Dir2, *right);

    if (connect(sock, (struct sockaddr *)&addr, sizeof(addr)) == -1) {
      perror("child connect error");
      exit(-1);
    }

    initscr(); /* Start curses mode              */
    noecho();
    do {
      int ch;
      ch = getch(); // waits for user input

      if (ch == '\033') {  // if the first value is esc
        getch();           // skip the [
        switch (getch()) { // the real value
        case 'A':
          printw("arrow up\n");

          CreateNewMessage('A', Dir1, Dir2, left, right, message);
          printw("message:%s\n", message);
          break;
        case 'B':
          mvprintw(1, 0, "arrow down\n");

          CreateNewMessage('B', Dir1, Dir2, left, right, message);
          mvprintw(2, 0, "message:%s\n", message);

          break;
        case 'C':
          mvprintw(1, 0, "code for arrow right\n");
          CreateNewMessage('C', Dir1, Dir2, left, right, message);
          mvprintw(2, 0, "message:%s\n", message);

          break;
        case 'D':
          mvprintw(1, 0, " code for arrow left\n");
          CreateNewMessage('D', Dir1, Dir2, left, right, message);
          mvprintw(2, 0, "message:%s\n", message);

          break;
        }
      } else if (ch == ' ') // request encoder
      {
        mvprintw(1, 0, " code for encorder\n");
        sprintf(message, "EN>");
      }

      int ret;
      ret = send(sock, message, strlen(message), 0);
      if (ret < 0) {
        perror("Send failed\n");
        return 0;
      }
    } while (1);
    free(Dir1);
    free(Dir2);
    free(left);
    free(right);
    endwin();
    exit(0);
  }
  // ------------CHILD END-------------------

  // -----------Parent---------------

  if (bind(sock, (struct sockaddr *)&addr, sizeof(addr)) == -1) {
    perror("bind error");
    exit(-1);
  }

  if (listen(sock, 5) == -1) {
    perror("listen error");
    exit(-1);
  }

  int cl;
  if ((cl = accept(sock, NULL, NULL)) == -1) {
    perror("accept error");
    exit(-1);
  }
  int n = 0;
  do {
    char server_reply[MAXCHARS + 1] = {0};
    int rc = 256;
    int ret;
    char msg[256];
    char msg2[256];
    n++;
    printf("been here %d times\n", n);
    if ((rc = read(cl, msg, sizeof(msg))) > 0) {
      // sprintf(msg2,"%s",msg);
      printf("received bytes: %s\n", msg);
      // printf("received %u bytes: %.*s\n", rc, rc, msg);
    } else {
      printf("No bytes received.\n");
    }

    int sock2;

    // printf("received bytes after loop: %s\n", msg);

    sock2 = pari_AskDataFromServer(addr2, port, msg, server_reply, MAXCHARS);
    printw("msg to arduinos:%d%s\n", sock2, msg);

    if (sock2 == 0) {
      printf("Server connection error\n");
      return 0;
    }
    // close(sock2);

  } while (1);
}
