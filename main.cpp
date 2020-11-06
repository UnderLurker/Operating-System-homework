#define _CRT_SECURE_NO_WARNINGS

#include"depend.h"

list<TASK> distribution;//�ѷ�����
list<TASK> unDistribution;//δ������
vector<PCB*> ready;
vector<PCB*> stop;
int length;//ÿ�ش����ʱ��Ƭ����
int manageState = 1;//���̵���״̬
//int startAddress = START_ADDRESS;
//int distributionMax = DISTRIBUTION_MAX;


void connectReady() {//����ready�ڵ�next
	unsigned int i;
	for (i = 0; i < ready.size() - 1; i++)
		ready[i]->setNext(ready[i + 1]);
	ready[i]->setNext(NULL);
}

void deletePCB(vector<PCB*>& s, string processName) {
	unsigned int i = 0;
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

void fresh(short x, short y, int len) {
	SHOW::gotoxy(x, y, "");
	for (int i = 0; i < len; i++)
		cout << " ";
}

string menu() {
	vector<string> word(8);
	word[0] = "1����������";
	word[1] = "2����������";
	word[2] = "3����������";
	word[3] = "4��������ֹ";
	word[4] = "5��������ʾ";
	word[5] = "6����������";
	word[6] = "7��������̬����";
	word[7] = "0�������˳�";
	SHOW::gotoxy(31, 3, "ϵͳ���˵�");
	short x = 30, y = 5;
	for (unsigned int i = 0; i < word.size(); i++, y += 2) {
		SHOW::gotoxy(x, y, word[i]);
	}
	SHOW::gotoxy(25, y, "����������Ҫ�Ĺ���(0--7)");
	SHOW::gotoxy(30, y + 2, "����ѡ�� = ");
	string ans;
	cin >> ans;
	return ans;
}

string menuManage(short x, short y) {
	string input;
	SHOW::gotoxy(x, y, "0���������������˵�");
	y += 2;
	SHOW::gotoxy(x, y, "1�����������ȼ�����");
	y += 2;
	SHOW::gotoxy(x, y, "2��������ʱ��Ƭ����");
	y += 2;
	SHOW::gotoxy(x - 2, y, "��ѡ������Ҫ�Ĺ���ѡ�");
	cin >> input;
	return input;
}

string menuDp(short x, short y) {
	string ans;
	SHOW::gotoxy(x, y, "0�������˳�");
	y += 2;
	SHOW::gotoxy(x, y, "1����������");
	y += 2;
	SHOW::gotoxy(x, y, "2����������");
	y += 2;
	SHOW::gotoxy(x, y, "3��������ʾ");
	y += 2;
	SHOW::gotoxy(x - 4, y, "��ѡ������Ҫ�Ĺ��ܣ�");
	cin >> ans;
	return ans;
}

void processShow(string title, short& x, short& y, void (*p)(short& x, short& y, string title)) {//���
	SHOW::gotoxy(x, y++, title);
	SHOW::gotoxy(x, y++, "-------------------------------------------------------");
	SHOW::gotoxy(x, y++, "���\t��������\t���ȼ�\t����ʱ��\t����״̬");
	SHOW::gotoxy(x, y++, "-------------------------------------------------------");
	p(x, y, title);
	SHOW::gotoxy(x, y++, "-------------------------------------------------------");
	SHOW::gotoxy(x, y, "");
	system("pause");
}

void processPageUp(int& count, short& x, short& y, string title) {
	if (count == 10) {
		count %= 10;
		y++;
		SHOW::gotoxy(x, y++, "-------------------------------------------------------");
		SHOW::gotoxy(x, y + 1, "");
		system("pause");
		x = 25, y = 4;
		fresh(0, 0, 3000);
		SHOW::gotoxy(x, y++, title);
		SHOW::gotoxy(x, y++, "-------------------------------------------------------");
		SHOW::gotoxy(x, y++, "���\t��������\t���ȼ�\t����ʱ��\t����״̬");
		SHOW::gotoxy(x, y++, "-------------------------------------------------------");
	}
}

void distributionPageUp(int& count, short& x, short& y, string title) {
	if (count == 10) {
		count %= 10;
		y++;
		SHOW::gotoxy(x, y++, "-----------------------------\t\t\t----------------------------");
		SHOW::gotoxy(x, y + 1, "");
		system("pause");
		x = 25, y = 4;
		fresh(0, 0, 3000);
		SHOW::gotoxy(x, y++, title);
		SHOW::gotoxy(x, y++, "-----------------------------\t\t\t----------------------------");
		SHOW::gotoxy(x, y++, "���\tʼַ\t����\t״̬\t\t\t��ҵ��\tʼַ\t����\t״̬");
		SHOW::gotoxy(x, y++, "-----------------------------\t\t\t----------------------------");
	}
}

void distributionShow(string title, short& x, short& y, void (*p)(short& x, short& y, string title)) {
	SHOW::gotoxy(x, y++, title);
	SHOW::gotoxy(x, y++, "-----------------------------\t\t\t----------------------------");
	SHOW::gotoxy(x, y++, "���\tʼַ\t����\t״̬\t\t\t��ҵ��\tʼַ\t����\t״̬");
	SHOW::gotoxy(x, y++, "-----------------------------\t\t\t----------------------------");
	p(x, y, title);
	SHOW::gotoxy(x, y++, "-----------------------------\t\t\t----------------------------");
	SHOW::gotoxy(x, y, "");
	system("pause");
}

int main() {
	string input = menu();
	SHOW error(30, 21, "�����������������..");
	unDistribution.push_back(TASK("ʣ��ռ�", DISTRIBUTION_MAX, START_ADDRESS));
	while (1) {
		if (input == "0")
			return 0;
		else if (input == "1") {//����
			fresh(0, 0, 3000);
			short x = 25, y = 8;
			int prior, rt, state;
			string processName;
			SHOW in(x, y, "������Ҫ�����Ľ�������"), priority(x, y + 2, "���������ȼ���"), runTime(x, y + 4, "����������ʱ�䣺");
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
			else if (state == RUN && run == NULL && !PCB::lookForward(ready, processName) && !PCB::lookForward(stop, processName)) {
				run = new PCB(processName, prior, rt, RUN, NULL);
				SHOW::gotoxy(x, y, "���̴����ɹ�!!!");
				run->setState(RUN);
			}
			else {
				PCB* create = new PCB(processName, prior, rt, state, NULL);
				if ((run && (run->getName() == processName)) || (state == RUN && run != NULL))
					SHOW::gotoxy(x, y, "����ͬ����������ִ�У�����");
				else if (state == READY && !run) {
					char ch;
					SHOW::gotoxy(x, y, "��ǰû�н��̴���ִ��״̬����״̬�Ƿ�תΪִ��״̬(Y/N)��");
					y += 2;
					cin >> ch;
					if (ch == 'Y' || ch == 'y') {
						run = create;
						run->setState(RUN);
						SHOW::gotoxy(x, y, "�ѽ���ǰ����תΪִ��״̬��");
					}
					else if (ch == 'N' || ch == 'n') {
						create->putReady(ready, stop);
						SHOW::gotoxy(x, y, "��ǰ�����Ѵ洢Ϊ����״̬��");
					}
					else SHOW::gotoxy(x, y, "������������ԡ�");
				}
				else if (state == READY && create->putReady(ready, stop))
					SHOW::gotoxy(x, y, "�������̴����ɹ�!!!");
				else if (state == STOP && create->putStop(ready, stop, STOP))
					SHOW::gotoxy(x, y, "�������̴����ɹ�!!!");
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
			fresh(0, 0, 3000);
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
			fresh(0, 0, 3000);
			if (stop.empty()) SHOW::gotoxy(x, y, "��������Ϊ�գ�����ִ�д˲���");
			else if (ready.size() == 10) SHOW::gotoxy(x, y, "������������������ִ�д˲���");
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
			fresh(0, 0, 3000);
			string processName;
			SHOW::gotoxy(x, y, "������Ҫ�����Ľ������ƣ�");
			y += 2;
			cin >> processName;
			int readyState = PCB::lookForward(ready, processName),
				stopState = PCB::lookForward(stop, processName);
			if (run && run->getName() == processName) {
				delete run;
				run->setNext(NULL);
				SHOW::gotoxy(x, y, "�ý������ͷš�");
				y += 2;
				SHOW::gotoxy(x, y, "�ý���Ϊ����״̬���Ѿ�������");
				y += 2;
				if (ready.empty()) {
					SHOW::gotoxy(x, y, "���������ѿգ�û�н�������ִ�С�");
					run = NULL;
				}
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
			fresh(0, 0, 3000);
			string title = (manageState == 1) ? "���н���\t���ȼ�����״̬" : "���н���\tʱ��Ƭ����״̬";
			processShow(title, x, y, [](short& x, short& y, string title) {
				int count = 0;
				if (run) {
					count++;
					SHOW::gotoxy(x, y++, run->toString(count));
				}
				for (unsigned int i = 0; i < ready.size(); i++, y++) {
					count++;
					SHOW::gotoxy(x, y, ready[i]->toString(count));
					processPageUp(count, x, y, title);
				}
				for (unsigned int i = 0; i < stop.size(); i++, y++) {
					count++;
					SHOW::gotoxy(x, y, stop[i]->toString(count));
					processPageUp(count, x, y, title);
				}
				});
			goto refreash;
		}
		else if (input == "6") {//����
			short x = 25, y = 4;
			fresh(0, 0, 3000);
			if (ready.empty()) {
				SHOW::gotoxy(25, 6, "�޾��������޷�����");
				SHOW::gotoxy(25, 8, "");
				system("pause");
				goto refreash;
			}
			string inputManage = menuManage(x, y);
			y += 2;
			while (1) {
				if (inputManage == "0")
					break;
				else if (inputManage == "1") {//���ȼ�����
					fresh(0, 0, 3000);
					string title = (manageState == 1) ? "����˳��\t���ȼ�����״̬" : "����˳��\tʱ��Ƭ����״̬";
					processShow(title, x, y, [](short& x, short& y, string title) {
						int count = 0;
						sort(ready.begin(), ready.end(), [](PCB* a, PCB* b) {
							return a->getPriority() > b->getPriority();
							});
						connectReady();
						if (!run && ready.size() >= 1) ready[0]->putRun(ready);
						while (run) {
							count++;
							SHOW::gotoxy(x, y++, run->toString(count));
							delete run;
							if (!ready.empty()) ready[0]->putRun(ready);
							else run = NULL;
							processPageUp(count, x, y, title);
						}
						});
					manageState = 1;
					break;
				}
				else if (inputManage == "2") {//ʱ��Ƭ����
					fresh(0, 0, 3000);
					SHOW::gotoxy(x, y, "������ÿ�ش����ʱ��Ƭ���ȣ�");
					cin >> length;
					fresh(0, 0, 3000);
					string title = (manageState == 1) ? "����˳��\t���ȼ�����״̬" : "����˳��\tʱ��Ƭ����״̬";
					processShow(title, x, y, [](short& x, short& y, string title) {
						int count = 0;
						if (!run && ready.size() >= 1) ready[0]->putRun(ready);
						while (run) {
							count++;
							SHOW::gotoxy(x, y++, run->toString(count));
							int size = run->getRunTime() - length;
							run->setRunTime(size);
							if (run->getRunTime() <= 0) delete run;
							else run->putReady(ready, stop);
							if (!ready.empty()) ready[0]->putRun(ready);
							else run = NULL;
							processPageUp(count, x, y, title);
						}
						});
					break;
				}
				else {
					fresh(0, 0, 3000);
					inputManage = menuManage(x, y);
				}
			}
			goto refreash;
		}
		else if (input == "7") {//��̬����
			short x = 25, y = 4;
			fresh(0, 0, 3000);
			string inputDp = menuDp(x, y);
			while (1) {
				if (inputDp == "0")
					break;
				else if (inputDp == "1") {//����
					short x = 25, y = 4;
					string taskName;
					int taskLength;
					bool isVic = false;//�Ƿ��ܹ��ɹ������β
					fresh(0, 0, 3000);
					SHOW::gotoxy(x, y, "��ҵ����");
					cin >> taskName;
					y += 2;
					SHOW::gotoxy(x, y, "��ҵ���ȣ�");
					cin >> taskLength;
					y += 2;
					isVic = TASK(taskName, taskLength).putlist(distribution, unDistribution);//��ַ������
					if (isVic) {
						SHOW::gotoxy(x, y, "ʣ��ռ䲻�����ҵ���ظ����������ҵ�����ԡ�");
						y += 2;
						SHOW::gotoxy(x, y, "");
						system("pause");
					}
					else {
						SHOW::gotoxy(x, y++, "�����ɹ�������");
						y++;
						SHOW::gotoxy(x, y, "");
						system("pause");
					}
					goto distri;
				}
				else if (inputDp == "2") {//����
					short x = 25, y = 4;
					fresh(0, 0, 3000);
					string taskName;
					SHOW::gotoxy(x, y++, "���յ���ҵ���ƣ�");
					y++;
					cin >> taskName;
					for (list<TASK>::iterator it = distribution.begin(); it != distribution.end(); it++) {
						if ((*it).name == taskName) {
							(*it).erase(it, distribution, unDistribution);
							SHOW::gotoxy(x, y, "���ճɹ�������");
							y += 2;
							SHOW::gotoxy(x, y, "");
							system("pause");
							goto distri;
						}
					}
					SHOW::gotoxy(x, y, "û�и���ҵ...");
					y += 2;
					SHOW::gotoxy(x, y, "");
					system("pause");
					goto distri;
				}
				else if (inputDp == "3") {//��ʾ
					short x = 15, y = 4;
					fresh(0, 0, 3000);
					distributionShow("δ������˵����\t\t\t\t\t�ѷ�����˵����", x, y, [](short& x, short& y, string title) {
						//��ʾ������ݲ���
						int count = 0;
						list<TASK>::iterator it1 = distribution.begin(), it2 = unDistribution.begin();
						for (; it1 != distribution.end() || it2 != unDistribution.end(); ) {
							count++;
							string s, s1, s2;
							if (it1 == distribution.end())
								s2 = "";
							else if (it2 == unDistribution.end())
								s1 = "\t\t\t";
							if (it1 != distribution.end()) {
								s2 = (*it1).toString();
								it1++;
							}
							if (it2 != unDistribution.end()) {
								s1 = itos(count, 10) + "\t" + itos((*it2).startAddress, 10) + "\t" + itos((*it2).length, 10) + "\t1";
								it2++;
							}
							if (s1 == "\t\t\t") s1 += "\t";
							s = s1 + "\t\t\t" + s2;
							SHOW::gotoxy(x, y++, s);
							distributionPageUp(count, x, y, "δ������˵����\t\t\t\t\t�ѷ�����˵����");
						}
						});
					goto distri;
				}
				else {
				distri:fresh(0, 0, 3000);
					inputDp = menuDp(x, y);
				}
			}
			goto refreash;
		}
		else {
			error.gotoxy();
			SHOW::gotoxy(30, 23, "");
			system("pause");
		refreash:fresh(0, 0, 3000);
			if (manageState == 1 && ready.size() >= 1) {
				sort(ready.begin(), ready.end(), [](PCB* a, PCB* b) {
					return a->getPriority() > b->getPriority();
					});
				connectReady();
				ready[0]->putRun(ready);
			}
			input = menu();
		}
	}
	return 0;
}