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
    int n;
    int len;                //编码长度
    unsigned int code;      //huffman编码本体
    unsigned char data;     //对应的数据
    struct code *next;
}codeHe;

struct huffman *head = NULL;            //初始化创建一个huffman树(第一次遍历文件时作为频率表)
struct code *codeHead = &codeHe;        //初始化创建一个huffman编码链表
struct code *pcode = codeHead;
int maxLen = 0;                         //huffman编码最大的值
int code_link_len = 0;                  //huffman编码链表的长度（个数）
unsigned long int sum_code_len = 0;              //正文区的长度（位）
unsigned long int old_file_len = 0;              //原始文件的长度（位）
long int head_len = 0;                          //头部区长度(位)


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
    unsigned char *buf = new unsigned char;
    if ((fp = fopen(file, "rb")) == NULL)
        cout << "Can not open " << file << endl;
    fseek(fp, 0, SEEK_END);
    long int end = ftell(fp);
    fseek(fp, 0, SEEK_SET);
    long int start = ftell(fp);
    old_file_len = end*8;                 //上报原始文件长度（位）

    while (end != start)
    {

        fread(buf, 1, 1, fp);
        count(buf);
        start = ftell(fp);
    }
    fclose(fp);
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
        pcode->n = root->n;
        pcode->code = code;
        pcode->data = root->data;
        pcode->len = len;
        pcode->next = (struct code *)malloc(sizeof(struct code));
        pcode->next->next = NULL;                       //初始化后一个节点的后驱指针为空
        if(pcode->len > maxLen) maxLen = pcode->len;    //取得全链表最长的huffman编码长度
        pcode = pcode->next;                            //code链表指针后移一位，准备下次接收
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
        sum_code_len = sum_code_len + (pcode->n)*(pcode->len);      //上报正文区总长度（位）
        code_link_len++;                                            //上报编码链表长度（个数）
        pcode = pcode->next;
    }
    head_len = (code_link_len*9 + 12) * 8;                          //上报压缩文件头部区的长度（位）
}

void huffwrite(char *new_file, char *old_file){
    pcode = codeHead;
    ofstream outfile;
    unsigned int tmp = 0;                   //写入huffmancode的凑整暂存区
    int tmp_len = 0;                        //tmp已填充的长度计数
    
    outfile.open(new_file, ios::out | ios::trunc | ios::binary );  //新建文件或覆盖，可写入模式
    
    outfile.write((char *)&code_link_len, 4);           //写入编码链表的长度(个数)
    outfile.write((char *)&sum_code_len, 8);            //写入正文的长度（位）
    
    cout << code_link_len;
    cout << sum_code_len;
    
    int i = 0;                              //向文件中写入huffman编码表，每个部分占9字节长度。
    for(i; i<code_link_len; i++) {
        outfile.write((char *)&pcode->code,4);             //huffman编码，unsigned int形式存放，长度4字节
        outfile.write((char *)&pcode->len, 4);             //huffman编码长度，int形式存放，长度4字节
        outfile.write((char *)&pcode->data,1);             //编码所代表的数据，unsigned形式存放，长度1字节

        cout << pcode->code;
        cout << pcode->len;
        cout << pcode->data;

        pcode = pcode->next;
    }

    FILE *fp;                               //遍历文件，将每个字节的内容翻译为huffman编码写入新文件
    unsigned char *buf = new unsigned char;
    if ((fp = fopen(old_file, "rb")) == NULL)
        cout << "Can not open " << old_file << endl;
    fseek(fp, 0, SEEK_END);
    long int end = ftell(fp);
    fseek(fp, 0, SEEK_SET);
    long int start = ftell(fp);

    while (end != start)
    {
        pcode = codeHead;
        fread(buf, 1, 1, fp);
        while(pcode->next!=NULL) {                          //遍历找到此字节对应的huffcode节点
            if(pcode->data == *buf) break;
            pcode = pcode->next;
        }
        tmp_len = tmp_len + pcode->len;                     //tmp的计数器工作一次
        if(tmp_len<=32) {                                   //tmp未满的情况
            tmp = tmp + ( pcode->code << (32-tmp_len));
        }
        if(tmp_len > 32) {                                  //tmp满了的情况
            tmp = tmp + ( pcode->code >> (tmp_len-32));         //向tmp中填入部分数据将其32位凑齐
            outfile.write((char *)&tmp,4);                      //将已经满了的tmp写入
            cout << tmp;
            tmp = 0;                                            //重新初始化tmp
            tmp = tmp + ( pcode->code << (32-(tmp_len-32)));    //刚刚没装载完的部分
            tmp_len = tmp_len - 32;                             //重新设置tmp的计数器
        }
        start = ftell(fp);
    }
    if(tmp_len!=0){                                         //文件读完了最后检查凑不够整还没有写入新文件的小尾巴
        outfile.write((char *)&tmp,4);
        cout << tmp;
    }
    fclose(fp);
    outfile.close();
}