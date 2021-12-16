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
#include "console.h"

Console::Console() 
{
	Hmode = false; 
	//получаем дескриптор
	HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(hStdOut, FOREGROUND_RED | FOREGROUND_GREEN /*| FOREGROUND_INTENSITY*/);
}

void Console::setConsoleTitle(int8_t mode) 
{
	switch(mode){
		case sStop:
			SetConsoleTitle("Processor [STOP]");
			break;
		case sAuto:
			SetConsoleTitle("Processor [AUTO]");
			break;
		case sStep:
			SetConsoleTitle("Processor [STEP]");
			break;
		default:
			SetConsoleTitle("Processor []");	
		}
} 

void Console::doConsole(Processor Dev) 
{
	string 			input 	= ""; //вводимая строка
	string			sinput  = ""; //вводимая строка для команды S
	vector<string> 	param; 	  // массив параметров - результат разбора
	char 			ch = {0};
	int 			c = 0; 
	int 			d = 0;
	int				e = 0;
	size_t* 		pos = nullptr;
	
	while(input != "."){
		cout << "\nOPT-";
		//ввод команды
		input = "";
		getline(cin, input);
		//парсинг введенной команды
		param.clear();
		stringstream sS;
		sS << input;
		while(!sS.eof()) {
			string s;
			sS >> s;
			param.push_back(s);
			}
		//выполняем команду
		ch = param[0][0];
		ch = toupper(ch);
		switch(ch) {
			case 'X':
				cout << "IP:"<< uppercase << setw(2) << setfill('0') << hex << (short)Dev.IP; 
				cout <<" CR:"<< uppercase << setw(4) << setfill('0') << hex << (short)Dev.CR;
				cout <<" OP1:"<< uppercase <<setw(4) << setfill('0') << hex << (short)Dev.OP1;
				cout <<" OP2:"<< uppercase << setw(4) << setfill('0') << hex << (short)Dev.OP2;
				cout <<" SM:"<< uppercase << setw(4) << setfill('0') << hex << (short)Dev.SM;
				if(Dev.rgW)	cout << " W-"; else  cout << " W+";
				if(Dev.rgO)	cout << " OV"; else  cout << " NO";
				cout << endl;
				break;
			case 'D':
				if(param.size() == 1) {
					c = 0;
					while(c < RAMsize) {
						cout<< uppercase << setw(2) << setfill('0') << hex << c << " : "<< uppercase << setw(4) << setfill('0') << hex << Dev.RAM[c];
						c++;
						cout<< " : " << uppercase << setw(4) << setfill('0') << hex << Dev.RAM[c] << " : " << uppercase << setw(2) << setfill('0') << hex << c; 
						c++;
						cout << endl;
						}
					}
				if(param.size() == 2) {
					c = stoi(param[1], pos, 16); 
					while(c < RAMsize) {
						if(c < RAMsize) cout<< uppercase << setw(2) << setfill('0') << hex << c << " : "<< uppercase << setw(4) << setfill('0') << hex << Dev.RAM[c];
						c++;
						if(c < RAMsize) cout<< " : " << uppercase << setw(4) << setfill('0') << hex << Dev.RAM[c] << " : " << uppercase << setw(2) << setfill('0') << hex << c; 
						c++;
						cout << endl;
						}	
					}
				
				if(param.size() == 3) {
					c = stoi(param[1], pos, 16); 
					d = stoi(param[2], pos, 16);
					if(d >= RAMsize) d = RAMsize;
					while(c <= d) {
						if(c <= d) cout << uppercase << setw(2) << setfill('0') << hex << c << " : "<< uppercase << setw(4) << setfill('0') << hex << Dev.RAM[c];
						c++;
						if(c <= d) cout<< " : " << uppercase << setw(4) << setfill('0') << hex << Dev.RAM[c] << " : " << uppercase << setw(2) << setfill('0') << hex << c; 
						c++;
						cout << endl;
						}	
					}
				break;
			case 'F':
				if(param.size() == 4) {
					c = stoi(param[1], pos, 16); 
					d = stoi(param[2], pos, 16);
					e = stoi(param[3], pos, 16);
					if( (d < RAMsize) &&(c <= d)) {
						for(auto i = c; i <= d; i++) {
							Dev.RAM[i] = e;
							}
						}
					else
					cout << "?" << endl;
					}
				else {
					cout << "?" << endl;
					}
				break;
			case 'M':
				if(param.size() == 4) {
					c = stoi(param[1], pos, 16); //c
					d = stoi(param[2], pos, 16); // по
					e = stoi(param[3], pos, 16); // на
					if( (d < RAMsize) &&(c <= d)) {
						for(auto i = c; i <= d; i++) {
							if((e+i-c) < RAMsize) {
								Dev.RAM[e+i-c] = Dev.RAM[i];
								Dev.RAM[i] = 0;
								}
							}
						}
					else
					cout << "?" << endl;
					}
				else {
					cout << "?" << endl;
					}
				break;
			case 'S':
				sinput = "";
				if(param.size() > 1) 
					c = stoi(param[1], pos, 16);
				else c = 0;
				while(sinput !=".") {
					cout << uppercase << setw(2) << setfill('0') << hex << c << " " << uppercase << setw(4) << setfill('0') << hex << Dev.RAM[c] <<":";	
					cin >> sinput; 
					if(sinput != "." && c < RAMsize) {
						e = stoi(sinput, pos, 16);
						Dev.RAM[c] = e;
						cout << uppercase << setw(2) << setfill('0') << hex << c << "     :"<< uppercase << setw(4) << setfill('0') << hex << Dev.RAM[c] << endl;
						c++;
					}
					}
 				break;
 			case 'G':
 				if(param.size() == 1) {
				 	Dev.IP = 0;
				 	Dev.mode = sAuto;
					}	
				break;
				if(param.size() == 2) {
						
					}
			case 'H':
				if(Hmode) {
					Hmode = false;
					cout << "LOGGING OFF" << endl;
					}
				else {
					Hmode = true;
					cout << "LOGGING ON" << endl;
					}
				break;
			param.clear();	
			}
		}
	cout  << "\nDONE\n";	//выход из консоли - команда "."
}
