#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <string.h>

 
int main() {
  int s;
  struct sockaddr_in server, client;
  int c, l, i;
  uint16_t old = 0;
  
  s = socket(AF_INET, SOCK_DGRAM, 0);
  if (s < 0) {
    printf("Eroare la crearea socketului server\n");
    return 1;
  }
  
  memset(&server, 0, sizeof(server));
  server.sin_port = htons(1234);
  server.sin_family = AF_INET;
  server.sin_addr.s_addr = INADDR_ANY;
  
  if (bind(s, (struct sockaddr *) &server, sizeof(server)) < 0) {
    printf("Eroare la bind\n");
    return 1;
  }
 
  l = sizeof(client);
  memset(&client, 0, sizeof(client));
 char str[100]; 
  uint16_t length;
  recvfrom(s, &length, sizeof(length), MSG_WAITALL, (struct sockaddr *) &client, &l);
  length = ntohs(length);
  printf("Am primit lungime: %d\n", length);
  
  recvfrom(s, str, length + 1, MSG_WAITALL, (struct sockaddr *) &client, &l);


  printf("Am primit sirul :%s\n", str);

  uint16_t subsirLength,startPoz;
  recvfrom(s, &startPoz, sizeof(startPoz), MSG_WAITALL, (struct sockaddr *) &client, &l);
  startPoz = ntohs(startPoz);
  printf("Am primit pozitia de inceput %d \n", startPoz);
 
  recvfrom(s, &subsirLength, sizeof(subsirLength), MSG_WAITALL, (struct sockaddr *) &client, &l);
  subsirLength = ntohs(subsirLength);
  printf("Am primit lungimea subsirului: %d \n", subsirLength);  
  int k = 0;
  char substr[100];
  for (i=startPoz;i < startPoz + subsirLength; i++){
  	substr[k] = str[i];
  k++;
  }
  sendto(s,substr, subsirLength+1, 0,  (struct sockaddr *) &client, sizeof(client));
  printf("Am trimis subsirul:%s-\n", substr);
  close(s);
}
