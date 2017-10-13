#ifndef IniParserExceptions_h
#define IniParserExceptions_h


#include <exception>
#include <string>

class InvalidIniFileException : public std::exception
{
public:
	InvalidIniFileException(const std::string& s, int stringNumber) : exception((std::string("Invalid ini file - ") + s + ", found in string " + std::to_string(stringNumber)).c_str()) {};
};

class WrongTypeException : public std::exception
{
public:
	WrongTypeException(const std::string& s) : exception(s.c_str()) {};
};

class WrongDataException : public std::exception
{
public:
	WrongDataException(const std::string& s) : exception(s.c_str()) {};
};


#endif