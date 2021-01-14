// gcc -Wall -o client_udp client_udp.c [-lsocket –lnsl]
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#define BUFFERSIZE 50
#define PORT_TCP 7923
#define PORT_UDP 4680
#define SERVER "87.118.112.203"
//#define SERVER "www.schorcht.net"

int main(int argc, const char* argv[]) {
	printf("Starting script.\n");
	int tcp = 0;

    if(tcp != 0 && tcp != 1){
        perror("no valid protocol! use either tcp or udp");
        return -1;
    }

	int sd;
	struct sockaddr_in saddr;
	socklen_t saddrlen;
	char sbuf[BUFFERSIZE];
	char rbuf[BUFFERSIZE];
	int sbuflen;
	int rbuflen;

	strcpy(sbuf, "Danny Steinbrecher");

    bzero ( ( char * ) &saddr, sizeof ( saddr ) );
    saddr.sin_family = AF_INET;
    saddr.sin_addr.s_addr = inet_addr ( SERVER );
    saddr.sin_port = htons (PORT_TCP);

    sbuflen = strlen(sbuf);
    saddrlen = sizeof(saddr);

    if ((sd = socket(PF_INET, SOCK_STREAM, 0)) < 0) {
        perror("socket");
        return -1;
    }

	if (tcp == 0) { 		// -------------------- UDP
		printf("UDP.\n");


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

	} else 		// -------------------- TCP
		printf("TCP.\n");

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

	}
    close(sd);
	return 0;
}

