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
	int startSerial(LPCSTR = "\\\\.\\COM3");
	int readSerial();
	int stopSerial();
	dataBiffer getData();
	~CliffSerial();

private:

	union {
		float f;
		unsigned char bytes[4];
	} dt, accx, accy, accz, gyx, gyy, gyz, yaw, pitch, roll;


	char input[80];
	string  permanentBuffer;
	DCB dcBus;
	HANDLE hSerial;
	DWORD  bytesRead, eventMask;
	COMMTIMEOUTS timeouts;
	dataBiffer dadosBiffer;


	template<typename Out>
	void split(const std::string &s, char delim, Out result) {
		std::stringstream ss;
		ss.str(s);
		std::string item;
		while (std::getline(ss, item, delim)) {
			*(result++) = item;
		}
	}

	std::vector<std::string> split(const std::string &s, char delim) {
		std::vector<std::string> elems;
		split(s, delim, std::back_inserter(elems));
		return elems;
	}

	//bool isFloat(string myString) {
	//	std::istringstream iss(myString);
	//	float f;
	//	iss >> noskipws >> f; // noskipws considers leading whitespace invalid
	//						  // Check the entire string was consumed and if either failbit or badbit is set
	//	return iss.eof() && !iss.fail();
	//}

};

