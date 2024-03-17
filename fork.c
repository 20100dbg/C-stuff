#include <stdio.h>


void pwncollege()
{
}

pid_t pwn(char** argv, char** envp)
{
    pid_t ch_pid = fork();

    if (ch_pid == -1) printf("probleme with fork()\n");
    else if (ch_pid > 0) { return ch_pid; }
    else {
        execve("/challenge/embryoio_level30", argv, envp);

    exit(EXIT_FAILURE);
    }
}


int main(int argc, char** argv, char** envp)
{
    int status;
    pid_t child = pwn(argv, envp);

    if (waitpid(child, &status, WUNTRACED | WCONTINUED) == -1)
    {
        exit(1);
    }
    exit(0);

    return 0;
}
