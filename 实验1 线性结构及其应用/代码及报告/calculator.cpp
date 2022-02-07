#include <iostream>
using namespace std;
const int N = 6;
typedef struct PolyNode *PP;
struct PolyNode
{
    float coef;
    int expon;
    PP next;
};
//ɾ��ͷ�ڵ�
void deletehead(PP &p)
{
    PP t = new struct PolyNode;
    t = p;
    p = p->next;
    delete t;
}
//β�巨������ָ�����
void Attach(float c, int e, PP &Rear)
{
    PP P= new struct PolyNode;
    P->coef = c;
    P->expon = e;
    P->next = NULL;
    Rear->next = P;//�µĽڵ�����
    Rear = P;//β�ڵ����
}
//���ݣ��鲢
PP Sort(PP &p)
{
    if(p==NULL||p->next==NULL)
        return p;
    PP mid = p,right = p->next;
    while(right&&right->next)
    {
        right = right->next->next;
        mid = mid->next;
    }//mid���м�
    PP i = p,j = mid->next;
    mid->next = NULL;
    i = Sort(i);
    j = Sort(j);
    PP res = new PolyNode;
    PP Rear = res;
    while(i&&j)
    {
        if(i->expon>=j->expon)
        {
            Attach(i->coef,i->expon,Rear);
            i = i->next;
        }
        else
        {
            Attach(j->coef,j->expon,Rear);
            j = j->next;
        }
    }
    while(i)
    {
        Attach(i->coef,i->expon,Rear);
        i = i->next;
    }
    while(j)
    {
        Attach(j->coef,j->expon,Rear);
        j = j->next;
    }
    deletehead(res);
    return res;
}
//�ļ��ж������ʽ,�����ظ�ָ��
PP ReadPoly(FILE *ff)
{
    int e;
    float c;
    char ch;
    PP p = new struct PolyNode;
    p->next = NULL;
    PP Rear = p;
    ch = fgetc(ff);
    //��һ������з���Ҳ����û�з���
    if (ch != '-')
    {
        rewind(ff);
        fscanf(ff,"%fx^%d",&c,&e);
        Attach(c,e,Rear);
    }
    else
    {
        fscanf(ff,"%fx^%d",&c,&e);
        Attach(c,e,Rear);
    }
    while (fscanf(ff,"%c%fx^%d",&ch,&c,&e) != EOF)
    {
        if (ch == '+')
        {
            Attach(c,e,Rear);
        }
        if (ch == '-')
        {
            Attach(-c,e,Rear);
        }
    }
    Rear->next = NULL;
    deletehead(p);
    return p;
}
//����ʽ�ӷ�
PP Add(PP P1,PP P2)
{
    if(!P1||!P2)
        cout<<"�ӷ����㴫�����"<<endl;
    PP t1 = P1;
    PP t2 = P2;
    PP P;
    PP Rear;
    P = new struct PolyNode;
    P->next = NULL;
    Rear = P;
    while(t1&&t2)
    {
        if(t1->expon==t2->expon)
        {
            if(t1->coef+t2->coef!=0)
                Attach(t1->coef+t2->coef,t1->expon,Rear);
            t1=t1->next;
            t2=t2->next;
        }
        else if(t1->expon>=t2->expon)
        {
            Attach(t1->coef,t1->expon,Rear);
            t1=t1->next;
        }
        else
        {
            Attach(t2->coef,t2->expon,Rear);
            t2=t2->next;
        }
    }
    while(t1)
    {
        Attach(t1->coef,t1->expon,Rear);
        t1 = t1->next;
    }
    while(t2)
    {
        Attach(t2->coef,t2->expon,Rear);
        t2 = t2->next;
    }
    Rear->next=NULL;
    deletehead(P);
    return P;
}
//����ʽ�˷�
PP Mult(PP P1,PP P2)
{
    if(!P1||!P2)
        cout<<"�˷����㴫�����"<<endl;
    PP P,Rear,t1,t2,t;
    float c;
    int e;
    t1=P1;
    t2=P2;
    P = new struct PolyNode;
    P->next = NULL;
    Rear=P;
    while(t2) //P1��1�����P2������ΪP
    {
        Attach(t1->coef*t2->coef,t1->expon+t2->expon,Rear);
        t2=t2->next;
    }
    t1=t1->next;
    while(t1)
    {
        t2=P2;
        Rear = P;
        while(t2)
        {
            e=t1->expon+t2->expon;
            c=t1->coef*t2->coef;
            while(Rear->next&&Rear->next->expon>e)
                Rear = Rear->next;//�ҵ�Ҫ�����λ��
            if(Rear->next&&Rear->next->expon==e)
            {
                if(Rear->next->coef+c)
                    Rear->next->coef+=c;
                else
                {
                    t=Rear->next;
                    Rear->next = t->next;
                    delete t;//���֮��ϵ��Ϊ0�Ľڵ�
                }
            }
            else //���µĲ������
            {
                t=new struct PolyNode;
                t->coef = c;
                t->expon=e;
                t->next = Rear->next;
                Rear->next = t;
                Rear=Rear->next;
            }
            t2=t2->next;
        }
        t1=t1->next;
    }
    deletehead(P);
    return P;
}
//����ʽ����(P1-P2)
PP Subtract(PP P1,PP P2)
{
    if(!P1||!P2)
        cout<<"�������㴫�����"<<endl;
    PP t1 = P1,t2 = P2,Rear;
    PP sub = new struct PolyNode;
    sub->next = NULL;
    Rear = sub;
    while(t1&&t2)
    {
        if(t1->expon==t2->expon)
        {
            if(t1->coef-t2->coef!=0)
            {
                Attach(t1->coef-t2->coef,t1->expon,Rear);
            }
            t1=t1->next;
            t2=t2->next;
        }
        else if(t1->expon>=t2->expon)
        {
            Attach(t1->coef,t1->expon,Rear);
            t1=t1->next;
        }
        else
        {
            Attach(t2->coef,t2->expon,Rear);
            t2=t2->next;
        }
    }
    while(t1)
    {
        Attach(t1->coef,t1->expon,Rear);
        t1 = t1->next;
    }
    while(t2)
    {
        Attach(-t2->coef,t2->expon,Rear);
        t2 = t2->next;
    }
    Rear->next=NULL;
    deletehead(sub);
    return sub;
}
//����
//p1 = p2*ress+resy
//ressΪ�̶���ʽ��resyΪ��������ʽ
void Division(PP p1, PP p2, PP& ress, PP& resy)
{
    if(!p1||!p2)
        cout<<"�������㴫�����"<<endl;
	PP p = p1;
	PP q = p2;
	PP temp1, temp2;
	temp1 = new struct PolyNode;
	PP Rear = temp1;
	Rear->next = NULL;
	while(p->expon>=q->expon){
        Attach(p->coef/q->coef,p->expon-q->expon,Rear);
        temp2 = Mult(temp1,p2);
        p1 = Subtract(p1,temp2);
        p = p1;
	}
	ress = temp1;
	temp2 = Mult(p2,temp1);
	resy = Subtract(p1,temp2);
    Sort(ress);
    Sort(resy);
}
//��
void Deri(PP p)
{
    if(!p)
        return;
    PP t = p;
    while(t){
        t->coef = t->coef*t->expon;
        t->expon--;
        t = t->next;
    }
    PP pre = p;
    t = p->next;
    while(t){
        if(t->coef==0){
            pre->next = t->next;
            delete t;
            t = pre->next;
        }
        else{
            t = t->next;
            pre = pre->next;
        }

    }
}
//�ؾ����㷨��ֵ
float cal(PP p,float x){
    int Max = p->expon;
    float a[Max+1]={0};
    while(p){
        a[p->expon] = p->coef;
        p = p->next;
    }
    int k = 1;
    float s = a[Max];
    for(int i = Max; i > 0; i--){
        s = a[i-1] + x * s;
    }
    return s;
}
//�������ʽ
void PrintPoly(PP P)
{
    if(!P)
    {
        cout<<"0"<<endl;
        return;
    }
    cout<<P->coef<<"x^"<<P->expon;
    P = P->next;
    while(P)
    {
        if(P->coef>0)
            cout<<"+";
        cout<<P->coef<<"x^"<<P->expon;
        P=P->next;
    }
    cout<<"\n";
}
//����ļ�
void OutFile(PP p){
    if (!p){
        cout<<"�����쳣��"<<endl;
        return;
    }
    FILE *fp = NULL;
    fp = fopen("out.txt","a");
    char ch;
    ch = getc(fp);
    if (ch != '-'){
        rewind(fp);
        fprintf(fp,"%.3fx^%d",p->coef,p->expon);
    }
    else{
        rewind(fp);
        fprintf(fp,"-%.3fx^%d",p->coef,p->expon);
    }
    p = p->next;
    while (p){
        if (p->coef>0){
            fprintf(fp,"+%.3fx^%d",p->coef,p->expon);
        }
        else{
            fprintf(fp,"-%.3fx^%d",p->coef,p->expon);
        }
        p = p->next;
    }
    fprintf(fp,"\n");
    fclose(fp);
}
int main(void)
{
    while(1)
    {
        //���������������ʽ
        PP P1,P2;
        FILE *fp1 = fopen("in1.txt","r");
        FILE *fp2 = fopen("in2.txt","r");
        if(fp1==NULL)
        {
            cout<<"���ļ�in1.txtʧ�ܣ�"<<endl;
            exit(1);
        }
        else
            P1 = ReadPoly(fp1);
        if(fp1==NULL)
        {
            cout<<"���ļ�in2.txtʧ�ܣ�"<<endl;
            exit(1);
        }
        else
            P2 = ReadPoly(fp2);
        fclose(fp1);
        fclose(fp2);

        P1 = Sort(P1);
        P2 = Sort(P2);
        //�洢���
        PP ans[N] = {NULL};
        int mode;//ģʽѡ��
        float x;//����x�Ķ���ʽֵ
        float anss;
        int k;//����k�׵�
        PP ress = new struct PolyNode;
        PP resy = new struct PolyNode;
        cout<<endl;
        cout<<"     һԪ����ʽ������     "<<endl;
        cout<<"��ѡ����Ҫ���еĲ�����"<<endl;
        cout<<"1.�ӷ�������in1+in2"<<endl;
        cout<<"2.�˷�������in1*in2"<<endl;
        cout<<"3.����������in1-in2"<<endl;
        cout<<"4.����������in1/in2"<<endl;
        cout<<"5.�󵼣�����in1��k�׵�"<<endl;
        cout<<"6.��ֵ������in1��x����ֵ"<<endl;
        cout<<"7.�˳�"<<endl;
        cout<<endl;
        cin>>mode;
        switch(mode)
        {
        case 1:
            ans[1] = Add(P1,P2);
            cout<<"���Ϊ��"<<endl;
            PrintPoly(ans[1]);
            OutFile(ans[1]);
            break;
        case 2:
            ans[2] = Mult(P1,P2);
            cout<<"���Ϊ��"<<endl;
            PrintPoly(ans[2]);
            OutFile(ans[2]);
            break;
        case 3:
            ans[3] = Subtract(P1,P2);
            cout<<"���Ϊ��"<<endl;
            PrintPoly(ans[3]);
            OutFile(ans[3]);
            break;
        case 4:
            Division(P1,P2,ress,resy);
            cout<<"���Ϊ��"<<endl;
            cout<<"�̶���ʽ��"<<endl;
            PrintPoly(ress);
            OutFile(ress);
            cout<<"��������ʽ��"<<endl;
            PrintPoly(resy);
            OutFile(resy);
            break;
        case 5:
            cout<<"��������Ҫ�󵼵Ľ�����"<<endl;
            cin>>k;
            if(k>P1->expon)
                cout<<"0"<<endl;
            else{
            ans[5] = P1;
            while(k--)
            {
                Deri(ans[5]);
                PrintPoly(ans[5]);
            }
            cout<<"���Ϊ��"<<endl;
            PrintPoly(ans[5]);
            }
            break;
        case 6:
            cout<<"��������Ҫ�������"<<endl;
            cin>>x;
            anss = cal(P1,x);
            cout<<"���Ϊ��"<<endl;
            cout<<anss<<endl;
            break;
        case 7:
            return 0;
        }
        getchar();
        getchar();
    }
    return 0;
}
