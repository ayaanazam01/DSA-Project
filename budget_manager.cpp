#include <iostream> // For input/output
using namespace std;

string PriorityNames[5]; // Array declared for quick access of Priority Names
int PriorityRank[5];     // Array declared for accessing Priority Ranks

struct Stack_Node
{
    int priorityrank_stack;
    Stack_Node *next;
};

