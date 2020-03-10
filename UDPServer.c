/*
 ============================================================================
 Name        : UDPServer.c
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
#endif
#include <stdio.h>
#include <string.h> /* for memset() */
#include "UDPCSInteraction.h"
#define ECHOMAX 255
#define PORT 48000
#define FLAG 0

int main() {

#if defined WIN32

	WSADATA wsaData;
	int iResult = WSAStartup(MAKEWORD(2 ,2), &wsaData);
	if (iResult != 0) {
		printf ("error at WSASturtup\n");
		return -1;
	}

#endif

	int SSocket;
	int port;
	struct sockaddr_in echoServAddr;
	struct sockaddr_in echoClntAddr;
	int cliAddrLen;
	char echoBuffer[ECHOMAX];

	port = createPort(PORT);

// CREAZIONE DELLA SOCKET
	SSocket = createSocket();

	printf(" \n");

// COSTRUZIONE DELL'INDIRIZZO DEL SERVER
	echoServAddr = initSockAddr(AF_INET, "127.0.0.1", PORT);

// BIND DELLA SOCKET
	assignSockIP(SSocket, echoServAddr);


	int recvMsgSize;

	while(1) {

		// RICEZIONE DELLA STRINGA ECHO DAL CLIENT
			cliAddrLen = sizeof(echoClntAddr);
			recvMsgSize = receiving(SSocket, FLAG, echoBuffer, &echoClntAddr);
			// IDK
			char * ip = inet_ntoa(echoClntAddr.sin_addr);
			char * hostName;
			struct hostent * host;
			struct in_addr addr;
			addr.s_addr = inet_addr(ip);
			host = gethostbyaddr((char *) &addr, 4, AF_INET);

			if(host == NULL) {
				hostName = "N/A";
			} else {
				hostName = host->h_name;
			}

			printf("%s ricevuto dal client con nomehost: %s \n", echoBuffer, hostName);

		//INVIO OK
			char * ok = "OK";
			sending(SSocket, ok, strlen(ok), FLAG, echoClntAddr);

		//RICEVO NUMERO DI VOCALI
			recvMsgSize = receiving(SSocket, FLAG, echoBuffer, &echoClntAddr);

			int vowelsToRecv = strtol(echoBuffer, NULL, 10);
			printf("Attendo %d vocali da convertire...\n", vowelsToRecv);

		// RICEVO VOCALI E INVIO VOCALI CONVERTITE
			char vowelsBuffer[ECHOMAX];

			int i = 0;
			char * uprVowel;
			while(i < vowelsToRecv) {
				recvMsgSize = receiving(SSocket, FLAG, vowelsBuffer, &echoClntAddr);
				uprVowel = strupr(vowelsBuffer);
				sending(SSocket, uprVowel, strlen(vowelsBuffer), FLAG, echoClntAddr);
				i++;
			}

			printf("Richiesta inoltrata da %s completata \n", hostName);
			printf(" \n");
			printf("In attesa di una richiesta... \n");
			printf(" \n");

		}
}
