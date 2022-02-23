#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

char buf[1024];
int time;

void
user_sleep(int t)
{
    sleep(t);
}

int
main(int argc, char *argv[])
{
    if(argc != 2) {
        fprintf(2, "usage: sleep [time]\n");
        exit(1);
    }
    time = atoi(argv[1]);
    user_sleep(time);
    exit(0);
}