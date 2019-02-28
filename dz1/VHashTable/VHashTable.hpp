#pragma once
#include "VList.hpp"


const unsigned HASH_TABLE_SIZE = 256*256;


class VHashTable
{
public:
	VError addStr(const char *str);

	char *getStr(const char *str);
	int hashFunc1(const char *str);

	void foreach(void (*func)(VListNode *node));

private:
	VList m_table[HASH_TABLE_SIZE];
};

