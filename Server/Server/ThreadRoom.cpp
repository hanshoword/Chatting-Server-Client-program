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
#define Nick_length 10
#define MAX_ROOM 10
#define INVALID -1
#define TOTAL_CLIENT 90
extern SOCKET sock[MAX_CLIENT];
extern int room_count[MAX_CLIENT];
extern char list[MAX_CLIENT + 2][MAX_BUF];
extern int equal_number[MAX_CLIENT][MAX_CLIENT];
extern int typing_errorflag;
extern int Nickflag;
extern int quit_flag;
extern int createFlag;
extern int create_number;
extern int thread_num;
extern char Nickname[Nick_length][TOTAL_CLIENT];
extern char NickTemp[Nick_length][TOTAL_CLIENT];
extern int create_flag[MAX_ROOM];
extern char print_client[MAX_BUF][MAX_BUF];
extern DWORD nExitCode;
char flagBuf[MAX_CLIENT] = { 0 };
extern char NickBuffer[TOTAL_CLIENT][TOTAL_CLIENT];

DWORD WINAPI Thread_room(LPVOID pParam)
{
	user *User;
	User = (user*)pParam;

	static int j = 0;
	for (int i = 1; i < TOTAL_CLIENT; i++)
	{
		if (User->room_number == i)
		{
			if (equal_number[i - 1][0] == -10 && i != 1)
			{
				j = 0;
			}
			equal_number[(User->room_number) - 1][j] = User->socket_number;

			j++;
		}
	}

	return 0;
}

DWORD WINAPI Thread_for_client(LPVOID pParam)
{
	user *User;
	User = (user*)pParam;
	SOCKET* pT = (SOCKET*)User->pParam;
	strcat(Nickname[User->socket_number], ")");
	strcpy(NickTemp[User->socket_number], Nickname[User->socket_number]);

	Sleep(10);

	strcpy(print_client[5], ".|_____You have entered the chat room_____|\n(-Leave chat room : '/q' - Leave the server : '/x')");
	send(sock[User->socket_number], print_client[5], strlen(print_client[5]), 0);
	
	if (*pT == INVALID_SOCKET) {
		printf("Error in socket():%ld\n", WSAGetLastError());
		WSACleanup();

		return -1;
	}
	while (1)
	{
		char rbuf[MAX_BUF] = { 0 };
		int rlen = recv(*pT, rbuf, MAX_BUF, 0); //rlen =0일때 잡음.

		if (rlen > 0 && *pT != INVALID_SOCKET)
		{
			if (strcmp(rbuf, "/q") == 0)
			{
				rbuf[rlen] = NULL;

				room_count[User->room_number - 1]--;
				if (room_count[User->room_number - 1] == 0)
				{
					create_flag[User->room_number - 1] = -1;
					create_number--;
					if (create_number == 0)
					{
						createFlag = 0;
					}
				}
				for (int a = 0; a < MAX_CLIENT; a++)
				{
					for (int b = 0; b < MAX_CLIENT; b++)
					{
						if (equal_number[a][b] == User->socket_number)
						{
							equal_number[a][b] = -10;

						}
					}

				}
				RoomList_Copy();


				while (1)
				{
					char rbuf_1[MAX_BUF] = { NULL };
					if (typing_errorflag != -1)
					{
						while (1)
						{
							strcpy(print_client[0], ".1. Create a chat room : input '/c' \n2. Enter the chat room: input '/e'");
							send(sock[User->socket_number], print_client[0], strlen(print_client[0]), 0);
							break;
						}
					}
					typing_errorflag = 0;

					char rlen_1 = recv(sock[User->socket_number], rbuf_1, MAX_BUF, 0);

					if (strcmp(rbuf_1, "/c") == 0)
					{
						char Nick_buf[MAX_BUF] = { 0 };

						for (int n = 0; n < MAX_CLIENT; n++)
						{
							if (create_flag[n] == -1)
							{
								create_flag[n] = 1;
								create_number++;
								createFlag = 1;
								User->room_number = n + 1;
								break;
							}
						}
						if (create_number != MAX_CLIENT)
						{
							strcpy(print_client[0], ".Room ");
							itoa(User->room_number, print_client[1], 10);
							strcat(print_client[0], print_client[1]);
							strcat(print_client[0], " was created.");
							send(sock[User->socket_number], print_client[0], strlen(print_client[0]), 0);
						}

						/*복수개의 채팅방 스레드*/

						room_count[User->room_number - 1]++; //현재원 계산
						if (create_number == MAX_CLIENT)
						{
							create_number--;
							create_flag[User->room_number - 1] = -1;
							room_count[User->room_number - 1]--;
							strcpy(print_client[0], ".You can not make a room anymore.. Please enter the room : ");
							send(sock[User->socket_number], print_client[0], strlen(print_client[0]), 0);
							continue;
						}
						EnterCriticalSection(User->pCS);
						static int j = 0;
						for (int i = 1; i < TOTAL_CLIENT; i++)
						{
							if (User->room_number == i)
							{
								for (int k = 0; k < MAX_CLIENT; k++)
								{
									if (equal_number[(User->room_number) - 1][k] == -10)
									{
										j = k;
										break;
									}
								}
								equal_number[(User->room_number) - 1][j] = User->socket_number;
							}
						}

						LeaveCriticalSection(User->pCS);
						Sleep(100);

						/*Create일 경우 List출력*/
						RoomList_Copy();
						send(sock[User->socket_number], list[0], strlen(list[0]), 0);
						Sleep(10);
						for (int k = 0; k < MAX_CLIENT; k++)
						{
							if (create_flag[k] == 1)
							{
								send(sock[User->socket_number], list[k + 1], strlen(list[k + 1]), 0);
								Sleep(10);
							}
						}

						Sleep(10);
						send(sock[User->socket_number], list[MAX_CLIENT + 1], strlen(list[MAX_CLIENT + 1]), 0);
						Sleep(10);

						strcpy(print_client[5], ".|_____You have entered the chat room_____|\n(-Leave chat room : '/q' - Leave the server : '/x')");
						send(sock[User->socket_number], print_client[5], strlen(print_client[5]), 0);

						break;
					}
					else if (strcmp(rbuf_1, "/e") == 0)
					{
						if (createFlag == 0)
						{
							strcpy(print_client[8], ".No room created. You have to make a room. (Input : '/c') : ");
							send(sock[User->socket_number], print_client[8], strlen(print_client[8]), 0);
							continue;
						}
						char rbuf_2[MAX_BUF] = { 0 };
						char Nick_buf_2[MAX_BUF] = { 0 };
						/*Enter일 경우 List출력*/
						send(sock[User->socket_number], list[0], strlen(list[0]), 0);
						Sleep(10);
						for (int k = 0; k < MAX_CLIENT; k++)
						{
							if (create_flag[k] == 1)
							{
								send(sock[User->socket_number], list[k + 1], strlen(list[k + 1]), 0);
								Sleep(10);
							}
						}
						Sleep(10);
						send(sock[User->socket_number], list[MAX_CLIENT + 1], strlen(list[MAX_CLIENT + 1]), 0);
						Sleep(10);

						/*Input Room Number*/
						strcpy(print_client[1], ".[  Choose room number  ]");
						send(sock[User->socket_number], print_client[1], strlen(print_client[1]), 0);
						Sleep(10);
						int roomNumFlag = 0;
						while (1)
						{
							char rbuf_4[MAX_BUF] = { 0 };
							char rlen_2 = recv(sock[User->socket_number], rbuf_4, MAX_BUF, 0);

							if (strcmp(rbuf_4, "0") == 0 || (strcmp(rbuf_4, "0") != 0 && atoi(rbuf_4) == 0))
							{
								strcpy(print_client[7], ".That is Invalid number. Please re-enter : ");
								send(sock[User->socket_number], print_client[7], strlen(print_client[7]), 0);
								continue;
							}

							if (create_flag[atoi(rbuf_4) - 1] == -1)
							{
								while (1)
								{
									strcpy(print_client[7], ".There is no room opened by that number.Please re-enter : ");
									send(sock[User->socket_number], print_client[7], strlen(print_client[7]), 0);
									char rbuf_3[MAX_BUF] = { 0 };
									char rlen_3 = recv(sock[User->socket_number], rbuf_3, MAX_BUF, 0);

									if (create_flag[atoi(rbuf_3) - 1] == -1 || strcmp(rbuf_3, "0") == 0 || (strcmp(rbuf_3, "0") != 0 && atoi(rbuf_3) == 0))
									{
										continue;
									}
									else
									{
										roomNumFlag = 1;
										User->room_number = atoi(rbuf_3);
										break;
									}
								}
								break;
							}
							else
							{
								strcpy(rbuf_2, rbuf_4);
								break;
							}
						}

						/*복수개의 채팅방 스레드*/

						if (roomNumFlag == 0)
						{
							User->room_number = atoi(rbuf_2);
						}

						room_count[User->room_number - 1]++; //현재원 계산
						if (room_count[User->room_number - 1] == MAX_CLIENT)
						{
							room_count[User->room_number - 1]--;
							strcpy(print_client[0], ".The room was full. Please Re-enter or Create");
							send(sock[User->socket_number], print_client[0], strlen(print_client[0]), 0);
							continue;
						}
						EnterCriticalSection(User->pCS);
						static int j = 0;
						for (int i = 1; i < TOTAL_CLIENT; i++)
						{
							if (User->room_number == i)
							{
								for (int k = 0; k < MAX_CLIENT; k++)
								{
									if (equal_number[(User->room_number) - 1][k] == -10)
									{
										j = k;
										break;
									}
								}
								equal_number[(User->room_number) - 1][j] = User->socket_number;
							}
						}

						LeaveCriticalSection(User->pCS);
						Sleep(100);

						strcpy(print_client[5], ".|_____You have entered the chat room_____|\n(-Leave chat room : '/q' - Leave the server : '/x')");
						send(sock[User->socket_number], print_client[5], strlen(print_client[5]), 0);

						RoomList_Copy(); //list update

						break;

					}
					else
					{
						strcpy(print_client[2], ".Please input / c (Create Chat Room) or / e (Enter Chat Room).");
						send(sock[User->socket_number], print_client[2], strlen(print_client[2]), 0);
						typing_errorflag = -1;
						continue;
					}

					break;
				}
			}
			else if (strcmp(rbuf, "/x") == 0)
			{
				strcpy(print_client[8], ".Leave the server");
				send(sock[User->socket_number], print_client[8], strlen(print_client[8]), 0);
				Sleep(10);
				send(sock[User->socket_number], rbuf, strlen(rbuf), 0);
				printf("send msg : %s\n", rbuf);


				room_count[User->room_number - 1]--;
				if (room_count[User->room_number - 1] == 0)
				{
					create_flag[User->room_number - 1] = -1;
					create_number--;
					if (create_number == 0)
					{
						createFlag = 0;
					}
				}
				for (int a = 0; a < MAX_CLIENT; a++)
				{
					for (int b = 0; b < MAX_CLIENT; b++)
					{
						if (equal_number[a][b] == User->socket_number)
						{
							equal_number[a][b] = -10;
						}						
					}
				}				
				strcpy(NickBuffer[User->socket_number], "\0");
				sock[User->socket_number] = INVALID;

				RoomList_Copy();
			}

			if (rlen == 0 || *pT == INVALID_SOCKET)
			{
				for (int i = 0; i < TOTAL_CLIENT; i++)
				{
					if (sock[i] == *pT)
					{
						closesocket(sock[i]);
					}
				}
				printf("Error in socket(): %ld\n", WSAGetLastError());
				WSACleanup();
				return -1;
			}


			printf("recv msg : %s\n", rbuf);


			EnterCriticalSection(User->pCS);
			strcat(Nickname[User->socket_number], rbuf);
			itoa((User->socket_number+1) % 10, flagBuf, 10);
			strcat(flagBuf, Nickname[User->socket_number]);
			for (int i = 0; i < MAX_CLIENT; i++)
			{
				if ((sock[User->socket_number] != INVALID) && strcmp(rbuf, "/q") != 0 && strcmp(rbuf, "/x") != 0) //&& (sock[i] != *pT)
				{
					if (equal_number[User->room_number - 1][i] != User->socket_number && equal_number[User->room_number - 1][i] != -10)
					{
						send(sock[equal_number[User->room_number - 1][i]], flagBuf, strlen(flagBuf), 0);
						printf("send msg : %s\n", rbuf);
					}
				}
			}

			strcpy(Nickname[User->socket_number], NickTemp[User->socket_number]);

			LeaveCriticalSection(User->pCS);
		}


	}

	closesocket(*pT);

	return 0;
}
