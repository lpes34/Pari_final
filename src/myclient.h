/**
 *      @file  myclient.h
 *      @brief  header file for a more advanced TCP client
 *
 * Detailed description starts here.
 *
 *     @author  aaa aaa, aaaaaa@ua.pt
 *
 * =====================================================================================
 */

#ifndef _MYCLIENT_H_
#define _MYCLIENT_H_

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <fcntl.h>
#include <sys/time.h>
#include <arpa/inet.h>

typedef enum {NOTAVAILABLE=0, PORTCLOSED, PORTOPEN} srv_status;

#include "prototypes.h"

#endif
