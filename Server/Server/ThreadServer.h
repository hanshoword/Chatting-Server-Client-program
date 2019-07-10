#pragma once
#ifndef SERVER_H
#define SERVER_H

#include "stdafx.h"
#include <WinSock2.h>
#include <stdio.h>
#include <string.h>

#include "ThreadRoom.h"
#include "Structure.h"
#include "RoomList.h"

DWORD WINAPI ThreadServer(LPVOID pParam);

#endif