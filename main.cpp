#define _CRT_SECURE_NO_WARNINGS

#include"depend.h"
//#include<iostream>
//#include<string>
//#include<vector>
//#include<queue>
//#include<cstdlib>
//#include<Windows.h>
//#include<algorithm>
//#include<sstream>
//using namespace std;
//
//
//#define READY 1//����
//#define RUN 2//����
//#define STOP 3//����
//class PCB;
//PCB* run = NULL;
////bool isRun = false;
//class SHOW{
//private:
//	short x,y;
//	string dialog;
//public:
//	SHOW(short x,short y,const string dialog){
//		this->x=x;
//		this->y=y;
//		this->dialog=dialog;
//	}
//	//��ӡdialog
//	void gotoxy(){
//		HANDLE handle=GetStdHandle(STD_OUTPUT_HANDLE);
//		COORD p;
//		p.X=this->x;
//		p.Y=this->y;
//		SetConsoleCursorPosition(handle,p);
//		cout<<dialog;
//	}
//	static void gotoxy(short x, short y,string infor) {
//		HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
//		COORD p;
//		p.X = x;
//		p.Y = y;
//		SetConsoleCursorPosition(handle, p);
//		cout << infor;
//	}
//};
//
//class PCB{
//private:
//	string processName;
//	int priority;		//���ȼ�
//	int runTime;		//����ʱ��
//	int state;			//״̬
//	PCB* next;			//ָ����һ��PCB
//public:
//	//���캯��	nextһ���ʼΪNULL
//	PCB(string processName,int priority,int runTime,int state,PCB* next){
//		//runTime��λΪs
//		this->processName=processName;
//		this->priority=priority;
//		this->runTime = runTime;
//		this->state=state;
//		this->next=next;
//	}
//	~PCB() {
//		SHOW::gotoxy(25, 10, "�����Ѿ��ͷ�.");
//	}
//	void setState(int state) {
//		this->state = state;
//	}
//	//�½���PCB�����������
//	bool putReady(vector<PCB*>& ready, vector<PCB*>& stop) {//falseΪ��������
//		this->state = READY;
//		if (ready.size() == 0) ready.push_back(this);
//		else if (ready.size() == 10||lookForward(ready)||lookForward(stop)) return false;
//		else {
//			ready.back()->next = this;
//			ready.push_back(this);
//		}
//		return true;
//	}
//	bool stopToReady(vector<PCB*>& stop,vector<PCB*>& ready) {
//		if (stop.empty()) return false;
//		int i;
//		this->state = READY;
//		vector<PCB*>::iterator it = stop.begin();
//		for (i = 0; it != stop.end(); it++, i++) {
//			if (stop[i]->processName == this->processName) {
//				if (i - 1 >= 0 && i + 1 < stop.size())
//					stop[i - 1]->next = stop[i]->next;
//				else if (i - 1 < 0 && i + 1 < stop.size())
//					this->next = NULL;
//				else if (i - 1 >= 0 && i + 1 >= stop.size())
//					stop[i - 1]->next = NULL;
//				if (!ready.empty()) ready.back()->next = this;
//				ready.push_back(this);
//				stop.erase(it);
//				return true;
//			}
//		}
//		return false;
//	}
//	int putStop(vector<PCB*>& ready,vector<PCB*>& stop,int former) {//������������ 0 ��������
//		this->state = STOP;
//		if (stop.size() == 10) return 0;
//		if (former == READY) {
//			int i = 0;
//			for (vector<PCB*>::iterator it = ready.begin(); it != ready.end(); it++,i++) {
//				if (ready[i]->processName == this->processName) {
//					if (i - 1 >= 0 && i + 1 < ready.size())
//						ready[i - 1]->next = ready[i]->next;
//					else if (i - 1 < 0 && i + 1 < ready.size())
//						this->next = NULL;
//					else if (i - 1 >= 0 && i + 1 >= ready.size())
//						ready[i - 1]->next = NULL;
//					if (!stop.empty()) stop.back()->next = this;
//					stop.push_back(this);
//					ready.erase(it);
//					break;
//				}
//			}
//		}
//		if (former == RUN) {
//			if (!ready.empty())
//				stop.back()->next = run;
//			stop.push_back(run);
//			run = NULL;
//		}
//		if (former == STOP) {
//			if (!stop.empty())
//				stop.back()->next = this;
//			stop.push_back(this);
//		}
//		return STOP;
//	}
//	bool deletePCB(vector<PCB*>& s) {//������s��ɾ��PCB
//		if (s.empty()) return false;
//		int i;
//		vector<PCB*>::iterator it = s.begin();
//		for (i = 0; it != s.end(); it++, i++) {
//			if (s[i]->processName == this->processName) {
//				if (i - 1 >= 0 && i + 1 < s.size())
//					s[i - 1]->next = s[i]->next;
//				else if (i - 1 < 0 && i + 1 < s.size())
//					this->next = NULL;
//				else if (i - 1 >= 0 && i + 1 >= s.size())
//					s[i - 1]->next = NULL;
//				s.erase(it);
//				return true;
//			}
//		}
//		return false;
//	}
//	/*static bool deletePCB(vector<PCB*>& ready, vector<PCB*>& stop, int state) {
//		vector<PCB*>& s = ;
//	}*/
//	bool putRun(vector<PCB*>& s) {//�����еĽ��̷�������״̬
//		if (s.empty()) return false;
//		int i;
//		vector<PCB*>::iterator it = s.begin();
//		for (i = 0; it != s.end(); it++, i++) {
//			if (s[i]->processName == this->processName) {
//				if (i - 1 >= 0 && i + 1 < s.size())
//					s[i - 1]->next = s[i]->next;
//				else if (i - 1 < 0 && i + 1 < s.size())
//					this->next = NULL;
//				else if (i - 1 >= 0 && i + 1 >= s.size())
//					s[i - 1]->next = NULL;
//				run = s[i];
//				s.erase(it);
//				return true;
//			}
//		}
//		return false;
//	}
//	bool lookForward(vector<PCB*>& s) {
//		if (s.empty()) return false;
//		PCB* temp = s.front();
//		while (temp != NULL) {
//			if (temp->processName == this->processName)
//				return true;
//			temp = temp->next;
//		}
//		return false;
//	}
//	static int lookForward(vector<PCB*>& s,string processName) {//0 ������
//		//���ش��ڵ�״̬
//		//if (run->processName == processName) return RUN;
//		if (s.empty()) return 0;
//		PCB* temp = s.front();
//		while (temp != NULL) {
//			if (temp->processName == processName)
//				return temp->state;
//			temp = temp->next;
//		}
//		return 0;
//	}
//	string getName() {
//		return this->processName;
//	}
//	int getState() {
//		return this->state;
//	}
//	string toString() {
//		string ans = processName+'\t'+'\t', s;
//		stringstream ss1,ss2,ss3;
//		ss1 << priority;
//		ss1 >> s;
//		ans += s + '\t';
//		ss2 << runTime;
//		ss2 >> s;
//		ans += s + '\t'+'\t';
//		ss3 << state;
//		ss3 >> s;
//		ans += s;
//		return ans;
//	}
//	PCB* getNext() {
//		return this->next;
//	}
//	void setNext(PCB* s) {
//		this->next = s;
//	}
//};

void deletePCB(vector<PCB*>& s,string processName) {
	int i = 0;
	for (vector<PCB*>::iterator it = s.begin(); it != s.end(); it++, i++) {
		if (s[i]->getName() == processName) {
			if (i - 1 >= 0 && i + 1 < s.size())
				s[i - 1]->setNext(s[i + 1]);
			else if (i - 1 < 0 && i + 1 < s.size())
				s[i]->setNext(NULL);
			else if (i - 1 >= 0 && i + 1 >= s.size())
				s[i - 1]->setNext(NULL);
			delete s[i];
			s.erase(it);
			break;
		}
	}
}


string menu(){
	vector<string> word(6);
	word[0]="1����������";
	word[1]="2����������";
	word[2]="3����������";
	word[3]="4��������ֹ";
	word[4]="5��������ʾ";
	word[5]="0�������˳�";
	SHOW::gotoxy(31,3,"ϵͳ���˵�");
	short x=30,y=5;
	for(int i=0;i<word.size();i++,y+=2){
		SHOW::gotoxy(x,y,word[i]);
	}
	SHOW::gotoxy(25,y,"����������Ҫ�Ĺ���(0--5)");
	SHOW::gotoxy(30,y+2,"����ѡ�� = ");
	string ans;
	cin>>ans;
	return ans;
}

void fresh(short x, short y, int len) {
	SHOW::gotoxy(x,y,"");
	for (int i = 0; i < len; i++)
		cout << " ";
}

void pageUp(int& count,short& x,short& y) {
	if (count == 10) {
		count %= 10;
		y++;
		SHOW::gotoxy(x, y++, "-------------------------------------------------------");
		SHOW::gotoxy(x, y + 1, "");
		system("pause");
		x = 25, y = 4;
		fresh(0, 0, 3000);
		SHOW::gotoxy(x, y++, "���н���");
		SHOW::gotoxy(x, y++, "-------------------------------------------------------");
		SHOW::gotoxy(x, y++, "���\t��������\t���ȼ�\t����ʱ��\t����״̬");
		SHOW::gotoxy(x, y, "-------------------------------------------------------");
	}
}

int main(){
	vector<PCB*> ready;
	vector<PCB*> stop;
	string input=menu();
	SHOW error(30, 21, "�����������������..");
	while(1){
		if(input=="0")
			return 0;
		else if (input == "1") {//����
			fresh(0,0,3000);
			short x = 25, y = 8;
			int prior, rt, state;
			string processName;
			SHOW in(x, y, "������Ҫ�����Ľ�������"), priority(x, y + 2, "���������ȼ���"),runTime(x,y+4,"����������ʱ�䣺");
			in.gotoxy();
			cin >> processName;
			priority.gotoxy();
			cin >> prior;
			runTime.gotoxy();
			cin >> rt;
			y += 6;
			SHOW::gotoxy(x, y, "1-����\t2-ִ��\t3-����");
			y += 2;
			SHOW::gotoxy(x, y, "����������״̬��");
			y += 2;
			cin >> state;
			if (state <= 0 || state > 3) {
				SHOW::gotoxy(x, y, "û�и�״̬");
				goto end;
			}
			if (state == RUN && run) SHOW::gotoxy(x, y, "�Ѿ��г���������״̬,�����ԡ�");
			else if (state == RUN && run == NULL&&!PCB::lookForward(ready,processName)&&!PCB::lookForward(stop,processName)) { 
				run = new PCB(processName, prior, rt, RUN, NULL);
				SHOW::gotoxy(x, y, "���̴����ɹ�!!!");
				run->setState(RUN);
			}
			else {
				PCB* create=new PCB(processName, prior, rt, state, NULL);
				if ((run && (run->getName() == processName))||(state==RUN&&run!=NULL))
					SHOW::gotoxy(x, y, "����ͬ����������ִ�У�����");
				else if ((state == READY&&create->putReady(ready, stop))||(state==STOP&&create->putStop(ready,stop,STOP)))
					SHOW::gotoxy(x, y, "���̴����ɹ�!!!");
				else SHOW::gotoxy(x, y, "����������ͬ�����̣�����");
			}
			end:y += 2;
			SHOW::gotoxy(x, y, "");
			system("pause");
			goto refreash;
		}
		else if (input == "2") {//����
			short x = 25, y = 8;
			string processName;
			fresh(0,0,3000);
			SHOW::gotoxy(x, y, "������Ҫ�����Ľ������ƣ�");
			y += 2;
			cin >> processName;
			int readyState = PCB::lookForward(ready, processName), stopState = PCB::lookForward(stop, processName);
			if (readyState || stopState || (run && run->getName() == processName)) {
				if (run && run->getName() == processName) {
					run->putStop(ready, stop, RUN);
					SHOW::gotoxy(x, y, "�ѽ����еĽ���Ų����������");
					y += 2;
					if (ready.empty()) SHOW::gotoxy(x, y, "���������ѿգ�û�н�������ִ�С�");
					else {//���������е�һ����Ϊ����״̬
						run = ready[0];
						run->setNext(NULL);
						run->setState(RUN);
						vector<PCB*>::iterator it = ready.begin();
						ready.erase(it);
					}
				}
				else if (stopState == STOP) SHOW::gotoxy(x, y, "�ý��̴�������״̬����");
				else if (readyState == READY) SHOW::gotoxy(x, y, "�ý��̴��ھ���״̬����");
			}
			else SHOW::gotoxy(x, y, "û�иý��̡�");
			y += 2;
			SHOW::gotoxy(x, y, "");
			system("pause");
			goto refreash;
		}
		else if (input == "3") {//����
			short x = 25, y = 8;
			fresh(0,0,3000);
			if (stop.empty()) SHOW::gotoxy(x, y, "��������Ϊ�գ�����ִ�д˲���");
			else if(ready.size()==10) SHOW::gotoxy(x, y, "������������������ִ�д˲���");
			else {
				string processName;
				SHOW::gotoxy(x, y, "������Ҫ���ѵĽ������ƣ�");
				y += 2;
				cin >> processName;
				int readyState = PCB::lookForward(ready, processName), stopState = PCB::lookForward(stop, processName);
				if (readyState || stopState || (run && run->getName() == processName)) {
					if (run && run->getName() == processName)
						SHOW::gotoxy(x, y, "�ý����Ѿ���������״̬");
					else if (stopState == STOP) {
						stop.back()->stopToReady(stop, ready);
						SHOW::gotoxy(x, y, "�ѽ��ý����������״̬����");
					}
					else if (readyState == READY) SHOW::gotoxy(x, y, "�ý����Ѵ��ھ���״̬����");
				}
				else SHOW::gotoxy(x, y, "û�иý��̡�");
			}
			y += 2;
			SHOW::gotoxy(x, y, "");
			system("pause");
			goto refreash;
		}
		else if (input == "4") {//��ֹ
			short x = 25, y = 8;
			fresh(0,0,3000);
			string processName;
			SHOW::gotoxy(x, y, "������Ҫ�����Ľ������ƣ�");
			y += 2;
			cin >> processName;
			int readyState = PCB::lookForward(ready, processName),
				stopState = PCB::lookForward(stop, processName);
			if (run && run->getName() == processName) {
				delete run;
				run->setNext(NULL);
				SHOW::gotoxy(x, y, "�ý���Ϊ����״̬���Ѿ�������");
				y += 2;
				if (ready.empty()) SHOW::gotoxy(x, y, "���������ѿգ�û�н�������ִ�С�");
				else {//���������е�һ����Ϊ����״̬
					run = ready[0];
					run->setNext(NULL);
					run->setState(RUN);
					vector<PCB*>::iterator it = ready.begin();
					ready.erase(it);
					SHOW::gotoxy(x, y, run->getName() + " ����ִ�С�");
				}
			}
			else if (readyState == READY) {
				deletePCB(ready, processName);
				SHOW::gotoxy(x, y, "�ý���Ϊ����״̬���Ѿ�������");
			}
			else if (stopState == STOP) {
				deletePCB(stop, processName);
				SHOW::gotoxy(x, y, "�ý���Ϊ����״̬���Ѿ�������");
			}
			else SHOW::gotoxy(x, y, "û�иý���");
			y += 2;
			SHOW::gotoxy(x, y, "");
			system("pause");
			goto refreash;
		}
		else if (input == "5") {//��ʾ
			short x = 25, y = 4;
			int count = 0;
			fresh(0,0,3000);
			SHOW::gotoxy(x, y++, "���н���");
			SHOW::gotoxy(x, y++, "-------------------------------------------------------");
			SHOW::gotoxy(x, y++, "���\t��������\t���ȼ�\t����ʱ��\t����״̬");
			SHOW::gotoxy(x, y++, "-------------------------------------------------------");
			if (run) {
				count++;
				SHOW::gotoxy(x, y++, run->toString(count));
			}
			for (int i = 0; i < ready.size(); i++, y++) {
				count++;
				SHOW::gotoxy(x, y, ready[i]->toString(count));
				pageUp(count, x, y);
			}
			for (int i = 0; i < stop.size(); i++, y++) {
				count++;
				SHOW::gotoxy(x, y, stop[i]->toString(count));
				pageUp(count, x, y);
			}
			SHOW::gotoxy(x, y++, "-------------------------------------------------------");
			SHOW::gotoxy(x, y + 1, "");
			system("pause");
			goto refreash;
		}
		else{
			error.gotoxy();
			SHOW::gotoxy(30, 23, "");
			system("pause");
			refreash:fresh(0, 0, 3000);
			input=menu();
		}
	}
	return 0;
}