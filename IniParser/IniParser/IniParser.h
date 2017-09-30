#ifndef IniParser_h
#define IniParser_h


#include <map>
#include <sstream>
#include "IniParserExceptions.h"
#include <logger.h>


class IniParser
{
public:
	IniParser() = default;
	IniParser(const std::string& fileName);
	void ParseFile(const std::string& fileName);
	void DeleteData();
	template<class T>
	T GetValue(const std::string& section, const std::string& parameter)
	{
		auto it = Data.find(make_pair(section, parameter));
		if (it == Data.end())
		{
			logger::GetInstance().Log(logger::error, "No section or parameter");
			throw WrongDataException("No section or parameter");
		}
		std::stringstream ss(it->second);
		T Val;
		ss >> Val;
		return Val;
	}

private:
	void ParseString(std::string& s, std::string& current_section, unsigned int string_number);

	std::map<std::pair<std::string, std::string>, std::string> Data;
};

#endif