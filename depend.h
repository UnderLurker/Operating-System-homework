#pragma once
#include<iostream>
#include<string>
#include<vector>
#include<queue>
#include<cstdlib>
#include<Windows.h>
#include<algorithm>
#include<sstream>
using namespace std;


#define READY 1//����
#define RUN 2//����
#define STOP 3//����

class PCB;
extern PCB* run = NULL;
//bool isRun = false;
class SHOW {
private:
	short x, y;
	string dialog;
public:
	SHOW(short x, short y, const string dialog);
	//��ӡdialog
	void gotoxy();
	static void gotoxy(short x, short y, string infor) {
		HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
		COORD p;
		p.X = x;
		p.Y = y;
		SetConsoleCursorPosition(handle, p);
		cout << infor;
	}
};

class PCB {
private:
	string processName;
	int priority;		//���ȼ�
	int runTime;		//����ʱ��
	int state;			//״̬
	string runState;
	PCB* next;			//ָ����һ��PCB
public:
	//���캯��	nextһ���ʼΪNULL
	PCB(string processName, int priority, int runTime, int state, PCB* next);
	~PCB();
	void setState(int state);
	//�½���PCB�����������
	bool putReady(vector<PCB*>& ready, vector<PCB*>& stop);//falseΪ��������
	bool stopToReady(vector<PCB*>& stop, vector<PCB*>& ready);
	int putStop(vector<PCB*>& ready, vector<PCB*>& stop, int former);//������������ 0 ��������
	bool deletePCB(vector<PCB*>& s);//������s��ɾ��PCB
	/*static bool deletePCB(vector<PCB*>& ready, vector<PCB*>& stop, int state) {
		vector<PCB*>& s = ;
	}*/
	bool putRun(vector<PCB*>& s);//�����еĽ��̷�������״̬
	bool lookForward(vector<PCB*>& s);
	static int lookForward(vector<PCB*>& s, string processName) {//0 ������
		//���ش��ڵ�״̬
		//if (run->processName == processName) return RUN;
		if (s.empty()) return 0;
		PCB* temp = s.front();
		while (temp != NULL) {
			if (temp->processName == processName)
				return temp->state;
			temp = temp->next;
		}
		return 0;
	}
	string getName();
	int getState();
	string toString(int count=0);
	PCB* getNext();
	void setNext(PCB* s);
};
