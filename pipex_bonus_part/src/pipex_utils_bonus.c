/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_utils_bonus.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: olugash <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/13 19:48:09 by olugash           #+#    #+#             */
/*   Updated: 2022/03/13 19:48:11 by olugash          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/pipex_bonus.h"

void	ft_error(char *str)
{
	perror(str);
	exit (errno);
}

static char	*ft_find_path(char *temp, char **bin_paths)
{
	char	*res_path;
	int		i;

	res_path = NULL;
	i = 0;
	while (bin_paths[i])
	{
		res_path = ft_strjoin(bin_paths[i], temp);
		if (res_path == NULL)
			break ;
		if (!access(res_path, 1))
			break ;
		else
			free(res_path);
		i++;
	}
	if (!bin_paths[i])
		perror(temp + 1);
	return (res_path);
}

char	*ft_define_paths(char *cmd, char **env)
{
	char	**bin_paths;
	char	*res_path;
	char	*temp;

	res_path = NULL;
	while (*env && ft_strncmp(*env, "PATH=", 5))
		env++;
	if (!*env)
		return (NULL);
	bin_paths = ft_split(*env + 5, ':');
	temp = ft_strjoin("/", cmd);
	if (temp && bin_paths)
		res_path = ft_find_path(temp, bin_paths);
	if (temp)
		free(temp);
	if (bin_paths)
		ft_clr_2d_arr((void *)&bin_paths);
	return (res_path);
}

void	ft_define_fds(int fds[2], char *infile, char *outfile)
{
	fds[0] = open(infile, O_RDONLY | EINVAL);
	if (fds[0] == -1)
		ft_error(infile);
	fds[1] = open(outfile, O_WRONLY | O_TRUNC | O_CREAT, S_IWRITE | S_IREAD);
	if (fds[1] == -1)
		ft_error(outfile);
}

void	ft_clr_2d_arr(void ***arr)
{
	int	i;

	if (!*arr)
		return ;
	i = -1;
	while (*arr[++i])
		free(*arr[i]);
	free(*arr);
}
