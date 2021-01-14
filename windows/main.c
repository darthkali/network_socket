// gcc -Wall -o client_udp client_udp.c [-lsocket –lnsl]

#ifndef _UNISTD_H
#define _UNISTD_H    1

/* This file intended to serve as a drop-in replacement for
 *  unistd.h on Windows.
 *  Please add functionality as neeeded.
 *  Original file from: http://stackoverflow.com/a/826027
 */

#include <stdlib.h>
#include <io.h>
#include <getopt.h> /* getopt at: https://gist.github.com/bikerm16/1b75e2dd20d839dcea58 */
#include <process.h> /* for getpid() and the exec..() family */
#include <direct.h> /* for _getcwd() and _chdir() */

#define srandom srand
#define random rand

/* Values for the second argument to access.
   These may be OR'd together.  */
#define R_OK    4       /* Test for read permission.  */
#define W_OK    2       /* Test for write permission.  */
#define X_OK    R_OK    /* execute permission - unsupported in Windows,
                           use R_OK instead. */
#define F_OK    0       /* Test for existence.  */

#define access _access
#define dup2 _dup2
#define execve _execve
#define ftruncate _chsize
#define unlink _unlink
#define fileno _fileno
#define getcwd _getcwd
#define chdir _chdir
#define isatty _isatty
#define lseek _lseek
/* read, write, and close are NOT being #defined here,
 * because while there are file handle specific versions for Windows,
 * they probably don't work for sockets.
 * You need to look at your app and consider whether
 * to call e.g. closesocket().
 */

#define ssize_t int

#define STDIN_FILENO 0
#define STDOUT_FILENO 1
#define STDERR_FILENO 2
/* should be in some equivalent to <sys/types.h> */
typedef __int8            int8_t;
typedef __int16           int16_t;
typedef __int32           int32_t;
typedef __int64           int64_t;
typedef unsigned __int8   uint8_t;
typedef unsigned __int16  uint16_t;
typedef unsigned __int32  uint32_t;
typedef unsigned __int64  uint64_t;

#endif /* unistd.h  */


#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <winsock2.h>


#define BUFFERSIZE 50
#define PORT_TCP 7923
#define PORT_UDP 4680
#define SERVER "87.118.112.203"
//#define SERVER "www.schorcht.net"

int main(int argc, const char* argv[]) {
	printf("Starting script.\n");
	int tcp = 0;
	int sd;
	struct sockaddr_in saddr;
	socklen_t saddrlen;
	char sbuf[BUFFERSIZE], rbuf[BUFFERSIZE];
	int sbuflen, rbuflen;

	strcpy(sbuf, "Danny Steinbrecher");

	if (tcp == 0) {
		// -------------------- UDP
		printf("UDP.\n");
        bzero ( ( char * ) &saddr, sizeof ( saddr ) );
 		saddr.sin_family = AF_INET;
 		saddr.sin_addr.s_addr = inet_addr ( SERVER );
		saddr.sin_port = htons(PORT_UDP);
		
		sbuflen = strlen(sbuf);
		saddrlen = sizeof(saddr);

		if ((sd = socket(PF_INET, SOCK_DGRAM, 0)) < 0) {
			perror("socket");
			return -1;
		}

		// Send my name
		if (sendto(sd, sbuf, sbuflen, 0, (struct sockaddr*)&saddr, saddrlen) != sbuflen) {
			perror("sendto");
				return -1;
		}

		// receive response
		if ((rbuflen = recvfrom(sd, rbuf, BUFFERSIZE, 0, (struct sockaddr*)&saddr, &saddrlen)) < 0) {
			perror("recvfrom");
			return -1;
		}
		rbuf[rbuflen] = 0;
		printf("%s\n", rbuf);

		// echo response		
		if (sendto(sd, rbuf, rbuflen, 0, (struct sockaddr*)&saddr, saddrlen) != rbuflen) {
			perror("sendto");
			return -1;
		}
		close(sd);

	} else if (tcp == 1) {
		// -------------------- TCP
		printf("TCP.\n");

		bzero ( ( char * ) &saddr, sizeof ( saddr ) );
        saddr.sin_family = AF_INET;
        saddr.sin_port = htons ( PORT_TCP );
        saddr.sin_addr.s_addr = inet_addr ( SERVER );

		sbuflen = strlen(sbuf);
		saddrlen = sizeof(saddr);
		
		if ((sd = socket(PF_INET, SOCK_STREAM, 0)) < 0) {
			perror("socket");
			return -1;
		}

		// Send my name
		if (connect(sd, (struct sockaddr*)&saddr, saddrlen) < 0) {
			perror("connect");
			return -1;
		}
		if (send(sd, sbuf, sbuflen, 0) != sbuflen) {
			perror("send");
			return -1;
		}

		// receive response
		if ((rbuflen = recv(sd, rbuf, BUFFERSIZE, 0)) < 0) {
			perror("recvfrom");
			return -1;
		}
		rbuf[rbuflen] = 0;
		printf("%s\n", rbuf);

		// echo response
		if (send(sd, rbuf, rbuflen, 0) != rbuflen) {
			perror("send");
			return -1;
		}
		close(sd);

	}
	else {
		perror("no valid protocol! use either tcp or udp");
	}
	return 0;
}

