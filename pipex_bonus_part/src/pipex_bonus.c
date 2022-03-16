/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_bonus.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: olugash <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/13 19:47:57 by olugash           #+#    #+#             */
/*   Updated: 2022/03/13 19:47:59 by olugash          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/pipex_bonus.h"

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
		ft_error(argv);
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

void	ft_pipex(char **argv, char **env, int fds[2], int cmd_index)
{
	int		pipes[2];

	if (dup2(fds[0], STDIN_FILENO) == -1)
		ft_error("File descriptor copying error");
	while (argv[cmd_index + 2])
	{
		if (pipe(pipes) == -1)
			ft_error("Pipe creation error");
		ft_subprocess(argv[cmd_index], env, pipes);
		close(pipes[1]);
		if (dup2(pipes[0], STDIN_FILENO) == -1)
			ft_error("File descriptor copying error");
		cmd_index++;
	}
	ft_subprocess(argv[cmd_index], env, fds);
}

void	ft_here_doc(int fds[2], char **argv, size_t len)
{
	pid_t	pid;
	char	*s;
	int		status;

	pid = fork();
	if (pid == -1)
		ft_error("Process creation error");
	if (!pid)
	{
		close(fds[0]);
		while (write(1, "pipe here_doc> ", 15) != -1)
		{
			s = get_next_line(STDIN_FILENO);
			if (!s)
				exit(write(1, "here_doc delimitted by end-of-file\n", 35) * 0);
			if (ft_strlen(s) - 1 == len && ft_strncmp(argv[2], s, len) == 0)
				exit(EXIT_SUCCESS);
			write(fds[1], s, ft_strlen(s));
		}
	}
	waitpid(pid, &status, 0);
	if (WIFEXITED(status) == 0)
		exit(WEXITSTATUS(status));
}

int	main(int argc, char **argv, char **env)
{
	int	fds[2];
	int	start;

	start = 2;
	if (argc < 5)
		return (write(1, "Incorrect number of arguments!\n", 31) * 0 - 1);
	if (ft_strlen(argv[1]) == 8 && ft_strncmp("here_doc", argv[1], 8) == 0)
	{
		if (pipe(fds) == -1)
			ft_error("Pipe creation error");
		ft_here_doc(fds, argv, ft_strlen(argv[2]));
		close(fds[1]);
		fds[1] = open(argv[argc - 1], O_WRONLY | O_CREAT | O_APPEND);
		if (fds[1] == -1)
			ft_error(argv[argc - 1]);
		start = 3;
	}
	else
		ft_define_fds(fds, argv[1], argv[argc - 1]);
	ft_pipex(argv, env, fds, start);
}
