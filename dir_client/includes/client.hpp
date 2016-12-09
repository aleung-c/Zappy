#ifndef CLIENT_HPP
# define CLIENT_HPP

// General Includes
# include <iostream>
# include <stdio.h>
# include <regex>

// Connection related structure.
typedef struct			s_ConnectionSettings
{
		std::string		TeamName;
		int				Port;
		std::string		HostName;
}						t_ConnectionSettings;

// Game related structure.
typedef struct		s_ClientPlayer
{

	// hp, level ... etc etc.

}					t_ClientPlayer;

// Class definitions.
class ClientInputHandler; // handle and check args.
class ClientConnection; // communicate with the server.
class ClientController;
//class ClientAI;

// Class files includes.
# include "CustomException.hpp"
# include "ClientInputHandler.hpp"
# include "ClientConnection.hpp"
# include "ClientController.hpp"


// color in text;
# define KNRM  "\x1B[0m"
# define KRED  "\x1B[31m"
# define KGRN  "\x1B[32m"
# define KYEL  "\x1B[33m"
# define KBLU  "\x1B[34m"
# define KMAG  "\x1B[35m"
# define KCYN  "\x1B[36m"
# define KWHT  "\x1B[37m"
# define KRESET "\x1B[0m"

#endif