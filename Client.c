/*
 ============================================================================
 Name        : Client.c
 Author      : Eleonora Ghizzota
 Version     :
 Copyright   : Your copyright notice
 Description : UDP Client implementation
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
#include "CSInteraction.h"
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

	int CSocket;
	int portNumber;
	int port;
	char * hostName;
	struct sockaddr_in echoServAddr;
	struct sockaddr_in fromAddr;
	char echoBuffer[ECHOMAX];
	int echoStringLen;

	// NOME DEL SERVER
	printf("Inserisci il nome del server: ");
	scanf("%s", hostName);
	if ((echoStringLen = strlen(hostName)) > ECHOMAX)
		ErrorHandler("echo word too long");

	struct hostent * host;
	struct in_addr * ina;
	host = gethostbyname(hostName);
	if (host == NULL) {
		fprintf(stderr, "gethostbyname() failed.\n");
		exit(EXIT_FAILURE);
	} else {
		ina = (struct in_addr*) host->h_addr_list[0];
		printf("Risultato di gethostbyname(%s): %s\n", hostName, inet_ntoa(*ina));
	}

	// NUMERO PORTA DEL SERVER
	printf("Inserisci il numero di porta: ");
	scanf("%d", &portNumber);

	port = createPort(portNumber);

	// CREAZIONE DELLA SOCKET
	CSocket = createSocket();

	printf(" \n");

	// COSTRUZIONE DELL'INDIRIZZO DEL SERVER
	echoServAddr = initSockAddr_Addr(PF_INET, ina, port);

	printf("Inserire un messaggio da inviare al server: ");
	scanf("%s", echoBuffer);
	if ((echoStringLen = strlen(echoBuffer)) > ECHOMAX)
			ErrorHandler("echo word too long");

	// INVIO DELLA STRINGA ECHO AL SERVER
	sending(CSocket, echoBuffer, strlen(echoBuffer), FLAG, echoServAddr);

	// RICEVO OK
	int fromSize;
	int respStringLen;
	fromSize = sizeof(fromAddr);
	respStringLen = receiving(CSocket, FLAG, echoBuffer, &fromAddr);

	if (echoServAddr.sin_addr.s_addr != fromAddr.sin_addr.s_addr) {
		fprintf(stderr, "Error: received a packet from unknown source.\n");
		exit(-1);
	} else {
		printf("Received: %s\n", echoBuffer);
	}

	printf("Inserire una stringa alfanumerica: ");
	scanf("%s", echoBuffer);

	int vowelsNum;
	char vowelsToSend[ECHOMAX];
	vowelsNum = countVowels(echoBuffer, vowelsToSend);

	printf("Ci sono %d vocali \n", vowelsNum);

	sprintf(echoBuffer, "%d", vowelsNum);

	// INVIO AL SERVER IL NUMERO DI VOCALI DA ASPETTARE
	sending(CSocket, echoBuffer, strlen(echoBuffer), FLAG, echoServAddr);

	// INVIO LE VOCALI AL SERVER
	sendVowels(vowelsNum, vowelsToSend, CSocket, FLAG, echoServAddr);

	recvVowels(vowelsNum, fromSize, CSocket, FLAG, echoBuffer, &fromAddr);

	printf(" \n");
	printf("CHIUSURA \n");
	printf(" \n");
	closesocket(CSocket);
	ClearWinSock();
	system("pause");
	return -1;
}
