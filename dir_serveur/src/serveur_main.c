#include "../includes/serveur.h"

int main(int argc, char **argv)
{
	t_serveur	serv;

	// Init data
	init_data(&serv);

	// Parse arg and fill server data
	get_input(&serv, argc, argv);

	// Init server
	init_serveur(&serv);

	// Boucle du jeux
	main_loop(&serv);

	// Close les connections
	close_all_connections(&serv);

	return (0);
}
