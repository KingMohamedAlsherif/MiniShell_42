/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_bonus.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chon <chon@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/08 09:17:25 by chon              #+#    #+#             */
/*   Updated: 2024/07/08 09:17:25 by chon             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execute.h"

void	execute(t_var *p, int cmd_idx, int fd_in, int fd_out)
{
	if (dup2(fd_in, STDIN_FILENO) < 0)
		ft_error(errno, ft_strdup("dup"), p, 1);
	if (dup2(fd_out, STDOUT_FILENO) < 0)
		ft_error(errno, ft_strdup("dup"), p, 1);
	close_fds(p);
	if (execve(p->exec_cmd_path[cmd_idx], p->cmd_args[cmd_idx], p->env) < 0)
		ft_error(errno, ft_strdup("execve"), p, 1);
}

void	multiple_cmds(t_var *p, int valid_cmds_start)
{
	p->i = -1;
	p->j = valid_cmds_start;
	p->pipe_ct = p->cmd_ct - valid_cmds_start - 1;
	setup_p_cp_arr(p);
	while (++valid_cmds_start != p->cmd_ct)
		if (pipe(p->fd[++p->i]) < 0)
			ft_error(errno, ft_strdup("pipe"), p, 1);
	p->i = -1;
	while (++p->orig_v_c_s != p->cmd_ct)
	{
		p->pid[++p->i] = fork();
		if (p->pid[p->i] < 0)
			ft_error(errno, ft_strdup("fork"), p, 1);
		if (!p->pid[p->i])
		{
			if (!p->i)
				execute(p, p->orig_v_c_s, p->infile, p->fd[p->i][1]);
			else if (p->orig_v_c_s < p->cmd_ct - 1)
				execute(p, p->orig_v_c_s, p->fd[p->i - 1][0], p->fd[p->i][1]);
			else
				execute(p, p->orig_v_c_s, p->fd[p->i - 1][0], p->outfile);
		}
		waitpid(p->pid[p->i], NULL, WNOHANG);
	}
	close_fds(p);
}

void	pipex(t_var *p, char *infile)
{
	int	valid_cmds_start;

	valid_cmds_start = p->cmd_ct;
	p->i = 0;
	while (valid_cmds_start > 0
		&& ft_strncmp(p->exec_cmd_path[valid_cmds_start - 1], "invalid", 7))
		valid_cmds_start--;
	if (!valid_cmds_start && !p->hd_shift && access(infile, R_OK) < 0)
		valid_cmds_start++;
	if (valid_cmds_start != p->cmd_ct)
	{
		p->orig_v_c_s = valid_cmds_start - 1;
		if (p->infile < 0 || valid_cmds_start)
			p->infile = p->empty_fd;
		if (valid_cmds_start == p->cmd_ct - 1)
			execute(p, valid_cmds_start, p->infile, p->outfile);
		else
			multiple_cmds(p, valid_cmds_start);
	}
}
