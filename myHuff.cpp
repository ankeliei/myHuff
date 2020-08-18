#include "myHuff.h"
#include "count.cpp"
using namespace std;


 
int main() 
{
    char old_file[255];
    char new_file[255];
    cout << "请输入要压缩的文件：";
    cin >> old_file;
    traversal(old_file);        //遍历文件，生成节点
    tree();                 //构建huffmanTree
    //printtree();
    code();
    cout << "请输入压缩后的文件名：";
    cin >> new_file;
    huffwrite(new_file, old_file);
    cout << "Hello, World!\n";
    return 0;
}