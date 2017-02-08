#include "../includes/Client.hpp"

//------------------------------------------------------------------------------//
//																				//
//		connection and communication with the server							//
//																				//
//																				//
//------------------------------------------------------------------------------//

ClientCommunication::ClientCommunication(string _hostName, int _port, ClientPlayer *_player, ClientIa *_ia) : hostName(_hostName), port(_port), player(_player), ia(_ia)
{
	isConnected = false;
	isAuthenticated = false;
	sock = -1;
	stateProcessAuth = 0;
}

ClientCommunication::~ClientCommunication()
{}

void	ClientCommunication::initFd()
{
	FD_ZERO(&read_fs);
	FD_ZERO(&write_fs);
	
	// Surveille l'entrée standard pour exit propre quand enter presse
	FD_SET(STDIN_FILENO, &read_fs);

	/*
		TODO: if disconnecting
	*/

	FD_SET(sock, &read_fs);
	if (bufferSend.getLen() > 0)
		FD_SET(sock, &write_fs);
}

void	ClientCommunication::doSelect()
{
	if (select(sock + 1, &read_fs, &write_fs, NULL, NULL) < 0)
		throw (CustomException("select() error"));
}

int		ClientCommunication::checkFd()
{
	if (FD_ISSET(STDIN_FILENO, &read_fs))
	{
		disconnect();
		return (-1);
	}
	if (FD_ISSET(sock, &read_fs))
	{
		if (pullData() == 0)//read from server
		{
			disconnect();
			return (-1);
		}
	}
	if (FD_ISSET(sock, &write_fs))
		pushData();//write to server
	return (1);
}

/*
**	Check if there has been any change on the client player buffer
**	-> has something been received ?
*/

void	ClientCommunication::manageRecv()
{
	if (bufferRecv.getLen() == 0)
		return ;
	if (isAuthenticated)
	{
		string msg = bufferRecv.getFirstMsg();
		if (msg.compare("mort\n") == 0)
		{
			//TODO ;)
		}
		else
		{
			//if commande
				ia->receiveCallbackCommande(msg);
			//if broadcast
				//TODO :)
		}
	}
	else
		clientAuthentication();
}

void	ClientCommunication::clientAuthentication()
{
	if (stateProcessAuth == 0) // BIENVENUE/n
	{
		string msg = bufferRecv.getFirstMsg();
		if (msg.compare("BIENVENUE\n") == 0)
		{
			bufferSend.pushMsg(player->teamName + "\n");
			stateProcessAuth = 1;
		}
	}
	if (stateProcessAuth == 1 && bufferRecv.getLen() > 0) // <slot_number>\n
	{		
		try
		{
			string msg = bufferRecv.getFirstMsg();
			parseTeamSlots(msg, &player->teamSlots);
			stateProcessAuth = 2;
		}
		catch (CustomException &e)
		{
			cout << KRED << e.what() << KRESET << endl;
		}
	}
	if (stateProcessAuth == 2 && bufferRecv.getLen() > 0) // <x_position> <y_position>\n
	{
		try
		{
			string msg = bufferRecv.getFirstMsg();
			parseWorldSize(msg, &player->worldSizeX, &player->worldSizeY);
			player->setBufferSend(&bufferSend);
			cout << KGRN << "client authenticated" << KRESET << endl;
			player->printStat();
			isAuthenticated = true;
			/**/
			ia->findItem();
			/**/
			
		}
		catch (CustomException &e)
		{
			cout << KRED << e.what() << KRESET << endl;
		}
	}
}

void	ClientCommunication::replaceEnd(string &str)
{
	size_t start_pos;
	while ((start_pos = str.find(END)) != string::npos)
	{
		str.replace(start_pos, LEN_END, "*");
	}
}

void	ClientCommunication::connectToServer()
{
	struct hostent		*host;
	
	if (!isConnected)
	{
		//cout << KCYN "- Connection attempt... -" KRESET << endl;
		if ((host = gethostbyname(hostName.c_str())))
			hostName = inet_ntoa(*((struct in_addr **)(host->h_addr_list))[0]);

		proto = getprotobyname("tcp");
		if (!proto)
			throw (CustomException("getprotobyname() error"));
		sock = socket(PF_INET, SOCK_STREAM, proto->p_proto);
		sin.sin_family = AF_INET;
		sin.sin_port = htons(port);
		sin.sin_addr.s_addr = inet_addr(hostName.c_str());
		if (connect(sock, (const struct sockaddr *)&(sin), sizeof(sin)) == -1)
			throw (CustomException("connect() error: cannot connect to hostname"));
		isConnected = true;
	}
	else
		cout << "Client already connected" << endl;
}

/*
** parse : <slot_number>\n<x_position> <y_position>\n
** TODO: check error
*/
void	ClientCommunication::parseTeamSlots(string msg, int *teamSlots)
{
	regex expression("^([0-9]+)\n$");
	smatch matches;

	if(regex_search(msg, matches, expression))
	{
		*teamSlots = strtol(matches[0].str().c_str(), NULL, 10);
	} 
	else
		throw (CustomException("error parse team slots"));
}

void	ClientCommunication::parseWorldSize(string msg, int *worldSizeX, int *worldSizeY)
{
	regex expression("^([0-9]+) ([0-9]+)\n$");
	smatch matches;

	if(regex_search(msg, matches, expression))
	{
		*worldSizeX = strtol(matches[1].str().c_str(), NULL, 10);
		*worldSizeY = strtol(matches[2].str().c_str(), NULL, 10);
	} 
	else
		throw (CustomException("client auth fail"));
}

void	ClientCommunication::disconnect()
{
	if (isConnected)
	{
		isConnected = false;
		close(sock);
		cout << KCYN "Disconnected" KRESET<< endl;
	}
	else
		cout << "Client not connected" << endl;
}

/*
** send data server
*/
void	ClientCommunication::pushData()
{
	string buffTmp;
	int			retSend;

	buffTmp = bufferSend.getBuffer();
	while (1)
	{
		retSend = send(sock, buffTmp.c_str(), bufferSend.getLen(), 0);
		if (retSend == -1 && (errno == EAGAIN || errno == EINTR))
			continue;
		else
			break ;
	}
	replaceEnd(buffTmp = buffTmp.substr(0, retSend));
	cout << KYEL << " Sending to server: "<< KRESET << "["<< buffTmp << "]" << endl;
	if (retSend == -1)
		throw (CustomException("send error"));
	if (retSend > 0)
		bufferSend.erase(retSend);
}

/*
** receive data server
*/
int		ClientCommunication::pullData()
{
	int		retRecv;
	int		sizeRead;
	char	*buffRecv;

	sizeRead = BUFF_SIZE - bufferRecv.getLen();
	if (sizeRead == 0)
	{
		throw (CustomException("buffer full")); //not catched
		return (-1);
	}
	buffRecv = new char[sizeRead];
	while (1)
	{
		retRecv = recv(sock, buffRecv, sizeRead, 0);
		if (retRecv == -1 && (errno == EAGAIN || errno == EINTR))
			continue;
		else
			break ;
	}	
	if (retRecv == -1)
		throw (CustomException("recv error"));
	else
		bufferRecv.pushBuffer(buffRecv, retRecv);
	string recvStr(buffRecv, retRecv);
	replaceEnd(recvStr);
	cout << KBLU << " Receiving to server: "<< KRESET << "["<< recvStr << "] len: " << retRecv << endl;
	delete[] buffRecv;
	return (retRecv);
}

/*
void	ClientCommunication::displayInfos()
{
	cout << KGRN "Connection infos:" KRESET << endl;
	cout << "TeamName: " << Settings.TeamName << endl;
	cout << "Port: " << Settings.Port << endl;
	cout << "HostName: " << Settings.HostName << endl;
}*/
