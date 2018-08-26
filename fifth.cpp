// 先定义二元树的数据结构
#include <iostream>
#include <vector>
struct BinaryTreeNode
{
    int m_nValue;
    BinaryTreeNode *m_pLeft;
    BinaryTreeNode *m_pRight;
};
//打印出路径，用vector存储 vector<BinaryTreeNode*>

std::vector<BinaryTreeNode *> pathVec;

void createTree(BinaryTreeNode *&root, int *a, int i, int len)
{
    if (i > len)
    {
        return;
    }
    root = new BinaryTreeNode;
    root->m_nValue = a[i];
    root->m_pLeft = NULL;
    root->m_pRight = NULL;
    createTree(root->m_pLeft, a, 2 * i + 1, len);
    createTree(root->m_pRight, a, 2 * i + 2, len);
}

void preorder(BinaryTreeNode *&root)
{
    if (!root)
    {
        return;
    }
    std::cout << root->m_nValue << " ";
    preorder(root->m_pLeft);
    preorder(root->m_pRight);
}

void printPath(std::vector<BinaryTreeNode *> &pathVal)
{
    std::vector<BinaryTreeNode *>::iterator it, it_end;
    for (it = pathVal.begin(), it_end = pathVal.end(); it != it_end; ++it)
    {
        std::cout << (*it)->m_nValue << " ";
    }
    std::cout << std::endl;
}

void pathTree(BinaryTreeNode* &root,int val)
{
    static int sum=0;
    sum+=root->m_nValue;
    pathVec.push_back(root);
    if(sum==val&&root->m_pLeft==NULL&&root->m_pRight==NULL)
    {
        printPath(pathVec);
    }
    if(root->m_pLeft)
    {
        pathTree(root->m_pLeft,val);
    }
    if(root->m_pRight)
    { 
        pathTree(root->m_pRight,val);
    }
}


int main(int argc, char const *argv[])
{
    BinaryTreeNode* root=0;
    int a[]={10,5,12,7,8};
    int len=sizeof a/sizeof a[0];
    createTree(root,a,0,len);
    pathTree(root,22);
    return 0;
}
