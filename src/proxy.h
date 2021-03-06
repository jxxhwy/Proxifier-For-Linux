/* proxy.c declarations. 

Copyright (C) 2020  Mohith Reddy

Proxifier is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

Proxifier is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>. */

#ifndef SRC_PROXY_H_
#define SRC_PROXY_H_

#include <stdlib.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <signal.h>

/* Proxy Constants */


/* Proxy Errors */

#define PROXY_ERROR_NONE 0
#define PROXY_ERROR_RETRY 1
#define PROXY_ERROR_FATAL 2
#define PROXY_ERROR_INVAL 3
#define PROXY_ERROR_TIMEOUT 4
#define PROXY_ERROR_SIGRCVD 5
#define PROXY_ERROR_BUFFER_FULL 6
#define PROXY_ERROR_DNS 7

/* Proxy Defaults */

#define PROXY_DEFAULT_ACCEPT_BACKLOG 25

struct in_addr* get_lo_interface_in_addr();


#define PROXY_DEFAULT_SERVER_LISTENER inet_ntop(AF_INET, (void*) get_lo_interface_in_addr(), \
		(char*) malloc(INET_ADDRSTRLEN), INET_ADDRSTRLEN)

#define PROXY_DEFAULT_SYNC_SIGNAL SIGRTMIN

sigset_t* get_syncmask(void);

#define PROXY_DEFAULT_SYNC_MASK get_syncmask()

#define PROXY_DEFAULT_IOTIMEOUT 60
#define PROXY_DEFAULT_HTTP_METHOD "CONNECT"
#define PROXY_DEFAULT_CONNECT_ADDRESS "127.0.0.1:80"
#define PROXY_DEFAULT_HTTP_VERSION "1.1"

/* Proxy Protocols */
#define PROXY_PROTOCOL_HTTP 0
#define PROXY_PROTOCOL_DNS 1

/* Proxy Maxs */

#define PROXY_MAX_TRANSACTION_SIZE 65536

struct proxy_options {
	/* Proxy-Server information */
	char* px_server;
	char* px_port;
	char* px_username;
	char* px_password;
	/* Redirection variables */
	char** rd_ports;
	long nrd_ports;
	/* Network-Variables */
	long io_timeout;
	/* Signal Variables */
	sigset_t* sigmask;
	int signo;
};

struct proxy_handler {
	/* Proxy Settings */
	struct proxy_options* px_opt;
	/* Thread variables */
	pthread_t ptid;
	pthread_t tid;
	struct proxy_client* pxl_server;
	/* Variables for Synchronization */
	int quit;
	/* Protocol specific data */
	int protocol;
	void* proto_data;
};

struct proxy_request {
	/* Proxy_Settings */
	struct proxy_options* px_opt;
	/* Thread variables */
	pthread_t ptid;
	pthread_t tid;
	struct proxy_client* px_client;
	/* Variables for Synchronization */
	int quit;
	/* Variables for socket operations */
	int sockfd;
	struct sockaddr_storage addr;
	int addr_len;
	/* Protocol specific data */
	int protocol;
	void* proto_data;
};

typedef int (*protocol_data_free) (void**);

struct proxy_options* create_proxy_options(struct proxy_options* px_opt);

int free_proxy_options(struct proxy_options** _px_opt);

struct proxy_handler* create_proxy_handler();

int free_proxy_handler(struct proxy_handler** _px_handler);

struct proxy_request* create_proxy_request(struct proxy_handler* px_handler);

int free_proxy_request(struct proxy_request** px_request);

#endif /* SRC_PROXY_H_ */
