//miniosc wrapper because raylib.h conflicts with windows.h
#define MINIOSC_IMPLEMENTATION
#include "miniosc.h"
#include <stdio.h>
#include <stdbool.h>

miniosc* osc;
//mobundle bundle;

const bool checkNumber(char* str)
{
	for (int i = 0; i < strlen(str); i++)
	{
		if (!isdigit(str[i]))
			return 1;
	}
	return 0;
}

const int initOSC(unsigned short rx, unsigned short tx, char* ip, int errorcode)
{
	osc = minioscInit(rx, tx, ip, &errorcode);
}

const void bundleOSCMessage(char* address, char* type, float message, mobundle bundle)
{
	minioscBundle(&bundle, address, type, message);
}

const void sendOSCBundle(mobundle bundle)
{
	minioscSendBundle(osc, &bundle);
}

const int sendOSCMessageFloat(char* address, float message)
{
	//printf("sending float: %s, %f\n", address, message);
	return minioscSend(osc, address, ",f", message);
}

const bool checkIP(char* ip)
{
	WSADATA wsaData;
	int iResult;

	unsigned long ulAddr = INADDR_NONE;

	ulAddr = inet_addr(ip);
	if (ulAddr == INADDR_NONE) {
		printf("inet_addr failed and returned INADDR_NONE\n");
		WSACleanup();
		PlaySound("SystemExclamation", NULL, SND_ASYNC);
		return 1;
	}

	if (ulAddr == INADDR_ANY) {
		printf("inet_addr failed and returned INADDR_ANY\n");
		WSACleanup();
		PlaySound("SystemExclamation", NULL, SND_ASYNC);
		return 1;
	}
	printf("inet_addr returned success\n");

	// Here we could implement code to retrieve each address and 
	// print out the hex bytes
	// for(i=0, ptr= (Char*) &ulAddr; i < 4; i++, ptr++) {

	WSACleanup();
	return 0;
}

const int checkPortString(char* port)
{
	if (checkNumber(port))
	{
		PlaySound("SystemExclamation", NULL, SND_ASYNC);
		return 1;
	}
		
	long buffer = strtol(port, NULL, 10);
	if (buffer < 0)
	{
		PlaySound("SystemExclamation", NULL, SND_ASYNC);
		printf("Port too small!\n");
		return 2;
	}
	else if (buffer > 65535)
	{
		PlaySound("SystemExclamation", NULL, SND_ASYNC);
		printf("Port too large!\n");
		return 3;
	}
	else
	{
		printf("Port check success!\n");
		return 0;
	}
}

const int checkPortInt(int port)
{
	if (port < 0)
	{
		PlaySound("SystemExclamation", NULL, SND_ASYNC);
		printf("Port too small!\n");
		return 2;
	}
	else if (port > 65535)
	{
		PlaySound("SystemExclamation", NULL, SND_ASYNC);
		printf("Port too large!\n");
		return 3;
	}
	else
	{
		printf("Port check success!\n");
		return 0;
	}
}

const bool checkBeatrateString(char* beatrate)
{
	if (checkNumber(beatrate))
	{
		PlaySound("SystemExclamation", NULL, SND_ASYNC);
		//printf("Beatrate is not number!\n");
		return 1;
	}
	long buffer = strtol(beatrate, NULL, 10);
	if (buffer < 1)
	{
		PlaySound("SystemExclamation", NULL, SND_ASYNC);
		//printf("Beatrate out of range!\n");
		return 2;
	}
	else if (buffer > 16)
	{
		PlaySound("SystemExclamation", NULL, SND_ASYNC);
		//printf("Beatrate too fast!\n");
		return 3;
	}
	else
	{
		//printf("beatrate check success!\n");
		return 0;
	}
}

const bool checkBeatrateInt(long beatrate)
{

	if (beatrate < 1)
	{
		PlaySound("SystemExclamation", NULL, SND_ASYNC);
		printf("Beatrate out of range!\n");
		return 2;
	}
	else if (beatrate > 16)
	{
		PlaySound("SystemExclamation", NULL, SND_ASYNC);
		printf("Beatrate too fast!\n");
		return 3;
	}
	else
	{
		printf("beatrate check success!\n");
		return 0;
	}
}
