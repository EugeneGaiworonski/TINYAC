using namespace std;
#define RAMsize 16
#include <bitset>
#include <ctype.h>
#include <iostream>
#include <sstream>
#include <iomanip>
#include <cstdint>
#include <conio.h>
#include <string>
#include <vector>
#include <windows.h>
#include <limits.h>
#include "tiniac.h"
#include "processor.h"
#include "console.h"


int main () 
{
	Processor 			Device;
	Console 		Con;
	unsigned char 	key = {0};
	
	//главный цикл
	cout <<"\nPOWER ON\n";
	do{
		Con.setConsoleTitle(Device.mode);
		if(_kbhit()) 
			key=getch();
		switch(toupper(key)) {
			case '`': // вызов консоли
				Device.mode = sStop;
				Con.doConsole(Device);
				break;
			case 'A':	
				Device.mode = sAuto;
				break;
			case 'S':	
				Device.mode = sStop;
				break;
			case 'T':	
				Device.mode = sStep;
				break;
			case 'Q':
				Device.mode = sQuit;
				break;	
			case ' ': // выполнить команду в шаговом режиме
				if (Device.mode == sStep) {
					Device.doCommand();
					if(Con.Hmode) {
						cout << "\nIP:"<< uppercase << setw(2) << setfill('0') << hex << (short)Device.IP; 
						cout <<" CR:"<< uppercase << setw(4) << setfill('0') << hex << (short)Device.CR;
						cout <<" OP1:"<< uppercase <<setw(4) << setfill('0') << hex << (short)Device.OP1;
						cout <<" OP2:"<< uppercase << setw(4) << setfill('0') << hex << (short)Device.OP2;
						cout <<" SM:"<< uppercase << setw(4) << setfill('0') << hex << (short)Device.SM;
						if(Device.rgW)	cout << " W-"; else  cout << " W+";
						if(Device.rgO)	cout << " OV"; else  cout << " NO";
						cout << endl;
						}
					} 
				break;
			default:
				break;		
			} 
		key = 0;
		// выполнить команду в автоматическом режиме
		if(Device.mode == sAuto) 
			Device.doCommand();
		} while(Device.mode != sQuit);
	
	cout <<"\nPOWER OFF\n";
	_getche();
	return 0;
}
