#include "ReversePolish.h"
RevPol::RevPol(std::string str)
{
	status = Status::Wait;
	exprs = str;
	//calculate();
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
	//reads();
	//correct();
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
						status = Status::Wait_Operation; //íàâåğíîå
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

void RevPol::correct()
{
}

void RevPol::One(int ind)
{
	oper.push(tmp[ind]);
}

//Cäåëàòü ïîñëå âûçîâà ind--
void RevPol::Two(int ind)
{
	polexprs.push_back(oper.top().str);
	oper.pop();
}

void RevPol::Three(int ind)
{
	oper.pop();
}
//ìåíÿåò ñòàòóñ íà Done
void RevPol::Four(int ind)
{
	status = Status::Done;
}
//ìåíÿåò ñòóòóñ íà Error
void RevPol::Five(int ind)
{
	status = Status::Error;
}
