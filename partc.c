#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>

int main(int argc, char* argv[]) {
    if (argc != 2) {
        printf("USAGE: partc FILEIN\n");
        return 1;
    }

    const char *filein = argv[1];

    int pipefd[2];
    if (pipe(pipefd) < 0) {
        perror("pipe");
        return 1;
    }

    pid_t pid = fork();

    if (pid < 0) {
        perror("fork");
        return 1;
    }
/// pipe between parta and the sorting
    if (pid > 0) {
        close(pipefd[0]);

        if (dup2(pipefd[1], STDOUT_FILENO) < 0) {
            perror("dup2");
            return 1;
        }
        close(pipefd[1]);

        char *pargs[] = {
            "./parta",
            (char *)filein,
            NULL
        };
        execv("./parta", pargs);
        perror("execv parta");
        return 1;

    } else {
        close(pipefd[1]); 
        if (dup2(pipefd[0], STDIN_FILENO) < 0) {
            perror("dup2");
            return 1;
        }
        close(pipefd[0]);

        char *sargs[] = {
            "sort",
            "-t,",
            "-k2",
            "-n",
            NULL
        };

        execv("/usr/bin/sort", sargs);
        perror("execv sort");
        return 1;
}

}
