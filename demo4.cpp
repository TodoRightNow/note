#include <iostream>
#include <atomic>
#include <thread>
#include <vector>

#if __cplusplus>=201103L
struct Node 
{
    int value;
    Node* next;
};

std::atomic<Node*> list_head(nullptr);

void append(int val)
{
    Node *oldHead=list_head;
    Node* newNode=new Node{val,oldHead};

    while(!list_head.compare_exchange_weak(oldHead,newNode))
    {
        newNode->next=oldHead;
    }
}

int main(int argc, char const *argv[])
{
    std::vector<std::thread> threads;
    
    for(int i = 0; i < 10; i++)
    {
        threads.push_back(std::thread(append,i));
    }

    
    for(auto& th :threads)
    {
        th.join();
        
    }
    
    
    for(Node* it = list_head; it !=nullptr; it=it->next)
    {
        std::cout<<" "<<it->value;
    }

    std::cout<<'\n';

    Node* it; 

    while(it=list_head)
    {
        list_head=it->next;
        delete it;
    }
    
    return 0;
}




#endif
