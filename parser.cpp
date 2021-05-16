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
	op,
	cp,
	dol,
	E,
	T,
	F
};

// 0 indicates empty
elem table[][9] = {
	//   | id       | +        | *        | (        | )         | $        | E         | T         | F     
	/*0*/{elem(S,5), elem(),    elem(),    elem(S,4), elem(),     elem(),    elem(ST,1), elem(ST,2), elem(ST,3)},
	/*1*/{elem(),    elem(S,6), elem(),    elem(),    elem(),     elem(A,0), elem(),     elem(),     elem()},
	/*2*/{elem(),    elem(R,2), elem(S,7), elem(),    elem(R,2),  elem(R,2), elem(),     elem(),     elem()},
	/*3*/{elem(),    elem(R,4), elem(R,4), elem(),    elem(R,4),  elem(R,4), elem(),     elem(),     elem()},
	/*4*/{elem(S,5), elem(),    elem(),    elem(S,4), elem(),     elem(),    elem(ST,8), elem(ST,2), elem(ST,3)},
	/*5*/{elem(),    elem(S,6), elem(S,6), elem(),    elem(R,6),  elem(R,6), elem(),     elem(),     elem()},
	/*6*/{elem(S,5), elem(),    elem(),    elem(S,4), elem(),     elem(),    elem(),     elem(ST,9), elem(ST,3)},
	/*7*/{elem(S,5), elem(),    elem(),    elem(S,4), elem(),     elem(),    elem(),     elem(),     elem(ST,10)},
	/*8*/{elem(),    elem(S,6), elem(),    elem(),    elem(S,11), elem(),    elem(),     elem(),     elem()},
	/*9*/{elem(),    elem(R,1), elem(S,7), elem(),    elem(R,1),  elem(R,1), elem(),     elem(),     elem()},
	/*0*/{elem(),    elem(R,3), elem(S,3), elem(),    elem(R,3),  elem(R,3), elem(),     elem(),     elem()},
	/*1*/{elem(),    elem(R,5), elem(S,5), elem(),    elem(R,5),  elem(R,5), elem(),     elem(),     elem()}
};

void parser(vector<sig_item>);
bool tos_is_num(string);
string create_input_string(vector<sig_item>);

void parser(vector<sig_item> v)
{
	//check for comments
	if(v.size() == 0) 
	{ return; }

	vector<string> stack;
	int i = 0;
	int qm = 0;

	//====================================================================
	//place $ at the end of the input string
	string input = create_input_string(v);
	
	//push state 0 on the stack
	stack.push_back("$");
	stack.push_back(v[0].lexeme);

	// cout << input << endl;
	TYPE result;
	do {
		if(tos_is_num(stack.back())){ //number always on TOS
			qm = stoi(stack.back());
			i = 	
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