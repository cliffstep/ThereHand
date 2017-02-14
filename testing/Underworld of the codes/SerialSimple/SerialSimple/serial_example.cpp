#include <stdio.h>
#include <conio.h>
#include <string.h>

#include <iostream>
#include <windows.h>
using namespace std;


char input[5];
DCB dcBus;
HANDLE hSerial;
DWORD  bytesRead, eventMask;
COMMTIMEOUTS timeouts;

void startSerial() {

	hSerial = CreateFile("\\\\.\\COM28", GENERIC_READ, 0, NULL, OPEN_EXISTING, 0, NULL);
	if (hSerial == INVALID_HANDLE_VALUE)
	{
		cout << "error opening handle\n";
	}
	else
	{
		cout << "port opened\n";
	}
	dcBus.DCBlength = sizeof(dcBus);
	if ((GetCommState(hSerial, &dcBus) == 0))
	{
		cout << "error getting comm state\n";
	}
	dcBus.BaudRate = CBR_115200;
	dcBus.ByteSize = DATABITS_8;
	dcBus.Parity = NOPARITY;
	dcBus.StopBits = ONESTOPBIT;
	if ((GetCommState(hSerial, &dcBus) == 0))
	{
		cout << "error setting comm state\n";
	}
	if ((GetCommTimeouts(hSerial, &timeouts) == 0))
	{
		cout << "error getting timeouts\n";
	}
	timeouts.ReadIntervalTimeout = 10;
	timeouts.ReadTotalTimeoutMultiplier = 1;
	timeouts.ReadTotalTimeoutConstant = 500;
	timeouts.WriteTotalTimeoutMultiplier = 1;
	timeouts.WriteTotalTimeoutConstant = 500;
	if (SetCommTimeouts(hSerial, &timeouts) == 0)
	{
		cout << "error setting timeouts\n";
	}
	if (SetCommMask(hSerial, EV_RXCHAR) == 0)
	{
		cout << "error setting comm mask\n";
	}
}

void stopSerial() {
	CloseHandle(hSerial);
}

void serialRead(){


	if (WaitCommEvent(hSerial, &eventMask, NULL))
	{
		if (ReadFile(hSerial, &input, 5, &bytesRead, NULL) != 0)
		{
			for (int i = 0; i < sizeof(input); i++)
			{
				cout << input[i];
			}
			//cout << endl;
		}
		else
		{
			cout << "error reading file\n";
		}
	}
	else
	{
		cout << "error waiting for comm event\n";
	}

}


int main(int argc, char **argv) {
	startSerial();
	while (true) {
		serialRead();
		//Sleep(1);
	}
}


