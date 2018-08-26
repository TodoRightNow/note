#include <string.h>
#include <iostream>
using namespace std;
void swap(char *a,char *b)
{
    *a=*a^*b;
    *b=*a^*b;
    *a=*a^*b;
}

void ReverseWord(char *input,int len)
{
     int low=0;
     int high=0;

     //单个单词的逆序
     while(high<len)
     {
        while(input[high]!=' '&&input[high]!='\0')
        {
            ++high;
        }
        for(int i=low;i<(high+low)/2;++i)
        {
            swap(input[i],input[high+low-i-1]);
        }
        low=high+1;
        high=high+1;
     }
     //整个句子的单字符倒序
     for(int i=0;i<len/2;++i)
     {
         swap(input[i],input[len-i-1]);
     }
}
int main()
{
    char input[]="I am a student";
    int len=strlen(input);
    ReverseWord(input,len);
    cout<<input<<endl;
}
