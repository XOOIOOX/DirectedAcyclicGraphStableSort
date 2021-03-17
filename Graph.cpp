#include "Graph.h"

void Graph::buildNextNodes()
{
	for (auto& i : items) { i.nextNode.clear(); }															// cleanup connections to next nodes
	for (size_t i = 0; i < items.size(); i++)
	{
		for (size_t j = 0; j < items[i].prevNode.size(); j++)
		{
			if (items[i].prevNode[j] != BadIndex && items[i].prevNode[j] != i)
			{
				items[items[i].prevNode[j]].nextNode.push_back(static_cast<int>(i));
			}
		}
	}
}

void Graph::buildPrevNodes()
{
	for (auto& i : items) { i.prevNode.clear(); }															// cleanup connections to previous nodes
	for (size_t i = 0; i < items.size(); i++)
	{
		for (size_t j = 0; j < items[i].nextNode.size(); j++)
		{
			if (items[i].nextNode[j] != BadIndex && items[i].nextNode[j] != i)
			{
				items[items[i].nextNode[j]].prevNode.push_back(static_cast<int>(i));
			}
		}
	}
}

void Graph::buildIndexes()
{
	for (size_t i = 0; i < items.size(); i++) { items[i].index = static_cast<int>(i); }
}

void Graph::buildOldIndexes()
{
	for (size_t i = 0; i < items.size(); i++) { items[i].indexOld = static_cast<int>(i); }
}

int Graph::findOldIndex(int index)
{
	if (index != BadIndex && index < items.size())
	{
		auto it = std::find_if(items.begin(), items.end(), [&index](auto item) { return item.indexOld == index; });
		if (it != items.end()) { return static_cast<int>(std::distance(items.begin(), it)); }
	}

	return BadIndex;
}

void Graph::rebuildConnections()
{
	for (size_t i = 0; i < items.size(); i++)
	{
		for (size_t j = 0; j < items[i].prevNode.size(); j++)
		{
			auto prev = items[i].prevNode[j];

			if (prev != BadIndex)
			{
				auto it = std::find_if(items.begin(), items.end(), [&prev](auto item) { return item.index == prev; });
				if (it != items.end()) { items[i].prevNode[j] = static_cast<int>(std::distance(items.begin(), it)); }
				else { items[i].prevNode[j] = BadIndex; }
			}
		}
	}

	buildIndexes();
}

bool Graph::removeItem(int itemToRemove)
{
	buildIndexes();

	if (itemToRemove != BadIndex && itemToRemove < items.size() - 1 && !items.empty() && itemToRemove > 0)
	{
		sort();
		buildNextNodes();

		for (auto& i : items[itemToRemove].nextNode)
		{
			for (auto& j : items[i].prevNode)
			{
				if (j = itemToRemove) { j = itemToRemove - 1; }
			}
		}

		items.erase(items.begin() + itemToRemove);
		rebuildConnections();
		return true;
	}
	else { return false; }
}

bool Graph::loopCheck()
{
	buildIndexes();
	buildNextNodes();

	stack = {};
	for (auto& i : items) { i.visited = NodeVisited::NotVisited; }
	for (size_t i = 0; i < items.size(); i++)
	{
		stack.push(static_cast<int>(i));

		while (!stack.empty())
		{
			auto idxCurrent = stack.top();
			if (idxCurrent < items.size() && idxCurrent >= 0)
			{
				if (items[idxCurrent].visited == NodeVisited::NotVisited)
				{
					items[idxCurrent].visited = NodeVisited::Visited;

					for (int j = 0; j < items[idxCurrent].nextNode.size(); j++)
					{
						auto idxNext = items[idxCurrent].nextNode[j];

						if (idxNext != BadIndex)
						{
							if (items[idxNext].visited == NodeVisited::Visited) { return true; }		// graph contain loops
							stack.push(idxNext);
						}
					}
				}
				else if (items[idxCurrent].visited == NodeVisited::Visited) { items[idxCurrent].visited = NodeVisited::Working; stack.pop(); }
				else { stack.pop(); }
			}
		}
	}

	return false;																						// graph not contain loops
}

void Graph::sort()
{
	buildIndexes();

	if (!loopCheck())
	{																									// sort only if graph not contain loops
		for (auto& i : items) { i.visited = NodeVisited::NotVisited; }

		int i{ 0 };

		while (i < items.size())
		{
			auto idxPrev{ BadIndex };
			items[i].visited = NodeVisited::Visited;

			for (size_t j = 0; j < items[i].prevNode.size(); j++)
			{
				if (items[i].prevNode[j] != i && items[i].prevNode[j] != BadIndex)
				{
					idxPrev = items[i].prevNode[j];

					if (items[idxPrev].visited == NodeVisited::NotVisited)
					{
						items[i].visited = NodeVisited::Working;
						break;
					}
				}
			}

			if (items[i].visited == NodeVisited::Working)
			{
				items[i].visited = NodeVisited::NotVisited;
				std::swap(items[i], items[idxPrev]);
				rebuildConnections();
			}
			else { ++i; }
		}

		rebuildConnections();
	}
}