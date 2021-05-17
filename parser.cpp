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
using std::left;
using std::right;
using std::to_string;

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

typedef enum {
	id = 0,
	add,
	mul,
	op, //open parenthesis
	cp, //close parenthesis
	dol, //dollar symbol
	E,
	T,
	F
} COLS;

// 0 indicates empty
elem table[12][9] = {
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
int find_i(char);
int find_pop(int);
string find_lhs(int);
COLS stoC(string);
void store_stack(output&, vector<string>&);
void store_input_string(output&, string, int);
string get_rule(int);

void parser(vector<sig_item> v)
{
	//check for comments
	if(v.size() == 0) 
	{ return; }

	vector<string> stack;
	vector<output> printable;
	int i = 0;
	int qm = 0;
	int input_index = 0; //increment this based on shift/reduce actions

	// begin bottom-up parsing
	string input = create_input_string(v);

	stack.push_back("$"); // Place $ at the end of the input string	
	stack.push_back("0"); // Push state 0 on to the stack
	TYPE result;
	bool err = false;

	cout << "\ninput string: " << input << endl;
	cout << setw(15) << left << "stack" 
		 << setw(15) << left << "input" 
		 << setw(15) << left << "table entry" 
		 << setw(15) << left << "action" << endl;
	do {
		output o;
		store_stack(o,stack);
		if(tos_is_num(stack.back())){ //number always on TOS
			qm = stoi(stack.back()); // let qm be the current state (tos)
			i = find_i(input[input_index]); // and i be the token.
			elem x; //Find x = Table[Qm,i]
			x.thing = table[qm][i].thing;
			x.num = table[qm][i].num; 
			result = x.thing;

			store_input_string(o, input, input_index);
			switch(result) //case x of
			{ 
				case S:
				{
					o.ao += ("Push(" + string(1,input[input_index]) + ");");
					stack.push_back(string(1,input[input_index])); //push back token
					input_index++;
					stack.push_back(to_string(x.num)); //push back state
					o.to += ("S" + to_string(x.num));
					o.ao += ("Push(" + to_string(x.num) + ")");

					break;
				}
				case R:
				{
					//Reduce by production #n by popping 2x # of RHS symbols
					int pop = 2 * find_pop(x.num);

					for (int i = 0; i < pop; ++i) 
					{ stack.pop_back(); }
					int qj = stoi(stack.back()); //let Qj be the TOS state
					 
					string l = find_lhs(x.num); //push back lhs on to stack,
					// cout << l << endl;
					stack.push_back(l);
					
					string qk = to_string(table[qj][stoC(l)].num); // push qk = table[qj,l] on to the stack
					stack.push_back(qk);
					o.to += ("R" + to_string(x.num));
					o.ao += (get_rule(x.num) + "Table[" + to_string(qj) + "," + l + "] = " + qk);
					break;
				}
				// case ST: //program will never reach this?
				// 	break;
				case A:
				{
					// cout << "parsing is complete" << endl;
					o.to += "ACCT";
					break;
				}
				case EMP:
				{
					// cout << "error parsing, not valid input" << endl << endl;
					err = true;
					break;
				}
				default: //program will never reach this
					cout << "default" << endl;
			}
		} else {
			cout << "number not at TOS" << endl;
			err = true;
			break;
		}
		
		printable.push_back(o);
	} while(!err && result != A && result != EMP); 
	//print out everything from printable
	int n = printable.size();
	for (int i = 0; i < n; ++i)
	{
		cout << setw(15) << left << printable[i].so << setw(15) << left << printable[i].io << setw(15) << left << printable[i].to << setw(15) << left << printable[i].ao << endl;
	}
	if(err){
		cout << "error parsing, not valid input" << endl << endl;
	}
} // end of parser

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

COLS stoC(string s)
{
	if(s == "E") {
		return E;
	} else if (s == "T") {
		return T;
	} else if (s == "F") {
		return F;
	} else {
		return E;
	}
}

void store_stack(output &o, vector<string> &stack)
{
	int n = stack.size();
	for(int i = 0; i < n ; ++i)
	{ 
		o.so += stack[i]; 
	}
}

void store_input_string(output &o, string s, int index)
{
	int n = s.size();
	for(int i = index; i < n; ++i)
	{
		o.io += s[i];
	}
}

string get_rule(int i)
{
	switch(i)
	{
		case 1:
			return "E -> E + T";
		case 2:
			return "E -> T";
		case 3:
			return "E -> T * F";
		case 4:
			return "T -> F";
		case 5:
			return "F -> (E)";
		case 6:
			return "F -> id";
		default:
			return "";
	}
}