#include <iostream>
#include <vector>
#include <math.h>
#include <algorithm>  
#include <time.h>
#include <map>
#include <fstream>
#include <string>
#include <list>
#include <stack>
#include <set>
#include <queue>
#include <climits>

using namespace std;

vector<vector<long long int>> graph;
vector<vector<long long int>> rGraph;
map<int, vector<int>> adj;
int cnt;
int pplCnt;
int coopCnt;
int sickCnt;
int toProtectCnt;
map<int, int> group;
map<int, int> groupToProtect;
queue<int> sick;
map<pair<int, int>, int> edges;
set<int> res;
vector<int> sickVec;
int sickNode;
int toProtectNode;

int bfs(map<int, vector<int>> adj, int s, int d, map<int, int>& path, vector<bool>* visited = NULL)
{
	queue <int> q;
	q.push(s);
	if (visited == NULL)
		visited = new vector<bool>(graph.size());
	(*visited)[s] = true;

	while (!q.empty())
	{
		int u = q.front();
		q.pop();

		vector<int> adjancent = adj[u];

		for (auto v : adjancent)
		{
			if (!(*visited)[v] && rGraph[u][v] > 0)
			{
				q.push(v);
				path.insert(make_pair(v, u));
				(*visited)[v] = true;
			}
		}
	}
	bool res = (*visited)[d];
	//delete visited;
	return res;
}


void minCut()
{
	int s = sickNode, t = toProtectNode;
	int u, v;
	rGraph = vector<vector<long long int>>(graph);
	int total = 0;
	map<int, int> path;
	while (bfs(adj, sickNode, toProtectNode, path))
	{
		long long int path_flow = LLONG_MAX;
		for (v = t; v != s; v = path[v])
		{
			u = path[v];
			path_flow = min(path_flow, rGraph[u][v]);
		}

		for (v = t; v != s; v = path[v])
		{
			u = path[v];
			rGraph[u][v] -= path_flow;
			rGraph[v][u] += path_flow;
		}
		path = map<int, int>();
		total += path_flow;
	}

	vector<bool>* visited = new vector<bool>(graph.size());
	bfs(adj, sickNode, toProtectNode, path, visited);

	cout << total << endl;
	set<int> res;
	for (size_t x = 0; x < graph.size(); x++)
		for (size_t y = 0; y < graph.size(); y++)
			if ((*visited)[x] && !(*visited)[y] && graph[x][y] && edges.find(make_pair(y, x)) != edges.end())
				res.insert(edges[make_pair(y, x)]);

	size_t i = 0;
	for (auto a : res)
	{
		cout << a;
		if (res.size() - 1 != i)
			cout << " ";
		else
			cout << endl;
	}
}
void LoadInput(istream& in)
{
	string line;
	if (std::getline(in, line))
	{
		pplCnt = std::stoi(line.substr(0, line.find(' '))) + 2;
		sickNode = pplCnt - 1;
		toProtectNode = pplCnt - 2;
		coopCnt = std::stoi(line.substr(line.find(' ') + 1, line.length()));
	}
	graph = vector<vector<long long int>>();
	for (int i = 0; i < pplCnt; i++)
	{
		vector<long long int> inside = vector<long long int>(pplCnt);
		graph.emplace_back(inside);
	}

	for (int i = 0; i < coopCnt; i++)
	{
		std::getline(in, line);
		int first = std::stoi(line.substr(0, line.find(' ')));
		int second = std::stoi(line.substr(line.find(' ') + 1, line.length()));
		line = line.substr(line.find(' ') + 1, line.length());
		int weight = std::stoi(line.substr(line.find(' ') + 1, line.length()));
		graph[first][second] = weight;
		graph[second][first] = weight;

		edges.insert(make_pair(make_pair(second, first), i));
		edges.insert(make_pair(make_pair(first, second), i));

		if (adj.find(first) != adj.end() || adj.find(second) != adj.end())
		{
			if (adj.find(first) != adj.end())
				adj[first].emplace_back(second);

			if (adj.find(second) != adj.end())
				adj[second].emplace_back(first);
		}


		if (adj.find(first) == adj.end())
			adj.insert(make_pair(first, vector<int> { second }));

		if (adj.find(second) == adj.end())
			adj.insert(make_pair(second, vector<int> { first }));
	}
	std::getline(in, line);
	sickCnt = std::stoi(line.substr(0, line.length()));
	std::getline(in, line);
	for (int i = 0; i < sickCnt; i++)
	{
		int id = std::stoi(line.substr(0, line.length()));
		group.insert(make_pair(id, 0));
		sick.push(id);
		sickVec.emplace_back(id);

		graph[id][sickNode] = LLONG_MAX;
		graph[sickNode][id] = LLONG_MAX;

		int first = id;
		int second = sickNode;

		if (adj.find(first) != adj.end() || adj.find(second) != adj.end())
		{
			if (adj.find(first) != adj.end())
				adj[first].emplace_back(second);

			if (adj.find(second) != adj.end())
				adj[second].emplace_back(first);
		}


		if (adj.find(first) == adj.end())
			adj.insert(make_pair(first, vector<int> { second }));

		if (adj.find(second) == adj.end())
			adj.insert(make_pair(second, vector<int> { first }));

		line = line.substr(line.find(' ') + 1, line.length());
	}

	std::getline(in, line);
	toProtectCnt = std::stoi(line.substr(0, line.length()));
	std::getline(in, line);
	for (int i = 0; i < toProtectCnt; i++)
	{
		int id = std::stoi(line.substr(0, line.length()));
		group.insert(make_pair(id, 1));
		graph[id][toProtectNode] = LLONG_MAX;
		graph[toProtectNode][id] = LLONG_MAX;



		int first = id;
		int second = toProtectNode;

		if (adj.find(first) != adj.end() || adj.find(second) != adj.end())
		{
			if (adj.find(first) != adj.end())
				adj[first].emplace_back(second);

			if (adj.find(second) != adj.end())
				adj[second].emplace_back(first);
		}


		if (adj.find(first) == adj.end())
			adj.insert(make_pair(first, vector<int> { second }));

		if (adj.find(second) == adj.end())
			adj.insert(make_pair(second, vector<int> { first }));

		groupToProtect.insert(make_pair(id, 1));
		line = line.substr(line.find(' ') + 1, line.length());
	}
}
int main()
{
	LoadInput(cin);
	minCut();

	return 0;
}
