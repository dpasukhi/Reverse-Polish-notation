#include <iostream>
#include "ReversePolish.h"
#include <string>
using namespace std;
int main()
{
	//string str{ "154+245*3+4-5+(8+1)" };
	//string str2{ "1+2" };
	string str{ "3/(-3)" };
	RevPol a(str);
	//a.Print_Begin();
	//a.Print_Revers();
	//cout << a.Result();
	//cout << endl << "asdsa" << endl;
	cout << RevPol(str);
	//system("pause>nul");
	string st;
	while (true)
	{

		cin >> st;
		if (st == "stop")
			break;
		cout << RevPol(st);
	}
	system("pause>nul");
	return 0;
}