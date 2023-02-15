#include <iostream>
#include <easyx.h>
#include <algorithm>
#include <cstring>
#include <queue>
#include <graphics.h>
#include <iomanip>
using namespace std;
typedef pair <int, int> PII;
const int N = 1e3 + 5;
int n = 0, T = 0, num = 0;
int WIN_WIDTH = 800, WIN_HEIGHT = 800;
float eps = 0;
char g[N][N];
int d[N][N];//d������������, �������Ϊ-2�����, -1ʱҺ��,>= 0Ϊ����
int dx[4] = { 0,1,0,-1 }, dy[4] = { 1,0,-1,0 };
bool check[N][N] = { 0 };
//�����Һ���num[i][j] = 1, ���num[i][j] = 0, ����num[i][j] = -1
queue<PII> q, rp;
class map {
public:
	int num[N][N];
	void paint()
	{
		initgraph(WIN_WIDTH, WIN_HEIGHT, SHOWCONSOLE);
		setlinecolor(GREEN);
		HWND hwnd = GetHWnd();
		MoveWindow(hwnd, 0, 0, WIN_WIDTH, WIN_HEIGHT, TRUE);
		cleardevice();
		for (int i = 0; i < n; i++)
			for (int j = 0; j < n; j++)
				if (num[i][j] == 0)
				{
					setfillcolor(RED);
					solidrectangle(j * eps, i * eps, (j + 1) * eps, (i + 1) * eps);
				}
				else if (num[i][j] == -1) {
					setfillcolor(BLUE);
					solidrectangle(j * eps, i * eps, (j + 1) * eps, (i + 1) * eps);
				}
				else if (num[i][j] == 1) {
					setfillcolor(GREEN);
					solidrectangle(j * eps, i * eps, (j + 1) * eps, (i + 1) * eps);
				}
				else if (num[i][j] == -1) {
					setfillcolor(YELLOW);
					solidrectangle(j * eps, i * eps, (j + 1) * eps, (i + 1) * eps);
				}
		for (int i = 0; i < n; i++)
			line(i * eps, 0, i * eps, WIN_HEIGHT);
		for (int i = 0; i < n; i++)
			line(0, i * eps, WIN_WIDTH, i * eps);
	}
}pic;
void dfsliquid(int i, int j, map &a, bool flag)
{
	check[i][j] = true;
	pic.num[i][j] = 1;
	for (int t = 0; t < 4; t++)
	{
		int x = i + dx[t], y = j + dy[t];
		if (x < n && y < n && x >= 0 && y >= 0)
		{
			if (check[x][y] && d[x][y] < 0) continue;
			if (d[x][y] != -1) d[x][y]++;
			if (d[x][y] == -1)
			{
				pic.num[x][y] = 1;
				dfsliquid(x, y, a, flag);
			}
			if (d[x][y] == 2)
			{
				if (!check[x][y]) {
					if (!flag)
						rp.push({ x, y });
					else q.push({ x, y });
					num--;
				}
				check[x][y] = true;
				pic.num[x][y] = -3;
			}
		}
	}
	return;
}
void process(map &a)
{
	int t[2] = { 0, n - 1 };
	for (int j = 0; j < 2; j++)
		for (int i = 0; i < n; i++)
			if (d[t[j]][i] == -1 && !check[t[j]][i])
				dfsliquid(t[j], i, a, 1);
	for (int j = 0; j < 2; j++)
		for (int i = 0; i < n; i++)
			if (d[i][t[j]] == -1 && !check[i][t[j]])
				dfsliquid(i, t[j], a, 1);
}
bool unmelted(map &a)
{
	queue<PII> tmp;
	while (!q.empty())
	{
		auto t = q.front(); q.pop();
		int x = t.first, y = t.second;
		for (int i = 0; i < 4; i++)
		{
			int x1 = x + dx[i], y1 = y + dy[i];
			if (d[x1][y1] == -1) continue;
			d[x1][y1]++;
			if (d[x1][y1] == -1) dfsliquid(x1, y1, a, 0);
			else if (d[x1][y1] == 2 && !check[x1][y1])
			{
				check[x1][y1] = true;
				pic.num[x1][y1] = -3;
				tmp.push({ x1,y1 });
				num--;
			}
		}
	}
	q.swap(tmp);
	while (!rp.empty())
	{
		q.push(rp.front()); rp.pop();
	}
	if (!q.empty()) return true;
	return false;
}
int main()
{
	cin >> n;
	int t = 0;
	eps = WIN_WIDTH / n;
	for (int i = 0; i < n; i++)
		for (int j = 0; j < n; j++)
		{
			cin >> g[i][j];
			if (g[i][j] == '_') {
				pic.num[i][j] = (i == 0 || j == 0 || i == n - 1 || j == n - 1) ? 1 : 0; //���ϵ�Һ����Ϊ����絼ͨ
				d[i][j] = (i == 0 || j == 0 || i == n - 1 || j == n - 1) ? -1 : -2; // ����絼ͨ��Һ�����Ϊ-1, ������������Ϊ-2, ���������ͨ��Ҫ��һ������,ģ����ɢ
			}
			else {
				num++;
				pic.num[i][j] = -1;
				d[i][j] = 0;
			}
		}
	pic.paint();
	getchar();
	process(pic);
	getchar();
	pic.paint();
	while (unmelted(pic))
	{
		t++;
		getchar();
		pic.paint();
	}
	t++;
	pic.paint();
	if (num != 0) cout << "ERROR" << endl;
	else {
		cout << t << "��ʱ�䵥λ" << endl;
	}
	system("pause");
	closegraph();
}