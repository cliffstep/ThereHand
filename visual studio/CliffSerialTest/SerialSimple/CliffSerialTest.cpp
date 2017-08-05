#include <stdio.h>
#include <conio.h>
#include <string.h>

#include <iostream>
#include <windows.h>

#include "CliffSerial.h"


int main(int argc, char **argv) {
	CliffSerial serialHdl;
	serialHdl.startSerial();
	while (true) {
		serialHdl.readSerial();
		Sleep(1);
	}
}


