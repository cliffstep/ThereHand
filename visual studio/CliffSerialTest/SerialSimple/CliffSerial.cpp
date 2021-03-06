#include "CliffSerial.h"


CliffSerial::CliffSerial(){
}

CliffSerial::~CliffSerial() {
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
					//cout << "Cortado inicio ruim: " << permanentBuffer << "\n";		// Print corrected buffer

																					//cout << "Final em: " << foundout << '\n';									// Print end point of string
					string dataBufferString = permanentBuffer.substr(1, foundout);	// Take the package data
					int sizebuf = dataBufferString.length();									// Calculate the size of the package
					dataBufferString.erase(remove(dataBufferString.begin(), dataBufferString.end(), '\x1c'), dataBufferString.end());
					//cout << "Tamanho do buffer " << sizebuf << '\n';							// ... and print it
														// If the size is the correct one
						vector<string> strdados = split(dataBufferString, ',');
						//cout << "\nTESTE    " << strdados[0] << " TESTE " << strdados.size() <<"\n";
						if (strdados.size() == 10) {

							try {
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

								dt.f = dt.f != dt.f ? 0 : dt.f;
								dt.f = dt.f != dt.f ? 0 : dt.f;
								accx.f = accx.f != accx.f ? 0 : accx.f;
								accy.f = accy.f != accy.f ? 0 : accy.f;
								accz.f = accz.f != accz.f ? 0 : accz.f;
								gyx.f = gyx.f != gyx.f ? 0 : gyx.f;
								gyy.f = gyy.f != gyy.f ? 0 : gyy.f;
								gyz.f = gyz.f != gyz.f ? 0 : gyz.f;
								pitch.f = pitch.f != pitch.f ? 0 : pitch.f;
								roll.f = roll.f != roll.f ? 0 : roll.f;

								dadosBiffer.dt = dt.f;
								dadosBiffer.accx = accx.f;
								dadosBiffer.accy = accy.f;
								dadosBiffer.accz = accz.f;
								dadosBiffer.gyx = gyx.f;
								dadosBiffer.gyy = gyy.f;
								dadosBiffer.gyz = gyz.f;
								dadosBiffer.pitch = pitch.f;
								dadosBiffer.roll = roll.f;
							}
							catch (...){
								
							}

							

							/*cout << "Delta t " << dt.f << '\n';
							cout << "AccX " << accx.f << '\n';
							cout << "AccY " << accy.f << '\n';
							cout << "AccZ " << accz.f << '\n';
							cout << "GyX " << gyx.f << '\n';
							cout << "GyY " << gyy.f << '\n';
							cout << "GyZ " << gyz.f << '\n';
							cout << "Pitch " << pitch.f << '\n';
							cout << "Roll " << roll.f << '\n';
							cout << "Yaw " << yaw.f << '\n';*/

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

dataBiffer CliffSerial::getData() {
	return dadosBiffer;

}






