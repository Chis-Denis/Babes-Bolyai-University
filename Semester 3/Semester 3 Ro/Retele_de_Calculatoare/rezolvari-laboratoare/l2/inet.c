#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

int main(){
	long int ip = inet_addr("10.11.17.129");
	printf(ip);
	return 1;


}

