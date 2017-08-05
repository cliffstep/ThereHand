#pragma once

#include <string>
#include <windows.h>
class CliffSerial
{
public:
	CliffSerial();
	int startSerial(LPCSTR = "\\\\.\\COM28");
	int readSerial();
	int stopSerial();
	~CliffSerial();
};

