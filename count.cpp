#include "myHuff.h"
using namespace std;


struct huffman{
    int n;                  //节点权重
    unsigned char data;     //节点中存放的数据
    struct huffman *p;      //指向父节点
    struct huffman *l;      //指向左节点
    struct huffman *r;      //指向右节点
    struct huffman *next;   //后驱指针
};

struct code{
    int len;                //编码长度
    unsigned int code;               //huffman编码本体
    unsigned char data;     //对应的数据
    struct code *next;
}codeHe;

struct huffman *head = NULL;
struct code *codeHead = &codeHe;        //初始化创建一个huffman编码链表
struct code * pcode = codeHead;
int maxLen = 0;

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

void _printtree(struct huffman *root) {
    if(root->l==NULL && root->r==NULL)
        cout << root->data << "===" << root->n <<endl;
    else{
        _printtree(root->l);
        _printtree(root->r);
    }
}

void printtree(){
    _printtree(head);
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

void _code(struct huffman * root, int code, int len){
    if( root->l == NULL && root->r == NULL ) {
        pcode->code = code;
        pcode->data = root->data;
        pcode->len = len;
        pcode->next = (struct code *)malloc(sizeof(struct code));
        pcode->next->next = NULL;       //初始化后一个节点的后驱指针为空
        if(pcode->len > maxLen) maxLen = pcode->len;    //取得全链表最长的huffman编码长度
        pcode = pcode->next;            //code链表指针后移一位，准备下次接收
    }
    else{
        _code(root->l, code*2, len+1);
        _code(root->r, code*2+1, len+1);
    }
}

void code(){
    _code( head, 0, 0 );
    pcode = codeHead;
    while(pcode->next!=NULL){
        cout << pcode->data << "==" << pcode->code << "==" << pcode->len << endl;
        pcode = pcode->next;
    }
}

