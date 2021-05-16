#include <string>

using std::string;

//make an array of these in the lexer function?
struct sig_item 
{

	sig_item()
	: token("N/A"), lexeme("N/A")
	{}
	
	sig_item(string t, string l) 
	: token(t), lexeme(l)
	{}

	string token;
	string lexeme; 
};