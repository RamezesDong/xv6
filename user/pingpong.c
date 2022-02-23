#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int
main(int argc, char *argv[])
{
    int pid;
    int pipes[2];
    pipe(pipes);

    if(fork() == 0) {
        char buf[2];
        if(read(pipes[0], buf, 1) != 1)
        {
            fprintf(2, "failed to read in child\n");
            exit(1);
        }
        close(pipes[0]);
        pid = getpid();
        printf( "%d: received ping\n", pid);
        if(write(pipes[1], buf, 1) != 1) {
            fprintf(2, "failed to write in child\n");
            exit(1);
        }
        close(pipes[1]);
        exit(0);
    } else {
        pid = getpid();
        char info[2] = "a";
        char buf[2];
        buf[1] = 0;
        if(write(pipes[1], info, 1) != 1) {
            fprintf(2, "failed to write in parent\n");
            exit(1);
        }
        close(pipes[1]);
        wait(0);
        if(read(pipes[0], buf, 1) != 1)
        {
            fprintf(2, "failed to read in parent\n");
            exit(1);
        }
        printf("%d: received pong\n", pid);
        close(pipes[0]);
        exit(0);
    }
}

