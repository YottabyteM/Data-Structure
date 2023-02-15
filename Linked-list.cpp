#include <iostream>
#include<string>
#include<fstream>
#include<algorithm>
#include <time.h>
using namespace std;
const int N = 18010;
int son[N][26] = { 0 }, idx = 0;
int f[N] = { 0 };
int cmpstr(string a, string b) //a排在b的前面返回1, 相等返回2, a在b后面返回0.
{
	int l = a.length(),  r = b.length();
	for (int i = 0; i < l && i < r; i ++)
	{
		if (a[i] > b[i]) return 0;
		if (a[i] < b[i]) return 1;
	}
	switch (l == r) {
	case 0: return l > r ? 0 : 1;
	case 1: return 2;
	}
}
void insertstop(string str)
{
	int p = 0;
	for (int i = 0; i < str.length(); i++)
	{
		int u = str[i] - 'a';
		if (!son[p][u]) {
			son[p][u] = ++idx;
		}
		p = son[p][u];
	}
	f[p] = 1;
}
int unnece(string str) //是否和停词表的单词有重复
{
	int p = 0;
	for (int i = 0; str[i]; i++)
	{
		int u = str[i] - 'a';
		if (!son[p][u]) return false;
		p = son[p][u];
	}
	if (f[p] == 1) return true;
	else return false;
}
struct Node {
	int times;
	Node* next, * pre;
	string word;
};
class List {
public:
	Node* head, * tail;
	void add(string a);
	List() :head(NULL), tail(NULL) {};
	~List();
};
void wsort(Node* begin, Node* end);
void process(string& str) //清理大写字母和数字
{
	string tmp;
	for (int i = 0; str[i]; i++)
	{
		if (str[i] >= 'a' && str[i] <= 'z') tmp += str[i];
		else if (str[i] >= 'A' && str[i] <= 'Z') tmp += char(str[i] - 'A' + 'a');
	}
	str = tmp;
}
void swap(Node*& a, Node*& b)
{
	int tmp1 = a->times;
	string tmp = a->word;
	a->times = b->times;
	a->word = b->word;
	b->times = tmp1;
	b->word = tmp;
}
int main()
{
	ifstream infile1("fiction.txt"), infile2("stop.txt");
	ofstream oufile("testprocess.txt");//先检验是否能正确处理并读入文件
	ofstream oufile2("testorder1.txt");//检验字典序排的对不对
	ofstream oufile3("result.txt");//检验按词频排的对不对
	long long num1 = 0 ,num2 = 0;
	if (!infile1.is_open() || !infile2.is_open())
	{
		cout << "can not open the files" << endl;
		return 0;
	}
	clock_t start, finish;
	double totaltime;
	start = clock();
	List article[26];//以26个字母为首的链表
	string stop, word;
	for (int i = 0; i < 891; i++)
	{
		infile2 >> stop;
		if (stop != "")
			insertstop(stop);
	}
	int line = 0;
	while (!infile1.eof())
	{
		infile1 >> word;
		process(word);
		num1++;
		if (!unnece(word) && word != "") {
			oufile << word << " ";
			num2++;
			line = max(line, word[0] - 'a');
			article[word[0] - 'a'].add(word);
		}
	}
	for (int i = 0; i < 26; i++)
	{
		Node* p = article[i].head;
		while (p != NULL)
		{
			oufile2 << p->word << " " << p->times << "\n"[i==line && p -> next == NULL];
			p = p->next;
		}
		wsort(article[i].head, article[i].tail);
		p = article[i].head;
		while (p != NULL)
		{
			oufile3 << p->word << " " << p->times << "\n"[i == line && p -> next == NULL];
			p = p->next;
		}
	}
	finish = clock();
	totaltime = (double)(finish - start) / CLOCKS_PER_SEC;
	cout << "程序运行时间为" << totaltime << "秒" << endl;
	cout << "总词数: " << num1 << " 和停词表不一样的词数: " << num2 << endl;
}
void List::add(string a)
{
	if (head == NULL)
	{
		head = new Node;
		head->word = a;
		head->next = NULL;
		head->pre = NULL;
		head->times = 1;
		tail = head;
	}
	else {
		if (cmpstr(a, head->word) == 2)
		{
			head->times ++;
		}
		else {
			if (cmpstr(a, head->word) == 1)
			{
				Node* aa = new Node;
				aa->next = head;
				aa->pre = NULL;
				aa->times = 1;
				aa->word = a;
				head->pre = aa;
				head = aa;
			}
			else {
				Node* p = head;
				while (p->next)
				{
					if (cmpstr(a, p->next->word) == 0)
						p = p->next;
					else break;
				}//移到位置i, 该位置满足字典序a[i - 1] < a[i] <= a[i + 1]或者移到队尾
				if (p->next)
				{
					if (cmpstr(a, p->next->word) == 2) p->next->times++;
					else
					{
						Node* tmp = p->next;
						p->next = new Node;
						p->next->word = a;
						p->next->times = 1;
						p->next->next = tmp;
						p->next->pre = p;
						tmp->pre = p->next;
					}
				}
				else
				{
					p->next = new Node;
					p->next->word = a;
					p->next->pre = p;
					p->next->times = 1;
					p->next->next = NULL;
					tail = p->next;
				}
			}
		}
	}
}
void wsort(Node* begin, Node* end)
{
	if (begin == end) return;
	Node* m = begin;
	Node* n = end;
	Node* target = begin;
	while (m != n)
	{
		while (m->times > target->times && m != n) m = m->next;
		while (n->times <= target->times && m != n) n = n->pre;
		if (m != n) swap(m, n);
	}
	wsort(begin, n);
	wsort(n->next, end);
}
List::~List()
{
	Node* tmp = head;
	while (head)
	{
		tmp = head->next;
		delete head;
		head = tmp;
	}
}