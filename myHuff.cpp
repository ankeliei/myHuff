#include "myHuff.h"
#include "count.cpp"
using namespace std;


 
int main() 
{
    char file[255];
    cout << "请输入要压缩的文件：";
    cin >> file;
    traversal(file);        //遍历文件，生成节点
    tree();                 //构建huffmanTree
    printtree();
    code();
    cout << "Hello, World!\n";
    return 0;
}