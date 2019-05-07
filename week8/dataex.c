#include <sys/types.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

int main(int argc, char *argv[]) {
    int fd;
    pid_t pid;
    caddr_t addr;
    struct stat statbuf;

    if (argc != 2) {
        fprintf(stderr, "Usage : %s filename\n", argv[0]);
        exit(1);
    }

    if (stat(argv[1], &statbuf) == -1) {
        perror("stat");
        exit(1);
    }

    if ((fd = open(argv[1], O_RDWR)) == -1) {
        perror("open");
        exit(1);
    }

    //mmap을 이용해 메모리 맵핑 실시
    addr = mmap(NULL, statbuf.st_size, PROT_READ|PROT_WRITE, 
                MAP_SHARED, fd, (off_t)0);
    
    if(addr == MAP_FAILED){
        perror("mmap");
	exit(1);
    }
    close(fd);

    switch (pid = fork()) {
        case -1 :  /* fork failed */
            perror("fork");
            exit(1);
            break;
        case 0 :   /* child process */
	    // parent process에서 변경된 데이터 확인
	    printf("child process : %s", addr);
	    break;
        default :   /* parent process */
            // data 변경
	    printf("parent process : %s", addr);
	    addr[0] = '!';
	    msync(addr, statbuf.st_size, MS_SYNC);
            break;
    }

    return 0;
}
