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
//	explicit vertex(vertex_label label) : label{label} {}

	bool explored = false;
	std::vector<edges::iterator> edges_out;
	std::vector<edges::iterator> edges_in;
};

struct edge
{
	vertices::iterator head;
	vertices::iterator tail;
};

labels timed;
unsigned g_time = 0;
std::vector<unsigned> sizes;

void dfs_unorder(vertices::iterator start)
{
  for (const auto& e : start->edges_in)
  {
    if (!e->tail->explored)
    {
      e->tail->explored = true;
      dfs_unorder(e->tail);
    }  
  }
  timed[++g_time] = start;
}

void dfs_loop(labels& l)
{
  for (auto it = l.rbegin(); it != l.rend(); ++it)
  {
    if (!it->second->explored)
    {
      it->second->explored = true;
      dfs_unorder(it->second);
    }
  }
}

unsigned dfs_inorder(vertices::iterator start)
{
  unsigned result = 0;
  for (const auto& e : start->edges_out)
  {
    if (e->head->explored)
    {
      e->head->explored = false;
      result += dfs_inorder(e->head) + 1;
    }
  }
  return result;
}

void dfs_loop2(labels& l)
{
  for (auto it = l.rbegin(); it != l.rend(); ++it)
  {
    unsigned size = 0;
    if (it->second->explored)
    {
      it->second->explored = false;
      size = dfs_inorder(it->second) + 1;
    }
    sizes.push_back(size);
  }
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
			head_it = n.insert(n.end(), vertex{});
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
			tail_it = n.insert(n.end(), vertex{});
			l[tail] = tail_it;
		}

		auto edge_it = m.insert(m.end(), {head_it, tail_it});
		tail_it->edges_out.push_back(edge_it);
    head_it->edges_in.push_back(edge_it);
	}

	f.close();

//  dfs_loop(l);
//  dfs_loop2(timed);
//
//  std::sort(sizes.begin(), sizes.end(), std::greater<unsigned>{});
//
//  for (size_t i = 0; i < sizes.size() && i < 5; ++i)
//  {
//    std::cout << "size: " << sizes[i] << std::endl;
//  }

//	std::cout << n.front().label << std::endl;
//	for (const auto& e : n.front().edges_out)
//	{
//		std::cout << e->tail->label << " <-> " << e->head->label << std::endl;
//	}
//
//	for (const auto& e : n.front().edges_in)
//	{
//		std::cout << e->tail->label << " <-> " << e->head->label << std::endl;
//	}
//
//  std::cout << "count: " << l.size() << std::endl;
//
//  std::cout << "last: " << (--l.end())->first << std::endl;
//	for (const auto& e : (--l.end())->second->edges_out)
//	{
//		std::cout << e->tail->label << " <-> " << e->head->label << std::endl;
//	}
}
