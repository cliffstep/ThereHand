#include <stdio.h>
#include <conio.h>
#include <string.h>
#include <string>
#include <iostream>
#include <windows.h>

#include "Serial.h"

using namespace std;

string  permanentBuffer;
int unpack(char* lpBuffer);

int main(int argc, char **argv) {
	CSerial serial;
	while (TRUE) {
		if (serial.Open(28, 115200))
		{
			char* lpBuffer = new char[500];
			int nBytesRead = serial.ReadData(lpBuffer, 500);
			cout << lpBuffer;
			unpack(lpBuffer);


			Sleep(1);

		}
	}
}

	int unpack(char* lpBuffer) {
		/* ------------- BRACE YOURSELF ---------------  */
		/* ---------- Ladies and gentleman ------------  */
		/* ----------- I present to you ---------------  */

		/* ********************************************* */
		/* -----------_____ M.A.B.U.A (FART)____ ------- */
		/* _ MARIGA'S AMAZING BUFFER UNPACKER ALGORITHM  *I       (FOR APLICATION in REAL TIME) */
		/* ********************************************* */

		permanentBuffer = permanentBuffer + string(lpBuffer);
		//cout << "\nBuffer original: " << permanentBuffer << "\n";
		size_t foundin = permanentBuffer.find("####");										// Search for package starting point

		if (foundin != std::string::npos) {													// found?
			//cout << "Inicio em: " << foundin << '\n';										// Print point of string
			permanentBuffer = permanentBuffer.substr(foundin);								// Everything that came before is from old packages (lost data, erase that)
			//cout << "Cortado inicio ruim: " << permanentBuffer << "\n";					// Print corrected buffer
			size_t foundout = permanentBuffer.find("@@@@");									// Search for end of package

			if (foundout != std::string::npos) {											// Found?
				//cout << "\nCortado inicio ruim: " << permanentBuffer << "\n";				// Print corrected buffer

				//cout << "Final em: " << foundout << '\n';									// Print end point of string
				//cout << "\nCortado"<<permanentBuffer.substr(4, foundout -4);
				string dataBufferString = permanentBuffer.substr(4, foundout-4);			// Take the package data
				int sizebuf = dataBufferString.length();									// Calculate the size of the package
				//cout << "\nTamanho do buffer " << sizebuf << '\n';						// ... and print it
				if (sizebuf == 40) {														// If the size is the correct one

				}
				else {
					permanentBuffer = "";													// Remove the processed package from the buffer (no loss of data)
					return 0;																// if not, return 0
				}
				cout << "\nPacote: " << dataBufferString << '\n';							// ... and print the package also
				permanentBuffer = "";
			}
			else {
				permanentBuffer = "";
				return 0;
			}
		}
		else {
			permanentBuffer = "";
			return 0;
		}
		cout << "\n";
		//cout << endl;

		delete[] lpBuffer;
}