#include <vector>
#include <string>
#include <cctype>
#include <iostream>
#include <iomanip>

#include "sig_item.cpp"

using std::vector;
using std::string;
using std::cout;
using std::endl;

enum states
{
	rej_ = 0,
	int_ = 1,
	ide_ = 2,
	sep_ = 3,
	opr_ = 4,
	com_ = 5,
	sps_ = 6,
	rea_ = 7,
	unk_ = 8
};

int statetable[][9] = {
	{rej_, int_, ide_, sep_, opr_, com_, sps_, unk_, unk_},
	{int_, int_, unk_, rej_, rej_, rej_, rej_, rea_, rej_},
	{ide_, ide_, ide_, rej_, rej_, rej_, rej_, unk_, rej_},
	{sep_, rej_, rej_, rej_, rej_, rej_, rej_, unk_, rej_},
	{opr_, rej_, rej_, rej_, opr_, rej_, rej_, unk_, rej_},
	{com_, com_, com_, com_, com_, rej_, com_, com_, rej_},
	{sps_, rej_, rej_, rej_, rej_, rej_, rej_, rej_, rej_},
	{rea_, rea_, unk_, rej_, rej_, rej_, rej_, unk_, rej_},
	{unk_, unk_, unk_, rej_, unk_, unk_, rej_, unk_, unk_}
};

string keywords[] = { "int", "float", "bool", "if", 
					  "else", "then", "endif", "while", 
					  "whileend", "do", "doend", "for", 
					  "forend", "input", "output", "and", 
					  "or", "function", "return", "string" };

vector<sig_item> lexer(string);
int what_char(char);
bool is_sep(char);
bool is_opr(char);
bool is_keyword(string);
string get_token(int);


//go through input file and sort everything out
vector<sig_item> lexer(string line)
{
	vector<sig_item> things;
	int endline = line.length();
	int current_state = rej_;
	int transition = rej_;
	int previous_state = rej_;
	string lexeme = "";
	char current_char = ' ';

	for(int i = 0; i < endline ; )
	{
		current_char = line[i];
		transition = what_char(current_char);
		current_state = statetable[current_state][transition];
		
		// cout << current_char << "" << previous_state << " " << current_state << endl;
		if(current_state == rej_){
			if(previous_state == com_)
			{
				++i;
				lexeme = "";
			}
			else if(previous_state != com_ && previous_state != sps_){
				things.push_back(sig_item(get_token(previous_state),lexeme));
				lexeme = "";
			}
		} else {
			//possibly increment here
			if(!isspace(current_char))
			{ lexeme += current_char; }
			++i;
		}
		previous_state = current_state;
	}

	if((current_state != sps_ && current_state != com_) && lexeme.length() > 0)
	{
		things.push_back(sig_item(get_token(previous_state),lexeme));
	}

	//find all keywords in buffer
	for(int i = 0; i < things.size(); ++i)
	{
		if(is_keyword(things[i].lexeme))
		{
			things[i].token = "KEYWORD";
		}
	}

	return things;
}

int what_char(char c)
{
	if(c == '!') 
	{ return com_; } 
	
	else if(isalpha(c) || c == '_' || c == '$')
	{ return ide_; }
	
	else if(is_sep(c))
	{ return sep_; }

	else if(isdigit(c))
	{ return int_; }

	else if(isspace(c))
	{ return sps_; }

	else if(is_opr(c))
	{ return opr_; }

	else if(c == '.')
	{ return rea_; }

	return unk_;
}

bool is_sep(char c)
{
	return (c == '{' || c == '}' || 
			c == '(' || c == ')' || 
			c == '[' || c == ']' ||
			c == ',' || /*c == '.' ||*/
			c == ':' || c == ';' );
}

bool is_opr(char c)
{
	return (c == '*' || c == '+' || 
			c == '-' || c == '=' || 
			c == '/' || c == '>' ||
			c == '<' || c == '%' );
}

bool is_keyword(string s)
{
	for(int i = 0; i < 20; ++i)
	{
		if(s == keywords[i])
		{ return true; }
	}
	return false;
}

//state to string
string get_token(int i)
{
	switch(i)
	{
		case int_:
			return "INTEGER";
			break;
		case ide_:
			return "IDENTIFIER";
			break;
		case sep_:
			return "SEPERATOR";
			break;
		case opr_:
			return "OPERATOR";
			break;
		case com_:
			return "COMMENT";
			break;
		case sps_:
			return "SPACE";
			break;
		case rea_:
			return "REAL";
			break;
		case unk_:
			return "UNKNOWN";
			break;
		default:
			return "REJECT";
	}
}