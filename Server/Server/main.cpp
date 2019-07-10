#include "stdafx.h"
#include <WinSock2.h>
#include <stdio.h>
#include <string.h>

#include "ThreadRoom.h"
#include "Structure.h"
#include "ThreadServer.h"
#include "RoomList.h"

#define MAX_BUF 100
#define MAX_CLIENT 10
#define TOTAL_CLIENT 90
#define Nick_length 10
#define MAX_ROOM 10
#define INVALID -1

SOCKET sock[TOTAL_CLIENT];
int room_count[MAX_CLIENT] = { 0 };
char list[MAX_CLIENT + 2][MAX_BUF] = { 0 };
int equal_number[MAX_CLIENT][MAX_CLIENT] =
{ -10, -10, -10, -10, -10, -10, -10, -10, -10, -10,
-10, -10, -10, -10, -10, -10, -10, -10, -10, -10,
-10, -10, -10, -10, -10, -10, -10, -10, -10, -10,
-10, -10, -10, -10, -10, -10, -10, -10, -10, -10,
-10, -10, -10, -10, -10, -10, -10, -10, -10, -10,
-10, -10, -10, -10, -10, -10, -10, -10, -10, -10,
-10, -10, -10, -10, -10, -10, -10, -10, -10, -10,
-10, -10, -10, -10, -10, -10, -10, -10, -10, -10,
-10, -10, -10, -10, -10, -10, -10, -10, -10, -10,
-10, -10, -10, -10, -10, -10, -10, -10, -10, -10,
};
int typing_errorflag = 0;
int Nickflag = 0;
int quit_flag = -1;
int createFlag = 0;
int create_number = 0;
int thread_num = 0;
char NickBuffer[TOTAL_CLIENT][TOTAL_CLIENT] = { 0 };
int NickOverlapFlag = 0;
char Nickname[Nick_length][TOTAL_CLIENT] = { 0 };
char NickTemp[Nick_length][TOTAL_CLIENT] = { 0 };
int create_flag[MAX_ROOM] = { -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 };
char print_client[MAX_BUF][MAX_BUF] = { 0 };
DWORD nExitCode = NULL;

int main(int argc, char *argv[])
{
	WSADATA wsa;
	CRITICAL_SECTION crit;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
	{
		printf("Too high Winsock version\n");
		return -1;
	}

	SOCKET s = socket(AF_INET, SOCK_STREAM, 0);
	if (s == INVALID_SOCKET) {
		printf("Error in socket():%ld\n", WSAGetLastError());
		WSACleanup();
		return -1;
	}


	SOCKADDR_IN serv;
	ZeroMemory(&serv, sizeof(serv));
	serv.sin_family = AF_INET;
	serv.sin_port = htons(50000);
	serv.sin_addr.s_addr = inet_addr("165.229.17.105");

	if (bind(s, (SOCKADDR*)&serv, sizeof(serv)) == SOCKET_ERROR)
	{
		printf("Error in bind():%ld\n", WSAGetLastError());
		closesocket(s);
		WSACleanup();
		return -1;
	}
	listen(s, 5);

	RoomList_Copy();

	for (int i = 0; i < TOTAL_CLIENT; i++)
	{
		sock[i] = INVALID;
	}
	InitializeCriticalSection(&crit);
	while (1)
	{
		SOCKET t = accept(s, NULL, NULL);
		if (t == INVALID_SOCKET) {
			printf("Error in socket():%ld\n", WSAGetLastError());
			WSACleanup();
			return -1;
		}

		user *pUser = new user;
		*((SOCKET*)pUser->pParam) = t;
		pUser->room_number = 0;
		pUser->socket_number = 0;
		pUser->pS = s;
		pUser->pCS = &crit;


		if (t == INVALID_SOCKET)
		{
			printf("Error in socket():%ld\n", WSAGetLastError());
			WSACleanup();
			return -1;
		}

		EnterCriticalSection(&crit);
		CreateThread(NULL, 0, ThreadServer, (LPVOID)pUser, 0, NULL);
		LeaveCriticalSection(&crit);

	}
	DeleteCriticalSection(&crit);
	closesocket(s);

	WSACleanup();

	return 0;
}