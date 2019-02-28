#include "VHashTable.hpp"
#include <stdio.h>


VError VHashTable::addStr(const char *str)
{
	int hash = hashFunc1(str);
	VError checkErr = m_table[hash].addHead();
	if(checkErr != SUCCESS)
		return checkErr;

	checkErr = m_table[hash].getHead()->setStr(str);

	return checkErr;
}


char *VHashTable::getStr(const char *str)
{
	if(!str)
		return nullptr;

	int hash = hashFunc1(str);

	VListNode *node = m_table[hash].getNodeData(str);
	
	if(!node)
		return nullptr;

	return node->getStr();
}


int VHashTable::hashFunc1(const char *str)
{
	int hash = 0;
	int i = 0;
	while(str[i] != '\0')
	{
		hash += (int)str[i];
		i++;
	}

	hash %= HASH_TABLE_SIZE;

	return hash;
}


void VHashTable::foreach(void (*func)(VListNode *node))
{
	VListNode *tmp;
	for(unsigned i = 0; i < HASH_TABLE_SIZE; i++)
	{
		tmp = m_table[i].getHead();
		while(tmp)
		{
			func(tmp);
			tmp = tmp->getNext();
		}
	}
}
