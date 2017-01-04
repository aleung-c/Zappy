#include "../includes/serveur.h"

void ckeck_all_clients_communication(t_serveur *serv, fd_set *read_fs)
{
	t_client_entity	*p_client;
	int ret_read;

	//printf("start_checked");
	p_client = serv->client_hdl.list_clients;
	while (p_client)
	{
		if (FD_ISSET(p_client->sock, read_fs)
			&& !(ret_read = read_client(p_client)))
		{
			disconnect_client(p_client->sock);
			remove_client(serv, p_client);
			return ;
		}
		p_client = p_client->next;
	}
}

int   read_client(t_client_entity*client)
{
	int   ret;
	int   i_loop;
	int   i_buff;
	char  buffer[BUFF_SIZE + 1];

	ret = 0;
	memset(buffer, 0, BUFF_SIZE + 1);
	if ((ret = recv(client->sock, buffer, BUFF_SIZE, 0)) < 0)
	{
		perror("recv()");
		ret = 0;
	}
	else if (!ret)
		return (0);

	buffer[ret] = '\0';
	printf("Recu : %s\n", buffer);

	i_loop = 0;
	i_buff = client->start_buff;
	while (i_loop < ret)
	{
		client->buff[i_buff % BUFF_SIZE] = buffer[i_loop];
		i_loop++;
		i_buff++;
	}
	client->len_buff += ret;
	if (client->len_buff > BUFF_SIZE)
		client->len_buff = BUFF_SIZE;

	return(ret);
}

t_team_entity	*get_team(t_serveur *serv, char *buff)
{
	char			*t_name;
	t_team_entity	*team;

	if (!(t_name = get_cmd_trim(buff)))
	{
		return (NULL);
	}

	team = get_team_by_name(serv, t_name);
	free(t_name);
	return (team);
}

t_team_entity	*new_client_communication(t_serveur *serv, t_client_entity *client)
{
	char			*buff;
	char			*to_print;
	int				ret;
	t_team_entity	*team;

	buff = (char *)s_malloc(sizeof(char) * BUFF_SIZE);

	// Send Bienvenue
	if (send(client->sock, "BIENVENUE\n", strlen("BIENVENUE\n"), 0) < 0)
		exit_error("send()");
	printf("Send : BIENVENUE*\n");

	// Recv an verify Team Name
	if ((ret = recv(client->sock, buff, BUFF_SIZE - 1, 0)) < 0)
		exit_error("recv()");
	buff[ret] = '\0';					//
	to_print = strdup(buff);			//	FOR DEBUG PRINT
	replace_nl(to_print);				//
	printf("Recv : %s\n", to_print);	//
	if (!(team = get_team(serv, buff)))
	{
		printf("Get_team() failed\n");
		return (NULL);
		free(buff);
	}

	// Format Send buff ans send
	memset((void *)buff, 0, BUFF_SIZE);
	sprintf(buff, "%d\n", team->available_slots);
	if (send(client->sock, buff, strlen(buff), 0) < 0)
		exit_error("send()");
	printf("Send : %d*\n", team->available_slots);

	memset((void *)buff, 0, BUFF_SIZE);
	sprintf(buff, "%d %d\n", serv->world_hdl.map_x, serv->world_hdl.map_y);
	if (send(client->sock, buff, strlen(buff), 0) < 0)
		exit_error("send()");
	printf("Send : %d %d*\n", serv->world_hdl.map_x, serv->world_hdl.map_y);

	free(buff);
	return (team);
}
