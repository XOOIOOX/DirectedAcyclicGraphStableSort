#pragma once
#include "Item.h"
#include <stack>

// useful usings
template<typename T>
using Stack = std::stack<T>;
using VectorItems = Vector<Item>;
using StackInt = Stack<int>;

struct Graph
{
	Graph() = default;
	Graph(const VectorItems& items) : items(items) {}
	VectorItems items;									// vector of items

	void buildNextNodes();								// build connections to next nodes from connections to previous nodes
	void buildPrevNodes();								// build connections to previous nodes from connections to next nodes
	void buildIndexes();								// build temporary indexes
	void buildOldIndexes();								// build temporary old indexes
	int findOldIndex(int index);						// finf old index
	void rebuildConnections();							// rebuild invalid connections against indexes
	bool removeItem(int itemToRemove);					// remove item (connections to removed item move to nodes that previously connected to this node)
	bool loopCheck();									// loop search
	void sort();										// sorting nodes according processing flow

private:
	StackInt stack;										// stack for loop search
};
