/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: olugash <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/13 19:44:52 by olugash           #+#    #+#             */
/*   Updated: 2022/03/13 19:44:53 by olugash          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/pipex.h"

static void	ft_execute_command(char *argv, char **env)
{
	char	**cmd;
	char	*bin;

	cmd = ft_split(argv, ' ');
	if (!cmd)
		exit(errno);
	if (!access(cmd[0], 1))
		bin = cmd[0];
	else
		bin = ft_define_paths(cmd[0], env);
	if (!bin)
	{
		ft_clr_2d_arr((void *)&cmd);
		exit(errno);
	}
	if (execve(bin, cmd, env) == -1)
		ft_error(bin);
}

static	void	ft_subprocess(char *cmd, char **env, int fds[2])
{
	pid_t	pid;
	int		status;

	pid = fork();
	if (pid == -1)
		ft_error("Process creation error");
	if (pid == 0)
	{
		close(fds[0]);
		if (dup2(fds[1], STDOUT_FILENO) == -1)
			ft_error("File descriptor copying error");
		ft_execute_command(cmd, env);
	}
	if (ft_strncmp(cmd, "yes", 3) != 0)
	{
		waitpid(pid, &status, 0);
		if (WIFEXITED(status) == 0)
			exit(WEXITSTATUS(status));
	}
}

void	ft_pipex(char *cmd, char **env)
{
	int		pipes[2];

	if (pipe(pipes) == -1)
		ft_error("File descriptor copying error");
	ft_subprocess(cmd, env, pipes);
	close(pipes[1]);
	if (dup2(pipes[0], STDIN_FILENO) == -1)
		ft_error("File descriptor copying error");
}

int	main(int argc, char **argv, char **env)
{
	int	fds[2];

	if (argc != 5)
		return (write(1, "Incorrect number of arguments!\n", 31) * 0 - 1);
	ft_define_fds(fds, argv[1], argv[argc - 1]);
	if (dup2(fds[0], STDIN_FILENO) == -1)
		ft_error("File descriptor copying error");
	ft_pipex(argv[2], env);
	ft_subprocess(argv[3], env, fds);
}
