#include <stdio.h>
#include <stdlib.h>
#include "VHashTable.hpp"


void listTest();
void hashTableTest();


int main()
{
	listTest();
	hashTableTest();

	printf("done!\n");
	
	return 0;
}


void listTest()
{
	VList list;
	VListNode node1;

	VError err = list.addHead();
	err = list.addHead();

	list.removeHead();
	list.removeHead();
	list.removeHead();	
	
	err = list.addTail();
	err = list.addTail();
	list.removeTail();
	list.removeTail();
	list.removeTail();

	err = list.addHead();

	list.getHead()->setStr("1234");

	err = list.addAfterNode(list.getHead());

	list.getTail()->setStr("567");
	list.getHead()->setStr("8943");

	err = list.addAfterNode(list.getHead()->getNext());
	err = list.addAfterNode(list.getHead());
	err = list.addBeforeNode(list.getTail());
	err = list.addBeforeNode(list.getTail()->getPrev());
	err = list.addBeforeNode(list.getTail());
	err = list.addBeforeNode(list.getHead());

	list.printList();

	VListNode *tmpnode = list.getHead();
	int i = 1;
	char snum[20] = "";

	while(tmpnode)
	{
		sprintf(snum, "%d", i);
		++i;
		tmpnode->setStr(snum);
		tmpnode = tmpnode->getNext();
	}

	list.printList();
	
	printf("%s\n", list.getNodeNumb(0)->getStr());
	printf("%s\n", list.getNodeData("4")->getStr());

	list.getNodeNumb(25);
	list.getNodeData("hey");	

	list.getHead()->setStr("213");
	list.getHead()->setStr("123412");
	printf("%s\n", list.getHead()->getStr());


	list.removeNode(list.getHead());

	list.addAfterNode(nullptr);
	list.addBeforeNode(nullptr);
	list.removeNode(nullptr);


	err = list.addHead();

	list.removeNode(list.getHead());

	err = list.addHead();

	err = list.addHead();
	if(err == SUCCESS)
		printf("success!\n");

	list.removeNode(list.getTail());
}


void hashTableTest()
{
	VHashTable table;
	table.addStr("qweasdf");
	table.addStr("qweasdq");
	table.addStr("qweasfd");

	table.getStr("qweasdf");
	table.getStr("qweasd");
	table.getStr("");
	table.getStr(nullptr);

	printf("\nforeach:\n");

	table.foreach( [](VListNode *node) 
	{ 
		printf("%s\n", node->getStr()); 
	});

	int hash = table.hashFunc1("Hello world!");
	printf("hash = %d\n", hash);

	puts("");
}
