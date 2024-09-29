// #include "../minishell.h"

// void env(char *exec_cmd_path, char *arg, t_lst *env)
// {
//     if (ft_strncmp(exec_cmd_path, "PATH", 5) && !arg)
//     {
//         printf("env cmd path: %s\n", exec_cmd_path);
//         while (env)
//         {
//             if (ft_strchr(env->var_n_val, '=') && !ft_strchr(env->var, '?'))
//                 printf("%s", env->var_n_val);
//             env = env->fwd;
//         }
//     }
//     else if (ft_strncmp(exec_cmd_path, "PATH", 5))
//         printf("env: '%s': No such file or directory\n", arg);
//     else
//         printf("Minishell: env: No such file or directory\n");
// }

// void execute_builtin(t_tree_node *n, char *cmd, bool exit_flag)
// {
//     if (ft_strlen(cmd) == 2 && !ft_strncmp(cmd, "cd", 3) && n->cmd_args_arr[1])
//         cd(n);
//     else if (ft_strlen(cmd) == 3 && !ft_strncmp(cmd, "pwd", 4))
//         pwd(n);
//     else if (ft_strlen(cmd) == 3 && !ft_strncmp(cmd, "env", 4))
//         env(n->exec_cmd_path, n->cmd_args_arr[1], n->ms->env);
//     else if (ft_strlen(cmd) == 5 && !ft_strncmp(cmd, "unset", 6))
//         unset(n);
//     if (ft_strlen(cmd) == 6 && !ft_strncmp(cmd, "export", 7))
//         export(n);
//     ft_error(0, ft_strdup(n->cmd_args_arr[0]), n, exit_flag);
// }

// bool is_builtin(char *cmd)
// {
//     if ((ft_strlen(cmd) == 2 && (!ft_strncmp(cmd, "cd", 3) || !ft_strncmp(cmd,
//                                                                           "$?", 3))) ||
//         (ft_strlen(cmd) == 3 && !ft_strncmp(cmd,
//                                             "pwd", 4)) ||
//         (ft_strlen(cmd) == 3 && !ft_strncmp(cmd, "env",
//                                             4)) ||
//         (ft_strlen(cmd) == 5 && !ft_strncmp(cmd, "unset", 6)) || (ft_strlen(cmd) == 6 && !ft_strncmp(cmd, "export", 7)))
//         return (1);
//     return (0);
// }
