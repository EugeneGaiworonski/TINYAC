   Name: Virtual Training Automatic Computing Machine TINYAC.
	Version: Build I.
	
   Copyright (C) 2022  Eugene Gaiworonski.

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see https://www.gnu.org/licenses/.
	
	Date: 1.12.21
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
	|CODE|OPERATION NAME                  |OPERATION CONTENT        |
	+----+--------------------------------+-------------------------+
	|000 |copy                            |A1 ==> A3                |
	|001 |addition                        |A1 + A2 ==> A3           |
	|010 |division                        |A1 / A2 ==> A3           |
	|011 |subtraction                     |A1 - A2 ==> A3           |
	|100 |conditional transition by equal |if A1=A2 transfer to A3  |
	|101 |multiplication                  |A1 * A2 ==> A3           |
	|110 |conditional transition by above |if A1>A2 transfer to A3  |
	|111 |output and stop                 |output A1, A2, A3; stop  |
	+----+--------------------------------+-------------------------+
	The TINYAC  operates with integers. Like the commands, the numbers 
	are 16-bit. Obviously, the maximum number that the TINYAC is still 
	able to place in its memory is 32767 and the minimum number is -32768. 
	If the result is larger, it goes beyond the 16-bit machine word and 
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
	|CODE|OPERATION NAME                  |ASSEMBLER MNEMONIC       |
	+----+--------------------------------+-------------------------+
	|000 |copy                            |COPY                     |
	|001 |addition                        |ADD                      |
	|010 |division                        |DIV                      |
	|011 |subtraction                     |SUB                      |
	|100 |conditional transition by equal |TREQ                     |
	|101 |multiplication                  |MPY                      |
	|110 |conditional transition by above |TRGT                     |
	|111 |output and stop                 |PRST                     |
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
