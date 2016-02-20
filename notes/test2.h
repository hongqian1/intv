// test.h
#include <vector>
struct Node
{
    Node * left;
    Node * right;
    int value;
};

struct TreeNode
{
    std::vector<TreeNode *> children_;
    int value_;
};

struct SingleNode
{
    SingleNode * next;
    int value;
};

struct DoubleNode
{
    DoubleNode * next;
    DoubleNode * prev;

    int value;
};

/*
class Singleton
{
protected:
    Singleton(){};
    ~Singleton(){};
public:
    static Singleton * getInstance();
private:
    static Singleton * pInstance;
    static pthread_mutex pMutex;
};
*/
