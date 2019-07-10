#pragma once
#ifndef STRUCTURE_H
#define STRUCTURE_H

#include "stdafx.h"
#include <WinSock2.h>
#include <stdio.h>
#include <string.h>

#include "ThreadRoom.h"
#include "ThreadServer.h"
#include "RoomList.h"


typedef struct user
{
	LPVOID pParam = (LPVOID)malloc(sizeof(SOCKET));
	SOCKET pS;
	int room_number;
	int socket_number;
	CRITICAL_SECTION* pCS;
};

#endif