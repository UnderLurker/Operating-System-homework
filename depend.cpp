#include "depend.h"
SHOW::SHOW(short x, short y, const string dialog) {
	this->x = x;
	this->y = y;
	this->dialog = dialog;
}
void SHOW::gotoxy() {
	HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
	COORD p;
	p.X = this->x;
	p.Y = this->y;
	SetConsoleCursorPosition(handle, p);
	cout << dialog;
}
PCB::PCB(string processName, int priority, int runTime, int state, PCB* next) {
	//runTime单位为s
	this->processName = processName;
	this->priority = priority;
	this->runTime = runTime;
	this->state = state;
	this->next = next;
	if (state == READY) {
		this->runState = "就绪";
	}
	else if (state == RUN) {
		this->runState = "运行";
	}
	else if (state == STOP) {
		this->runState = "阻塞";
	}
}
PCB::~PCB() {
}
void PCB::setState(int state) {
	this->state = state;
	if (state == READY) {
		this->runState = "就绪";
	}
	else if (state == RUN) {
		this->runState = "运行";
	}
	else if (state == STOP) {
		this->runState = "阻塞";
	}
}
bool PCB::putReady(vector<PCB*>& ready, vector<PCB*>& stop) {//false为队列已满
	this->state = READY;
	this->runState = "就绪";
	if (ready.size() == 0) ready.push_back(this);
	else if (ready.size() == 10 || lookForward(ready) || lookForward(stop)) return false;
	else {
		ready.back()->next = this;
		ready.push_back(this);
	}
	return true;
}
bool PCB::stopToReady(vector<PCB*>& stop, vector<PCB*>& ready) {
	if (stop.empty()) return false;
	int i;
	this->state = READY;
	vector<PCB*>::iterator it = stop.begin();
	for (i = 0; it != stop.end(); it++, i++) {
		if (stop[i]->processName == this->processName) {
			if (i - 1 >= 0 && i + 1 < stop.size())
				stop[i - 1]->next = stop[i]->next;
			else if (i - 1 < 0 && i + 1 < stop.size())
				this->next = NULL;
			else if (i - 1 >= 0 && i + 1 >= stop.size())
				stop[i - 1]->next = NULL;
			if (!ready.empty()) ready.back()->next = this;
			ready.push_back(this);
			stop.erase(it);
			return true;
		}
	}
	return false;
}
int PCB::putStop(vector<PCB*>& ready, vector<PCB*>& stop, int former) {//放入阻塞队列 0 队列已满
	this->state = STOP;
	this->runState = "阻塞";
	if (stop.size() == 10) return 0;
	if (former == READY) {
		int i = 0;
		for (vector<PCB*>::iterator it = ready.begin(); it != ready.end(); it++, i++) {
			if (ready[i]->processName == this->processName) {
				if (i - 1 >= 0 && i + 1 < ready.size())
					ready[i - 1]->next = ready[i]->next;
				else if (i - 1 < 0 && i + 1 < ready.size())
					this->next = NULL;
				else if (i - 1 >= 0 && i + 1 >= ready.size())
					ready[i - 1]->next = NULL;
				if (!stop.empty()) stop.back()->next = this;
				stop.push_back(this);
				ready.erase(it);
				break;
			}
		}
	}
	if (former == RUN) {
		if (!stop.empty())
			stop.back()->next = run;
		stop.push_back(run);
		run = NULL;
	}
	if (former == STOP) {
		if (!stop.empty())
			stop.back()->next = this;
		stop.push_back(this);
	}
	return STOP;
}
bool PCB::deletePCB(vector<PCB*>& s) {//从向量s中删除PCB
	if (s.empty()) return false;
	int i;
	vector<PCB*>::iterator it = s.begin();
	for (i = 0; it != s.end(); it++, i++) {
		if (s[i]->processName == this->processName) {
			if (i - 1 >= 0 && i + 1 < s.size())
				s[i - 1]->next = s[i]->next;
			else if (i - 1 < 0 && i + 1 < s.size())
				this->next = NULL;
			else if (i - 1 >= 0 && i + 1 >= s.size())
				s[i - 1]->next = NULL;
			s.erase(it);
			return true;
		}
	}
	return false;
}
bool PCB::putRun(vector<PCB*>& s) {//将已有的进程放入运行状态
	if (s.empty()) return false;
	int i;
	vector<PCB*>::iterator it = s.begin();
	for (i = 0; it != s.end(); it++, i++) {
		if (s[i]->processName == this->processName) {
			if (i - 1 >= 0 && i + 1 < s.size())
				s[i - 1]->next = s[i]->next;
			else if (i - 1 < 0 && i + 1 < s.size())
				this->next = NULL;
			else if (i - 1 >= 0 && i + 1 >= s.size())
				s[i - 1]->next = NULL;
			run = s[i];
			run->state = RUN;
			run->runState = "运行";
			run->next = NULL;
			s.erase(it);
			return true;
		}
	}
	return false;
}
bool PCB::lookForward(vector<PCB*>& s) {//看是否有this
	if (s.empty()) return false;
	PCB* temp = s.front();
	while (temp != NULL) {
		if (temp->processName == this->processName)
			return true;
		temp = temp->next;
	}
	return false;
}
string PCB::getName() {
	return this->processName;
}
int PCB::getState() {
	return this->state;
}
int PCB::getRunTime()
{
	return this->runTime;
}
int PCB::getPriority(){
	return this->priority;
}
void PCB::setPriority(int size){
	this->priority = size;
}
void PCB::setRunTime(int size){
	this->runTime = size;
}
string PCB::toString(int count) {
	string ans, s;
	stringstream ss1, ss2, ss3;
	if (count != 0) {
		ss3 << count;
		ss3 >> s;
		ans += s + '\t';
	}
	ans += processName + '\t' + '\t';
	ss1 << priority;
	ss1 >> s;
	ans += s + '\t';
	ss2 << runTime;
	ss2 >> s;
	ans += s + '\t' + '\t';
	ans += runState;
	return ans;
}
PCB* PCB::getNext() {
	return this->next;
}
void PCB::setNext(PCB* s) {
	this->next = s;
}
TASK::TASK(string name, int length) {
	this->name = name;
	this->length = length;
}
TASK::TASK(string name, int length,int startAddress) {
	this->name = name;
	this->length = length;
	this->startAddress = startAddress;
}

TASK::~TASK(){
}

bool TASK::putlist(list<TASK>& distribution, list<TASK>& unDistribution){
	for (list<TASK>::iterator it = distribution.begin(); it != distribution.end(); it++)
		if ((*it).name == this->name)
			return true;
	for (list<TASK>::iterator it = unDistribution.begin(); it != unDistribution.end(); it++) {
		if (this->length <= (*it).length) {
			this->startAddress = (*it).startAddress;
			for (list<TASK>::iterator it1 = distribution.begin(); it1 != distribution.end(); it1++) {
				if ((*it1).startAddress >= this->startAddress+this->length) {
					distribution.insert(it1, *this);
					(*it).length -= this->length;
					if ((*it).length == 0) unDistribution.erase(it);
					(*it).startAddress += this->length;
					return false;
				}
			}
			distribution.push_back(*this);
			(*it).length -= this->length;
			(*it).startAddress += this->length;
			if ((*it).length == 0) unDistribution.erase(it);
			return false;
		}
	}
	return true;
}

void TASK::erase(list<TASK>::iterator& it, list<TASK>& distribution, list<TASK>& unDistribution){
	for (list<TASK>::iterator i = unDistribution.begin(); i != unDistribution.end(); i++) {
		if ((this->length + this->startAddress) == (*i).startAddress) {
			(*i).length += (*it).length;
			(*i).startAddress -= (*it).length;
			if ((*i).length == 0) unDistribution.erase(i);
			distribution.erase(it);
			return;
		}
		if ((*i).startAddress > this->startAddress) {
			unDistribution.insert(i, *this);
			distribution.erase(it);
			return;
		}
	}
	if (unDistribution.empty()||(this->startAddress != unDistribution.back().startAddress+unDistribution.back().length)) {
		unDistribution.push_back(*this);
		distribution.erase(it);
	}
	else {
		unDistribution.back().length += this->length;
		distribution.erase(it);
	}
}

string TASK::toString(){
	string ans = name + "\t" + itos(startAddress, 10) + "\t" + itos(length, 10) + "\t1";
	return ans;
}

