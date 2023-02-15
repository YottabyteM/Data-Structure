#include <iostream>
#include <string>
#include <list>
#include <queue>
#include <vector>
#include <easyx.h>
#include <graphics.h>
using namespace std;
static int num = 0;
static int pos = 0;
int WIN_WIDTH = 1000, WIN_HEIGHT = 800, eps = 20;
int assist[10] = { 0 };
void ininum()
{
	num = 0;
	pos = 0;
	return;
}
void reverse(string& a)
{
	string tmp;
	for (int i = 0; i < a.length(); i++) tmp += a[a.length() - 1 - i];
	a = tmp;
}
class BNode {
public:
	list <int> keys;
	list<BNode*> child;
	BNode* father;
	BNode();
	BNode(int);
	~BNode();
	bool isleaf();
	bool hasKey(int key);
	bool replace(int old_key, int new_key);
	bool erasekey(int key);
	bool erasechild(BNode*);
	void updatechild();
	void insert(int data, BNode* child1, BNode* child2);
	void print(int x, int y);
	BNode* next(int key);
	int getKey(int pos);
	int size();
};
class BTree {
public:
	int a[100] = { 0 };
	BNode * root;
	int k;
	int minKey, maxKey;
	int nsize;
	void maintainAfterInsert(BNode*);	
	void maintainAfterErase(BNode*);
	void mergeNode(BNode*, BNode*, int, bool flag);
	void deleteNode(BNode*);
	void print(BNode*);
	void paint();
	BNode* splitLeftNode(BNode*, int);
	BNode* splitRightNode(BNode*, int);

	BNode* findBrotherNode(BNode*, int&, bool flag);
	BNode* findNode(int);
	void count();

public:
	BTree();
	~BTree();
	BTree(int k);
	bool insert(int data);
	bool erase(int key);
	void printTree();
	int tsize();
	BNode* findNextNode(BNode* p, int, int&);
	BNode* findPreNode(BNode* p, int, int&);
	void dfs(BNode* p, int, float, float);
	void painting(BNode* p, int w, int h, int depth, float order);
	int height();
	int getK();

	BNode* getRoot();
	vector<BNode*> v;
};
int main()
{
	int n = 0, k = 0;
	string e;
	puts("请输入初始化的b树元素以及阶数:");
	cin >> n >> k;
	BTree ans(k);
	assist[0] = k - 1;
	for (int i = 1; i < 10; i++)
		assist[i] = assist[i - 1] * k;
	cout << "b树的元素有: " << n << "个 b树的阶数是: " << k << " " << endl;
	cout << "按照'x,y,z,m,n'的形式输入插入的数字" << endl;
	cin >> e;
	int i = 0;
	while (i < e.length())
	{
		if (isdigit(e[i]))
		{
			int t = e[i] - '0', j = i;
			while (j < e.length() - 1 && isdigit(e[j + 1]))
			{
				j++;
				t = t * 10 + e[j] - '0';
			}
			i = j;
			ans.insert(t);
		}
		i++;
	}
	initgraph(WIN_WIDTH, WIN_HEIGHT, SHOWCONSOLE);
	HWND hwnd = GetHWnd();
	MoveWindow(hwnd, 0, 0, WIN_WIDTH, WIN_HEIGHT, TRUE);
	setbkcolor(YELLOW);
	cleardevice();
	setlinecolor(BLUE);
	ans.painting(ans.root, WIN_WIDTH / 2 + 200, 100, 0, 0);
	cout << "规则: 插入则输入'I' + space + 数字, 删除则输入'D' + space + 数字" << endl;
	char c = 'I'; int d = 0;
	while (c != 'E')
	{
		cin >> c >> d;
		if (c == 'I') ans.insert(d);
		else if (c == 'D') ans.erase(d);
		initgraph(WIN_WIDTH, WIN_HEIGHT, SHOWCONSOLE);
		HWND hwnd = GetHWnd();
		MoveWindow(hwnd, 0, 0, WIN_WIDTH, WIN_HEIGHT, TRUE);
		setbkcolor(YELLOW);
		cleardevice();
		setlinecolor(BLUE);
		ans.painting(ans.root, WIN_WIDTH / 2 + 200, 100, 0, 0);
	}
	system("pause");
	closegraph();
	return 0;
}

bool BNode::isleaf()
{
	if (child.empty()) return true;
	for (list<BNode*>::iterator it = child.begin(); it != child.end(); it++)
		if (*it != NULL)
			return false;
	return true;
}

bool BNode::hasKey(int key) {
	list<int>::iterator it_key;
	for (it_key = keys.begin(); it_key != keys.end(); it_key++)
		if (*it_key == key)return true;
	return false;
}

bool BNode::replace(int old_key, int new_key) {
	list<int>::iterator it_key;
	for (it_key = keys.begin(); it_key != keys.end(); it_key++)
		if (*it_key == old_key) {
			*it_key = new_key;
			return true;
		}
	return false;
}

bool BNode::erasekey(int key) {
	if (!hasKey(key))return false;
	list<int>::iterator it_key;
	for (it_key = keys.begin(); it_key != keys.end(); it_key++)
		if (*it_key == key) {
			keys.erase(it_key);
			break;
		}
	return true;
}

bool BNode::erasechild(BNode* p) {
	list<BNode*>::iterator it_child;
	for (it_child = child.begin(); it_child != child.end(); it_child++)
		if (*it_child == p) {
			child.erase(it_child);
			return true;
		}
	return false;
}

BNode* BNode::next(int data)
{
	list<int>::iterator it_key = keys.begin();
	list<BNode*>::iterator it_child = child.begin();
	while (it_key != keys.end() && data >= *it_key) {
		it_child++;
		it_key++;
	}
	return *it_child;
}

void BNode::insert(int key, BNode* child1, BNode* child2)
{
	list<int>::iterator it_key = keys.begin();
	list<BNode*>::iterator it_child = child.begin();
	while (it_key != keys.end() && key > *it_key) {
		it_child++;
		it_key++;
	}
	keys.insert(it_key, key);
	if (!child.empty())it_child = child.erase(it_child);
	it_child = child.insert(it_child, child2);
	it_child = child.insert(it_child, child1);
}

void BNode::updatechild() {
	list<BNode*>::iterator it_child;
	for (it_child = child.begin(); it_child != child.end(); it_child++)
		if (*it_child)
			(*it_child)->father = this;
}

void BNode::print(int x, int y)
{
	list<int>::iterator it;
	for (it = keys.begin(); it != keys.end(); it++)
	{
		string tm = "";
		setlinecolor(BLUE);
		settextcolor(BLUE);
		rectangle(x, y, x + eps, y + eps);
		if (*it < 0) {
			tm += '-';
			tm += (-*it) + '0';
			outtextxy(x + 2, y + 2, tm.c_str());
		}
		else {
			if (*it < 10) outtextxy(x + 2, y + 2, *it + '0');
			else {
				int yy = *it;
				while (yy) tm += '0' + yy % 10, yy /= 10;
				reverse(tm);
				outtextxy(x + 2, y + 2, tm.c_str());
			}
		}
		x += eps;
	}
	num++;
}

void BTree::painting(BNode* p, int w, int h, int depth, float l)
{
	if (p == NULL) return;
	int x1 = w - (eps * assist[depth] + 10 * (assist[depth] / (k - 1) - 1)) / 2 + l * ((k - 1) * eps + 30), y1 = depth * 60 + h;
	p->print(x1, y1);
	depth++;
	int i = 0;
	list<BNode*>::iterator it;
	for (it = p->child.begin(); it != p->child.end(); it++, i ++)
	{
		float l1 = (float)i * (k - 1) / p->child.size() + l * (k - 1);
		if (*it) {
			painting(*it, w, h, depth, l1);
			line(x1, y1 + eps, w - (eps * assist[depth] + 10 * (assist[depth] / (k - 1) - 1)) / 2 + l1 * ((k - 1) * eps + 30), depth * 60 + h);
			x1 += eps;
		}
	}
	/*int tt = 0;
	if (p == NULL) return;
	else {
		queue<BNode*> q;
		q.push(p);
		while (q.size())
		{
			BNode* t = q.front();
			q.pop();
			t->print(0, tt * eps);
			tt++;
			for (list<BNode*>::iterator it = t->child.begin(); it != t->child.end(); it++)
				if (*it)
					q.push(*it);
		}
	}*/
}

int BNode::getKey(int pos)
{
	list<int>::iterator it = keys.begin();
	for (int i = 0; i < pos; ++i) {	//异常处理
		++it;
	}
	return *it;
}

int BNode::size()
{
	return keys.size();
	return 0;
}

BNode::BNode()
{
	keys.clear();
	child.clear();
	father = NULL;
}

BNode::BNode(int data)
{
	keys.push_back(data);
	child.push_back(NULL);
	child.push_back(NULL);
	father = NULL;
}

BNode::~BNode()
{
	child.clear();
	keys.clear();
}

bool BTree::insert(int key)
{
	BNode* p = root;
	if (root == NULL) {
		root = new BNode(key);
		nsize++;
		return 1;
	}
	while (!p->isleaf()) {
		if (p->hasKey(key))return false;
		p = p->next(key);
	}
	if (p->hasKey(key))return false;
	p->insert(key, NULL, NULL);
	maintainAfterInsert(p);
	nsize++;
	return 1;
}

bool BTree::erase(int key) {
	BNode* p = findNode(key);
	if (!p)return false; //未找到key
	if (!p->isleaf()) {
		int next_key;
		BNode* next_node = findNextNode(p, key, next_key);
		p->replace(key, next_key);
		key = next_key;
		p = next_node;
	}
	p->erasekey(key);
	p->erasechild(NULL);
	maintainAfterErase(p);
	nsize--;
	return true;
}

void BTree::maintainAfterErase(BNode* p) {
	BNode* fa = p->father;
	if (p == root) {
		if (p->size() == 0) {
			root = *(root->child.begin());
			if (root) root->father = NULL;
		}
		return;
	}
	if (p->size() >= minKey) return;
	int father_key;
	BNode* brother_node;
	brother_node = findBrotherNode(p, father_key, 0);//0左，1右

	if (brother_node) mergeNode(p, brother_node, father_key, 0);//存在左节点则合并左节点
	else {
		brother_node = findBrotherNode(p, father_key, 1);//0左，1右
		mergeNode(p, brother_node, father_key, 1);
	}
	this->maintainAfterErase(fa);
}

void BTree::mergeNode(BNode* p1, BNode* p2, int father_key, bool flag) {
	BNode* fa = p1->father;
	if (p2->size() > minKey) {
		int replace_key = flag ? *(p2->keys.begin()) : *(p2->keys.rbegin());
		BNode* replace_child = flag ? *(p2->child.begin()) : *(p2->child.rbegin());
		p2->erasekey(replace_key);
		p2->erasechild(replace_child);
		fa->replace(father_key, replace_key);
		if (flag == 0) {
			p1->keys.push_front(father_key);
			p1->child.push_front(replace_child);
		}
		else {
			p1->keys.push_back(father_key);
			p1->child.push_back(replace_child);
		}
		p1->updatechild();
	}
	else {
		fa->erasekey(father_key);
		fa->erasechild(p1);
		if (flag == 0) {
			p2->keys.push_back(father_key);
			p2->keys.splice(p2->keys.end(), p1->keys);
			p2->child.splice(p2->child.end(), p1->child);
		}
		else {
			p2->keys.push_front(father_key);
			p2->keys.splice(p2->keys.begin(), p1->keys);
			p2->child.splice(p2->child.begin(), p1->child);
		}
		fa->updatechild();
		p2->updatechild();
		delete p1;
	}
}

BNode* BTree::findBrotherNode(BNode* p, int& key, bool flag) {
	BNode* fa = p->father;
	list<int>::iterator it_key = fa->keys.begin();
	list<BNode*>::iterator it_child = fa->child.begin();
	while (*it_child != p) {
		it_child++; it_key++;
	}
	if (flag == 0) {//左兄弟
		if (it_child == fa->child.begin())return NULL;
		key = *(--it_key);
		return *(--it_child);
	}
	else {
		if ((++it_child) == fa->child.end())return NULL;
		key = *it_key;
		return  *it_child;
	}
}

BNode* BTree::findNextNode(BNode* p, int key, int& next_key) {
	if (!p) return NULL;
	list<int>::iterator it_key;
	list<BNode*>::iterator it_child;
	do {
		it_key = p->keys.begin();
		it_child = p->child.begin();
		while (it_key != p->keys.end()) {
			if (key < *it_key)break;
			it_child++; it_key++;
		}
		if (it_key != p->keys.end())next_key = *it_key;
		if (*it_child == NULL)
			return p;
	} while (p = *it_child);
}

BNode* BTree::findPreNode(BNode* p, int key, int& pre_key)
{
	if (!p)return NULL;
	list<int>::reverse_iterator it_key;
	list<BNode*>::reverse_iterator it_child;
	do {
		it_key = p->keys.rbegin();
		it_child = p->child.rbegin();
		while (it_key != p->keys.rend()) {
			if (*it_key < key)break;
			it_child++; it_key++;
		}
		if (it_key != p->keys.rend())pre_key = *it_key;
		if (*it_child == NULL)
			return p;
	} while (p = *it_child);
}

BNode* BTree::findNode(int key) {
	BNode* p = root;
	while (p && !(p->hasKey(key))) {
		p = p->next(key);
	}
	return p;
}

void BTree::maintainAfterInsert(BNode* p)
{
	while (p->size() >= k) {
		BNode* left_node = splitLeftNode(p, (k - 1) / 2);
		BNode* right_node = splitRightNode(p, k - (k - 1) / 2 - 1);
		int key = p->getKey((k - 1) / 2);	//上移的值
		delete p;
		p = left_node->father;	//父节点
		if (!p) {
			root = p = new BNode();
			left_node->father = right_node->father = root;
		}
		p->insert(key, left_node, right_node);
	}
}

BNode* BTree::splitLeftNode(BNode* p, int pos)
{
	BNode* node = new BNode();
	node->father = p->father;
	list<int>::iterator it_key = p->keys.begin();
	list<BNode*>::iterator it_child = p->child.begin();
	node->child.push_back(*it_child);
	int i = 0;
	while (i++ < pos) {
		it_child++;
		node->keys.push_back(*it_key);
		node->child.push_back(*it_child);
		it_key++;
	}
	node->updatechild();
	return node;
}

BNode* BTree::splitRightNode(BNode* p, int pos)
{
	BNode* node = new BNode();
	node->father = p->father;
	list<int>::reverse_iterator it_key = p->keys.rbegin();
	list<BNode*>::reverse_iterator it_child = p->child.rbegin();
	node->child.push_front(*it_child);
	int i = 0;
	while (i++ < pos) {
		it_child++;
		node->keys.push_front(*it_key);
		node->child.push_front(*it_child);
		it_key++;
	}
	node->updatechild();
	return node;
}

int BTree::tsize() {
	return nsize;
}

int BTree::getK()
{
	return this->k;
}

int BTree::height()
{
	int i = 0;
	BNode* p = root;
	while (p) {
		p = *(p->child.begin());
		++i;
	}
	return i;
}

BNode* BTree::getRoot()
{
	return this->root;
}

void BTree::deleteNode(BNode* p)
{
	if (!p)return;
	list<BNode*>::iterator it;
	for (it = p->child.begin(); it != p->child.end(); it++) {
		deleteNode(*it);
	}
	delete p;
}

BTree::BTree()
{
	k = 3;
	nsize = 0;
	maxKey = k - 1;
	minKey = (k - 1) / 2;
	root = NULL;
}

BTree::BTree(int k) :k(k)
{
	nsize = 0;
	maxKey = k - 1;
	minKey = (k - 1) / 2;
	root = NULL;
}

BTree::~BTree()
{
	deleteNode(root);
}

void BTree::count()
{
	int i = 0;
}
