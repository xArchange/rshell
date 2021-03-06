/**
 * \file Arguments.cpp
 * \author Candice Bentejac
 * \date November 6, 2015
 * \brief Definitions of the Arguments class.
 */

#include "Arguments.hpp"

using namespace std;


Arguments::Arguments(string arg) : arguments(arg)
{

}


Arguments::~Arguments()
{

}


string Arguments::getArguments()
{
  return arguments;
}


void Arguments::setArguments(string arg)
{
  arguments = string(arg); 
}


void Arguments::readArguments()
{
  cout << arguments << endl;
}
