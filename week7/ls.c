#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <time.h>

static void do_ls(char *path);

int main(int argc, char *argv[])
{
    int i;

    if (argc < 2) {
        fprintf(stderr, "%s: no arguments\n", argv[0]);
        exit(1);
    }
    for (i = 1; i < argc; i++) {
        do_ls(argv[i]);
    }
    exit(0);
}

static void do_ls(char *path)
{
    DIR *d;
    struct dirent *ent;

    d = opendir(path);          /* (1) */
    if (!d) {
        perror(path);
        exit(1);
    }

    struct stat st;
    char *mtime;
    int muid;
    while (ent = readdir(d)) {  /* (2) */
        if(lstat(ent->d_name, &st) < 0) exit(1);
        mtime = ctime(&st.st_mtime);
	muid = st.st_uid;
        printf("time : %s uid : %d name : %s\n\n",mtime, muid, ent->d_name);
    }
    closedir(d);                /* (1') */
}
