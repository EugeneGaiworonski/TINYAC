#ifndef ProcessorH
#define ProcessorH

enum { //
	sStop = 0x0,
	sAuto = 0x1,
	sStep = 0x2,
	sQuit = 0x3
};

enum {
	cMove 	= 0x0, // MOVE A1 00  A3 ; [A3] := [A1]
	cAdd 	= 0x1, // IADD A1 A2 A3 ; [A3] := [A1] + [A2]
	cDiv 	= 0x2, // IDIV A1 A2 A3 ; [A3] := [A1] / [A2]
	cSub 	= 0x3, // ISUB A1 A2 A3 ; [A3] := [A1] - [A3]
	cTrEq 	= 0x4, // TREQ A1 A2 A3 ; if [A1] = [A2] IP := A3 
	cMpy 	= 0x5, // IMPY A1 A2 A3 ; [A3] := [A1] * [A2]
	cTrGt	= 0x6, // TRGT A1 A2 A3 ; if [A1] > [A2] IP := A3
	cStop 	= 0x7, // STOP 00 00 00 ; останов
	cPrint	= 0x8, // PRNT 00 00 A3 ; печатать [A3]  
	cRead	= 0x9, // READ A1 00 A3 ; читать с ленты A1 слов в адрес [A3]
	cWrite	= 0xA, // WRTE A1 00 A3 ; писать на ленту A1 слов с адреса [A3]
	cRewind = 0xB, // RWND 00 00 A3 ; перемотать ленту на зону A3
	cTrOv	= 0xC, // TROV 00 00 A3 ; if OV IP := A3
	cTrLt	= 0xD, // TRLT A1 A2 A3 ; if [A1] < [A2] IP := A3
	cC		= 0xE,
	cD		= 0xF
};

class Processor 
{
	public:	
	int16_t RAM[RAMsize]; //оперативная память
	int8_t 	IP;		// указатель на инструкцию
	int16_t CR; 	// регистр команды
	int16_t SM;		// сумматор
	int16_t OP1;	// операнд 1
	int16_t OP2;	// операнд 2
	int8_t	BR1;    // адрес точки разрыва 1
	int8_t	BR2;	// адрес точки разрыва 1
	bool 	rgW;	// регистр результата 
	bool	rgO;    // регистр переполнения 
	int8_t  mode;   // режим работы
	
	Processor();
	void doCommand();
	void getCommand(int8_t);
};


#endif