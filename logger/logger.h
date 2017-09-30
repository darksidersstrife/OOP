#ifndef logger_h
#define logger_h
 

 
#include <iostream>
 

 
class logger
 
{
 
public:
 
  enum LogLevel
 
  {
 
    error = 1,
 
    warning = 2,
 
    debug = 4
 
  };
 
  static logger& GetInstance()
 
  {
 
    static logger log;
 
    return log;
 
  }
 

 
  void SetLevels(char level = 7)
 
  {
 
    Level = level;
 
  }
 

 
  void Log(char l, const std::string& s)
 
  {
 
    if(Level & l)
 
      std::cerr << s << std::endl;
 
  }
 

 
private:
 
  char Level;
 

 
  logger()
 
  {
 
    Level = 0;
 
  }
 
  
 
  
 
  logger(const logger&) = delete;
 
  logger operator =(const logger&) = delete;
 
};
 

 

 
#endif