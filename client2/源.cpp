//#include "Card.h"
#include <cstdio>
#include<iostream>
#include<string>
#include<WinSock2.h>
#include <Ws2tcpip.h>  
#pragma comment(lib,"ws2_32.lib")
using namespace std;
const int PORT = 8000;
#define MaxBufSize 1024
#define _CRT_SECURE_NO_WARINGS

// ���ƽṹ��
struct CardShare {
	int cardArr[15]; // ����������
	int number; // ��ұ��
};


int main() {
	WSADATA wsd;
	int i = 1; // �ж��Ƿ��Ѿ�����

	WSAStartup(MAKEWORD(2, 2), &wsd);
	SOCKET SocketClient = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	SOCKADDR_IN  ClientAddr;

	ClientAddr.sin_family = AF_INET;
	ClientAddr.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");
	ClientAddr.sin_port = htons(PORT);
	int n = 0;
	n = connect(SocketClient, (struct sockaddr*)&ClientAddr, sizeof(ClientAddr));
	if (n == SOCKET_ERROR) {
		cout << "����ʧ��" << endl;
		return -1;
	}
	cout << "�Ѿ����ӵ��������������������������Ϣ�ˣ�" << endl;
	char info[1024], SendBuff[MaxBufSize], RecvBuff[MaxBufSize];
	while (1) {
		if (i==1) {
			char temp[100];
			CardShare p2;
			memset(temp, 0, sizeof(temp));
			recv(SocketClient, temp, 101, 0);
			memcpy(&p2, temp, sizeof(CardShare));
			cout << p2.cardArr[0] << endl;
			i = 0;
		}
		cout << "������Ҫ���͵���Ϣ,���س��������ͣ�" << endl;
		gets_s(info);
		//if (info[0] == '\0')
		//	break;
		strcpy_s(SendBuff, info);
		memset(info, 0, sizeof(info));
		int k = 0;
		n = send(SocketClient, SendBuff, sizeof(SendBuff), 0);
		memset(SendBuff, 0, sizeof(SendBuff));
		if (k < 0) {
			cout << "����ʧ��" << endl;
		}
		Sleep(500);
		int n = 0;
		n = recv(SocketClient, RecvBuff, sizeof(RecvBuff), 0);
		if (n > 0) {
			cout << "���յ����Է���������ϢΪ��" << RecvBuff << endl;
		}
		memset(RecvBuff, 0, sizeof(RecvBuff));
	}
	closesocket(SocketClient);
	WSACleanup();
	return 0;
}