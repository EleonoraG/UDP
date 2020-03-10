/*
 ============================================================================
 Name        : UDPCSInteraction.c
 Author      : Eleonora Ghizzota
 Version     :
 Copyright   : Your copyright notice
 Description : Hello World in C, Ansi-style
 ============================================================================
 */

#if defined WIN32
#include <winsock.h>
#else
#define closesocket close
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <netdb.h>
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "UDPCSInteraction.h"
#define ECHOMAX 255
#define PORT 48000


void ErrorHandler(char *errorMessage) {
	printf ("%s", errorMessage);
}

///////////////////////////////

void ClearWinSock() {
	#if defined WIN32
	WSACleanup();
	#endif
}

///////////////////////////////

int createPort(int portNumber) {

	if (portNumber < 0) {
		ErrorHandler("bad port number\n");
		return 0;
	} else {
		printf("Port: %d OK\n", portNumber);
		return portNumber;
	}
}

///////////////////////////////

int createSocket() {

	int mySocket = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP);

	if (mySocket < 0) {
		ErrorHandler("Socket creation failed.\n");
		return -1;
	} else {
		printf("Socket creation succeeded!\n");
		return mySocket;
	}
}

///////////////////////////////

struct sockaddr_in initSockAddr(int family, char* addr, int port){

	struct sockaddr_in sad;
	memset(&sad, 0, sizeof(sad)); // ensures that extra bytes contain 0
	sad.sin_family = family;
	sad.sin_addr.s_addr = inet_addr(addr);
	sad.sin_port = htons(port);
	return sad;
}

///////////////////////////////

struct sockaddr_in initSockAddr_Addr(int family, struct in_addr * ina, int port){

	struct sockaddr_in sad;
	memset(&sad, 0, sizeof(sad)); // ensures that extra bytes contain 0
	sad.sin_family = family;
	sad.sin_addr.s_addr = ina->s_addr;
	sad.sin_port = htons(port);
	return sad;
}

///////////////////////////////

void assignSockIP(int mySocket, struct sockaddr_in sad) {

	if ((bind(mySocket, (struct sockaddr *) &sad, sizeof(sad))) < 0) {
		ErrorHandler("bind() failed");
	}
}

///////////////////////////////

void sending(int mySocket, char buf[ECHOMAX], int echoLen, int flags, struct sockaddr_in echoAddr) {
	if (sendto(mySocket, buf, echoLen, flags, (struct sockaddr *)&echoAddr, sizeof(echoAddr)) != echoLen) {
		ErrorHandler("sendto() sent different number of bytes than expected\n");
	}
}

///////////////////////////////

int receiving(int mySocket, int flags, char buf[ECHOMAX], struct sockaddr_in * echoAddr) {
	int addrLen = sizeof(struct sockaddr_in);
	int recvMsgSize = recvfrom(mySocket, buf, ECHOMAX, flags, (struct sockaddr*) echoAddr, &addrLen);
	buf[recvMsgSize] = '\0'; //add /0 so printf knows where to stop
	return recvMsgSize;
}

///////////////////////////////

int countVowels(char buf[ECHOMAX], char vowelsToSend[ECHOMAX]) {

	  int i = 0;
	  int v = 0;
	  while (i < strlen(buf)) {

	    switch(buf[i]) {
	    case 'A':
	    case 'a':
	    case 'E':
	    case 'e':
	    case 'I':
	    case 'i':
	    case 'O':
	    case 'o':
	    case 'U':
	    case 'u':
	    	vowelsToSend[v] = buf[i];
	        v++;
	        i++;
	        break;

	    default:
	        i++;
	        break;
	    }
	  }

	  vowelsToSend[v] = '\0';
	  return v;

}

///////////////////////////////

void sendVowels(int vowelsNum, char vowelsToSend[ECHOMAX], int mySocket, int flags, struct sockaddr_in echoAddr) {

	int i = 0;
	while (i < vowelsNum) {

		char * v;
		sprintf(v, "%c", vowelsToSend[i]);
		sending(mySocket, v, strlen(v), flags, echoAddr);
		i++;

	}
}

///////////////////////////////

void recvVowels(int vowelsNum, int recvMsgSize, int mySocket, int flags, char buf[ECHOMAX], struct sockaddr_in * echoAddr) {

	int i = 0;
	while (i < vowelsNum) {
		recvMsgSize = receiving(mySocket, flags, buf, echoAddr);
		printf("%s \n", buf);
		i++;
	}

}

///////////////////////////////

