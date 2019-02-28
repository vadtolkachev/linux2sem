#pragma once
#include "VListNode.hpp"


class VList
{
public:
	VList();
	~VList();
	VListNode *getHead() const;
	VListNode *getTail() const;
	VListNode *getNodeNumb(unsigned numb) const;
	VListNode *getNodeData(const char *str) const;

	VError addHead();
	VError addTail();
	VError removeHead();
	VError removeTail();
	VError addAfterNode(VListNode *node);
	VError addBeforeNode(VListNode *node);
	VError removeNode(VListNode *node);

	void freeList();
	void printList();

private:
	VListNode *m_head;
	VListNode *m_tail;
};