#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <string.h>

int main() {
  int s;
  struct sockaddr_in server, client;
  int c, l;
  
  s = socket(AF_INET, SOCK_STREAM, 0);
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

  listen(s, 5);
  
  l = sizeof(client);
  memset(&client, 0, sizeof(client));
  
  while (1) {
    uint16_t a, b, suma;
    c = accept(s, (struct sockaddr *) &client, &l);
    printf("S-a conectat un client.\n");
    recv(c, &a, sizeof(a), MSG_WAITALL);

    a = ntohs(a);
 
    uint16_t  d = 2;
    while (d <= a/2){
        if (a % d == 0){
	    printf("S-a transmis catre client divizorul %hu\n", d);
	    d = htons(d);
	    send(c, &d, sizeof(d), 0);
	    d = ntohs(d);
	}
	d = d + 1;
    }
    
    d = 1;
    d = htons(d);
    send(c,&d ,sizeof(d) , 0);
    close(c);
  }
}

