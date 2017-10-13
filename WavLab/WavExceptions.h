#pragma once

class WavHeaderException : public std::exception
{
public:
	WavHeaderException(const std::string& s = "") : exception((std::string("Wav header error: ") + s).c_str()) {};
};