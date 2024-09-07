#pragma once
#include <Windows.h>
#include <string.h>
//#include <MechanicalClockOSC/getTimes.h>

SYSTEMTIME getTimes(void)
{
	SYSTEMTIME time;
	GetLocalTime(&time);
	return time;
}

char* convertDow(unsigned short dow)
{
	switch (dow)
	{
	case 0:
		return "Sun.\0";
	case 1:
		return "Mon.\0";
	case 2:
		return "Tue.\0";
	case 3:
		return "Wed.\0";
	case 4:
		return "Thu.\0";
	case 5:
		return "Fri.\0";
	case 6:
		return "Sat.\0";
	default:
		exit(1);
	}
}