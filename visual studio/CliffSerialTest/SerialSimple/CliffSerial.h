#pragma once

#include <string>
#include <windows.h>

struct dataBiffer {
	float dt;
	float accx;
	float accy;
	float accz;
	float gyx;
	float gyy;
	float gyz;
	float yaw;
	float pitch;
	float roll;
};

class CliffSerial
{

private:
	char input[80];
	string  permanentBuffer;
	DCB dcBus;
	HANDLE hSerial;
	DWORD  bytesRead, eventMask;
	COMMTIMEOUTS timeouts;
	dataBiffer dados;


public:
	CliffSerial();
	int startSerial(LPCSTR = "\\\\.\\COM28");
	int readSerial();
	int stopSerial();
	dataBiffer getData();
	~CliffSerial();


};

