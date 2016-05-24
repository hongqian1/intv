#include <vector>
#include <unordered_map>

struct Node
{
    Node * left;
    Node * right;
    int value;
};

struct TreeNode
{
public:
    int val;
    TreeNode *left, *right;
    TreeNode(int val) {
        this->val = val;
        this->left = this->right = NULL;
    }
};

/*
struct TreeNode
{
    std::vector<TreeNode *> children_;
    int value_;
};
*/
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

class ListNode {
public:
    int val;
    ListNode *next;
    ListNode(int val) {
        this->val = val;
        this->next = NULL;
    }
};

struct TrieNode {
    TrieNode() : isWord(false) {}
    bool isWord;
    std::unordered_map<char, TrieNode *> children;
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
