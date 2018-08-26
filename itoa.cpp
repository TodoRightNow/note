//实现从整数到字符串的转换
#include <algorithm>
#include <string>
#include <iostream>
const char *convert(char buf[],int value)
{
    static char digits[19]=
    {'9','8','7','6','5','4','3','2','1',
    '0','1','2','3','4','5','6','7','8','9'};
    //指针一开始指向0,这样可以让下标为负数 
    static const char *zero=digits+9;
    
    int i =value;
    //使用两个指针来
    char *p=buf;
    do
    {
        int lsd=i%10;
        i /=10;
        *p++=zero[lsd];
    } while(i!=0);

    if(value<0)
    {
        *p++='-';
    }

    *p++='\0';
    
    std::reverse(buf,p);
    return p;
}


template<class BidirIt>
void reverse(BidirIt first,BidirIt last)
{
    while((first !=last)&&(first !=--last))
    {
        std::iter_swap(first++,last);
    }
}

//测试
int main(int argc, char const *argv[])
{
     std::string p="abcd";

     std::string::iterator beg=p.begin();
     std::string::iterator last=p.end();
     std::reverse(beg,last);
     std::cout<<p<<std::endl;
    return 0;
}
