#ifndef CLIENTCONTROLLER_HPP
# define CLIENTCONTROLLER_HPP

# include "Client.hpp"

/*
**	Main instance of the client. Controls every other client class;
*/
class ClientController
{
	public:
		//--------------------------------//
		// Attributes.					  //
		//--------------------------------//
		//bool					IsConnected; // default false;

		//--------------------------------//
		// Methods						  //
		//--------------------------------//
		ClientController(); // Default constructor
		~ClientController(); // Default destructor

		void		OnInitialize(int argc, char **argv); // handle input and connection to serveur

		void		OnGameStart();
		void		MainLoop();

		//--------------------------------//
		// Sub classes references.		  //
		//--------------------------------//
		ClientInputHandler		InputHandler;
		ClientConnection		Connection;	// will handle every server related discussion.
		ClientPlayer			Player;

};

#endif