#include <iostream>
#include <stdio.h>
#include <stdlib.h>
using namespace std;
 
int main() 
{
    int i = 0;
    FILE *fp;
    if((fp = fopen("file.txt", "rb")) == NULL)
        cout << "error!\n" << endl;
    unsigned char a;
    char * buffer;
    while(!feof(fp)) {
        fread(&a, 1, 1, fp);
        if( a <= 127 ){
            fseek(fp, -1, SEEK_CUR);
            buffer = (char*)malloc(1);
            fread(buffer, 1, 1, fp);
            cout << buffer << endl;
        }
        else if( a <= 223 ){
            fseek(fp, -1, SEEK_CUR);
            buffer = (char*)malloc(2);
            fread(buffer, 2, 1, fp);
            cout << buffer << endl;
        }
        else if( a <= 239 ){
            fseek(fp, -1, SEEK_CUR);
            buffer = (char*)malloc(3);
            fread(buffer, 3, 1, fp);
            cout << buffer << endl;
        }
        else if( a <= 247 ){
            fseek(fp, -1, SEEK_CUR);
            buffer = (char*)malloc(4);
            fread(buffer, 4, 1, fp);
            cout << buffer << endl;
        }
    }

    cout << "Hello, World!\n";
    return 0;
}