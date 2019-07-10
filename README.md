## Chatting-Server-Client-program

### 프로젝트 목적 및 목표

#### 목적
```
- C를 기반으로 한 TCP SOCKET 채팅 프로그램을 설계 및 구현 할 수 있도록 한다. 
- 하나의 서버가 여러 클라이언트에 대응 할 수 있도록 컴퓨터 네트워크 이론과 소켓 프로그래밍에 대한 전반적인 능력을 갖춘다.
- Class(or 구조체)와 Multi-thread를 운용한 소켓 프로그래밍 설계능력을 갖춘다.
- 위를 토대로한 실무능력 함양
```

#### 목표

```
현실에서는 클라이언트가 여러 명이므로, 서버가 여러 클라이언트에 일 대 다로 대응 하도록 프로그램을 구현한다.
```

#### 설계 목표

```
- 복수개의 채팅방을 제공
  1) Client가 새로운 채팅방을 만들거나 참가 할 수 있으며, Client가 사라진 채팅방은 없어진다. (O)
  2) Client에게 채팅방 List(방목록 및 Client 수)를 보여준다. (O)
  3) Client는 채팅방을 자유롭게 드나들 수 있으며 서버를 나갈 수도 있어야 한다.(O)
  
  - 채팅방이 없는 경우 참여 할 수 없다.
  - 특정 명령어가 아닌 경우 다시 입력해야한다.
  
- Client 닉네임
  - Client는 닉네임을 정할 수 있다.(O)
    - 닉네임 중복을 체크해야한다.
    - 닉네임 길이가 너무 길면 안 된다.
  
- 기타 사항
  - 각 클라이언트의 메시지의 색깔은 다르게 한다.(O)
    - 닉네임+메시지의 색만 다르게 한다.
```

##### 기타사항
```
각 클라이언트의 메시지의 색깔은 다르게 하여 닉네임 이외에 메시지가 같은 사람에게서 	왔는지 바로 판단할 수 있도록 하였고 심미성도 추가되었다. 
```

#### 설계 기술
```
* Socket Programming
* MultiThread
* Class / Structure
```

#### 제한요소
```
- Socket
  TCP Multiplexing을 구현 할 수 있어야 한다.
   Socket 
    - s (접속대기)
    - t (Server socket)
    
- Thread
  MultiThread를 이용하여 양방향 통신이 가능하게 구현해야한다.
  복수의 Client에 대응 할 수 있어야하며, 양방향 통신이 가능해야한다.
  
- Class / Structure
  Thread parameter로 구조체를 넘겨주도록 구현해야한다.  
  Threadparam을 여러 필요한 정보를 포함하여 인자로 넘겨준다.  
```

#### 구현사양

```
- Socket
  1) s (접속 대기) socket
    : 하나의 socket은 항상 접속 대기 중이다.
  2) t (Server socket)
    : Client가 접속 할 때 마다 일대일 대응되는 서버소켓이 만들어 진다.
    
- Thread
  1) Client Thread
    : 받은 메시지를 Console창에 출력한다. (양방향 통신을 위해 Thread로 구현한다.)
  2) Server Thread
    : 여러 Client가 동시에 채팅방을 생성, 들어 갈 수 있도록 스레드로 생성한다.
  3) Room Thread
    : 채팅방에서 Room number가 같은 채팅방에게 메시지를 send해주는 역할을 한다. (양방향 통신을 위해 Thread로 구현한다.)
  
- Structure
  Thread parameter로 구조체를 넘기며 구조체 인자로 Room number, Soket number, Critical section parameter를 포함한다.
```
    
#### Functional Block Diagram & Sequence Diagram


<img src = "https://user-images.githubusercontent.com/47768726/60975889-a5323200-a367-11e9-8651-e508250e2d48.png" width = 70% height = 70%></img>

<img src = "https://user-images.githubusercontent.com/47768726/60975890-a5323200-a367-11e9-8a92-11c5e3ce29a5.png" width = 70% height = 70%></img>

#### Thread Pseudo Code
* Client 1) Thread_for_display
```
1. DWORD WINAPI Thread_for_display(LPVOID pParam)
2. {
3. 	SOCKET* pS = (SOCKET*)pParam;
4. 
5. 	if (*pS == INVALID_SOCKET)
6. 	{
7. 		printf("Error in socket(): %ld\n", WSAGetLastError());
8. 		WSACleanup();
9. 		return -1;
10. 	}
11. 
12. 	while (1)
13. 	{
14. 		char rbuf[MAX_BUF] = { 0 };
15. 		recv(*pS, rbuf, MAX_BUF, 0);
16. 
17. 		if (rbuf != 0)
18. 		{
19. 			for (int n = 0; n < strlen(rbuf); n++)
20. 			{
21. 				rbuf_2[n] = rbuf[n + 1]; //re-mapping
22. 			}
23. 
24. 			switch (atoi(&(rbuf[0]))) //Enter the color for each socket
25. 			{
26.                                /*COLOR MSG PRINT*/
27. 			case 1: RED; print(rbuf_2); ORIGINAL; break;
28. 			case 2: BLUE; print(rbuf_2); ORIGINAL; break;
29. 			case 3: YELLOW; print(rbuf_2); ORIGINAL; break;
30. 			case 4: GREEN; print(rbuf_2); ORIGINAL; break;
31. 			case 5:SKY_BLUE; print(rbuf_2); ORIGINAL; break;
32. 			case 6: HIGH_GREEN; print(rbuf_2); ORIGINAL; break;
33. 			case 7: GOLD; print(rbuf_2); ORIGINAL; break;
34. 			case 8: PURPLE; print(rbuf_2); ORIGINAL; break;
35. 			case 9: BLUE_GREEN; print(rbuf_2); ORIGINAL; break;
36. 			case 10: PLUM; printf(rbuf_2); ORIGINAL; break;
37. 
38. 			default: ORIGINAL; print(rbuf_2); ORIGINAL; break;
39. 			}			
40. 		}
41. 
42. 	}
43. 	return 0;
44. }
```
* Server 1) ThreadServer
```c
1. DWORD WINAPI ThreadServer(LPVOID pParam)
2. {
3. 	user *User;
4. 	User = (user*)pParam;
5. 	SOCKET* pT = (SOCKET*)User->pParam;
6. 	for (int i = 0; i < TOTAL_CLIENT; i++)
7. 	{
8. 		if (sock[i] == INVALID)
9. 		{
10. 			sock[i] = *pT;
11. 			while (1)
12. 			{
13. 				SEND CREATE OR ENTER MSG
14. 				break;
15. 			}
16. 			char rbuf_1[MAX_BUF] = { NULL };
17. 			while (1)
18. 			{
19. 				char rlen_1 = recv(sock[i], rbuf_1, MAX_BUF, 0);
20.                                           /* IF input ‘/c’ (Create) */
21. 				if (strcmp(rbuf_1, "/c") == 0)
22. 				{
23. 					char Nick_buf[MAX_BUF] = { 0 };
24. 					for (int n = 0; n < MAX_CLIENT; n++)
25. 					{
26. 						if (create_flag[n] == -1)
27. 						{
28. 							create_flag[n] = 1; 
29.                                                                       //n번방이 생성되었음을 표시하는 flag
30. 							create_number++;
31.                                                                       //방의 개수
32. 							createFlag = 1;
33.                                                                       //최소 한 개의 방이 있는지 표시하는 flag
34. 							User->room_number = n + 1;
35.                                                                       //Create 시에 방 번호는 1번부터 오름차순 
36. 							break;
37. 						}
38. 					}
39.                                                       /*몇 번 방이 Create 됐는지 모든 Client에게 알림*/
40.                                                       SEND CREATE ROOM MSG (BROAD CAST)
41. 
42. 					User->socket_number = i;
43. 					room_count[User->room_number - 1]++; //현재원 계산
44. 					if (create_number == MAX_CLIENT) //방이 꽉 찼을 때
45. 					{
46. 						create_number--;
47. 						create_flag[User->room_number - 1] = -1;
48. 						room_count[User->room_number - 1]--;
49. 						SEND ROOM IS FULL MSG
50. 						continue;
51. 					}
52. 
53. 					/*복수개의 채팅방 스레드*/
54. 					EnterCriticalSection(User->pCS);
55. 					CreateThread(NULL, 0, Thread_room, (LPVOID)User, 0, NULL);
56. 					LeaveCriticalSection(User->pCS);
57. 					Sleep(100);
58. 
59. 					/*Create일 경우 List출력*/
60. 					RoomList_Copy();
61. 					SEND ROOM LIST (방 번호 및 현재원/총원)
62. 					Sleep(10);
63. 				           
64. 					/*Nickname 입력*/
65. 					EnterCriticalSection(User->pCS);
66.                                                       SEND INPUT NICKNAME MSG
67. 					Sleep(10);
68. 
69. 					while (1)
70. 					{
71. 						char Nick = recv(sock[i], Nick_buf, MAX_BUF, 0);
72. 						if (strlen(Nick_buf) <= Nick_length)
73. 						{
74. 							strcpy(NickBuffer[i], Nick_buf);
75. 						}
76. 						else if (strlen(Nick_buf) > Nick_length)
77. 						{
78. 							while (1)
79. 							{
80. 				                                       SEND RE INPUT NICKNAME MSG
81. 				                                       char Nick_buf_[MAX_BUF] = { 0 };
82. 				                                       char Nick_=recv(sock[i], Nick_buf_)
83. 							    if (strlen(Nick_buf_)>= Nick_length)
84. 							    {
85. 								continue;
86. 							    }
87. 							    else
88. 							    {
89. 						                     Nickflag = 1;
90. 							         COPY(NickBuffer[i], Nick_buf_)
91. 							         strcpy(Nick_buf, Nick_buf_);
92. 							         break;
93. 							     }
94. 							 }
95. 						}
96. 
97. 						for (int a = 0; a < TOTAL_CLIENT; a++)
98. 						{
99. 						   if (strcmp(NickBuffer[a], Nick_buf) == 0 && a != i)
100. 						   {
101. 						      NickOverlapFlag = 1;
102. 						      SEND RE INPUT NICKNAME MSG
103. 						   }
104. 						}
105. 						if (NickOverlapFlag == 1)
106. 						{
107. 							NickOverlapFlag = 0;
108. 							continue;
109. 						}
110. 						else
111. 						{
112. 							strcpy(Nickname[i], Nick_buf);
113. 							Nickflag = 0;
114. 							break;
115. 						}
116. 					}					
117. 					LeaveCriticalSection(User->pCS);
118. 					/*채팅 스레드*/
119. 					EnterCriticalSection(User->pCS);
120. 					CreateThread(NULL, 0, Thread_for_client, (LPVOID)User, 0, NULL);
121. 					LeaveCriticalSection(User->pCS);
122. 					break;
123. 				}
124. 				else if (strcmp(rbuf_1, "/e") == 0)
125. 				{
126. 					if (createFlag == 0)
127. 					{
128. 						SEND CREATE ROOM MSG
129. 						continue;
130. 					}
131. 					char rbuf_2[MAX_BUF] = { 0 };
132. 					char Nick_buf_2[MAX_BUF] = { 0 };
133. 					/*Enter일 경우 List출력*/
134. 					SEND ROOM LIST (방 번호, 현재원/총원)
135. 					Sleep(10);
136. 					/*Input Room Number*/
137. 					SEND CHOOSE ROOM NUMBER MSG
138. 					Sleep(10);
139. 					int roomNumFlag = 0;
140. 					while (1)
141. 					{
142. 						char rbuf_4[MAX_BUF] = { 0 };
143. 						char rlen_2 = recv(sock[i], rbuf_4, MAX_BUF, 0);
144. 				                       if (strcmp(rbuf_4, "0") == 0 || 
145.                                                                   (strcmp(rbuf_4, "0") != 0 && atoi(rbuf_4) == 0))
146. 						{
147. 							SEND RE-ENTER ROOM NUM MSG
148. 							continue;
149. 						}
150. 						if (create_flag[atoi(rbuf_4) - 1] == -1)
151. 						{
152. 						         while (1)
153. 						         {
154. 							SEND RE-ENTER ROOM NUM MSG	
155. 							char rbuf_3[MAX_BUF] = { 0 };
156. 							char rlen_3 = recv(sock[i], rbuf_3, MAX_BUF, 0);
157. 	                                                                     if (create_flag[atoi(rbuf_3) - 1] == -1 || 
158.                                                                             strcmp(rbuf_3, "0") == 0 || 
159.                                                                            (strcmp(rbuf_3, "0") != 0 && atoi(rbuf_3) == 0))
160. 							{
161. 							     continue;
162. 							}
163. 							else
164. 							{
165. 							   roomNumFlag = 1;
166. 							   User->room_number = atoi(rbuf_3);
167. 							   break;
168. 							}
169. 						          }
170. 						          break;
171. 						  }
172. 						  else
173. 						  {
174. 							strcpy(rbuf_2, rbuf_4);
175. 							break;
176. 						  }
177. 					}
178. 					/*Nickname 입력*/
179. 					EnterCriticalSection(User->pCS);
180. 					SEND INPUT NICKNAME MSG
181. 					Sleep(10);					
182. 					while (1)
183. 					{
184. 						char Nick = recv(sock[i], Nick_buf_2, MAX_BUF, 0);
185. 						if (strlen(Nick_buf_2) <= Nick_length)
186. 						{
187. 							strcpy(NickBuffer[i], Nick_buf_2);
188. 						}
189. 						else if (strlen(Nick_buf_2) > Nick_length)
190. 						{
191. 							while (1)
192. 							{
193. 								SEND RE-ENTER MSG
194. 								char Nick_buf_[MAX_BUF] = { 0 };
195. 								NICK RECV
196. 								if (strlen(Nick_buf_)>= Nick_length)
197. 								{
198. 									continue;
199. 								}
200. 								else
201. 								{
202. 								    Nickflag = 1;
203. 								    strcpy(NickBuffer[i], Nick_buf_);
204. 								    strcpy(Nick_buf_2, Nick_buf_);
205. 								    break;
206. 								}
207. 							}
208. 						}
209. 						for (int a = 0; a < TOTAL_CLIENT; a++)
210. 						{
211. 						    if (strcmp(NickBuffer[a], Nick_buf_2) == 0 && a != i)
212. 						    {
213. 							NickOverlapFlag = 1;
214. 							SEND RE-ENTER MSG
215. 						    }
216. 						}
217. 						if (NickOverlapFlag == 1)
218. 						{
219. 							NickOverlapFlag = 0;
220. 							continue;
221. 						}
222. 						else
223. 						{
224. 							strcpy(Nickname[i], Nick_buf_2);
225. 							Nickflag = 0;
226. 							break;
227. 						}
228. 					}
229. 					LeaveCriticalSection(User->pCS);
230. 					/*복수개의 채팅방 스레드*/
231. 					if (roomNumFlag == 0)
232. 					{
233. 						User->room_number = atoi(rbuf_2);
234. 					}
235. 					roomNumFlag = 0;
236. 					User->socket_number = i;
237. 					room_count[User->room_number - 1]++; //현재원 계산
238. 					if (room_count[User->room_number - 1] == MAX_CLIENT+1)
239. 					{
240. 					   room_count[User->room_number - 1]--;
241. 					   for (int a = 0; a < MAX_CLIENT; a++)
242. 					   {
243. 					     for (int b = 0; b < MAX_CLIENT; b++)
244. 					     {
245. 					       if (equal_number[a][b] == User->socket_number)
246. 					       {
247. 				 		equal_number[a][b] = -1;
248. 				 	       }
249. 					     }
250. 					   }
251. 					   SEND ROOM IS FULL MSG
252. 					   continue;
253. 					  }
254. 					CreateThread(NULL, 0, Thread_room, (LPVOID)User, 0, NULL);
255. 					Sleep(100);
256. 					RoomList_Copy(); //list update
257. 					EnterCriticalSection(User->pCS);
258. 					CreateThread(NULL, 0, Thread_for_client, (LPVOID)User, 0, NULL);
259. 					LeaveCriticalSection(User->pCS);
260. 					break;
261. 				}
262. 				else
263. 				{
264. 					SEND INPUT “/c” or “/e” MSG
265. 					typing_errorflag = -1;
266. 					break;
267. 				}
268. 				break;
269. 			}
270. 			if (typing_errorflag == -1)
271. 			{
272. 				sock[i] = INVALID;
273. 				i--;
274. 				typing_errorflag = 0;
275. 				continue;
276. 			}
277. 			break;
278. 		}
279. 	}
280. 	return 0;
281. }
```

* Server 2) Thread_FOR_CLIENT
```c
1. DWORD WINAPI Thread_for_client(LPVOID pParam)
2. {
3. 	user *User;
4. 	User = (user*)pParam;
5. 	SOCKET* pT = (SOCKET*)User->pParam;
6. 	strcat(Nickname[User->socket_number], ")");
7. 	strcpy(NickTemp[User->socket_number], Nickname[User->socket_number]);
8. 
9. 	Sleep(10);
10. 
11. 	SEND INPUT “YOU ENTERED ROOM” MSG
12. 	if (*pT == INVALID_SOCKET) 
13.        {
14. 		printf("Error in socket():%ld\n", WSAGetLastError());
15. 		WSACleanup();
16. 		return -1;
17. 	}
18. 	while (1)
19. 	{
20. 		char rbuf[MAX_BUF] = { 0 };
21. 		int rlen = recv(*pT, rbuf, MAX_BUF, 0);
22. 		if (rlen > 0 && *pT != INVALID_SOCKET)
23. 		{
24.                               /*‘/q’ 를 입력해서 해당 채팅방에서 나갔을 때*/
25. 			if (strcmp(rbuf, "/q") == 0)
26. 			{
27. 				rbuf[rlen] = NULL;
28. 				room_count[User->room_number – 1]--; //해당 방 인원 -1
29. 				if (room_count[User->room_number - 1] == 0) //나가고 난 후 방이 비었으면
30. 				{
31. 					create_flag[User->room_number - 1] = -1; //해당 방의 FLAG -1
32. 					create_number--; // 생성된 방 -1
33. 					if (create_number == 0)
34. 					{
35. 						createFlag = 0; //방이 없다면 방이 없다는 표시로 0
36. 					}
37. 				}
38.                                           /*Room-num 별로 정렬된 2차원 배열의 해당 소켓넘버를 제거*/
39. 				for (int a = 0; a < MAX_CLIENT; a++)
40. 				{
41. 					for (int b = 0; b < MAX_CLIENT; b++)
42. 					{
43. 						if (equal_number[a][b] == User->socket_number)
44. 						{
45. 							equal_number[a][b] = -1;
46. 						}
47. 					}
48. 				}
49. 				RoomList_Copy();
50. 				while (1)
51. 				{
52. 					char rbuf_1[MAX_BUF] = { NULL };
53. 					if (typing_errorflag != -1)
54. 					{
55. 						while (1)
56. 						{
57. 							SEND INPUT “/c” or “/e” MSG
58. 							break;
59. 						}
60. 					}
61. 					typing_errorflag = 0;
62. 					RECV ‘/c’ or ‘/e’ WITH ERROR CHECK
63. 					if (strcmp(rbuf_1, "/c") == 0)
64. 					{
65. 						char Nick_buf[MAX_BUF] = { 0 };
66. 						for (int n = 0; n < MAX_CLIENT; n++)
67. 						{
68. 							if (create_flag[n] == -1)
69. 							{
70. 								create_flag[n] = 1;
71. 								create_number++;
72. 								createFlag = 1;
73. 								User->room_number = n + 1;
74. 								break;
75. 							}
76. 						}
77. 						SEND ROOM CREATE MSG (BREAD CAST)
78. 						/*복수개의 채팅방 스레드*/
79. 						room_count[User->room_number-1]++; //현재원 계산
80. 						if (create_number == MAX_CLIENT)
81. 						{
82. 							create_number--;
83. 							create_flag[User->room_number-1] = -1;
84. 							room_count[User->room_number - 1]--;
85. 							SEND “YOU CANT CREATE ROOM“ MSG
86. 							continue;
87. 						}
88. 						EnterCriticalSection(User->pCS);
89. 						MAPPING MATRIX BY ROOM NUMBER
90. 						LeaveCriticalSection(User->pCS);
91. 						Sleep(100);
92. 						/*Create일 경우 List출력*/
93. 						RoomList_Copy();
94. 						SEND ROOM LIST
95. 						Sleep(10);
96. 						break;
97. 					}
98. 					else if (strcmp(rbuf_1, "/e") == 0)
99. 					{
100. 						if (createFlag == 0)
101. 						{
102. 							SEND INPUT “/c” or RE “/e” MSG
103. 							continue;
104. 						}
105. 						char rbuf_2[MAX_BUF] = { 0 };
106. 						char Nick_buf_2[MAX_BUF] = { 0 };
107. 						/*Enter일 경우 List출력*/
108. 						SEND ROOM LIST
109. 						Sleep(10);
110. 						/*Input Room Number*/
111. 						SEND CHOOSE ROOM NUM MSG
112. 						Sleep(10);
113. 						int roomNumFlag = 0;
114. 						while (1)
115. 						{
116. 						  char rbuf_4[MAX_BUF] = { 0 };
117. 						  Receive in Rbuffer_4
118. 						  if (strcmp(rbuf_4, "0") == 0 
119.                                                                       || (strcmp(rbuf_4, "0") != 0 
120.                                                                              && atoi(rbuf_4) == 0))
121. 							{
122. 								SEND RE-ENTER MSG
123. 								continue;
124. 							}
125. 							if (create_flag[atoi(rbuf_4) - 1] == -1)
126. 							{
127. 							  while (1)
128. 							  {
129. 							    SEND RE-ENTER MSG
130. 							    char rbuf_3[MAX_BUF] = { 0 };
131. 					                            Receive in Rbuffer_3
132. 							    if (create_flag[atoi(rbuf_3) - 1] == -1 
133.                                                                                     || strcmp(rbuf_3, "0") == 0 
134.                                                                                       || (strcmp(rbuf_3, "0") != 0 
135.                                                                                              && atoi(rbuf_3) == 0))
136. 								      {
137. 									continue;
138. 								       }
139. 								       else
140. 								       {
141. 									roomNumFlag = 1;
142. 							       User->room_number = atoi(rbuf_3);
143. 									break;
144. 								         }
145. 							      }
146. 							      break;
147. 							}
148. 							else
149. 							{
150. 								strcpy(rbuf_2, rbuf_4);
151. 								break;
152. 							}
153. 						}
154. 						/*복수개의 채팅방 스레드*/
155. 						if (roomNumFlag == 0)
156. 						{
157. 							User->room_number = atoi(rbuf_2);
158. 						}
159. 						room_count[User->room_number -1]++;//현재원 계산
160. 					          if(room_count[User->room_number-1]== MAX_CLIENT)
161. 					          {
162. 						 room_count[User->room_number - 1]--;
163. 						 SEND ROOM IS FULL MSG
164. 						 continue;
165. 					           }
166. 						EnterCriticalSection(User->pCS);
167. 	                                                         MAPPING MATRIX BY ROOM NUMBER
168. 						LeaveCriticalSection(User->pCS);
169. 						Sleep(100);
170. 						RoomList_Copy(); //list update
171. 						break;
172. 					}
173. 					else
174. 					{
175. 						SEND INPUT “/c” or RE “/e” MSG
176. 						typing_errorflag = -1;
177. 						continue;
178. 					}
179. 					break;
180. 				}
181. 			}
182. 			else if (strcmp(rbuf, "/x") == 0) //클라이언트가 서버를 나가고 싶다면 “/x”
183. 			{
184. 				SEND LEAVE SERVER MSG
185. 				sock[User->socket_number] = INVALID; //해당소켓을 비워주고
186. 				room_count[User->room_number – 1]--; //해당 방 인원수 - 1
187. 				if (room_count[User->room_number - 1] == 0) //해당 방 인원수가 0이면
188. 				{
189. 					create_flag[User->room_number-1] = -1;//만들어도 되는 방 표시
190. 					create_number--; //생성된 방 개수 - 1
191. 					if (create_number == 0)
192. 					{
193. 						createFlag = 0; //생성된 방 개수가 0이면 표시
194. 					}
195. 				}
196. 				for (int a = 0; a < MAX_CLIENT; a++)
197. 				{
198. 					for (int b = 0; b < MAX_CLIENT; b++)
199. 					{
200. 						if (equal_number[a][b] == User->socket_number)
201. 						{
202. 							equal_number[a][b] = -1;
203. 
204. 						}
205. 					}
206. 				} // 해당 소켓넘버를 같은 방 번호 끼리로 Mapping된 매트릭스에서 없애줌
207. 				RoomList_Copy();
208. 				return -1;
209. 			}
210. 			if (rlen == 0 || *pT == INVALID_SOCKET)
211. 			{
212. 				for (int i = 0; i < TOTAL_CLIENT; i++)
213. 				{
214. 					if (sock[i] == *pT)
215. 					{
216. 						closesocket(sock[i]);
217. 					}
218. 				}
219. 				printf("Error in socket(): %ld\n", WSAGetLastError());
220. 				WSACleanup();
221. 				return -1;
222. 			}
223. 			printf("recv msg : %s\n", rbuf);
224. 			EnterCriticalSection(User->pCS);
225. 			strcat(Nickname[User->socket_number], rbuf);
226. 			itoa((User->socket_number+1) % 10, flagBuf, 10);
227. 			strcat(flagBuf, Nickname[User->socket_number]); // NICKNAME 및 필요한 정보를 붙임
228. 
229. 			for (int i = 0; i < MAX_CLIENT; i++)
230. 			{
231. 			  if ((sock[i] != INVALID) && strcmp(rbuf, "/q") != 0) //&& (sock[i] != *pT)
232. 			  {
233. 		                 if(equal_number[User->room_number-1][i] != User->socket_number 
234.                                        && equal_number[User->room_number - 1][i] != -1) 
235.                                     // Equal Matrix에는 같은 room num을 가진 소켓 넘버들이 같은 행에 매핑 되어있음
236. 			     {
237. 			      send(sock[equal_number[User->room_number - 1][i]], flagBuf, strlen(flagBuf), 0);
238. 			      printf("send msg : %s\n", rbuf);
239. 			     }
240. 			  }
241. 			}
242. 			strcpy(Nickname[User->socket_number], NickTemp[User->socket_number]);
243. 			LeaveCriticalSection(User->pCS);
244. 		}
245. 	}
246. 	closesocket(*pT);
247. 	return 0;
248. }
```

* Server 3) Thread_room
```
1. DWORD WINAPI Thread_room(LPVOID pParam) // Equal matrix를 mapping하는 스레드
2. {
3. 	user *User;
4. 	User = (user*)pParam;
5. 	static int j = 0;
6. 	for (int i = 1; i < TOTAL_CLIENT; i++)
7. 	{
8. 		if (User->room_number == i)
9. 		{
10. 			if (equal_number[i - 1][0] == -1 && i != 1) 
11.                                 //Room num 1은 0행. 2는 1행 ... (오름차 순)
12. 			{
13. 				j = 0;
14. 			}
15. 			equal_number[(User->room_number) - 1][j] = User->socket_number; 
16.                               //같은 Room num이면 같은 행에 매핑
17. 			j++;
18. 		}
19. 	}
20. 	return 0;
21. }
```

#### STRUCTURE 설계
```
Structure USER

- LPVOID pParam
  : SOCKET t, Client가 접속 할 때 마다 일대일 대응되는 서버소켓이 만들어 지는데 스레드 파라미터로 넘겨주기 위해 구조체 멤버변수로 선언한다.

- int room_number
  : Client가 원하는 방 번호가 어디인지 알아야 하며, 오로지 같은 방 번호 끼리만 통신이 되어야하므로 room_number를 포함한 구조체를 스레드 파라미터로 넘겨준다. 

- int socket_number
  : 현재 자신이 몇 번 소켓인지에 대한정보를 스레드 파라미터로 넘겨준다.

- CRITICAL_SECTION* pCS
  : 스레드끼리 서로 영역을 침범하지 않아야하는 곳들을 CRITICAL SECTION으로 막아 줘야하기 때문에 스레드 파라미터로 넘겨준다.

```

#### ERROR 처리

* INVAILD_SOCKET : 받아온 SOCKET이 INVALID일 


```
if (*pT == INVALID_SOCKET) {
 printf(%d\n", WSAGetLastError());
 WSACleanup();  	      
 return -1;
 }
 
 if (rlen > 0 &&
 *pT != INVALID_SOCKET)
{
...
}

if (t,s == INVALID_SOCKET)
{
 printf(%d\n",WSAGetLastError());	      
 WSACleanup();  	      
 return -1;
}
```

* 더 이상 방을 만들 수 없는데 방 생성 명령어를 입력했을 경우


```
if (create_number == MAX_CLIENT)
{
    create_number--; //방 개수를 하나 줄인다.
    create_flag[User->room_number - 1] = -1; //그 방 번호는 생성 가능한 방(-1)이라고 표시
    room_count[User->room_number – 1]--; //해당 방의 현재원 수를 하나 줄인다.
    strcpy(print_client[0], ".You can not make a room anymore.. Please enter the room : ");
    send(sock[User->socket_number], print_client[0], strlen(print_client[0]), 0);
    continue;
}
```

* Client가 닉네임 입력 시에 제한된 닉네임 길이보다 긴 닉네임 혹은 중복된 닉네임을 입력


```
while (1)
{
	char Nick = recv(sock[i], Nick_buf, MAX_BUF, 0); //닉네임을 받는다.
	if (strlen(Nick_buf) <= Nick_length) //닉네임 길이가 제한된 길이보다 짧으면
	{
		strcpy(NickBuffer[i], Nick_buf); //닉네임 버퍼에 그 닉네임을 넣는다.
	}
	else if (strlen(Nick_buf) > Nick_length) // 길면
	{
		while (1)
              	{
			strcpy(print_client[6], ".Your nickname is too long. Please re-enter : "); 
			send(sock[i], print_client[6], strlen(print_client[6]), 0); //재입력 메시지 전송
			char Nick_buf_[MAX_BUF] = { 0 };
			char Nick_ = recv(sock[i], Nick_buf_, MAX_BUF, 0); //다시 입력받음
			if (strlen(Nick_buf_) >= Nick_length) //그래도 길면
			{
				continue; //짧을 때까지 반복
			}
			else //짧아지면
			{
				strcpy(NickBuffer[i], Nick_buf_); //그 닉네임을 버퍼에넣고
				strcpy(Nick_buf, Nick_buf_); //버퍼를 옮긴다.
				break;
			}
		}
	}
	for (int a = 0; a < TOTAL_CLIENT; a++) 
	{
		if (strcmp(NickBuffer[a], Nick_buf) == 0 && a != I) //버퍼에 있는 닉네임과 받은 닉네임이 동일하면
		{
			NickOverlapFlag = 1; //중복 플래그 세움
			strcpy(print_client[4], ".The nickname is a duplicate. Input Re-enter :");
			send(sock[i], print_client[4], strlen(print_client[4]), 0); // 닉네임 재입력 메시지 전송
		}
	}
	if (NickOverlapFlag == 1) //중복이면
	{
		NickOverlapFlag = 0; 
		continue; //0으로만든후 다시 반복
	}
	else //중복이 아니면
	{
		strcpy(Nickname[i], Nick_buf); //그 닉네임을 사용
		break;
	}
}
```

* Client가 방이 없는데 방 입장을 선택한 경우

```
if (createFlag == 0)
{
    strcpy(print_client[8], ".No room created. You have to make a room. (Input : '/c') : ");
    send(sock[i], print_client[8], strlen(print_client[8]), 0); //방 생성을 하라는 메시지를 보내고
    continue; //다시 위로 올라가 반복한다.
}
```

* 현재 없는 방 번호를 선택하거나 이상한 문자를 입력 했을 경우

```
int roomNumFlag = 0;
while (1)
{
 char rbuf_4[MAX_BUF] = { 0 };
 char rlen_2 = recv(sock[i], rbuf_4, MAX_BUF, 0);
 if (strcmp(rbuf_4, "0") == 0 || (strcmp(rbuf_4, "0") != 0 && atoi(rbuf_4) == 0)) 
//받은 방번호가 0이거나 0이 아니면서 동시에 atoi()했을 때 0일경우 즉 문자 일 경우
 {
	strcpy(print_client[7], ".That is Invalid number. Please re-enter : "); 
	send(sock[i], print_client[7], strlen(print_client[7]), 0); 
	continue; //잘못된 방 번호라고 전송 후 재 반복
 }
 if (create_flag[atoi(rbuf_4) - 1] == -1) //채팅방이 생성 되어있지 않은 방 번호 일 경우
 {
   while (1)
   {
        strcpy(print_client[7], ".There is no room opened by that number.Please re-enter : ");
        send(sock[i], print_client[7], strlen(print_client[7]), 0); //재입력 메시지 전송
        char rbuf_3[MAX_BUF] = { 0 };
        char rlen_3 = recv(sock[i], rbuf_3, MAX_BUF, 0); // 재입력
        if (create_flag[atoi(rbuf_3) - 1] == -1 || strcmp(rbuf_3, "0") == 0 || (strcmp(rbuf_3, "0") != 0 && atoi(rbuf_3) == 0))
        //다시 받았는데 생성된 방이 아닐 경우, 방 번호가 0일 경우, 방 번호가 문자 일 경우
        {
	continue; //재 반복
        }
        else //옳은 번호이면
        {
	roomNumFlag = 1; // Room number를 버퍼 3에 받았다는 걸 표시하는 flag
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
```

* 하나의 방이 가득 찼을 때 들어가려고 시도하는 경우

```
if (room_count[User->room_number - 1] == MAX_CLIENT+1)
{
	room_count[User->room_number – 1]--; //현재원수를 다시 - 1
	for (int a = 0; a < MAX_CLIENT; a++)
	{
		for (int b = 0; b < MAX_CLIENT; b++)
		{
			if (equal_number[a][b] == User->socket_number)
			{
				equal_number[a][b] = -1; //소켓넘버 매트릭스를 초기화
			}
		}
	}
	strcpy(print_client[0], ".The room was full. Please Re-enter or Create");
	send(sock[User->socket_number], print_client[0], strlen(print_client[0]), 0); 
            //해당 방이 가득차서 방을 만들거나 다른 방으로 입장하라는 메시지 전송
	continue;
}
```


* 방 생성 ‘/c’ 혹은 방 입장 ‘/e’ 커맨드 이외의 문자를 입력한 경우

```
...

else
{
 strcpy(print_client[2], ".Please input / c (Create Chat Room) or / e (Enter Chat Room).");
 send(sock[i], print_client[2], strlen(print_client[2]), 0);
//재입력 메시지 전송
 typing_errorflag = -1; //에러 플래그 세움
 break;
}
break;
}
if (typing_errorflag == -1) 
{
	sock[i] = INVALID; //그 소켓을 INVALID로 만들고
	i--; 
	typing_errorflag = 0;
	continue; //다시 재 반복
}
```

#### Result

<img src = "https://user-images.githubusercontent.com/47768726/60976927-8765cc80-a369-11e9-8c6e-1f74a0d5332d.JPG" width = 70% height = 70%></img>

<img src = "https://user-images.githubusercontent.com/47768726/60976928-8765cc80-a369-11e9-8c46-46ce0386d572.JPG" width = 70% height = 70%></img>

<img src = "https://user-images.githubusercontent.com/47768726/60976929-8765cc80-a369-11e9-8507-ba709de41fb6.JPG" width = 70% height = 70%></img>

<img src = "https://user-images.githubusercontent.com/47768726/60976930-87fe6300-a369-11e9-8cc6-f660df0fe8b9.JPG" width = 70% height = 70%></img>

<img src = "https://user-images.githubusercontent.com/47768726/60976931-87fe6300-a369-11e9-8bb6-921dd612acdf.JPG" width = 70% height = 70%></img>

<img src = "https://user-images.githubusercontent.com/47768726/60976932-87fe6300-a369-11e9-948d-b4c6b032ce43.JPG" width = 70% height = 70%></img>

<img src = "https://user-images.githubusercontent.com/47768726/60976934-87fe6300-a369-11e9-8e6e-71cd27af29d5.JPG" width = 70% height = 70%></img>

<img src = "https://user-images.githubusercontent.com/47768726/60976936-8896f980-a369-11e9-81b7-97f978d1f473.JPG" width = 70% height = 70%></img>

#### 프로젝트 결과물 성능 분석 및 평가
```
1) Client가 새로운 채팅방을 만들거나 참가 할 수 있으며, Client가 사라진 채팅방은 없어진다. 
: 
클라이언트는 새로운 채팅방을 초기에 명령어(‘/c’, ‘/e’)를 사용하여 채팅방을 만들거나 들어 갈 

수 있다. 채팅방을 Create 하기 위해 명령어 /c를 사용하면 room 1번부터 순서대로 방이 만들어

진다. 

(ex) 1,2,3,4,5 번방을 만들고 1,3번방이 사라진 후 에 채팅방을 생성하면 1->3->6 순서로 만들어진다.

 ‘/x’, ‘/q’ 명령어로 서버에서 나가거나 해당 채팅방에서 나갔을 때, 채팅방에 있는 인원이 

0이면 그 채팅방은 사라지도록 구현하였다.

2) Client에게 채팅방 List(방목록 및 Client 수)를 보여준다.
: 
[방 번호]Chatting room(현재 해당 채팅방 인원/ 들어갈 수 있는 인원)의 형식으로 구현하였다.

3) Client는 채팅방을 자유롭게 드나들 수 있으며 서버를 나갈 수도 있어야 한다.

‘/x’, ‘/q’ 명령어로 서버에서 나가거나 해당 채팅방에서 나갈 수 있다.

/x를 입력하면 해당 클라이언트는 종료되고, 소켓은 INVALID로 돌아간다.
/q를 입력하면 해당 채팅방에서 나가면서 그 채팅방의 현재원이 –1이 되고 –1한순간 인원이 0이되면 그채팅방은 사라진다. 채팅방을 나간후 /c나 /e를 통해 방을 생성하거나 다시 들어갈수 있다.

4) Client는 닉네임을 정할 수 있다.

Client가 보낸 메시지를 입력받은 닉네임 + ‘)’에 붙여서 Send하도록 구현하였다.
ex) 21212018)안녕하세요

5) 각 클라이언트의 메시지의 색깔은 다르게 한다.

클라이언트가 메시지를 보내면 서버는 맨 앞에 해당 소켓넘버 정보와 닉네임을 붙여서 다시 클라이언트에게 전송한다. 그러면 클라이언트는 맨 앞의 소켓넘버 정보를 이용해서 각각의 소켓넘버 별로 색깔을 부여 한 후, 맨 앞의 소켓넘버 정보는 버리고 콘솔 창에 출력한다.

```

#### 결론
```
 구현된 프로그램은 핵심기능 및 성능목표에 부합하도록 채팅방에 있는 클라이언트끼리만 채팅을 할 수 있도록 구현하였고, 
 
 닉네임을 정할 수 있도록 구현하였습니다. 
 
 그리고 초기에 채팅방을 만들거나 만들어진 채팅방에 들어 갈 수 있으며, 채팅방에서 나와서 다른 채팅방으로 들어가거나, 다시 채팅방을 만들 수 있도록 구현하였습니다.

 서버를 나가면 해당 클라이언트의 프로그램은 종료되고, 소켓 넘버 별로 다른 색깔의 메시지를 출력하도록 구현하였습니다.
 
 프로젝트를 통해 소켓 네트워크 및 멀티스레드 프로그래밍 설계능력을 갖출 수 있었습니다. 
 
 이번 설계를 해보면서 코딩보다는 알고리즘의 중요성에 대해서 실감 할 수 있었고 더 좋은 알고리즘으로 구현했다면 훨씬 효율적인 프로그램이 됐을 것 같습니다. 
 
 돌발 상황에 대한 에러 처리를 해두었지만 생각지 못한 돌발 상황이 발생할경우가 많이 생길 것 같은데, 프로그래밍의 유지보수나 업데이트가 중요하다는 것을 실감하였습니다.  
```


