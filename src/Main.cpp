#include <iostream>
#include <memory>
#include "MemTable.hpp"

using namespace std;

int main() {
	shared_ptr<MemTable> table = make_shared<MemTable>(0, 10);

	MemNode* node1 = table->addNode(5);
	if (!node1)
		cout << "Allocate node1 failed" <<endl;
	MemNode* node2 = table->addNode(5);
	if (!node2)
		cout << "Allocate node2 failed" <<endl;
	table->dump();

	table->removeNode(node1);
	table->dump();

	MemNode* node3 = table->addNode(2);
	if (!node3)
		cout << "Allocate node3 failed" <<endl;
	table->dump();

	return 0;
}
