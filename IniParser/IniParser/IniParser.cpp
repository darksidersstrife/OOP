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
				logger::GetInstance().Log(logger::debug, string("Section ") + current_section + string(" found"));
			}
			else
			{
				logger::GetInstance().Log(logger::error, "No pair for the '[' symbol");
				throw InvalidIniFileException("No pair for the '[' symbol", string_number);
			}
		}
		else		{
			if (current_section == "")
			{
				logger::GetInstance().Log(logger::error, "Data goes before any section");
				throw InvalidIniFileException("Data goes before any section", string_number);
			}

			string parameter_name = str;
			logger::GetInstance().Log(logger::debug, string("parameter ") + parameter_name + string(" found"));
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
						logger::GetInstance().Log(logger::debug, string("value ") + value + string(" found"));
					}
					else
					{
						logger::GetInstance().Log(logger::error, "Value containes spaces");
						throw InvalidIniFileException("Value containes spaces", string_number);
					}
				}
				else
				{
					logger::GetInstance().Log(logger::error, "No equality symbol or parameter name contains spaces");
					throw InvalidIniFileException("No equality symbol or parameter name contains spaces", string_number);
				}
			}
			else
			{
				logger::GetInstance().Log(logger::error, "No value for parameter");
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
		logger::GetInstance().Log(logger::error, "Failed to open file");
		throw std::ifstream::failure("Failed to open file");
	}
	logger::GetInstance().Log(logger::debug, string("File") + fileName + string("opened"));

	string s, current_section;
	unsigned int string_number = 0;
	while (!File.eof())
	{
		getline(File, s);
		string_number++;
		ParseString(s, current_section, string_number);
	}
}