#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"

char*
fmtname(char *path)
{
//    static char buf[DIRSIZ+1];
    char *p;

    // Find first character after last slash.
    for(p = path+ strlen(path); p >= path && *p != '/'; p--)
        ;
    p++;

    // Directly return the p is OK.
    return p;
}


void find(char *path, char *file_name) {
    char buf[512], *p;
    int fd;
    struct dirent de;
    struct stat st;

    if((fd = open(path, 0)) < 0) {
        fprintf(2, "find: cannot open %s\n", path);
    }

    if(fstat(fd, &st) < 0){
        fprintf(2, "find: cannot stat %s\n", path);
        close(fd);
        return;
    }

    switch (st.type) {
        case T_FILE:
            fprintf(2, "find: the input path is not a directory\n");
            exit(1);
            break;
        case T_DIR:
            strcpy(buf, path);
            p = buf+ strlen(buf);
            *p = '/';
            p++;
            while(read(fd, &de, sizeof(de)) == sizeof (de)){
                if(de.inum == 0)
                    continue;
                // get the complete file path of the file
                memmove(p, de.name, strlen(de.name));
                p[strlen(de.name)] = 0;
                if(stat(buf, &st) < 0){
                    printf("find: cannot stat %s\n", buf);
                }
                if (st.type == T_DIR) {
                    if ((strcmp(fmtname(buf), ".") == 0) || (strcmp(fmtname(buf), "..") == 0)){
                        continue;
                    } else
                    find(buf, file_name);
                } else {
                    if(strcmp(fmtname(buf), file_name) == 0) {
                        fprintf(1, "%s\n", buf);
                    }
                }
            }
            break;
    }
    close(fd);
}

int
main(int argc, char *argv[])
{
    if (argc != 3) {
        fprintf(2, "usage: find [path] [filename]\n");
        exit(1);
    }
    find(argv[1], argv[2]);
    exit(0);
}