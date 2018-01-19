#include <arpa/inet.h> //inet_addr
#include <netdb.h>
#include <stdio.h> //puts, printf, etc.
#include <stdlib.h>
#include <string.h>     //strlen
#include <sys/socket.h> //socket, connect, send, recv, etc.
#include <sys/types.h>
#include <sys/un.h>
//#include <unistd.h>   //close
#define MAXCHARS 4096 // just a large buffer for data chunks

int main() {

  int port = 50000;
  char ip[20];
  sprintf(ip, "192.168.0.10");
  int sock;
  int ret;
  char message[20];
  sprintf(message, "MMF20-F20>");
  // int ch = -1;
  struct sockaddr_in server;
  char server_reply[MAXCHARS + 1] = {0};

  bzero(&server, sizeof(server)); // fill with zeros
  server.sin_addr.s_addr = inet_addr(ip);
  server.sin_family = AF_INET;
  server.sin_port = htons(port);

  // Create a socket for communications
  sock = socket(AF_INET, SOCK_STREAM, 0);
  if (sock == -1) {
    printf("Could not create socket\n");
    return 0;
  }

  // Connect to remote server using the created socket
  ret = connect(sock, (struct sockaddr *)&server, sizeof(server));
  if (ret < 0) {
    printf("connect error\n");
    return 0;
  }

  // send the query message
  ret = send(sock, message, strlen(message), 0);
  if (ret < 0) {
    printf("Send failed\n");
    return 0;
  }
  printf("message to arduino:%s. Waiting response...\n", message);
  int n = 4;
  int i = 0;
  // receive the answer up to a maximal size...
  while (0 != strcmp(server_reply, "PASSED HERE!")) {

    bzero(server_reply, MAXCHARS); // important when reading repeatidely

    // printw ("%d:teste\n", n);
    // printw("nunber of cicles:%d\n",n);
    ret = recv(sock, server_reply, MAXCHARS, MSG_DONTWAIT);
    // printw ("->%d<-", ret);
    /*if (ret < 0)
       {
       printw ("recv failed\n");
       return 0;
       } */
    if (ret > 0) { // printw ("cicle=%d:ret=%d\n", n,ret);
      n++;
      printf("%d:%s\n", n, server_reply);
      i = 0;
    } else {
      i++;
    }
    //      printw("chi=%d\n",ch);
    //
    // printw("ch=%d\n",ch);
    /*if (n > 50)
       {
       break;
       } */

    // refresh();
  }

  // while (1);
  // ch = getch ();
  // printw("%d\n",ch);
  // close the socket before exiting
  close(sock);
  printf("sock closed\n");
  return sock; // success
}