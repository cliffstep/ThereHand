#pragma once

#include <string>
#include <windows.h>
class CliffSerial
{
public:
	CliffSerial();
	int startSerial(LPCSTR = "\\\\.\\COM29");
	int readSerial();
	int stopSerial();
	~CliffSerial();
};

