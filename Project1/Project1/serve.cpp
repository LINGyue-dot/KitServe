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
int portArrLength = -1; // �˿�����
int gameGoon = 1; //�ж���Ϸ�Ƿ����
int gameInitialization = 0; //�ж���Ϸ�Ƿ��ʼ�����Ƿ��Ѿ�����
#define MaxClient 10
#define MaxBufSize 1024
#define _CRT_SECURE_NO_WARINGS

// ���ƽṹ��
struct CardShare {
	int cardArr[15]; // ����������
	int number; // ��ұ��
};

// function
vector<int> randperm();

//�����߳�
DWORD WINAPI ServerThread(LPVOID lpParameter) {
	SOCKET* ClientSocket = (SOCKET*)lpParameter;
	int receByt = 0;
	char RecvBuf[MaxBufSize];
	char SendBuf[MaxBufSize];
	while (1) {
		if (gameInitialization == 0) {
			// ��Ϸ��ʼ��������
			vector<int> temp = randperm();
			char tranChar[100];
			for (int i = 1; i <= 3; i++) {
				CardShare p1;
				p1.number = i - 1;
				for (int j = 0; j <= 12; j++) {
					p1.cardArr[j] = temp[(i - 1) * 13 + j]; //����
				}
				send(portArr[i - 1], (char*)&p1, sizeof(CardShare), 0);
				cout << "���Ƴɹ�" << portArr[i - 1] << endl;
			}
			gameInitialization = 1;
		}
		receByt = recv(*ClientSocket, RecvBuf, sizeof(RecvBuf), 0);
		//buf[receByt]='\0';
		if (receByt > 0) {
			cout << "���յ�����Ϣ�ǣ�" << RecvBuf << "            ���Կͻ���:" << *ClientSocket << endl;
			// ��������Ա������Ϣ���Դﵽ��Ϣ����
			for (int i = 0; i <= portArrLength; i++) {
				if (portArr[i] != *ClientSocket) {
					cout << portArr[i];
					if (send(portArr[i], RecvBuf, sizeof(RecvBuf), 0) < 0) {
						cout << "����ʧ��" << endl;
					}
				}
			}
			memset(SendBuf, 0, sizeof(SendBuf));
		}
		else {
			cout << "������Ϣ������" << endl;
			break;
		}
		//memset(RecvBuf, 0, sizeof(RecvBuf));
		cout << "������Ҫ���͵��ͻ��˵���Ϣ��" << endl;
		//gets_s(SendBuf);
		//int k = 0;
		//k = send(*ClientSocket, "�յ�", sizeof("�յ�"), 0);
		if (send(*ClientSocket, "�յ�", sizeof("�յ�"), 0)< 0) {
			cout << "����ʧ��" << endl;
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
	ListenAddr.sin_addr.S_un.S_addr = INADDR_ANY;//��ʾ���뱾��ip
	ListenAddr.sin_port = htons(PORT);
	int n;
	n = bind(ListenSocket, (LPSOCKADDR)&ListenAddr, sizeof(ListenAddr));
	if (n == SOCKET_ERROR) {
		cout << "�˿ڰ�ʧ�ܣ�" << endl;
		return -1;
	}
	else {
		cout << "�˿ڰ󶨳ɹ���" << PORT << endl;
	}
	int l = listen(ListenSocket, 20);
	cout << "�����׼���������ȴ���������" << endl;
	while (1) {
		if (!gameGoon)
			break;
		//ѭ�����տͻ����������󲢴��������߳�
		SOCKET* ClientSocket = new SOCKET;
		ClientSocket = (SOCKET*)malloc(sizeof(SOCKET));
		//���տͻ�����������
		int SockAddrlen = sizeof(sockaddr);
		*ClientSocket = accept(ListenSocket, 0, 0);
		portArr[++portArrLength] = *ClientSocket;
		cout << portArr[portArrLength];
		cout << "һ���ͻ��������ӵ���������socket�ǣ�" << *ClientSocket << endl;
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
			for (int t = 0; t < 4; t++) // 1 -13 ÿ����4��
				temp.push_back(i + 1); // -1 0 ��ʾ������С��
		}
		else
			temp.push_back(i + 1); // -1 0 ��ʾ������С��
	}
	random_shuffle(temp.begin(), temp.end());
	return temp;
}
