#include "Command.hpp"

using namespace std;

Command::Command(string line) : command(line)
{
  executables.clear();
  arguments.clear();
  connectors.clear();
}

Command::~Command()
{

}

/* Getters and Setters */
string Command::getLine()
{
  return command;
}

void Command::setLine(string cmd)
{
  command = cmd;
}

vector<Executable> Command::getExecutables()
{
  return executables;
}

void Command::setExecutables(vector<Executable> ex)
{
  executables = ex;
}

vector<Arguments> Command::getArguments()
{
  return arguments;
}

void Command::setArguments(vector<Arguments> args)
{
  arguments = args;
}

vector<Connector> Command::getConnectors()
{
  return connectors;
}

void Command::setConnectors(vector<Connector> connect)
{
  connectors = connect;
}


void Command::addExecutable(Executable e)
{
  executables.push_back(e);
}

void Command::addArguments(Arguments a)
{
  arguments.push_back(a);
}

void Command::addConnector(Connector c)
{
  connectors.push_back(c);
}


/* Static methods that will be used to handle the case were the user doesn't respect the connectors syntax */
/*static bool containSemicolon(char* str)
{
  char* s = strchr(str, ';');
  if(s != NULL)
    return true;
  return false;
}*/


static bool endWithSemicolon(char* str) // Returns true if the string ends with a semicolon
{
  string s(str);
  if(s[s.size() - 1] == ';')
    return true;
  return false;
}

/*static bool endWithAnd(char *str)
{
  string s(str);
  if(s.size() < 2)
    return false;
  if(s[s.size() - 1] == '&' && s[s.size() - 2] == '&')
    return true;
  return false;
}


static bool endWithOr(char *str)
{
  string s(str);
  if(s.size() < 2)
    return false;
  if(s[s.size() - 1] == '|' && s[s.size() - 2] == '|')
    return true;
  return false;
}


static bool endWithConnector(char* str)
{
  return endWithSemicolon(str) || endWithAnd(str) || endWithOr(str);
}


static bool beginWithSemicolon(char *str)
{
  string s(str);
  if(s[0] == ';')
    return true;
  return false;
}

static bool beginWithAnd(char* str)
{
  string s(str);
  if(s.size() < 2)
    return false;
  if(s[0] == '&' && s[1] == '&')
    return true;
  return false;
}

static bool beginWithOr(char* str)
{
  string s(str);
  if(s.size() < 2)
    return false;
  if(s[0] == '|' && s[1] == '|')
    return true;
  return false;
}

static bool beginWithConnector(char* str)
{
  return beginWithSemicolon(str) || beginWithAnd(str) || beginWithOr(str);
}
*/

static bool isConnector(char* str) // Returns true if the string is a connector 
{
  vector<Connector> v;
  v.push_back(Semicolon());
  v.push_back(DoubleAnd());
  v.push_back(DoubleOr());

  string s(str);
  for(unsigned i = 0; i < v.size(); i++)
  {
    if(s == v[i].getRepresentation())
      return true;
  }
  return false;
}

static Connector recognizeConnector(char *str)
{
  vector<Connector> v;
  v.push_back(Semicolon());
  v.push_back(DoubleAnd());
  v.push_back(DoubleOr());

  string s(str);
  for(unsigned i = 0; i < v.size(); i++)
  {
    if(s == v[i].getRepresentation())
      return v[i];
  }
  return Semicolon();
}


/* Parsing methods */

string Command::stripComments() // Removes comments from the command line typed by the user
{
  string str = command;
  char* search = strchr(const_cast<char*>(str.c_str()), '#'); // Casts a copy of the command attribute; then searches for the integer corresponding to # and return a pointer to the first occurence
  if(search != NULL) // If an occurrence has been found, the pointer is not NULL
  {
    setLine(command.substr(0, command.size()-(strlen(search)))); // Modify the commnand attribute by removing the part of the original command line that starts with # 
  }
  return getLine();
}


void Command::parse() // Determines the executables, arguments, and connectors of the command line
{
  setLine(stripComments()); // Removes comments

  /* We need to get a copy of the command line to tokenize it without modifying it in case we would need it later */
  string str = getLine();
  char* str_conv = const_cast<char*>(str.c_str());
  char* token = strtok(str_conv, " "); // Launches tokenization

  int i = 0; // "Position" in the command; 0 will always be the position of an executable

  /* Tokenization of the command line; the string is divided in substrings that were separated by spaces */
  while(token != NULL)
  {
    cout << "Token: " << token << endl;
    string s(token); // Copy of the token 
   
    if(i == 0) // If i == 0, then it must be an executable
    {
      if(getExecutables().size() > getArguments().size())
        addArguments(Arguments(const_cast<char*>("arg bidon")));


      if(endWithSemicolon(token))// Check if it ends with a semicolon 
      {
        s = s.substr(0, s.size() - 1); // Removes the semicolon
        addExecutable(Executable(const_cast<char*>(s.c_str())));
        addConnector(Semicolon());
	addArguments(Arguments(const_cast<char*>("arg vide ajoute avec exec_semicolon"))); // No arguments for this command, as the executable is immediately followed by a semicolon
	i = 0; // If there is a next token, it will necessarily be an executable
	cout << "Semicolon found" << endl;
      }
      else // The executable doesn't end with a semicolon: either there is an argument or a connector or nothing after
      {
        addExecutable(Executable(const_cast<char*>(s.c_str()))); 
	cout << "Executable added alone" << endl;
        i++; // The next token to be analyzed won't be an executable, so i must be different from 0
      }
    }

    else if(isConnector(token))
    {
      addConnector(recognizeConnector(token));
      i = 0;
    }

    else
    {
      if(getExecutables().size() > getArguments().size())
      {
        if(endWithSemicolon(token))
	{
	  s = s.substr(0, s.size() - 1);
	  addArguments(Arguments(const_cast<char*>(s.c_str())));
	  addConnector(Semicolon());
	  i = 0;
	}
        else
	{
	  addArguments(Arguments(const_cast<char*>(s.c_str())));
	}
      }
      else
      {
        if(endWithSemicolon(token))
	{
	  s = s.substr(0, s.size() - 1);
	  string preArg(getArguments()[getArguments().size() - 1].getArguments());
	  preArg += s;
	  getArguments().pop_back();
	  addArguments(Arguments(const_cast<char*>(preArg.c_str())));
	  addConnector(Semicolon());
	  i = 0;
	}
	else
	{
	  string preArg(getArguments()[getArguments().size() - 1].getArguments());
	  preArg += s;
	  getArguments().pop_back();
	  addArguments(Arguments(const_cast<char*>(preArg.c_str())));
	}
      }
    }


    token = strtok(NULL, " ");
  }

  if(getExecutables().size() > getConnectors().size())
    addConnector(DoubleOr());
  if(getExecutables().size() > getArguments().size())
    addArguments(Arguments(const_cast<char*>("arg vide ajoute a la fin")));


}


int main()
{
  Command cmd("ls -LR #; test commentaires");
  cmd.stripComments();
  cout << cmd.getLine() << endl << endl;
  cmd.parse();
  cout << "TAILLE VECTORS: " << cmd.getExecutables().size() << ", " << cmd.getArguments().size() << ", " << cmd.getConnectors().size() << endl;
  
  for(unsigned i = 0; i < cmd.getExecutables().size(); i++)
  {
    cout << endl << "Affichage executables" << endl; 
    cout << cmd.getExecutables()[i].getExecutable() << endl;
    cout << endl << "Affichage arguments" << endl;
    cout << cmd.getArguments()[i].getArguments() << endl;
    cout << endl << "Affiche connecteurs" << endl;
    cout << cmd.getConnectors()[i].getRepresentation();
    cout << endl << "Fin tour" << endl;
    
  }

/*
  cmd.addConnector(Semicolon());
  cmd.addArguments(Arguments(const_cast<char*>("arg bidon")));
  cmd.getArguments()[0].readArguments();
  cout << cmd.getConnectors()[0].getRepresentation() << endl;  

  */
  return 0;
}
