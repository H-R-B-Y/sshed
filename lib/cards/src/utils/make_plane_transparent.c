/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   make_plane_transparent.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hbreeze <hbreeze@student.42london.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/08 16:50:33 by hbreeze           #+#    #+#             */
/*   Updated: 2025/11/08 20:06:10 by hbreeze          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include <notcurses/notcurses.h>

int make_plane_transparent(struct ncplane *plane)
{
	struct nccell c;

	if (!plane)
		return (1);
	nccell_init(&c);
	nccell_set_bg_alpha(&c, NCALPHA_TRANSPARENT);
	// nccell_set_fg_alpha(&c, NCALPHA_TRANSPARENT);
	ncplane_set_base_cell(plane, &c);
	return (0);
}