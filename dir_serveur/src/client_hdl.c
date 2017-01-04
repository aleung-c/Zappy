#include "../includes/serveur.h"

t_client_entity	*create_client(SOCKET sock)
{
	t_client_entity	*new_client;

	new_client = (t_client_entity *)s_malloc(sizeof(t_client_entity));
	memset((void *)new_client, 0, sizeof(t_client_entity));
	new_client->sock = sock;
	return (new_client);
}

void			add_client(t_serveur *serv, t_client_entity *client)
{
	client->next = serv->client_hdl.list_clients;
	serv->client_hdl.list_clients = client;
}

void			remove_client(t_serveur *serv, t_client_entity *client)
{
	t_client_entity	*current;
	t_client_entity	*prev;

	if (serv->client_hdl.list_clients == client)
	{
		serv->client_hdl.list_clients = client->next;
		free(client);
		return ;
	}
	prev = serv->client_hdl.list_clients;
	current = serv->client_hdl.list_clients->next;
	while (current && current != client)
	{
		prev = current;
		current = current->next;
	}
	if (!current || current != client)
	{
		perror("remove_client() : client not found");
		return ;
	}
	prev->next = client->next;
	free(client);
}
