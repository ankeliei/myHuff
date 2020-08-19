#include "myHuff.h"
using namespace std;

void unzip(char * old_file, char * new_file){
    
    struct code{
        unsigned int code;
        int len;
        unsigned char data;
        struct code *next = NULL;
    };

    struct code *head = new struct code;
    struct code *p = head;

    unsigned int code_link_len = 0;             //编码表的长度（个）
    unsigned long sum_code_len = 0;             //正文的长度（位）

    FILE *fp;
    ofstream outfile;
    outfile.open(new_file, ios::out | ios::trunc | ios::binary );  //新建文件或覆盖，可写入模式
    unsigned char *buf1 = new unsigned char;
    unsigned int *buf4 = new unsigned int;
    unsigned long *buf8 = new unsigned long;
    unsigned int tmp1 = 0;
    unsigned int tmp2 = 0;
    unsigned int tmp3 = 0;
    int tmp1_len = 32;                       //tmp1中huffmanCode经过左移操作剩余的有效位数
    int tmp3_len = 32;
    int i = 0;

    if ((fp = fopen(old_file, "rb")) == NULL)
        cout << "Can not open " << old_file << endl;
    
    fread(buf4, 4, 1, fp);
    code_link_len = *buf4;
    fread(buf8, 8, 1, fp);
    sum_code_len = *buf8;
    
    for( i=0; i<code_link_len; i++){        //读取编码表存为链表
        fread(buf4,4,1,fp);
        p->code = *buf4;
        fread(buf4,4,1,fp);
        p->len = *buf4;
        fread(buf1,1,1,fp);
        p->data = *buf1;
        p->next = new struct code;
        p = p->next;
    }

    if(sum_code_len <= 32){                 //正文区过短的情况
        fread(buf4, 4, 1, fp);
        tmp1 = *buf4;
        while(sum_code_len){
            for(i=1; i<=sum_code_len; i++){
                p=head;
                tmp2 = tmp1 >> (32-i);
                while(p->next!=NULL){
                    if(p->code==tmp2 && p->len==i) break;
                    else p=p->next;
                }
                if(p->next==NULL) continue;
                else break;
            }
            outfile.write((char *)&p->data,1);
            tmp1 = tmp1 << i;
            sum_code_len = sum_code_len - i;
        }
    }

    else{                                   //正文足够长的情况下
        fread(buf4, 4, 1, fp);              //先取两次4字节
        tmp1 = *buf4;
        fread(buf4, 4, 1, fp);
        tmp3 = *buf4;
        while(sum_code_len){
            for(i=1; i<=32; i++){           //对tmp1中前i个位进行匹配
                p=head;
                tmp2 = tmp1 >> (32-i);
                while(p->next!=NULL){
                    if(p->code==tmp2 && p->len==i) break;
                    else p=p->next;
                }
                if(p->next==NULL) continue;
                else break;
            }
            if(i>32){                       //没找到对应编码表项
                cout << "damaged file!" << endl;
            }
            else{                           //找到对应编码表项
                outfile.write((char *)&p->data,1);              //将对应数据写入
                if(tmp3_len < i){                               //后背的tmp3不够补充tmp1时
                    tmp1 = (tmp1 << i) + (tmp3 >> (32-i));              //tmp去掉已经匹配过的部分，再加上tmp3的残值
                    fread(buf4, 4, 1, fp);                          //再读4个字节进tmp3
                    tmp3 = *buf4;
                    tmp1 = tmp1 + (tmp3 >> (32-(i-tmp3_len)));        //tmp1完全补充
                    tmp3 = tmp3 << (i-tmp3_len);                    //tmp3去掉tmp1已经引走的部分
                    tmp3_len = 32 - (i-tmp3_len);                   //tmp3的有效位数值对应调整
                    sum_code_len = sum_code_len - i;                //对剩余正文长度进行调整
                }
                else{                                           //后背的tmp3足够补充tmp1时
                    tmp1 = (tmp1 << i) + (tmp3 >> (32-i));              //使用tmp3的高位对tmp1的低位进行补充
                    tmp3 = tmp3 << i;
                    tmp3_len = tmp3_len - i;
                    sum_code_len = sum_code_len - i;
                }
            }
        }
    }
    fclose(fp);
    outfile.close();
}