// binary search tree verification
struct BinaryTreeNode
{
    int nValue;
    BinaryTreeNode * pLeft;
    BinaryTreeNode * pRight;
};

bool isBST(BinaryTreeNode * pRoot)
{
    return isBST(pRoot, std::nemeric_limits<int>::max(), std::numeric_limits<int>::min());
}

bool isBST(BinaryTreeNode * pRoot, int max, int min)
{
    if (pRoot == NULL)
        return true;

    if (pRoot->nValue<=min ||  pRoot->nValue>=max)
        return false;

    if (!isBST(pRoot->left, pRoot->nValue, min))
        return false;

    if (!isBST(pRoot->right, max, pRoot->nValue))
        return false;

    return true;
}

isBST2(BinaryTreeNode * pRoot, int & lowerValue)
{
    if (pRoot==NULL) return true;

    if (!isBST2(pRoot->left, lowerValue))
        return false;
    
    if (pRoot->nValue <= lowerValue)
        return false;
    lowerValue=pRoot->nValue;
    
    if (!isBST2(pRoot->right, lowerValue))
        return false;
}
