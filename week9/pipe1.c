#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

int main(void) {
    int fd[2][2];
    pid_t pid;
    char buf[257];
    int len, status;

    if (pipe(fd[0]) == -1) {
        perror("pipe 1");
        exit(1);
    }
    if (pipe(fd[1]) == -1){
        perror("pipe 2");
	exit(1);
    }

    switch (pid = fork()) {
        case -1 :
            perror("fork");
            exit(1);
            break;
        case 0 : /* child */
            close(fd[0][1]);
	    close(fd[1][0]);

            write(1, "Pipe1 read:\n", 12);
            len = read(fd[0][0], buf, 256);
            write(1, buf, len);
            close(fd[0][0]);

	    buf[0] = '\0';
	    write(fd[1][1], "Pipe2 write\n", 12);
	    close(fd[1][1]);
            break;
        default :
            close(fd[0][0]);
	    close(fd[1][1]);

	    buf[0] = '\0';
            write(fd[0][1], "Pipe1 write\n", 12);
            close(fd[0][1]);
            
	    write(1, "Pipe2 read:\n", 12);
	    len = read(fd[1][0], buf, 256);
	    write(1, buf, len);
	    close(fd[1][0]);
	    waitpid(pid, &status, 0);
            break;
    }

    return 0;
}
