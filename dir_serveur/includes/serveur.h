#ifndef SERVEUR_H
# define SERVEUR_H

# include <stdlib.h>
# include <regex.h>
# include <string.h>
# include <errno.h>
# include <unistd.h>
# include <stdio.h>
# include <sys/types.h>
# include <sys/socket.h>
# include <netinet/in.h>
# include <arpa/inet.h>
# include <syslog.h>

/*
**	color in text;
*/
# define KNRM  "\x1B[0m"
# define KRED  "\x1B[31m"
# define KGRN  "\x1B[32m"
# define KYEL  "\x1B[33m"
# define KBLU  "\x1B[34m"
# define KMAG  "\x1B[35m"
# define KCYN  "\x1B[36m"
# define KWHT  "\x1B[37m"
# define KRESET "\x1B[0m"

# define BUFF_SIZE 256
# define MAX_CLIENT 20

typedef int                 SOCKET;
typedef struct sockaddr_in  SOCKADDR_IN;
typedef struct sockaddr     SOCKADDR;
typedef struct in_addr      IN_ADDR;

typedef struct s_list_cmds_entity		t_list_cmds_entity;

/*
** ************************ Network **************************
*/
typedef struct 				s_network_data
{
	int						port;
	SOCKET					sock_serveur;
	SOCKET					sock_max;
	fd_set					*read_fs;
	int						max_clients;
}							t_network_data;

/*
** ************************ Team *****************************
*/
typedef struct 				s_team_entity
{
	char					*name;
	int						available_slots;
}							t_team_entity;

typedef struct 				s_team_hdl
{
	int						nb_teams;
	int						nb_teams_slots;
	t_team_entity			*array_teams;
}							t_team_hdl;

/*
** ************************ Client **************************
*/
typedef struct 				s_client_entity
{
	SOCKET					sock;
	t_team_entity			*team;
	char					buff[BUFF_SIZE];
	int						start_buff;
	int						len_buff;
	int						nb_pending_cmds;
	t_list_cmds_entity		*list_pending_cmds;
	struct s_client_entity	*next;

}							t_client_entity;

typedef struct 				s_client_hdl
{
	int						nb_clients;
	t_client_entity			*list_clients;
}							t_client_hdl;

/*
** ************************ Cmds *****************************
*/
typedef struct 				s_cmd_entity
{
	char					*name;
	int						available_slots;
}							t_cmd_entity;

typedef struct 				s_list_cmds_entity
{
	t_cmd_entity			*cmd;
	t_list_cmds_entity		*next;
}							t_list_cmds_entity;

typedef struct 				s_cmd_hdl
{
	int						nb_cmds;
	t_client_entity			*array_cmds;
}							t_cmd_hdl;

/*
** ************************ World struct ***************************** => Game board
*/

typedef enum e_item_type 
{
	FOOD,
	MINERALS

}			t_item_type;

typedef struct				s_world_item
{
	t_item_type 			type;// type -> nourriture ou mineral
	//  
}							t_world_item;

typedef struct				s_world_case
{
	// une case contient:
	t_client_entity			*players;// players
	t_world_item			*items;// items


}							t_world_case;

typedef struct 				s_world_hdl
{
	int						map_x;
	int						map_y;
	double					t_unit;


	t_world_case			**world_board; // ==> access with	world_board[y_pos][x_pos]

}							t_world_hdl;



/*
** ******************** Serveur Main Struct ******************
*/
typedef struct				s_serveur
{
	t_network_data 			network;
	t_team_hdl				team_hdl;
	t_client_hdl			client_hdl;
	t_cmd_hdl				cmd_hdl;
	t_world_hdl				world_hdl;
}							t_serveur;

/*typedef struct				s_loh_hdl
{

}							t_log_hdl;*/

//***********************OLD TOUT POURRRS**************************************

// recupéré dans input_handling.c
/*typedef struct				s_serv_settings
{
	int						port;
	int						map_width;
	int						map_height;
	int						nb_of_teams;
	char					**team_names;
	int						nb_of_clients;
	int						t;
}							t_serv_settings;

typedef struct				s_client
{
  SOCKET					sock;
  char						team[BUFF_SIZE];
  char						buff[BUFF_SIZE];
  int						start_buff;
  int						len_buff;
}							t_client;

typedef struct				s_server_data
{
  SOCKET					sock_serveur;
  int						last_sock;
  t_client					list_clients[MAX_CLIENT];
  int						nb_clients;
  t_serv_settings			serv_settings;
}							t_server_data;*/

//***********************OLD TOUT POURRRS**************************************

/*
** serveur_data.c
*/
void						init_data(t_serveur *serv);
void						init_serveur(t_serveur *serv);
void						fill_team_info(t_serveur *serv);

/*
** tools.c
*/
void						exit_error(char *error_log);
void						*s_malloc(size_t size);
void						replace_nl(char * str);
void						logs(int type, char *log);
int							get_len_cmd(char *str);
char						*get_cmd_trim(char *str);

/*
** input_handler.c
*/
void						get_input(t_serveur *serv, int argc, char **argv);
void						check_input_format(t_serveur *serv, int argc, char **argv);
void						parse_input(int argc, char **argv);
void						fill_input(t_serveur *serv, int argc, char **argv);
void						error_in_args(int pos, char *str);
int							regex_match(char *string_to_search, char *regex_str);

/*
** serveur_loop.c
*/
void						main_loop(t_serveur *serv);

/*
** connection.c
*/
SOCKET						accept_connection(t_serveur *serv);
void						new_client_connection(t_serveur *serveur);
void						disconnect_client(SOCKET c_sock);
void						close_all_connections(t_serveur *serv);

/*
** communication.c
*/
void						ckeck_all_clients_communication(t_serveur *serv, fd_set *read_fs);
int							read_client(t_client_entity *client);
t_team_entity				*get_team(t_serveur *serv, char *buff);
t_team_entity				*new_client_communication(t_serveur * serv, t_client_entity *client);

/*
** client_hdl.c
*/
t_client_entity				*create_client(SOCKET sock);
void						add_client(t_serveur *serv, t_client_entity *client);
void						remove_client(t_serveur *serv, t_client_entity *client);

/*
** team_hdl.c
*/
t_team_entity				*get_team_by_name(t_serveur *serv, char *name);

/*
** cmd_clients_manager.c
*/
void						manage_cmd_clients(t_serveur *serv);
void						exec_cmd_client(t_client_entity *client);

#endif
