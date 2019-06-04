#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <arpa/inet.h>
#include <netdb.h>

static int
listen_socket(char *port)
{
    struct addrinfo hints, *res, *ai;
    int err;

    memset(&hints, 0, sizeof(struct addrinfo));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;
    if ((err = getaddrinfo(NULL, port, &hints, &res)) != 0) {
      perror("getaddrinfo");
      exit(1);
    }
    for (ai = res; ai; ai = ai->ai_next) {
      int sockfd;

      sockfd = socket(ai->ai_family, ai->ai_socktype, ai->ai_protocol);
      if (sockfd < 0) continue;
      if (bind(sockfd, ai->ai_addr, ai->ai_addrlen) < 0) {
        close(sockfd);
        continue;
      }
      if (listen(sockfd, 5) < 0) {
        close(sockfd);
        continue;
      }
      freeaddrinfo(res);
      return sockfd;
    }
    perror("failed to listen socket");
    exit(1);
}

int main(void) {
  struct sockaddr_storage addr;
  socklen_t addrlen = sizeof(addr);
  int server_fd, sock;
  struct sockaddr_in *sin;
  FILE *outf;

  server_fd = listen_socket("9000");

  if ((sock = accept(server_fd, (struct sockaddr *)&addr, &addrlen)) < 0) {
    perror("accept");
    exit(1);
  }

  outf = fdopen(sock, "w");
  sin = (struct sockaddr_in *)&addr;
  fprintf(outf, "Your IP addr is %s", inet_ntoa(sin->sin_addr));
  fflush(outf);

  fclose(outf);
  close(sock);
  close(server_fd);

  return 0;
}

