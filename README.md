# TINIAC
Simple tutorial mainframe model

Диапазон адресов 0000-0377

Длина машинного слова 32 разряда

Формат машинной команды 

A1(32..24) A2(23..16) A3(15..8) КОП(7..0)


Система команд TINIAC
КОП Смысл операции и её мнемоническое обозначение
00 NOOP - пустая операция
01 RADD – сложение вещественных чисел
11 IADD – сложение целых чисел
02 RSUB – вычитание вещественных чисел
12 ISUB – вычитание целых чисел
03 RMPY – умножение вещественных чисел
13 IMPY – умножение целых чисел
04 RDIV – деление вещественных чисел
14 IDIV – деление целых чисел (то же, что и div в Паскале)
15 IMOD – остаток от деления (то же, что и mod в Паскале)
09 MOVE – пересылка: <A1>:=<A3>
10 INT – вещественное в целое: <A1>:=Round(<A3>)
30 REAL – целое в вещественное: <A1>:=Real(<A3>)
09 TRA – безусловный переход: goto A2, т.е. RA:=A2
19 TRC – условный переход:case ω of 0: RA:=A1; 1: RA:=A2; 2: RA:=A3 end
20 TR0 – переход по S = 0: if ω=0 then RA:=A2
21 ПНР – переход по S <> 0: if ω<>0 then RA:=A2
22 TR+ – переход по S > 0: if ω=2 then RA:=A2
23 TR- – переход по S < 0: if ω=1 then RA:=A2
24 TR+= – переход по S >= 0: if ω<>1 then RA:=A2
25 TR-= – переход по S <= 0: if ω<2 then RA:=A2
27 TROV - переход по переполнению  
31 STOP – остановка выполнения программы
06 LDR – ввод A2 вещественных чисел в память, начиная с адреса A1
for i:=1 to А2 do Readln(<А1+i-1>)
07 STR – вывод A2 вещественных чисел, начиная с адреса A1
for i:=1 to А2 do Writeln(<А1+i-1>)
16 LDI – ввод массива целых чисел, аналогично ВВВ
17 STI – вывод массива целых чисел, аналогично ВЫВ
  
Пример программы:
.Программа решения уравнения
.       | x+2, при x < 2,
. y := <  2, при x = 2,
.       | 2*(x+2), при x > 2;
.Раздел объявления имен
*NAME 
X 100
Y 101
C2 011
MIN 005
ZERO 007
PLUS 009
.Раздел кода
*CODE
LDI  X,1,1              .100 101 000 016 - Ввод X, 1 значение, устройство 1
IADD Y, X, C2           .101 100 011 011 - y := x+2
ISUB 000, X, C2         .000 100 011 012 - <000> := x–2; формирование ω
TRC  MIN, ZERO, PLUS    .005 007 009 019 - Case ω of 0: goto 005; 1: goto 007; 2: goto 009
STI  C2,1,0             .011 001 000 017 - Вывод С2, 1 значение, устройство 0
STOP                    .000 000 000 031
STI  Y,1,0              .101 001 000 017 - Вывод Y, 1 значение, устройство 0
STOP                    .000 000 000 031
IADD Y,C2,Y             .101 011 101 011 - y := 2*y = y+y;
TRA  007                .000 007 000 009 - Goto 007
.Раздел данных
*DATA
C2 02                   .000 000 000 002 - число +2
*END
