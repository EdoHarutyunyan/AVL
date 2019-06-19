#include <iostream>
#include "AVL.h"

int main()
{
	Tree<int> tree;

	tree.insert(5);
	tree.insert(4);
	tree.insert(7);
	tree.insert(2);
	tree.insert(3);

	auto it = tree.begin();

	tree.remove(it);

	system("pause");
	return 0;
}