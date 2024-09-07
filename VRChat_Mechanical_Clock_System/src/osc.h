//miniosc wrapper because raylib.h conflicts with windows.h
#pragma once
#define MINIOSCBUFFER 1536 //check miniosc.h for any changes!
typedef struct miniosc_t
{
	int sock;
	int portout;
} miniosc;

typedef struct
{
	char* bundleplace;
	char bundledata[MINIOSCBUFFER];
} mobundle;

const int initOSC(unsigned short rx, unsigned short tx, char* ip, int errorcode);

const int sendOSCMessageFloat(char* address, float message);

const void bundleOSCMessage(char* address, char* type, float message, mobundle bundle);

const void sendOSCBundle(mobundle bundle);

const bool checkIP(char* ip);

const int checkPortString(char* port);

const int checkPortInt(int port);

const bool checkBeatrateString(char* beatrate);

const bool checkBeatrateInt(long beatrate);

const bool checkNumber(char* str);