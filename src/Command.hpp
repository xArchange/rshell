#ifndef _COMMAND_HPP_
#define _COMMAND_HPP_

#include "Semicolon.hpp"
#include "DoubleAnd.hpp"
#include "DoubleOr.hpp"
#include "Arguments.hpp"
#include "Executable.hpp"

class Command 
{
  public:
    Command(char* e, char* arg, Connector c);
    ~Command();

    Executable getExecutable();
    Arguments getArguments();
    Connector getConnector();

    void setExecutable(Executable e);
    void setArguments(Arguments arg);
    void setConnector(Connector c);
  
  protected:
    Executable executable;
    Arguments arguments;
    Connector connector;
};

#endif
