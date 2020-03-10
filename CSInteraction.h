/*
 ============================================================================
 Name        : CSInteraction.h
 Author      : Eleonora Ghizzota
 Version     :
 Copyright   : Your copyright notice
 Description : UDP Server-Client interaction library
 ============================================================================
 */

#ifndef CSINTERACTION_H_
#define CSINTERACTION_H_

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
#include <ctype.h>

void ErrorHandler();
void ClearWinSock();
int createPort();
int createSocket();
struct sockaddr_in initSockAddr();
struct sockaddr_in initSockAddr_Addr(); //per gethostbyname()
void assignSockIP ();
void sending();
int receiving();
int countVowels();
void sendVowels();
void recvVowels();


#endif /* CSINTERACTION_H_ */
