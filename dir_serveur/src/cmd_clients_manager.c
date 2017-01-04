#include "../includes/serveur.h"

void manage_cmd_clients(t_serveur *serv)
{
	t_client_entity	*p_client;

	p_client = serv->client_hdl.list_clients;
	while (p_client)
	{
		if (p_client->len_buff > 0)
			exec_cmd_client(p_client);
		p_client = p_client->next;
	}
}

void exec_cmd_client(t_client_entity *client)
{
	int		i_loop;
	int		i_buff;
	char	to_process[BUFF_SIZE + 1];

	memset(to_process, 0, BUFF_SIZE);
	i_loop = 0;
	i_buff = client->start_buff;
	while (i_loop < client->len_buff)
	{
		to_process[i_loop] = client->buff[i_buff % BUFF_SIZE];
		i_loop++;
		i_buff++;
	}
	client->start_buff = (i_buff % BUFF_SIZE);
	client->len_buff = 0;
	printf("cmd read : %s\n", to_process);
	printf("*state buffer :\n");
	write(1, client->buff, BUFF_SIZE);
	printf("*\n");
}
