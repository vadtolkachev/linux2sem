#pragma once
#include <cstring>
#include <new>


extern int hashFunc1(const char *str);



enum VError : char
{
	SUCCESS,
	ALLOC_ERR,
	NULLPTR_ERR,
	NOTNULLPTR_ERR,
	EMPTY_ERR,
	NOTEMPTY_ERR
};


class VListNode
{
public:
	VListNode();
	~VListNode();

	VListNode *getPrev() const;
	VListNode *getNext() const;
	char *getStr() const;

	VError setPrev(VListNode *prev);
	VError setNext(VListNode *next);
	VError setStr(const char *str);
	void setHash(int hash);

private:
	VListNode *m_prev;
	VListNode *m_next;
	char *m_str;
};