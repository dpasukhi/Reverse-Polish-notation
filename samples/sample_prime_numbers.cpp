#include <iostream>
#include "ReversePolish.h"
#include <string>
using namespace std;
int main()
{
	string str{ "154+245*3+4-5+(8+1)" };
	//string str{ "1+2" };
	//string str{ "-5+(8+1)" };
	RevPol a(str);
	a.Print_Begin();
	a.Print_Revers();
	cout << a.Result();
	system("pause>nul");
	return 0;
}