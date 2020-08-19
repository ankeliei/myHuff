#include "myHuff.h"
#include "count.cpp"
#include "unzip.cpp"
using namespace std;

int main(int argc, char *argv[])
{
    if(argc==3){
        char old_file[255];
        char new_file[255];
        strcpy(old_file, argv[1]);
        strcpy(new_file, argv[2]);
        traversal(old_file);        //遍历文件，生成节点
        tree();                     //构建huffmanTree
        code();
        huffwrite(new_file, old_file);
        return 0;
    }
    if(argc==4){
        char old_file[255];
        char new_file[255];
        strcpy(old_file, argv[2]);
        strcpy(new_file, argv[3]);
        unzip(old_file,new_file);
        return 0;
    }
    else{
        cout << "Wrong Formant!" <<endl;
        return 0;
    }
    
    cout << "Hello, World!\n";
    return 0;
}