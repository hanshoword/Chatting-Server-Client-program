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
extern int NickOverlapFlag;
extern char NickBuffer[TOTAL_CLIENT][TOTAL_CLIENT];
extern char Nickname[Nick_length][TOTAL_CLIENT];
extern char NickTemp[Nick_length][TOTAL_CLIENT];
extern int create_flag[MAX_ROOM];
extern char print_client[MAX_BUF][MAX_BUF];
extern DWORD nExitCode;

DWORD WINAPI ThreadServer(LPVOID pParam)
{
	user *User;
	User = (user*)pParam;
	SOCKET* pT = (SOCKET*)User->pParam;
	for (int i = 0; i < TOTAL_CLIENT; i++)
	{
		if (sock[i] == INVALID)
		{
			sock[i] = *pT;

			while (1)
			{
				strcpy(print_client[0], ".1. Create a chat room : input '/c' \n2. Enter the chat room: input '/e'");
				send(sock[i], print_client[0], strlen(print_client[0]), 0);
				break;
			}

			char rbuf_1[MAX_BUF] = { NULL };
			while (1)
			{
				char rlen_1 = recv(sock[i], rbuf_1, MAX_BUF, 0);

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


					

					User->socket_number = i;

					room_count[User->room_number - 1]++; //현재원 계산
					if (create_number == MAX_CLIENT)
					{					
						strcpy(print_client[0], ".You can not make a room anymore.. Please enter the room : ");
						send(sock[User->socket_number], print_client[0], strlen(print_client[0]), 0);
						create_number--;
						create_flag[User->room_number - 1] = -1;
						room_count[User->room_number - 1]--;
						continue;
					}
					else if (create_number != (MAX_CLIENT))
					{
						strcpy(print_client[0], ".Room ");
						itoa(User->room_number, print_client[1], 10);
						strcat(print_client[0], print_client[1]);
						strcat(print_client[0], " was created.");
						for (int n = 0; n < TOTAL_CLIENT; n++)
							send(sock[n], print_client[0], strlen(print_client[0]), 0);
					}
					
					/*복수개의 채팅방 스레드*/

					EnterCriticalSection(User->pCS);

					CreateThread(NULL, 0, Thread_room, (LPVOID)User, 0, NULL);
					LeaveCriticalSection(User->pCS);

					Sleep(100);

					/*Create일 경우 List출력*/
					RoomList_Copy();
					send(sock[i], list[0], strlen(list[0]), 0);
					Sleep(10);
					for (int k = 0; k < MAX_CLIENT; k++)
					{
						if (create_flag[k] == 1)
						{
							send(sock[i], list[k + 1], strlen(list[k + 1]), 0);
							Sleep(10);
						}
					}

					Sleep(10);
					send(sock[i], list[MAX_CLIENT + 1], strlen(list[MAX_CLIENT + 1]), 0);
					Sleep(10);

					/*Nickname 입력*/
					EnterCriticalSection(User->pCS);
					strcpy(print_client[3], ".Input your Nickname : ");
					send(sock[i], print_client[3], strlen(print_client[3]), 0);
					Sleep(10);

					while (1)
					{
						char Nick = recv(sock[i], Nick_buf, MAX_BUF, 0);

						if (strlen(Nick_buf) <= Nick_length)
						{
							strcpy(NickBuffer[i], Nick_buf);
						}
						else if (strlen(Nick_buf) > Nick_length)
						{
							while (1)
							{
								strcpy(print_client[6], ".Your nickname is too long. Please re-enter : ");
								send(sock[i], print_client[6], strlen(print_client[6]), 0);
								char Nick_buf_[MAX_BUF] = { 0 };
								char Nick_ = recv(sock[i], Nick_buf_, MAX_BUF, 0);
								if (strlen(Nick_buf_) >= Nick_length)
								{
									continue;
								}
								else
								{
									Nickflag = 1;
									strcpy(NickBuffer[i], Nick_buf_);
									strcpy(Nick_buf, Nick_buf_);
									break;
								}
							}
						}

						for (int a = 0; a < TOTAL_CLIENT; a++)
						{
							if (strcmp(NickBuffer[a], Nick_buf) == 0 && a != i)
							{
								NickOverlapFlag = 1;
								strcpy(print_client[4], ".The nickname is a duplicate. Input Re-enter :");
								send(sock[i], print_client[4], strlen(print_client[4]), 0);
							}
						}
						if (NickOverlapFlag == 1)
						{
							NickOverlapFlag = 0;
							continue;
						}
						else
						{
							strcpy(Nickname[i], Nick_buf);
							Nickflag = 0;
							break;
						}
					}
					
					LeaveCriticalSection(User->pCS);
					/*채팅 스레드*/
					EnterCriticalSection(User->pCS);
					CreateThread(NULL, 0, Thread_for_client, (LPVOID)User, 0, NULL);
					LeaveCriticalSection(User->pCS);
					//


					break;
				}
				else if (strcmp(rbuf_1, "/e") == 0)
				{
					if (createFlag == 0)
					{
						strcpy(print_client[8], ".No room created. You have to make a room. (Input : '/c') : ");
						send(sock[i], print_client[8], strlen(print_client[8]), 0);
						continue;
					}
					char rbuf_2[MAX_BUF] = { 0 };
					char Nick_buf_2[MAX_BUF] = { 0 };
					/*Enter일 경우 List출력*/
					send(sock[i], list[0], strlen(list[0]), 0);
					Sleep(10);
					for (int k = 0; k < MAX_CLIENT; k++)
					{
						if (create_flag[k] == 1)
						{
							send(sock[i], list[k + 1], strlen(list[k + 1]), 0);
							Sleep(10);
						}
					}
					Sleep(10);
					send(sock[i], list[MAX_CLIENT + 1], strlen(list[MAX_CLIENT + 1]), 0);
					Sleep(10);

					/*Input Room Number*/
					strcpy(print_client[1], ".[  Choose room number  ]");
					send(sock[i], print_client[1], strlen(print_client[1]), 0);
					Sleep(10);

					int roomNumFlag = 0;
					while (1)
					{
						char rbuf_4[MAX_BUF] = { 0 };
						char rlen_2 = recv(sock[i], rbuf_4, MAX_BUF, 0);

						if (strcmp(rbuf_4, "0") == 0 || (strcmp(rbuf_4, "0") != 0 && atoi(rbuf_4) == 0))
						{
							strcpy(print_client[7], ".That is Invalid number. Please re-enter : ");
							send(sock[i], print_client[7], strlen(print_client[7]), 0);
							continue;
						}

						if (create_flag[atoi(rbuf_4) - 1] == -1)
						{
							while (1)
							{
								strcpy(print_client[7], ".There is no room opened by that number.Please re-enter : ");
								send(sock[i], print_client[7], strlen(print_client[7]), 0);
								char rbuf_3[MAX_BUF] = { 0 };
								char rlen_3 = recv(sock[i], rbuf_3, MAX_BUF, 0);

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

					/*Nickname 입력*/

					EnterCriticalSection(User->pCS);
					strcpy(print_client[4], ".Input your Nickname : ");
					send(sock[i], print_client[4], strlen(print_client[4]), 0);
					Sleep(10);
					
					while (1)
					{
						char Nick = recv(sock[i], Nick_buf_2, MAX_BUF, 0);

						if (strlen(Nick_buf_2) <= Nick_length)
						{
							strcpy(NickBuffer[i], Nick_buf_2);
						}
						else if (strlen(Nick_buf_2) > Nick_length)
						{
							while (1)
							{
								strcpy(print_client[6], ".Your nickname is too long. Please re-enter : ");
								send(sock[i], print_client[6], strlen(print_client[6]), 0);
								char Nick_buf_[MAX_BUF] = { 0 };
								char Nick_ = recv(sock[i], Nick_buf_, MAX_BUF, 0);
								if (strlen(Nick_buf_) >= Nick_length)
								{
									continue;
								}
								else
								{
									Nickflag = 1;
									strcpy(NickBuffer[i], Nick_buf_);
									strcpy(Nick_buf_2, Nick_buf_);
									break;
								}
							}
						}

						for (int a = 0; a < TOTAL_CLIENT; a++)
						{
							if (strcmp(NickBuffer[a], Nick_buf_2) == 0 && a != i)
							{
								NickOverlapFlag = 1;
								strcpy(print_client[4], ".The nickname is a duplicate. Input Re-enter :");
								send(sock[i], print_client[4], strlen(print_client[4]), 0);
							}
						}
						if (NickOverlapFlag == 1)
						{
							NickOverlapFlag = 0;
							continue;
						}
						else
						{
							strcpy(Nickname[i], Nick_buf_2);
							Nickflag = 0;
							break;
						}
					}

					LeaveCriticalSection(User->pCS);

					/*복수개의 채팅방 스레드*/
					if (roomNumFlag == 0)
					{
						User->room_number = atoi(rbuf_2);
					}
					roomNumFlag = 0;

					User->socket_number = i;


					//EnterCriticalSection(User->pCS);
					room_count[User->room_number - 1]++; //현재원 계산
					if (room_count[User->room_number - 1] == MAX_CLIENT+1)
					{
						room_count[User->room_number - 1]--;
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
						strcpy(print_client[0], ".The room was full. Please Re-enter or Create");
						send(sock[User->socket_number], print_client[0], strlen(print_client[0]), 0);
						continue;
					}
					CreateThread(NULL, 0, Thread_room, (LPVOID)User, 0, NULL);
					//LeaveCriticalSection(User->pCS);

					Sleep(100);
					RoomList_Copy(); //list update

					EnterCriticalSection(User->pCS);
					CreateThread(NULL, 0, Thread_for_client, (LPVOID)User, 0, NULL);
					LeaveCriticalSection(User->pCS);

					break;

				}
				else
				{
					strcpy(print_client[2], ".Please input / c (Create Chat Room) or / e (Enter Chat Room).");
					send(sock[i], print_client[2], strlen(print_client[2]), 0);
					typing_errorflag = -1;
					break;
				}


				break;
			}
			if (typing_errorflag == -1)
			{
				sock[i] = INVALID;
				i--;
				typing_errorflag = 0;
				continue;
			}

			break;
		}
	}



	return 0;
	/*2*/

}