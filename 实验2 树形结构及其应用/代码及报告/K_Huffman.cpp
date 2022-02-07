//�����ַ�����
//���Ż�
//K��������
#include <iostream>
#include <fstream>
#include <string>
#include <string.h>
#include <sys/stat.h>
#include <iomanip>
using namespace std;

#define ASC 128
#define m 2*ASC-1//2n-1���ڵ�

#define Max_K 9

int k;//kֵ
int totalch;//�ַ�������
int nn;//��0֮����ַ�������
long long countch;//���ַ���

typedef struct {
	double weight;
	int child[Max_K];//K���ӽڵ�
	int parent;
} HTNode;

typedef struct {
	char ch;//�ַ�
	double freq;//�ַ�Ƶ��
	char code[m];//����������
} Orchar;

HTNode HTree[m];
Orchar Or[ASC];


//�ѽṹ
//��С��
typedef struct {
	int num;//��Orword��Ӧ
	double weight;
} ElemType;
typedef struct{
	ElemType data[m];
	int n;
}heap;

//�ѹ���
void InitHeap(heap& heap);//��ʼ��
bool isHeapEmpty(heap heap);//�п�
bool HeapFull(heap heap);//����
void HeapInsert(heap& heap, ElemType t);//����
ElemType HeapDeleteMin(heap& heap, int& num);//ɾ����С

void Initheap(heap& heap){
	heap.n = 0;
}
bool isHeapEmpty(heap heap){
	return (!heap.n);
}
bool HeapFull(heap heap){
	return (heap.n == m - 1);
}

void HeapInsert(heap& heap, ElemType t){
	int i;
	if (!HeapFull(heap))
	{
		i = ++heap.n;
		while ((i != 1) && (t.weight < heap.data[i / 2].weight))
		{
			heap.data[i] = heap.data[i / 2];
			i /= 2;
		}
	}
	heap.data[i].num = t.num;
	heap.data[i].weight = t.weight;
}
ElemType HeapDeleteMin(heap& heap){
	int parent = 1, child = 2;
	ElemType tmp,elem;
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
void CreatHeap(heap& heap){
	Initheap(heap);
	for(int i = 0;i<totalch;i++){
		ElemType t;
		t.num = i;
		t.weight = Or[i].freq;
		HeapInsert(heap,t);
	}
}

//���ö��� K ����Сֵ
void SelectMinword(heap& heap,int p[]) {
	for(int i = 0;i<k;i++){
		p[i] = HeapDeleteMin(heap).num;
		//cout<<p[i]<<endl;
	}
}

//���ļ�
long long ReadFile(){
	ifstream file("InputText.txt");
	if (!file) {
		cout << "Failed To Open The File!" << endl;
		return 0;
	}
	int List[ASC] = {0};//Ƶ��
	char ch;
	long long i = 0;//���ַ���
	int total = 0;//�ַ�������
	while (file.get(ch)) {
		List[(int)ch]++;
		i++;
	}
	file.close();
	int p = 0;
	for (int k = 0; k < ASC; ++k) {
		if (List[k]) {
			total++;
			Or[p].ch = k;
			Or[p].freq = (double)List[k] / i;
			p++;
		}
	}
	totalch = total;//�ַ�������
	return i;
}
//��ʼ���ڵ�ֵ
void InitHTree(){
	for (int i = 0; i < totalch; ++i)
		HTree[i].weight = Or[i].freq;
	for (int i = totalch; i < 2*totalch-1; ++i)
		HTree[i].weight = 0;
	for (int i = 0; i < m; ++i){
		HTree[i].parent = -1;
		for (int j = 0; j < k; ++j){
			HTree[i].child[j] = -1;
		}
	}
}
//������
void CreatHTree() {
	int i = totalch;
	InitHTree();
	heap heap;
	CreatHeap(heap);
	int n = totalch;
	//��һ��ȡ k ���������൱��ȡk-1������ʣ�ಿ�ֲ�0
	if ((n - 1) % (k - 1)) {
		for (int i = 0; i < (k - 1) - (n - 1) % (k - 1); ++i){
			ElemType t;
			t.num = -1;
			t.weight = 0;
			HeapInsert(heap, t);
		}
	}
	//������ n + (k - 1) - (n - 1) % (k - 1)��Ҷ�ڵ�
	nn = n + (k - 1) - (n - 1) % (k - 1);
	//��Ҷ�ڵ��� (nn-1)/(k-1) ��
	for (i = n; i < n + (nn-1)/(k-1) - 1; ++i){
		int p[Max_K];//��СȨֵ�ڵ�
		SelectMinword(heap,p);
		for (int j = 0; j < k; ++j){
			HTree[p[j]].parent = i;
			HTree[i].child[j] = p[j];
			HTree[i].weight += HTree[p[j]].weight;
		}
		ElemType t;
		t.num = i;
		t.weight = HTree[i].weight;
		HeapInsert(heap,t);
	}
}
//����
void SetHuffmanEncoding() {
	int c, p, i; //cp�ֱ�Ϊ���Ӻ�˫�׵�λ��
	char cd[m + 1]; //��ʱ��ű���
	cd[totalch] = '\0';
	int start;//������cd�е�λ��
	for (i = 0; i < totalch; ++i) {
		start = totalch;
		c = i;//c��������Ҷ�ӽڵ�
		//cout<<HTree[c].parent<<endl;
		while ((p = HTree[c].parent) >= 0) {
			int j;
			for(j = 0;j<k;j++){
				if(HTree[p].child[j] == c)
					break;
			}
			cd[--start] = j+'0';//��j������
			c = p;//����
		}
		strcpy(Or[i].code, &cd[start]);
	}
}
void test(){
	countch = ReadFile();
	CreatHTree();
	SetHuffmanEncoding();
	printf("-------------------- %d-����������ѹ��--------------------",k);
	for (int i = 0; i < totalch; ++i){
		cout<< Or[i].ch<<":"<<endl;
		cout<<"Ƶ��:"<<Or[i].freq<<"   "<<"���룺"<<Or[i].code<<endl;
	}
}
int main(void){
	cout<<"������ K ֵ��"<<endl;
	cin>>k;
	test();
}
