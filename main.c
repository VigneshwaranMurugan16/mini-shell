#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>

#define MAX_LINE 1024
#define MAX_ARGS 64

// function to parse command into args array so we can the array to check and send as single command or multiple command using pipline
void parse_command(char *cmd, char **args) {
    char *token = strtok(cmd, " ");
    int i = 0;
    while (token != NULL && i < MAX_ARGS - 1) {
        args[i++] = token;
        token = strtok(NULL, " ");
    }
    args[i] = NULL;
}


void execute_single_command(char **args) {
    // Output redirection
    for (int j = 0; args[j] != NULL; j++) {
        if (strcmp(args[j], ">") == 0) {
            int fd = open(args[j + 1], O_CREAT | O_WRONLY | O_TRUNC, 0644);
            if (fd < 0) {
                perror("mysh");
                exit(1);
            }
            dup2(fd, STDOUT_FILENO);
            close(fd);
            args[j] = NULL;
            break;
        }
    }

    // input redirection
    for (int j = 0; args[j] != NULL; j++) {
        if (strcmp(args[j], "<") == 0) {
            int fd = open(args[j + 1], O_RDONLY);
            if (fd < 0) {
                perror("mysh");
                exit(1);
            }
            dup2(fd, STDIN_FILENO);
            close(fd);
            args[j] = NULL;
            break;
        }
    }

    execvp(args[0], args);
    perror("mysh");
    exit(EXIT_FAILURE);
}


void execute_pipeline(char *line) {
    char *commands[10]; // total in maximuxm only 10 pip commands
    int num_cmds = 0;

    // Split by pipe strings 
    char *cmd = strtok(line, "|");
    while (cmd != NULL && num_cmds < 10) {
        commands[num_cmds++] = cmd;
        cmd = strtok(NULL, "|");
    }

    int fd[2];
    int in_fd = 0; // Initially input is from STDIN

    for (int i = 0; i < num_cmds; i++) {
        char *args[MAX_ARGS];
        parse_command(commands[i], args);

        pipe(fd);
        pid_t pid = fork();

        if (pid == 0) {
            // CHILD
            dup2(in_fd, STDIN_FILENO); // input from prev
            if (i < num_cmds - 1) {
                dup2(fd[1], STDOUT_FILENO); // output to next
            }
            close(fd[0]);
            close(fd[1]);

            execute_single_command(args);
        } else {
            // PARENT
            wait(NULL);
            close(fd[1]);
            in_fd = fd[0]; // next input is current read end
        }
    }
}

void shell_loop() {
    char line[MAX_LINE];

    while (1) {
        printf("mysh> ");
        fflush(stdout);

        // Read input
        if (!fgets(line, MAX_LINE, stdin)) break;
        line[strcspn(line, "\n")] = 0;

        // Skip empty input
        if (strlen(line) == 0) continue;

        // Built-in: exit
        if (strcmp(line, "exit") == 0) break;

        // Check for 'cd'
        if (strncmp(line, "cd ", 3) == 0) {
            char *path = line + 3;
            if (chdir(path) != 0) {
                perror("mysh");
            }
            continue;
        }

        // Check for pipe
        if (strchr(line, '|') != NULL) {
            execute_pipeline(line);
            continue;
        }

        // Normal (non-pipe) command
        pid_t pid = fork();
        if (pid == 0) {
            char *args[MAX_ARGS];
            parse_command(line, args);
            execute_single_command(args);
        } else if (pid > 0) {
            wait(NULL);
        } else {
            perror("fork");
        }
    }
}

int main() {
    shell_loop();
    return 0;
}
