#pragma once
#include <vector>

// useful usings
template<typename T>
using Vector = std::vector<T>;									
using VectorInt = Vector<int>;
enum class NodeVisited { NotVisited, Working, Visited };		// node visited enum
constexpr auto BadIndex{ -1 };									// bad index of node

struct Item
{
	VectorInt prevNode{};										// connections to previous nodes
	VectorInt nextNode{};										// connections to next nodes
	NodeVisited visited{ NodeVisited::NotVisited };				// node vizited flag
	int index{ BadIndex };										// node index
	int indexOld{ BadIndex };									// node old index
};
