#include <iostream>
#include <list>
#include <vector>
#include <fstream>
#include <string>
#include <sstream>
#include <map>

struct vertex;
struct edge;

using vertex_label = int;
using vertices = std::list<vertex>;
using edges = std::list<edge>;
using labels = std::map<vertex_label, vertices::iterator>;

struct vertex
{
	explicit vertex(vertex_label label) : label{label} {}

	bool explored = false;
	unsigned time = 0;
	vertex_label label = 0;
	std::vector<edges::iterator> edge_ptrs; // outgoing edges
};

struct edge
{
	vertices::iterator head;
	vertices::iterator tail;
};

void dfs(vertices& n, edges& m, vertices::iterator start, bool inorder)
{
}

int main(int argc, char** argv)
{
	if (argc != 2)
	{
		std::cerr << "not enough args" << std::endl;
		return 1;
	}

	std::fstream f{argv[1]};
	if (!f)
	{
		std::cerr << "could not open data file" << std::endl;
		return 2;
	}

	vertices n;
	edges m;
	labels l;
	std::string line;
	while (std::getline(f, line))
	{
		int head = 0;
		int tail = 0;
		
		std::istringstream iss{line};
		iss >> tail;
		iss >> head;

		vertices::iterator head_it;
		auto label_it = l.find(head);
		if (l.end() != label_it)
		{
			head_it = label_it->second;
		}
		else
		{
			head_it = n.insert(n.end(), vertex{head});
			l[head] = head_it;
		}

		vertices::iterator tail_it;
		label_it = l.find(tail);
		if (l.end() != label_it)
		{
			tail_it = label_it->second;
		}
		else
		{
			tail_it = n.insert(n.end(), vertex{head});
			l[head] = tail_it;
		}

		auto edge_it = m.insert(m.end(), {head_it, tail_it});
		tail_it->edge_ptrs.push_back(edge_it);	
	}

	std::cout << n.front().label << std::endl;
	for (const auto& e : n.front().edge_ptrs)
	{
		std::cout << e->tail->label << " <-> " << e->head->label << std::endl;
	}

	f.close();
}
