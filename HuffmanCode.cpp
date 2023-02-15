#define SET_BYTE(vbyte, index) ((vbyte) |= (1 << ((index) ^ 7)))
#define CLR_BYTE(vbyte, index) ((vbyte) &= (~(1 << ((index) ^ 7))))
#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <stdio.h>
#include <fstream>
#include <string>
#include <algorithm>
#include <time.h>
#include <queue>
//定义哈夫曼树向左为0,向右为1
using namespace std;
const int N = 128;
struct Node {
	char a;
	Node* l = NULL, * r = NULL;
	int number = 0;
	bool flag = false;
	Node* real = NULL;
	string ad;
};
void swap(Node& a, Node& b)
{
	Node tmp = a;
	a = b;
	b = tmp;
}
Node h[N * 2];
int cnt = N;//堆的主体,所存的元素是row数组的下标
void down(int x)
{
	int t = x;
	if (2 * x <= cnt && h[t].number > h[2 * x].number) t = 2 * x;
	if (2 * x + 1 <= cnt && h[t].number > h[2 * x + 1].number) t = x * 2 + 1;
	if (t != x)
	{
		swap(h[t], h[x]);
		down(t);
	}
}
void up(int x)
{
	while (x > 1 && h[x / 2].number > h[x].number)
	{
		swap(h[x], h[x / 2]);
		x >>= 1;
	}
}
void insert(Node* a)
{
	cnt++;
	h[cnt] = *a;
	up(cnt);
}
Node* root;
string tm[N * 2];
int main()
{
	ifstream infile1("data.txt"), infile2("inputfile2.txt");
	ofstream oufile1("outputfile.dat", fstream::out | fstream::binary), oufile2("outputfile1.txt"), oufile3("outputfile2.txt");
	if (!infile1.is_open() || !infile2.is_open())
	{
		cout << "can not open the file!" << endl;
	}
	string tmp;
	for (int i = 0; i <= N; i++) h[i].a = i;
	while (!infile1.eof())
	{
		getline(infile1, tmp);
		for (int i = 0; i < tmp.length(); i+=2)
		{
			infile1 >> h[tmp[i]].number;
		}
	}
	for (int i = N / 2; i; i--) down(i);
	Node* aa, * b, *c;
	while (cnt != 1)
	{
		if (!h[1].flag)
		{
			aa = new Node;
			*aa = h[1];
			h[1] = h[cnt--];
		}
		else {
			aa = h[1].real;
			h[1] = h[cnt--];
		}
		down(1);
		if (!h[1].flag)
		{
			b = new Node;
			*b = h[1];
			h[1] = h[cnt--];
		}
		else {
			b = h[1].real;
			h[1] = h[cnt--];
		}
		down(1);
		c = new Node;
		c->number = aa->number + b->number;
		c->flag = true;
		c->l = aa, c->r = b;
		if (cnt == 0) root = c;
		h[++cnt] = *c;
		h[cnt].real = c;
		up(cnt);
	}
	Node* q = root;
	queue <Node*> aaa;
	aaa.push(root);
	while (aaa.size() > 0)
	{
		auto t = aaa.front();
		aaa.pop();
		if (t->l != NULL)
		{
			aaa.push(t->l);
			t->l->ad = t->ad + '0';
		}
		if (t->r != NULL)
		{
			aaa.push(t->r);
			t->r->ad = t->ad + '1';
		}
		if (t->flag) continue;
		else {
			oufile2 << t->a << " " << t->number << " " << t->ad << endl;
			oufile3 << t->a << " " << t->ad << endl;
			tm[t->a] = t->ad;
		}
	}
	int index = 0;
	FILE* fpout;
	fpout = fopen("outputfile.dat", "wb");
	char value;
	while (!infile2.eof())
	{
		getline(infile2, tmp);
		int t = 0;
		for (t = 0; t < tmp.length(); t++)
		{
			char aaaa = tmp[t];
			for (int j = 0; j < tm[aaaa].length(); j++)
			{
				string s = tm[aaaa];
				if ('0' == s[j])
				{
					CLR_BYTE(value, index);
				}
				else {
					SET_BYTE(value, index);
				}
				index++;
				if (index >= 8)
				{
					index = 0;
					fwrite(&value, sizeof(char), 1, fpout);
				}
			}
		}
	}//编码
	if (index) {
		fwrite(&value, sizeof(char), 1, fpout);
	}
	q = root;
	aaa.push(root);
	while (aaa.size() > 0)
	{
		auto t = aaa.front();
		aaa.pop();
		if (t->l != NULL)
		{
			aaa.push(t->l);
			t->l->ad = t->ad + '0';
		}
		if (t->r != NULL)
		{
			aaa.push(t->r);
			t->r->ad = t->ad + '1';
		}
		delete t;
	}
}