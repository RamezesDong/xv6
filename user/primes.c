#include "kernel/types.h"
#include "kernel/stat.h"
#include "user.h"


// learn from the pkuflypig
void
new_proc(int pipes[2])
{
    close(pipes[1]);
    int num;
    int n;
    int p[2];
    pipe(p);
    if (read(pipes[0], &num, 4) != 4) {
        fprintf(2, "failed to read in child\n");
        exit(1);
    }
    printf("prime %d\n", num);
    int flag;
    flag = read(pipes[0], &n, 4);
    if(flag) {
        if(fork() == 0) {
            new_proc(p);
        } else {
            close(p[0]);
            if(n%num)
                write(p[1], &n, 4);
            while(read(pipes[0], &n, 4)){
                if(n%num)
                    write(p[1], &n, 4);
            }
            close(pipes[0]);
            close(p[1]);
            wait(0);
        }
    }

}

int
main(int argc, char *argv[])
{
    int pipes[2];
    pipe(pipes);
    if(fork() == 0) {
        new_proc(pipes);
    } else {
        for (int i = 2; i <= 35; ++i) {
            if(write(pipes[1], &i, 4) != 4) {
                fprintf(2, "failed to write in parent\n");
                exit(1);
            }
        }
        close(pipes[1]);
        wait(0);
        exit(0);
    }
    return 0;
}

