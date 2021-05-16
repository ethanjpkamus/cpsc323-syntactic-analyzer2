#include <vector>
#include <string>
#include <cctype>
#include <ctype.h> //isdigit()
#include <iostream>
#include <iomanip>

#include "elem.cpp"

using std::vector;
using std::string;
using std::cout;
using std::endl;
using std::setw;

/*
 * Arithmetic Production Rules
 * <Expression> -> <Expression> + <Term> | <Term>
 * <Term> -> <Term> * <Factor> | <Factor>
 * <Factor> -> ( <Expression> ) | id
 *
 * or
 *
 * E -> E + T
 * E -> T
 * T -> T * F
 * T -> F
 * F -> (E)
 * F -> id
 */

enum cols {
	id = 0,
	add,
	mul,
	op, //open parenthesis
	cp, //close parenthesis
	dol, //dollar symbol
	E,
	T,
	F
};

// table
// 0 indicates empty
elem table[][9] = {
	//   | id       | +        | *        | (        | )         | $        | E         | T         | F
	/*0*/{elem(S,5), elem(),    elem(),    elem(S,4), elem(),     elem(),    elem(ST,1), elem(ST,2), elem(ST,3) },
	/*1*/{elem(),    elem(S,6), elem(),    elem(),    elem(),     elem(A,0), elem(),     elem(),     elem()     },
	/*2*/{elem(),    elem(R,2), elem(S,7), elem(),    elem(R,2),  elem(R,2), elem(),     elem(),     elem()     },
	/*3*/{elem(),    elem(R,4), elem(R,4), elem(),    elem(R,4),  elem(R,4), elem(),     elem(),     elem()     },
	/*4*/{elem(S,5), elem(),    elem(),    elem(S,4), elem(),     elem(),    elem(ST,8), elem(ST,2), elem(ST,3) },
	/*5*/{elem(),    elem(S,6), elem(S,6), elem(),    elem(R,6),  elem(R,6), elem(),     elem(),     elem()     },
	/*6*/{elem(S,5), elem(),    elem(),    elem(S,4), elem(),     elem(),    elem(),     elem(ST,9), elem(ST,3) },
	/*7*/{elem(S,5), elem(),    elem(),    elem(S,4), elem(),     elem(),    elem(),     elem(),     elem(ST,10)},
	/*8*/{elem(),    elem(S,6), elem(),    elem(),    elem(S,11), elem(),    elem(),     elem(),     elem()		},
	/*9*/{elem(),    elem(R,1), elem(S,7), elem(),    elem(R,1),  elem(R,1), elem(),     elem(),     elem()		},
	/*0*/{elem(),    elem(R,3), elem(S,3), elem(),    elem(R,3),  elem(R,3), elem(),     elem(),     elem()		},
	/*1*/{elem(),    elem(R,5), elem(S,5), elem(),    elem(R,5),  elem(R,5), elem(),     elem(),     elem()		}
};

void parser(vector<sig_item>);
bool tos_is_num(string);
string create_input_string(vector<sig_item>);
int find_i(string);
int find_pop(int);
string find_lhs(int);

void parser(vector<sig_item> v)
{
	//check for comments
	if(v.size() == 0)
	{ return; }

	vector<string> stack;
	int i = 0;
	int qm = 0;
	int input_index = 0; //increment this based on shift/reduce actions

	//====================================================================
	string input = create_input_string(v);

	stack.push_back("$");
	stack.push_back("0");

	TYPE result;
	do {
		if(tos_is_num(stack.back())){ //number always on TOS
			qm = stoi(stack.back());
			i = find_i(input[input_index]);//token from input string
			elem x = table[qm,i];
			TYPE t = x.thing;

			switch(t){
				case S:
					stack.push_back(input[input_index]); //push back token
					input_index++;
					stack.push_back(to_string(x.num)); //push back state
					break;
				case R:
					int pop = find_pop(x.num);

					for (int i = 0; i < pop; ++i) //pop based on rhs
					{ stack.pop_back(); }

					//push back lhs,
					string s = find_lhs(x.num);
					stack.push_back(s);
					//then state in GOTO part

					break;
				case ST: //program will never reach this?
					break;
				case A:
					break;
				case EMP:
					break;
				default: //program will never reach this
					break;
			}
		} else {
			cout << "error" << endl;
			break;
		}
	} while(result != A);
	//====================================================================
}

bool tos_is_num(string s)
{
	return isdigit(s[0]);
}

string create_input_string(vector<sig_item> v)
{
	string s = "";
	int n = v.size();

	for(int i = 0; i < n; ++i)
	{ s += v[i].lexeme; }

	s += "$";
	return s;
}

int find_i(char c)
{
	if(isalpha(c)) {
		return id;
	} else if (c == '+') {
		return add;
	} else if (c == '*') {
		return mul;
	} else if (c == '(') {
		return op;
	} else if (c == ')') {
		return cp;
	} else if (c == '$') {
		return dol;
	} else {
		return -1;
	}
}

int find_pop(int r)
{
	//determine number of elements on rhs of production
	if(r % 2 == 1) { // rules 1,3,5
		return 3;
	} else if (r % 2 == 0) { // rules 2,4,6
		return 1;
	} else {
		return -1;
	}
}

string find_lhs(int r)
{
	//determine the rule that is used
	if (r == 1 || r == 2){
		return "E";
	} else if (r == 3 || r == 4) {
		return "T";
	} else if (r == 5 || r == 6) {
		return "F";
	} else {
		return "";
	}
}
