#include <iostream>
#include <map>
#include <boost/assign.hpp>
using namespace std;
using namespace boost::assign;
int main(int argc, char const *argv[])
{
    map<int,int> m;
    insert(m)(1,2)(3,4)(5,6);

    map<int,int>::const_iterator iter=m.begin();
    
    for( ; iter!=m.end(); ++iter)
    {
        cout<<iter->first<<" "<<iter->second<<endl;
    }
    
    return 0;
}
