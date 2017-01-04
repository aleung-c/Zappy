#include "../includes/Client.hpp"

int		main(int argc, char **argv)
{
	ClientController		*Client;

	Client = new ClientController();
	Client->OnInitialize(argc, argv);

	if (Client->Connection.IsConnected)
	{
		Client->OnGameStart();
		Client->MainLoop();
	}

	delete Client;
	return (0);
}
