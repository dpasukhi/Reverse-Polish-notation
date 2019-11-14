#include <iostream>
#include <queue>
#include <vector>
#include <string>
#include <cstdlib>
#include  <stack>
enum class Priority 
{
	NUl=-1,Z=0,O,T,F
};
enum class Status
{
	Wait = 0,
	Wait_Lexeme,
	Wait_Operation,
	Done,
	Error
};
enum class EType
{
	Operation = 0,
	Lexeme,
	ParTh
};
class Type :std::string
{
public:
	EType type;
	Priority pr;
	std::string str;
	Type(std::string st, EType t, Priority  p=Priority::NUl) :std::string(st) 
	{
		type = t;
		pr=p;
		str = st;
	}
};

class RevPol
{
private:

	std::vector<Type> tmp;
	Status status;
	//std::stack<double> st;
	std::string exprs;
	std::stack<Type> oper;
	std::vector<std::string> polexprs;
	bool is_operation(char);
	bool is_digit(const char&);
	bool is_ParTh(const char&);
	void reads();
	void calculate();
	void One(int);
	void Two(int);
	void Three(int);
	void Four(int);
	void Five(int);
public:	
	RevPol(std::string);
	void Print_Revers();
	void Print_Begin();
	double Result();
};