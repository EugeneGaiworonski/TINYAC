/*
	Name: Virtual Training Automatic Computing Machine TINYAC.
	Version: Build I.
	
	Copyright (C) 2022  Eugene Gaiworonski.

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see https://www.gnu.org/licenses/.
	
	Date: 
	1.12.21  - Alpha
	09.05.22 - Beta
	15.05.22 - First program executed successfully! 
	
	Description: Virtual Training Automatic Computing Machine TINYAC.
	
	1. PREFACE
	
	"Krokha" was the first computer model developed specifically for 
	a school textbook of computer science. It was proposed by a group 
	of authors from Yekaterinburg: A.G.Gein and others in 1989.
	TINYAC is a computer model that is compatible with a "Krokha" 
	at the command level, supplemented by a control console similar 
	in functionality to the DOS DEBUG program.
	
	2. SYSTEM DESIGN
	
	The TINYAC is a classic example of a three-address 1st generation
	computer, i.e. its command specifies 3 addresses (operands): 
	for 2 initial values and the address of the cell where the result 
	should be written. 
	
	The general structure of the machine word has the form
	+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+
	| 15| 14| 13| 12|X11| 10|  9|  8| X7|  6|  5|  4| X3|  2|  1|  0|
	+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+
	|instruction code|address 1     |address 2      |address 3      |
	+---------------+---------------+---------------+---------------+
	|action         |data                           |result         |
	+---------------+-------------------------------+---------------+
	Bits 11, 7 and 3 in machine word are ignored in address manipulating
	operations, but have the usual  meaning if machine word is considered
	as a instruction operand.
	
	The memory of the TINYAC has a volume of 8 machine words. Therefore, 
	the address of any cell is encoded with exactly three binary digits. 
	The fourth digit is ignored. The instruction code is made 4-bit. 
	The first 3 bits encode 8 "Krokha" instructions.  Here is list:
	+----+--------------------------------+-------------------------+
	|CODE|OPERATION NAME 				  |OPERATION CONTENT        |
	+----+--------------------------------+-------------------------+
	|000 |copy 							  |A1 ==> A3			    |
	|001 |addition 						  |A1 + A2 ==> A3		    |
	|010 |division 						  |A1 / A2 ==> A3			|
	|011 |subtraction 					  |A1 - A2 ==> A3			|
	|100 |conditional transition by equal |if A1=A2 transfer to A3	|
	|101 |multiplication 				  |A1 * A2 ==> A3  			|
	|110 |conditional transition by above |if A1>A2 transfer to A3	|
	|111 |output and stop 				  |output A1, A2, A3; stop	|
	+----+--------------------------------+-------------------------+
	The TINYAC  operates with integers. Like the commands, the numbers 
	are 16-bit. Obviously, the maximum number that the TINYAC is still 
	able to place in its memory is 32767 and the minimum number	is -32768. 
	If the result is larger, it goes beyond the 16-bit machine 	word and 
	cannot be stored correctly and overflow indication is established. 
	Overflow is an emergency situation and leads to the further incorrect 
	program execution.

	Since all the numbers in the TINYAC must be integers, the division 
	is always performed entirely and the remainder is simply discarded. 
	For example, when dividing 14 by 3, 4 is obtained, and 3 by 15 is 
	zero. By the way, division by zero is impossible, which is also 
	the reason for the emergency shutdown of the computer and division 
	by zero indication.
	
	During the copy operation, information from the cell with the first 
	address is copied to cell with third one. The second address does not 
	matter in this case; it is customary to fill it with zeros.
	
	When performing a conditional transition, the TINYAC compares both 
	operands with each other and, if the condition is met (equal to or 
	greater, depending on the instruction), goes to third: simply put, 
	the next command will execute third address. If the condition is not 
	met (for example, by inequality in the first case), the transition is 
	ignored and the next program command is executed. Please remember 
	this logic: it is the basis of all branches and cycles. All modern 
	real computers work according to this logic.
	
	Here we only note that the TINYAC, like the "Krokha", always starts 
	the execution of the program from the zero address.
	
	3.CONSOLE OPERATIONS
	
	The console allows to control the computer at a low level: viewing 
	the contents of RAM and processor registers, executing machine code, 
	assembling, disassembling, loading and saving programs in binary code. 
	The console supports a very limited input language: decimal or hexadecimal 
	numbers encoding addresses and data, as well as single-character commands; 
	also symbolic assembly language commands in assembly mode.
	
	Supported commands are:
	Q - shutting down and exiting the program.
	G - starting of program execution.
	D - outputs the contents of the memory.
	A [p1] - converting an assembler instruction into machine code.
	If p1 is skipped the first assembling address is considered 0, else
	it is considered equal to p1.
	+----+--------------------------------+-------------------------+
	|CODE|OPERATION NAME 				  |ASSEMBLER MNEMONIC       |
	+----+--------------------------------+-------------------------+
	|000 |copy 							  |COPY     			    |
	|001 |addition 						  |ADD           		    |
	|010 |division 						  |DIV           			|
	|011 |subtraction 					  |SUB           			|
	|100 |conditional transition by equal |TREQ                   	|
	|101 |multiplication 				  |MPY             			|
	|110 |conditional transition by above |TRGT                   	|
	|111 |output and stop 				  |PRST                   	|
	| -  |define decimal value            |DEFD                     |
	| -  |define hexadecimal value        |DEFH                     |  
	+----+--------------------------------+-------------------------+
	Hexadecimal values must be started from zero.
	All three operands after mnemonic must be specified as numbers.
	DEFD and DEFH require one operand.
	U - converting binary code into assembly language instructions.
	N - specifies the file name for the read (L) and write (W) operations 
	of the program. File extension (.bin) is added automatically.
	L - loading the binary file of the program.
	W - saving the binary file of the program.
	F p1 [p2] [p3]- fills a memory area from p2 to p3 with value p1.
	M p1 p2 p3 - moves values from memory area p1...p2 to p3.
	X - editing internal registers and indications.
	R - viewing internal registers and indications.
	S [p1] - editing values in memory from p1 or 0.
	H p1 p2- computing sum and difference of values p1 and p2.
	T - execute one command (Trace mode).
	
*/

#include <iostream>
#include <sstream>
#include <iomanip>
#include <string>
#include <vector>
#include <array>
#include <cctype>
#include <cstdio>
#include <algorithm>
#include <windows.h>
#include <limits.h>

#define Word int16_t
#define Byte int8_t
#define MEMSIZE 8
#define LASTADDR 7

#define cmCOPY 0 // copy
#define cmADD  1 // add
#define cmDIV  2 // divide
#define cmSUB  3 // substract
#define cmTREQ 4 // trace if equal
#define cmMPY  5 // multiply
#define cmTRGT 6 // trace if greater
#define cmPRST 7 // print & stop

typedef union {
	struct {
		int8_t byte3 : 4;
		int8_t byte2 : 4;
		int8_t byte1 : 4;
		int8_t byte0 : 4;
	};
	Word word;
} _WORD;

void print_char(char c) {
	unsigned char mask = 128;
	int i;
	for(i = 0; i < 8; ++i){
		printf("%s",((mask & c)?"1":"0"));
		mask >>= 1;
	}
}

class TINYAC {
	public:
		bool OV; //overflow state 		OV/NO
		bool D0; //division by zero		D0/ND
		Word IR; //instruction register
		Word IP; //instruction pointer
		Word memory[MEMSIZE];
		Word OP1;
		
		std::string dir;
		std::vector<std::string> parsedDir; //разобранная команда
		std::string fileName;
		
		TINYAC();
		void DumpMem();
		void LoadTest();
		void Do();
		int  Step();
		void Console();
		void ParseDir();
		void Assemble();
		void Unassemble();
		void SetName();
		void LoadFile();
		void WriteFile();
		void FillMem();
		void MoveMem();
		void EditRegs();
		void ViewRegs();
		void EditMem();
		void Compute();
		void Trace();
};

int main(int argc, char** argv) {
	TINYAC tinyac;
	tinyac.Console();
	return 0;
}

TINYAC::TINYAC() {
	for(int i = 0; i < MEMSIZE; i++) {
		memory[i] = 0;
		system("color 0A");
		system("cls");
		std::cout <<"Training Automatic Computing Machine \"TINYAC\" Build I"<<std::endl;
		std::cout <<"It's more fun to compute..."<<std::endl;
		std::cout <<""<<std::endl;
		std::cout << i+1 << "W Ok";
		Sleep(50);
		fileName = "program.bin";
	}
	OV = false;
	D0 = false;	
	IR = 0;
	IP = 0;
}

int TINYAC::Step() {
	std::vector<int8_t> parsedWord;
	_WORD wd;
	
	IR = memory[IP];
	IP++; if(IP > LASTADDR) IP = 0; // достигли конца памяти, переходим на 0
	wd.word = IR;
	parsedWord.clear();
    parsedWord.push_back(wd.byte0); 
	parsedWord.push_back(wd.byte1); 
	parsedWord.push_back(wd.byte2); 
	parsedWord.push_back(wd.byte3);
	switch (wd.byte0) {
		case cmCOPY:
			memory[wd.byte3] = memory[wd.byte1];
			break;
		case cmADD: 
			if (((memory[wd.byte2] > 0) && (memory[wd.byte1] > (SHRT_MAX - memory[wd.byte2]))) || ((memory[wd.byte2] < 0) && (memory[wd.byte1] < (SHRT_MIN - memory[wd.byte2])))) {
    			OV = true;
  			} 
			else memory[wd.byte3] = memory[wd.byte1] + memory[wd.byte2];
			break;
		case cmDIV:
			if (memory[wd.byte2]==0) D0 = true;
			else if ((memory[wd.byte2] == 0) || ((memory[wd.byte1] == SHRT_MIN) && (memory[wd.byte2] == -1))) {
    			OV = true;
  			} 
			else memory[wd.byte3] = memory[wd.byte1] / memory[wd.byte2];
			break;
		case cmSUB:
			if ((memory[wd.byte2] > 0 && memory[wd.byte1] < SHRT_MIN + memory[wd.byte2]) || (memory[wd.byte2] < 0 && memory[wd.byte1] > SHRT_MAX + memory[wd.byte2])) {
				OV = true;
			} 
			else memory[wd.byte3] = memory[wd.byte1] - memory[wd.byte2];
			break;
		case cmTREQ: 
			if (memory[wd.byte1] == memory[wd.byte2]) IP = wd.byte3;
			break;
		case cmMPY:
			if (memory[wd.byte1] > 0) {  /* 1 is positive */
    			if (memory[wd.byte2] > 0) {  /* 1 and 2 are positive */
      				if (memory[wd.byte1] > (SHRT_MAX / memory[wd.byte2])) {
        				OV = true;
      				}
    			} 
				else { /* 1 positive, 2 nonpositive */
      				if (memory[wd.byte2] < (SHRT_MIN / memory[wd.byte1])) {
        				OV = true;
      				}
    			} /* 1 positive, 2 nonpositive */
  			} 
			else { /* 1 is nonpositive */
    			if (memory[wd.byte2] > 0) { /* 1 is nonpositive, 2 is positive */
      				if (memory[wd.byte1] < (SHRT_MIN / memory[wd.byte2])) {
        				OV = true;
      				}
    			} 
				else { /* 1 and 2 are nonpositive */
      				if ( (memory[wd.byte1] != 0) && (memory[wd.byte2] < (SHRT_MAX / memory[wd.byte1]))) {
        				OV = true;
      				}
    			} /* End if 1 and 2 are nonpositive */
  			} /* End if 1 is nonpositive */

  			memory[wd.byte3] = memory[wd.byte1] * memory[wd.byte2];
			break;
		case cmTRGT:
			if (memory[wd.byte1] > memory[wd.byte2]) IP = wd.byte3;
			break;
		case cmPRST: 
			std::cout << memory[wd.byte1] << " " << memory[wd.byte2] << " " << memory[wd.byte3] << std::endl;
			break;
		default:
			return cmPRST; //неизвестная инструкция - STOP!
			break;
	}
	return wd.byte0;
}

void TINYAC::DumpMem() {
	std::cout << "Dumping..." << std::endl;
	for(int i = 0; i < MEMSIZE; i++) {
		std::cout << std::setw(2) << std::setfill('0') << i << ":" << std::setw(6) << std::setfill('0') << std::hex << memory[i]<<':'
		          << std::setw(6) << std::setfill(' ') << std::dec << memory[i] << "D\t";
		if (i==3) std::cout <<std::endl;
	}
}

void TINYAC::LoadTest() {
	//00 0001 0110 0111 0101 0x1675 ; ADD  6 7 5 ;P=A+B
	//01 0001 0101 0101 0101 0x1555 ; ADD  5 5 5 ;P=P+P
	//02 0111 0110 0111 0101 0x7675 ; PRST 6 7 5 ;print A B P
	//03 0000 0000 0000 0000 0x0000
	//04 0000 0000 0000 0000 0x0000
	//05 0000 0000 0000 0000 0x0000 ; P
	//06 0000 0000 0000 0002 0x0002 ; A
	//07 0000 0000 0000 0001 0x0001 ; B
	memory[0] = 0x1675;
	memory[1] = 0x1555;
	memory[2] = 0x7675;
	memory[3] = 0x0000;
	memory[4] = 0x0000;
	memory[5] = 0x0000;
	memory[6] = 0x0002;
	memory[7] = 0x0001;
}

void TINYAC::Do() {
	for(;;) if (Step()==cmPRST) break;
}

void TINYAC::Trace() {
	Step();
	ViewRegs();
}

void TINYAC::Console() {
	for (;;) {
		std::cout <<"\n-";
		dir="";
		parsedDir.clear();
		std::getline(std::cin,dir);
		ParseDir();
		switch(parsedDir[0][0]) {
			case 'q':
			case 'Q':
				return;//++
				break;
			case 'g':
			case 'G':
				Do();
				break;
			case 'd':
			case 'D':
				DumpMem();//++
				break;
 			case 'a':
			case 'A':
				Assemble();//++
				break;
			case 'u':
			case 'U':
				Unassemble();
				break;
			case 'n':
			case 'N':
				SetName();//++
				break;
			case 'l':
			case 'L':
				LoadFile();//++
				break;
			case 'w':
			case 'W':
				WriteFile();//++
				break;
			case 'f':
			case 'F':
				FillMem();//++
				break;	
			case 'm':
			case 'M':
				MoveMem();//++
				break;
			case 'x':
			case 'X':
				EditRegs();//++
				break;
			case 'r':
			case 'R':
				ViewRegs();//++
				break;
			case 's':
			case 'S':
				EditMem();
				break;
			case 'h':
			case 'H':
				Compute();
				break;
			case 't':
			case 'T':
				Trace();
				break;
			case '!':
				LoadTest();//++
				break;
			default	:
				std::cout << dir[0] <<"?" << std::endl;
		}
	}
}

void TINYAC::ParseDir() {
	if (dir != "") {
		parsedDir.clear();
		std::stringstream cstream;
		std::string word;
		cstream << dir;
		while (cstream >> word) {
			parsedDir.push_back(word);
		}
	}
}

void TINYAC::Assemble() {
	int org;
	Word word;
	std::vector<std::string> parsedInstr;
	std::string instr;

	if (parsedDir.size() == 1) //без параметров
		org = 0;
	if (parsedDir.size() > 1) {
		org = std::stoi(parsedDir[1]);
	}
	if ((org >=0) & (org <= LASTADDR)) {
		std::cout <<"Assembling...\n";
		do {
			std:: cout << "" << std::setw(2) << std::setfill('0') << org <<":";
			instr.clear();
			parsedInstr.clear();
			word = 0;
			std::getline(std::cin,instr);
			if (instr != "") { //assembling
				std::stringstream cs;
				std::string token;
				cs << instr;
				while (cs >> token) parsedInstr.push_back(token);
				if (parsedInstr.size() == 4) word = ((std::stoi(parsedInstr[1])) << 8) + ((std::stoi(parsedInstr[2])) << 4) + (std::stoi(parsedInstr[3]));
				for (int i = 0; i < parsedInstr[0].size(); i++) parsedInstr[0][i] = toupper(parsedInstr[0][i]);
				if      ((parsedInstr[0]=="COPY") && (parsedInstr.size() == 4)) word += 0x0000;
				else if ((parsedInstr[0]=="ADD")  && (parsedInstr.size() == 4)) word += 0x1000;
				else if ((parsedInstr[0]=="DIV")  && (parsedInstr.size() == 4)) word += 0x2000;
				else if ((parsedInstr[0]=="SUB")  && (parsedInstr.size() == 4)) word += 0x3000;
				else if ((parsedInstr[0]=="TREQ") && (parsedInstr.size() == 4)) word += 0x4000;
				else if ((parsedInstr[0]=="MPY")  && (parsedInstr.size() == 4)) word += 0x5000;
				else if ((parsedInstr[0]=="TRGT") && (parsedInstr.size() == 4)) word += 0x6000;
				else if ((parsedInstr[0]=="PRST") && (parsedInstr.size() == 4)) word += 0x7000;
				else if ((parsedInstr[0]=="DEFD") && (parsedInstr.size() == 2)) word = std::stoi(parsedInstr[1]);
				else if ((parsedInstr[0]=="DEFH") && (parsedInstr.size() == 2)) {
					int b {0};
					std::stringstream ss;
					ss << std::hex << parsedInstr[1];
					ss >> b;
					word = b;
				} else {
					std::cout << "Illegal instruction\n";
					continue;
				};
				memory[org] = word;
				org++;
				if (org>LASTADDR) org = 0; //заворачиваем адреса
			} // end assembling
		} while (instr !="");
	} else std::cout << "Illegal address";
}

void TINYAC::Unassemble() {
	std::vector<std::string> programText;
	std::string programString;
	std::string buffer;
	std::array<bool,MEMSIZE> isData {false, false, false, false, false, false, false, false}; //таблица данных
	bool isPRST {false}; //достигнут ли конец программы - инструкция PRST всегда последняя
	int lastPRSTaddr {0};
	Word word {0};
	_WORD wd;
	std::vector<int8_t> parsedWord;
	
	programText.clear();
	parsedWord.clear();	//3-2-1-0
	buffer = "";
	isPRST = false;
    // --0 pass
    // ищем конец программы
    for (auto adr0 = 0; adr0 < MEMSIZE; adr0++) {
    	word = memory[adr0]; 
    	wd.word = word;
    	parsedWord.clear();
    	parsedWord.push_back(wd.byte0); 
		parsedWord.push_back(wd.byte1); 
		parsedWord.push_back(wd.byte2); 
		parsedWord.push_back(wd.byte3);
		if (parsedWord[0] == cmPRST) lastPRSTaddr = adr0;	 
	}
	// --1 pass
    for(auto adr = 0; adr < MEMSIZE; adr++) {
	// parse word
		//_WORD wd;
		word = memory[adr]; //std::cout <<"Adr=" <<adr << " "<< "word="<<word << " ";
		wd.word = word;
		parsedWord.clear();
		parsedWord.push_back(wd.byte0); //std::cout <<"byte0=" << wd.byte0+30 << " ";
		parsedWord.push_back(wd.byte1); //std::cout <<"byte1=" << wd.byte1+30 << " ";
		parsedWord.push_back(wd.byte2); //std::cout <<"byte2=" << wd.byte2+30 << " ";
		parsedWord.push_back(wd.byte3); //std::cout <<"byte3=" << wd.byte3+30 << " ";
		for(auto idx=0; idx<4; idx++) {
			parsedWord[idx] = parsedWord[idx] & 0x00F; // обнулить старшую тетраду
			//parsedWord[idx] = parsedWord[idx] | 0x0F0; // установить в 1 старшую тетраду
		}
    // end parse word
	// parse for instructions/data
		if (isPRST==false) {
			switch (parsedWord[0]) {
				case cmCOPY:
					isData.at(parsedWord[1]) = true;
					isData.at(parsedWord[3]) = true;
					break;
				case cmADD:
				case cmDIV:
				case cmSUB:
				case cmMPY:
					isData.at(parsedWord[2]) = true;
					isData.at(parsedWord[1]) = true;
					isData.at(parsedWord[3]) = true;
					break;
				case cmPRST:
					if (adr==lastPRSTaddr) isPRST = true;
					isData.at(parsedWord[2]) = true;
					isData.at(parsedWord[1]) = true;
					isData.at(parsedWord[3]) = true;
					break;
			}
		} else isData.at(adr) = true;
	// end parse for instructions/data
	// !debug 
	//for(auto i =0; i <8;i++ ) std:: cout << isData[i] << " ";
	//std::cout << std::endl;
	// !end debug
	}
    // --2 pass
    // decoding
	for(auto adr2 = 0; adr2 < MEMSIZE; adr2++) { // по нулевому адресу ВСЕГДА инструкция
	 	std::stringstream ss;
		programString ="";
	 	buffer="";
	 	word = memory[adr2];
		ss << std::setw(2) << std::setfill('0') << adr2 << ":" << std::setw(6) << std::setfill('0') << std::hex << word;
		ss >> buffer; ss.clear();
		programString = programString + buffer+"    ";
	// parse word
		//_WORD wd;
		word = memory[adr2];
		wd.word = word;
		parsedWord.clear();
		parsedWord.push_back(wd.byte0);
		parsedWord.push_back(wd.byte1);
		parsedWord.push_back(wd.byte2);
		parsedWord.push_back(wd.byte3);
		for(auto idx=0; idx<4; idx++) parsedWord[idx] = parsedWord[adr2] & 0x00F; // обнулить старшую тетраду
		if(isData.at(adr2)==false) { //если инструкция
			switch (wd.byte0) {
				case cmCOPY: buffer = "COPY "; break; 
				case cmADD:  buffer = "ADD ";  break; 
				case cmDIV:  buffer = "DIV ";  break; 
				case cmSUB:  buffer = "SUB ";  break; 
				case cmTREQ: buffer = "TREQ "; break;
				case cmMPY:  buffer = "MPY ";  break; 
				case cmTRGT: buffer = "TRGT "; break; 
				case cmPRST: buffer = "PRST "; break; 
			}
			programString = programString + buffer;
			buffer = "";
			ss << std::setw(2) << std::setfill('0') << std::hex << wd.byte1+0;
			ss >> buffer; ss.clear();
			programString = programString + buffer+' ';
			buffer = "";
			ss << std::setw(2) << std::setfill('0') << std::hex << wd.byte2+0;
			ss >> buffer; ss.clear();
			programString = programString + buffer+ ' ';
			buffer = "";
			ss << std::setw(2) << std::setfill('0') << std::hex << wd.byte3+0;
			ss >> buffer;ss.clear();
			programString = programString + buffer;
			buffer = "";
		} else {
			std::stringstream sS;
			sS << std::setw(6) << std::setfill('0') << std::hex << memory[adr2];
			buffer = "DEFH ";
			programString = programString + buffer;
			sS >> buffer;
			programString = programString + buffer;
		}
		// end parse word
		programText.push_back(programString);	          
		}
    	// end decoding
	
	//вывод программы на экран
	for(auto i = 0; i < MEMSIZE; i++) std::cout<< programText.at(i)<<std::endl;
}

void TINYAC::SetName() {
	std::string tmp;
	std::cout << "Old Name: "<< std::endl << fileName << " " << std::endl;
	std::cout << "New Name: ";
	std::getline(std::cin, tmp);
	if (tmp != "") {
		//if (tmp.substr(tmp.length()-5,4) !=".bin")
			tmp +=".bin";
		fileName = tmp;	
	};
	std::cout << fileName;
}

void TINYAC::LoadFile() {
	FILE* fptr;
	if ((fptr = fopen(fileName.c_str(), "rb")) == NULL) {
		std::cout << "File open error";
		return;
	};
	if (fread(memory, sizeof(Word),MEMSIZE,fptr) != MEMSIZE){
		if(feof(fptr)) std::cout << "Unexpected end of file";
		else std::cout << "File read error";
	};
	fclose(fptr);
	std::cout<< "8 words read";
}

void TINYAC::WriteFile() {
	FILE* fptr;
	if ((fptr = fopen(fileName.c_str(), "wb")) == NULL) {
		std::cout << "File open error";
		return;
	};
	if (fwrite(memory, sizeof(Word),MEMSIZE,fptr) != MEMSIZE){
		if(feof(fptr)) std::cout << "Unexpected end of file";
		else std::cout << "File write error";
	};
	
	fclose(fptr);
	std::cout<< "8 words written";
}

void TINYAC::FillMem() {
	Word fillValue {0};
	int fillFrom {0};
	int fillTo {0};
	if(parsedDir.size()>1) {
		if(parsedDir.size() == 2) {
			if(parsedDir[1][0]=='0') { //hex
				int b {0};
					std::stringstream ss;
					ss << std::hex << parsedDir[1];
					ss >> b;
					fillValue = b;
			}
			else fillValue = std::stoi(parsedDir[1]);
			fillFrom = 0;
			fillTo = LASTADDR;		 
		}
		else if(parsedDir.size() == 3) {
			if(parsedDir[1][0]=='0') { //hex
				int b {0};
					std::stringstream ss;
					ss << std::hex << parsedDir[1];
					ss >> b;
					fillValue = b;
			}
			else fillValue = std::stoi(parsedDir[1]);
			if(parsedDir[2][0]=='0') { //hex
				int b {0};
					std::stringstream ss;
					ss << std::hex << parsedDir[2];
					ss >> b;
					fillFrom = b;
			}
			else fillFrom = std::stoi(parsedDir[2]);
			if (fillFrom < 0) fillFrom = 0;
			if (fillFrom > LASTADDR) fillFrom = LASTADDR;
			fillTo = LASTADDR;		 
		}
		else if(parsedDir.size() >= 4) {
			if(parsedDir[1][0]=='0') { //hex
				int b {0};
					std::stringstream ss;
					ss << std::hex << parsedDir[1];
					ss >> b;
					fillValue = b;
			}
			else fillValue = std::stoi(parsedDir[1]);
			if(parsedDir[2][0]=='0') { //hex
				int b {0};
					std::stringstream ss;
					ss << std::hex << parsedDir[2];
					ss >> b;
					fillFrom = b;
			}
			else fillFrom = std::stoi(parsedDir[2]);
			if (fillFrom < 0) fillFrom = 0;
			if (fillFrom > LASTADDR) fillFrom = LASTADDR;
			if(parsedDir[3][0]=='0') { //hex
				int b {0};
					std::stringstream ss;
					ss << std::hex << parsedDir[3];
					ss >> b;
					fillTo = b;
			}
			else fillTo = std::stoi(parsedDir[3]);
			if (fillTo < 0) fillTo = 0;
			if (fillTo > LASTADDR) fillTo = LASTADDR;
			if (fillTo < fillFrom) {
				int b;
				b = fillTo;
				fillTo = fillFrom;
				fillFrom = b;
			}		 
		}
		for(auto i = fillFrom; i <= fillTo; i++) memory[i] = fillValue;
		std::cout << fillTo - fillFrom + 1 << " words(s) written";  
	} 
}

void TINYAC::MoveMem() {
	int srcFrom;
	int srcTo;
	int trgFrom;
	if(parsedDir.size() >= 4) {
			if(parsedDir[1][0]=='0') { //hex
				int b {0};
					std::stringstream ss;
					ss << std::hex << parsedDir[1];
					ss >> b;
					srcFrom = b;
			}
			else srcFrom = std::stoi(parsedDir[1]);
			if (srcFrom < 0) srcFrom = 0;
			if (srcFrom > LASTADDR) srcFrom = LASTADDR;			
			if(parsedDir[2][0]=='0') { //hex
				int b {0};
					std::stringstream ss;
					ss << std::hex << parsedDir[2];
					ss >> b;
					srcTo = b;
			}
			else srcTo = std::stoi(parsedDir[2]);
			if (srcTo < 0) srcFrom = 0;
			if (srcTo > LASTADDR) srcTo = LASTADDR;
			if(parsedDir[3][0]=='0') { //hex
				int b {0};
					std::stringstream ss;
					ss << std::hex << parsedDir[3];
					ss >> b;
					trgFrom = b;
			}
			else trgFrom = std::stoi(parsedDir[3]);
			if (trgFrom < 0) trgFrom = 0;
			if (srcTo < srcFrom) {
				int b;
				b = srcTo;
				srcTo = srcFrom;
				srcFrom = b;
			}		 
		for (auto i = srcFrom; i <= srcTo; i++) memory[i+trgFrom-srcFrom] = memory[i];
		std::cout << srcTo - srcFrom + 1 << " words(s) moved";
		}
}

void TINYAC::EditRegs() {
	std::string w;
	std::cout << "IR:" << std::setw(6) << std::setfill('0') << std::hex << IR <<':'
		      << std::setw(6) << std::setfill(' ') << std::dec << IR << "D:";
	std::getline(std::cin,w);
	if (w !="") {
		if(w[0]=='0') { //hex
			int b {0};
			std::stringstream ss;
			ss << std::hex << w;
			ss >> b;
			IR = b;
		}
	else IR = std::stoi(w);
	}
	std::cout << "IR set to " << std::setw(6) << std::setfill('0') << std::hex << IR<<std::endl;
	std::cout << "IP:" << std::setw(6) << std::setfill('0') << std::hex << IP <<':'
		      << std::setw(6) << std::setfill(' ') << std::dec << IP << "D:";
	std::getline(std::cin,w);
	if (w !="") {
		if(w[0]=='0') { //hex
			int b {0};
			std::stringstream ss;
			ss << std::hex << w;
			ss >> b;
			IP = b;
		}
	else IP = std::stoi(w);
	}
	if (IP > 7) IP = 7;
	if (IP < 0) IP = 0;
	std::cout << "IP set to " << std::setw(6) << std::setfill('0') << std::hex << IP<<std::endl;
	
	if (OV == true) std::cout << "OV"; else std::cout <<"NO";
	std::cout << "\n1-set/0-reset:"; std::getline(std::cin,w);
	if (w !="") if (w=="1") OV = true; else OV = false;
	if (D0 == true) std::cout << " D0"; else std::cout << "ND";
	std::cout << "\n1-set/0-reset:"; std::getline(std::cin,w);
	if (w !="") if (w=="1") D0 = true; else D0 = false;
}

void TINYAC::ViewRegs() {
	std::cout << "IR:" << std::setw(6) << std::setfill('0') << std::hex << IR <<':'
		      << std::setw(6) << std::setfill(' ') << std::dec << IR << "D\n";
	std::cout << "IP:" << std::setw(6) << std::setfill('0') << std::hex << IP <<':'
		      << std::setw(6) << std::setfill(' ') << std::dec << IP << "D\n";
	if (OV == true) std::cout << "OV"; else std::cout <<"NO";
	if (D0 == true) std::cout << " D0"; else std::cout << " ND";
}

void TINYAC::EditMem() {
	int org {0};
	std::string instr;
	Word word {0};
	if (parsedDir.size() == 2) {
		if(parsedDir[1][0]=='0') { //hex
				int b {0};
					std::stringstream ss;
					ss << std::hex << parsedDir[1];
					ss >> b;
					org = b;
			}
			else org = std::stoi(parsedDir[1]);
	} 
	else if(parsedDir.size() == 1) 	org = 0;
	if ((org >=0) & (org <= LASTADDR)) {
		do {
			std:: cout << "" << std::setw(2) << std::setfill('0') << org <<":";
			instr.clear();
			word = 0;
			std::getline(std::cin,instr);
			if (instr !="") {
				if(instr[0]=='0') { //hex
					int b {0};
					std::stringstream ss;
					ss << std::hex << instr;
					ss >> b;
					word = b;
				}
				else word = std::stoi(instr);
				memory[org] = word;
				org++;
				if (org>LASTADDR) org = 0; //заворачиваем адреса
			}
		} while (instr !="");
	}
}

void TINYAC::Compute() {
	int first  {0};
	int second {0};
	if(parsedDir.size() >= 3) {
			if(parsedDir[1][0]=='0') { //hex
				int b {0};
					std::stringstream ss;
					ss << std::hex << parsedDir[1];
					ss >> b;
					first = b;
			}
			else first = std::stoi(parsedDir[1]);
			
			if(parsedDir[2][0]=='0') { //hex
				int b {0};
					std::stringstream ss;
					ss << std::hex << parsedDir[2];
					ss >> b;
					second = b;
			}
			else second = std::stoi(parsedDir[2]);
		std::cout << "+ "<<std::setw(6) << std::setfill('0') << std::hex << first+second <<':'
		      << std::setw(6) << std::setfill(' ') << std::dec << first+second << "D\n";
		std::cout << "- "<<std::setw(6) << std::setfill('0') << std::hex << first-second <<':'
		      << std::setw(6) << std::setfill(' ') << std::dec << first-second << "D\n";
	}
}
