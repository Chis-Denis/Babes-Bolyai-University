#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>
int main(int argc, char **argv) {
  int c;
  struct sockaddr_in server;
  uint16_t a, b, suma;
  
  c = socket(AF_INET, SOCK_STREAM, 0);
  if (c < 0) {
    printf("Eroare la crearea socketului client\n");
    return 1;
  }
  
  if (strlen(argv) != 2){
	printf("Invalid number of parameters!\n");
	close(c);
	return 1;}

  memset(&server, 0, sizeof(server));
  server.sin_port = htons(atoi(argv[2]));
  server.sin_family = AF_INET;
  server.sin_addr.s_addr = inet_addr(argv[1]);
  
  if (connect(c, (struct sockaddr *) &server, sizeof(server)) < 0) {
    printf("Eroare la conectarea la server\n");
    return 1;
  }
  
  printf("a = ");
  scanf("%hu", &a);
  a = htons(a);
  send(c, &a, sizeof(a), 0);
  uint16_t d = 2;
  
  
  //while (d <= a/2){
  //    if (a % d == 0){
  //        recv(c, &d, sizeof(d), 0);
  //        d = ntohs(d);
  //	  printf(" %hu ", d);
  //    }
  //    d = d + 1;
  //}
  int t = 0;
  while (t == 0){
      recv(c, &d, sizeof(d), 0);
      d = ntohs(d);
      if (d == 1){
      t =1;
      }
      else{
      printf(" %hu ", d);}
  }


  close(c);
}

//------------->Client
