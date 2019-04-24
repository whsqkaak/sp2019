/*
    whsqkaak@naver.com
    2019/04/24
    my cat program
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#define BUF_SIZE 2048

// cat function
static void do_cat(const char *path){
    int fd, n;
    unsigned char buf[BUF_SIZE];

    fd = open(path, O_RDONLY);
    if(fd<0) die(path);
    for(;;){
        n = read(fd, buf, sizeof(buf));
	if(n<0) die(path);
	if(n==0) break;
	if(write(STDOUT_FILENO, buf, n)<0) die(path);
    }
    if(close(fd) < 0) die(path);
}

// print error "s" and exit(1)
static void die(const char *s){
    perror(s);
    exit(1);
}
