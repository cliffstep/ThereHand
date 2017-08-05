#include <stdio.h>
#include <conio.h>
#include <string.h>

#include <iostream>
#include <windows.h>

#include "CliffSerial.h"
using namespace std;
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

dataBiffer dados;

int main(int argc, char **argv) {
	CliffSerial serialHdl;
	serialHdl.startSerial();
	while (true) {
		serialHdl.readSerial();
		dados = serialHdl.getData();
		Sleep(1);
		cout << dados.pitch;
	}
}


