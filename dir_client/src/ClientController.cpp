#include "../includes/Client.hpp"

// Default Constructor
ClientController::ClientController()
{
	std::cout << KCYN "Client Controller starting..." KRESET << std::endl;
}

// Default destructor
ClientController::~ClientController()
{


}

// First method called from the program.
void	ClientController::OnInitialize(int argc, char **argv)
{
	//------------------------------------------------------//
	// I. get input from prog exec 							//
	//	-> goes into Connection.Settings struct;			//
	//------------------------------------------------------//
	try
	{
		// ClientInputHandler.cpp
		InputHandler.GetInput(this->Connection, argc, argv);
	}
	catch (CustomException &e)
	{
		std::cout << KRED << e.what() << KRESET << std::endl;
		// print usage.
		std::cout << std::endl << KMAG << "Usage: ./client -n <team> -p <port> [-h <hostname>]"
					<< KRESET << std::endl;
		return ;
	}

	//------------------------------------------------------//
	// II. try to connect to serveur						//
	//------------------------------------------------------//
	try
	{
		// ClientConnection.cpp
		Connection.DisplayInfos();
		Connection.SetMockMode(false);
		Connection.Connect(); // also handles first data exchange.(FirstDialog)
	}
	catch (CustomException &e)
	{
		std::cout << KRED << e.what() << KRESET << std::endl;
		return ;
	}
}

// Called just before the main loop: the client is connected to the server.
void	ClientController::OnGameStart()
{
	std::cout << KCYN "- OnGameStart called -" KRESET << std::endl;

	// fill the player position with the received string during connection.
	Player.x = Connection.Startx;
	Player.y = Connection.Starty;

	std::cout << KGRN "Player starting datas:" KRESET << std::endl;
	std::cout << "World size: " << Player.x << "x " << Player.y << "y " << std::endl;
}

void	ClientController::MainLoop()
{
	bool	running = true;

	// Main loop
	std::cout << KCYN "- Main loop starting -" KRESET << std::endl;
	while (running)
	{

	}
}
