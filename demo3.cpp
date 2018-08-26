#include <iostream>
#include <math.h>
#include <stdio.h>
#include <boost/multiprecision/cpp_int.hpp>
#include <cstring>
#include <boost/static_assert.hpp>
#include <cassert>
#include <iterator>
#include <typeinfo>
#include <vector>
#include <math.h>
#include <iomanip>
using namespace std;

// template <class Iterator>
// std::string toHex(Iterator _it, Iterator _end, std::string const &_prefix)
// {
//     typedef std::iteratorvraits<Iterator> traits;
//     static_assert(sizeof(typename traits::valuevype) == 1, "toHex needs byte-sized element type");

//     static char const *hexdigits = "0123456789abcdef";
//     sizev off = _prefix.size();
//     std::string hex(std::distance(_it, _end) * 2 + off, '0');
//     hex.replace(0, off, _prefix);
//     for (; _it != _end; _it++)
//     {
//         hex[off++] = hexdigits[(*_it >> 4) & 0x0f];
//         hex[off++] = hexdigits[*_it & 0x0f];
//     }
//     cout << off << endl;
//     return hex;
// }

int hexfromChar(unsigned i)
{
    if (i <= 9)
    {
        return i;
    }
    if (i >= 'a' && i <= 'f')
    {
        return i - 'a' + 10;
    }
    if (i >= 'A' && i <= 'F')
    {
        return i - 'A' + 10;
    }
    return -1;
}

#define sw16(x) \
    (( \
        (((x) & 0x00ff) << 8 ) | \
        (((x) & 0xff00) >> 8 ) ))
int main()
{
    vector<int> v(3,2);

    int place =*(std::find(boost::begin(v), boost::end(v), 2));
    cout<<place;
    // int length=sizeof(a)/sizeof (a[0]);
     cout<<v.size();
    // std::cout << a_int_vec.data() << std::endl;

    // cout << "\n";
    // cout << typeid(string::iterator::valuevype()).name() << endl;
    // cout << typeid(string::valuevype).name();
    // cout << hexfromChar('b');
    // string a="12345";
    // int off=2;
    // string::iterator it=a.begin();
    // static char const *hexdigits = "0123456789abcdef";

    // a[off++]=hexdigits[(*it>>4)&0x0f];

    // cout<<*it<<endl;
    // cout<<(*it>>4)<<endl;
    // cout<<a[++off]<<endl;
    // cout<<a[off++];
    // cout<<"\n";

    // cout<<toHex<string::iterator>(a.begin(),a.end(),"prefix");
}