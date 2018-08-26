#include <stdio.h>
int max=0,tmp=0;
int find(int *p,int n)
{
    
    for(int i=0;i<n;i++)
    {
        if(tmp<=0)
        {
            tmp=p[i];
            if(max<tmp)
            {
                max=tmp;
            }
        }
        else
        {
            tmp+=p[i];
            if(max<tmp)
            {
                max=tmp;
            }
        }
    }
    return max;
}


int main(int argc, char const *argv[])
{
    int arr[]={1,-2,3,10,-4,7,2,-5};
    int n=sizeof(arr)/sizeof(*arr);
    printf("%d\n",find(arr,n));
    return 0;
}
