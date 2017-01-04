#include "../includes/Client.hpp"

//------------------------------------------------------------------------------//
//																				//
//		This class takes care of connection and communication with the			//
//		server.																	//
//																				//
//------------------------------------------------------------------------------//

ClientConnection::ClientConnection()
{
	Settings.TeamName = "unset";
	Settings.Port = -1;
	Settings.HostName = "unset";
	this->IsInMockMode = false;
	this->IsConnected = false;
}

ClientConnection::~ClientConnection()
{


}

void	ClientConnection::Connect()
{
	std::cout << KCYN "- Connection attempt... -" KRESET << std::endl;

	if (!IsConnected)
	{
		if (IsInMockMode)
		{
			std::cout << KMAG "Engaging MOCK CONNECTION..." KRESET << std::endl;
			this->IsConnected = true;
		}
		else
		{
			std::cout << KCYN "Engaging REAL CONNECTION..." KRESET << std::endl;
			try
			{
				ConnectToServer();
				this->IsConnected = true;
			}
			catch (CustomException &e)
			{
				std::cout << KRED << e.what() << KRESET << std::endl;
			}
		}
	}
	else
	{
		std::cout << "Client already connected" << std::endl;
	}
}

void	ClientConnection::ConnectToServer()
{
	if (Settings.HostName.compare("localhost") == 0)
	{
		Settings.HostName = "127.0.0.1";
	}
	this->proto = getprotobyname("tcp");
	if (!this->proto)
		throw (CustomException("getprotobyname() error"));
	this->sock = socket(PF_INET, SOCK_STREAM, proto->p_proto);
	this->sin.sin_family = AF_INET;
	this->sin.sin_port = htons(Settings.Port);
	this->sin.sin_addr.s_addr = inet_addr(Settings.HostName.c_str());
	if (connect(this->sock, (const struct sockaddr *)&(this->sin), sizeof(this->sin)) == -1)
	{
		throw (CustomException("connect() error: cannot connect to hostname"));
		exit (-1);
	}
	std::cout << KGRN "SUCCESS - Connected to server." KRESET << std::endl;
	std::cout << "Port: " << Settings.Port << std::endl;
	std::cout << "Client socket: " << this->sock << std::endl << std::endl;
	// If connection successful, engage first dialog.
	DialogStart();
}

/*
**	Here is the beginning of the connection server <-> client.
*/
void	ClientConnection::DialogStart()
{
	struct timeval		timeout;
	fd_set				readSet;
	int					select_ret;

	// First batch of data reception
	std::cout << KCYN "- Awaiting server response... -" KRESET << std::endl;
	timeout.tv_sec = 3; // seconds.
	timeout.tv_usec = 0; // milliseconds.
	FD_ZERO(&readSet);
	FD_SET(this->sock, &readSet);
	select_ret = select(this->sock + 1, &readSet, NULL, NULL, &timeout);
	if (select_ret == -1)
	{
		perror("Select");
		throw (CustomException("Select error"));
	}
	if (FD_ISSET(this->sock, &readSet))
	{
		std::string		receivedString = ReceiveMessage();
		std::cout << "Received: " << receivedString << std::endl;
		// Is it the right server ?
		if (receivedString.compare("BIENVENUE\n") == 0)
		{
			std::cout << "Sending team name: " << Settings.TeamName << std::endl;
			SendMessage(Settings.TeamName + "\n");

			// Receiving first datas.
			receivedString = ReceiveMessage();
			std::cout << "Received: " << receivedString << std::endl;

			// Recuperating : <slot_number>\n<x_position> <y_position>\n
			char	*splittedString;
			// getting slot number.
			if (!(splittedString = strtok((char *)receivedString.c_str(), " \n")))
			{
				throw (CustomException("Error while getting slot number"));
			}
			this->TeamSlots = strtol(splittedString, NULL, 10);
			// getting X start position.
			if (!(splittedString = strtok(NULL, " \n")))
			{
				throw (CustomException("Error while getting x starting position"));
			}
			this->Startx = strtol(splittedString, NULL, 10);
			// getting Y start position.
			if (!(splittedString = strtok(NULL, " \n")))
			{
				throw (CustomException("Error while getting y starting position"));
			}
			this->Starty = strtol(splittedString, NULL, 10);
			std::cout << "Nb of slots received: " << this->TeamSlots << std::endl;
			if (this->TeamSlots <= 0)
			{
				throw (CustomException("No slot available in team or team does not exist."));
			}
		}
		else
		{
			throw (CustomException("Wrong starting message received (incorrect server?)"));
		}
	}
	else
	{
		throw (CustomException("Connection timed out (the server did not answer). Closing client..."));
	}
	std::cout << KGRN "First dialog with server successful: Zappy server recognized" KRESET << std::endl;
}

void	ClientConnection::Disconnect()
{
	std::cout << KCYN "Disconnection" KRESET<< std::endl;
	if (IsConnected)
	{
		IsConnected = false;
	}
	else
	{
		std::cout << "Client is not connected" << std::endl;
	}
}

void	ClientConnection::SendMessage(std::string msg)
{
	int						send_ret;

	send_ret = send(this->sock, msg.c_str(), msg.size() + 1, MSG_OOB); // +1 for \n
	// OR
	// send_ret = send(this->sock, msg, MSG_BUFIZE, MSG_OOB);
	if (send_ret == -1)
	{
		perror("send");
		throw (CustomException("send() error"));
	}
}

std::string		ClientConnection::ReceiveMessage()
{
	std::string			ret_string;
	int					recv_ret;

	recv_ret = recv(this->sock, buf, MSG_BUFSIZE - 1, 0);
	if (recv_ret == 0)
	{
		throw (CustomException("Disconnected from server"));
	}
	else if (recv_ret == -1)
	{
		perror("recv");
		throw (CustomException("recv() error"));
	}
	buf[recv_ret] = '\0';
	ret_string = buf;
	return (ret_string);
}

void	ClientConnection::DisplayInfos()
{
	std::cout << KGRN "Connection infos:" KRESET << std::endl;
	std::cout << "TeamName: " << this->Settings.TeamName << std::endl;
	std::cout << "Port: " << this->Settings.Port << std::endl;
	std::cout << "HostName: " << this->Settings.HostName << std::endl;
}

void	ClientConnection::SetMockMode(bool b)
{
	this->IsInMockMode = b;
}
