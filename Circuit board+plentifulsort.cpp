#include <iostream>
#include <algorithm>
#include <fstream>
using namespace std;
const int N = 1e4 + 5;
int a[N], b[N], c[N], d[N * 2], e[N], f[N];
int rcount[10] = { 0 };
int ap[105][21] = { 0 };
int comp[5] = { 0 }, swapn[5] = { 0 };
void quicksort(int q[], int l, int r)
{
	if (l >= r) return;
	int x = q[rand() % (r - l + 1) + l], i = l - 1, j = r + 1;
	while (i < j)
	{
		do i++, comp[0]++; while (q[i] < x);
		do j--, comp[0]++; while (q[j] > x);
		if (i < j) {
			swap(q[i], q[j]);
			swapn[0]++;
		}
	}
	quicksort(q, l, j);
	quicksort(q, j + 1, r);
}
void quicksort1(int q[], int p[], int l, int r)
{
	if (l >= r) return;
	int x = q[rand() % (r - l + 1) + l], i = l - 1, j = r + 1;
	while (i < j)
	{
		do i++; while (q[i] < x);
		do j--; while (q[j] > x);
		if (i < j) {
			swap(q[i], q[j]);
			swap(p[i], p[j]);
		}
	}
	quicksort1(q, p, l, j);
	quicksort1(q, p, j + 1, r);
}
int tmp1[N];
void mergesort(int q[], int l, int r)
{
	if (l >= r) return;
	int mid = l + (r - l) / 2;
	mergesort(q, l, mid), mergesort(q, mid + 1, r);
	int i = l, j = mid + 1, k = 0;
	while (i <= mid && j <= r)
	{
		if (q[i] < q[j]) tmp1[k++] = q[i++];
		else tmp1[k++] = q[j++];
		comp[1] ++;
	}
	while (i <= mid) tmp1[k++] = q[i++];
	while (j <= r) tmp1[k++] = q[j++];
	for (int i = l, j = 0; i <= r; i++, j++) q[i] = tmp1[j];
}
const int increment = 3;
void Shellsort(int q[], int len)
{
	for (int gap = len >> 1; gap; gap >>= 1)
	{
		for (int i = gap; i < len; i++)
		{
			int x = q[i];
			int j = i;
			for (; j >= gap && q[j - gap] > x; j -= gap)
			{
				q[j] = q[j - gap];
				comp[2] ++; swapn[2]++;
			}			
			q[j] = x;
			swapn[2]++;
		}
	}
}
void down(int u, int q[], int len)
{
	int t = u;
	if (u << 1 <= len && q[u << 1] < q[t]) t = u << 1;
	if (u * 2 + 1 <= len && q[u * 2 + 1] < q[t]) t = u * 2 + 1;
	comp[3]++;
	if (u != t)
	{
		swap(q[u], q[t]);
		swapn[3]++;
		down(t, q, len);
	}
}
void heapsort(int q[], int len)
{
	for (int i = len / 2; i; i--) down(i, q, len);
}
int maxbit(int q[], int len)
{
	int maxv = q[0];
	for (int i = 1; i < len; i++)
		maxv = max(q[i], maxv);
	int cnt = 1;
	while (maxv >= 10) maxv /= 10, cnt++;
	return cnt;
}
void radixsort(int q[], int len)
{
	int t = maxbit(q, len);
	int radix = 1;
	int k;
	for (int i = 1; i <= t; i++)
	{
		for (int j = 0; j < 10; j++) rcount[j] = 0;
		for (int j = 0; j < len; j++)
		{
			k = (q[j] / radix) % 10;
			rcount[k] ++;
		}
		for (int j = 1; j < 10; j++) rcount[j] += rcount[j - 1];
		for (int j = len - 1; j >= 0; j--)
		{
			k = (q[j] / radix) % 10;
			tmp1[rcount[k] - 1] = q[j];
			rcount[k] --;
		}
		for (int j = 0; j < len; j++) q[j] = tmp1[j];
		radix *= 10;
	}
}
void quicksort1(int q[], int l, int r, int p[])
{
	if (l >= r) return;
	int x = q[rand() % (r - l + 1) + l], i = l - 1, j = r + 1;
	while (i < j)
	{
		do i++; while (q[i] < x);
		do j--; while (q[j] > x);
		if (i < j)
		{
			swap(q[i], q[j]);
			swap(p[i], p[j]);
		}
	}
	quicksort1(q, l, j, p);
	quicksort1(q, j + 1, r, p);
}
int duplicate(int q[], int n, int p[], int r[])
{
	int j = 0;
	for (int i = 0; i <= n; i++)
		if (!i || q[i] != q[i - 1]) {
			r[j] = r[i];
			q[j ++] = q[i];
		}
		else {
			p[j]++;
		}
	return j;
}
int main()
{
	ifstream infile("randomnum.txt");
	ofstream oufile1("quicksortresult.txt"), oufile2("mergesort.txt"), oufile3("Shellsort.txt"), oufile4("heapsort.txt"), oufile5("radixsort.txt");
	int n = 10000;
	if (!infile.is_open())
	{
		cout << "Failed to open the file!" << endl;
		return 0;
	}
	int x = 0, i = 0;
	while (!infile.eof())
	{
		infile >> x;
		f[i] = b[i] = c[i] = e[i] = d[i + 1] = a[i] = x;
		i++;
	}
	sort(f, f + 10000);
	quicksort(a, 0, n - 1);
	for (int i = 0; i < n; i++) {
		if (a[i] != f[i]) {
			cout << "快排出现错误" << endl;
			return 0;
		}
		oufile1 << a[i] << " ";
	}
	mergesort(b, 0, n - 1);
	for (int i = 0; i < n; i++) {
		if (b[i] != f[i]) {
			cout << "归并出现错误" << endl;
			return 0;
		}
		oufile2 << b[i] << " ";
	}
	Shellsort(c, n);
	for (int i = 0; i < n; i++)
	{
		if (c[i] != f[i]) {
			cout << "希尔出现错误" << endl;
			return 0;
		}
		oufile3 << c[i] << " ";
	}
	radixsort(e, n);
	for (int i = 0; i < n; i++)
	{
		if (e[i] != f[i]) {
			cout << "基数出现错误" << endl;
			return 0;
		}
		oufile5 << e[i] << " ";
	}
	int pt = 0;
	heapsort(d, n);
	while (1)
	{
		if (!n) break;
		if (f[pt ++] != d[1]) {
			cout << "桶排序出现错误" << endl;
			return 0;
		}
		oufile4 << d[1] << " ";
		d[1] = d[n];
		n--;
		down(1, d, n);
	}
	cout << "排序均与标准一致" << endl;
	for (int i = 0; i < 5; i++)
		cout << "第i种排序比较次数" << comp[i] << " " << "第i种排序的交换次数" << swapn[i] << endl;
	//第二个部分
	int N = 0, M = 0, K = 0;
	cin >> N >> M >> K;
	int count[100] = { 0 }, rc[100] = { 0 }, rp[100] = { 0 };
	for (int i = 0; i < N; i++)
	{
		int eps = 1;
		for (int j = 0; j < M; j++)
		{
			cin >> ap[i][j];
			if (ap[i][j] == 0) count[i] ++;
			rc[i] = eps * rc[i] + ap[i][j];
			eps *= 2;
		}
	}
	quicksort1(rc, 0, N - 1, count);
	for (int i = 0; i < N; i++) rp[i] = 1;
	int t = duplicate(rc, N - 1, rp, count);
	int l = 0, r = t;
	while (l < r)
	{
		int mid = (l + r + 1) >> 1;
		if (count[mid] > K) r = mid - 1;
		else l = mid;
	}
	int ri = l;
	int tmax = 0;
	for (int i = 0; i <= ri; i++)
	{
		if (K < count[i]) break;
		if ((K - count[i]) % 2 == 0) tmax = max(tmax, rp[i]);
	}
	cout << tmax;
}