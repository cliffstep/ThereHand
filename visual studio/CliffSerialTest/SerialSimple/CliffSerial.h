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



public:
	CliffSerial();
	int startSerial(LPCSTR = "\\\\.\\COM28");
	int readSerial();
	int stopSerial();
	dataBiffer getData();
	~CliffSerial();


};

