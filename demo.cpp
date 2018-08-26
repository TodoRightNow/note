#include <stack>
#include <iostream>
#include <assert.h>
using namespace std;
template <class T>
class StackDemo
{
  public:
    StackDemo(){};
    ~StackDemo(){};

  public:
    T min();
    void push(T data);
    void pop();
    T top() const;
    int stack_size();

  private:
    stack<T> base_data;
    stack<T> min_data;
};

template <class T>
void StackDemo<T>::push(T data)
{
    base_data.push(data);
    if (base_data.size() == 1)
    {
        min_data.push(data);
        return;
    }
    else if (min_data.top() > data)
    {
        min_data.push(data);
    }
    
    else
    {
        min_data.push(min_data.top());
    }
}

template<class T>
int StackDemo<T>::stack_size()
{
    return base_data.size();
}

template <class T> 
void StackDemo<T>::pop()
{
    if(min_data.size()>=1&&base_data.size()>=1)
    {
        min_data.pop();
        base_data.pop();
    }
}

template<class T>
T StackDemo<T>::top()const
{
     assert(min_data>=1);
     return min_data.top();
}

template <class T>
T StackDemo<T>::min()
{
    assert(min_data.size()>0);
    assert(base_data.size()>0);
     return min_data.top();    
}

