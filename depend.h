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


#define READY 1//就绪
#define RUN 2//运行
#define STOP 3//阻塞

class PCB;
extern PCB* run = NULL;
//bool isRun = false;
class SHOW {
private:
	short x, y;
	string dialog;
public:
	SHOW(short x, short y, const string dialog);
	//打印dialog
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
	int priority;		//优先级
	int runTime;		//运行时间
	int state;			//状态
	string runState;
	PCB* next;			//指向下一个PCB
public:
	//构造函数	next一般初始为NULL
	PCB(string processName, int priority, int runTime, int state, PCB* next);
	~PCB();
	void setState(int state);
	//新建的PCB放入就绪队列
	bool putReady(vector<PCB*>& ready, vector<PCB*>& stop);//false为队列已满
	bool stopToReady(vector<PCB*>& stop, vector<PCB*>& ready);
	int putStop(vector<PCB*>& ready, vector<PCB*>& stop, int former);//放入阻塞队列 0 队列已满
	bool deletePCB(vector<PCB*>& s);//从向量s中删除PCB
	/*static bool deletePCB(vector<PCB*>& ready, vector<PCB*>& stop, int state) {
		vector<PCB*>& s = ;
	}*/
	bool putRun(vector<PCB*>& s);//将已有的进程放入运行状态
	bool lookForward(vector<PCB*>& s);
	static int lookForward(vector<PCB*>& s, string processName) {//0 不存在
		//返回处于的状态
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
