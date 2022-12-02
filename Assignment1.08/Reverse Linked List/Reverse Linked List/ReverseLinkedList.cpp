#include <iostream>
using namespace std;

struct LinkNode
{
	int n;
	LinkNode* pNext;
};

// Implementing using for loop
/*LinkNode* Reverse(LinkNode* i_pRoot)
{
	LinkNode* pCurrentFirst = i_pRoot;
	LinkNode* pNextNode = nullptr;

	for (; i_pRoot->pNext;)
	{
		pNextNode = i_pRoot->pNext->pNext;
		i_pRoot->pNext->pNext = pCurrentFirst;
		pCurrentFirst = i_pRoot->pNext;
		i_pRoot->pNext = pNextNode;
	}

	return pCurrentFirst;
}*/

// Implementing using Recursion
LinkNode* Reverse(LinkNode* i_pRoot)
{
	if (!i_pRoot || !i_pRoot->pNext)
		return i_pRoot;
	LinkNode* pNextNode = i_pRoot->pNext;
	LinkNode* pNewHead = Reverse(i_pRoot->pNext);
	i_pRoot->pNext = nullptr;
	pNextNode->pNext = i_pRoot;

	return pNewHead;
}

int main()
{
	LinkNode a, b, c, d;
	a.n = 0;
	a.pNext = &b;
	b.n = 1;
	b.pNext = &c;
	c.n = 2;
	c.pNext = &d;
	d.n = 3;
	d.pNext = nullptr;

	LinkNode* i_pRoot = &a;

	cout << i_pRoot->n << i_pRoot->pNext->n << i_pRoot->pNext->pNext->n << i_pRoot->pNext->pNext->pNext->n << endl;

	i_pRoot = Reverse(i_pRoot);

	cout << i_pRoot->n << i_pRoot->pNext->n << i_pRoot->pNext->pNext->n << i_pRoot->pNext->pNext->pNext->n << endl;

	return 0;
}