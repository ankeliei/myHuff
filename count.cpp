#include "myHuff.h"
using namespace std;


struct huffman
{
    int n;                  //节点前驱
    unsigned char data;     //节点中存放的数据
    struct huffman *p;      //指向父节点
    struct huffman *l;      //指向左节点
    struct huffman *r;      //指向右节点
    struct huffman *next;   //后驱指针
};

struct huffman *head = NULL;

void sort(struct huffman **h){                //传入head指针的地址
    head = *h;
    struct huffman *p = (struct huffman *)malloc(sizeof(struct huffman));
    p->next = head;
    for( ;p->next->next!=NULL;){
        if(p->next->n > p->next->next->n) {
            struct huffman *tmp1 = p->next;
            struct huffman *tmp2 = p->next->next;
            if(p->next == head){
                head = tmp2;
                tmp1->next = tmp2->next;
                tmp2->next = tmp1;
                p = head;
            }
            else {    
                p->next = tmp2;
                tmp1->next = tmp2->next;
                tmp2->next = tmp1;
                p = p->next;
            }
        }
        else{
            p = p->next;
        }
    }
}

void count(unsigned char *buf)
{

    if (head == NULL){ //当前队列还为空的情况
        head = (struct huffman *)malloc(sizeof(struct huffman));
        head->data = *buf;
        head->n = 1;
        head->next = NULL;
        head->p = NULL;
        head->l = NULL;
        head->r = NULL;
        return;
    }

    
    struct huffman *p = head;

    while (p != NULL){ //遍历队列找到对应节点时
        if (p->data == *buf)
        {
            p->n++;
            sort(&head);
            break;
            return;
        }
        p = p->next;
    }

    if (p == NULL){ //经过前一次遍历没有找到对应节点时
        p = head;
        head = (struct huffman *)malloc(sizeof(struct huffman));
        head->data = *buf;
        head->n = 1;
        head->next = p;
        head->p = NULL;
        head->l = NULL;
        head->r = NULL;
    }
}

void traversal(char *file)
{
    FILE *fp;
    unsigned char *buf;
    if ((fp = fopen(file, "rb")) == NULL)
        cout << "Can not open " << file << endl;
    fseek(fp, 0, SEEK_END);
    long end = ftell(fp);
    fseek(fp, 0, SEEK_SET);
    long start = ftell(fp);

    while (end != start)
    {

        fread(buf, 1, 1, fp);
        count(buf);
        start = ftell(fp);
    }
}

void printhuffman(struct huffman *root) {
    if(root->l==NULL && root->r==NULL)
        cout << root->data << "===" << root->n <<endl;
    else{
        printhuffman(root->l);
        printhuffman(root->r);
    }
}

void printtree(){
    printhuffman(head);
}

void tree(){
    while( head->next != NULL ){
        struct huffman *p = (struct huffman *)malloc(sizeof(struct huffman));

        p->l = head;
        p->r = head->next;
        p->n = head->n + head->next->n;
        p->p = NULL;
        p->next = head->next->next;

        head->p = p;
        head->next->p = p;

        head = p;
        sort(&head);
    }

}