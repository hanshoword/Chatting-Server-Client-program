#pragma once
#ifndef ROOM_H
#define ROOM_H

#include "stdafx.h"
#include <WinSock2.h>
#include <stdio.h>
#include <string.h>

#include "Structure.h"
#include "ThreadServer.h"
#include "RoomList.h"

DWORD WINAPI Thread_room(LPVOID pParam);
DWORD WINAPI Thread_for_client(LPVOID pParam);

#endif