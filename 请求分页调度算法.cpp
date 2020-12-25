#include<iostream>
#include<vector>
using namespace std;
#define SIZE 1024

class PAGE {
public:
	int id;
	bool local;//ÄÚ´æfalse Íâ´ætrue
	int nId;//Ö÷´æ¿éºÅ
	bool flag;//¸ü¸Ätrue Î´¸ü¸Äfalse
	int wId;//´ÅÅÌ¿éºÅ
	PAGE(int _id, bool _local, int _nId, bool _flag, int _wId) {
		id = _id;
		local = _local;
		nId = _nId;
		flag = _flag;
		wId = _wId;
	}
};

int main() {
	vector<PAGE*> q(3);
	vector<PAGE*> s(8);
	s[0] = new PAGE(0, 0, 0, 0, 0);
	s[1] = new PAGE(1, 0, 1, 0, 0);
	s[2] = new PAGE(2, 0, 2, 0, 0);
	s[3] = new PAGE(3, 1, 0, 0, 0);
	s[4] = new PAGE(4, 1, 0, 0, 4);
	s[5] = new PAGE(5, 1, 0, 0, 5);
	s[6] = new PAGE(6, 1, 0, 0, 6);
	s[7] = new PAGE(7, 1, 0, 0, 7);
	q[0] = s[0];
	q[1] = s[1];
	q[2] = s[2];
	while (1) {
		int logicLocal, page, pageLocal;
		bool flag = false;
		cout << "ÊäÈëÂß¼­µØÖ·£º";
		cin >> logicLocal;
		if (logicLocal <= 0 || logicLocal >= 7 * 1024) {
			cout << "ÊäÈë´íÎó";
			return 0;
		}
		page = logicLocal / SIZE;
		pageLocal = logicLocal % SIZE;
		A:for (int i = 0; i < 3; i++) {
			if (!q[i]->local && q[i]->id == page) {
				cout << "page£º" << i << "\tpageLocal£º" << pageLocal << endl;
				flag = true;
				break;
			}
		}
		if (!flag) {
			for (int i = 0; i < 8; i++) {
				if (s[i]->local && s[i]->id == page) {
					q.front()->local = true;
					q.erase(q.begin());
					s[i]->local = false;
					q.push_back(s[i]);
					break;
				}
			}
			goto A;
		}
	}
	return 0;
}