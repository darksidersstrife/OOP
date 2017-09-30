#include <iostream>
#include <fstream>
#include <string>
#include "IniParser.h"




using std::cin;
using std::cout;
using std::string;


int main()
{
	logger::GetInstance().SetLevels();
	string s;
	std::getline(cin, s);
	try
	{
		IniParser ini_parser(s);
		cout << ini_parser.GetValue<string>("SECTION", "MaxWaitTimeMs");
	}
	catch (std::exception f)
	{
		cout << f.what();
	}
	return 0;
}