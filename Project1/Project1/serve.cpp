#include <cstdio>
#include<iostream>
#include<string>
#include<WinSock2.h>

#include <algorithm>
#include <vector>
#pragma comment(lib,"ws2_32.lib")
using namespace std;
const int PORT = 8000;
int portArr[10];
int portArrLength = -1; // 端口数组
int gameGoon = 1; //判断游戏是否进行
int gameInitialization = 0; //判断游戏是否初始化，是否已经发牌
#define MaxClient 10
#define MaxBufSize 1024
#define _CRT_SECURE_NO_WARINGS

// 发牌结构体
struct CardShare {
	int cardArr[15]; // 发配牌数组
	int number; // 玩家编号
};

// function
vector<int> randperm();

//服务线程
DWORD WINAPI ServerThread(LPVOID lpParameter) {
	SOCKET* ClientSocket = (SOCKET*)lpParameter;
	int receByt = 0;
	char RecvBuf[MaxBufSize];
	char SendBuf[MaxBufSize];
	while (1) {
		if (gameInitialization == 0) {
			// 游戏初始化即发牌
			vector<int> temp = randperm();
			char tranChar[100];
			for (int i = 1; i <= 3; i++) {
				CardShare p1;
				p1.number = i - 1;
				for (int j = 0; j <= 12; j++) {
					p1.cardArr[j] = temp[(i - 1) * 13 + j]; //发牌
				}
				send(portArr[i - 1], (char*)&p1, sizeof(CardShare), 0);
				cout << "发牌成功" << portArr[i - 1] << endl;
			}
			gameInitialization = 1;
		}
		receByt = recv(*ClientSocket, RecvBuf, sizeof(RecvBuf), 0);
		//buf[receByt]='\0';
		if (receByt > 0) {
			cout << "接收到的消息是：" << RecvBuf << "            来自客户端:" << *ClientSocket << endl;
			// 向其他成员发送消息，以达到消息共享
			for (int i = 0; i <= portArrLength; i++) {
				if (portArr[i] != *ClientSocket) {
					cout << portArr[i];
					if (send(portArr[i], RecvBuf, sizeof(RecvBuf), 0) < 0) {
						cout << "发送失败" << endl;
					}
				}
			}
			memset(SendBuf, 0, sizeof(SendBuf));
		}
		else {
			cout << "接收消息结束！" << endl;
			break;
		}
		//memset(RecvBuf, 0, sizeof(RecvBuf));
		cout << "请输入要发送到客户端的信息：" << endl;
		//gets_s(SendBuf);
		//int k = 0;
		//k = send(*ClientSocket, "收到", sizeof("收到"), 0);
		if (send(*ClientSocket, "收到", sizeof("收到"), 0)< 0) {
			cout << "发送失败" << endl;
		}
		//memset(SendBuf, 0, sizeof(SendBuf));
	}//while
	closesocket(*ClientSocket);
	free(ClientSocket);
	return 0;
}

int main() {
	WSAData wsd;
	WSAStartup(MAKEWORD(2, 2), &wsd);
	SOCKET ListenSocket = socket(AF_INET, SOCK_STREAM, 0);
	SOCKADDR_IN ListenAddr;
	ListenAddr.sin_family = AF_INET;
	ListenAddr.sin_addr.S_un.S_addr = INADDR_ANY;//表示填入本机ip
	ListenAddr.sin_port = htons(PORT);
	int n;
	n = bind(ListenSocket, (LPSOCKADDR)&ListenAddr, sizeof(ListenAddr));
	if (n == SOCKET_ERROR) {
		cout << "端口绑定失败！" << endl;
		return -1;
	}
	else {
		cout << "端口绑定成功：" << PORT << endl;
	}
	int l = listen(ListenSocket, 20);
	cout << "服务端准备就绪，等待连接请求" << endl;
	while (1) {
		if (!gameGoon)
			break;
		//循环接收客户端连接请求并创建服务线程
		SOCKET* ClientSocket = new SOCKET;
		ClientSocket = (SOCKET*)malloc(sizeof(SOCKET));
		//接收客户端连接请求
		int SockAddrlen = sizeof(sockaddr);
		*ClientSocket = accept(ListenSocket, 0, 0);
		portArr[++portArrLength] = *ClientSocket;
		cout << portArr[portArrLength];
		cout << "一个客户端已连接到服务器，socket是：" << *ClientSocket << endl;
		CreateThread(NULL, 0, &ServerThread, ClientSocket, 0, NULL);
	}
	closesocket(ListenSocket);
	WSACleanup();
	return(0);
}//main




vector<int> randperm()
{
	vector<int> temp;
	for (int i = -2; i < 13; ++i) {
		if (i > 0) {
			for (int t = 0; t < 4; t++) // 1 -13 每种有4张
				temp.push_back(i + 1); // -1 0 表示大王和小王
		}
		else
			temp.push_back(i + 1); // -1 0 表示大王和小王
	}
	random_shuffle(temp.begin(), temp.end());
	return temp;
}
