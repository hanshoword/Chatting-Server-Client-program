#include "stdafx.h"
#include <stdio.h>
#include <winsock2.h>
#include <Windows.h>
#include<conio.h>

#define COL GetStdHandle(STD_OUTPUT_HANDLE)
#define RED SetConsoleTextAttribute(COL, 0x000c)
#define BLUE SetConsoleTextAttribute(COL, 0x0009)
#define YELLOW SetConsoleTextAttribute(COL, 0x000e)
#define GREEN SetConsoleTextAttribute(COL, 0x0002)
#define SKY_BLUE SetConsoleTextAttribute(COL, 0x000b)
#define HIGH_GREEN SetConsoleTextAttribute(COL, 0x000a)
#define GOLD SetConsoleTextAttribute(COL, 0x0006)
#define PURPLE SetConsoleTextAttribute(COL, 0x0005)
#define BLUE_GREEN SetConsoleTextAttribute(COL, 0x0003)
#define PLUM SetConsoleTextAttribute(COL, 0x000d)
#define ORIGINAL SetConsoleTextAttribute(COL, 0x0007)
#define MAX_BUF 100

char rbuf_2[MAX_BUF] = { 0 };

DWORD WINAPI Thread_for_display(LPVOID pParam)
{
	SOCKET* pS = (SOCKET*)pParam;
	if (*pS == INVALID_SOCKET)
	{
		printf("Error in socket(): %ld\n", WSAGetLastError());
		WSACleanup();
		return -1;
	}
	while (1)
	{
		char rbuf[MAX_BUF] = { 0 };
		recv(*pS, rbuf, MAX_BUF, 0);
		if (rbuf != 0)
		{
			if (strcmp(rbuf, "/x") == 0)
			{
				exit(1);
			}
			for (int n = 0; n < strlen(rbuf); n++)
			{
				rbuf_2[n] = rbuf[n + 1];
			}
			switch (atoi(&(rbuf[0])))
			{
			case 1: RED; printf("%s\n", rbuf_2); ORIGINAL; break;
			case 2: BLUE; printf("%s\n", rbuf_2); ORIGINAL; break;
			case 3: YELLOW; printf("%s\n", rbuf_2); ORIGINAL; break;
			case 4: GREEN; printf("%s\n", rbuf_2); ORIGINAL; break;
			case 5: SKY_BLUE; printf("%s\n", rbuf_2); ORIGINAL; break;
			case 6: HIGH_GREEN; printf("%s\n", rbuf_2); ORIGINAL; break;
			case 7: GOLD; printf("%s\n", rbuf_2); ORIGINAL; break;
			case 8: PURPLE; printf("%s\n", rbuf_2); ORIGINAL; break;
			case 9: BLUE_GREEN; printf("%s\n", rbuf_2); ORIGINAL; break;
			case 10: PLUM; printf("%s\n", rbuf_2); ORIGINAL; break;
			default: ORIGINAL; printf("%s\n", rbuf_2); ORIGINAL; break;
			}
		}
	}
	return 0;
}

int main()
{
	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
	{
		printf("Too high Winsock version\n");
		return -1;
	}
	//시작
	SOCKET s;
	SOCKADDR_IN cli;
	int cli_len = sizeof(cli);
	HANDLE hthread = 0;

	s = socket(AF_INET, SOCK_STREAM, 0);
	if (s == INVALID_SOCKET)
	{
		printf("Error in socket(): %ld\n", WSAGetLastError());
		WSACleanup();
		return -1;
	}
	ZeroMemory(&cli, sizeof(cli));
	cli.sin_family = AF_INET;
	cli.sin_port = htons(50000); //바이트 오더링
	cli.sin_addr.s_addr = inet_addr("165.229.17.105");

	connect(s, (SOCKADDR*)&cli, sizeof(cli));

	LPVOID pParam = (LPVOID)malloc(sizeof(SOCKET));
	*((SOCKET*)pParam) = s;

	hthread = CreateThread(NULL, 0, Thread_for_display, pParam, 0, NULL);
	while (1)
	{
		char sbuf[MAX_BUF] = { 0 };
		if (gets_s(sbuf) != NULL)
		{
			send(s, sbuf, strlen(sbuf), 0);
		}
		else
		{
			break;
		}
	}
	//소켓생성
	//소켓이름은 OS가 붙여준다

	closesocket(s);
	//소켓소멸

	WSACleanup();
	//끝

	return 0;
}

