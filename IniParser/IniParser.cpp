#include "IniParser.h"
#include <string>
#include <sstream>
#include <fstream>
using namespace std;

IniParser::IniParser(const string& fileName)
{
	ParseFile(fileName);
}

void IniParser::DeleteData()
{
	Data.clear();
}
void IniParser::ParseString(std::string& s, std::string& current_section, unsigned int string_number){	int comment_beginning = s.find_first_of(';');
	if (comment_beginning != string::npos)
	{
		s.erase(comment_beginning);
	}	istringstream ss(s);
	string str = "";
	ss >> str;
	if (s != "")
	{
		if (s[0] == '[')
		{
			if (s.back() == ']')
			{
				current_section = str.substr(1, str.size() - 2);
			}
			else
			{
				throw InvalidIniFileException("No pair for the '[' symbol", string_number);
			}
		}
		else		{
			if (current_section == "")
			{
				throw InvalidIniFileException("Data goes before any section", string_number);
			}

			string parameter_name = str;
			if (!ss.eof())
			{
				ss >> str;
				if (str == "=" && !ss.eof())
				{
					string value;
					ss >> value;
					ss >> str;
					if (ss.eof())
					{
						Data[pair<string, string>(current_section, parameter_name)] = value;
					}
					else
					{
						throw InvalidIniFileException("Value containes spaces", string_number);
					}
				}
				else
				{
					throw InvalidIniFileException("No equality symbol or parameter name contains spaces", string_number);
				}
			}
			else
			{
				throw InvalidIniFileException("No value for parameter", string_number);
			}
		}
	}}
void IniParser::ParseFile(const string& fileName)
{
	std::ifstream File;
	File.open(fileName);
	if (!File.good())
	{
		throw std::ifstream::failure("Failed to open file");
	}

	string s, current_section;
	unsigned int string_number = 0;
	while (!File.eof())
	{
		getline(File, s);
		string_number++;
		ParseString(s, current_section, string_number);
	}
}