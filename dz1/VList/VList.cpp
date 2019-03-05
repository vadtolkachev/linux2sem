#include "VList.hpp"
#include "stdio.h"


VList::VList():
	m_head(nullptr),
	m_tail(nullptr)
{}


VList::~VList()
{
	freeList();
}


VListNode *VList::getHead() const
{
	return m_head;
}


VListNode *VList::getTail() const
{
	return m_tail;
}


VListNode *VList::getNodeNumb(unsigned numb) const
{
	VListNode *tmp = m_head;

	for(unsigned i = 0; i < numb; i++)
	{
		if(!tmp)
			break;

		tmp = tmp->getNext();		
	}

	return tmp;
}


VListNode *VList::getNodeData(const char *str) const
{
	VListNode *tmp = m_head;

	while(tmp)
	{
		if(!strcmp(str, tmp->getStr()))
			return tmp;

		tmp = tmp->getNext();
	}

	return nullptr;
}


VError VList::addHead()
{
	VListNode *newNode = new (std::nothrow) VListNode();

	if(!newNode)
		return ALLOC_ERR;

	if(m_head)
	{
		newNode->setNext(m_head);
		m_head->setPrev(newNode);
	}
	else
		m_tail = newNode;

	m_head = newNode;

	return SUCCESS;
}


VError VList::addTail()
{
	VListNode *newNode = new (std::nothrow) VListNode();

	if(!newNode)
		return ALLOC_ERR;

	if(m_tail)
	{
		newNode->setPrev(m_tail);
		m_tail->setNext(newNode);
	}
	else
		m_head = newNode;

	m_tail = newNode;

	return SUCCESS;
}


VError VList::removeHead()
{
	if(!m_head)
		return EMPTY_ERR;

	if(m_head != m_tail)
	{
		VListNode *newHead = m_head->getNext();
		newHead->setPrev(nullptr);
		delete m_head;
		m_head = newHead;
	}
	else
	{
		delete m_head;
		m_tail = nullptr;
		m_head = nullptr;
	}

	return SUCCESS;
}


VError VList::removeTail()
{
	if(!m_tail)
		return EMPTY_ERR;

	if(m_head != m_tail)
	{
		VListNode *newTail = m_tail->getPrev();
		newTail->setNext(nullptr);
		delete m_tail;
		m_tail = newTail;
	}
	else
	{
		delete m_tail;
		m_tail = nullptr;
		m_head = nullptr;
	}

	return SUCCESS;
}


VError VList::addAfterNode(VListNode *node)
{
	if(!node)
		return NULLPTR_ERR;

	VListNode *newNode = new (std::nothrow) VListNode();

	if(!newNode)
		return ALLOC_ERR;

	VListNode *tmp = node->getNext();
	node->setNext(newNode);
	if(tmp)
		tmp->setPrev(newNode);
	else
		m_tail = newNode;

	newNode->setPrev(node);
	newNode->setNext(tmp);

	return SUCCESS;
}


VError VList::addBeforeNode(VListNode *node)
{
	if(!node)
		return NULLPTR_ERR;

	VListNode *newNode = new (std::nothrow) VListNode();

	if(!newNode)
		return ALLOC_ERR;

	VListNode *tmp = node->getPrev();
	node->setPrev(newNode);
	if(tmp)
		tmp->setNext(newNode);
	else
		m_head = newNode;

	newNode->setNext(node);
	newNode->setPrev(tmp);

	return SUCCESS;
}


VError VList::removeNode(VListNode *node)
{
	if(!node)
		return NULLPTR_ERR;

	if(m_tail != node)
		node->getNext()->setPrev(node->getPrev());
	else
		m_tail = node->getPrev();

	if(m_head != node)		
		node->getPrev()->setNext(node->getNext());
	else
		m_head = node->getNext();

	delete node;

	return SUCCESS; 
}


void VList::freeList()
{
	while(m_head)
		removeHead();
	
	m_head = nullptr;
	m_tail = nullptr;
}


void VList::printList()
{
	VListNode *tmp = m_head;
	int i = 0;
	while(tmp)
	{
		printf("%d.str = \"%s\"\n", ++i, tmp->getStr());
		tmp = tmp->getNext();
	}
}


