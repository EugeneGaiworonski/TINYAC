//
// РАЗЛОЖЕНИЕ ЦЕЛОГО ЧИСЛА НА РАЗРЯДЫ
//

#include <iostream>
#include <cmath>
using namespace std;

int main()
{
    int value = 0, digit = 0, result = 0, pwr = 0;
    cout << "V= ";
    cin >> value;
   
    while (value)
    {
        digit = value % 10;
        result = result * 10 + digit;
        value = value / 10;
        if (value) pwr++; 
    }
   
    cout << "R= " << '\n';
   
    while (result)
    {
        cout <<  result % 10 << "x" << pow(10,pwr)<< '\n';
        result = result / 10;
        pwr--;
    }

}
