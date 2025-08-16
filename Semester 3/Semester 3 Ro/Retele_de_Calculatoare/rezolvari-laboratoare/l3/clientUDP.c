#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <string.h>
 
int main() {
  int c;
  struct sockaddr_in server;
  uint16_t a, b, suma, i, k;
  char str[100];
  
  c = socket(AF_INET, SOCK_DGRAM, 0);
  if (c < 0) {
    printf("Eroare la crearea socketului client\n");
    return 1;
  }
  
  memset(&server, 0, sizeof(server));
  server.sin_port = htons(1234);
  server.sin_family = AF_INET;
  server.sin_addr.s_addr = inet_addr("127.0.0.1");

  uint16_t subsirLength, startPoz;
  scanf("%s", str);
  printf("Pozitia de inceput a subsirului");


  scanf("%hd", &startPoz);
  printf("Am citit pozitia de inceput a sirului:%d\n",startPoz );

  printf("Lungimea subsirului=");
  scanf("%hd", &subsirLength);

  uint16_t length;
  length = strlen(str);
  length = htons(length);
  sendto(c, &length, sizeof(length), 0, (struct sockaddr *) &server, sizeof(server));
  length = ntohs(length);
  
  sendto(c, str, length + 1, 0, (struct sockaddr *) &server, sizeof(server));

  startPoz = htons(startPoz);
  sendto(c, &startPoz, sizeof(startPoz), 0, (struct sockaddr *) &server, sizeof(server));
  startPoz = ntohs(startPoz);
  printf("Am trimis pozitia de inceput: %d\n",startPoz );

  subsirLength = htons(subsirLength);
  sendto(c, &subsirLength, sizeof(subsirLength), 0, (struct sockaddr *) &server, sizeof(server));
  subsirLength = ntohs(subsirLength);
  printf("Am trimis lungimea subsirului: %d\n",subsirLength );
   char substr[100];
  recvfrom(c,substr, subsirLength+1, MSG_WAITALL, (struct sockaddr *) &server, sizeof(server));
  printf("Am primit sirul:%s-\n", substr);

  close(c);
}
