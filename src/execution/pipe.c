//
// Created by clesucre on 7/7/24.
//

// X_OK means executable

/**
 * Execute cmds in the shell and redirect
 * the output of each command to the
 * input of the next one using pipes given in parameter
 *
 * @param fd[2]		pipe files descriptors to communicate
 * 	input & output between each process
 * @param input		file descriptor from the input file
 * @param output	file descriptor from the output file
 * @param cmds		commands to execute,
 * 		cmds[0] ---> intput file
 * 		cmds[1] ---> first command to execute
 * 		cmds[2] ---> second command to execute
 * 		...
 * 		cmds[n] ---> output file
 * @param envp		environment variables
 */

static void	ft_execute_shell(int io[2], char **cmds, char **envp)
{
    int		i;
    int		fd[2];
    char	**cmd;
    size_t	cmds_len;

    cmds_len = ft_strlentab((const char **)cmds);
    i = 1;
    while (i < cmds_len - 2)
    {
        pipe(fd);
        cmd = ft_split(cmds[i], " ");
        io[1] = fd[1];
        ft_exec_cmd(io, cmd, envp, fd[0]);
        close(fd[1]);
        close(io[0]);
        close(io[0]);
        io[0] = fd[0];
        i++;
    }
    io[1] = open(cmds[i + 1], O_WRONLY | O_CREAT | O_TRUNC, 0644);
    cmd = ft_split(cmds[i], " ");
    ft_printf("cmds_len: %d\n", cmds_len);
    if (cmds_len > 3)
        ft_exec_cmd(io, cmd, envp, fd[1]);
    else
        ft_exec_cmd(io, cmd, envp, -1);
    close(io[0]);
    close(io[1]);
    if (cmds_len < 3)
    {
        close(fd[0]);
        close(fd[1]);
    }
}
