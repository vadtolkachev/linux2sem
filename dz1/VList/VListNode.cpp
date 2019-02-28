#include "VListNode.hpp"


VListNode::VListNode()
{
	m_prev = nullptr;
	m_next = nullptr;
	m_str = nullptr;
}


VListNode::~VListNode()
{
	if(m_str)
		delete[] m_str;
}


VListNode *VListNode::getPrev() const
{
	return m_prev; 
}


VListNode *VListNode::getNext() const
{
	return m_next;
}


char *VListNode::getStr() const
{
	return m_str;
}


VError VListNode::setPrev(VListNode *prev)
{
	m_prev = prev;

	return SUCCESS;
}


VError VListNode::setNext(VListNode *next)
{
	m_next = next;

	return SUCCESS;
}


VError VListNode::setStr(const char *str)
{
	if(m_str)
		delete[] m_str;

	m_str = new (std::nothrow) char[strlen(str)+1]();
	
	if(!m_str)
		return ALLOC_ERR;

	strcpy(m_str, str);

	return SUCCESS;
}
