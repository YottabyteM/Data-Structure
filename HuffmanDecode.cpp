#define GET_BYTE(vbyte, index) (((vbyte) & (1 << ((index) ^ 7))) != 0)
#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <fstream>
#include <string>
using namespace std;
struct Node {
	char a;
	Node* l = NULL, * r = NULL;
	bool flag = false;
};
Node* newroot = new Node;
void buildtrie(string a, char ac)
{
	Node* q = newroot;
	for (int i = 0; i < a.length(); i++)
	{
		if (a[i] == '0')
		{
			if (q->l == NULL) q->l = new Node;
			q = q->l;
		}
		else {
			if (q->r == NULL) q->r = new Node;
			q = q->r;
		}
	}
	q->a = ac;
	q->flag = true;
}
Node* q = newroot;
int main()
{
	ifstream infile2("outputfile2.txt");//读入密码本
	char mi; string cal, tmp;
	int m;
	while (!infile2.eof())
	{
		getline(infile2, tmp);
		mi = tmp[0];
		for (int i = 2; i < tmp.size(); i++) cal += tmp[i];
		buildtrie(cal, mi);
		cal = "";
	}
	FILE* fin, * fpout;
	fin = fopen("outputfile.dat", "rb");//二进制压缩文件
	fpout = fopen("testcode.txt", "wb");
	char value;
	int index = 0;
	bool finished = false;
	fseek(fin, 0L, SEEK_END);
	long filesize = ftell(fin);
	fseek(fin, 0, SEEK_SET);
	long curlocation = ftell(fin);
	fread(&value, sizeof(char), 1, fin);
	while (!finished)
	{
		cout << index << endl;
		if (curlocation >= filesize) break;
		if (q->l == NULL && q->r == NULL && !feof(fin))
		{
			fwrite(&q->a, sizeof(char), 1, fpout);
			q = newroot;
		}
		if (GET_BYTE(value, index)) {
			q = q->r;
		}
		else {
			q = q->l;
		}
		if (q == NULL) { cout << "NOT FOUND" << endl;  break; }
		if (feof(fin)) break;
		if (++index >= 8)
		{
			index = 0;
			fread(&value, sizeof(char), 1, fin);
			curlocation = ftell(fin);
		}
	}//解码
	fclose(fin);
	fclose(fpout);
}