#include <string.h>
#include <iostream>
using namespace std;
void swap(char *a,char *b)
{
    *a=*a^*b;
    *b=*a^*b;
    *a=*a^*b;
}

void ReverseWord(char *input)
{
    int len=strlen(input);
    int begin=0;
    int step=0;

    while(step<len)
    {
        while (input[step] !=' '&&input[step]!='\0')
        {
            ++step;
        }

        for(int i=begin;i<(step+begin)/2;++i)
        {
            swap(input[i],input[step+begin-i-1]);
        }

        begin=step+1;
        step=step+1;
    }
    for(int i=0;i<len/2;++i)
    {
        swap(input[i],input[len-1-i]);
    }
}

int main()
{
    char *input;
    
    while(cin>>input){
    cout<<input<<endl;
        
    }
}