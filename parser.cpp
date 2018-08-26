#include <boost/spirit.hpp>
#include <boost/spirit/include/classic.hpp>
#include <iostream>
using namespace std;

int main(int argc, char const *argv[])
{
    float value=0.0f;
    
    boost::spirit::rule<> testRule1=boost::spirit::real_p[boost::spirit::assign_a(value)];
    boost::spirit::parse_info<> info=boost::spirit::parse(("0.1235sfsdf"),testRule1);

    if(info.hit)
    {
        
    }
    if(info.full)
    {
        
    }
    if(info.stop!=NULL)
    {
        cout<<info.stop<<endl;     
    }
    cout<<info.length<<endl;
    cout<<value;

    return 0;
}


