#ifndef CLIENTCONNECTION_HPP
# define CLIENTCONNECTION_HPP

# include "Client.hpp"

class ClientConnection
{
	public:
		t_ConnectionSettings	Settings;
		bool					IsInMockMode; // allow false connection.
		bool					IsConnected; // default false;
		int						TeamSlots;
		int						Startx;
		int						Starty;
		
		// Circular buffer;
		char			buf[MSG_BUFSIZE];
		char			*char_found;
		int				ret;

		ClientConnection();
		~ClientConnection();

		void			Connect();
		void			ConnectToServer();

		void			DialogStart();

		void			Disconnect();

		// to implement
		void			SendMessage(std::string msg);
		std::string		ReceiveMessage();

		void			DisplayInfos();
		void			SetMockMode(bool b);

	private:
		// Connection variables
		int						sock;
		struct protoent			*proto;
		struct sockaddr_in		sin;
};

#endif