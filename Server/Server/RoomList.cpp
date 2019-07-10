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
#define TOTAL_CLIENT 100
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

void RoomList_Copy()
{
	strcpy(list[0], ".|_______CHATTING ROOM LIST_______|");
	for (int i = 1; i < MAX_CLIENT + 1; i++)
	{
		strcpy(&(list[i][0]), ".");
		strcpy(&(list[i][1]), "|");
		strcpy(&(list[i][2]), " ");
		strcpy(&(list[i][3]), " ");
		strcpy(&(list[i][4]), " ");
		strcpy(&(list[i][5]), " ");
		strcpy(&(list[i][6]), " ");
		strcpy(&(list[i][7]), " ");
		strcpy(&(list[i][8]), "[");
		if (i == 10)
		{
			itoa(1, &(list[i][9]), MAX_CLIENT);
			itoa(0, &(list[i][10]), MAX_CLIENT);
			strcpy(&(list[i][11]), "]");
			strcpy(&(list[i][12]), "C");
			strcpy(&(list[i][13]), "h");
			strcpy(&(list[i][14]), "a");
			strcpy(&(list[i][15]), "t");
			strcpy(&(list[i][16]), "t");
			strcpy(&(list[i][17]), "i");
			strcpy(&(list[i][18]), "n");
			strcpy(&(list[i][19]), "g");
			strcpy(&(list[i][20]), " ");
			strcpy(&(list[i][21]), "R");
			strcpy(&(list[i][22]), "o");
			strcpy(&(list[i][23]), "o");
			strcpy(&(list[i][24]), "m");
			strcpy(&(list[i][25]), "(");
			if (MAX_CLIENT >= 10 && MAX_CLIENT < 100)
			{
				if (room_count[i - 1] / 10 != 0)
				{
					itoa(room_count[i - 1] / 10, &(list[i][26]), MAX_CLIENT + 1);
					itoa(room_count[i - 1] % 10, &(list[i][27]), MAX_CLIENT + 1);
					strcpy(&(list[i][28]), "/");
					if (MAX_CLIENT / 10 != 0)
						itoa(MAX_CLIENT / 10, &(list[i][29]), 10);
					itoa(MAX_CLIENT % 10, &(list[i][30]), 10);
					strcpy(&(list[i][31]), ")");
					strcpy(&(list[i][32]), " ");
					strcpy(&(list[i][33]), " ");
					strcpy(&(list[i][34]), " ");
					strcpy(&(list[i][35]), " ");
					strcpy(&(list[i][36]), "|");
				}
				else
				{
					itoa(room_count[i - 1] % 10, &(list[i][26]), MAX_CLIENT + 1);
					strcpy(&(list[i][27]), "/");
					if (MAX_CLIENT / 10 != 0)
						itoa(MAX_CLIENT / 10, &(list[i][28]), 10);
					itoa(MAX_CLIENT % 10, &(list[i][29]), 10);
					strcpy(&(list[i][30]), ")");
					strcpy(&(list[i][31]), " ");
					strcpy(&(list[i][32]), " ");
					strcpy(&(list[i][33]), " ");
					strcpy(&(list[i][34]), "|");
				}
			}
			else
			{
				itoa(room_count[i - 1], &(list[i][26]), MAX_CLIENT + 1);
				strcpy(&(list[i][27]), "/");
				itoa(MAX_CLIENT, &(list[i][28]), 10);
				strcpy(&(list[i][29]), ")");
				strcpy(&(list[i][30]), " ");
				strcpy(&(list[i][31]), " ");
				strcpy(&(list[i][32]), " ");
				strcpy(&(list[i][33]), " ");
				strcpy(&(list[i][34]), "|");
			}
		}
		else
		{
			itoa(i, &(list[i][9]), MAX_CLIENT);
			strcpy(&(list[i][10]), "]");
			strcpy(&(list[i][11]), "C");
			strcpy(&(list[i][12]), "h");
			strcpy(&(list[i][13]), "a");
			strcpy(&(list[i][14]), "t");
			strcpy(&(list[i][15]), "t");
			strcpy(&(list[i][16]), "i");
			strcpy(&(list[i][17]), "n");
			strcpy(&(list[i][18]), "g");
			strcpy(&(list[i][19]), " ");
			strcpy(&(list[i][20]), "R");
			strcpy(&(list[i][21]), "o");
			strcpy(&(list[i][22]), "o");
			strcpy(&(list[i][23]), "m");
			strcpy(&(list[i][24]), "(");
			if (MAX_CLIENT >= 10 && MAX_CLIENT < 100)
			{
				if (room_count[i - 1] / 10 != 0)
				{
					itoa(room_count[i - 1] / 10, &(list[i][25]), MAX_CLIENT + 1);
					itoa(room_count[i - 1] % 10, &(list[i][26]), MAX_CLIENT + 1);
					strcpy(&(list[i][27]), "/");
					if (MAX_CLIENT / 10 != 0)
						itoa(MAX_CLIENT / 10, &(list[i][28]), 10);
					itoa(MAX_CLIENT % 10, &(list[i][29]), 10);
					strcpy(&(list[i][30]), ")");
					strcpy(&(list[i][31]), " ");
					strcpy(&(list[i][32]), " ");
					strcpy(&(list[i][33]), " ");
					strcpy(&(list[i][34]), " ");
					strcpy(&(list[i][35]), "|");
				}
				else
				{
					itoa(room_count[i - 1] % 10, &(list[i][25]), MAX_CLIENT + 1);
					strcpy(&(list[i][26]), "/");
					if (MAX_CLIENT / 10 != 0)
						itoa(MAX_CLIENT / 10, &(list[i][27]), 10);
					itoa(MAX_CLIENT % 10, &(list[i][28]), 10);
					strcpy(&(list[i][29]), ")");
					strcpy(&(list[i][30]), " ");
					strcpy(&(list[i][31]), " ");
					strcpy(&(list[i][32]), " ");
					strcpy(&(list[i][33]), " ");
					strcpy(&(list[i][34]), "|");
				}
			}
			else
			{
				itoa(room_count[i - 1], &(list[i][25]), MAX_CLIENT + 1);
				strcpy(&(list[i][26]), "/");
				itoa(MAX_CLIENT, &(list[i][27]), 10);
				strcpy(&(list[i][28]), ")");
				strcpy(&(list[i][29]), " ");
				strcpy(&(list[i][30]), " ");
				strcpy(&(list[i][31]), " ");
				strcpy(&(list[i][32]), " ");
				strcpy(&(list[i][33]), "|");
			}
		}
		
	}
	strcpy(list[MAX_CLIENT + 1], "-----------------------------------");
}