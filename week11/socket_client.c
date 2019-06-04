#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

static int
open_connection(char *host, char *port) {
  int sockfd;
  struct addrinfo hints, *res, *ai;
  int err;

  memset(&hints, 0, sizeof(struct addrinfo));
  hints.ai_family = AF_INET;
  hints.ai_socktype = SOCK_STREAM;
  if ((err = getaddrinfo(host, port, &hints, &res)) != 0) {
    fprintf(stderr, "getaddrinfo(3): %s\n", gai_strerror(err));
    exit(1);
  }
  for (ai = res; ai; ai = ai->ai_next) {
    sockfd = socket(ai->ai_family, ai->ai_socktype, ai->ai_protocol);
    if (sockfd < 0) {
      continue;
    }
    if (connect(sockfd, ai->ai_addr, ai->ai_addrlen) < 0) {
      close(sockfd);
      continue;
    }
    /* success */
    freeaddrinfo(res);
    return sockfd;
  }
  fprintf(stderr, "socket(2)/connect(2) failed");
  freeaddrinfo(res);
  exit(1);
}

int main(void) {
  int sockfd;
  FILE *f;
  char buf[256];

  sockfd = open_connection("localhost", "9000");
  f = fdopen(sockfd, "r");
  if (!f) {
    perror("fdopen(3)");
    exit(1);
  }

  fgets(buf, sizeof(buf), f);
  fclose(f);

  close(sockfd);
  printf("From Server: %s\n", buf);

  return 0;
}
