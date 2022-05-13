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
	1.12.21
	09.05.22 
	
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
	
*/

#include <iostream>
#include <sstream>
#include <iomanip>
#include <string>
#include <vector>
#include <cctype>
#include <cstdio>
#include <algorithm>
#include <windows.h>

#define Word int16_t
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


class TINYAC {
	public:
		bool OV; //overflow state 		OV/NO
		bool D0; //division by zero		D0/ND
		Word IR; //instruction register
		Word IP; //instruction pointer
		Word memory[MEMSIZE];
		
		std::string dir;
		std::vector<std::string> parsedDir; //разобранная команда
		std::string fileName;
		
		TINYAC();
		void DumpMem();
		void LoadTest();
		void Do();
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

}

void TINYAC::Console() {
	for (;;) {
		std::cout <<"\n-";
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

/*
TINIAC - виртуальная машина, моделирующая обшие черты ЭВМ первого поколения.

ОСНОВНЫЕ ХАРАКТЕРИСТИКИ
VM TINIAC
----------------------------
Назначение VM TINIAC				арифметические расчеты с целыми числами
Интерфейс с пользователем			ввод с консоли, магнитной ленты, вывод на консоль, магнитную ленту, принтер
Размер оперативной памяти			16 слов
Размер машинного слова			16 бит
Представление чисел в памяти		в форме с фиксированной точкой
Система счисления				двоично-шестнадцатеричная
Языки программирования			язык машинных команд (ЯМК), Автокод
Структура команд				трехадресная

TINIAC поддерживает набор команд учебной ЭВМ "Кроха", за исключением команды 07h (111b) ВЫВОД И СТОП, которая заменена парой команд 07h (0111b) STOP и 08h (1000b) PRNT. Добавлены дополнительные команды.


МИНИМАЛЬНЫЕ ТРЕБОВАНИЯ
----------------------------
Intel 286 compatible
100 Kb HD space
16 Mb RAM
MS-DOS 3.0


УСТРОЙСТВО VM TINIAC
----------------------------

Память
Память (оперативная память) служит для хранения данных и программы во время работы компьютера. Оперативная память состоит из элементов -
ячеек. Каждая ячейка имеет свой номер, который называется адресом. В машине TINIAC 16 ячеек памяти, каждая из которых может хранить 16-разрядное значение - машинное слово. Ячейки памяти последовательно адресуются с 00 до 0F. В соответствии с принципом Фон Неймана, если машинное слово попадает в АЛУ, то оно трактуется как число. Внутреннее представление чисел в TINIAC полностью совпадает с представлением чисел в IBM PC. Команды TINIAC поддерживают знаковую целочисленную арифметику. Набор значений находится в диапазоне -32768..+32767 (08000..07FFF). Имеются две основные операции работы с памятью: 
1. Запись данного в ячейку. ЦП сообщает ОП, что именно надо записать и по какому адресу. При записи в ячейку ее старое содержимое теряется, становится недоступным.
2. Чтение содержимого ячейки. ЦП передает ОП нужный адрес. Содержимое ячейки с этим адресом считывается и пересылается в ЦП. При чтении содержимое ячейки не изменяется.
При включении машины все ячейки памяти содержат 0.

Процессор
Процессор включает в себя следующие устройства:
АЛУ (арифметическо-логическое устройство) - оно выполняет арифметические и логические операции (например, сложение, вычитание, умно-
жение, сравнение);
УУ (устройство управления) - управляет работой процессора.
Регистры - специальные ячейки, которые находятся внутри процессора.
В АЛУ имеются следующие регистры:
1. регистр 1 операнда OP1 разрядностью  16 бит,
2. регистр 2 операнда OP2 разрядностью 16 бит,
3. регистр результата (сумматор) SM разрядностью 16 бит.
При выполнении арифметических команд на эти регистры из памяти считываются операнды, в сумматор помещается результат операции, который затем сохраняется в памяти.
Регистры УУ:
1. указатель на инструкцию IP разрядностью 8 бит, во время выполнения текущей команды содержит адрес команды, следующей за выполняемой,
2. регистр команды CR разрядностью 16 бит (машинное слово), содержит выполняемую команду,
3. регистры точек разрыва BR1 и BR2 разрядностью 8 бит. В эти регистры записываются с консоли адреса команд. Когда адрес команды в регистре IP равен адресу в регистре BR1 или BR2, процессор переходит в шаговый режим работы и выполнение программы приостанавливается.
4. регистр признака Омега разрядностью 1 бит, устанавливается после каждой арифметической операции в 1, если результат строго меньше нуля и в 0 в противном случае,
5. регистр переполнения разрядностью 1 бит, устанавливается в 1, если при выполнении арифметической операции произошло переполнение, сбросить регистр в 0 можно только перезапуском машины.  
При включении машины все регистры содержат 0.

Команды
Машинное слово в устройстве управления трактуется как команда и логически подразделяется на 4 части:
0000-0000-0000-0000
   ^    ^    ^    ^
   |    |    |    |
   |    |    |    --- A3
   |    |    -------- A2
   |    ------------- A1
   ------------------ КОП

КОП - код операции, 4 бита
А1  - первый исполнительный адрес либо непосредственное значение, 4 бита
А2  - второй исполнительный адрес либо непосредственное значение, 4 бита
А3  - третий исполнительный адрес либо непосредственное значение, 4 бита

Для арифметических команд схема выполнения следующая:
[А1] := [А2] Х [А3]  (здесь вместо 'Х' - любая арифметическая операция)

Ниже приведен список команд:
Код Мнемоника   Название        Операция                    Операнды                Пример
00  MOVE        Пересылка       [A3] := [A1]                MOVE [A1]  00  [A3]     00 05 00 02
01  IADD        Сложение        [A3] := [A1] + [A2]         IADD [A1] [A2] [A3]     01 05 02 0A
02  IDIV        Деление         [A3] := [A1] / [A2]         IDIV [A1] [A2] [A3]     02 05 02 0A
03  ISUB        Вычитание       [A3] := [A1] - [A3]         ISUB [A1] [A2] [A3]     03 05 02 0A
04  TREQ        Переход если =  if [A1] = [A2] IP := A3     TREQ [A1] [A2]  A3      04 05 04 0C
05  IMPY        Умножение       [A3] := [A1] * [A2]         IMPY [A1] [A2] [A3]     05 05 02 0A
06  TRGT        Переход если >  if [A1] > [A2] IP := A3     TREQ [A1] [A2]  A3      06 05 04 0C
07  STOP        Стоп            состояние стоп              STOP  00   00   00      07 00 00 00
08  PRNT        Печать          печатать [A3]               PRNT  00   00  [A3]     08 00 00 0C       
09  READ        Чтение ленты    читать с ленты A1 слов      READ  A1   00   A3      09 0F 00 0C
                                в адрес [A3]     
0A  WRTE        Запись на ленту писать на ленту A1 слов     WRTE  A1   00   A3      0A 0F 00 0C 
                                с адреса [A3]
0B  RWND        Подвод ленты    перемотать ленту на зону A3 RWND  00   00   A3      0B 00 00 01
0C  TROV        Переход при     if OV IP := A3              TROV  00   00   A3      0C 00 00 0E 
                переполнении
0D  TRLT        Переход если <  if [A1] < [A2] IP := A3     TRLT [A1] [A2]  A3      0D 05 04 0C
0E  NOOP        Нет операции                                NOOP                    0E 00 00 00
0F  NOOP        Нет операции                                NOOP                    0F 00 00 00
[A] - операнд трактуется как адрес
А - операнд трактуется, как непосредственное значение
00 - операнд игнорируется
*/