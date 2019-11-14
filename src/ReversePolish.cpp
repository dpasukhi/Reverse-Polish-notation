#include "ReversePolish.h"
#include <cmath>
RevPol::RevPol(std::string str)
{
	status = Status::Wait;
	exprs = str;
	calculate();
}

bool RevPol::is_operation(char in)
{
	return (in == '+' || in == '-' || in == '*' || in == '/' ||	in == '^');
}

bool RevPol::is_digit(const char& in)
{
	return (in >= '0' && in <= '9');
}
bool RevPol::is_ParTh(const char& in)
{
	return (in == '(' || in == ')');
}
double RevPol::Result()
{
	std::vector<double> tmp;
	for (int i=0;i<polexprs.size();i++)
	{
		std::string v = polexprs[i];
		if (v == "+" || v == "-" || v == "*" || v == "/" || v == "^")
		{
			if (tmp.size() == 1)
			{
				if (v == "-")
					tmp[0] = -tmp[0];
			}
			else 
			{
				if (v == "+")
				{
					tmp[tmp.size() - 2] += tmp[tmp.size() - 1];
				}
				else if (v == "-")
				{
					tmp[tmp.size() - 2] -= tmp[tmp.size() - 1];
				}
				else if (v == "*")
				{
					tmp[tmp.size() - 2] *= tmp[tmp.size() - 1];
				}
				else if (v == "/")
				{
					tmp[tmp.size() - 2] /= tmp[tmp.size() - 1];
				}
				else if (v == "^")
				{
					tmp[tmp.size() - 2] = pow(tmp[tmp.size() - 2],tmp[tmp.size() - 1]);
				}
				tmp.pop_back();
			}
		}
		else
		{
			tmp.push_back(stod(v));
		}
	}
	if (tmp.size()!=1)
	{
		std::cout << "Error" << std::endl;
	}
	return tmp.at(0);

}
void RevPol::reads()
{
	for (int i=0;i<exprs.length();i++)
	{
		if (is_digit(exprs[i]))
		{
			std::string st;
			st = exprs[i];
			while (is_digit(exprs[i + 1]))
			{
				i++;
				st += exprs[i];
			}
			Type temp(st, EType::Lexeme);
			tmp.push_back(temp);
		}
		else if (is_operation(exprs[i]))
		{
			std::string st;
			st = exprs[i];
			Type temp(st, EType::Operation);
			if ((exprs[i] == '+' || exprs[i] == '-') & (i == 0 || exprs[i - 1] == '('))
				temp.pr = Priority::O;
			else if ((exprs[i] == '*' || exprs[i] == '/'|| exprs[i] == '^'))
				temp.pr = Priority::T;
			else if ((exprs[i] == '+' || exprs[i] == '-'))
				temp.pr = Priority::F;
			tmp.push_back(temp);
		}
		else if (is_ParTh(exprs[i]))
		{
			std::string st;
			st = exprs[i];
			Type temp(st, EType::ParTh);
			if (exprs[i] == '(')
				temp.pr = Priority::Z;
			else 
				temp.pr = Priority::F;
			tmp.push_back(temp);
		}
		else throw "Error input";
	}
}

void RevPol::calculate()
{
	reads();
	for (int i = 0; i < tmp.size(); i++)
	{
		switch (status)
		{
		case Status::Wait:
			switch (tmp[i].type)
			{
			case EType::Operation:
				switch (tmp[i].pr)
				{
				case Priority::O:
					if (tmp[i].str == "-") 
					{
						One(i);
						status = Status::Wait_Lexeme;
					}
					break;
				default:
					status = Status::Error;
					break;
				}
				break;
			case EType::Lexeme:
				polexprs.push_back(tmp[i].str);
				status = Status::Wait_Operation;
				break;
			case EType::ParTh:
				if (tmp[i].pr == Priority::F)
					status = Status::Error;
				else
				{
					oper.push(tmp[i]);
					status = Status::Wait;
				}
				break;
			default:
				status = Status::Error;
				break;
			}
			break;
			//Done
		case Status::Wait_Lexeme:
			switch (tmp[i].type)
			{
			case EType::Operation:
				status = Status::Error;
				break;
			case EType::Lexeme:
				polexprs.push_back(tmp[i].str);
				status = Status::Wait_Operation;
				break;
			case EType::ParTh:
				if (tmp[i].pr == Priority::F)
					status = Status::Error;
				else
				{
					oper.push(tmp[i]);
					status = Status::Wait;
				}
				break;
			default:
				status = Status::Error;
				break;
			}
			break;
		case Status::Wait_Operation:
			switch (tmp[i].type)
			{
			case EType::Operation:
				if (tmp[i].pr==Priority::F)
				{
					if (oper.empty()) 
					{
						One(i);
						status = Status::Wait_Lexeme;
					}
					else {
						switch (oper.top().pr)
						{
						case Priority::T:
							//polexprs.push_back(oper.top().str);
							//oper.pop();
							Two(i);
							i--;
							status = Status::Wait_Operation;
							break;
						case Priority::F:
							//polexprs.push_back(oper.top().str);
							//oper.pop();
							Two(i);
							i--;
							status = Status::Wait_Operation;;
							break;
						case Priority::Z:
							One(i);
							status = Status::Wait_Lexeme;
							break;
						case Priority::O:
							Two(i);
							i--;
							status = Status::Wait_Operation;
							break;
						default:
							status = Status::Error;
							break;
						}
					}
				}
				else if (tmp[i].pr == Priority::T)
				{
					if (oper.empty())
					{
						One(i);
						status = Status::Wait_Lexeme;
					}
					else {

						switch (oper.top().pr)
						{
						case Priority::T:
							//polexprs.push_back(oper.top().str);
							//oper.pop();
							Two(i);
							i--;
							status = Status::Wait_Operation;;
							break;
						case Priority::F:
							//oper.push(tmp[i]);
							One(i);
							status = Status::Wait_Lexeme;;
							break;
						case Priority::Z:
							One(i);
							status = Status::Wait_Lexeme;
							break;
						case Priority::O:
							One(i);
							status = Status::Wait_Lexeme;
							break;
							break;
						default:
							status = Status::Error;
							break;
						}
					}
				}
				else 
				{
					status = Status::Error;
				}
				break;
			case EType::Lexeme:
				status = Status::Error;
				break;
			case EType::ParTh:
				if (tmp[i].pr == Priority::Z)
					status = Status::Error;
				else
				{
					if (oper.empty() )
						status = Status::Error;
					else if(oper.top().type==EType::Operation)
					{
						/*polexprs.push_back(oper.top().str);
						oper.pop();*/
						Two(i);
						i--;
						status = Status::Wait_Operation;
					}
					else if (oper.top().type == EType::ParTh&&oper.top().pr==Priority::Z)
					{
						//oper.pop();
						Three(i);
						status = Status::Wait_Operation; //наверное
					} 
					else
					{
						status = Status::Error;
					}
				}
				break;
			default:
				status = Status::Error;
				break;
			}
			break;
		case Status::Done:
			i = tmp.size() + 222;
			break;
		case Status::Error:
			std::cout << "Error" << std::endl;
			//throw "Error"; 
			goto MOMO;
			break;
		default:
			Status::Error;
			break;
		}
	}
	if (!oper.empty() && status != Status::Error)
	{
		int si = oper.size();
		for (int j = 0; j < si; j++)
		{
			polexprs.push_back(oper.top().str);
			oper.pop();
		}
	}
MOMO:
	int k;
		//std::cout << "Error" << std::endl;
}


void RevPol::One(int ind)
{
	oper.push(tmp[ind]);
}

//Cделать после вызова ind--
void RevPol::Two(int ind)
{
	polexprs.push_back(oper.top().str);
	oper.pop();
}

void RevPol::Three(int ind)
{
	oper.pop();
}
//меняет статус на Done
void RevPol::Four(int ind)
{
	status = Status::Done;
}
//меняет стутус на Error
void RevPol::Five(int ind)
{
	status = Status::Error;
}

void RevPol::Print_Revers()
{
	for (const auto& v:polexprs)
	{
		std::cout << v << " ";
	}
	std::cout << std::endl;
}

void RevPol::Print_Begin()
{
	std::cout << exprs;
	std::cout << std::endl;
}
