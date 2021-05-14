#include <fstream>
#include <iomanip>
// #include <iostream>
// #include <vector>
#include "lexer.cpp"
#include "parser.cpp"

using std::vector;
using std::cout;
using std::endl;
using std::string;
using std::ifstream;
using std::setw;

int main(int argc, char* argv[])
{
	//check if command line input is correct
	if(argc != 2)
	{
		cout << "not swag" << endl;
		return EXIT_FAILURE;
	}

	string filename = argv[1];
	string s = "";
	char t;
	ifstream inputfile;
	vector<sig_item> stuff;
	

	//open input file and add it to ifstream object
	inputfile.open(filename);
	
	if(!inputfile.is_open())
	{
		cout << "file cannot be opened" << endl;
		return EXIT_FAILURE;
	}
	while (getline(inputfile,s))
	{
		stuff = lexer(s);
		// cout << "parser called" << endl;
		parser(stuff);
	}

	inputfile.close();

	return 0;
}