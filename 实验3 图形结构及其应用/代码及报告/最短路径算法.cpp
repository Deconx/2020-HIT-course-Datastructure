#include<iostream>
#include<iomanip>
#include<fstream>
#include<time.h>
using namespace std;

#define inf 10000//���������
//�ڽӾ����ʾ����ͼ
#define VertexData int
#define EdgeData int
#define Numvertices 20
//�ڽӾ���
typedef struct {
	VertexData vertex[Numvertices];
	EdgeData edge[Numvertices][Numvertices];
	int n;
	int e;
} Graph;
//�ڽӱ�
typedef struct node { //�߱�
	int adjvex;//�ڽӵ��±�
	int cost;//Ȩֵ
	struct node*next;//��һ��ָ��
} EdgeNode;
typedef struct { //�����
	int vertex;//������Ϣ
	EdgeNode *firstedge;//������ͷָ��
} VertexNode;
typedef struct { //�ڽӱ�
	VertexNode vexlist[Numvertices];
	int n;
	int e;
} AdjGraph;
//�ѽṹ
//��С��
typedef struct {
	int num;
	int weight;
} ElemType;
typedef struct {
	ElemType data[Numvertices];
	int n;
} heap;
//�ѹ���
void InitHeap(heap& heap);//��ʼ��
bool isHeapEmpty(heap heap);//�п�
bool HeapFull(heap heap);//����
void HeapInsert(heap& heap, ElemType m);//����
ElemType HeapDeleteMin(heap& heap);//ɾ����С

void InitHeap(heap& heap) {
	heap.n = 0;
}
bool isHeapEmpty(heap heap) {
	return (!heap.n);
}
bool HeapFull(heap heap) {
	return (heap.n == Numvertices - 1);
}

void HeapInsert(heap& heap, ElemType m) {
	int i;
	if (!HeapFull(heap))
	{
		i = ++heap.n;
		while ((i != 1) && (m.weight < heap.data[i / 2].weight))
		{
			heap.data[i] = heap.data[i / 2];
			i /= 2;
		}
	}
	heap.data[i].num = m.num;
	heap.data[i].weight = m.weight;
}
ElemType HeapDeleteMin(heap& heap) {
	int parent = 1, child = 2;
	ElemType tmp, elem;
	if (!isHeapEmpty(heap))
	{
		elem = heap.data[1];
		tmp = heap.data[heap.n--];
		while (child <= heap.n)
		{
			if ((child < heap.n) && (heap.data[child].weight > heap.data[child + 1].weight))
				++child;
			if (tmp.weight <= heap.data[child].weight)
				break;
			heap.data[parent] = heap.data[child];
			parent = child;
			child *= 2;
		}
	}
	heap.data[parent] = tmp;
	return elem;
}
/*--------------------------------------------------------------*/
AdjGraph* ConverttoM(Graph *G) {
	AdjGraph* Gc = new AdjGraph;
	Gc->e = G->e;
	Gc->n = G->n;
	for (int i = 0; i < Gc->n; ++i) {
		Gc->vexlist[i].vertex = G->vertex[i];
		Gc->vexlist[i].firstedge = NULL;
	}
	EdgeNode* temp;
	for (int i = 0; i < G->n; ++i) {
		for (int j = 0; j < G->n; ++j) {
			if (G->edge[i][j]) {
				temp = Gc->vexlist[i].firstedge;//β��
				Gc->vexlist[i].firstedge = new EdgeNode;
				Gc->vexlist[i].firstedge->adjvex = j;
				Gc->vexlist[i].firstedge->cost = G->edge[i][j];//Ȩֵ
				Gc->vexlist[i].firstedge->next = temp;
			}
		}
	}
	return Gc;
}
void CreatGraph(Graph *G) {
	ifstream file("Graph.txt");
	file >> G->n;
	file >> G->e;
	for (int i = 0; i < G->n; ++i) { //��ʼ��
		for (int j = 0; j < G->n; ++j) {
			G->edge[i][j] = inf;
		}
	}
	for (int i = 0; i < G->n; ++i) { //�Խ�����1
		G->edge[i][i] = 0;
	}
	for (int i = 0; i < G->n; ++i) {
		file >> G->vertex[i];
	}
	for (int i = 0; i < G->e; i++) {
		int a, b, weight;
		file >> a >> b >> weight;
		G->edge[a][b] = weight;
	}
}
int selectmin(Graph* G, int d[], bool s[]) { //V-s���ҵ�wʹd[w]��С
	int temp = inf;
	int w = 0;
	for (int i = 0; i < G->n; i++) {
		if (!s[i] && d[i] < temp) {
			temp = d[i];
			w = i;
		}
	}
	return w;
}
//Dijkstra�㷨
void Dijkstra(Graph *G, int start, int d[], int p[], bool s[]) {
	for (int i = 0; i < G->n; ++i) {
		d[i] = G->edge[start][i]; //��ʼ��d
		s[i] = false;
		p[i] = start;
	}
	s[start] = true;
	for (int i = 0; i < G->n - 1; ++i) {
		int w = selectmin(G, d, s);
		s[w] = true;
		for (int v = 0; v < G->n; ++v) {
			if (!s[v]) {
				int sum = d[w] + G->edge[w][v];
				if (sum < d[v]) {
					d[v] = sum;
					p[v] = w;
				}
			}
		}
	}
}
//Dijkstra�㷨���Ż�
void Dijkstra_heap(AdjGraph *A, int start, int d[], int p[], bool s[]) {
	heap heap;
	InitHeap(heap);
	for (int i = 0; i < A->n; ++i) {
		d[i] = inf;
		s[i] = false;
		p[i] = start;
	}
	d[start] = 0;
	ElemType temp;
	temp.num = start;
	temp.weight = d[start];
	HeapInsert(heap, temp);
	while (!isHeapEmpty(heap)) {
		temp = HeapDeleteMin(heap);
		int v = temp.num;//��ǰ��
		if (s[v]) continue;
		s[v] = true;
		EdgeNode* ptr;
		for (ptr = A->vexlist[v].firstedge; ptr != NULL; ptr = ptr->next) {
			if (d[ptr->adjvex] > d[v] + ptr->cost) {
				d[ptr->adjvex] = d[v] + ptr->cost;
				temp.num = ptr->adjvex;
				temp.weight = d[ptr->adjvex];
				p[ptr->adjvex] = v;
				HeapInsert(heap, temp);
			}
		}
	}
}
//Floyd�㷨
void Floyd(int d[][Numvertices], Graph* G, int p[][Numvertices]) {
	for (int i = 0; i < G->n; ++i) {
		for (int j = 0; j < G->n; ++j) {
			d[i][j] = G->edge[i][j];
			p[i][j] = -1;//p��¼�м�ڵ�
		}
	}
	for (int k = 0; k < G->n; k++) {
		for (int i = 0; i < G->n; ++i) {
			for (int j = 0; j < G->n; j++) {
				if (d[i][k] + d[k][j] < d[i][j]) {
					d[i][j] = d[i][k] + d[k][j];
					p[i][j] = k;
				}
			}
		}
	}
}
//��ӡ·���㷨
void printroad(Graph* G, int start, int d[], int p[]) {
	for (int i = 0; i < G->n; ++i) {
		cout << start << "->" << i << ":" << endl;
		int temp = i;//��¼·���ڵ�
		int r[Numvertices];//����·���ڵ�
		int j = 0;
		if (d[i] == inf) {
			cout << "��·����" << endl;
			cout << endl;
		}
		else if (i == start) {
			cout << "·��";
			cout << start << "->" << start << endl;
			cout << "���룺" << endl;
			cout << 0 << endl;
			cout << endl;
		}
		else {
			while (temp != start) {
				r[j++] = temp;
				temp = p[temp];
			}
			r[j++] = start;
			cout << "·����";
			while (j--) {
				cout << r[j];
				if (j != 0)
					cout << "->";
			}
			cout << endl;
			cout << "���룺" << d[i] << endl;
			cout << endl;
		}
	}
}
void printroad_A(AdjGraph* G, int start, int d[], int p[]) {
	for (int i = 0; i < G->n; ++i) {
		cout << start << "->" << i << ":" << endl;
		int temp = i;//��¼·���ڵ�
		int r[Numvertices];//����·���ڵ�
		int j = 0;
		if (d[i] == inf) {
			cout << "��·����" << endl;
			cout << endl;
		}
		else if (i == start) {
			cout << "·��";
			cout << start << "->" << start << endl;
			cout << "���룺" << endl;
			cout << 0 << endl;
			cout << endl;
		}
		else {
			while (temp != start) {
				r[j++] = temp;
				temp = p[temp];
			}
			r[j++] = start;
			cout << "·����";
			while (j--) {
				cout << r[j];
				if (j != 0)
					cout << "->";
			}
			cout << endl;
			cout << "���룺" << d[i] << endl;
			cout << endl;
		}
	}
}
//Dijkstra�㷨��ز���
void ManageDijkstra(int mode) {
	Graph *G = new Graph;
	CreatGraph(G);
	AdjGraph *A = ConverttoM(G);
	int d[Numvertices];//Դ��0������i�ĵ�ǰ���·������
	int p[Numvertices];//���·����󾭹��ĵ�
	bool s[Numvertices];
	int start;
	cout << "������Դ��:" << endl;
	cin >> start;
	if (mode == 1) {
		Dijkstra(G, start, d, p, s);
		printroad(G, start, d, p);
	}
	else {
		Dijkstra_heap(A, start, d, p, s);
		printroad_A(A, start, d, p);
		//ʵ��Ƚ�ʱ��
		const int Times = 100000;
		clock_t _start, _stop;
		int m = Times;
		_start = clock();
		while (m--) {
			Dijkstra(G, start, d, p, s);
		}
		_stop = clock();
		double duration1 = (double)(_stop - _start) / CLK_TCK;
		m = Times;
		_start = clock();
		while (m--) {
			Dijkstra_heap(A, start, d, p, s);
		}
		_stop = clock();
		double duration2 = (double)(_stop - _start) / CLK_TCK;
		cout << "ԭʼDijkstra�㷨����" << Times << "�ε�ʱ��Ϊ��" << endl;
		cout << duration1 << endl;
		cout << "���Ż�Dijkstra�㷨����" << Times << "�ε�ʱ��Ϊ��" << endl;
		cout << duration2 << endl;
	}

}
//���·������õ���������·��(�ݹ�)
void printpath(int p[][Numvertices], int i , int j) {
	int k = p[i][j];
	if (k != -1) {
		printpath(p, i, k);
		cout << k << "->";
		printpath(p, k, j);
	}
}
//�������2��Floyd�㷨,�������
void PrintAns2Floyd(Graph* G, int d[][Numvertices], int p[][Numvertices]) {
	cout << "��̾������Ϊ��" << endl;
	for (int i = 0; i < G->n; i++) {
		for (int j = 0; j < G->n; j++) {
			cout << setw(6) << d[i][j];
		}
		cout << endl;
	}
	cout << "���·������Ϊ(-1��ʾֱ�ӵ���)��" << endl;
	for (int i = 0; i < G->n; i++) {
		for (int j = 0; j < G->n; j++) {
			cout << setw(4) << p[i][j] << " ";
		}
		cout << endl;
	}
	cout << "���е�����·��Ϊ:" << endl;
	for (int i = 0; i < G->n; ++i) {
		for (int j = 0; j < G->n; ++j) {
			if (i != j) {
				cout << i << "->" << j << ":" << endl;
				cout << "·����";
				cout << i << "->";
				printpath(p, i, j);
				cout << j;
				cout << endl;
				cout << "���룺" << d[i][j] << endl;
				cout << endl;
			}
		}
	}
}
//�������3��Floyd�㷨,ָ���յ�
void PrintAns3Floyd(Graph *G, int d[][Numvertices], int p[][Numvertices]) {
	cout << "�������յ�:" << endl;
	int c;
	cin >> c;
	for (int i = 0; i < G->n; ++i) {
		while (d[i][c] == inf) {
			cout << i << "->" << c << ":" << endl;
			cout << "��·����" << endl;
			cout << endl;
			i++;
		}
		cout << i << "->" << c << ":" << endl;
		cout << "·����";
		cout << i << "->";
		printpath(p, i, c);
		cout << c;
		cout << endl;
		cout << "���룺" << d[i][c] << endl;
		cout << endl;
	}
}
//�������4��Floyd�㷨,ָ�������յ�
void PrintAns4Floyd(Graph *G, int d[][Numvertices], int p[][Numvertices]) {
	cout << "��ֱ���������:" << endl;
	int u, v;
	cin >> u >> v;
	if (d[u][v] == inf) {
		cout << u << "->" << v << ":" << endl;
		cout << "��·����" << endl;
	}
	else {
		cout << u << "->" << v << ":" << endl;
		cout << "·����";
		cout << u << "->";
		printpath(p, u, v);
		cout << v;
		cout << endl;
		cout << "���룺" << d[u][v] << endl;
		cout << endl;
	}
	if (d[v][u] == inf) {
		cout << v << "->" << u << ":" << endl;
		cout << "��·����" << endl;
		return;
	}
	else {
		cout << v << "->" << u << ":" << endl;
		cout << "·����";
		cout << v << "->";
		printpath(p, v, u);
		cout << u;
		cout << endl;
		cout << "���룺" << d[v][u] << endl;
		cout << endl;
	}
}
//Floyd�㷨��ز���
void ManageFloyd(int mode) {
	Graph *G = new Graph;
	CreatGraph(G);
	int d[Numvertices][Numvertices];//��̾������
	int p[Numvertices][Numvertices];//���·������
	Floyd(d, G, p);
	if (mode == 2)
		PrintAns2Floyd(G, d, p);
	else if (mode == 3)
		PrintAns3Floyd(G, d, p);
	else
		PrintAns4Floyd(G, d, p);
}
//Warshall�㷨
void Warshall() {
	Graph *G = new Graph;
	CreatGraph(G);
	int nn[Numvertices][Numvertices];
	//�ڽӾ����ʼ����
	for (int i = 0; i < G->n; ++i) {
		for (int j = 0; j < G->n; j++) {
			if (G->edge[i][j] == inf)
				nn[i][j] = 0;
			else
				nn[i][j] = 1;
		}
	}
	for (int k = 0; k < G->n; k++) {
		for (int i = 0; i < G->n; ++i) {
			for (int j = 0; j < G->n; j++) {
				if (nn[i][k] && nn[k][j]) {
					nn[i][j] = 1;
				}
			}
		}
	}
	cout << "�ɴ����Ϊ��" << endl;
	for (int i = 0; i < G->n; ++i) {
		for (int j = 0; j < G->n; ++j) {
			cout << nn[i][j] << " ";
		}
		cout << endl;
	}
}
void test() {
	while (1) {
		cout << "1����Դ���·����Dijkstra�㷨" << endl;
		cout << "2��ȫ�����·����Floyd-Warshall�㷨" << endl;
		cout << "3��ָ���������·��" << endl;
		cout << "4��ָ��һ�Ե�ĶԼ����·��" << endl;
		cout << "5��Warshall�㷨��ɴ����" << endl;
		cout << "6���ѽṹʵ��Dijkstra�㷨,ʵ��Ƚ�ʱ��" << endl;
		int mode;
		cout << "������ģʽ��" << endl;
		cin >> mode;
		switch (mode)
		{
		case 1:
			ManageDijkstra(1);
			break;
		case 2:
			ManageFloyd(2);
			break;
		case 3:
			ManageFloyd(3);
			break;
		case 4:
			ManageFloyd(4);
			break;
		case 5:
			Warshall();
			break;
		case 6:
			ManageDijkstra(2);
		}

	}
}
int main(void) {
	test();
	return 0;
}
