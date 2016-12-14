#include "../includes/serveur.h"

/*
**	Usage: ./serveur -p <port> -x <width> -y <height> -n <team> [<team>] [<team>] ... -c <nb> -t <t>
**
**	-p numero de port
**	-x largeur du Monde
**	-y hauteur du Monde
**	-n nom\_equipe\_1 nom\_\_equipe_2 ...
**	-c nombre de client autorises au commencement du jeu
**	-t diviseur de l'unite de temps (plus t est grand, plus le jeu va vite)
*/
void	get_input(t_serv_settings *serv_settings, int argc, char **argv)
{
	int i;

	i = 0;
	if (argc < 13)
	{
		error_in_args(0, "Invalid number of arguments");
	}
	else
	{
		//serv_settings->  = 0;
		check_input_format(serv_settings, argc, argv);
		fill_input(serv_settings, argc, argv);

		// printing Server settings values.
		printf(KGRN "Server settings:\n" KRESET);
		printf("Port: %d\n", serv_settings->port);
		printf("Map width: %d\n", serv_settings->map_width);
		printf("Map height: %d\n", serv_settings->map_height);
		printf("Nb of teams: %d\n", serv_settings->nb_of_teams);
		printf("Team names:");
		while (i < serv_settings->nb_of_teams)
		{
			printf("  %s", serv_settings->team_names[i]);
			i++;
		}
		printf("\n");
		printf("nb of clients: %d\n", serv_settings->nb_of_clients);
		printf("t: %d\n\n", serv_settings->t);
	}
}

void	check_input_format(t_serv_settings *serv_settings, int argc, char **argv)
{
	int i;

	i = 1;
	// for each arg.
	while (i != argc && i < 8)
	{
		// port -p n
		if (i == 1 && strncmp(argv[i], "-p", 2) != 0) // pos 1
		{
			error_in_args(i, "invalid argument -p");
		}
		if (i == 2 && !regex_match(argv[i], "^[0-9]+$"))
		{
			printf("%s\n", argv[i]);
			error_in_args(i, "invalid port number format");
		}
		// map width -x n
		if (i == 3 && strncmp(argv[i], "-x", 2) != 0)
		{
			error_in_args(i, "invalid map width specifier (-x)");
		}
		if (i == 4 && !regex_match(argv[i], "^[0-9]+$"))
		{
			error_in_args(i, "invalid map width format");
		}
		// map height -y n
		if (i == 5 && strncmp(argv[i], "-y", 2) != 0)
		{
			error_in_args(i, "invalid map height specifier (-y)");
		}
		if (i == 6 && !regex_match(argv[i], "^[0-9]+$"))
		{
			error_in_args(i, "invalid map height format");
		}
		// team names -n teamname [teamname] [teamname] ...
		if (i == 7 && strncmp(argv[i], "-n", 2) != 0)
		{
			error_in_args(i, "invalid team names specifier (-n)");
		}
		i++;
	}

	// Handling multiple possible team names.
	if (i >= 8)
	{
		if (i == 8 && strncmp(argv[i], "-c", 2) == 0)
		{
			error_in_args(i, "at least one team name is required");
		}

		while (argv[i]
			&& strncmp(argv[i], "-c", 2) != 0
			&& strncmp(argv[i], "-t", 2) != 0)
		{
			if (argv[i] && !regex_match(argv[i], "^[a-zA-Z0-9]+"))
			{
				error_in_args(i, "invalid team name format");
			}
			serv_settings->nb_of_teams += 1;
			i++;
		}
		if (!argv[i])
		{
			error_in_args(i, "missing required arguments");
		}
		if (argv[i] && strncmp(argv[i], "-c", 2) == 0)
		{
			if (argv[i + 1] && !regex_match(argv[i + 1], "^[0-9]+$"))
			{
				error_in_args(i + 1, "invalid client number format");
			}
			else if (!argv[i + 1])
			{
				error_in_args(i + 1, "missing required arguments");
			}
		}
		i += 2;
		if (argv[i] && strncmp(argv[i], "-t", 2) == 0)
		{
			if (argv[i + 1] && !regex_match(argv[i + 1], "^[0-9]+$"))
			{
				error_in_args(i + 1, "invalid time value format");
			}
			else if (!argv[i + 1])
			{
				error_in_args(i + 1, "missing required arguments");
			}
		}
	}
}

/*
**	At this point, the values are format OK. it is just about filling
**	and checking their meaning.
*/
void	fill_input(t_serv_settings *serv_settings, int argc, char **argv)
{
	int i;
	int team_i;

	i = 1;
	team_i = 0;
	// for each arg.
	while (i != argc && i < 7)
	{
		// port -p n
		if (i == 2)
		{
			serv_settings->port = strtol(argv[i], NULL, 10);
			if (serv_settings->port < 1023 || serv_settings->port > 65535)
			{
				error_in_args(i, "Invalid port number [1023 <=> 65535]");
			}
		}
		// map width -x n
		if (i == 4)
		{
			serv_settings->map_width = strtol(argv[i], NULL, 10);
		}
		// map height -y n
		if (i == 6)
		{
			serv_settings->map_height = strtol(argv[i], NULL, 10);
		}
		i++;
	}
	i = 8;
	// team names -n teamname [teamname] [teamname] ...
	serv_settings->team_names = (char **)malloc(sizeof(char *) * serv_settings->nb_of_teams);
	if (i == 8)
	{
		while (argv[i]
		&& strncmp(argv[i], "-c", 2) != 0
		&& strncmp(argv[i], "-t", 2) != 0)
		{
			if (strlen(argv[i]) > 40)
				error_in_args(i, "Team Name is too long (40 char max)");
			serv_settings->team_names[team_i] = (char *)malloc(sizeof(char) * strlen(argv[i]) + 1);
			strncpy(serv_settings->team_names[team_i], argv[i], strlen(argv[i]));
			serv_settings->team_names[team_i][strlen(argv[i])] = '\0';
			team_i++;
			i++;
		}

		if (argv[i] && strncmp(argv[i], "-c", 2) == 0)
		{
			serv_settings->nb_of_clients = strtol(argv[i + 1], NULL, 10);
		}
		i += 2;
		if (argv[i] && strncmp(argv[i], "-t", 2) == 0)
		{
			serv_settings->t = strtol(argv[i + 1], NULL, 10);
		}
	}
}

void	error_in_args(int pos, char *str)
{
	printf(KRED "Error: col %d: %s\n" KRESET, pos, str);
	printf(KMAG "Usage: ./serveur -p <port> -x <width> -y <height>");
	printf(" -n <team> [<team>] [<team>] ... -c <nb> -t <t>\n" KRESET);
	exit(-1);
}

int		regex_match(char *string_to_search, char *regex_str)
{
	int				err;
	int				match;
	regex_t			regex;

	err = regcomp(&regex, regex_str, REG_NOSUB | REG_EXTENDED);
	if (err)
	{
		printf("Could not compile regex.\n");
		exit(1);
	}
	else
	{
		match = regexec(&regex, string_to_search, 0, NULL, 0);
		regfree(&regex);
		if (match == 0)
			return (1);
		else if (match == REG_NOMATCH)
			return (0);
		return (-1);
	}
	return (0);
}
