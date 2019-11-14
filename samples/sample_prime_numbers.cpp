#include <iostream>
#include "ReversePolish.h"
#include <string>
#include <vector>
using namespace std;
int main()
{
	string str{ "154+245*3+4-5+(8+1)" };
	//string str{ "1+2" };
	//string str{ "-5+(8+1)" };
	RevPol a(str);
	a.reads();
	a.calculate();
	cout << str << endl;
	for (const auto& i : a.tmp)
		cout << i.str << " ";
	cout << endl;
	for (const auto& i : a.polexprs)
		cout << i << " ";
}