#include "../includes/serveur.h"

void	init_data(t_serveur *serv)
{
	memset(serv, 0, sizeof(t_serveur));
}

void	fill_team_info(t_serveur *serv)
{
	int	i;

	i = -1;
	while (++i < serv->team_hdl.nb_teams)
	{
		serv->team_hdl.array_teams[i].available_slots
			= serv->team_hdl.nb_teams_slots;
	}
}

void	init_serveur(t_serveur *serv)
{
	SOCKET      sock;
	SOCKADDR_IN sin;

	if((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0)
		exit_error("socket()");
	sin.sin_addr.s_addr = htonl(INADDR_ANY);
	sin.sin_port = htons(serv->network.port);
	sin.sin_family = AF_INET;
	if(bind(sock,(SOCKADDR *)&sin, sizeof sin) < 0)
		exit_error("bind()");
	if(listen(sock, serv->network.max_clients) < 0)
		exit_error("listen()");

	// Team info
	fill_team_info(serv);

	// Network info
	serv->network.sock_serveur = sock;
	serv->network.sock_max = sock;
	serv->network.read_fs = (fd_set *)s_malloc((sizeof(fd_set)));

	// Client infos
	serv->client_hdl.nb_clients = 0;
	serv->client_hdl.list_clients = NULL;
}
