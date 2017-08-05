#include "CliffSerial.h"

#include <stdio.h>

#include <conio.h>
#include <string.h>

#include <iostream>
#include <windows.h>
#include <algorithm>
#include <sstream>
#include <vector>
#include <iterator>

using namespace std;


char input[80];
string  permanentBuffer;
DCB dcBus;
HANDLE hSerial;
DWORD  bytesRead, eventMask;
COMMTIMEOUTS timeouts;

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

bool isFloat(string myString) {
	std::istringstream iss(myString);
	float f;
	iss >> noskipws >> f; // noskipws considers leading whitespace invalid
						  // Check the entire string was consumed and if either failbit or badbit is set
	return iss.eof() && !iss.fail();
}

union {
	float f;
	unsigned char bytes[4];
} dt, accx, accy, accz, gyx, gyy, gyz, yaw, pitch, roll;


CliffSerial::CliffSerial(){
}


int CliffSerial::startSerial(LPCSTR comPort) {

	hSerial = CreateFile(comPort, GENERIC_READ, 0, NULL, OPEN_EXISTING, 0, NULL);
	if (hSerial == INVALID_HANDLE_VALUE)
	{
		cout << "error opening handle\n";
		return -1;
	}
	else
	{
		cout << "port opened\n";
	}
	dcBus.DCBlength = sizeof(dcBus);
	if ((GetCommState(hSerial, &dcBus) == 0))
	{
		cout << "error getting comm state\n";
		return -2;
	}
	dcBus.BaudRate = CBR_115200;
	dcBus.ByteSize = DATABITS_8;
	dcBus.Parity = NOPARITY;
	dcBus.StopBits = ONESTOPBIT;
	if ((GetCommState(hSerial, &dcBus) == 0))
	{
		cout << "error setting comm state\n";
		return -3;
	}
	if ((GetCommTimeouts(hSerial, &timeouts) == 0))
	{
		cout << "error getting timeouts\n";
		return -4;
	}
	timeouts.ReadIntervalTimeout = 1;
	timeouts.ReadTotalTimeoutMultiplier = 1;
	timeouts.ReadTotalTimeoutConstant = 1;
	timeouts.WriteTotalTimeoutMultiplier = 1;
	timeouts.WriteTotalTimeoutConstant = 1;
	if (SetCommTimeouts(hSerial, &timeouts) == 0)
	{

		cout << "error setting timeouts\n";
		return -5;
	}
	if (SetCommMask(hSerial, EV_RXCHAR) == 0)
	{

		cout << "error setting comm mask\n";
		return -6;
	}
	return 1;
}

int CliffSerial::readSerial() {
	if (WaitCommEvent(hSerial, &eventMask, NULL)){
		if (ReadFile(hSerial, &input, sizeof(input), &bytesRead, NULL) != 0)
		{
		/*	for (int i = 0; i < sizeof(input); i++){
				cout <<  input[i];
			}
			cout << "\n";*/


			/* ------------- BRACE YOURSELF ---------------  */
			/* ---------- Ladies and gentleman ------------  */
			/* ----------- I present to you ---------------  */

			/* ********************************************* */
			/* -----------_____ M.A.B.U.A (FART)____ ------- */
			/* _ MARIGA'S AMAZING BUFFER UNPACKER ALGORITHM  *I       (FOR APLICATION in REAL TIME) */
			/* ********************************************* */

			permanentBuffer = permanentBuffer + string(input);
			//permanentBuffer = string(input);
			//cout << "Buffer original: " << permanentBuffer << "\n";
			size_t foundin = permanentBuffer.find("#");							// Search for package starting point

			if (foundin != std::string::npos) {										// found?
				//cout << "Inicio em: " << foundin << '\n';							// Print point of string
				permanentBuffer = permanentBuffer.substr(foundin);				    // Everything that came before is from old packages (lost data, erase that)
																					//cout << "Cortado inicio ruim: " << permanentBuffer << "\n";		// Print corrected buffer
				size_t foundout = permanentBuffer.find("@");						// Search for end of package

				if (foundout != std::string::npos) {											// Found?
					cout << "Cortado inicio ruim: " << permanentBuffer << "\n";		// Print corrected buffer

																					//cout << "Final em: " << foundout << '\n';									// Print end point of string
					string dataBufferString = permanentBuffer.substr(1, foundout);	// Take the package data
					int sizebuf = dataBufferString.length();									// Calculate the size of the package
					dataBufferString.erase(remove(dataBufferString.begin(), dataBufferString.end(), '\x1c'), dataBufferString.end());
					cout << "Tamanho do buffer " << sizebuf << '\n';							// ... and print it
														// If the size is the correct one
						vector<string> strdados = split(dataBufferString, ',');
						cout << "\nTESTE    " << strdados[0] << " TESTE " << strdados.size() <<"\n";
						if (strdados.size() == 10) {

							dt.f = stof(strdados[0]);

							accx.f = stof(strdados[1]);
							accy.f = stof(strdados[2]);
							accz.f = stof(strdados[3]);

							gyx.f = stof(strdados[4]);
							gyy.f = stof(strdados[5]);
							gyz.f = stof(strdados[6]);

							pitch.f = stof(strdados[7]);
							roll.f = stof(strdados[8]);
							yaw.f = stof(strdados[9]);

							cout << "Delta t " << dt.f << '\n';
							cout << "AccX " << accx.f << '\n';
							cout << "AccY " << accy.f << '\n';
							cout << "AccZ " << accz.f << '\n';
							cout << "GyX " << gyx.f << '\n';
							cout << "GyY " << gyy.f << '\n';
							cout << "GyZ " << gyz.f << '\n';
							cout << "Pitch " << pitch.f << '\n';
							cout << "Roll " << roll.f << '\n';
							cout << "Yaw " << yaw.f << '\n';

						}
						else {

						}
						permanentBuffer = "";

				}
				else {
					return 0;
				}

			}
			else {
				return 0;
			}


		}
		else
		{
			cout << "error reading file\n";
			return -1;
		}
	}else{
		cout << "error waiting for comm event\n";
		return -2;
	}
	return 1;

}

int CliffSerial::stopSerial() {
	CloseHandle(hSerial);
	return 1;
}


CliffSerial::~CliffSerial(){
}




