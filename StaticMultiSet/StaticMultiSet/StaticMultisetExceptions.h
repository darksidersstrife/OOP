#ifndef _StaticMultiSet_h_
#define _StaticMultiSet_h_
#include <exception>

class EmptySetException : public std::exception
{
	EmptySetException() : exception("Set is empty") {};
};

#endif