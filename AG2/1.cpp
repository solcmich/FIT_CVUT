
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
using namespace std;
struct Point
{
	int old;
	int curr;
};
class Graph
{
private:
	Graph* GetConnectedComponent(vector<bool>& visited, int initial)
	{
		Graph* g = new Graph();
		g->m_points = 0;
		vector<vector<int>> adjancencyMatrixCp = vector<vector<int>>(m_points);

		queue<int> q = queue<int>();

		q.push(initial);
		visited[initial] = true;
		while (!q.empty())
		{
			int curr = q.front();
			q.pop();
			g->m_points++;
			for (auto a : (adjancencyMatrix)[curr])
			{
				(adjancencyMatrixCp)[curr].emplace_back(a);
				(adjancencyMatrixCp)[a].emplace_back(curr);

				if (visited[a])
					continue;
				visited[a] = true;
				q.push(a);
			}

		}
		g->adjancencyMatrix = adjancencyMatrixCp;

		return g;
	}

	void BiconnectedComponentsRec(int ** lows, int**  ins, const int & next,
		stack<pair<int, int>> * edges, int ** parents, vector<Graph*> * comps)
	{
		static int time = 0;

		(*lows)[next] = (*ins)[next] = ++time;
		int childs = 0;

		for(auto a : (adjancencyMatrix)[next]) {
			
			if ((*ins)[a] == -1) {
				(*parents)[a] = next;
				(*edges).push(make_pair(next, a));
				childs++;
				BiconnectedComponentsRec(lows, ins, a, edges, parents, comps);

				(*lows)[next] = min((*lows)[next], (*lows)[a]); //Propagate after recusrion

				if (((*ins)[next] == 1 && childs > 1) || ((*ins)[next] > 1 && (*lows)[a] >= (*ins)[next])) { //Next is articulation, flush points from stack as one biconected comp
					Graph* g = new Graph();
					g->initial = a;
					g->m_points = 1;
	
					while ((*edges).top().first != next || (*edges).top().second != a) { //Iter till comp ends.
						int first = (*edges).top().first;
						int second = (*edges).top().second;
						g->verts.insert(first);
						g->verts.insert(second);
						g->edges.emplace_back((*edges).top());
						(*edges).pop();
					}
					int first = (*edges).top().first;
					int second = (*edges).top().second;
					 g->verts.insert(first);
					 g->verts.insert(second);
					 g->edges.emplace_back((*edges).top());
					 (*edges).pop();
					(*comps).emplace_back(g);
				}
			}
 
			else if (a != (*parents)[next]) {
				(*lows)[next] = min((*lows)[next], (*ins)[a]);
				if ((*ins)[a] < (*ins)[next]) {
					(*edges).emplace(make_pair(next, a));
				}
			}
		}
		if ((adjancencyMatrix)[next].size() == 0)
		{
			Graph* foreverAlone = new Graph();
			foreverAlone->initial = next;
			//foreverAlone->adjancencyMatrix = vector<vector<int>>(m_points);
			foreverAlone->m_points = 1;
			(*comps).emplace_back(foreverAlone);
			foreverAlone->verts.insert(next);
			//free(foreverAlone);
		}
	}
public:
	vector<Graph*> GetBiconnectedComponents() {
		int* lows = (int*)malloc(m_points * sizeof(*lows));
		int* ins = (int*)malloc(m_points * sizeof(*ins));
		int* parents = (int*)malloc(m_points * sizeof(*parents));
		fill_n(lows, m_points, -1);
		fill_n(ins, m_points, -1);
		fill_n(parents, m_points, -1);
		vector<Graph*> * g = new vector<Graph*>();
		stack<pair<int, int>> * st = new stack<pair<int, int>>();
		for (int i = 0; i < m_points; i++)
		{
			if (ins[i] == -1)
				BiconnectedComponentsRec(&lows, &ins, i, st, &parents, g);
		}

		bool left = false;
		
		Graph* gr = new Graph();
		gr->m_points = 1;
		gr->adjancencyMatrix = vector<vector<int>>(m_points);
		while ((*st).size() > 0) {
			left = true;
			gr->initial = (*st).top().first;
			gr->verts.insert(gr->initial);
			gr->verts.insert((*st).top().second);
			gr->edges.emplace_back((*st).top());
			(*st).pop();
		}
		if (left) 
			(*g).push_back(gr);


		sort((*g).begin(), (*g).end(), [](Graph* a, Graph* b) {return a->verts.size() > b->verts.size(); }); //Sort
		return (*g);
	}
	void Print(int originalPointsCnt)
	{
		cout << verts.size() << endl;
		for (auto a : verts)
			cout << a << " ";
		cout << endl;
	}
	bool cmp(Graph* a, Graph* b)
	{
		return a->m_points > b->m_points;
	}


	void DoTheMagic(Graph * g)
	{
		g->adjancencyMatrix = vector<vector<int>>(g->verts.size());
		g->adjancencyMatrixMap = map<int, int>();

		int i = 0;
		for (auto a : g->verts)
			g->adjancencyMatrixMap.insert(make_pair(a, i++));

		g->initial = 0;

		for (auto a : g->edges)
		{
			int k = g->adjancencyMatrixMap.find(a.first)->second;
			int l = g->adjancencyMatrixMap.find(a.second)->second;
			g->adjancencyMatrix[l].emplace_back(k);
			g->adjancencyMatrix[k].emplace_back(l);
		}	
	}

	bool isBipartite(Graph* g)
	{
		
		DoTheMagic(g);
		if (g->verts.size() == 1)
			return true;
		//use colors
		vector<int> colors = vector<int>(g->verts.size());
		for (size_t i = 0; i < g->verts.size(); i++)
			colors[i] = -1;

		colors[g->initial] = 1;
		queue<int> q;
		q.push(g->initial);
		vector<bool> visited = vector<bool>(g->verts.size());
		while (!q.empty())
		{
			int curr = q.front();
			q.pop();

			for (auto a : (g->adjancencyMatrix)[curr])
			{
				if (colors[a] == -1)
				{
					colors[a] = 1 - colors[curr];
					q.push(a);
				}

				else if (colors[a] == colors[curr])
					return false;
			}
		}
		return true;
	}

	Graph* GetBiggestBipartite(vector<Graph*> toCheck) {
		for (auto a : toCheck)
		{
			if (isBipartite(a))
				return a;
		}
		return NULL;
	}

	void LoadInput(istream& in)
	{
		string line;
		if (std::getline(in, line))
		{
			m_points = std::stoi(line.substr(0, line.find(' ')));
			m_edges = std::stoi(line.substr(line.find(' ') + 1, line.length()));
		}

		adjancencyMatrix = vector<vector<int>>(m_points);
		m_deg = vector<int>(m_points);
		for (int i = 0; i < m_edges; i++)
		{
			std::getline(in, line);
			int first = std::stoi(line.substr(0, line.find(' ')));
			int second = std::stoi(line.substr(line.find(' ') + 1, line.length()));
			(adjancencyMatrix)[first].emplace_back(second);
			(adjancencyMatrix)[second].emplace_back(first);
			m_deg[first]++;
			m_deg[second]++;

		}
	}
	int initial = 0;
	vector<vector<int>> adjancencyMatrix;
	map<int, int> adjancencyMatrixMap;
	int m_edges = 0;
	int m_points = 0;
	vector<int> m_deg;
	set<int> verts;
	vector<pair<int, int>> edges;
};


int main()
{
	Graph* g = new Graph();
	g->LoadInput(cin);
	vector<Graph*> biconnectedComponents = g->GetBiconnectedComponents();
	Graph* res = g->GetBiggestBipartite(biconnectedComponents);
	if (res == NULL)
	{
		cout << 0;
		return 0;
	}
	res->Print(g->m_points);
	return 0;
}

