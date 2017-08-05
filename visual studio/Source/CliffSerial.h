#pragma once

#include <string>
#include <windows.h>

#include <stdio.h>

#include <conio.h>
#include <string.h>

#include <iostream>
#include <windows.h>
#include <algorithm>
#include <sstream>
#include <vector>
#include <iterator>

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


using namespace std;

class CliffSerial
{
public:
	CliffSerial();
	int startSerial(LPCSTR = "\\\\.\\COM28");
	int readSerial();
	int stopSerial();
	dataBiffer getData();
	~CliffSerial();

private:

	char input[80];
	string  permanentBuffer;
	DCB dcBus;
	HANDLE hSerial;
	DWORD  bytesRead, eventMask;
	COMMTIMEOUTS timeouts;
	dataBiffer dadosBiffer;

};

