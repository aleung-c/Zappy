/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_incantation.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aleung-c <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/03/03 17:06:35 by aleung-c          #+#    #+#             */
/*   Updated: 2017/03/03 17:10:30 by aleung-c         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/serveur.h"

int		on_start_cmd_incantation(t_serveur *serv, t_client_entity *client_cur,
									char *param)
{
	if (param)
	{
		printf(KMAG "Bad format to cmd [incantation] "
					"from sock %d\n" KRESET, client_cur->sock);
		return (-1);
	}
	return (init_incantation(serv, client_cur, param));
}

void	on_end_cmd_incantation(t_serveur *serv, t_client_entity *client_cur,
								char *param)
{
	char	*gfx_msg;

	(void)param;
	if (serv->settings_hdl.can_interrupt_incantation == B_TRUE)
	{
		end_incantation_interruptable(serv, client_cur);
	}
	else
	{
		finish_incantation(serv, client_cur, 1);
		asprintf(&gfx_msg, "pie %d %d %d\n",
			client_cur->player.pos.x, client_cur->player.pos.y, 1);
		push_gfx_msg(serv, gfx_msg);
		free(gfx_msg);
	}
}

/*
**	If the incantation is interruptable, we check the ressources AGAIN,
**	and strip the case's ressources at the END.
*/

void	end_incantation_interruptable(t_serveur *serv,
				t_client_entity *client_cur)
{
	int		*target_res;
	char	*gfx_msg;

	target_res = set_incantation_target_res(client_cur->player.level);
	if (are_incantation_cdts_ok(serv, &client_cur->player, target_res))
	{
		finish_incantation(serv, client_cur, 1);
		strip_case_ressources(serv, client_cur, target_res);
		asprintf(&gfx_msg, "pie %d %d %d\n",
			client_cur->player.pos.x, client_cur->player.pos.y, 1);
	}
	else
	{
		finish_incantation(serv, client_cur, 0);
		asprintf(&gfx_msg, "pie %d %d %d\n",
			client_cur->player.pos.x, client_cur->player.pos.y, 0);
	}
}

/*
**	Launched in the incantation on_start(). Will check if the conditions are OK
**	for starting incantation. If yes, every concerned player except the incanter
**	will be set at is_incanting = 1, blocking them until the caster launches
**	the incantation on_end() function.
*/

int		init_incantation(t_serveur *serv, t_client_entity *client_cur,
							char *param)
{
	t_player		*cur_player;
	int				*target_res;

	(void)param;
	cur_player = &client_cur->player;
	target_res = set_incantation_target_res(cur_player->level);
	if (are_incantation_cdts_ok(serv, cur_player, target_res))
	{
		start_incantating(serv, client_cur);
		if (serv->settings_hdl.can_interrupt_incantation == B_FALSE)
			strip_case_ressources(serv, client_cur, target_res);
		free(target_res);
		return (0);
	}
	else
	{
		write_buffer(&client_cur->buff_send, "ko\n", 3);
		free(target_res);
		return (-1);
	}
	free(target_res);
	return (-1);
}

void	start_incantating(t_serveur *serv, t_client_entity *client_cur)
{
	char			*client_msg;
	t_player		*cur_player;

	cur_player = &client_cur->player;
	serv->world_hdl.nb_of_incantations += 1;
	cur_player->incantation_id = serv->world_hdl.nb_of_incantations;
	printf(KGRN "[Server]: Incantation #%d starting ...\n",
			cur_player->incantation_id);
	client_cur->player.is_incanter = 1;
	client_msg = strdup("elevation en cours\n");
	write_buffer(&client_cur->buff_send, client_msg, strlen(client_msg));
	free(client_msg);
	set_players_incanting(serv, client_cur);
}
