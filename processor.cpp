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
Processor::Processor() 
{
	IP = 0;	
	CR = 0;
	SM = 0;
	OP1 = 0;
	OP2 = 0;
	rgW = 0;
	rgO = 0;
	BR1 = 0;
	BR2 = 0;
	for(auto i = 0; i <RAMsize; i++)	
		RAM[i] = 0; 
	mode = sStop;
}

void Processor::doCommand() 
{	
	const uint16_t 	opcodeMask = 0xF000;
	const uint16_t 	op1Mask = 0x0F00;
	const uint16_t 	op2Mask = 0x00F0;
	const uint16_t 	op3Mask = 0x000F;	
	int8_t 			opcode = {0};
	int8_t 			op1adr = {0};
	int8_t 			op2adr = {0};
	int8_t 			op3adr = {0};
	
	//читаем команду на регистр CR
	getCommand(IP);
	//**************************************
	Sleep(1000/100); //100 операций в сек 
	//**************************************
	//debug
	//cout<< hex << uppercase << setw(2) << setfill('0') << (short)IP<< ' ' << dec;
	//end debug
	// переходим к следующей команде
	IP++; 
	// если достигли конца памяти
	if(IP >= RAMsize) {
		mode = sStop; 
		IP = 0;
		}
	// обработка точек разрыва
	if((IP == BR1) && (BR1 != 0)) mode = sStep;
	if((IP == BR2) && (BR2 != 0)) mode = sStep;
	// разбираем команду на операнды
	opcode = (CR & opcodeMask) >> 12;
	op1adr = (CR & op1Mask) >> 8;
	op2adr = (CR & op2Mask) >> 4;
	op3adr = (CR & op3Mask);
	//выполняем команду
	switch(opcode) {
		case cMove:
			OP1 = RAM[op1adr];
			SM = OP1;
			RAM[op3adr] = SM;
			break;
		case cAdd:
			OP1 = RAM[op1adr];
			OP2 = RAM[op2adr];
			SM = OP1 + OP2;
			RAM[op3adr] = SM;
			if(SM < 0) 
				rgW = true; 
			// переполнение
			if(((OP2 > 0) && (OP1 > (SHRT_MAX - OP2))) || ((OP2 < 0) && (OP1 < (SHRT_MIN - OP2))))  
				rgO = true; 
			else 
				rgO = false;
			break;
		case cDiv:
			OP1 = RAM[op1adr];
			OP2 = RAM[op2adr];
			SM = OP1 / OP2;
			RAM[op3adr] = SM;
			if (SM < 0) 
				rgW = true;
			// переполнение
			if((OP2 == 0) || ((OP1 == SHRT_MIN) && (OP2 == -1))) 
				rgO = true;
			else 
				rgO = false;
			break;
		case cSub:
			OP1 = RAM[op1adr];
			OP2 = RAM[op2adr];
			SM = OP1 - OP2;
			RAM[op3adr] = SM;
			if(SM < 0) 
				rgW = true;
			// переполнение
			if((OP2 > 0 && OP1 < SHRT_MIN + OP2) || (OP2 < 0 && OP1 > SHRT_MAX + OP2)) 
				rgO = true;
			else 
				rgO = false;
			break;
		case cTrEq:
			OP1 = RAM[op1adr];
			OP2 = RAM[op2adr];
			SM = OP1 - OP2;
			if(SM < 0) 
				rgW = true;
			if(SM == 0) 
				IP = op3adr;
			break;
		case cMpy:
			OP1 = RAM[op1adr];
			OP2 = RAM[op2adr];
			SM = OP1 * OP2;
			RAM[op3adr] = SM;
			if(SM < 0) 
				rgW = true;
			//переполнение
			rgO = false;
			if(OP1 > 0) { /* OP1 > 0 */
				if(OP2 > 0) { /* OP1 > 0 & OP2 > 0 */
					if(OP1 > (SHRT_MAX / OP2)) {
						rgO = true;
						}
					} 
					else{ /* OP1 > 0, OP <= 0 */
						if(OP2 < (SHRT_MIN / OP2)) {
							rgO = true; /* Handle error */
							}
						} /* OP1 > 0, OP2 <= 0 */
					} 
			else{ /* OP1 <= 0si_a is nonpositive */
				if(OP2 > 0) { /* OP1 <= 0, OP2 > 0 */
					if(OP1 < (SHRT_MIN / OP2)) {
						rgO = true; /* Handle error */
						}
					} 
				else{ /* OP1 <= 0 & OP2 <= 0 */
					if((OP1 != 0) && (OP2 < (SHRT_MAX / OP1))) {
						rgO = true; /* Handle error */
						}
					}	 
				}	 
			break;
		case cTrGt:
			OP1 = RAM[op1adr];
			OP2 = RAM[op2adr];
			OP1 = RAM[op1adr];
			OP2 = RAM[op2adr];
			SM = OP1 - OP2;
			if(SM < 0) 
				rgW = true;
			if(SM > 0) 
				IP = op3adr;
			break;
		case cStop:
			mode = sStop;
			break;
		case cPrint:
			
			break;
		case cRead:
			
			break;
		case cWrite:
			
			break;
		case cRewind:
			
			break;
		case cTrOv:
			if(rgO) 
				IP = op3adr;
			break;
		case cTrLt:
			OP1 = RAM[op1adr];
			OP2 = RAM[op2adr];
			OP1 = RAM[op1adr];
			OP2 = RAM[op2adr];
			SM = OP1 - OP2;
			if(SM < 0) 
				rgW = true;
			if(SM > 0) 
				IP = op3adr;
			break;
		default:
			
			break;
	}	 		
}

void Processor::getCommand(int8_t) 
{
	 CR = RAM[IP];
}
