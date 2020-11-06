#define _CRT_SECURE_NO_WARNINGS

#include"depend.h"

list<TASK> distribution;//已分配区
list<TASK> unDistribution;//未分配区
vector<PCB*> ready;
vector<PCB*> stop;
int length;//每回处理的时间片长度
int manageState = 1;//进程调度状态
//int startAddress = START_ADDRESS;
//int distributionMax = DISTRIBUTION_MAX;


void connectReady() {//连接ready内的next
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
	word[0] = "1………创建";
	word[1] = "2………阻塞";
	word[2] = "3………唤醒";
	word[3] = "4………终止";
	word[4] = "5………显示";
	word[5] = "6………调度";
	word[6] = "7………动态分区";
	word[7] = "0………退出";
	SHOW::gotoxy(31, 3, "系统主菜单");
	short x = 30, y = 5;
	for (unsigned int i = 0; i < word.size(); i++, y += 2) {
		SHOW::gotoxy(x, y, word[i]);
	}
	SHOW::gotoxy(25, y, "请输入您需要的功能(0--7)");
	SHOW::gotoxy(30, y + 2, "输入选择 = ");
	string ans;
	cin >> ans;
	return ans;
}

string menuManage(short x, short y) {
	string input;
	SHOW::gotoxy(x, y, "0…………返回主菜单");
	y += 2;
	SHOW::gotoxy(x, y, "1…………优先级调度");
	y += 2;
	SHOW::gotoxy(x, y, "2…………时间片调度");
	y += 2;
	SHOW::gotoxy(x - 2, y, "请选择您需要的功能选项：");
	cin >> input;
	return input;
}

string menuDp(short x, short y) {
	string ans;
	SHOW::gotoxy(x, y, "0………退出");
	y += 2;
	SHOW::gotoxy(x, y, "1………分配");
	y += 2;
	SHOW::gotoxy(x, y, "2………回收");
	y += 2;
	SHOW::gotoxy(x, y, "3………显示");
	y += 2;
	SHOW::gotoxy(x - 4, y, "请选择您需要的功能：");
	cin >> ans;
	return ans;
}

void processShow(string title, short& x, short& y, void (*p)(short& x, short& y, string title)) {//打表
	SHOW::gotoxy(x, y++, title);
	SHOW::gotoxy(x, y++, "-------------------------------------------------------");
	SHOW::gotoxy(x, y++, "序号\t进程名称\t优先级\t运行时间\t运行状态");
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
		SHOW::gotoxy(x, y++, "序号\t进程名称\t优先级\t运行时间\t运行状态");
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
		SHOW::gotoxy(x, y++, "序号\t始址\t长度\t状态\t\t\t作业名\t始址\t长度\t状态");
		SHOW::gotoxy(x, y++, "-----------------------------\t\t\t----------------------------");
	}
}

void distributionShow(string title, short& x, short& y, void (*p)(short& x, short& y, string title)) {
	SHOW::gotoxy(x, y++, title);
	SHOW::gotoxy(x, y++, "-----------------------------\t\t\t----------------------------");
	SHOW::gotoxy(x, y++, "序号\t始址\t长度\t状态\t\t\t作业名\t始址\t长度\t状态");
	SHOW::gotoxy(x, y++, "-----------------------------\t\t\t----------------------------");
	p(x, y, title);
	SHOW::gotoxy(x, y++, "-----------------------------\t\t\t----------------------------");
	SHOW::gotoxy(x, y, "");
	system("pause");
}

int main() {
	string input = menu();
	SHOW error(30, 21, "输入错误，请重新输入..");
	unDistribution.push_back(TASK("剩余空间", DISTRIBUTION_MAX, START_ADDRESS));
	while (1) {
		if (input == "0")
			return 0;
		else if (input == "1") {//创建
			fresh(0, 0, 3000);
			short x = 25, y = 8;
			int prior, rt, state;
			string processName;
			SHOW in(x, y, "请输入要创建的进程名："), priority(x, y + 2, "请输入优先级："), runTime(x, y + 4, "请输入运行时间：");
			in.gotoxy();
			cin >> processName;
			priority.gotoxy();
			cin >> prior;
			runTime.gotoxy();
			cin >> rt;
			y += 6;
			SHOW::gotoxy(x, y, "1-就绪\t2-执行\t3-阻塞");
			y += 2;
			SHOW::gotoxy(x, y, "请输入运行状态：");
			y += 2;
			cin >> state;
			if (state <= 0 || state > 3) {
				SHOW::gotoxy(x, y, "没有该状态");
				goto end;
			}
			if (state == RUN && run) SHOW::gotoxy(x, y, "已经有程序处于运行状态,请重试。");
			else if (state == RUN && run == NULL && !PCB::lookForward(ready, processName) && !PCB::lookForward(stop, processName)) {
				run = new PCB(processName, prior, rt, RUN, NULL);
				SHOW::gotoxy(x, y, "进程创建成功!!!");
				run->setState(RUN);
			}
			else {
				PCB* create = new PCB(processName, prior, rt, state, NULL);
				if ((run && (run->getName() == processName)) || (state == RUN && run != NULL))
					SHOW::gotoxy(x, y, "已有同名进程正在执行！！！");
				else if (state == READY && !run) {
					char ch;
					SHOW::gotoxy(x, y, "当前没有进程处于执行状态，该状态是否转为执行状态(Y/N)：");
					y += 2;
					cin >> ch;
					if (ch == 'Y' || ch == 'y') {
						run = create;
						run->setState(RUN);
						SHOW::gotoxy(x, y, "已将当前程序转为执行状态。");
					}
					else if (ch == 'N' || ch == 'n') {
						create->putReady(ready, stop);
						SHOW::gotoxy(x, y, "当前程序已存储为就绪状态。");
					}
					else SHOW::gotoxy(x, y, "输入错误，请重试。");
				}
				else if (state == READY && create->putReady(ready, stop))
					SHOW::gotoxy(x, y, "就绪进程创建成功!!!");
				else if (state == STOP && create->putStop(ready, stop, STOP))
					SHOW::gotoxy(x, y, "阻塞进程创建成功!!!");
				else SHOW::gotoxy(x, y, "队列中已有同名进程！！！");
			}
		end:y += 2;
			SHOW::gotoxy(x, y, "");
			system("pause");
			goto refreash;
		}
		else if (input == "2") {//阻塞
			short x = 25, y = 8;
			string processName;
			fresh(0, 0, 3000);
			SHOW::gotoxy(x, y, "请输入要阻塞的进程名称：");
			y += 2;
			cin >> processName;
			int readyState = PCB::lookForward(ready, processName), stopState = PCB::lookForward(stop, processName);
			if (readyState || stopState || (run && run->getName() == processName)) {
				if (run && run->getName() == processName) {
					run->putStop(ready, stop, RUN);
					SHOW::gotoxy(x, y, "已将运行的进程挪入阻塞队列");
					y += 2;
					if (ready.empty()) SHOW::gotoxy(x, y, "就绪队列已空，没有进程正在执行。");
					else {//将就绪队列第一个变为运行状态
						run = ready[0];
						run->setNext(NULL);
						run->setState(RUN);
						vector<PCB*>::iterator it = ready.begin();
						ready.erase(it);
					}
				}
				else if (stopState == STOP) SHOW::gotoxy(x, y, "该进程处于阻塞状态。。");
				else if (readyState == READY) SHOW::gotoxy(x, y, "该进程处于就绪状态。。");
			}
			else SHOW::gotoxy(x, y, "没有该进程。");
			y += 2;
			SHOW::gotoxy(x, y, "");
			system("pause");
			goto refreash;
		}
		else if (input == "3") {//唤醒
			short x = 25, y = 8;
			fresh(0, 0, 3000);
			if (stop.empty()) SHOW::gotoxy(x, y, "阻塞队列为空，不能执行此操作");
			else if (ready.size() == 10) SHOW::gotoxy(x, y, "就绪队列已满，不能执行此操作");
			else {
				string processName;
				SHOW::gotoxy(x, y, "请输入要唤醒的进程名称：");
				y += 2;
				cin >> processName;
				int readyState = PCB::lookForward(ready, processName), stopState = PCB::lookForward(stop, processName);
				if (readyState || stopState || (run && run->getName() == processName)) {
					if (run && run->getName() == processName)
						SHOW::gotoxy(x, y, "该进程已经处于运行状态");
					else if (stopState == STOP) {
						stop.back()->stopToReady(stop, ready);
						SHOW::gotoxy(x, y, "已将该进程移入就绪状态。。");
					}
					else if (readyState == READY) SHOW::gotoxy(x, y, "该进程已处于就绪状态。。");
				}
				else SHOW::gotoxy(x, y, "没有该进程。");
			}
			y += 2;
			SHOW::gotoxy(x, y, "");
			system("pause");
			goto refreash;
		}
		else if (input == "4") {//终止
			short x = 25, y = 8;
			fresh(0, 0, 3000);
			string processName;
			SHOW::gotoxy(x, y, "请输入要结束的进程名称：");
			y += 2;
			cin >> processName;
			int readyState = PCB::lookForward(ready, processName),
				stopState = PCB::lookForward(stop, processName);
			if (run && run->getName() == processName) {
				delete run;
				run->setNext(NULL);
				SHOW::gotoxy(x, y, "该进程已释放。");
				y += 2;
				SHOW::gotoxy(x, y, "该进程为运行状态，已经结束。");
				y += 2;
				if (ready.empty()) {
					SHOW::gotoxy(x, y, "就绪队列已空，没有进程正在执行。");
					run = NULL;
				}
				else {//将就绪队列第一个变为运行状态
					run = ready[0];
					run->setNext(NULL);
					run->setState(RUN);
					vector<PCB*>::iterator it = ready.begin();
					ready.erase(it);
					SHOW::gotoxy(x, y, run->getName() + " 正在执行。");
				}
			}
			else if (readyState == READY) {
				deletePCB(ready, processName);
				SHOW::gotoxy(x, y, "该进程为就绪状态，已经结束。");
			}
			else if (stopState == STOP) {
				deletePCB(stop, processName);
				SHOW::gotoxy(x, y, "该进程为阻塞状态，已经结束。");
			}
			else SHOW::gotoxy(x, y, "没有该进程");
			y += 2;
			SHOW::gotoxy(x, y, "");
			system("pause");
			goto refreash;
		}
		else if (input == "5") {//显示
			short x = 25, y = 4;
			fresh(0, 0, 3000);
			string title = (manageState == 1) ? "所有进程\t优先级调度状态" : "所有进程\t时间片调度状态";
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
		else if (input == "6") {//调度
			short x = 25, y = 4;
			fresh(0, 0, 3000);
			if (ready.empty()) {
				SHOW::gotoxy(25, 6, "无就绪进程无法调度");
				SHOW::gotoxy(25, 8, "");
				system("pause");
				goto refreash;
			}
			string inputManage = menuManage(x, y);
			y += 2;
			while (1) {
				if (inputManage == "0")
					break;
				else if (inputManage == "1") {//优先级调度
					fresh(0, 0, 3000);
					string title = (manageState == 1) ? "调度顺序\t优先级调度状态" : "调度顺序\t时间片调度状态";
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
				else if (inputManage == "2") {//时间片调度
					fresh(0, 0, 3000);
					SHOW::gotoxy(x, y, "请输入每回处理的时间片长度：");
					cin >> length;
					fresh(0, 0, 3000);
					string title = (manageState == 1) ? "调度顺序\t优先级调度状态" : "调度顺序\t时间片调度状态";
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
		else if (input == "7") {//动态分区
			short x = 25, y = 4;
			fresh(0, 0, 3000);
			string inputDp = menuDp(x, y);
			while (1) {
				if (inputDp == "0")
					break;
				else if (inputDp == "1") {//分配
					short x = 25, y = 4;
					string taskName;
					int taskLength;
					bool isVic = false;//是否能够成功插入队尾
					fresh(0, 0, 3000);
					SHOW::gotoxy(x, y, "作业名：");
					cin >> taskName;
					y += 2;
					SHOW::gotoxy(x, y, "作业长度：");
					cin >> taskLength;
					y += 2;
					isVic = TASK(taskName, taskLength).putlist(distribution, unDistribution);//地址有问题
					if (isVic) {
						SHOW::gotoxy(x, y, "剩余空间不足或作业名重复，请回收作业后再试。");
						y += 2;
						SHOW::gotoxy(x, y, "");
						system("pause");
					}
					else {
						SHOW::gotoxy(x, y++, "创建成功。。。");
						y++;
						SHOW::gotoxy(x, y, "");
						system("pause");
					}
					goto distri;
				}
				else if (inputDp == "2") {//回收
					short x = 25, y = 4;
					fresh(0, 0, 3000);
					string taskName;
					SHOW::gotoxy(x, y++, "回收的作业名称：");
					y++;
					cin >> taskName;
					for (list<TASK>::iterator it = distribution.begin(); it != distribution.end(); it++) {
						if ((*it).name == taskName) {
							(*it).erase(it, distribution, unDistribution);
							SHOW::gotoxy(x, y, "回收成功。。。");
							y += 2;
							SHOW::gotoxy(x, y, "");
							system("pause");
							goto distri;
						}
					}
					SHOW::gotoxy(x, y, "没有该作业...");
					y += 2;
					SHOW::gotoxy(x, y, "");
					system("pause");
					goto distri;
				}
				else if (inputDp == "3") {//显示
					short x = 15, y = 4;
					fresh(0, 0, 3000);
					distributionShow("未分配区说明表\t\t\t\t\t已分配区说明表", x, y, [](short& x, short& y, string title) {
						//显示表的数据部分
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
							distributionPageUp(count, x, y, "未分配区说明表\t\t\t\t\t已分配区说明表");
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