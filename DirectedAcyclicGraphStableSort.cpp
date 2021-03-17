#include <iostream>
#include <algorithm>
#include "Graph.h"

// useful usings
using int8 = int8_t;
using int16 = int16_t;
using int32 = int32_t;
using int64 = int64_t;
using uint8 = uint8_t;
using uint16 = uint16_t;
using uint32 = uint32_t;
using uint64 = uint64_t;

constexpr int maxRandomizeIterations{ 300 };
constexpr int maxNodes{ 200 };

void randomizeGraphNodes(Graph& graph)
{
	for (int i = 0; i < maxRandomizeIterations; ++i)
	{
		auto idx1 = static_cast<int>((static_cast<double>(std::rand()) / static_cast<double>(RAND_MAX)) * (graph.items.size() - 1)) + 1;
		auto idx2 = static_cast<int>((static_cast<double>(std::rand()) / static_cast<double>(RAND_MAX)) * (graph.items.size() - 1)) + 1;
		std::swap(graph.items[idx1], graph.items[idx2]);									// randomly swap nodes
	}
}

void equalityUnitTest(Graph& graph)
{
	static int testNum{ 1 };
	graph.buildIndexes();																	// build indexes before randomization
	graph.buildOldIndexes();																// build "old" indexes for solidity check
	auto graphReference = graph;															// copy graph to reference
	randomizeGraphNodes(graph);																// randomize nodes
	graph.rebuildConnections();																// reconnect to initial indexes after randomize

	// compare graphs through nodes connections and their "old" indexes
	bool isEqualBeforeSort = std::equal(graph.items.begin(), graph.items.end(), graphReference.items.begin(), graphReference.items.end());
	graph.sort();																			// sort graphs
	bool isEqualAfterSort = std::equal(graph.items.begin(), graph.items.end(), graphReference.items.begin(), graphReference.items.end());

	std::cout << "Unit test #" << testNum << std::endl;
	std::cout << std::boolalpha;
	std::cout << "Graphs eqaulity before sorting: " << isEqualBeforeSort << std::endl;
	std::cout << "Graphs eqaulity after sorting: " << isEqualAfterSort << std::endl << std::endl;

	graph.items.clear();
	testNum++;
}

int main()
{
	std::srand(static_cast<uint32>(std::time(nullptr)));
	Graph graph;

	//////////////////////////////////////////////////////////////////////////
	// Unit test 1: simple chain
	//////////////////////////////////////////////////////////////////////////

	graph.items.push_back({ { BadIndex } });												// first node is not connected
	for (int i = 0; i < maxNodes - 1; ++i) { graph.items.push_back({ { i } }); }			// simple chain, connect to previous node
	equalityUnitTest(graph);

	//////////////////////////////////////////////////////////////////////////
	// Unit test 2: connect nodes to all previous nodes
	//////////////////////////////////////////////////////////////////////////

	graph.items.push_back({ { BadIndex } });												// first node is not connected

	for (int i = 0; i < maxNodes - 1; ++i)
	{
		Item item;
		for (int j = 0; j < i + 1; ++j) { item.prevNode.push_back(j); }
		graph.items.push_back(item);
	}

	equalityUnitTest(graph);
}